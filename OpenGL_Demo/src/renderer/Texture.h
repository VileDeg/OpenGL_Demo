#pragma once
#include "pch.h"

class Texture
{
private:
	static constexpr const char* BASE_TEXTURE_PATH = "res/textures/";
	static constexpr const char* BASE_CUBEMAP_PATH = "res/textures/cubemaps/";
public:
	Texture(const std::string& texName, bool useRelativePath = true);
	Texture(const std::string& folderName, 
		const char* faces[]);
	Texture(const unsigned width, const unsigned height);
	~Texture();

	Texture(const Texture&& t) = delete;
	Texture(const Texture&) = delete;

	void Bind(int slot);
	void Unbind() const;
	const unsigned Id() const   { return m_Id; }
	const unsigned Slot() const { return m_BoundSlot; }
private:
	int m_Type;
	unsigned m_BoundSlot;
	unsigned m_Id;
};
