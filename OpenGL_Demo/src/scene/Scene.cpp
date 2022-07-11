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

void Scene::OnUpdate(float deltaTime)
{
	auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
	for (auto entity : group)
	{
		auto& [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
		if (mesh.HasTextures)
			Renderer::Draw(transform, mesh.Mesh_->Vao(), mesh.Mesh_->Diffuse(), mesh.Mesh_->Specular());
		else
			Renderer::Draw(transform, mesh.Mesh_->Vao(), mesh.Color);
	}

	Renderer::DrawSkybox();
}

