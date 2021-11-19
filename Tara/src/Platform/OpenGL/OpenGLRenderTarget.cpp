#include "tarapch.h"
#include "OpenGLRenderTarget.h"
#include "glad/glad.h"

namespace Tara {
	OpenGLRenderTarget::OpenGLRenderTarget(uint32_t width, uint32_t height, const std::string& name)
		: RenderTarget(name), m_Width(width), m_Height(height)
	{
		//generate the framebuffer
		glGenFramebuffers(1, &m_FramebufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);


		//deal with color buffer
		//generate color buffer texture
		glGenTextures(1, &m_TextureColorID);
		glBindTexture(GL_TEXTURE_2D, m_TextureColorID);
		//make the texture data format
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		//deal with filters.
		if (Texture::s_DefaultTextureFiltering == Texture::Filtering::Nearest) {
			glTextureParameteri(m_TextureColorID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(m_TextureColorID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else {
			glTextureParameteri(m_TextureColorID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_TextureColorID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		//unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);

		//attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureColorID, 0);


		//deal with depth and stencil buffer
		//can be raw data (Renderbuffer) instead of texture
		//make render buffer object, setup
		glGenRenderbuffers(1, &m_BufferDepthStencilID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_BufferDepthStencilID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		//attach to frame buffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_BufferDepthStencilID);

		//error check (sanity!)
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			LOG_S(ERROR) << "Framebuffer is not complete! Something happened";
		}

		//UNBIND!
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	OpenGLRenderTarget::~OpenGLRenderTarget()
	{
		//clean up OpenGL objects
		glDeleteRenderbuffers(1, &m_BufferDepthStencilID);
		glDeleteTextures(1, &m_TextureColorID);
		glDeleteFramebuffers(1, &m_FramebufferID);
	}

	void OpenGLRenderTarget::Bind(int slot) const
	{
		//bind the texture for rendering
		glBindTextureUnit(slot, m_TextureColorID);
	}

	void OpenGLRenderTarget::SetFiltering(Filtering filter)
	{
		if (filter == Texture::Filtering::Nearest) {
			glTextureParameteri(m_TextureColorID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(m_TextureColorID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else {
			glTextureParameteri(m_TextureColorID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_TextureColorID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
	}

	void OpenGLRenderTarget::RenderTo(bool render) const
	{
		if (this && render) {
			glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void OpenGLRenderTarget::SetSize(uint32_t width, uint32_t height)
	{
		m_Width = width;
		m_Height = height;

		glBindTexture(GL_TEXTURE_2D, m_TextureColorID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glBindRenderbuffer(GL_RENDERBUFFER, m_BufferDepthStencilID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

}
