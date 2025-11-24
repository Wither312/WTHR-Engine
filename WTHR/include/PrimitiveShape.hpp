#pragma once
#include "Mesh.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <memory>
#include <Texture.hpp>

namespace Shapes
{
	// Abstract Base Class for all shapes
	class PrimitiveShape
	{
	public:
		virtual ~PrimitiveShape() = default;
		// Mesh data for each shape (to be used by derived classes)
		Mesh mesh;
		void Draw(Shader& shader) { mesh.Draw(shader); }
	};
}

namespace Shapes
{
	class Cube : public PrimitiveShape
	{
	public:
		Cube()
		{
			static std::vector<Vertex> cubeVertices = {
				{{-0.5f, -0.5f, -0.5f}, {1,0,0}, {0.0f, 0.0f}},
				{{ 0.5f, -0.5f, -0.5f}, {0,1,0}, {1.0f, 0.0f}},
				{{ 0.5f,  0.5f, -0.5f}, {0,0,1}, {1.0f, 1.0f}},
				{{-0.5f,  0.5f, -0.5f}, {1,1,0}, {0.0f, 1.0f}},
				{{-0.5f, -0.5f,  0.5f}, {1,0,1},    {1.0f, 0.0f}},
				{{ 0.5f, -0.5f,  0.5f}, {0,1,1},    {0.0f, 0.0f}},
				{{ 0.5f,  0.5f,  0.5f}, {1,0.5f,0}, {0.0f, 1.0f}},
				{{-0.5f,  0.5f,  0.5f}, {0.5f,0,1}, {1.0f, 1.0f}},
			};


			static std::vector<unsigned int> cubeIndices = {
				0,1,2, 2,3,0,
				4,5,6, 6,7,4,
				4,0,3, 3,7,4,
				1,5,6, 6,2,1,
				4,5,1, 1,0,4,
				3,2,6, 6,7,3
			};

			std::vector<Texture> cubeTextures;

			mesh = Mesh(cubeVertices, cubeIndices, cubeTextures);
		}

	
	};
}
namespace Shapes
{
	class Pyramid : public PrimitiveShape
	{
	public:
		Pyramid()
		{
			std::vector<Vertex> pyramidVertices = {
				{{-0.5f, 0.0f, -0.5f}, {1,0,0}, {0.0f, 0.0f}},
				{{ 0.5f, 0.0f, -0.5f}, {0,1,0}, {1.0f, 0.0f}},
				{{ 0.5f, 0.0f,  0.5f}, {0,0,1}, {1.0f, 1.0f}},
				{{-0.5f, 0.0f,  0.5f}, {1,1,0}, {0.0f, 1.0f}},
				{{ 0.0f, 1.0f,  0.0f}, {1,0,1}, { 0.0f, 0.0f }}
			};

			std::vector<unsigned int> pyramidIndices = {
				0, 1, 4, // front
				1, 2, 4, // right
				2, 3, 4, // back
				3, 0, 4, // left
				0, 1, 2, // base triangle 1
				2, 3, 0  // base triangle 2
			};

			std::vector<Texture> pyramidTextures;

			mesh = Mesh(pyramidVertices, pyramidIndices, pyramidTextures);
		}

	
	};
}

