#include "pch.h"
#include "Input.h"
#define MAGIC_ENUM_RANGE_MIN -1
#define MAGIC_ENUM_RANGE_MAX 512
#include "magic_enum.hpp"
#include "imgui.h"

#include <GLFW/glfw3.h>
#include "core/Window.h"

namespace Input
{
	namespace //private
	{
		std::vector<Keybind>     m_Keybinds{};
		std::vector<std::string> m_KeybindNames{};
		std::vector<Keybind>     m_CamKeybinds{};
		std::vector<std::string> m_CamKeybindNames{};
	}

	bool Input::IsKeyDown(Key key)
	{
		return glfwGetKey(Window::Handle(), (int)key) == GLFW_PRESS;
	}

	bool Input::IsMouseButtonDown(MouseButton btn)
	{
		return glfwGetMouseButton(Window::Handle(), (int)btn) == GLFW_PRESS;
	}
	void ProcessInput(int key, int action)
	{
		for (auto& kb : m_Keybinds)
		{
			if (key == (int)kb.GlId() && action == (int)kb.GlType())
				kb.Exec();
		}
	}

	void ProcessCameraInput(GLFWwindow* handle)
	{
		for (auto key : m_CamKeybinds)
		{
			if (glfwGetKey(handle, (int)key.GlId()) == (int)key.GlType())
				key.Exec();
		}
	}

	void SetKeybind(const std::string& name, Key keyCode, 
		KeyEvent eventType, Keybind::actionType func)
	{
		Keybind kb(keyCode, eventType);
		kb.BindAction(func);
		m_Keybinds.push_back(kb);
		m_KeybindNames.push_back(name);
	}

	void SetCameraKeybind(const std::string& name, Key keyCode, 
		KeyEvent eventType, Keybind::actionType func)
	{
		Keybind kb(keyCode, eventType);
		kb.BindAction(func);
		m_CamKeybinds.push_back(kb);
		m_CamKeybindNames.push_back(name);
	}

	void UIDisplayControlsInfo(bool* p_open, ImGuiWindowFlags panelFlags)
	{
		ImGui::Begin("Controls", p_open, panelFlags);
		{
			std::vector<std::string> text{};
			size_t maxlen = 0;
			std::string str{};

			for (size_t i = 0; i < m_Keybinds.size(); ++i)
			{
				str = m_KeybindNames[i] + ": ";
				text.push_back(str);
				maxlen = str.length() > maxlen ? str.length() : maxlen;
			}
			for (size_t i = 0; i < m_Keybinds.size(); ++i)
			{
				size_t diff = maxlen - text[i].length();
				for (size_t j = 0; j < diff; ++j)
					text[i] += ' ';
				text[i] += magic_enum::enum_name(m_Keybinds[i].GlId());
				ImGui::BulletText(text[i].c_str());
			}

			ImGui::Separator(); ImGui::Separator();
			text.clear();
			maxlen = 0;
			str.clear();

			for (size_t i = 0; i < m_CamKeybinds.size(); ++i)
			{
				str = m_CamKeybindNames[i] + ": ";
				text.push_back(str);
				maxlen = str.length() > maxlen ? str.length() : maxlen;
			}
			for (size_t i = 0; i < m_CamKeybinds.size(); ++i)
			{
				size_t diff = maxlen - text[i].length();
				for (size_t j = 0; j < diff; ++j)
					text[i] += ' ';
				text[i] += magic_enum::enum_name(m_CamKeybinds[i].GlId());
				ImGui::BulletText(text[i].c_str());
			}
		}
		ImGui::End();
	}
}