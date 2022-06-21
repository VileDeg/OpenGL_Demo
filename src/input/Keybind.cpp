#include "InputManager.h"

static Camera& cam = InputManager::getInstance().GetCamera();
static GLFWwindow* window = GLContext::getTnstance().Window();

void Keybind_Forward::OnPress() const
{
	cam.MoveForward();
}

void Keybind_Backward::OnPress() const
{
	cam.MoveBackward();
}

void Keybind_Left::OnPress() const
{
	cam.MoveLeft();
}

void Keybind_Right::OnPress() const
{
	cam.MoveRight();
}

void Keybind_CloseWindow::OnPress() const
{
	glfwSetWindowShouldClose(window, true);
}
