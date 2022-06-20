#pragma once

#include "GLContext.h"
#include "Camera.h"

class Keybind
{
public:
	Keybind(int id) : m_GlId(id) {}
	~Keybind() {}

	virtual void OnPress(Camera) const = 0;
	inline const int GlId() const { return m_GlId; }
private:
	int m_GlId;
};

class Keybind_Forward : public Keybind
{
public:
	Keybind_Forward(int id) : Keybind(id) {}
	~Keybind_Forward() {}

	virtual void OnPress(Camera) const;	
};

class Keybind_Backward : public Keybind
{
public:
	Keybind_Backward(int id) : Keybind(id) {}
	~Keybind_Backward() {}

	virtual void OnPress(Camera) const;
};

class Keybind_Left : public Keybind
{
public:
	Keybind_Left(int id) : Keybind(id) {}
	~Keybind_Left() {}

	virtual void OnPress(Camera) const;
};

class Keybind_Right : public Keybind
{
public:
	Keybind_Right(int id) : Keybind(id) {}
	~Keybind_Right() {}

	virtual void OnPress(Camera) const;
};