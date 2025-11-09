#pragma once
#include <pch.hpp>

#include <Camera.hpp>
#include <Scene.hpp>
#include <Shader.hpp>
#include <Mesh.hpp>
#include <PrimitiveShape.hpp>

class Renderer
{
public:
    Renderer() = default;
    ~Renderer()
    {

    }

    void Init();
    void Clear(const glm::vec3& color = { 0.1f, 0.1f, 0.1f });
    void DrawTriangle();
    void DrawTriangle(const glm::vec3& color);
    void RenderScene(Scene&,Shader& );


private:
    GLuint m_VAO = 0;
    GLuint m_VBO = 0;
    GLuint m_ShaderProgram = 0;

    GLuint CompileShader(const std::string& source, GLenum type);
    void CreateShaderProgram();
};
