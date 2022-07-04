#pragma once
#include "pch.h"

class Texture
{

public:
    static constexpr const char* BASE_TEXTURE_PATH = "res/textures/";

	Texture(const std::string& texName)
		:id(0), path(BASE_TEXTURE_PATH + texName),
		buffer(nullptr), width(0), height(0)
	{
		LoadTexture();
	}
	~Texture();

	Texture(const Texture&& t) noexcept
	{
		id     = 0;
		buffer = nullptr;
		width  = height = 0;
		path   = t.path;
		LoadTexture();
	}
	Texture(const Texture&) = delete;

	void Bind(int slot) const;
	void Unbind() const;
private:
	void LoadTexture();

	unsigned id;
	std::string path;
	unsigned char* buffer;
	int width, height, BPP;
};

