#include "pch.h"
#include "Input.h"
#define MAGIC_ENUM_RANGE_MIN -1
#define MAGIC_ENUM_RANGE_MAX 512
#include "magic_enum.hpp"
#include "imgui.h"

namespace Input
{
	namespace //private
	{
		std::vector<Keybind>     m_Keybinds{};
		std::vector<std::string> m_KeybindNames{};
	}

	void ProcessInput(int key, int action)
	{
		for (auto& kb : m_Keybinds)
		{
			if (key == (int)kb.GlId() && action == (int)kb.GlType())
				kb.Exec();
		}
	}

	void SetKeybind(const std::string& name, Key keyCode, KeyEvent eventType, Keybind::actionType func)
	{
		Keybind kb(keyCode, eventType);
		kb.BindAction(func);
		m_Keybinds.push_back(kb);
		m_KeybindNames.push_back(name);
	}

	void UIDisplayControlsInfo(bool* p_open, ImGuiWindowFlags panelFlags)
	{
		ImGui::Begin("Controls", p_open, panelFlags);
		{
			for (size_t i = 0; i < m_Keybinds.size(); ++i)
			{
				std::string text = m_KeybindNames[i] + ":\t";
				text += magic_enum::enum_name(m_Keybinds[i].GlId());

				ImGui::BulletText(text.c_str());
			}
		}
		ImGui::End();
	}
}