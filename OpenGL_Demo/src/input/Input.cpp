#include "pch.h"
#include "Input.h"

std::vector<Keybind> Input::m_Keybinds{};

void Input::ProcessInput(int key, int action)
{
	for (auto& kb : m_Keybinds)
	{
		if (key == kb.GlId() && action == kb.GlType())
			kb.Exec();
	}
}

void Input::SetKeybind(Key keyCode, KeyEvent eventType, Keybind::actionType func)
{
	Keybind kb((int)keyCode, (int)eventType);
	kb.BindAction(func);
	m_Keybinds.push_back(kb);
}