#pragma once
#include "input/Keybind.h"

struct GLFWwindow;

namespace Input
{
	namespace {
		typedef int ImGuiWindowFlags;
	};
	
	bool IsMouseButtonDown(MouseButton btn);
	bool IsKeyDown(Key key);

	void ProcessInput(int key, int action);
	void ProcessCameraInput(GLFWwindow* handle);
	void SetKeybind(const std::string& name, Key keyCode, 
		KeyEvent eventType, Keybind::actionType func);
	void SetCameraKeybind(const std::string& name, Key keyCode, 
		KeyEvent eventType, Keybind::actionType func);
	void UIDisplayControlsInfo(bool* p_open, ImGuiWindowFlags panelFlags);
};