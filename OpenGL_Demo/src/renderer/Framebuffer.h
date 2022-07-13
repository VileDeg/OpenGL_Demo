#pragma once

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer() {}

	void GenerateDepthMap(const unsigned width, const unsigned height);
	void AttachDepthCubemap(const unsigned textureId);
	void Bind();
	void Unbind();
private:
	unsigned m_Id;
	unsigned m_TextureId;
};