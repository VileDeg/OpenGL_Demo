#include "InputManager.h"
#include <thread>

static GLContext& context = GLContext::getTnstance();
static GLFWwindow* window = context.Window();

void KeybindCamera::OnPress()
{
	if (cam && !context.CursorVisible())
		Action();
	else
		cam = InputManager::getInstance().GetCamera();
}


void KeybindDelayed::DelayTimer()
{
	while (m_DelayTimer < m_Delay)
	{
		std::cout << m_DelayTimer << "  :  " << m_Delay << '\n';
		m_DelayTimer += context.DeltaTime();
	}
}

void KeybindDelayed::OnPress()
{
	//Action();
	//m_DelayTimer = 0;
	//if (m_DelayTimer >= m_Delay)
	//{
	//	
	//}
	//else
	//{
	//	/*std::thread timer([this]{ this->DelayTimer(); });
	//	timer.detach();*/
	//}
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

