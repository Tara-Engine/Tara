#pragma once
#include "Tara/Renderer/RenderCommand.h"

namespace Tara {

	class OpenGLRenderCommand : public RenderCommand {
	public:
		OpenGLRenderCommand();
	protected:
		virtual void ISetClearColor(float r, float g, float b) override;
		virtual void ISetDrawType(RenderDrawType drawType, bool wireframe) override;

		virtual void IClear() override;
		virtual void IDraw(VertexArrayRef vertexArray) override;
		virtual void IDrawCount(uint32_t count) override;
		virtual uint32_t IGetMaxTextureSlotsPerShader() override;
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