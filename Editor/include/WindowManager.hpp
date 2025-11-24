#pragma once
#include <pch.hpp>


class WindowManager
{
public:
    WindowManager(int width = 5, int height = 5, const char* title = "WTHR Engine");
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
    static int GetWidth()  { return m_Width; }
    static int GetHeight()  { return m_Height; }
    const char* GetTitle() const { return m_Title; }
    GLFWwindow* GetWindow() const { return m_Window; }
    bool isOpen() const { return !glfwWindowShouldClose(m_Window); }

    void SetTitle(const char* title);
    void SetSize(int width, int height);

private:
    static int m_Width;
    static int m_Height;
    const char* m_Title;
    GLFWwindow* m_Window = nullptr;

    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
};
