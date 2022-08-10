#pragma once
#include "OpenGL_Demo.h"

class SceneHierarchyPanel
{
public:
	SceneHierarchyPanel() = default;
	SceneHierarchyPanel(const Ref<Scene>& scene);

	void SetContext(const Ref<Scene>& scene);

	Entity GetSelectedEntity() const { return m_SelectionContext; }

	void OnImGuiRender();
private:
	void DrawEntityNode(Entity entity);
	void DrawComponents(Entity entity);
private:
	Ref<Scene> m_Context;
	Entity m_SelectionContext;
};