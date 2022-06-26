#include "InputManager.h"
#include <thread>

static Camera* cam = InputManager::getInstance().GetCamera();
static GLContext& context = GLContext::getTnstance();
static GLFWwindow* window = context.Window();



void Keybind_Forward::OnPress()
{
	if (cam)
		cam->MoveForward();
	else
		cam = InputManager::getInstance().GetCamera();
}

void Keybind_Backward::OnPress()
{
	if (cam)
		cam->MoveBackward();
	else
		cam = InputManager::getInstance().GetCamera();
}

void Keybind_Left::OnPress()
{
	if (cam)
		cam->MoveLeft();
	else
		cam = InputManager::getInstance().GetCamera();
}

void Keybind_Right::OnPress()
{
	if (cam)
		cam->MoveRight();
	else
		cam = InputManager::getInstance().GetCamera();
}

void KeybindDelayed::DelayTimer()
{
	while (m_DelayTimer < m_Delay)
	{
		std::cout << "Timer is running.\n";
		++m_DelayTimer;
	}

}


void KeybindDelayed::OnPress()
{
	if (m_DelayTimer >= m_Delay)
	{
		Action();
		m_DelayTimer = 0;
	}
	else
	{
		std::thread timer([this]{ this->DelayTimer(); });
		timer.detach();
	}
}

void Keybind_CloseWindow::Action() const
{
	if (!context.CursorVisible())
		context.ShowCursor();
	else
		glfwSetWindowShouldClose(window, true);
}

void Keybind_ToggleCursor::Action() const
{
	if (context.CursorVisible())
		context.HideCursor();
	else
		context.ShowCursor();
}