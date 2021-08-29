#pragma once
#include "Tara/Renderer/Renderer.h"
#include "Tara/Renderer/Shader.h"

namespace Tara {

	struct BufferElement {
		std::string Name;
		Shader::Datatype Type;
		bool Normalized;
		uint32_t Offset;
		uint32_t Size;
		BufferElement(Shader::Datatype type, const std::string& name = "", bool normalized = false)
			: Name(name), Type(type), Normalized(normalized), 
			Size(Shader::GetDatatypeSize(type)), Offset(0) {}
	};

	struct BufferLayout {
	public:
		BufferLayout(const std::initializer_list<BufferElement>& elements);
		inline const uint32_t GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		//iterators!
		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer {
		using VertexBufferRef = std::shared_ptr<VertexBuffer>;
	public:
		VertexBuffer() : m_BufferLayout({}) {}
		virtual ~VertexBuffer() {}
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void SetLayout(const BufferLayout& layout) { m_BufferLayout = layout; };
		const BufferLayout& GetLayout() const { return m_BufferLayout; }

	public:
		static VertexBufferRef Create(float* vertexes, uint32_t count);

	protected:
		BufferLayout m_BufferLayout;
	};

	//Index Buffer Class
	class IndexBuffer {
		using IndexBufferRef = std::shared_ptr<IndexBuffer>;
	public:
		virtual ~IndexBuffer() {}
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual uint32_t GetCount() const = 0;
	public:
		static IndexBufferRef Create(uint32_t* indecies, uint32_t count);
	};



	using VertexBufferRef = std::shared_ptr<VertexBuffer>;
	using IndexBufferRef = std::shared_ptr<IndexBuffer>;
}