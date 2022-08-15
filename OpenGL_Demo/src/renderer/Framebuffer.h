#pragma once

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer() {}

	const unsigned GetColorAttachmentId(unsigned index) const 
	{ 
		switch (index)
		{
		case 0:
			return m_ColorAtnId;
		case 1:
			return m_IntAtnId;
		}
		ASSERT(0, "");
	}
	void Invalidate(const unsigned textureWidth, const unsigned textureHeight);
	void AttachDepthCubemap(const unsigned textureId,
		const unsigned textureWidth, const unsigned textureHeight);

	int ReadPixelInt(unsigned x, unsigned y);
	void ClearIntAttachment(int clearVal);

	void Bind();
	void Unbind();
	const unsigned Id() const { return m_Id; }
private:
	/*int  m_ColorAtnNum{0};
	bool m_HasDepthAtn{true};
	bool m_HasIntAtn  {true};*/

	unsigned m_Id{};
	unsigned m_ColorAtnId{};
	unsigned m_DepthAtnId{};
	unsigned m_IntAtnId{};
	unsigned m_Width{}, m_Height{};
};