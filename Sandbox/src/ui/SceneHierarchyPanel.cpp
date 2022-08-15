#include "pch.h"
#include "SceneHierarchyPanel.h"
#include <imgui/imgui_internal.h>
#include <magic_enum/magic_enum.hpp>

using namespace Component;

SceneHierarchyPanel::SceneHierarchyPanel(Ref<Scene> context)
{
	SetContext(context);
}

void SceneHierarchyPanel::SetContext(Ref<Scene> scene)
{
	m_Scene = scene;
}

void SceneHierarchyPanel::OnImGuiRender(ImGuiWindowFlags panelFlags)
{
	
	ImGui::Begin("Scene Hierarchy", (bool*)0, panelFlags);

	m_Scene->m_Registry.each([&](auto entityID)
		{
			Entity entity{ entityID , m_Scene.get() };
			if (entity.GetComponent<Transform>().Parent == nullptr)
				DrawEntityNode(entity);
		});

	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		m_Scene->m_SelectedEntity = {};

	// Right-click on blank space
	if (ImGui::BeginPopupContextWindow(0, 1, false))
	{
		if (ImGui::MenuItem("Create Empty Entity"))
			m_Scene->CreateEntity("Empty Entity");

		ImGui::EndPopup();	
	}

	ImGui::End();

	ImGui::Begin("Inspector", (bool*)0, panelFlags);
	if (m_Scene->m_SelectedEntity)
		DrawComponents(m_Scene->m_SelectedEntity);

	ImGui::End();
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity)
{
	auto& tag = entity.GetComponent<Tag>().String;

	ImGuiTreeNodeFlags flags = 
		((m_Scene->m_SelectedEntity == entity) ?
			ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
	if (ImGui::IsItemClicked())
	{
		m_Scene->m_SelectedEntity = entity;
	}

	bool entityDeleted = false; 
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Delete Entity"))
			entityDeleted = true;

		ImGui::EndPopup();
	}

	if (opened)
	{
		auto& tr = entity.GetComponent<Transform>();
		for (auto& child : tr.Children)
		{
			//Entity childEnt{ child->Entity, m_Scene.get()};
			DrawEntityNode(child->Entity);
			/*auto& tag = childEnt.GetComponent<Tag>().String;
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)childEnt, flags, tag.c_str());
			if (opened)
			{
				DrawEntityNode
				ImGui::TreePop();
			}*/
			
		}
		ImGui::TreePop();
	}

	if (entityDeleted)
	{
		m_Scene->DestroyEntity(entity);
		if (m_Scene->m_SelectedEntity == entity)
			m_Scene->m_SelectedEntity = {};
	}
}

static bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
{
	//Function code was taken unmodified from repository
	//https://github.com/TheCherno/Hazel
	//For license see vendor\Third-party_licenses\Hazel\LICENSE
	bool wasUsed = false;
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
	{
		values.x = resetValue;
		wasUsed = true;
	}
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	if (ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f"))
		wasUsed = true;
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
	{
		values.y = resetValue;
		wasUsed = true;
	}
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	if (ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f"))
		wasUsed = true;
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
	{
		values.z = resetValue;
		wasUsed = true;
	}
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	if (ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f"))
		wasUsed = true;
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
	return wasUsed;
}

template<typename T, typename UIFunction>
static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
{
	//Modified function code was taken from repository
	//https://github.com/TheCherno/Hazel
	//For license see vendor\Third-party_licenses\Hazel\LICENSE
	const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed
		| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];
	if (entity.HasComponent<T>())
	{
		auto& component = entity.GetComponent<T>();
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
		ImGui::PopStyleVar();
		ImGui::SameLine(contentRegionAvailable.x - lineHeight*0.45f);
		ImGui::PushFont(boldFont);
		if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
		{
			ImGui::OpenPopup("ComponentSettings");
		}
		ImGui::PopFont();

		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem("Remove component"))
				removeComponent = true;

			ImGui::EndPopup();
		}

		if (open)
		{
			uiFunction(component);
			ImGui::TreePop();
		}

		if (removeComponent)
			entity.RemoveComponent<T>();
	}
}

void SceneHierarchyPanel::DrawComponents(Entity entity)
{
	if (entity.HasComponent<Tag>())
	{
		auto& tag = entity.GetComponent<Tag>().String;

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), tag.c_str());
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
		{
			tag = std::string(buffer);
		}
	}

	ImGui::SameLine();
	ImGui::PushItemWidth(-1);

	if (ImGui::Button("Add Component"))
		ImGui::OpenPopup("AddComponent");

	if (ImGui::BeginPopup("AddComponent"))
	{
		

		ImGui::EndPopup();
	}

	ImGui::PopItemWidth();

	DrawComponent<Transform>("Transform", entity, [](auto& tr)
		{
			DrawVec3Control("Position", tr.Position);
			if (DrawVec3Control("Rotation", tr.EulerAngles))
				tr.Quaternion = glm::quat(glm::radians(tr.EulerAngles));

			DrawVec3Control("Scale", tr.Scale, 1.0f);
		});

	DrawComponent<MeshInstance>("MeshInstance", entity, [](auto& mi)
		{
			ImGui::Checkbox("HasTextures", &mi.HasTextures);
			ImGui::ColorEdit4("Color", glm::value_ptr(mi.Color));
			for (auto& [texType, texVector] : mi.PMesh->Textures())
			{
				if (texVector.empty())
					continue;
				ImGui::Text(magic_enum::enum_name(texType).data());
				for (auto& tex : texVector)
				{
					ImGui::Image((void*)tex->Id(), ImVec2{128, 128}, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
				}
			}
		});
}