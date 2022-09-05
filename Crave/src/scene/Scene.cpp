#include "pch.h"
#include "Scene.h"
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include "Entity.h"
#include "renderer/Renderer.h"
#include "import/Model.h"

namespace Crave
{
	using namespace Component;

	Scene::Scene()
	{
		m_NumOfEntities++;

		m_RootEntity = { m_Registry.create(), this };
		m_RootEntity.AddComponent<Transform>(m_RootEntity, Entity(entt::null, nullptr));
		m_RootEntity.AddComponent<Tag>("SCENE_ROOT");
	}

	Scene::~Scene() {}

	Entity Scene::GetEntity(entt::entity id)
	{
		Entity entity{ id, this };
		auto& tag = entity.AddComponent<Tag>();
		tag.String = "Id wrapper";
		return entity;
	}


	Entity Scene::CreateEntity(const std::string& name, bool hasParent, Entity parent)
	{
		m_NumOfEntities++;

		Entity entity = { m_Registry.create(), this };
		auto& tag = entity.AddComponent<Tag>();
		tag.String = name;

		Entity Parent = hasParent ? parent : m_RootEntity;
		Parent.GetComponent<Transform>().Children.push_back(entity);

		entity.AddComponent<Transform>(entity, Parent);

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_NumOfEntities--;
		m_Registry.destroy(entity);
	}

	Entity Scene::PocessNodeData(const Import::ModelNodeData& nd, Entity parent)
	{
		Entity entity = CreateEntity(nd.name, true, parent);
		entity.AddComponent<MeshInstance>(nd.meshes[0]);

		auto& tr = entity.GetComponent<Transform>();
		//tr.Parent = parent;
		//Transform* pTr = &entity.GetComponent<Transform>();

		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(nd.transform, scale, rotation, translation, skew, perspective);

		tr.Position = translation;
		tr.Scale = scale;
		tr.Quaternion = rotation;
		tr.EulerAngles = glm::degrees(glm::eulerAngles(rotation));

		for (auto& child : nd.childData)
		{
			PocessNodeData(child, entity);//tr.Children.push_back(
		}

		return entity;
	}

	Entity Scene::ImportModel(const std::string& path)
	{
		Import::Model m{ path };
		m_ImportedModels.push_back(m);

		return PocessNodeData(m.m_NodeData, m_RootEntity);
	}

	void Scene::RenderScene()
	{
		auto& meshView = m_Registry.view<Transform, MeshInstance, Tag>();

		for (auto& entity : meshView)
		{
			auto& [transform, mi, tag] = meshView.get(entity);
			//Find out if parent is a selected entity
			bool parentSelected = false;
			auto tmp = Entity(entity, this);
			if ((bool)m_SelectedEntity)
			{
				parentSelected = true;
				while (m_SelectedEntity != tmp)
				{
					auto tmpTr = tmp.GetComponent<Transform>();
					if (tmpTr.Parent.GetComponent<Tag>() == m_RootEntity.GetComponent<Tag>())
					{
						parentSelected = false;
						break;
					}
					tmp = tmpTr.Parent;
				}
			}
			if (!parentSelected)
			{
				mi.Draw((int)entity, transform);
			}
		}

		Renderer::DrawSkybox();

		//Draw selected entity and all children after all others to avoid if-checking every iteration
		//and to make selection outline appear on top of all objects.
		if ((bool)m_SelectedEntity)
		{
			std::queue<Entity> que{};
			
			auto& [transform, mi, tag] = meshView.get(m_SelectedEntity);
			
			for (auto& child : transform.Children)
				que.push(child);

			glm::vec4 blueColor = { 0.08f, 0.6f, 1.f, 1.f };
			glm::vec4 prevColor = Renderer::SetOutlineColor(blueColor);
		
			while (que.size() != 0)
			{
				auto& curr = que.front();
				auto& currTr = curr.GetComponent<Transform>();

				if (curr.HasComponent<MeshInstance>())
				{
					auto& currMi = curr.GetComponent<MeshInstance>();
					currMi.DrawOutlined((int)(entt::entity)curr, currTr);
				}
				for (auto& child : currTr.Children)
					que.push(child);

				que.pop();
			}
			
			Renderer::SetOutlineColor(prevColor);

			if (m_SelectedEntity.HasComponent<MeshInstance>())
				mi.DrawOutlined((int)(entt::entity)m_SelectedEntity, transform);
		}
	}

	void Scene::RenderSceneDepth(ShaderType shType)
	{
		auto& meshView = m_Registry.view<Transform, MeshInstance>();
		for (auto& entity : meshView)
		{
			auto& [transform, mi] = meshView.get(entity);
			Renderer::DrawDepth(transform, mi.PMesh, shType);
		}
	}

	void Scene::RenderShadow()
	{
		int frameNum = 0;
		auto& view = m_Registry.view<Transform, Light>();
		for (auto& entity : view)
		{
			auto& [transform, light] = view.get(entity);

			//Update dynamic light position
			if (light.IsDynamic)
				light.UpdatePosition(transform.Position);

			ShaderType shType;
			switch (light.Data.type)
			{
			case LightType::Point:
				
				shType = ShaderType::PointDepth;
				Renderer::PointShadowSetup(transform.Position, light.SSBOindex, 0);
				//frameNum += 6;
				break;
			case LightType::Directional:
				/*Renderer::DirShadowSetup(transform.Position);
				shType = ShaderType::DirDepth;
				break;*/
			case LightType::Spot:
			default:
				ASSERT(false, "");
			}
			RenderSceneDepth(shType);
			

			Renderer::ShadowRenderEnd();
		}
	}

	void Scene::OnUpdate(float deltaTime)
	{
		RenderShadow();
		RenderScene();
	}

}