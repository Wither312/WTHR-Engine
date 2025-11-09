#include "Renderer.hpp"
#include <spdlog/spdlog.h>
#include <Texture.hpp>
#include <Components.hpp>
#include <Model.hpp>



void Renderer::Init()
{
	// OpenGL state
	glEnable(GL_DEPTH_TEST);

	// Triangle vertices
	float vertices[] = {
		// positions        // colors
		 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// colors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	// Compile shaders
	CreateShaderProgram();
	spdlog::info("Renderer initialized");

	// In your main application initialization

}

void Renderer::Clear(const glm::vec3& color)
{
	glClearColor(color.r, color.g, color.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DrawTriangle()
{
	glUseProgram(m_ShaderProgram);
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}
void Renderer::DrawTriangle(const glm::vec3& color)
{
	glUseProgram(m_ShaderProgram);

	// Set uniform
	GLint loc = glGetUniformLocation(m_ShaderProgram, "u_Color");
	glUniform3f(loc, color.r, color.g, color.b);

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void Renderer::RenderScene(Scene& scene, Shader& shader)
{
	// Retrieve the camera (owned by the scene)
	Camera camera = scene.GetCamera();
	auto view = camera.GetViewMatrix();
	float nearPlane = 0.1f;  // don’t make it too small
	float farPlane = 100.f; // must be farther than your objects

	float width = 1280, height = 720;
	glm::mat4 projection = glm::perspective(glm::radians(45.f), width / height, nearPlane, farPlane);

	glm::mat4 model = glm::mat4(1.0f);
	// Log for debug
	/*spdlog::trace("Rendering Scene with {} meshes and {} shaders",
		scene.GetMeshes().size(), scene.GetShaders().size());*/


	auto& registry = scene.GetRegistry(); // getter for the registry
	registry.view<Transform, MeshComponent>().each([](auto entity, auto& transform, auto& mesh) {
		});
	// Iterate over all entities with MeshComponent and Transform
	shader.use();
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);

	registry.view<MeshComponent, Transform>().each([&](auto entity, auto& meshComp, auto& transform) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), transform.position);
		model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));
		model = glm::scale(model, transform.scale);

		glUniform1i(glGetUniformLocation(shader.ID, "useModel"), false);

		shader.setMat4("model", model);

		//if no textures render color // just color
		if (meshComp.mesh->mesh.textures.empty() && scene.HasComponent<Color>(entity))
		{
			glUniform1i(glGetUniformLocation(shader.ID, "useTexture"), false);
			glUniform1i(glGetUniformLocation(shader.ID, "useColor"), true);

			auto& color = scene.GetComponent<Color>(entity);
			glUniform4f(glGetUniformLocation(shader.ID, "uColor"),
				color.value.r, color.value.g, color.value.b, color.value.a);
		}
		else
		{
			if (scene.HasComponent<Color>(entity))
			{
				glUniform1i(glGetUniformLocation(shader.ID, "useTexture"), true);
				glUniform1i(glGetUniformLocation(shader.ID, "useColor"), true);

				auto& color = scene.GetComponent<Color>(entity);
				glUniform4f(glGetUniformLocation(shader.ID, "uColor"),
					color.value.r, color.value.g, color.value.b, color.value.a);
			}
			else
			{
				glUniform1i(glGetUniformLocation(shader.ID, "useTexture"), true);
				glUniform1i(glGetUniformLocation(shader.ID, "useColor"), false);
			}
		}




		if (meshComp.mesh) meshComp.mesh->Draw(shader);

		});

	registry.view<ModelComponent, Transform>().each([&](auto entity, ModelComponent& meshComp, auto& transform) {
	

		glUniform1i(glGetUniformLocation(shader.ID, "useModel"), true);

		shader.setMat4("model", model);
		//Model* model = meshComp.model.get()->GetModel().get();
		//model->Draw(shader);
		if(meshComp.model.get()->IsLoaded())
		meshComp.model.get()->Draw(shader);
		});
	//registry.view<ModelComponent, Transform>().each([&](auto entity, auto& modelComp, auto& transform) {
	//	// Quick null check
	//	
	//	// Build transform matrix
	//	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), transform.position);
	//	modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.x), glm::vec3(1, 0, 0));
	//	modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.y), glm::vec3(0, 1, 0));
	//	modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));
	//	modelMatrix = glm::scale(modelMatrix, transform.scale);

	//	glUniform1i(glGetUniformLocation(shader.ID, "useModel"), true);
	//	shader.setMat4("model", modelMatrix);

	//	modelComp.model->Draw(shader);
	//
	//	});

	
	/*if(scene.modelWrapper.get()->IsLoaded())
	scene.modelWrapper.get()->Draw(shader);
	*/
	
	//registry.view<ThreadSafeModel, Transform>().each([&](auto entity, auto& modelComp, auto& transform) {
	//	glm::mat4 model = glm::translate(glm::mat4(1.0f), transform.position);
	//	model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1, 0, 0));
	//	model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0, 1, 0));
	//	model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));
	//	model = glm::scale(model, transform.scale);

	//	shader.setMat4("model", model);




	//	if (modelComp.model)
	//	{
	//		modelComp.model.get()->Draw(shader);
	//	}

	//	});






	// Typically you'd use 1 shader per mesh, but we’ll keep it simple
	//for (const auto& shader : scene.GetShaders())
	//{
	//	shader->use();
	//	shader->setMat4("view", view);
	//	shader->setMat4("projection", projection);

	//	static glm::vec3 color(1.f);


	//	ImGui::Begin("Cube color");
	//	ImGui::ColorEdit3("Color:", &color.x);
	//	ImGui::End();

	//	glm::mat4 model = glm::mat4(1.0f);
	//	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	//	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
	//	shader->setMat4("model", model);
	//	scene.m_Model.Draw(*shader);


	//	for (const auto& shape : scene.GetMeshes())
	//	{
	//		// Example: each shape might have its own transform
	//		shader->setMat4("model", shape.get()->GetModelMatrix());
	//		// Draw the shape (assuming Mesh::Draw() exists)
	//		shape->mesh.Draw(*shader);
	//	}

	//	glUseProgram(0);
	//}
}


GLuint Renderer::CompileShader(const std::string& source, GLenum type)
{
	GLuint shader = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char info[512];
		glGetShaderInfoLog(shader, 512, nullptr, info);
		spdlog::error("Shader compilation failed: {}", info);
	}
	return shader;
}

void Renderer::CreateShaderProgram()
{
	const std::string vertexShader = R"(
        #version 460 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aColor;
        out vec3 vColor;
        void main()
        {
            vColor = aColor;
            gl_Position = vec4(aPos, 1.0);
        }
    )";

	const std::string fragmentShader = R"(
        #version 460 core
        in vec3 vColor;
        uniform vec3 u_Color; // ImGui-controlled color
        out vec4 FragColor;
        void main()
        {
            FragColor = vec4(u_Color * vColor, 1.0);
        }
    )";

	GLuint vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
	GLuint fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

	m_ShaderProgram = glCreateProgram();
	glAttachShader(m_ShaderProgram, vs);
	glAttachShader(m_ShaderProgram, fs);
	glLinkProgram(m_ShaderProgram);

	int success;
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		char info[512];
		glGetProgramInfoLog(m_ShaderProgram, 512, nullptr, info);
		spdlog::error("Shader linking failed: {}", info);
	}

	glDeleteShader(vs);
	glDeleteShader(fs);
}
