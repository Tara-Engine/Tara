#pragma once
#include "Tara/Renderer/Texture.h"

namespace Tara {

	class OpenGLRenderTarget : public RenderTarget {
	public:
		OpenGLRenderTarget(uint32_t width, uint32_t height, uint32_t colorTargets, InternalType type, const std::string& name);
		~OpenGLRenderTarget();
		inline virtual uint32_t GetWidth()const override { return m_Width; };
		inline virtual uint32_t GetHeight()const override { return m_Height; };
		virtual void ImplBind(int slot, int)const override;
		virtual void ImplUnbind() const override;
		//inline virtual std::pair<int32_t, int32_t> GetLastBindPoint() const override { return std::make_pair(m_LastBindPoint.first, m_LastBindPoint.second); }
		virtual void SetFiltering(Filtering filter) override;

		virtual void SetWrap(Wrapping wrap) override;
		virtual void SetBorderColor(const glm::vec4& color) override;

		virtual void ImplRenderTo(bool render) const override;
		virtual void SetSize(uint32_t width, uint32_t height) override;

		virtual void BlitDepthToOther(RenderTargetRef other) override;
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_FramebufferID;
		std::vector<uint32_t> m_TextureColorIDs;
		uint32_t m_BufferDepthStencilID;
		uint32_t m_TextureInternalType;
		uint32_t m_TextureInternalFormat;
		//mutable std::pair<int32_t, int32_t> m_LastBindPoint;
	};
}