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
	auto& view = m_Registry.view<TransformComponent, MeshComponent>();
	for (auto& entity : view)
	{
		auto& [transform, mesh] = view.get(entity);
		if (mesh.FlipNormals)
		{
			Renderer::DrawInside(transform, mesh.Mesh_->Vao(),
				mesh.Mesh_->Diffuse(), mesh.Mesh_->Specular());
			continue;
		}
		if (mesh.HasTextures)
			Renderer::Draw(transform, mesh.Mesh_->Vao(),
				mesh.Mesh_->Diffuse(), mesh.Mesh_->Specular());
		else
			Renderer::Draw(transform, mesh.Mesh_->Vao(), mesh.Color);
	}
}

void Scene::RenderSceneDepth()
{
	auto& view = m_Registry.view<TransformComponent, MeshComponent>();
	for (auto& entity : view)
	{
		auto& [transform, mesh] = view.get(entity);
		if (mesh.FlipNormals)
		{
			Renderer::DrawDepthInside(transform, mesh.Mesh_->Vao());
			continue;
		}
		Renderer::DrawDepth(transform, mesh.Mesh_->Vao());
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

		switch (light.Type())
		{
			case LightType::Directional:
				Renderer::DirDepthRenderSetup  (transform.Position(), light.Index());
				break;
			case LightType::Spot:
				Renderer::SpotDepthRenderSetup (transform.Position(), light.Index());
				break;
			case LightType::Point:
				Renderer::PointDepthRenderSetup(transform.Position(), light.Index());
				break;
		}
		
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

