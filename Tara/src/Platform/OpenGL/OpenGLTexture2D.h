#pragma once
#include "Tara/Renderer/Texture.h"

namespace Tara {

	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const std::string& path, const std::string& name);
		OpenGLTexture2D(const uint8_t* bytes, uint32_t width, uint32_t height, uint32_t bytesPerPixel, const std::string& name);
		~OpenGLTexture2D();
		virtual inline uint32_t GetWidth()const override { return m_Width; }
		virtual inline uint32_t GetHeight()const override { return m_Height; }
		virtual void Bind(int slot)const override;
		virtual void SetFiltering(Filtering filter) override;
		virtual void SetWrap(Wrapping wrap) override;
		virtual void SetBorderColor(const glm::vec4& color) override;
	protected:
		void LoadFromFile();
		void LoadFromArray(const uint8_t* imageData, uint32_t channels);
	private:
		std::string m_Path;   
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;

	};
}