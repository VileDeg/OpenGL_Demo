#pragma once
#include "input/Keybind.h"

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

enum class KeyEvent : int
{
	None=-1, Release = GLFW_RELEASE, Press, Repeat
};

class Input
{
public:
	static void ProcessInput(int key, int action);
	static void SetKeybind(Key keyCode, KeyEvent eventType, Keybind::actionType func);

private:
	static std::vector<Keybind> m_Keybinds;
};