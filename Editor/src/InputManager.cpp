#include <pch.hpp>
#include "InputManager.hpp"

InputManager::InputManager()
{
    // Initialize keys and mouse buttons if needed
}

void InputManager::Update()
{
    // Update keyboard state
    for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key)
    {
        m_Keys[key] = glfwGetKey(m_Window, key) == GLFW_PRESS;
    }

    // Update mouse button state
    for (int button = GLFW_MOUSE_BUTTON_1; button <= GLFW_MOUSE_BUTTON_LAST; ++button)
    {
        m_MouseButtons[button] = glfwGetMouseButton(m_Window, button) == GLFW_PRESS;
    }

    // Update mouse position
    glfwGetCursorPos(m_Window, &m_MouseX, &m_MouseY);
}

bool InputManager::IsKeyPressed(int key) const
{
    auto it = m_Keys.find(key);
    return it != m_Keys.end() && it->second;
}

bool InputManager::IsMouseButtonPressed(int button) const
{
    auto it = m_MouseButtons.find(button);
    return it != m_MouseButtons.end() && it->second;
}

void InputManager::GetMousePosition(double& x, double& y) const
{
    x = m_MouseX;
    y = m_MouseY;
}
