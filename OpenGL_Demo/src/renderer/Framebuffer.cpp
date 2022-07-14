#include "pch.h"
#include "Framebuffer.h"
#include "glad/glad.h"

Framebuffer::Framebuffer()
{
	glGenFramebuffers(1, &m_Id);
}

void Framebuffer::AttachDepthCubemap(const unsigned textureId)
{
    if (m_TextureId = textureId)
        return;
    glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureId, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_TextureId = textureId;
}

void Framebuffer::AttachDepthFlatmap(const unsigned textureId)
{
    if (m_TextureId = textureId)
        return;
    glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureId, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_TextureId = textureId;
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
}
void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}