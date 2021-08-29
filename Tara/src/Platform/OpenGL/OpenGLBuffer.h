#pragma once
#include "Tara/Renderer/Buffer.h"

namespace Tara {

	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(float* verteces, uint32_t count);
		~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		uint32_t m_RendererID;
	};


	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indecies, uint32_t count);
		~OpenGLIndexBuffer();

		//TODO: layout stuff

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual inline uint32_t GetCount() const override { return m_Count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

}