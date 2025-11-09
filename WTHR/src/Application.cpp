#include "pch.hpp"      // precompiled header
#include "Application.hpp"
#include <PrimitiveShape.hpp>
#include <filesystem>
#include <Components.hpp>


Application::Application(int width, int height, const char* title)
	: m_Width(width), m_Height(height), m_Title(title)
{
}

Application::~Application()
{
	Shutdown();
}
bool Application::isFocused = true;

bool Application::Init()
{
	m_WindowManager.Init();

	InitImGui();
	InitEngineSystems();
	m_Renderer.Init();

	m_Input.setWindow(m_WindowManager.GetWindow());
	if (m_WindowManager.GetWindow() == nullptr) __debugbreak();
	return true;
}
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <thread>
#include <atomic>

bool CheckSharedContextThreads(GLFWwindow* contextA, GLFWwindow* contextB, std::thread& threadB)
{
	if (!contextA || !contextB)
	{
		spdlog::error("Invalid GLFW window pointer(s) provided.");
		return false;
	}

	std::atomic<bool> result{ false };
	std::atomic<bool> done{ false };

	// Launch a thread to test contextA against contextB
	std::thread testThread([&]() {
		// Make contextA current on this thread
		glfwMakeContextCurrent(contextA);

		// Try to query some OpenGL state that should exist if contextB is shared
		GLint maxTexUnits = 0;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTexUnits);

		// If we got a valid value (>0), assume contexts are shared
		result = (maxTexUnits > 0);

		done = true;
		});

	// Wait for the test to finish (timeout optional)
	while (!done)
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

	testThread.join();

	return result;
}


