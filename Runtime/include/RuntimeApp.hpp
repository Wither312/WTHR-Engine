#pragma once
#include <glad/glad.h>
#include <memory>
#include "Window.hpp"
#include <WTHR.hpp>

class RuntimeApp {
public:
    RuntimeApp(std::string);
    ~RuntimeApp() = default;

    void Run();

private:
    std::string m_SceneName;
    std::shared_ptr<Window> m_Window;
    std::shared_ptr<Scene> m_Scene;
    std::shared_ptr<Renderer> m_Renderer;

    void Init();
    void Shutdown();
};
