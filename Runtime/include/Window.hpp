#pragma once
#include <string>
#include <GLFW/glfw3.h>

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    void PollEvents();
    bool ShouldClose() const;
    void SwapBuffers();

    GLFWwindow* GetNativeWindow() const { return window_; }

private:
    bool InitGLFW();
    bool InitGLAD();

    GLFWwindow* window_;
    int width_;
    int height_;
    std::string title_;
};
