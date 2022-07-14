#pragma once

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer() {}

	void AttachDepthCubemap(const unsigned textureId);
	void AttachDepthFlatmap(const unsigned textureId);
	void Bind();
	void Unbind();
private:
	unsigned m_Id;
	unsigned m_TextureId;
};