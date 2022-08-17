#pragma once
#include "pch.h"
#include <cereal/archives/json.hpp>
#include <cereal/access.hpp>

class Texture
{
public:
	enum class Type
	{
		None = -1, Diffuse, Specular, Normal, Height
	};
private:
	static constexpr const char* BASE_TEXTURE_PATH = "res/textures/";
	static constexpr const char* BASE_CUBEMAP_PATH = "res/textures/cubemaps/";
public:
	Texture() {}
	Texture(const std::string& texName, bool useRelativePath = true);
	Texture(const std::string& folderName, 
		const char* faces[]);
	Texture(const unsigned width, const unsigned height);
	~Texture();

	Texture(const Texture&& t) = delete;
	Texture(const Texture&) = delete;

	void Bind(int slot);
	void Unbind() const;

	unsigned Id()   const { return m_Id;		}
	unsigned Slot() const { return m_BoundSlot; }
	int		 GetType() const { return m_Type;		}

private:
	int m_Type;
	unsigned m_BoundSlot;
	unsigned m_Id;

private:
	friend class cereal::access;
	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar& cereal::make_nvp("ID", m_Id);
		ar& cereal::make_nvp("Type", m_Type);
		ar& cereal::make_nvp("BoundSlot", m_BoundSlot);
	}
};
