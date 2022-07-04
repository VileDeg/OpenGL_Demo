#include "pch.h"
#include "InputManager.h"
//#include <thread>
#include "GLContext.h"

static GLContext& context = GLContext::getTnstance();
static GLFWwindow* window = context.Window();

KeybindCamera::~KeybindCamera()
{
}

void KeybindCamera::Command()
{
	if (cam) // && !context.CursorVisible()
		Action();
	else
		cam = InputManager::getInstance().GetCamera();
}



void KeybindDelayed::Command(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	Action();

}

void Keybind_CloseWindow::Action()
{
	if (!context.CursorVisible())
		context.ShowCursor();
	else
		glfwSetWindowShouldClose(window, true);
}

void Keybind_ToggleCursor::Action()
{
	if (context.CursorVisible())
		context.HideCursor();
	else
		context.ShowCursor();
}


