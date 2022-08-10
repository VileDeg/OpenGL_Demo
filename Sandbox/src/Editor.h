#pragma once
#include "OpenGL_Demo.h"
#include "imgui/ImguiLayer.h"
#include "imgui/imgui.h"
#include "ui/SceneHierarchyPanel.h"

class Editor
{
public:
	Editor();
	~Editor();

	void Run();
	void UIRender();
private:
	void UIDrawMenuBar();
	void UIDrawViewport();
	void UIDrawGizmos();
	void SaveSceneAs();
	void LoadScene();
private:
	/*GLContext* m_Context;
	Window*    m_Window;*/

	Ref<Camera> m_Camera;
	Ref<Scene> m_ActiveScene;

	int m_GizmoType = -1;
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	SceneHierarchyPanel m_SceneHierarchyPanel;
private:
	static constexpr const unsigned WINDOW_WIDTH		 = 1600;
	static constexpr const unsigned WINDOW_HEIGHT		 = 900;
};