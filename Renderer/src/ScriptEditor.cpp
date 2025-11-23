#include "ScriptEditor.hpp"
#include <imgui.h>
#include <spdlog/spdlog.h>
#include <cstring>

// Resize callback for ImGui text input
static int InputTextCallback(ImGuiInputTextCallbackData* data) {
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
        // Resize the vector to fit the new text
        std::vector<char>* buffer = (std::vector<char>*)data->UserData;
        buffer->resize(data->BufTextLen + 1); // +1 for null terminator
        data->Buf = buffer->data();
    }
    return 0;
}

void ScriptEditor::set(Script* sc) {
    scriptManager = sc;
    if (scriptManager && !scriptManager->scripts.empty()) {
        currentScript = scriptManager->scripts.begin()->first;
        loadCurrentScript();
    }
}

void ScriptEditor::loadCurrentScript() {
    if (scriptManager && !currentScript.empty()) {
        auto it = scriptManager->scripts.find(currentScript);
        if (it != scriptManager->scripts.end()) {
            const auto& code = it->second.code;
            // Resize buffer to fit code + null terminator
            editableBuffer.resize(code.size() + 1);
            std::memcpy(editableBuffer.data(), code.c_str(), code.size() + 1);
        }
        else {
            // Script not found, clear buffer
            editableBuffer.resize(1);
            editableBuffer[0] = '\0';
        }
    }
}

