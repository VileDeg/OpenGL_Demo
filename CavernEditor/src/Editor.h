#pragma once
#include "Cavern.h"
#include "imgui/ImguiLayer.h"
#include "imgui/imgui.h"
#include "ui/SceneHierarchyPanel.h"

namespace Crave
{
	namespace Editor
	{
		void Init();
		void Run();
		void SaveSceneAs();
		void LoadScene();
	};
}