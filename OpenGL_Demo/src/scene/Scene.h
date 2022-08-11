#pragma once

#include "entt.hpp"
#include "scene/Entity.h"

//class Entity;

class Scene
{
protected:
	typedef int ImGuiWindowFlags;
public:
	Scene();
	virtual ~Scene() = 0;

	Entity CreateEntity(const std::string& name = std::string());
	void DestroyEntity(Entity entity);
	//void SelectEntity(Entity entity);
	Entity SelectedEntity() const { return m_SelectedEntity; }
	void SelectEntity(Entity entity) { m_SelectedEntity = entity; }
	
	virtual void OnUpdate(float deltaTime);
	virtual void OnImGuiRender(ImGuiWindowFlags panelFlags) = 0;
private:
	void RenderScene();
	void RenderSceneDepth();
	void RenderShadow();
protected:
	entt::registry m_Registry{};
	size_t m_NumOfEntities{};
	Entity m_SelectedEntity;

	friend class Entity;
	friend class SceneHierarchyPanel;
	friend class SceneSerializer;
};