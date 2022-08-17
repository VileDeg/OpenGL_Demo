#include "pch.h"
#include "Framebuffer.h"
#include "glad/glad.h"

Framebuffer::Framebuffer()
{
	glCreateFramebuffers(1, &m_Id);
}

void Framebuffer::Invalidate(const unsigned textureWidth, const unsigned textureHeight)
{
	m_Width = textureWidth;
	m_Height = textureHeight;

	if (m_Id)
	{
		glDeleteFramebuffers(1, &m_Id);

		glDeleteTextures(1, &m_ColorAtnId);
		glDeleteTextures(1, &m_IntAtnId);
		glDeleteTextures(1, &m_DepthAtnId);
	}

	glCreateFramebuffers(1, &m_Id);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Id);

	//RGBA attachment
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAtnId);
		glBindTexture(GL_TEXTURE_2D, m_ColorAtnId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAtnId, 0);
	}
	//Integer attachment
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_IntAtnId);
		glBindTexture(GL_TEXTURE_2D, m_IntAtnId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, textureWidth, textureHeight, 0, GL_RED_INTEGER, GL_INT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_IntAtnId, 0);
	}
	
	
	//Depth attachment
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAtnId);
		glBindTexture(GL_TEXTURE_2D, m_DepthAtnId);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, textureWidth, textureHeight);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAtnId, 0);
	}

	GLenum buffers[2]{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, buffers);

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
		"Framebuffer is incomplete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


int Framebuffer::ReadPixelInt(unsigned x, unsigned y)
{
	//int out = -1;
	int pixelData = -1;
	glReadBuffer(GL_COLOR_ATTACHMENT1);
	glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
	return pixelData;
}

void Framebuffer::ClearIntAttachment(int clearVal)
{
	glClearTexImage(m_IntAtnId, 0,
		GL_RED_INTEGER, GL_INT, &clearVal);
}

void Framebuffer::AttachDepthCubemap(const unsigned textureId,
    const unsigned width, const unsigned height)
{
    m_Width   = width;
    m_Height  = height;
	m_DepthAtnId = textureId;

    glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureId, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
    glViewport(0, 0, m_Width, m_Height);
}

void Framebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}