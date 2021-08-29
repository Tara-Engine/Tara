#pragma once
#include "Tara/Renderer/Renderer.h"
#include "Tara/Renderer/Buffer.h"

namespace Tara {

	class VertexArray{
		using VertexArrayRef = std::shared_ptr<VertexArray>;
	public:
		virtual ~VertexArray() {}
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		//these are virtual as they are platform-dependant
		virtual void AddVertexBuffer(const VertexBufferRef vertexBuffer) = 0;
		virtual void SetIndexBuffer(const IndexBufferRef indexBuffer) = 0;

		//these are not virtual as they are platform-independant
		inline const std::vector<VertexBufferRef>& GetVertexBuffers() const { return m_VertexBuffers; }
		inline const IndexBufferRef& GetIndexBuffer() const { return m_IndexBuffer; }

	public:
		static VertexArrayRef Create();

	protected:
		std::vector<VertexBufferRef> m_VertexBuffers;
		IndexBufferRef m_IndexBuffer;
	};
	using VertexArrayRef = std::shared_ptr<VertexArray>;
}