#pragma once
#include <vector>

#include "GLContext.h"
#include "Keybind.h"
#include "Camera.h"

class InputManager
{
private:
	using mouse_callback = void (*)(GLFWwindow* window, double xpos, double ypos);

public:
	static InputManager& getInstance();
	void ProcessInput() const;
	inline void SetCamera(Camera* camera) { m_Camera = camera; }

	inline void SetCursor(const float x, const float y) { m_CursorX = x; m_CursorY = y; }
	inline void GetCursor(float& x, float& y) { x = m_CursorX; y = m_CursorY; }
	inline Camera* GetCamera() { return m_Camera; }

	float xoffset, yoffset;

	void SetCursorCallback(mouse_callback cpcb);
	void SetScrollCallback(mouse_callback sccb);
	void HideCursor();

	template<typename T>
	void RegisterKeybind(int keyId);

	
	InputManager(InputManager const&) = delete;
	void operator=(InputManager const&) = delete;
private:
	InputManager();
	~InputManager();
	void SetGlobalSettings();

	static void default_cursor_callback(GLFWwindow* window, double xposIn, double yposIn);
	static void default_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	Camera* m_Camera;
	GLContext& m_Context;
	double m_CursorX;
	double m_CursorY;
	std::vector<Keybind*> m_Keybinds;
	mouse_callback m_CursorPosCallback;
	mouse_callback m_ScrollCallback;

	static InputManager& m_Instance;
};


template<typename T>
inline void InputManager::RegisterKeybind(int keyId)
{
	m_Keybinds.push_back(new T(keyId));
}