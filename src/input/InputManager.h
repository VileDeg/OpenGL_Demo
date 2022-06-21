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
	void SetCamera(Camera camera) { m_Camera = &camera; }

	inline void SetCursor(const float x, const float y) { m_CursorX = x; m_CursorY = y; }
	inline void GetCursor(float& x, float& y) { x = m_CursorX; y = m_CursorY; }
	inline Camera& GetCamera() { return *m_Camera; }

	inline void SetCursorCallback(mouse_callback cpcb) { m_CursorPosCallback = cpcb; }
	inline void SetScrollCallback(mouse_callback sccb) { m_ScrollCallback = sccb; }

	template<typename T>
	void RegisterKeybind(int keyId);

	
	InputManager(InputManager const&) = delete;
	void operator=(InputManager const&) = delete;
private:
	InputManager();
	~InputManager();
	void SetGlobalSettings();

	static void default_mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

	Camera* m_Camera;
	GLContext& m_Context;
	double m_CursorX;
	double m_CursorY;
	std::vector<Keybind*> m_Keybinds;
	mouse_callback m_CursorPosCallback;
	mouse_callback m_ScrollCallback;

	static InputManager& m_Instance;
};


