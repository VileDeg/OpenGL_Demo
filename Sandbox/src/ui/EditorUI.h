#pragma once
#include "SceneHierarchyPanel.h"

namespace EditorUI
{
	void Init(Ref<Framebuffer> viewportFB, Ref<Camera> camera, Ref<Scene> activeScene);
	void Render();
};