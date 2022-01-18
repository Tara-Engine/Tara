#pragma once
#include "Tara/Renderer/RenderTarget.h"

namespace Tara {

	class OpenGLRenderTargetCubemap : public RenderTargetCubemap {
	public:
		OpenGLRenderTargetCubemap(uint32_t width, uint32_t height, uint32_t colorTargets,
			InternalType type, const std::string& name);
		~OpenGLRenderTargetCubemap();
		inline virtual uint32_t GetWidth()const override { return m_Width; };
		inline virtual uint32_t GetHeight()const override { return m_Height; };
		virtual void ImplBind(int slot, int index)const override;
		virtual void ImplUnbind() const override;
		virtual void SetFiltering(Filtering filter) override;

		virtual void SetWrap(Wrapping wrap) override;
		virtual void SetBorderColor(const glm::vec4& color) override;

		virtual void ImplRenderTo(bool render) const override;
		virtual void SetSize(uint32_t width, uint32_t height) override;
		inline virtual uint32_t GetTextureCount() override { return m_TextureColorIDs.size(); }

		virtual void SetTargetedFace(Face face) override;
		virtual void SetTargetFaceAll() override;


	private:
		uint32_t m_Width, m_Height;
		uint32_t m_FramebufferID;
		std::vector<uint32_t> m_TextureColorIDs;
		uint32_t m_TextureDepth;
		uint32_t m_TextureInternalType;
		uint32_t m_TextureInternalFormat;
	};

}