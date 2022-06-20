#pragma once
#include <vector>

#include "GLContext.h"
#include "Keybind.h"

class InputManager
{
private:
	using cursor_pos_callback = void (*)(GLFWwindow* window, double xpos, double ypos);
public:
	static InputManager& getTnstance(cursor_pos_callback = NULL);

	void ProcessInput() const;

	InputManager(InputManager const&) = delete;
	void operator=(InputManager const&) = delete;
private:
	InputManager(cursor_pos_callback);

	void SetGlobalSettings();

	float m_CursorX;
	float m_CursorY;
	std::vector<Keybind> m_Keybinds;
	cursor_pos_callback m_CursorPosCallback;
};

