#pragma once

enum class KeyActionType
{
	None=-1, WindowClose, CursorToggle,
	Camera = 42, CameraForward, CameraBackward, CameraRight, CameraLeft,
};

class Keybind
{
public:
	using actionType = std::function<void(void)>;
public:
	Keybind(int id, int glEventType) 
		: m_GlId(id), m_GlEventType(glEventType), m_Action(nullptr)
	{}
	Keybind() {}
	~Keybind() {}

	void Exec() { m_Action(); };
	void BindAction(actionType action) { m_Action = action; }
	inline const int GlId() const { return m_GlId; }
	inline const int GlType() const { return m_GlEventType; }
protected:
	int m_GlId;
	int m_GlEventType;
	actionType m_Action;
};