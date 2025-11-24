#include "Scene.hpp"
#include <spdlog/spdlog.h> // optional for logging
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>
#include <entt/entt.hpp>
#include "Components.hpp" // Transform, MeshComponent, etc.

using json = nlohmann::json;

void Scene::Save(const std::filesystem::path& filepath)
{
	json sceneJson;

	for (auto& [path, texture] : m_Textures)
	{
		json textureJson;
		textureJson["name"] = "NAME:";
		textureJson["path"] = path;
		sceneJson["textures"].push_back(textureJson);
	}


	// Iterate over all entities
	m_Registry.view<Transform>().each([&](entt::entity entity, Transform& trans) {
		json entityJson;
		entityJson["id"] = static_cast<uint32_t>(entity);

		if (m_Registry.any_of<MeshComponent>(entity)) {
			auto& meshComp = m_Registry.get<MeshComponent>(entity);
			if (meshComp.mesh) {
				json meshJson;

				if (auto cube = dynamic_cast<Shapes::Cube*>(meshComp.mesh.get())) {
					meshJson["type"] = "Cube";
					//	meshJson["size"] = cube->size;
				}
				else if (auto sphere = dynamic_cast<Shapes::Pyramid*>(meshComp.mesh.get())) {
					meshJson["type"] = "Pyramid";
					//	meshJson["radius"] = sphere->radius;
				}

				entityJson["MeshComponent"] = meshJson;
			}
		}


		// Transform
		if (m_Registry.any_of<Transform>(entity)) {
			auto& transform = m_Registry.get<Transform>(entity);
			entityJson["Transform"] = {
				{"position", {transform.position.x, transform.position.y, transform.position.z}},
				{"rotation", {transform.rotation.x, transform.rotation.y, transform.rotation.z}},
				{"scale",    {transform.scale.x,    transform.scale.y,    transform.scale.z}}
			};
		}
		if (m_Registry.any_of<Color>(entity)) {
			auto& color = m_Registry.get<Color>(entity);
			entityJson["Color"] = {
				{"whatColor", {color.value.r, color.value.g, color.value.b,color.value.a}}
			};
		}
		if (m_Registry.any_of<Texture>(entity)) {
			auto& texture = m_Registry.get<MeshComponent>(entity);
			entityJson["Texture"] = texture.mesh.get()->mesh.textures[0].path;
		}


		sceneJson["entities"].push_back(entityJson);
		});

	// Save to file
	std::ofstream file(filepath);
	if (file.is_open()) {
		file << sceneJson.dump(4); // pretty print with 4 spaces
	}
}

void Scene::Load(const std::filesystem::path& filepath)
{
	std::ifstream file(filepath);
	if (!file.is_open()) return;

	json sceneJson;
	file >> sceneJson;

	m_Registry.clear(); // Remove existing entities
	m_Textures.clear();

	for (auto& textureJson : sceneJson["textures"])
	{

		if (textureJson.contains("path"))
		{
			auto stringJson = textureJson["path"].get<std::string>();;
			Texture texture(stringJson, "texture_diffuse");
			m_Textures.insert({ stringJson,texture });
		}
	}

	for (auto& entityJson : sceneJson["entities"]) {
		auto entity = m_Registry.create();
		if (entityJson.contains("MeshComponent")) {
			auto& meshJson = entityJson["MeshComponent"];
			MeshComponent meshComp;

			if (meshJson["type"] == "Cube") {
				auto cube = std::make_shared<Shapes::Cube>();
				//	cube->size = meshJson["size"];
				meshComp.mesh = cube;
			}
			else if (meshJson["type"] == "Sphere") {
				auto sphere = std::make_shared<Shapes::Pyramid>();
				//	sphere->radius = meshJson["radius"];
				meshComp.mesh = sphere;
			}

			m_Registry.emplace<MeshComponent>(entity, meshComp);
		}


		// Transform
		if (entityJson.contains("Transform")) {
			Transform t;
			auto& tJson = entityJson["Transform"];
			t.position = glm::vec3(tJson["position"][0], tJson["position"][1], tJson["position"][2]);
			t.rotation = glm::vec3(tJson["rotation"][0], tJson["rotation"][1], tJson["rotation"][2]);
			t.scale = glm::vec3(tJson["scale"][0], tJson["scale"][1], tJson["scale"][2]);
			m_Registry.emplace<Transform>(entity, t);
		}
		if (entityJson.contains("Color")) {
			Color c;
			auto& tJson = entityJson["Color"];
			c.value = glm::vec4(tJson["whatColor"][0], tJson["whatColor"][1], tJson["whatColor"][2], tJson["whatColor"][3]);
			m_Registry.emplace<Color>(entity, c);
		}


		
		if (entityJson.contains("Texture")) {
			Texture texture;
			auto strPath = entityJson["Texture"].get<std::string>();
			auto& compp = GetComponent<MeshComponent>(entity);

			texture.LoadFromFile(strPath,"diffuse_texture");
			compp.mesh->mesh.textures.push_back(texture);
			m_Registry.emplace<Texture>(entity);
		}

	}
}