static auto lastFrame = std::chrono::high_resolution_clock::now();
void Application::Run()
{
	if (!m_WindowManager.isOpen())
		return;

	spdlog::info("Running WTHR Application...");

	glm::vec3 triangleColor(1, 0, 0);
	ImGuiIO& io = ImGui::GetIO();


	Scene scene(m_WindowManager.GetWindow());

	GLFWwindow*  workerWindow = glfwCreateWindow(1, 1, "GPU Worker", nullptr, m_WindowManager.GetWindow());


	GLFWwindow* contextA = m_WindowManager.GetWindow();
	//if (CheckSharedContextThreads(contextA, workerWindow,scene.gpuWorker.GetThread()))
	//{
	//	spdlog::info("Contextes are shared");
	//}
	//else
	//{
	//	spdlog::error("Contextes are NOT shared");

	//}
	//std::shared_ptr<Shapes::Cube> ptrCube = std::make_shared<Shapes::Cube>();
	//std::shared_ptr<Shapes::Pyramid>  ptrPyramid = std::make_shared<Shapes::Pyramid>();

	//Texture texture("texture.png","texture_diffuse");
	//ptrCube.get()->mesh.textures.push_back(texture);
	//ptrPyramid.get()->mesh.textures.push_back(texture);

	auto ptrShdr = std::make_shared<Shader>("shaders/default.vert", "shaders/default.frag");



	//scene.AddMesh(ptrCube);
	//scene.AddMesh(ptrPyramid);
	//scene.AddShader(ptrShdr);

	////TODO MODEL LOADING

	auto ent = scene.CreateCube();

	auto& transform = scene.GetRegistry().get<Transform>(ent);
	auto& mesh = scene.GetRegistry().get<MeshComponent>(ent);
	Texture texture("texture.png", "texture_diffuse");
	mesh.mesh->mesh.textures.push_back(texture);


	auto& registry = scene.GetRegistry();

	while (m_WindowManager.isOpen())
	{
		auto currentFrame = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration<float>(currentFrame - lastFrame).count();
		lastFrame = currentFrame;

		io.DeltaTime = deltaTime;


		m_WindowManager.PollEvents();
		m_Input.Update();
		m_WindowManager.BeginFrame();




		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0)); // transparent background



		// Start full-screen dockspace host window
		ImGui::Begin("DockSpace Window", nullptr, window_flags);
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);

		// Create the actual DockSpace
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);






		if (glfwGetKey(m_WindowManager.GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
			scene.GetCamera().ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(m_WindowManager.GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
			scene.GetCamera().ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(m_WindowManager.GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
			scene.GetCamera().ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(m_WindowManager.GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
			scene.GetCamera().ProcessKeyboard(RIGHT, deltaTime);

		float rotationSpeed = 50.0f; // degrees per second
		double time = glfwGetTime();

		transform.rotation.y = fmod(time * 30.0, 360.0); // rotate Y



		ImGui::Begin("Cube transformation");
		ImGui::SliderFloat3("Position:", &transform.position.x, -10, 10);
		ImGui::SliderFloat3("rotation:", &transform.rotation.x, -360, 360);
		ImGui::End();
		ImGui::Begin("ECS");
		if (ImGui::Button("+ Entity"))
		{
			scene.CreateCube();
		}

		ImGui::SameLine(); // Put next button on the same row

		if (ImGui::Button("- Entity"))
		{
			spdlog::trace("No entity selected");
		}

		ImGui::Separator();

		registry.view<MeshComponent, Transform>().each([&](auto entity, auto& meshComp, auto& transform) {
			ImGui::Text("Current Components for entity %d", (int)entity); // optional display

			auto& all_Textures = scene.GetTextures();

			ImGui::Text("Textures");


			if (ImGui::Button(("+ Texture##" + std::to_string((int)entity)).c_str()))
			{
				ImGui::OpenPopup(("Set Texture##" + std::to_string((int)entity)).c_str());
			}
			if (ImGui::BeginPopupModal(("Set Texture##" + std::to_string((int)entity)).c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				static char filepath[256] = "";
				ImGui::InputText("File Path", filepath, IM_ARRAYSIZE(filepath));

				if (ImGui::Button("Load"))
				{
					std::string pathStr = filepath;

					// Check if texture exists in unordered_map
					auto it = all_Textures.find(pathStr);
					if (it != all_Textures.end()) // exists
					{
						meshComp.mesh->mesh.textures.push_back(it->second); // push the Texture object
						ImGui::CloseCurrentPopup();
					}
					else
					{
						// Optionally handle missing texture, e.g., load it or show error
						std::cout << "Texture not found in all_Textures: " << pathStr << std::endl;
					}
				}

				ImGui::SameLine();
				if (ImGui::Button("Cancel"))
				{
					filepath[0] = '\0';
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button(("- Texture##" + std::to_string((int)entity)).c_str()))
			{
				auto& vecTextures = meshComp.mesh->mesh.textures;
				vecTextures.clear();
			}



			for (auto& texture : meshComp.mesh->mesh.textures)
			{
				// Loop through the map to find the matching Texture
				for (auto& [name, tex] : all_Textures)
				{
					if (tex.id == texture.id) // assuming Texture has a unique 'id' or comparable field
					{
						ImGui::Bullet();
						ImGui::Text("%s", name.c_str());
						break; // stop after finding the first match
					}
				}
			}










			if (registry.any_of<Transform>(entity))
			{
				ImGui::Text("Transforms");
				// Encode entity into ID by using "##" suffix
		// Create unique labels using "##" + entity ID
				std::string posLabel = "Position##" + std::to_string((uint32_t)entity);
				std::string scaleLabel = "Scale##" + std::to_string((uint32_t)entity);
				std::string rotLabel = "Rotation##" + std::to_string((uint32_t)entity);

				// Position
				ImGui::SliderFloat3(posLabel.c_str(), &transform.position.x, -10.0f, 10.0f);

				// Scale
				ImGui::SliderFloat3(scaleLabel.c_str(), &transform.scale.x, -10.0f, 10.0f);

				// Rotation
				ImGui::SliderFloat3(rotLabel.c_str(), &transform.rotation.x, -360.0f, 360.0f);

			}
			ImGui::Text("Color");
			std::string butLabel = "Add Component##" + std::to_string((uint32_t)entity);
			if (ImGui::Button(butLabel.c_str()))
			{
				registry.emplace<Color>(entity, glm::vec4(1.f));
			}


			if (scene.HasComponent<Color>(entity)) {
				std::string colorLLabel = "Color##" + std::to_string((uint32_t)entity);
				auto& color = scene.GetComponent<Color>(entity);
				ImGui::ColorEdit4(colorLLabel.c_str(), &color.value.r); // RGBA
			}
			else
			{
				ImGui::Text("Entity has no color component");
			}

			if (registry.any_of<MeshComponent>(entity))
			{
				ImGui::Text("MeshComponent");
			}
			});
		//MODEL COMPONENTS
		registry.view<ModelComponent, Transform>().each([&](auto entity, auto& modelComp, auto& transform) {
			ImGui::Text("Current Components for entity %d", (int)entity); // optional display


			if (registry.any_of<Transform>(entity))
			{
				ImGui::Text("Transforms");
				// Encode entity into ID by using "##" suffix
		// Create unique labels using "##" + entity ID
				std::string posLabel = "Position##" + std::to_string((uint32_t)entity);
				std::string scaleLabel = "Scale##" + std::to_string((uint32_t)entity);
				std::string rotLabel = "Rotation##" + std::to_string((uint32_t)entity);

				// Position
				ImGui::SliderFloat3(posLabel.c_str(), &transform.position.x, -10.0f, 10.0f);

				// Scale
				ImGui::SliderFloat3(scaleLabel.c_str(), &transform.scale.x, -10.0f, 10.0f);

				// Rotation
				ImGui::SliderFloat3(rotLabel.c_str(), &transform.rotation.x, -360.0f, 360.0f);

			}

			if (registry.any_of<ModelComponent>(entity))
			{
				ImGui::Text("ModelComponent");
			}
			});

		ImGui::End();



		ImGui::Begin("Texture Inspector");

		auto& m_Textures = scene.GetTextures();
		static std::string currentTextureKey;

		for (auto& [name, tex] : m_Textures)
		{
			ImGui::Text("Current Texture: %s", currentTextureKey.c_str());

			// Display thumbnail if you have OpenGL ID
			if (tex.id != 0) // assuming Texture struct has 'id' as GPU handle
			{
				ImVec2 size = ImVec2(128, 128);
				ImGui::Image((void*)(intptr_t)tex.id, size, ImVec2(0, 1), ImVec2(1, 0));
				ImGui::SameLine();
			}
		}



		// Button to open file path popup
		if (ImGui::Button("Add/Load Texture"))
		{
			ImGui::OpenPopup("Load Texture");
		}

		if (ImGui::BeginPopupModal("Load Texture", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			static char filepath[256] = "";
			ImGui::InputText("File Path", filepath, IM_ARRAYSIZE(filepath));

			if (ImGui::Button("Load"))
			{
				std::string pathStr = filepath;
				if (!pathStr.empty())
				{
					// Call your background loader here:
					// Texture tex = LoadTexture(filepath);
					// m_Textures[pathStr] = tex;
					Texture texture(filepath, "texture_diffuse");

					m_Textures.insert({ filepath, texture });

					// Optionally set it as current
					currentTextureKey = pathStr;

					filepath[0] = '\0';
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				filepath[0] = '\0';
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Modal inspector");
		if (ImGui::Button("Load Model"))
		{
			scene.CreateModel("backpack.obj", glm::vec4(1.0f));
		}
		ImGui::End();

		ImGui::Begin("Registry Debug");


		registry.view<Transform, ModelComponent>().each([&](auto entity, Transform& t, ModelComponent& mc) {
			ImGui::Separator();
			ImGui::Text("Entity: %u", static_cast<uint32_t>(entity));
			ImGui::Text("Transform ptr: %p", &t);
			ImGui::Text("Position: %.2f %.2f %.2f", t.position.x, t.position.y, t.position.z);
			ImGui::Text("Scale: %.2f %.2f %.2f", t.scale.x, t.scale.y, t.scale.z);
			ImGui::Text("ModelComponent shared_ptr: %p", mc.model.get());
			ImGui::Text("ModelComponent loaded: %s", mc.model->IsLoaded() ? "true" : "false");
			});

		ImGui::End();

		ImGui::Begin("GPU Worker Debug");

		//auto& glWorker = scene.gpuWorker;
		//std::stringstream ss;
		//ss << glWorker.GetThreadId(); // glWorker is your GLContextWorker instance
		//ImGui::Text("Worker Thread ID: %s", ss.str().c_str());

		//ImGui::Text("Render Queue Size: %zu", glWorker.GetQueueSize());

		//ImGui::Text("Context GPU: %zu", glWorker.GetWorkerWindow());

		ImGui::End();





		ImGui::End(); // end dockspace


		/*	ImGui::Begin("Pyramid transformation");
			ImGui::SliderFloat3("position:", &ptrPyramid.get()->position.x, -10, 10);
			ImGui::SliderFloat3("rotation:", &ptrPyramid.get()->rotation.x, -360, 360);
			ImGui::End();*/


		m_Renderer.Clear();

		m_Renderer.RenderScene(scene, *ptrShdr);

		m_WindowManager.EndFrame();

		m_WindowManager.SwapBuffers();
		static bool firstMouse = false;
		static double lastX;
		static double lastY;
		if (m_Input.IsKeyPressed(GLFW_KEY_ESCAPE))
		{
			isFocused = !isFocused;

			if (isFocused)
			{
				double xpos, ypos;
				glfwGetCursorPos(m_WindowManager.GetWindow(), &xpos, &ypos);
				lastX = static_cast<float>(xpos);
				lastY = static_cast<float>(ypos);
				firstMouse = true; // reset to avoid huge jump
			}
		}

		if (isFocused)
		{
			double xposIn, yposIn;
			glfwGetCursorPos(m_WindowManager.GetWindow(), &xposIn, &yposIn);
			float xpos = static_cast<float>(xposIn);
			float ypos = static_cast<float>(yposIn);

			if (firstMouse)
			{
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
			}

			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

			lastX = xpos;
			lastY = ypos;

			scene.GetCamera().ProcessMouseMovement(xoffset, yoffset);

			glfwSetInputMode(m_WindowManager.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else
		{
			glfwSetInputMode(m_WindowManager.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

	}
}


void Application::Update()
{
	// ECS, input, editor logic
}

void Application::Render()
{
	// Rendering is handled by Renderer library
}

void Application::InitEngineSystems()
{
	spdlog::info("Initializing ECS, asset loaders, physics, etc.");
	// Example: EnTT registry, JSON parser, Assimp importer setup
}

void Application::InitImGui()
{
	spdlog::info("Initializing ImGui...");
	// Setup ImGui context here
	// ImGui::CreateContext();
	// ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	// ImGui_ImplOpenGL3_Init("#version 460");
}

void Application::Shutdown()
{
	spdlog::info("Shutting down WTHR Application...");

	// Cleanup ImGui
	// ImGui_ImplOpenGL3_Shutdown();
	// ImGui_ImplGlfw_Shutdown();
	// ImGui::DestroyContext();


}
