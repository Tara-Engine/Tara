#pragma once
#include "Tara/Renderer/RenderCommand.h"

namespace Tara {

	class OpenGLRenderCommand : public RenderCommand {
	public:
		OpenGLRenderCommand() {}
	protected:
		virtual void ISetClearColor(float r, float g, float b) override;
		virtual void IClear() override;
		virtual void IDraw(VertexArrayRef vertexArray) override;
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
	};

}