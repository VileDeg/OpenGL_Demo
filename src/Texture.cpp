#include "Texture.h"
#include "glad/glad.h"
#include "stb_image/stb_image.h"
#include <iostream>

std::string Texture::texture_path = "res/textures/";

Texture::Texture(const std::string& filePath)
	: id(0), path(texture_path + filePath), buffer(nullptr), width(0), height(0)
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	stbi_set_flip_vertically_on_load(1);


	buffer = stbi_load(path.c_str(), &width, &height, &BPP, 4);
	if (!buffer)
	{
		std::cerr << "Error: Failed to load texture! " << path << std::endl;
	}
	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

	glBindTexture(GL_TEXTURE_2D, 0);
	if (buffer)
		stbi_image_free(buffer);
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

void Texture::Bind(int slot /*= 0*/) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
