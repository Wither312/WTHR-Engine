#pragma once
#include "pch.hpp"

#include <WindowManager.hpp>
#include <InputManager.hpp>
#include <Renderer.hpp>

class Application
{
public:
	Application(int width = 1280, int height = 720, const char* title = "WTHR Editor");
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
	int m_Width;
	int m_Height;
	const char* m_Title;
	WindowManager m_WindowManager;
	InputManager m_Input;
	Renderer m_Renderer;
};
