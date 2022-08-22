#pragma once

#include "renderer/Texture.h"

namespace Crave
{
	class Framebuffer
	{
	public:
		enum class DrawBuffers : int
		{
			Undefined = -1, None, AllColorAttachments
		};
		enum class ReadBuffers : int
		{
			Undefined = -1, None, ColorAttachment0
		};
		struct Config
		{
			bool isComplete = false;
			DrawBuffers drawBuffers = DrawBuffers::AllColorAttachments;
			ReadBuffers readBuffers = ReadBuffers::ColorAttachment0;

			std::vector<Texture::Config> texConfigs{};

			Config() = default;
			Config(bool ic, DrawBuffers db, ReadBuffers rb, std::initializer_list<Texture::Config> texConf)
				: isComplete(ic), drawBuffers(db), readBuffers(rb), texConfigs(texConf) {}
			Config(bool ic, std::initializer_list<Texture::Config> texConf)
				: isComplete(ic), texConfigs(texConf) {}
		};
	public:
		Framebuffer(Config config);
		~Framebuffer() {}

		//unsignedm_IntColorAttachmentId
		/*unsigned GetIntegerColorAttachmentId() const
		{
			for (auto& ca : m_ColorAttachments)
			{
				if (ca.)
			}
			return m_ColorAttachments[index].Id();
		}*/
		Ref<Texture> GetDepthAttachment() { return m_DepthAttachment; }
		unsigned GetColorAttachmentId(unsigned index) const
		{
			ASSERT(index <= m_ColorAttachments.size(), "");
			return m_ColorAttachments[index]->Id();
		}
		void Invalidate(glm::vec2 newDimensions);
		/*void AttachDepthCubemap(const unsigned textureId,
			const unsigned textureWidth, const unsigned textureHeight);*/

		int ReadPixelInt(unsigned x, unsigned y);
		void ClearIntAttachment(int clearVal);

		void Bind();
		void Unbind();
		const unsigned Id() const { return m_Id; }
	private:
		unsigned m_Id{};

		std::vector<Ref<Texture>> m_ColorAttachments{};
		Ref<Texture> m_DepthAttachment{};
		unsigned m_IntColorAttachmentId{};

		Config m_Config{};
		glm::vec2 m_Dimensions{};
	private:
		static constexpr const int MAX_COLOR_ATTACHMENTS = 4;
	};
}