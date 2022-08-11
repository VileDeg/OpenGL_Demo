#pragma once
#include "scene/Scene.h"

class SceneSerializer
{
public:
	SceneSerializer(Ref<Scene> scene)
		: m_Scene(scene) {}
	~SceneSerializer() {}

	void SaveScene(const std::string& filePath);
	void LoadScene(const std::string& filePath);
private:
	Ref<Scene> m_Scene;
};