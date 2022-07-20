#include "pch.h"
#include "Framebuffer.h"
#include "glad/glad.h"

Framebuffer::Framebuffer()
    : m_Id(0), m_ColorAttachment(0), m_DepthAttachment(0),
    m_ViewportWidth(0), m_ViewportHeight(0)
{
	glGenFramebuffers(1, &m_Id);
}

void Framebuffer::ResetForRender(const unsigned textureWidth, const unsigned textureHeight)
{
	m_ViewportWidth = textureWidth;
	m_ViewportHeight = textureHeight;

	if (m_Id)
	{
		glDeleteFramebuffers(1, &m_Id);
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_DepthAttachment);
	}

	glCreateFramebuffers(1, &m_Id);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Id);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
	glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
	glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, textureWidth, textureHeight);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
		"Framebuffer is incomplete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Framebuffer::AttachDepthCubemap(const unsigned textureId,
    const unsigned width, const unsigned height)
{
    m_ViewportWidth   = width;
    m_ViewportHeight  = height;
    m_DepthAttachment = textureId;

    glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureId, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//void Framebuffer::SetViewportSize(const unsigned width, const unsigned height)
//{
//	m_ViewportWidth = width;
//	m_ViewportHeight = height;
//}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
    glViewport(0, 0, m_ViewportWidth, m_ViewportHeight);
}

void Framebuffer::Unbind(const unsigned width, const unsigned height)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glViewport(0, 0, width, height);
}