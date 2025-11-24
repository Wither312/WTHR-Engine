#pragma once
#include "scripts.hpp"
#include <string>
#include <vector>
#include <entt/entt.hpp>

struct ScriptEditor {
    Script* scriptManager = nullptr;
    std::string currentScript;
    std::vector<char> editableBuffer; // mutable buffer for ImGui

    ScriptEditor() = default;
    ScriptEditor(Script* manager) : scriptManager(manager) {}

    void draw(entt::registry&);
    void set(Script* manager);

    void drawDebugPanel(entt::registry& registry);
private:
    void loadCurrentScript(); // helper to copy std::string to editableBuffer
};
