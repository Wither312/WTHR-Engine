#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "Mesh.hpp"
#include "Model.hpp"
#include <PrimitiveShape.hpp>

//Color component
struct Color {
	glm::vec4 value{ 1.0f, 1.0f, 1.0f, 1.0f }; // RGBA, default white
};

// Transform component
struct Transform {
	glm::vec3 position{ 0.0f };
	glm::vec3 rotation{ 0.0f };
	glm::vec3 scale{ 1.0f };
};

// Mesh component (wraps a primitive shape)
struct MeshComponent {
	std::shared_ptr<Shapes::PrimitiveShape> mesh;
};

class ThreadSafeModel {
public:
	ThreadSafeModel() : isLoaded(false) {}

	void SetModel(std::unique_ptr<Model> newModel) {
		std::lock_guard<std::mutex> lock(modelMutex);
		model = std::move(newModel);
		isLoaded = (model != nullptr);
	}

	bool Draw(Shader& shader) {
		std::lock_guard<std::mutex> lock(modelMutex);
		if (model && isLoaded) {
			model->Draw(shader);
			return true;
		}
		return false;
	}

	bool IsLoaded() const {
		return isLoaded;
	}

	void Reset() {
		std::lock_guard<std::mutex> lock(modelMutex);
		model.reset();
		isLoaded = false;
	}

private:
	std::unique_ptr<Model> model;
	std::mutex modelMutex;
	std::atomic<bool> isLoaded;
};


struct ModelComponent {
    std::shared_ptr<ThreadSafeModel> model;
};