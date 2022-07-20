#include "pch.h"
#include "Scene.h"
#include "Components.h"
#include <glm/glm.hpp>
#include "Entity.h"
#include "renderer/Renderer.h"

Scene::Scene()
{

}

Scene::~Scene()
{
}

Entity Scene::CreateEntity(const std::string& name)
{
	Entity entity = { m_Registry.create(), this };
	entity.AddComponent<TransformComponent>();
	auto& tag = entity.AddComponent<TagComponent>();
	tag.Tag = name.empty() ? "Entity" : name;

	return entity;
}

void Scene::RenderScene()
{
	auto& view = m_Registry.view<TransformComponent, ModelComponent>();
	for (auto& entity : view)
	{
		auto& [transform, model] = view.get(entity);
		model.Draw(transform);
	}
}

void Scene::RenderSceneDepth()
{
	auto& view = m_Registry.view<TransformComponent, ModelComponent>();
	for (auto& entity : view)
	{
		auto& [transform, model] = view.get(entity);
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
			light.UpdatePosition(transform.Position());

		Renderer::ShadowRenderSetup(transform.Position());
		
		// render scene from light's point of view
		RenderSceneDepth();

		Renderer::ShadowRenderEnd();
	}
}

void Scene::OnUpdate(float deltaTime)
{
	RenderShadow();

	RenderScene();

	Renderer::DrawSkybox();
}

