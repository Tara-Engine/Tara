#pragma once
#include "Tara/Renderer/Shader.h"

namespace Tara {
	REFTYPE(VertexBuffer);
	REFTYPE(IndexBuffer);

	/// <summary>
	/// BufferElement is a piece of a layout for a VertexBuffer
	/// while this discusses the layout of a piece of data, it does not contain it.
	/// </summary>
	struct BufferElement {
		/// <summary>
		/// name for debugging
		/// </summary>
		std::string Name;
		/// <summary>
		/// the type of data in this element
		/// </summary>
		Shader::Datatype Type;
		/// <summary>
		/// if that data is normalized
		/// </summary>
		bool Normalized;
		/// <summary>
		/// The offset of the data (auto-calculated by layout)
		/// </summary>
		uint32_t Offset;
		/// <summary>
		/// The size of the data (auto-calculated from type by constructor)
		/// </summary>
		uint32_t Size;
		/// <summary>
		/// BufferElement constructor
		/// </summary>
		/// <param name="type">Shader::Datatype, the type of the element</param>
		/// <param name="name">std::string = "", for debugging</param>
		/// <param name="normalized">bool = false, in case the data is normalized</param>
		BufferElement(Shader::Datatype type, const std::string& name = "", bool normalized = false)
			: Name(name), Type(type), Normalized(normalized), 
			Offset(0), Size(Shader::GetDatatypeSize(type)) {}
	};

	/// <summary>
	/// BufferLayout describes the layout of the data in a VertexBuffer
	/// It does not contain that data, just a list of BufferElements
	/// </summary>
	struct BufferLayout {
	public:
		/// <summary>
		/// Constructor for the BufferLayout
		/// </summary>
		/// <param name="elements">a list of elements to make the layout. Can be declared inline.</param>
		BufferLayout(const std::initializer_list<BufferElement>& elements);
		
		/// <summary>
		/// Get the stride of the layout
		/// </summary>
		/// <returns>returns layout size in bytes</returns>
		inline const uint32_t GetStride() const { return m_Stride; }
		
		/// <summary>
		/// Get a reference to the internal vector of BufferElements.
		/// </summary>
		/// <returns>const vector reference, list of elements.</returns>
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		
		/// <summary>
		/// Get an iterator to the beginning of the element list
		/// </summary>
		/// <returns>iterator</returns>
		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		
		/// <summary>
		/// Get an iterator to the end of the element list
		/// </summary>
		/// <returns></returns>
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		
		/// <summary>
		/// Get a const iterator to the beginning of the element list
		/// </summary>
		/// <returns>iterator</returns>
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		
		/// <summary>
		/// Get a const iterator to the end of the element list
		/// </summary>
		/// <returns></returns>
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		
		/// <summary>
		/// internal vector of elements
		/// </summary>
		std::vector<BufferElement> m_Elements;
		
		/// <summary>
		/// total size in bytes the elements represents
		/// </summary>
		uint32_t m_Stride = 0;
	};

	/// <summary>
	/// VertexBuffer class, stores the buffer of data that is on the GPU, holding vertex data.
	/// </summary>
	class VertexBuffer : public Bindable {
	public:
		
		/// <summary>
		/// Create a new VertexBuffer
		/// </summary>
		/// <param name="vertexes">a pointer to an array of floats, or, the verteies</param>
		/// <param name="count">the number of floats in the array</param>
		/// <returns>a reference to the new VertexBuffer</returns>
		static VertexBufferRef Create(const float* vertexes, uint32_t count);

		VertexBuffer() : m_BufferLayout({}) {}

		virtual ~VertexBuffer() {}
		
		/// <summary>
		/// Set the layout of the buffer.
		/// </summary>
		/// <param name="layout">BufferLayout can be declared inline, and its elemnts can be declared inline</param>
		virtual void SetLayout(const BufferLayout& layout) { m_BufferLayout = layout; };
		
		/// <summary>
		/// Get the buffer layout
		/// </summary>
		/// <returns>BufferLayout gotten, as const ref.</returns>
		const BufferLayout& GetLayout() const { return m_BufferLayout; }

		/// <summary>
		/// Set the data in the buffer. Can be queued.
		/// </summary>
		/// <param name="data">the new data</param>
		/// <param name="count">the new count, in number of floats</param>
		virtual void SetData(const float* data, uint32_t count);

		/// <summary>
		/// Implementation-specific set the data in the buffer. Not queued
		/// </summary>
		/// <param name="data">the data to set</param>
		/// <param name="count">the float count</param>
		virtual void ImplSetData(const float* data, uint32_t count) = 0;
	public:
		

		/// <summary>
		/// Create a new VertexBuffer
		/// </summary>
		/// <param name="vertexes">a pointer to an array of data</param>
		/// <param name="size">ise of array in bites</param>
		/// <returns>a reference to the new VertexBuffer</returns>
		//static VertexBufferRef Create(void* vertexes, uint32_t size);

	protected:
		/// <summary>
		/// private storage of the buffer layout
		/// </summary>
		BufferLayout m_BufferLayout;
	};

	/// <summary>
	/// IndexBuffer class, stores the index draw order for a Draw call
	/// </summary>
	class IndexBuffer : public Bindable{
	public:
		/// <summary>
		/// Create a new IndexBuffer
		/// </summary>
		/// <param name="indecies">unsigned int pointer to the index array</param>
		/// <param name="count">number of indecies in the array</param>
		/// <returns>Reference to the new IndexBuffer</returns>
		static IndexBufferRef Create(const uint32_t* indecies, uint32_t count);

		virtual ~IndexBuffer() {}
		
		/// <summary>
		/// Get the number of indecies to draw
		/// </summary>
		/// <returns></returns>
		virtual uint32_t GetCount() const = 0;
	};

}