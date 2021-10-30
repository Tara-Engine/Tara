#include "tarapch.h"
#include "OpenGLTexture2D.h"
#include "Tara/Asset/AssetLibrary.h"

#include "glad/glad.h"
#include "stb_image.h"


namespace Tara{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path, const std::string& name)
		: Texture2D(name), m_Path(path), m_RendererID(0), m_Width(0), m_Height(0)
	{
		LoadFromFile();
		LOG_S(1) << "Image Loaded from File: " << path;
	}

	OpenGLTexture2D::OpenGLTexture2D(const uint8_t* bytes, uint32_t width, uint32_t height, uint32_t bytesPerPixel, const std::string& name)
		: Texture2D(name), m_Path(""), m_RendererID(0), m_Width(width), m_Height(height)
	{
		LoadFromArray(bytes, bytesPerPixel);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(int slot) const
	{
		//glActiveTexture(GL_TEXTURE0 + slot);
		//glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTexture2D::SetFiltering(Texture::Filtering filter)
	{
		if (filter == Texture::Filtering::Nearest) {
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else {
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
	}

	void OpenGLTexture2D::LoadFromFile()
	{
		int32_t width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		//stbi_set_unpremultiply_on_load(1);
		stbi_uc* imageData = stbi_load(m_Path.c_str(), &width, &height, &channels, 0);
		DCHECK_NOTNULL_F(imageData, "Failed to load image! Path: %s", m_Path.c_str());
		m_Width = width;
		m_Height = height;

		LoadFromArray(imageData, channels);

		stbi_image_free(imageData);
	}
	void OpenGLTexture2D::LoadFromArray(const uint8_t* imageData, uint32_t channels)
	{
		GLenum internalFormat = 0, dataFormat = 0;

		switch (channels) {
		case 1: {
			internalFormat = GL_R8;
			dataFormat = GL_RED;
			break;
		}
		case 2: {
			internalFormat = GL_RG8;
			dataFormat = GL_RG;
			break;
		}
		case 3: {
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
			break;
		}
		case 4: {
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
			break;
		}
		default: break;
		}

		DCHECK_F(internalFormat && dataFormat, "Unsupported number of channels in an image!");

		//should be able to regenerate on the fly...
		if (m_RendererID == 0) {
			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		}
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		if (Texture::s_DefaultTextureFiltering == Texture::Filtering::Nearest) {
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else {
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		//if a single chanel texture, make that chanel Alpha, and the color white
		if (channels == 1) {
			GLint swizzleMask[] = { GL_ONE, GL_ONE, GL_ONE, GL_RED };
			glTextureParameteriv(m_RendererID, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
		}

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(
			m_RendererID,
			0, 0, 0,
			m_Width, m_Height,
			dataFormat, GL_UNSIGNED_BYTE,
			imageData
		);
	}
}
