#include "tarapch.h"
#include "OpenGLRenderTarget.h"
#include "glad/glad.h"

#include "Tara/Core/Application.h"

namespace Tara {
	OpenGLRenderTarget::OpenGLRenderTarget(uint32_t width, uint32_t height, uint32_t colorTargets, InternalType type, const std::string& name)
		: RenderTarget(name), m_Width(width), m_Height(height)
	{
		//generate the framebuffer
		glGenFramebuffers(1, &m_FramebufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);

		if (colorTargets > 0){
			LOG_S(INFO) << "RenderTarget being made with " << colorTargets << " colorTargets";

			m_TextureColorIDs.reserve(colorTargets);
			colorTargets = std::min(colorTargets, 32u); //there are only 32 color target attachments

			m_TextureInternalType = 0;
			m_TextureInternalFormat = 0;
			switch (type) {
			case InternalType::INT8: m_TextureInternalType = GL_BYTE; m_TextureInternalFormat = GL_RGBA8I; break;
			case InternalType::INT16: m_TextureInternalType = GL_SHORT; m_TextureInternalFormat = GL_RGBA16I; break;
			case InternalType::INT32: m_TextureInternalType = GL_INT; m_TextureInternalFormat = GL_RGBA32I; break;

			case InternalType::UINT8: m_TextureInternalType = GL_UNSIGNED_BYTE; m_TextureInternalFormat = GL_RGBA8UI; break;
			case InternalType::UINT16: m_TextureInternalType = GL_UNSIGNED_SHORT; m_TextureInternalFormat = GL_RGBA16UI; break;
			case InternalType::UINT32: m_TextureInternalType = GL_UNSIGNED_INT; m_TextureInternalFormat = GL_RGBA32UI; break;
			
			case InternalType::FLOAT: m_TextureInternalType = GL_FLOAT; m_TextureInternalFormat = GL_RGBA32F;  break;
			
			default: m_TextureInternalType = GL_UNSIGNED_BYTE;
			}
			
			std::vector<uint32_t> attachments(colorTargets);
			for (int i=0;i<colorTargets;i++){
				uint32_t textureID;
				//deal with color buffer
				//generate color buffer texture
				glGenTextures(1, &textureID);

				m_TextureColorIDs.push_back(textureID);

				glBindTexture(GL_TEXTURE_2D, textureID);
				//make the texture data format
				glTexImage2D(GL_TEXTURE_2D, 0, m_TextureInternalFormat, m_Width, m_Height, 0, GL_RGBA, m_TextureInternalType, NULL);
				//deal with filters.
				if (Texture::s_DefaultTextureFiltering == Texture::Filtering::Nearest) {
					glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				}
				else {
					glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}


				//deal with wrapping
				switch (Texture::s_DefaultTextureWrapping) {
				case Texture::Wrapping::Clamp: {
					glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				}
				case Texture::Wrapping::Border: {
					glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
					glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
				}
				case Texture::Wrapping::Repeat: {
					glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
				}
				case Texture::Wrapping::Mirror: {
					glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
					glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
				}
				}

				


				//attach texture to framebuffer
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureID, 0);
				attachments[i] = GL_COLOR_ATTACHMENT0 + i;
				//unbind texture
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			glDrawBuffers(colorTargets, attachments.data());
		}
		else {
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}

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
		for (auto& id : m_TextureColorIDs) {
			glDeleteTextures(1, &id);
		}
		glDeleteFramebuffers(1, &m_FramebufferID);
	}

	void OpenGLRenderTarget::ImplBind(int slot, int index) const
	{
		//bind the texture for rendering
		glBindTextureUnit(slot, m_TextureColorIDs[index]);
	}

	void OpenGLRenderTarget::ImplUnbind() const
	{
		//pass
	}

	void OpenGLRenderTarget::SetFiltering(Filtering filter)
	{
		for (auto& id : m_TextureColorIDs) {
			if (filter == Texture::Filtering::Nearest) {
				glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
			else {
				glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
		}
	}


	void OpenGLRenderTarget::SetWrap(Wrapping wrap)
	{
		for (auto& id : m_TextureColorIDs) {
			switch (wrap) {
			case Texture::Wrapping::Clamp: {
				glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			case Texture::Wrapping::Border: {
				glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
				glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			}
			case Texture::Wrapping::Repeat: {
				glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}
			case Texture::Wrapping::Mirror: {
				glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
				glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			}
			}
		}
	}

	void OpenGLRenderTarget::SetBorderColor(const glm::vec4& color)
	{
		for (auto& id : m_TextureColorIDs) {
			glTextureParameterfv(id, GL_TEXTURE_BORDER_COLOR, (GLfloat*)&color);
		}
	}


	void OpenGLRenderTarget::ImplRenderTo(bool render) const
	{
		if (this && render) {
			glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);
			std::vector<uint32_t> attachments(m_TextureColorIDs.size());
			std::iota(attachments.begin(), attachments.end(), GL_COLOR_ATTACHMENT0);
			glDrawBuffers(attachments.size(), attachments.data());
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void OpenGLRenderTarget::SetSize(uint32_t width, uint32_t height)
	{
		m_Width = width;
		m_Height = height;
		for (auto& id : m_TextureColorIDs){
			glBindTexture(GL_TEXTURE_2D, id);
			glTexImage2D(GL_TEXTURE_2D, 0, m_TextureInternalFormat, m_Width, m_Height, 0, GL_RGBA, m_TextureInternalType, NULL);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		glBindRenderbuffer(GL_RENDERBUFFER, m_BufferDepthStencilID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	void OpenGLRenderTarget::BlitDepthToOther(RenderTargetRef other)
	{
		std::shared_ptr<OpenGLRenderTarget> otherGL = std::dynamic_pointer_cast<OpenGLRenderTarget>(other);
		if (otherGL || !other) {
			//the other is invalid (default framebuffer) or a valid OpenGLRenderTarget

			//Get the data, accounting for this or other being null (default framebuffer)
			uint32_t source = 0;
			uint32_t sourceWidth = Application::Get()->GetWindow()->GetWidth();
			uint32_t sourceHeight = Application::Get()->GetWindow()->GetHeight();

			uint32_t dest = 0;
			uint32_t destWidth = sourceWidth;
			uint32_t destHeight = sourceHeight;
			if (this) {
				source = m_FramebufferID;
				sourceWidth = GetWidth();
				sourceHeight = GetHeight();
			}
			if (otherGL) {
				dest = otherGL->m_FramebufferID;
				destWidth = otherGL->GetWidth();
				destHeight = otherGL->GetHeight();
			}
			if (source == dest){
				LOG_S(WARNING) << "Attempted to copy the depth buffer from a RenderTarget to itself. Ignored.";
				return;
			}
			//bind buffers and copy
			glBindFramebuffer(GL_READ_FRAMEBUFFER, source);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dest);
			glBlitFramebuffer(0, 0, sourceWidth, sourceHeight, 0, 0, destWidth, destHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
			//done, immedately bind other buffer as RenderTarget
			if (otherGL){
				otherGL->ImplRenderTo(true);
			}
			else {
				ImplRenderTo(false);
			}
		}
		else {
			//other is invalid, ie, not an OpenGLRenderTarget, but is still valid. This is an error
			LOG_S(WARNING) << "Attempted to copy the depth buffer from a non-OpenGL RenderTarget to an OpenGL RenderTarget, an invalid operation. Ignored.";
		}
	}

}
