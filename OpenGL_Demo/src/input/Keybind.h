#pragma once

//enum class KeyActionType
//{
//	None=-1, WindowClose, CursorToggle,
//	Camera = 42, CameraForward, CameraBackward, CameraRight, CameraLeft,
//};

/* Printable keys */
#define GLFW_KEY_SPACE              32
#define GLFW_KEY_0                  48
#define GLFW_KEY_A                  65
#define GLFW_KEY_WORLD_2            162
/* end */

/* Function keys */
#define GLFW_KEY_ESCAPE             256
#define GLFW_KEY_LEFT_CONTROL       341
#define GLFW_KEY_MENU               348
#define GLFW_KEY_LAST               GLFW_KEY_MENU
/* end */

enum class Key : int 
{
	None = -1,
	Space = GLFW_KEY_SPACE,
	Zero, One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
	A = GLFW_KEY_A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
	Esc = GLFW_KEY_ESCAPE, LeftCtrl = GLFW_KEY_LEFT_CONTROL
};

#define GLFW_RELEASE 0

enum class KeyEvent 
{
	None = -1, Release = GLFW_RELEASE, Press, Repeat
};

class Keybind
{
public:
	using actionType = std::function<void(void)>;
public:
	Keybind(Key id, KeyEvent glEventType) 
		: m_GlId(id), m_GlEventType(glEventType), m_Action(nullptr)
	{}
	Keybind() {}
	~Keybind() {}

	void Exec() { m_Action(); };
	void BindAction(actionType action) { m_Action = action; }

	Key GlId() const { return m_GlId; }
	KeyEvent GlType() const { return m_GlEventType; }
private:
	Key m_GlId;
	KeyEvent m_GlEventType;
	actionType m_Action;
};