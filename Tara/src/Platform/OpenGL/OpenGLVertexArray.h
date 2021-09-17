#pragma once
#include "Tara/Renderer/VertexArray.h"
//#include "Tara/Renderer/Shader.h"

namespace Tara {

	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const VertexBufferRef vertexBuffer) override;
		virtual void SetIndexBuffer(const IndexBufferRef indexBuffer) override;
	private:
		uint32_t m_RendererID;
	};

}
