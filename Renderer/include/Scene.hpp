#pragma once
#include <entt/entt.hpp>
#include <memory>


#include "Components.hpp"
#include "PrimitiveShape.hpp"
#include "Model.hpp"
#include <GLContextWorker.hpp>
#include "Camera.hpp"

class Scene {
public:
	Scene(GLFWwindow* window) : worker(window)
	{

	}

	entt::registry& GetRegistry() { return m_Registry; }
	Camera& GetCamera() { return m_Camera; }
	std::unordered_map<std::string, Texture>& GetTextures() { return m_Textures; }; // path or name → texture data


	// -------------------------
	// Helper to create a cube
	// -------------------------
	entt::entity CreateCube(const glm::vec3& position = glm::vec3(0.f)) {
		auto entity = m_Registry.create();
		m_Registry.emplace<Transform>(entity, position);
		m_Registry.emplace<MeshComponent>(entity, std::make_shared<Shapes::Cube>());
		return entity;
	}

	// -------------------------
	// Helper to create a model (async)
	// -------------------------
	entt::entity CreateModel(const std::string& path, const glm::vec3& position)
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
private:
	std::unordered_map<std::string, Texture> m_Textures; // path or name → texture data
	entt::registry m_Registry;
	Camera m_Camera;
};
