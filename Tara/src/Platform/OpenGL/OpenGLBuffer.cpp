#include "tarapch.h"
#include "OpenGLBuffer.h"
#include "glad/glad.h"

namespace Tara {
	OpenGLVertexBuffer::OpenGLVertexBuffer(const float* vertices, uint32_t count)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), vertices, GL_STATIC_DRAW);
	}

	/*
	OpenGLVertexBuffer::OpenGLVertexBuffer(void* verteces, uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, verteces, GL_STATIC_DRAW);
	}
	*/

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::ImplBind(int, int) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::ImplUnbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	void OpenGLVertexBuffer::ImplSetData(const float* data, uint32_t count)
	{
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, GL_STATIC_DRAW);
	}



	OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* indices, uint32_t count)
		:m_Count(count)
	{
		glCreateBuffers((uint32_t)1, &m_RendererID); //somehow 1 is actually -1?
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::ImplBind(int, int) const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::ImplUnbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}

