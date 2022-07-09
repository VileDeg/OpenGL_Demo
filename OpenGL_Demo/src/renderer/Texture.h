#pragma once
#include "pch.h"

class Texture
{
private:
	static constexpr const char* BASE_TEXTURE_PATH = "res/textures/";
	static constexpr const char* BASE_CUBEMAP_PATH = "res/textures/cubemaps/";
public:
	Texture(const std::string& texName);
	Texture(const std::string& folderName, 
		const char* faces[]);
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

//class Cubemap
//{
//private:
//	static constexpr const char* BASE_CUBEMAP_PATH = "res/textures/cubemaps/";
//public:
//	Cubemap(const std::string& folderName, const std::vector<std::string> faces);
//	~Cubemap();
//
//	Cubemap(const Cubemap&& t) = delete;
//	Cubemap(const Cubemap&) = delete;
//
//	void Bind(int slot);
//	void Unbind() const;
//	const unsigned Id() const   { return m_Id; }
//	const unsigned Slot() const { return m_BoundSlot; }
//private:
//	unsigned m_BoundSlot;
//	unsigned m_Id;
//};