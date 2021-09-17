#pragma once

#include "Tara/Renderer/Buffer.h"

namespace Tara {
	/// <summary>
	/// Vertex Array class.
	/// Used to hold the Vertex and Index buffers 
	/// When drawing, this is what is bound, not the various buffers.
	/// </summary>
	class VertexArray{
		/// <summary>
		/// A reference to a VertexArray
		/// </summary>
		using VertexArrayRef = std::shared_ptr<VertexArray>;
	public:
		virtual ~VertexArray() {}
		/// <summary>
		/// Bind the Vertex Array to be drawn
		/// </summary>
		virtual void Bind() const = 0;
		/// <summary>
		/// Unbind the Vertex Array (not nesecary)
		/// </summary>
		virtual void Unbind() const = 0;
		
		//these are virtual as they are platform-dependant
		
		/// <summary>
		/// Add a VertexBuffer to this VertexArray. 
		/// </summary>
		/// <param name="vertexBuffer">the VertexBuffer to add.</param>
		virtual void AddVertexBuffer(const VertexBufferRef vertexBuffer) = 0;
		/// <summary>
		/// Set the IndexBuffer for this VertexArray
		/// </summary>
		/// <param name="indexBuffer">the IndexBuffer to use.</param>
		virtual void SetIndexBuffer(const IndexBufferRef indexBuffer) = 0;

		//these are not virtual as they are platform-independant
		
		/// <summary>
		/// Get a vector of all VertexBuffers in this VertexArray.
		/// </summary>
		/// <returns>the vector of VertexBufferRef objects</returns>
		inline const std::vector<VertexBufferRef>& GetVertexBuffers() const { return m_VertexBuffers; }
		/// <summary>
		/// Get the IndexBuffer of this VertexArray
		/// </summary>
		/// <returns>the IndexBufferRef object</returns>
		inline const IndexBufferRef& GetIndexBuffer() const { return m_IndexBuffer; }

	public:
		/// <summary>
		/// Create a new VertexArray object
		/// </summary>
		/// <returns>the new VertexArrayRef object</returns>
		static VertexArrayRef Create();

	protected:
		/// <summary>
		/// vector of all added VertexBuffers
		/// </summary>
		std::vector<VertexBufferRef> m_VertexBuffers;
		/// <summary>
		/// reference of current IndexBuffer
		/// </summary>
		IndexBufferRef m_IndexBuffer;
	};
	
	/// <summary>
	/// Reference to a Vertex Array class.
	/// Used to hold the Vertex and Index buffers 
	/// When drawing, this is what is bound, not the various buffers.
	/// </summary>
	using VertexArrayRef = std::shared_ptr<VertexArray>;
}