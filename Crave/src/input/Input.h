#pragma once
#include "input/Keybind.h"


struct GLFWwindow;

namespace Crave
{
	namespace Input
	{
		enum class KeybindName
		{
			None=-1, 
			CloseWindow,
			ToggleCameraProjectionType,
			GizmoNone, GizmoTranslate, GizmoRotate, GizmoScale,
		};
		
		namespace 
		{
			using ImGuiWindowFlags = int;
			constexpr const char* const CONTROLS_FILE_PATH = "controls.json";
		};

		bool IsMouseButtonDown(MouseButton btn);
		bool IsKeyDown(Key key);

		void Init();

		void ProcessInput(int key, int action);
		//void ProcessCameraInput(GLFWwindow* handle);
		void KeybindBindAction(KeybindName kbName, Keybind::actionType func);
		//void SetCameraKeybind(const std::string& name, Key keyCode,
		//	KeyEvent eventType, Keybind::actionType func);
		void UIDisplayControlsConfig(bool* p_open, ImGuiWindowFlags panelFlags);

		//void SaveControls();
		//void LoadControls();
	}
}