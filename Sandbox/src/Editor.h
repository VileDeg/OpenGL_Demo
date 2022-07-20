#pragma once
#include "OpenGL_Demo.h"
#include "imgui/ImguiLayer.h"
#include "imgui/imgui.h"
#include "tests/TestRenderer.h"

class Editor
{
public:
	Editor();
	~Editor();

	void Run();
	void ImGuiShowViewport();
	void ImGuiRender();
private:
	GLContext* m_Context;
	Window*    m_Window;
	Ref<Camera> m_Camera;
	//Ref<Framebuffer> m_Framebuffer;
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	Scope<test::TestRenderer> m_RendererTest;
private:
	static constexpr const unsigned WINDOW_WIDTH = 1600;
	static constexpr const unsigned WINDOW_HEIGHT = 900;
};