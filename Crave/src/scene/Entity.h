#pragma once

//#include "Scene.h"
#include "entt.hpp"
#include <cereal/access.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/bitset.hpp>

namespace Crave
{
	class Scene;

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene)
			: m_EntityHandle(handle), m_Scene(scene) {}
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

		bool operator==(const entt::entity& rhs) const
		{
			return m_EntityHandle == rhs;
		}
		bool operator!=(const entt::entity& rhs) const
		{
			return !(*this == rhs);
		}

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;

	private:
		friend class cereal::access;

		template<class Archive>
		void save(Archive& ar) const
		{
			std::bitset<Component::NUMBER_OF_COMPONENTS> config(0b0);

			config[0] = HasComponent<Tag>();
			config[1] = HasComponent<Transform>();
			config[2] = HasComponent<PrimitiveMesh>();
			config[3] = HasComponent<Model>();
			config[4] = HasComponent<Light>();

			ar& cereal::make_nvp("ComponentConfiguration", config.to_string());

			if (HasComponent<Tag>())
			{
				ar& cereal::make_nvp("Tag", GetComponent<Tag>().String);
			}
			if (HasComponent<Transform>())
			{
				ar& cereal::make_nvp("Transform", GetComponent<Transform>());
			}
			if (HasComponent<PrimitiveMesh>())
			{
				ar& cereal::make_nvp("PrimitiveMesh", GetComponent<PrimitiveMesh>());
			}
			if (HasComponent<Model>())
			{
				ar& cereal::make_nvp("Model", GetComponent<Model>());
			}
			if (HasComponent<Light>())
			{
				ar& cereal::make_nvp("Light", GetComponent<Light>());
			}
		}
		template<typename Archive>
		void load(Archive& ar)
		{
			std::string str{};

			ar& str;

			std::bitset<Component::NUMBER_OF_COMPONENTS> config(str);
			if (config[0])
			{
				if (!HasComponent<Tag>())
				{
					AddComponent<Tag>();
				}
				ar& GetComponent<Tag>().String;
			}
			if (config[1])
			{
				if (!HasComponent<Transform>())
				{
					AddComponent<Transform>();
				}
				ar& GetComponent<Transform>();
			}
			if (config[2])
			{
				ar& AddComponent<PrimitiveMesh>();
			}
			if (config[3])
			{
				ar& AddComponent<Model>();
			}
			if (config[4])
			{
				ar& AddComponent<Light>();
			}
		}
	};

}