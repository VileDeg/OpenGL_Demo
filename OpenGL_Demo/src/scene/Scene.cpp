#include "pch.h"
#include "Scene.h"
#include "Components.h"
#include <glm/glm.hpp>
#include "Entity.h"
#include "renderer/Renderer.h"
#include <glad/glad.h>

using namespace Component;

Scene::Scene() {}

Scene::~Scene() {}

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

void Scene::RenderScene()
{
	auto& modelGroup = m_Registry.view<Transform, Model, Tag>();

	for (auto& entity : modelGroup)
	{
		auto& [transform, model, tag] = modelGroup.get(entity);
		if (m_SelectedEntity != entity)
		{
			model.Draw((int)entity, transform);
		}
	}

	auto& pmeshGroup = m_Registry.view<Transform, PrimitiveMesh, Tag>();

	for (auto& entity : pmeshGroup)
	{
		auto& [transform, model, tag] = pmeshGroup.get(entity);
		if (m_SelectedEntity != entity)
		{
			model.Draw((int)entity, transform);
		}
	}

	Renderer::DrawSkybox();

	//Draw selected entity after all others to avoid if-checking every iteration
	//and to make selection outline appear on top of all objects.
	if ((bool)m_SelectedEntity)
	{
		if (modelGroup.contains(m_SelectedEntity))
		{
			auto& [transform, model, tag] = modelGroup.get(m_SelectedEntity);
			model.DrawOutlined((int)(entt::entity)m_SelectedEntity, transform);
		}
		else if (pmeshGroup.contains(m_SelectedEntity))
		{
			auto& [transform, pmesh, tag] = pmeshGroup.get(m_SelectedEntity);
			pmesh.DrawOutlined((int)(entt::entity)m_SelectedEntity, transform);
		}
	}
}

void Scene::RenderSceneDepth()
{
	auto& modelView = m_Registry.view<Transform, Model, Tag>();
	for (auto& entity : modelView)
	{
		auto& [transform, model, tag] = modelView.get(entity);
		for (auto& m : model.Meshes)
			Renderer::DrawDepth(transform, m);
	}

	auto& pmeshView = m_Registry.view<Transform, PrimitiveMesh, Tag>();
	for (auto& entity : pmeshView)
	{
		auto& [transform, pmesh, tag] = pmeshView.get(entity);
		Renderer::DrawDepth(transform, pmesh.PMesh);
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

