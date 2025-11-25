#pragma once
#include <entt/entt.hpp>
#include <memory>
#include <filesystem>

#include <Scripts.hpp>
#include "Components.hpp"
#include "PrimitiveShape.hpp"
#include "Model.hpp"
#include <GLContextWorker.hpp>
#include "Camera.hpp"

class Scene {
public:
	Scene(GLFWwindow* window) : worker(window)
	{
		spdlog::set_level(spdlog::level::debug);
		spdlog::set_pattern("[%H:%M:%S.%e] [%^%l%$] %v");



	}
	void print_transform(const Transform& t) {
		std::cout << "Position: (" << t.position.x << ", " << t.position.y << ", " << t.position.z << ")\n";
		std::cout << "Rotation: (" << t.rotation.x << ", " << t.rotation.y << ", " << t.rotation.z << ")\n";
		std::cout << "Scale:    (" << t.scale.x << ", " << t.scale.y << ", " << t.scale.z << ")\n";
	}
	entt::registry& GetRegistry() { return m_Registry; }
	Camera& GetCamera() { return m_Camera; }
	std::unordered_map<std::string, Texture>& GetTextures() { return m_Textures; }; // path or name → texture data



	void CreateCubeGrid(int width, int length, int height, glm::vec3 pos = glm::vec3(0.f))
	{

		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < length; j++)
			{
				for (int k = 0; k < height; k++)
				{
					CreateCube(glm::vec3(i * 1.1, j * 1.1, k * 1.1));
				}
			}
		}

	}

	// -------------------------
	// Helper to create a cube
	// -------------------------
	entt::entity CreateCube(const glm::vec3& position = glm::vec3(0.f)) {
		auto entity = m_Registry.create();
		m_Registry.emplace<Transform>(entity, position);
		auto& mesh = m_Registry.emplace<MeshComponent>(entity, std::make_shared<Shapes::Cube>());
		auto it = m_Textures.find("stone.png");
		if (it != m_Textures.end())
		{
			mesh.mesh->mesh.textures.push_back(it->second);
			m_Registry.emplace<Texture>(entity);
		}
		return entity;
	}

	// -------------------------
	// Helper to create a model (async)
	// -------------------------
	entt::entity CreateModel(const std::string& path, const glm::vec3& position = glm::vec3(0.f))
	{
		auto entity = m_Registry.create();

		// Add a Transform component
		m_Registry.emplace<Transform>(entity, position);

		// Create a ThreadSafeModel wrapper
		auto modelWrapper = std::make_shared<ThreadSafeModel>();

		// Add the ModelComponent to the registry
		m_Registry.emplace<ModelComponent>(entity, modelWrapper);

		// Reset wrapper just in case
		modelWrapper->Reset();

		// Queue asynchronous GPU load
		worker.AsyncLoadModel(modelWrapper, path);


		return entity;
	}

	void Save(const std::filesystem::path&);
	void Load(const std::filesystem::path&);


	template<typename T>
	bool HasComponent(entt::entity entity) {
		return m_Registry.any_of<T>(entity);
	}

	template<typename T>
	T& GetComponent(entt::entity entity) {
		return m_Registry.get<T>(entity);
	}

	template<typename T, typename... Args>
	T& AddComponent(entt::entity entity, Args&&... args) {
		return m_Registry.emplace<T>(entity, std::forward<Args>(args)...);
	}
	GLContextWorker worker;
	Script script;
private:
	std::unordered_map<std::string, Texture> m_Textures; // path or name → texture data
	entt::registry m_Registry;
	Camera m_Camera;
};
