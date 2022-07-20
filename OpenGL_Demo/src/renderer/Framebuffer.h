#pragma once

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer() {}

	const unsigned GetColorAttachmentId() const { return m_ColorAttachment; }
	void ResetForRender(const unsigned textureWidth, const unsigned textureHeight);
	void AttachDepthCubemap(const unsigned textureId,
		const unsigned textureWidth, const unsigned textureHeight);

	//void SetViewportSize(const unsigned width, const unsigned height);

	void Bind();
	void Unbind(const unsigned width, const unsigned height);
	const unsigned Id() const { return m_Id; }
private:
	unsigned m_Id;
	unsigned m_ColorAttachment;
	unsigned m_DepthAttachment;
	unsigned m_ViewportWidth{0}, m_ViewportHeight{0};
};