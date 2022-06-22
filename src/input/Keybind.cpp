#include "InputManager.h"

static Camera* cam = InputManager::getInstance().GetCamera();
static GLFWwindow* window = GLContext::getTnstance().Window();

void Keybind_Forward::OnPress() const
{
	if (cam)
		cam->MoveForward();
	else
		cam = InputManager::getInstance().GetCamera();
}

void Keybind_Backward::OnPress() const
{
	if (cam)
		cam->MoveBackward();
	else
		cam = InputManager::getInstance().GetCamera();
}

void Keybind_Left::OnPress() const
{
	if (cam)
		cam->MoveLeft();
	else
		cam = InputManager::getInstance().GetCamera();
}

void Keybind_Right::OnPress() const
{
	if (cam)
		cam->MoveRight();
	else
		cam = InputManager::getInstance().GetCamera();
}

void Keybind_CloseWindow::OnPress() const
{
	glfwSetWindowShouldClose(window, true);
}
