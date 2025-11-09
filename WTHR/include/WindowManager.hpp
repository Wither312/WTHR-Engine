#pragma once
#include <pch.hpp>


class WindowManager
{
public:
    WindowManager(int width = 1280, int height = 720, const char* title = "WTHR Engine");
    ~WindowManager();

    // Initialization & shutdown
    bool Init();
    void Shutdown();

    // Main loop helpers
    void PollEvents();
    void SwapBuffers();
    bool ShouldClose() const;

    void BeginFrame();
    void EndFrame();

    // Getters & setters
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    const char* GetTitle() const { return m_Title; }
    GLFWwindow* GetWindow() const { return m_Window; }
    bool isOpen() const { return !glfwWindowShouldClose(m_Window); }

    void SetTitle(const char* title);
    void SetSize(int width, int height);

private:
    int m_Width;
    int m_Height;
    const char* m_Title;
    GLFWwindow* m_Window = nullptr;

    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
};
