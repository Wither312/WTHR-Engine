#pragma once
#include "pch.hpp"

#include <WindowManager.hpp>
#include <InputManager.hpp>
#include <Renderer.hpp>

class Application
{
public:
	Application(int width = 5, int height = 5, const char* title = "WTHR Editor");
	virtual ~Application();

	bool Init();
	void Run();

	static bool isFocused;
protected:
	virtual void Update();   // ECS, input, editor logic
	virtual void Render();   // Delegate to Renderer library

	void InitEngineSystems();
	void InitImGui();
	void Shutdown();

protected:
	WindowManager m_WindowManager;
	InputManager m_Input;
	Renderer m_Renderer;
};
