#pragma once
#include <string>

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

	void Bind(int slot = 0) const;
	void Unbind() const;
};

