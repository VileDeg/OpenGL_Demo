#include "pch.h"
#include "Scene.h"
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include "Entity.h"
#include "renderer/Renderer.h"
#include <glad/glad.h>

using namespace Component;

Scene::Scene() {}

Scene::~Scene() {}

const Transform& Scene::GetTransformComponent(entt::entity entity)
{
	ASSERT(m_Registry.any_of<Transform>(entity), "");
	return m_Registry.get<Transform>(entity);
}

Entity Scene::CreateEntity(const std::string& name)
{
	m_NumOfEntities++;

	Entity entity = { m_Registry.create(), this };
	entity.AddComponent<Transform>();
	auto& tag = entity.AddComponent<Tag>();
	tag.String = name.empty() ? "Entity" : name;

	return entity;
}

void Scene::DestroyEntity(Entity entity)
{
	m_NumOfEntities--;
	m_Registry.destroy(entity);
}

Transform* Scene::PocessNodeData(const Import::Model::NodeData& nd, Transform* parent)
{
	Entity entity = CreateEntity(nd.name);
	entity.AddComponent<MeshInstance>(nd.meshes[0]);

	auto& tr = entity.GetComponent<Transform>();
	tr.Entity = entity;
	Transform* pTr = &entity.GetComponent<Transform>();
	tr.Parent = parent;

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
		tr.Children.push_back(PocessNodeData(child, pTr));
	}

	return pTr;
}

void Scene::ImportModel(const std::string& path)
{
	Import::Model m{path};
	m_ImportedModels.push_back(m);
	
	PocessNodeData(m.m_NodeData, nullptr);
}

void Scene::RenderScene()
{
	auto& meshView = m_Registry.view<Transform, MeshInstance>();

	for (auto& entity : meshView)
	{
		auto& [transform, mi] = meshView.get(entity);
		if (m_SelectedEntity != entity)
		{
			mi.Draw((int)entity, transform);
		}
	}

	Renderer::DrawSkybox();

	//Draw selected entity after all others to avoid if-checking every iteration
	//and to make selection outline appear on top of all objects.
	if ((bool)m_SelectedEntity)
	{
		ASSERT(meshView.contains(m_SelectedEntity), "");
		auto& [transform, mi] = meshView.get(m_SelectedEntity);
		mi.DrawOutlined((int)(entt::entity)m_SelectedEntity, transform);
	}
}

void Scene::RenderSceneDepth()
{
	auto& meshView = m_Registry.view<Transform, MeshInstance>();
	for (auto& entity : meshView)
	{
		auto& [transform, mi] = meshView.get(entity);
		Renderer::DrawDepth(transform, mi.PMesh);
	}
}

void Scene::RenderShadow()
{
	auto& view = m_Registry.view<Transform, Light>();
	for (auto& entity : view)
	{
		auto& [transform, light] = view.get(entity);

		//Update dynamic light position
		if (light.IsDynamic)
			light.UpdatePosition(transform.Position);

		Renderer::ShadowRenderSetup(transform.Position);
		
		// render scene from light's point of view
		RenderSceneDepth();

		Renderer::ShadowRenderEnd();
	}
}

void Scene::OnUpdate(float deltaTime)
{
	//Renderer::Clear(GLBuffer::Color | GLBuffer::Depth | GLBuffer::Stencil);

	RenderShadow();

	RenderScene();
	
	
}

