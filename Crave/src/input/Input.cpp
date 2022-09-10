#include "pch.h"
#include "Input.h"
#define MAGIC_ENUM_RANGE_MIN -1
#define MAGIC_ENUM_RANGE_MAX 512
#include "magic_enum.hpp"
#include "imgui.h"

#include "core/Window.h"
#include <GLFW/glfw3.h>
//#include <cereal/cereal.hpp>
//#include <cereal/types/unordered_map.hpp>
//#include <cereal/archives/json.hpp>

namespace Crave
{
	namespace Input
	{
		namespace //private
		{
			struct KeybindsData
			{
				std::unordered_map<KeybindName, Keybind> Keybinds{};
				
				//std::unordered_map<std::string, Keybind> CamKeybinds{};

				//friend class cereal::access;
				//template<typename Archive>
				//void serialize(Archive& ar)
				//{
				//	ar& cereal::make_nvp("Keybinds", Keybinds);
				//	//ar& cereal::make_nvp("CameraKeybinds", CamKeybinds);

				//}
			};

			/*void AddKeybind(const std::string& name, Keybind kb)
			{
				m_KbData.Keybinds[name] = kb;
			}*/

			KeybindsData m_KbData{};
		}

		void KeybindBindAction(KeybindName kbName, Keybind::actionType func)
		{
			m_KbData.Keybinds[kbName].BindAction(func);
		}

		void Init()
		{
			m_KbData.Keybinds[KeybindName::CloseWindow	 ] = { Key::Esc, KeyEvent::Press };
			m_KbData.Keybinds[KeybindName::ToggleCameraProjectionType] 
				= { Key::T, KeyEvent::Press };
			m_KbData.Keybinds[KeybindName::GizmoNone	 ] = { Key::Q  , KeyEvent::Press };
			m_KbData.Keybinds[KeybindName::GizmoTranslate] = { Key::W  , KeyEvent::Press };
			m_KbData.Keybinds[KeybindName::GizmoRotate	 ] = { Key::E  , KeyEvent::Press };
			m_KbData.Keybinds[KeybindName::GizmoScale	 ] = { Key::R  , KeyEvent::Press };
		}

		/*void SaveControls()
		{
			std::ofstream ofs(CONTROLS_FILE_PATH);
			ASSERT(ofs.is_open() && ofs.good(), "Open file failed.");

			cereal::JSONOutputArchive oa(ofs);

			oa& m_KbData;
		}

		void LoadControls()
		{
			std::ifstream ifs(CONTROLS_FILE_PATH);
			ASSERT(ifs.is_open() && ifs.good(), "Open file failed.");

			cereal::JSONInputArchive ia(ifs);

			ia& m_KbData;
		}*/

		bool IsKeyDown(Key key)
		{
			return glfwGetKey(Window::Handle(), static_cast<int>(key)) == GLFW_PRESS;
		}

		bool IsMouseButtonDown(MouseButton btn)
		{
			return glfwGetMouseButton(Window::Handle(), (int)btn) == GLFW_PRESS;
		}
		void ProcessInput(int key, int action)
		{
			for (auto& [name, kb] : m_KbData.Keybinds)
			{
				if (key == (int)kb.GlId() && action == (int)kb.GlType())
					kb.Exec();
			}
		}

	

		void UIDisplayControlsConfig(bool* p_open, ImGuiWindowFlags panelFlags)
		{
			ImGui::Begin("Controls", p_open, panelFlags);
			{
				for (auto& [name, kb] : m_KbData.Keybinds)
				{
					ImGui::Text("%-16s: %s", magic_enum::enum_name(name).data(),
						magic_enum::enum_name(kb.GlId()).data());
				}
				ImGui::Separator(); ImGui::Separator();
				ImGui::Text("%-16s: %s", "Zoom In/Out", "Scroll mouse wheel");
				ImGui::Text("%-16s: %s", "Rotate View", "Hold RMB/LeftAlt + Move cursor");
				ImGui::Text("%-16s: %s", "Pan View", "Hold MMB + Move cursor");
				ImGui::Text("%-16s: %s", "Select Objects", "LMB click");
			}
			ImGui::End();
		}
	}
}