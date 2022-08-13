#include "pch.h"
#include "Scene.h"
#include "Components.h"
#include <glm/glm.hpp>
#include "Entity.h"
#include "renderer/Renderer.h"
#include <glad/glad.h>

Scene::Scene() {}

Scene::~Scene() {}

Entity Scene::CreateEntity(const std::string& name)
{
	m_NumOfEntities++;

	Entity entity = { m_Registry.create(), this };
	entity.AddComponent<TransformComponent>();
	auto& tag = entity.AddComponent<TagComponent>();
	tag.Tag = name.empty() ? "Entity" : name;

	return entity;
}

void Scene::DestroyEntity(Entity entity)
{
	m_NumOfEntities--;
	m_Registry.destroy(entity);
}

void Scene::RenderScene()
{
	auto& group = m_Registry.group<TransformComponent, ModelComponent, TagComponent>();

	for (auto& entity : group)
	{
		auto& [transform, model, tag] = group.get(entity);
		/*if (tag.Tag != "Cube0")
			continue;*/
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
		auto& [transform, model, tag] = group.get(m_SelectedEntity);
		model.DrawOutlined((int)(entt::entity)m_SelectedEntity, transform);
	}
}

void Scene::RenderSceneDepth()
{
	auto& view = m_Registry.view<TransformComponent, ModelComponent, TagComponent>();
	for (auto& entity : view)
	{
		auto& [transform, model, tag] = view.get(entity);
		for (auto& m : model.mis)
			Renderer::DrawDepth(transform, m);
	}
}

void Scene::RenderShadow()
{
	auto& view = m_Registry.view<TransformComponent, LightComponent>();
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

