#pragma once
#include "input/Keybind.h"

namespace Input
{
	namespace {
		typedef int ImGuiWindowFlags;
	};
	
	void ProcessInput(int key, int action);
	void SetKeybind(const std::string& name, Key keyCode, 
		KeyEvent eventType, Keybind::actionType func);
	void UIDisplayControlsInfo(bool* p_open, ImGuiWindowFlags panelFlags);
};