void ScriptEditor::draw(entt::registry& registry) {
    static char newScriptName[128] = "";
    static char loadScriptName[128] = "";
    static uint32_t entityID = 0;

    if (!scriptManager) {
        ImGui::Text("No script manager set!");
        return;
    }

    ImGui::Begin("Lua Script Editor");

    // --- Create new script ---
    ImGui::InputText("New Script Name", newScriptName, sizeof(newScriptName));
    ImGui::SameLine();
    if (ImGui::Button("Create")) {
        std::string nameStr = newScriptName;
        if (!nameStr.empty() && scriptManager->scripts.find(nameStr) == scriptManager->scripts.end()) {
            scriptManager->addScript(nameStr, "-- New script\n");
            currentScript = nameStr;
            loadCurrentScript();
            newScriptName[0] = '\0';
        }
    }

    ImGui::Separator();

    // --- Load existing script by filename ---
    ImGui::InputText("Load Script (no .lua)", loadScriptName, sizeof(loadScriptName));
    ImGui::SameLine();
    if (ImGui::Button("Load")) {
        std::string filename(loadScriptName);
        if (!filename.empty()) {
            std::string path = "scripts/" + filename + ".lua";
            if (scriptManager->loadScriptFile(path, filename)) {
                currentScript = filename;
                loadCurrentScript();
                spdlog::info("Loaded script '{}'", filename);
            }
            else {
                spdlog::error("Failed to load script '{}'", filename);
            }
        }
    }

    ImGui::Separator();

    // --- Script selection combo ---
    if (scriptManager->scripts.empty()) {
        ImGui::Text("No scripts available");
    }
    else {
        if (ImGui::BeginCombo("Scripts", currentScript.c_str())) {
            for (const auto& [name, _] : scriptManager->scripts) {
                bool selected = (name == currentScript);
                if (ImGui::Selectable(name.c_str(), selected)) {
                    currentScript = name;
                    loadCurrentScript();
                }
                if (selected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    // --- Script editor ---
    if (!currentScript.empty()) {
        ImGui::Text("Edit Lua script: %s", currentScript.c_str());

        // Ensure we have a valid buffer
        if (editableBuffer.empty()) {
            editableBuffer.resize(1);
            editableBuffer[0] = '\0';
        }

        // Use InputTextMultiline with callback for resizing
        ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_CallbackResize;
        if (ImGui::InputTextMultiline("##script", editableBuffer.data(), editableBuffer.size(),
            ImVec2(-FLT_MIN, 300), flags, InputTextCallback, (void*)&editableBuffer)) {
            // Text was modified - the callback handles resizing automatically
        }

        // --- Run / Save ---
        if (ImGui::Button("Run Script")) {
            std::string codeStr = editableBuffer.data();
            scriptManager->addScript(currentScript, codeStr);
            spdlog::info("Ran script '{}'", currentScript);
        }
        ImGui::SameLine();
        if (ImGui::Button("Save Script")) {
            std::string codeStr = editableBuffer.data();
            scriptManager->addScript(currentScript, codeStr);
            std::string path = "scripts/" + currentScript + ".lua";
            if (scriptManager->saveScriptFile(path, currentScript)) {
                spdlog::info("Saved script '{}' to {}", currentScript, path);
            }
            else {
                spdlog::error("Failed to save script '{}'", currentScript);
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Remove Script")) {
            scriptManager->removeScript(currentScript);
            if (!scriptManager->scripts.empty()) {
                currentScript = scriptManager->scripts.begin()->first;
                loadCurrentScript();
            }
            else {
                currentScript.clear();
                editableBuffer.clear();
            }
        }
    }

    ImGui::Separator();

    // --- Bind transforms (rebind) ---
    if (ImGui::Button("Rebind Transforms")) {
        scriptManager->bindTransforms(registry);
        spdlog::info("Rebound transforms to Lua");
    }

    // --- Attach script to entity ---
    ImGui::InputScalar("Entity ID", ImGuiDataType_U32, &entityID);
    ImGui::SameLine();
    if (ImGui::Button("Attach to Entity") && entityID != 0 && !currentScript.empty()) {
        entt::entity e = static_cast<entt::entity>(entityID);
        if (registry.valid(e)) {
            scriptManager->attachScript(currentScript, e);
            spdlog::info("Attached script '{}' to entity {}", currentScript, entityID);
        }
        else {
            spdlog::error("Entity {} is not valid", entityID);
        }
    }

    ImGui::End();
}

void ScriptEditor::drawDebugPanel(entt::registry& registry) {
    if (!scriptManager) return;

    ImGui::Begin("Script Debug");

    ImGui::Text("Loaded Scripts: %zu", scriptManager->scripts.size());
    for (auto& [name, instance] : scriptManager->scripts) {
        if (ImGui::TreeNode(name.c_str())) {
            ImGui::Text("Callbacks:");
            ImGui::Indent();
            ImGui::Text("onInit: %s", instance.onInit.valid() ? "Yes" : "No");
            ImGui::Text("onUpdate: %s", instance.onUpdate.valid() ? "Yes" : "No");
            ImGui::Text("onDestroy: %s", instance.onDestroy.valid() ? "Yes" : "No");
            ImGui::Unindent();

            // Show attached entities
            ImGui::Text("Attached to %zu entities:",
                std::count_if(scriptManager->objectScripts.begin(), scriptManager->objectScripts.end(),
                    [&](const auto& pair) {
                        return std::find(pair.second.begin(), pair.second.end(), name) != pair.second.end();
                    }));

            ImGui::Indent();
            for (auto& [entity, scriptNames] : scriptManager->objectScripts) {
                if (std::find(scriptNames.begin(), scriptNames.end(), name) != scriptNames.end()) {
                    ImGui::Text("Entity ID: %u", static_cast<uint32_t>(entity));

                    // Show entity's Transform if it exists
                    if (registry.all_of<Transform>(entity)) {
                        auto& t = registry.get<Transform>(entity);
                        ImGui::Text("  Pos: %.2f, %.2f, %.2f", t.position.x, t.position.y, t.position.z);
                        ImGui::Text("  Rot: %.2f, %.2f, %.2f", t.rotation.x, t.rotation.y, t.rotation.z);
                        ImGui::Text("  Scale: %.2f, %.2f, %.2f", t.scale.x, t.scale.y, t.scale.z);
                    }
                }
            }
            ImGui::Unindent();

            ImGui::TreePop();
        }
    }

    ImGui::End();
}