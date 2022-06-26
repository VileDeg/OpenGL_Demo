#pragma once

#include "GLContext.h"
#include "Camera.h"
#include <chrono>
#include <thread>

class Keybind
{
public:
	Keybind(int id) : m_GlId(id) {}
	~Keybind() {}

	virtual void OnPress() = 0;
	inline const int GlId() const { return m_GlId; }
private:
	int m_GlId;
	
};

class KeybindDelayed : public Keybind
{
public:
	KeybindDelayed(int id) : Keybind(id), m_Delay(1000), m_DelayTimer(0) {}
	~KeybindDelayed() {}

	void OnPress() final;

	virtual void Action() const = 0;
private:
	void DelayTimer();

	int m_Delay;
	int m_DelayTimer;
};

class Keybind_Forward : public Keybind
{
public:
	Keybind_Forward(int id) : Keybind(id) {}
	~Keybind_Forward() {}

	void OnPress() override;	
};

class Keybind_Backward : public Keybind
{
public:
	Keybind_Backward(int id) : Keybind(id) {}
	~Keybind_Backward() {}

	void OnPress() override;
};

class Keybind_Left : public Keybind
{
public:
	Keybind_Left(int id) : Keybind(id) {}
	~Keybind_Left() {}

	void OnPress() override;
};

class Keybind_Right : public Keybind
{
public:
	Keybind_Right(int id) : Keybind(id) {}
	~Keybind_Right() {}

	void OnPress() override;
};

class Keybind_CloseWindow : public KeybindDelayed
{
public:
	Keybind_CloseWindow(int id) : KeybindDelayed(id) {}
	~Keybind_CloseWindow() {}

	void Action() const override;
};

class Keybind_ToggleCursor : public KeybindDelayed
{
public:
	Keybind_ToggleCursor(int id) : KeybindDelayed(id) {}
	~Keybind_ToggleCursor() {}

	void Action() const override;
};