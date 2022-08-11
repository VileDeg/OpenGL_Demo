#pragma once
#include "OpenGL_Demo.h"
#include "imgui/ImguiLayer.h"
#include "imgui/imgui.h"
#include "ui/SceneHierarchyPanel.h"

namespace Editor
{
	void Init();
	void Run();
	void SaveSceneAs();
	void LoadScene();
	/*const Camera& GetCamera();
	const Scene&  GetActiveScene();*/
};