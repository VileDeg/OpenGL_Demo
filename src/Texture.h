#pragma once
#include "pch.h"

class Texture
{
private:
	unsigned id;
	std::string path;
	unsigned char* buffer;
	int width, height, BPP;
public:
    static constexpr const char* texture_path = "res/textures/";

	Texture(const std::string& path);
	~Texture();

	void Bind(int slot) const;
	void Unbind() const;
};

