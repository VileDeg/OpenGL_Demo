#include "pch.h"
#include "Framebuffer.h"

#include <glad/glad.h>

namespace Crave
{
	Framebuffer::Framebuffer(Config config)
		: m_Config(config)
	{
		glCreateFramebuffers(1, &m_Id);
	}

	void Framebuffer::Invalidate(glm::vec2 newDimensions)
	{
		m_Dimensions = newDimensions;

		if (m_Id)
		{
			glDeleteFramebuffers(1, &m_Id);
			m_ColorAttachments.clear();
			//m_DepthAttachment = {};
			m_IntColorAttachmentId = 0;
		}

		glCreateFramebuffers(1, &m_Id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
		auto& texConfigs = m_Config.texConfigs;
		for (size_t i = 0; i < texConfigs.size(); ++i)
		{
			auto& config = texConfigs[i];
			switch (config.type)
			{
			case Texture::Type::RGBA:
			case Texture::Type::Integer:
			{
				auto& thisAtt = m_ColorAttachments.emplace_back(CreateRef<Texture>(newDimensions, config));
				glFramebufferTexture2D(
					GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, (int)config.target, thisAtt->Id(), 0);
				if (config.type == Texture::Type::Integer)
					m_IntColorAttachmentId = thisAtt->Id();
				break;
			}
			case Texture::Type::Depth:
				m_DepthAttachment = CreateRef<Texture>(newDimensions, config);

				if (config.target == Texture::Target::Texture2D)
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment->Id(), 0);
				else if (config.target == Texture::Target::TextureCubeMap)
					glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthAttachment->Id(), 0);
				else
					ASSERT(false, "");
				break;
			case Texture::Type::DepthNStencil:
				m_DepthAttachment = CreateRef<Texture>(newDimensions, config);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, (int)config.target, m_DepthAttachment->Id(), 0);
				break;
			default:
				ASSERT(false, "Unsupported type");
			}
			
		}

		size_t size = m_ColorAttachments.size();
		ASSERT(size <= MAX_COLOR_ATTACHMENTS, "");
		GLenum buffers[MAX_COLOR_ATTACHMENTS];

		switch (m_Config.drawBuffers)
		{

		case DrawBuffers::AllColorAttachments:
			for (size_t i = 0; i < size; ++i)
				buffers[i] = GL_COLOR_ATTACHMENT0 + i;

			glDrawBuffers(size, buffers);
			break;
		case DrawBuffers::None:
			glDrawBuffer(GL_NONE);
			break;
		default:
			ASSERT(false, "Unsupported");
		}

		switch (m_Config.readBuffers)
		{
		case ReadBuffers::ColorAttachment0:
			glReadBuffer(GL_COLOR_ATTACHMENT0);
			break;
		case ReadBuffers::None:
			glReadBuffer(GL_NONE);
			break;
		default:
			ASSERT(false, "Unsupported");
		}
		
		if (m_Config.isComplete)
			ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
				"Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	int Framebuffer::ReadPixelInt(unsigned x, unsigned y)
	{
		int pixelData = -1;
		glReadBuffer(GL_COLOR_ATTACHMENT1);
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void Framebuffer::ClearIntAttachment(int clearVal)
	{
		glClearTexImage(m_IntColorAttachmentId, 0,
			GL_RED_INTEGER, GL_INT, &clearVal);
	}


	/*void Framebuffer::AttachDepthCubemap(const unsigned textureId,
		const unsigned width, const unsigned height)
	{
		m_Width = width;
		m_Height = height;
		m_DepthAtnId = textureId;

		glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureId, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}*/

	void Framebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
		glViewport(0, 0, m_Dimensions.x, m_Dimensions.y);
	}

	void Framebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}