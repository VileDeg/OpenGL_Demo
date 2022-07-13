#pragma once

#include <glm/glm.hpp>
#include "renderer/Renderer.h"
#include "renderer/Mesh.h"


struct TagComponent
{
	std::string Tag;

	TagComponent() = default;
	TagComponent(const TagComponent&) = default;
	TagComponent(const std::string& tag)
		: Tag(tag) {}
};

struct TransformComponent
{
	glm::mat4 Transform{ 1.0f };
	float Scale{ 1.f };

	void TranslateTo(const glm::vec3& pos)
	{
		Transform[3] = glm::vec4(pos, 1.f);
	}
	
	void ScaleTo(const float units)
	{
		Transform = glm::scale(Transform, glm::vec3(1.f / Scale));
		Transform = glm::scale(Transform, glm::vec3(units));
	}

	const glm::vec3& Position() const
	{
		return Transform[3];
	}

	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent(const glm::mat4& transform)
		: Transform(transform) {}

	operator glm::mat4& () { return Transform; }
	operator const glm::mat4& () const { return Transform; }
};

struct MeshComponent
{
	Mesh* Mesh_{nullptr};
	bool  HasTextures{true};
	bool  FlipNormals{ false };
	glm::vec4 Color{1.f, 0.f, 1.f, 1.f}; //magenta

	MeshComponent() = default;
	MeshComponent(const MeshComponent&) = default;
	MeshComponent(Mesh* mesh)
		: Mesh_(mesh) {}

	/*operator Mesh& () { return Mesh_; }
	operator const Mesh& () const { return Mesh_; }*/
};

struct LightComponent
{
	Light Data;
	bool  IsDynamic{ false };
	unsigned SSBOindex;

	void UpdatePosition(const glm::vec3& pos)
	{
		Renderer::UpdateLightPosition(glm::value_ptr(pos), SSBOindex);
	}

	LightComponent() = default;
	LightComponent(const Light& lightData, bool isDynamic = false)
		: Data(lightData), IsDynamic(isDynamic)
	{
		SSBOindex = Renderer::UploadLightData(&Data);
	}
};