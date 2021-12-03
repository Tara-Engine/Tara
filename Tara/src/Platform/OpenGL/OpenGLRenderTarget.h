#pragma once
#include "Tara/Renderer/Texture.h"

namespace Tara {

	class OpenGLRenderTarget : public RenderTarget {
	public:
		OpenGLRenderTarget(uint32_t width, uint32_t height, const std::string& name);
		~OpenGLRenderTarget();
		virtual inline uint32_t GetWidth()const override { return m_Width; }
		virtual inline uint32_t GetHeight()const override { return m_Height; }
		virtual void Bind(int slot)const override;
		virtual void SetFiltering(Filtering filter) override;

		virtual void SetWrap(Wrapping wrap) override;
		virtual void SetBorderColor(const glm::vec4& color) override;

		virtual void RenderTo(bool render) const override;
		virtual void SetSize(uint32_t width, uint32_t height) override;
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_FramebufferID;
		uint32_t m_TextureColorID;
		uint32_t m_BufferDepthStencilID;
	};
}