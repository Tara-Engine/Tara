#pragma once
#include "Tara/Renderer/Texture.h"

namespace Tara {

	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const std::string& path, const std::string& name);
		~OpenGLTexture2D();
		virtual inline uint32_t GetWidth()const override { return m_Width; }
		virtual inline uint32_t GetHeight()const override { return m_Height; }
		virtual void Bind(int slot)const override;
		virtual void SetFiltering(Filtering filter) override;
	protected:
		void loadFromFile();
	private:
		std::string m_Path;   
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;

	};
}