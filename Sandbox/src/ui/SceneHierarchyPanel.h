#pragma once
#include "OpenGL_Demo.h"

class SceneHierarchyPanel
{
public:
	SceneHierarchyPanel() = default;
	SceneHierarchyPanel(Ref<Scene> scene);

	void SetContext(Ref<Scene> scene);

	//Entity GetSelectedEntity() const { return m_SelectedEntity; }
	
	void OnImGuiRender(ImGuiWindowFlags panelFlags);
private:
	void DrawEntityNode(Entity entity);
	void DrawComponents(Entity entity);
private:
	Ref<Scene> m_Scene;
	//Entity m_SelectedEntity;
};