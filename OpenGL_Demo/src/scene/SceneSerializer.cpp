#include "pch.h"
#include "SceneSerializer.h"
#include "scene/Components.h"
#include "scene/Entity.h"

#include <fstream>
#include <cereal/types/vector.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/json.hpp>

#include "renderer/MeshManager.h"

namespace cereal
{
	template<typename Archive>
	static void serialize(Archive& ar, glm::quat& quat)
	{
		ar& cereal::make_nvp("X", quat.x);
		ar& cereal::make_nvp("Y", quat.y);
		ar& cereal::make_nvp("Z", quat.z);
		ar& cereal::make_nvp("W", quat.w);
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

	template<typename Archive>
	static void serialize(Archive& ar, VertexLayout::VertexAttribute& attrib)
	{
		ar & cereal::make_nvp("Type", attrib.type);
		ar & cereal::make_nvp("Count", attrib.count);
		ar & cereal::make_nvp("Normalized", attrib.normalized);
	}

	template<typename Archive>
	void serialize(Archive& ar, MeshData& data)
	{
		ar & cereal::make_nvp("PrimitiveType", data.primType);
		ar & cereal::make_nvp("Textures", data.textures);
	}

	template<typename Archive>
	static void serialize(Archive& ar, TransformComponent& tr)
	{
		ar & cereal::make_nvp("Pos", tr.Position);
		ar & cereal::make_nvp("Quat", tr.Quaternion);
		ar & cereal::make_nvp("Scale", tr.Scale);
	}

	template<class Archive>
	void save(Archive& ar, const MeshInstance& mi)
	{
		ar & cereal::make_nvp("HasTextures", mi.HasTextures);
		ar & cereal::make_nvp("NormalsOut", mi.NormalsOut);
		ar & cereal::make_nvp("Color", mi.Color);
		ar & cereal::make_nvp("MeshData", MeshManager::GetMData(mi.mesh));
	}

	template<class Archive>
	void load(Archive& ar, MeshInstance& mi)
	{
		ar & mi.HasTextures;
		ar & mi.NormalsOut;
		ar & mi.Color;

		MeshData mData;
		ar & mData;
		mi.mesh = MeshManager::GetMesh(mData);
	}

	template<typename Archive>
	static void serialize(Archive& ar, ModelComponent& model)
	{
		ar & cereal::make_nvp("MeshInstances", model.mis);
	}

	template<typename Archive>
	static void serialize(Archive& ar, Light& l)
	{
		ar & cereal::make_nvp("Position", l.position);
		ar & cereal::make_nvp("Constant", l.constant);
		ar & cereal::make_nvp("Direction", l.direction);
		ar & cereal::make_nvp("Linear", l.linear);
		ar & cereal::make_nvp("Ambient", l.ambient);
		ar & cereal::make_nvp("Quadratic", l.quadratic);
		ar & cereal::make_nvp("Diffuse", l.diffuse);
		ar & cereal::make_nvp("CutOff", l.cutOff);
		ar & cereal::make_nvp("Specular", l.specular);
		ar & cereal::make_nvp("OuterCutOff", l.outerCutOff);
		ar & cereal::make_nvp("Type", l.type);
	}

	template<typename Archive>
	static void serialize(Archive& ar, LightComponent& l)
	{
		ar & cereal::make_nvp("LightData", l.Data);
		ar & cereal::make_nvp("IsDynamic", l.IsDynamic);
		ar & cereal::make_nvp("SSBOIndex", l.SSBOindex);
	}
}

//void SceneSerializer::SaveScene(const std::string& filePath) {}
//void SceneSerializer::LoadScene(const std::string& filePath) {}
void SceneSerializer::SaveScene(const std::string& filePath)
{
	std::ofstream ofs(filePath);
	ASSERT(ofs.is_open() && ofs.good(), "Open file failed.");

	cereal::JSONOutputArchive oa(ofs);

	oa& cereal::make_nvp("NumberOfEntities", m_Scene->m_Registry.size());

	unsigned noTagCount = 0;
	m_Scene->m_Registry.each([&](auto entityID)
		{
			Entity entity = { entityID, m_Scene.get() };
			if (!entity)
				return;

			std::string name;
			if (entity.HasComponent<TagComponent>())
				name = entity.GetComponent<TagComponent>().Tag;
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

	//m_Scene->m_Registry.clear();
	Renderer::ClearState();
	m_Scene->m_Registry = entt::registry();
	size_t entity_count{};
	ia & entity_count;
	for (size_t i = 0; i < entity_count; ++i)
	{
		Entity entity = m_Scene->CreateEntity();
		ia & entity;
	}
}