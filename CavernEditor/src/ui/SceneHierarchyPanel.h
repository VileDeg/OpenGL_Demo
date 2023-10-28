#pragma once
#include "Cavern.h"

namespace Crave
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(Ref<Scene> scene);

		void SetContext(Ref<Scene> scene);

		void OnImGuiRender(ImGuiWindowFlags panelFlags);
	private:
		void ReparentTransform(Entity dragged, Entity newParent);

		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

	private:
		Ref<Scene> m_Scene{};
	};
}