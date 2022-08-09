#pragma once

#include "entt.hpp"

class Entity;

class Scene
{
public:
	Scene();
	virtual ~Scene() = 0;

	Entity CreateEntity(const std::string& name = std::string());
	void DestroyEntity(Entity entity);
	
	virtual void OnUpdate(float deltaTime);
	virtual void OnImGuiRender() = 0;
private:
	void RenderScene();
	void RenderSceneDepth();
	void RenderShadow();
protected:
	entt::registry m_Registry;

	friend class Entity;
	friend class SceneHierarchyPanel;
	friend class SceneSerializer;
};