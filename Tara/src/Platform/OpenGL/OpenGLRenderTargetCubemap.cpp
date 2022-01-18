#include "tarapch.h"
#include "OpenGLRenderTargetCubemap.h"
#include "glad/glad.h"

#include "Tara/Core/Application.h"

namespace Tara{
	OpenGLRenderTargetCubemap::OpenGLRenderTargetCubemap(uint32_t width, uint32_t height, uint32_t colorTargets, InternalType type, const std::string& name)
		: Texture(name), RenderTargetCubemap(name), m_Width(width), m_Height(height)
	{
		glGenFramebuffers(1, &m_FramebufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);

		if (colorTargets > 0) {
			LOG_S(INFO) << "RenderTargetCubemap being made with " << colorTargets << " Color Targets";

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
			for (int i = 0; i < colorTargets; i++) {
				uint32_t textureID;
				//deal with color buffer
				//generate color buffer texture
				glGenTextures(1, &textureID);

				m_TextureColorIDs.push_back(textureID);
				glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
				for (unsigned int i = 0; i < 6; ++i){
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_TextureInternalFormat, m_Width, m_Height, 0, GL_RGBA, m_TextureInternalType, NULL);
				}

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
				glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, textureID, 0);
				attachments[i] = GL_COLOR_ATTACHMENT0 + i;
				//unbind texture
				//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			}
			glDrawBuffers(colorTargets, attachments.data());
		}
		else {
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}
		
		//deal with the depth buffer.
		//cubemaps always have it as a texture.
		glGenTextures(1, &m_TextureDepth);

		m_TextureColorIDs.push_back(m_TextureDepth);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureDepth);
		for (unsigned int i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}

		//deal with filters.
		if (Texture::s_DefaultTextureFiltering == Texture::Filtering::Nearest) {
			glTextureParameteri(m_TextureDepth, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(m_TextureDepth, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else {
			glTextureParameteri(m_TextureDepth, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_TextureDepth, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}


		//deal with wrapping
		switch (Texture::s_DefaultTextureWrapping) {
		case Texture::Wrapping::Clamp: {
			glTextureParameteri(m_TextureDepth, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(m_TextureDepth, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		case Texture::Wrapping::Border: {
			glTextureParameteri(m_TextureDepth, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTextureParameteri(m_TextureDepth, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		}
		case Texture::Wrapping::Repeat: {
			glTextureParameteri(m_TextureDepth, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_TextureDepth, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		case Texture::Wrapping::Mirror: {
			glTextureParameteri(m_TextureDepth, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTextureParameteri(m_TextureDepth, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		}
		}

		//attach texture to framebuffer
		//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_TextureDepth, 0);
		glNamedFramebufferTexture(m_FramebufferID, GL_DEPTH_ATTACHMENT, m_TextureDepth, 0);
		
		//unbind texture
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		//check framebuffer for errors
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			LOG_S(ERROR) << "Framebuffer is not complete! Something happened";
		}

		//UNBIND!
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	OpenGLRenderTargetCubemap::~OpenGLRenderTargetCubemap()
	{
		for (auto& id : m_TextureColorIDs) {
			glDeleteTextures(1, &id);
		}
		glDeleteFramebuffers(1, &m_FramebufferID);
	}

	void OpenGLRenderTargetCubemap::ImplBind(int slot, int index) const
	{
		if (index < m_TextureColorIDs.size()) {
			glBindTextureUnit(slot, m_TextureColorIDs[index]);
		}
		else {
			LOG_S(WARNING) << "Attemtpted to bind an invalid index in a RenderTarget: " << index << " is greater than the number of textures (" << m_TextureColorIDs.size() << ")";
		}
	}
	
	void OpenGLRenderTargetCubemap::ImplUnbind() const {}

	void OpenGLRenderTargetCubemap::SetFiltering(Filtering filter)
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
	
	void OpenGLRenderTargetCubemap::SetWrap(Wrapping wrap)
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
	
	void OpenGLRenderTargetCubemap::SetBorderColor(const glm::vec4& color)
	{
		for (auto& id : m_TextureColorIDs) {
			glTextureParameterfv(id, GL_TEXTURE_BORDER_COLOR, (GLfloat*)&color);
		}
	}
	
	void OpenGLRenderTargetCubemap::ImplRenderTo(bool render) const
	{
		if (render) {
			glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);
			std::vector<uint32_t> attachments(m_TextureColorIDs.size() - 1);
			std::iota(attachments.begin(), attachments.end(), GL_COLOR_ATTACHMENT0);
			glDrawBuffers(attachments.size(), attachments.data());
			glViewport(0, 0, m_Width, m_Height);
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			auto& window = Application::Get()->GetWindow();
			glViewport(0, 0, window->GetWidth(), window->GetHeight());
		}
	}
	
	void OpenGLRenderTargetCubemap::SetSize(uint32_t width, uint32_t height)
	{
		m_Width = width;
		m_Height = height;
		//colors
		for (int i = 0; i < m_TextureColorIDs.size() - 1; i++) {
			uint32_t id = m_TextureColorIDs[i];
			glBindTexture(GL_TEXTURE_CUBE_MAP, id);
			for (unsigned int i = 0; i < 6; ++i) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_TextureInternalFormat, m_Width, m_Height, 0, GL_RGBA, m_TextureInternalType, NULL);
			}
		}

		//depth
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureDepth);
		for (unsigned int i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}


	void OpenGLRenderTargetCubemap::SetTargetedFace(Face face)
	{
		ImplRenderTo(true);

		GLenum glFace = GL_TEXTURE_CUBE_MAP_POSITIVE_X + (unsigned int)face;
		for (int i = 0; i < m_TextureColorIDs.size() - 1; i++) {
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, glFace, m_TextureColorIDs[i], 0);
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, glFace, m_TextureDepth, 0);
	}

	void OpenGLRenderTargetCubemap::SetTargetFaceAll()
	{
		for (int i = 0; i < m_TextureColorIDs.size() - 1; i++) {
			glNamedFramebufferTexture(m_FramebufferID, GL_COLOR_ATTACHMENT0 + i, m_TextureColorIDs[i], 0);
		}

		glNamedFramebufferTexture(m_FramebufferID, GL_DEPTH_ATTACHMENT, m_TextureDepth, 0);
	}
}