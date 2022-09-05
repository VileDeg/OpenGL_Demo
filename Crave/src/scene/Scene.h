#pragma once

#include "entt.hpp"
#include "scene/Entity.h"
//#include "import/Model.h"
#include "scene/Component.h"

namespace Crave
{
	namespace Import { class Model; struct ModelNodeData; }
	
	//struct Import::Model::NodeData;

	class Scene
	{
	protected:
		typedef int ImGuiWindowFlags;
	public:
		Scene();
		virtual ~Scene() = 0;

		//const Component::Transform& GetTransformComponent(entt::entity entity);
		Entity GetEntity(entt::entity id);

		Entity CreateEntity(const std::string& name, bool hasParent = false, Entity parent = { entt::null, nullptr });
		void DestroyEntity(Entity entity);

		Entity ImportModel(const std::string& path);

		Entity SelectedEntity() const { return m_SelectedEntity; }
		void SelectEntity(Entity entity) { m_SelectedEntity = entity; }

		virtual void OnUpdate(float deltaTime);
		virtual void OnImGuiRender(ImGuiWindowFlags panelFlags) = 0;
	private:
		Entity Scene::PocessNodeData(const Import::ModelNodeData& nd, Entity parent);

		void RenderScene();
		void RenderSceneDepth(ShaderType shType);
		void RenderShadow();
	protected:
		entt::registry m_Registry{};
		size_t m_NumOfEntities{};
		Entity m_SelectedEntity{};
		Entity m_RootEntity{};
		//Component::Transform m_Root;
		std::vector<Import::Model> m_ImportedModels{};


		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};
}