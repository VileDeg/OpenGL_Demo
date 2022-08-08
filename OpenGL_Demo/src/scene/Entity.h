#pragma once

#include "Scene.h"
#include "entt.hpp"
#include <cereal/access.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/bitset.hpp>

class Entity
{
public:
	Entity() = default;
	Entity(entt::entity handle, Scene* scene);
	Entity(const Entity& other) = default;

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		ASSERT(!HasComponent<T>(), "Entity already has component!");
		return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
	}

	template<typename T>
	T& GetComponent() const
	{
		ASSERT(HasComponent<T>(), "Entity does not have component!");
		return m_Scene->m_Registry.get<T>(m_EntityHandle);
	}

	template<typename T>
	bool HasComponent() const
	{
		return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
	}

	template<typename T>
	void RemoveComponent()
	{
		ASSERT(HasComponent<T>(), "Entity does not have component!");
		m_Scene->m_Registry.remove<T>(m_EntityHandle);
	}

	operator bool() const { return m_EntityHandle != entt::null; }
	operator entt::entity() const { return m_EntityHandle; }
	operator uint32_t() const { return (uint32_t)m_EntityHandle; }

	bool operator==(const Entity& other) const
	{
		return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
	}

	bool operator!=(const Entity& other) const
	{
		return !(*this == other);
	}
private:
	entt::entity m_EntityHandle{ entt::null };
	Scene* m_Scene = nullptr;

private:
	friend class cereal::access;

	template<class Archive>
	void save(Archive& ar) const
	{
		std::bitset<Components::NUMBER_OF_COMPONENTS> componentsSet{};
		if (HasComponent<TagComponent>())
			componentsSet[0] = 1;
		if (HasComponent<TransformComponent>())
			componentsSet[1] = 1;
		if (HasComponent<ModelComponent>())
			componentsSet[2] = 1;
		ar& cereal::make_nvp("ComponentConfiguration", componentsSet); //.to_string()

		if (HasComponent<TagComponent>())
		{
			ar & cereal::make_nvp("Tag", GetComponent<TagComponent>().Tag);
		}
		if (HasComponent<TransformComponent>())
		{
			ar & cereal::make_nvp("Transform", GetComponent<TransformComponent>());
		}
		if (HasComponent<ModelComponent>())
		{
			ar & cereal::make_nvp("Model", GetComponent<ModelComponent>());
		}
	}
	template<typename Archive>
	void load(Archive& ar)
	{
		std::bitset<Components::NUMBER_OF_COMPONENTS> componentsSet;
		ar & componentsSet; //.to_string()
		if (componentsSet[0])
		{
			if (!HasComponent<TagComponent>())
			{
				AddComponent<TagComponent>();
			}
			ar & GetComponent<TagComponent>().Tag;
		}
		if (componentsSet[1])
		{
			if (!HasComponent<TransformComponent>())
			{
				AddComponent<TransformComponent>();
			}
			ar & GetComponent<TransformComponent>();
		}
		if (componentsSet[2])
		{
			ar & AddComponent<ModelComponent>();
		}
	}
};


