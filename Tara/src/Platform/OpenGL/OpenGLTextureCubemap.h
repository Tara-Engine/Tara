#pragma once
#include "Tara/Renderer/Texture.h"

namespace Tara {

	class OpenGLTextureCubemap : public TextureCubemap {
	public:
		OpenGLTextureCubemap(std::string path, uint32_t size, const std::string& name);
		~OpenGLTextureCubemap();
		inline virtual uint32_t GetWidth()const override { return m_Width; };
		inline virtual uint32_t GetHeight()const override { return m_Height; };
		virtual void ImplBind(int slot, int index)const override;
		virtual void ImplUnbind() const override;
		virtual void SetFiltering(Filtering filter) override;

		virtual void SetWrap(Wrapping wrap) override;
		virtual void SetBorderColor(const glm::vec4& color) override;

	private:
		void LoadHDRIFile();

	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
	};

}