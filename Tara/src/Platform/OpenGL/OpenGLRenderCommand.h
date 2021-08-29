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
	};

}