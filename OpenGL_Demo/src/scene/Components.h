#pragma once

#include <glm/glm.hpp>
#include "renderer/Renderer.h"
#include "renderer/Mesh.h"
#include "renderer/Model.h"
#include "renderer/Light.h"

namespace Components
{
	constexpr const int NUMBER_OF_COMPONENTS = 3;
}

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
	glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
	glm::quat Quaternion{};
	glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

	void ScaleF(const float units)
	{
		Scale = glm::vec3(units);
	}

	void RotateTo(const float angle, const glm::vec3& axis)
	{
		Quaternion = glm::angleAxis(glm::radians(angle), axis);
	}

	void RotateTo(const glm::vec3& eulerAngles)
	{
		Quaternion = glm::quat(glm::radians(eulerAngles));
	}

	void RotateAroundPoint(const glm::vec3& point, const float angle, const glm::vec3& axis)
	{
		glm::vec3 iniPos = Position - point;
		glm::quat quatRot = glm::angleAxis(glm::radians(angle), glm::normalize(axis));
		glm::mat4 rotMat = glm::toMat4(quatRot);
		Position = glm::vec3(rotMat * glm::vec4(iniPos,1.f)) + point;
	}
		
	const glm::vec3& EulerAngles() const
	{
		return glm::eulerAngles(Quaternion);
	}

	glm::mat4 GetTransform() const
	{
		return glm::translate(glm::mat4(1.0f), Position)
			* glm::toMat4(Quaternion)
			* glm::scale(glm::mat4(1.0f), Scale);
	}

	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent(const glm::vec3& pos)
		: Position(pos) {}

	operator glm::mat4 () { return GetTransform(); }
	operator const glm::mat4 () const { return GetTransform(); }
};

struct ModelComponent
{
	std::vector<MeshInstance> mis;

	void Draw(const glm::mat4& modelMat)
	{
		for (auto& mesh : mis)
		{
			Renderer::Draw(modelMat, mesh);
		}
	}

	ModelComponent() = default;
	ModelComponent(Mesh& mesh, bool hasTex = true, bool normOut = true)
	{
		mis.push_back({ mesh, hasTex, normOut });
	}
	ModelComponent(Model model, bool hasTex = true, bool normOut = true)
	{
		for (auto& mesh : model.Meshes())
			mis.push_back({ mesh, hasTex, normOut });
	}
	ModelComponent(const ModelComponent& mc) : mis(mc.mis) {}
	ModelComponent(ModelComponent&& mc) : mis(mc.mis) {}

	ModelComponent& operator=(ModelComponent& mc)
	{ 
		if (this == &mc)
			return *this;
		mis = mc.mis;
		return *this;
	};

	operator std::vector<MeshInstance>() { return mis; }
	operator const std::vector<MeshInstance>() const { return mis; }
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