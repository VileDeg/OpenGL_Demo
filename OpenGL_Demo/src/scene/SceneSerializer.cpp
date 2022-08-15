#include "pch.h"
#include "SceneSerializer.h"
#include "scene/Component.h"
#include "scene/Entity.h"

#include <fstream>
#include <cereal/types/vector.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/json.hpp>

#include "renderer/MeshManager.h"

using namespace Component;

#if 0
namespace cereal
{
#if 1
	template<typename Archive>
	static void serialize(Archive& ar, glm::quat& quat)
	{
		ar& cereal::make_nvp("X", quat.x);
		ar& cereal::make_nvp("Y", quat.y);
		ar& cereal::make_nvp("Z", quat.z);
		ar& cereal::make_nvp("W", quat.w);
	}

	template<typename Archive>
	static void serialize(Archive& ar, glm::vec2& vec)
	{
		ar& cereal::make_nvp("X", vec.x);
		ar& cereal::make_nvp("Y", vec.y);
	}

	template<typename Archive>
	static void serialize(Archive& ar, glm::vec3& vec)
	{
		ar& cereal::make_nvp("X", vec.x);
		ar& cereal::make_nvp("Y", vec.y);
		ar& cereal::make_nvp("Z", vec.z);
	}

	template<typename Archive>
	static void serialize(Archive& ar, glm::vec4& vec)
	{
		ar& cereal::make_nvp("X", vec.x);
		ar& cereal::make_nvp("Y", vec.y);
		ar& cereal::make_nvp("Z", vec.z);
		ar& cereal::make_nvp("W", vec.w);
	}
#endif

#if 1
	template<typename Archive>
	static void serialize(Archive& ar, Transform& tr)
	{
		ar& cereal::make_nvp("Pos", tr.Position);
		ar& cereal::make_nvp("Quat", tr.Quaternion);
		ar& cereal::make_nvp("EulerAngles", tr.EulerAngles);
		ar& cereal::make_nvp("Scale", tr.Scale);
	}

	template<typename Archive>
	static void serialize(Archive& ar, LightData& l)
	{
		ar& cereal::make_nvp("Position", l.position);
		ar& cereal::make_nvp("Constant", l.constant);
		ar& cereal::make_nvp("Direction", l.direction);
		ar& cereal::make_nvp("Linear", l.linear);
		ar& cereal::make_nvp("Ambient", l.ambient);
		ar& cereal::make_nvp("Quadratic", l.quadratic);
		ar& cereal::make_nvp("Diffuse", l.diffuse);
		ar& cereal::make_nvp("CutOff", l.cutOff);
		ar& cereal::make_nvp("Specular", l.specular);
		ar& cereal::make_nvp("OuterCutOff", l.outerCutOff);
		ar& cereal::make_nvp("Type", l.type);
	}

	template<typename Archive>
	static void serialize(Archive& ar, Light& l)
	{
		ar& cereal::make_nvp("LightData", l.Data);
		ar& cereal::make_nvp("IsDynamic", l.IsDynamic);
		ar& cereal::make_nvp("SSBOIndex", l.SSBOindex);
	}
#endif

	
	template<typename Archive>
	void serialize(Archive& ar, Mesh::Vertex& vertex)
	{
		ar & cereal::make_nvp("Position", vertex.Position);
		ar & cereal::make_nvp("Normal", vertex.Normal);
		ar & cereal::make_nvp("TexCoords", vertex.TexCoords);
		ar & cereal::make_nvp("Tangent", vertex.Tangent);
		ar & cereal::make_nvp("Bitangent", vertex.Bitangent);
		ar & cereal::make_nvp("Color", vertex.Color);
		ar & cereal::make_nvp("m_BoneIDs", vertex.m_BoneIDs);
		ar & cereal::make_nvp("m_Weights", vertex.m_Weights);
	}

	template<typename Archive>
	void serialize(Archive& ar, Mesh::PrimitiveData& data)
	{
		ar & cereal::make_nvp("PrimitiveType", data.primType);
		ar & cereal::make_nvp("Textures", data.textures);
	}

	template<class Archive>
	void save(Archive& ar, const MeshInstance& pmesh)
	{
		ar& cereal::make_nvp("HasTextures", pmesh.HasTextures);
		ar& cereal::make_nvp("Color", pmesh.Color);
		ar& cereal::make_nvp("MeshData",
			MeshManager::GetPrimitiveMeshData(pmesh.PMesh));
	}

	template<class Archive>
	void load(Archive& ar, MeshInstance& pmesh)
	{
		ar& pmesh.HasTextures;
		ar& pmesh.Color;
		Mesh::PrimitiveData pData;
		ar& pData;
		pmesh.PMesh = MeshManager::GetPrimitiveMesh(pData);
	}

	template<typename Archive>
	void serialize(Archive& ar, Mesh::ModelData& data)
	{
		ar& cereal::make_nvp("Vertices", data.vertices);
		ar& cereal::make_nvp("Indices", data.indices);
		ar& cereal::make_nvp("Textures", data.textures);
	}

	template<class Archive>
	void save(Archive& ar, const Model& model)
	{
		ar& cereal::make_nvp("NumberOfMeshes", model.Meshes.size());
		for (size_t i = 0; i < model.Meshes.size(); ++i)
		{
			ar & cereal::make_nvp("MeshData"+std::to_string(i), 
				MeshManager::GetModelMeshData(model.Meshes[i]));
		}
	}

	template<class Archive>
	void load(Archive& ar, Model& model)
	{
		size_t numfMeshes = 0;
		ar& numfMeshes;
		model.Meshes.resize(numfMeshes);
		for (size_t i = 0; i < numfMeshes; ++i)
		{
			Mesh::ModelData mData;
			ar & mData;
			model.Meshes[i] = MeshManager::GetModelMesh(mData);
		}
	}
}
#endif

#if 1
void SceneSerializer::SaveScene(const std::string& filePath) {}
void SceneSerializer::LoadScene(const std::string& filePath) {}
#endif

#if 0
void SceneSerializer::SaveScene(const std::string& filePath)
{
	std::ofstream ofs(filePath);
	ASSERT(ofs.is_open() && ofs.good(), "Open file failed.");

	cereal::JSONOutputArchive oa(ofs);

	oa& cereal::make_nvp("NumberOfEntities", m_Scene->m_NumOfEntities);

	unsigned noTagCount = 0;
	m_Scene->m_Registry.each([&](auto entityID)
		{
			Entity entity = { entityID, m_Scene.get() };
			if (!entity)
				return;

			std::string name;
			if (entity.HasComponent<Tag>())
				name = entity.GetComponent<Tag>().String;
			else
			{
				name = "NoTagEntity" + std::to_string(noTagCount);
				noTagCount++;
			}

			oa& cereal::make_nvp(name, entity);
		});
}

void SceneSerializer::LoadScene(const std::string& filePath)
{
	std::ifstream ifs(filePath);
	ASSERT(ifs.is_open() && ifs.good(), "Open file failed.");

	cereal::JSONInputArchive ia(ifs);

	Renderer::ClearState();
	MeshManager::Clear();
	m_Scene->m_Registry.clear();

	size_t entity_count{};
	ia & entity_count;
	for (size_t i = 0; i < entity_count; ++i)
	{
		Entity entity = m_Scene->CreateEntity();
		ia & entity;
	}
}
#endif