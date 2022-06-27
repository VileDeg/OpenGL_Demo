#pragma once


#include "Camera.h"
//#include <chrono>
//#include <thread>




class Keybind
{
protected:
	//using key_command = void(GLFWwindow*, int, int, int, int);
public:
	Keybind(int id, int type) 
		: m_GlId(id), m_GlEventType(type) {}
	~Keybind() {}

	//virtual void Command(GLFWwindow*, int, int, int, int) = 0;
	virtual void Action() = 0;
	inline const int GlId() const { return m_GlId; }
	inline const int GlType() const { return m_GlEventType; }
protected:
	int m_GlId;
	int m_GlEventType;
};


class KeybindCamera : public Keybind
{
public:
	KeybindCamera(int id) 
		: Keybind(id, GL_TYPE), cam(nullptr) {}
	virtual ~KeybindCamera() = 0;

	void Command();
	Camera* cam;
private:
	static constexpr int GL_TYPE = GLFW_PRESS;
};

class KeybindDelayed : public Keybind
{
public:
	KeybindDelayed(int id) : Keybind(id, GL_TYPE) {}
	~KeybindDelayed() {}

	void Command(GLFWwindow*, int, int, int, int);

	virtual void Action() = 0;
private:
	//void DelayTimer();

	int m_Delay;
	int m_DelayTimer;
	static constexpr int GL_TYPE = GLFW_PRESS;
	//static constexpr int DELAY = 100000;
};

class Keybind_Forward : public KeybindCamera
{
public:
	Keybind_Forward(int id) : KeybindCamera(id) {}
	~Keybind_Forward() {}

	inline void Action() override { cam->MoveForward(); }
};

class Keybind_Backward : public KeybindCamera
{
public:
	Keybind_Backward(int id) : KeybindCamera(id) {}
	~Keybind_Backward() {}

	inline void Action() override { cam->MoveBackward(); }
};

class Keybind_Left : public KeybindCamera
{
public:
	Keybind_Left(int id) : KeybindCamera(id) {}
	~Keybind_Left() {}

	inline void Action() override { cam->MoveLeft(); };
};

class Keybind_Right : public KeybindCamera
{
public:
	Keybind_Right(int id) : KeybindCamera(id) {}
	~Keybind_Right() {}

	inline void Action() override { cam->MoveRight(); }
};

class Keybind_CloseWindow : public KeybindDelayed
{
public:
	Keybind_CloseWindow(int id) : KeybindDelayed(id) {}
	~Keybind_CloseWindow() {}

	void Action() override;
};

class Keybind_ToggleCursor : public KeybindDelayed
{
public:
	/*GLContext& context;
	using key_callback = void(*)(GLFWwindow*, int, int, int, int);
	Keybind_ToggleCursor(int id) : KeybindDelayed(id), context(GLContext::getTnstance()) {
		key_callback keyCB = [=](GLFWwindow* win, int keycode, int scancode, int action , int mods)
		{
			if (keycode == id)
		};
		glfwSetKeyCallback()
	}*/
	Keybind_ToggleCursor(int id) : KeybindDelayed(id) {}
	~Keybind_ToggleCursor() {}

	void Action() override;
};