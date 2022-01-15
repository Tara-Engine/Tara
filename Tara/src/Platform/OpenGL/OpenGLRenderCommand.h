#pragma once
#include "Tara/Renderer/RenderCommand.h"

namespace Tara {

	class OpenGLRenderCommand : public RenderCommand {
	public:
		OpenGLRenderCommand();
		virtual ~OpenGLRenderCommand() = default;
	protected:
		virtual void ISetClearColor(float r, float g, float b) override;
		virtual void ISetDrawType(RenderDrawType drawType, bool wireframe) override;

		virtual void IClear() override;
		virtual void IDraw(VertexArrayRef vertexArray) override;
		virtual void IDrawCount(uint32_t count) override;
		virtual uint32_t IGetMaxTextureSlotsPerShader() override;
		virtual void IEnableDepthTesting(bool enable) override;
		virtual void IEnableBackfaceCulling(bool enable) override;
		virtual void IEnableFrontfaceCulling(bool enable) override;
		virtual void ISetBlendMode(RenderBlendMode mode) override;
	public:
		static void GLError(
			uint32_t source,
			uint32_t type,
			uint32_t id,
			uint32_t severity,
			int32_t length,
			const char* message,
			const void* userParam
		);
	private:
		uint32_t m_DrawMode;
	};

}