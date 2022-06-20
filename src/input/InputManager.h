#pragma once
#include <vector>

#include "GLContext.h"
#include "Keybind.h"
#include "Camera.h"

class InputManager
{
private:
	using cursor_pos_callback = void (*)(GLFWwindow* window, double xpos, double ypos);
public:
	static InputManager& getTnstance(cursor_pos_callback = NULL);

	void ProcessInput() const;
	void SetCamera(Camera& camera) { m_Camera = camera; }

	template<typename T>
	void RegisterKeybind(int keyId);

	~InputManager();
	InputManager(InputManager const&) = delete;
	void operator=(InputManager const&) = delete;
private:
	InputManager(cursor_pos_callback);

	void SetGlobalSettings();

	Camera& m_Camera;
	GLContext& m_Context;
	float m_CursorX;
	float m_CursorY;
	std::vector<Keybind*> m_Keybinds;
	cursor_pos_callback m_CursorPosCallback;
};


