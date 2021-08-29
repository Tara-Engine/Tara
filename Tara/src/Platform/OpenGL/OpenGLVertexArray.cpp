#include "tarapch.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"
#include "OpenGLShader.h"
namespace Tara {

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const VertexBufferRef vertexBuffer)
	{
		DCHECK_F(!vertexBuffer->GetLayout().GetElements().empty(), "Vertex Buffer has no layout!");
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		const BufferLayout& layout = vertexBuffer->GetLayout();
		uint32_t index = 0;
		for (auto& elm : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				Shader::GetDatatypeCount(elm.Type),
				OpenGLShader::DatatypeToGLenum(elm.Type),
				elm.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)(uint64_t)elm.Offset
			);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const IndexBufferRef indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}

}