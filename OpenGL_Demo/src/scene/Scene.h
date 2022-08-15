#pragma once

#include "entt.hpp"
#include "scene/Entity.h"
#include "import/Model.h"
#include "scene/Component.h"

class Scene
{
protected:
	typedef int ImGuiWindowFlags;
public:
	Scene();
	virtual ~Scene() = 0;

	const Component::Transform& GetTransformComponent(entt::entity entity);

	Entity CreateEntity(const std::string& name = std::string());
	void DestroyEntity(Entity entity);

	void ImportModel(const std::string& path);

	Entity SelectedEntity() const { return m_SelectedEntity; }
	void SelectEntity(Entity entity) { m_SelectedEntity = entity; }
	
	virtual void OnUpdate(float deltaTime);
	virtual void OnImGuiRender(ImGuiWindowFlags panelFlags) = 0;
private:
	Component::Transform* Scene::PocessNodeData(const Import::Model::NodeData& nd, Component::Transform* parent);

	void RenderScene();
	void RenderSceneDepth();
	void RenderShadow();
protected:
	entt::registry m_Registry{};
	size_t m_NumOfEntities{};
	Entity m_SelectedEntity;
	std::vector<Import::Model> m_ImportedModels;

	friend class Entity;
	friend class SceneHierarchyPanel;
	friend class SceneSerializer;
};