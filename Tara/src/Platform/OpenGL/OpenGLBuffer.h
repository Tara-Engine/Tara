#pragma once
#include "Tara/Renderer/Buffer.h"

namespace Tara {

	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(const float* verteces, uint32_t count);
		//OpenGLVertexBuffer(void* verteces, uint32_t size);
		~OpenGLVertexBuffer();

		virtual void ImplBind(int,int) const override;
		
		virtual void SetData(const float* data, uint32_t count) override;
	private:
		uint32_t m_RendererID;
	};


	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(const uint32_t* indecies, uint32_t count);
		~OpenGLIndexBuffer();

		//TODO: layout stuff

		virtual void ImplBind(int, int) const override;

		virtual inline uint32_t GetCount() const override { return m_Count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

}