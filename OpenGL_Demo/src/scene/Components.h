#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "renderer/Renderer.h"
#include "renderer/Mesh.h"
#include "renderer/Model.h"

namespace Component
{
	constexpr const int NUMBER_OF_COMPONENTS = 5;


	struct Tag
	{
		std::string String;

		Tag() = default;
		Tag(const Tag&) = default;
		Tag(const std::string& tag)
			: String(tag) {}
	};

	struct Transform
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::quat Quaternion{};
		glm::vec3 EulerAngles{};
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		void ScaleF(const float units)
		{
			Scale = glm::vec3(units);
		}

		void RotateTo(const float angle, const glm::vec3& axis)
		{
			Quaternion = glm::angleAxis(glm::radians(angle), axis);
			EulerAngles = glm::degrees(glm::eulerAngles(Quaternion));
		}

		void RotateTo(const glm::vec3& eulerAngles)
		{
			EulerAngles = eulerAngles;
			Quaternion = glm::quat(glm::radians(eulerAngles));
		}

		void RotateAroundPoint(const glm::vec3& point, const float angle, const glm::vec3& axis)
		{
			glm::vec3 iniPos = Position - point;
			glm::quat quatRot = glm::angleAxis(glm::radians(angle), glm::normalize(axis));
			glm::mat4 rotMat = glm::toMat4(quatRot);
			Position = glm::vec3(rotMat * glm::vec4(iniPos, 1.f)) + point;
		}

		glm::mat4 GetTransform() const
		{
			return glm::translate(glm::mat4(1.0f), Position)
				* glm::toMat4(Quaternion)
				* glm::scale(glm::mat4(1.0f), Scale);
		}

		Transform() = default;
		Transform(const Transform&) = default;
		Transform(const glm::vec3& pos)
			: Position(pos) {}

		operator glm::mat4() { return GetTransform(); }
		operator const glm::mat4() const { return GetTransform(); }
	};

	struct PrimitiveMesh
	{
		Ref<Mesh> PMesh{};
		bool HasTextures{ true };
		//bool NormalsOut { true };
		glm::vec4 Color{ 1.f, 0.f, 1.f, 1.f }; //magenta

		void Draw(int drawID, const glm::mat4& modelMat)
		{
			Renderer::DrawMesh(drawID, modelMat, PMesh, HasTextures, Color);
		}

		void DrawOutlined(int drawID, const glm::mat4& modelMat)
		{
			
			Renderer::DrawOutlined(drawID, modelMat, PMesh, HasTextures, Color);
			
		}

		PrimitiveMesh() = default;
		PrimitiveMesh(const PrimitiveMesh&) = default;
		PrimitiveMesh(Ref<Mesh> mesh, bool hasTex = true)
			: PMesh(mesh), HasTextures(hasTex)
		{
		}
	};

	struct Model
	{
		std::vector<Ref<Mesh>> Meshes;

		void Draw(int drawID, const glm::mat4& modelMat)
		{
			for (auto& mesh : Meshes)
			{
				Renderer::DrawMesh(drawID, modelMat, mesh, true);
			}
		}

		void DrawOutlined(int drawID, const glm::mat4& modelMat)
		{
			for (auto& mesh : Meshes)
			{
				Renderer::DrawOutlined(drawID, modelMat, mesh, true);
			}
		}

		Model() = default;
		Model(AssimpImport::Model& model, bool hasTex = true, bool normOut = true)
		{
			for (auto& mesh : model.Meshes())
				Meshes.push_back(mesh);
		}
		Model(const std::string& path, bool hasTex = true, bool normOut = true)
		{
			AssimpImport::Model model{ path };
			for (auto& mesh : model.Meshes())
				Meshes.push_back(mesh);
		}
		Model(const Model& mc) : Meshes(mc.Meshes) {}
	/*	Model(Model&& mc) : Meshes(mc.Meshes) {}

		Model& operator=(Model& mc)
		{
			if (this == &mc)
				return *this;
			Meshes = mc.Meshes;
			return *this;
		};*/

		//operator std::vector<MeshInstance>() { return Meshes; }
		//operator const std::vector<MeshInstance>() const { return Meshes; }
	};

	struct Light
	{
		LightData Data;
		bool	  IsDynamic{ false };
		unsigned  SSBOindex;

		void UpdatePosition(const glm::vec3& pos)
		{
			Renderer::UpdateLightPosition(glm::value_ptr(pos), SSBOindex);
		}

		Light() = default;
		Light(const LightData& lightData, bool isDynamic = false)
			: Data(lightData), IsDynamic(isDynamic)
		{
			SSBOindex = Renderer::UploadLightData(&Data);
		}
	};
}