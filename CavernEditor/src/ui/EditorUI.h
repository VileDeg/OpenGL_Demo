#pragma once
#include "SceneHierarchyPanel.h"

namespace Crave
{
	namespace EditorUI
	{
		void Init(Ref<Framebuffer> viewportFB, Ref<Camera> camera, Ref<Scene> activeScene);
		void Render();
	}
}