#include "pch.h"
#include "SceneSerializer.h"
#include "scene/Components.h"
#include "scene/Entity.h"

#include <fstream>
#include <cereal/types/vector.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/json.hpp>

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
	static void serialize(Archive& ar, TransformComponent& tr)
	{
		ar & cereal::make_nvp("Pos", tr.Position);
		ar & cereal::make_nvp("Quat", tr.Quaternion);
		ar & cereal::make_nvp("Scale", tr.Scale);
	}

	template<typename Archive>
	static void serialize(Archive& ar, MeshInstance& mi)
	{
		ar & cereal::make_nvp("Mesh", mi.mesh);
		ar & cereal::make_nvp("HasTextures", mi.HasTextures);
		ar & cereal::make_nvp("NormalsOut", mi.NormalsOut);
		ar & cereal::make_nvp("Color", mi.Color);
	}

	template<typename Archive>
	static void serialize(Archive& ar, ModelComponent& model)
	{
		ar & cereal::make_nvp("MeshInstances", model.mis);
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

	m_Scene->m_Registry.clear();
	size_t entity_count{};
	ia & entity_count;
	for (size_t i = 0; i < entity_count; ++i)
	{
		Entity entity = m_Scene->CreateEntity();
		ia & entity;
	}
}