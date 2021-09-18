#pragma once
#include "Tara/Renderer/Renderer.h"
#include "Tara/Renderer/VertexArray.h"

namespace Tara {
	
	/// <summary>
	/// The RenderCommand class is a (mostly?) static class, providing an interface to a private 
	/// PlatformRenderCommand instance. (Unique Pointer)
	/// </summary>
	class RenderCommand {
	public:
		/// <summary>
		/// Initialize the RenderCommand internal instance
		/// </summary>
		static void Init();
		/// <summary>
		/// Set the clear color
		/// </summary>
		/// <param name="color">clear color as a vec3</param>
		inline static void SetClearColor(glm::vec3 color) { s_RC->ISetClearColor(color.r, color.g, color.b); }
		/// <summary>
		/// Set the clear color
		/// </summary>
		/// <param name="color">clear color as a vec4 (alpha is ignored)</param>
		inline static void SetClearColor(glm::vec4 color) { s_RC->ISetClearColor(color.r, color.g, color.b); }
		/// <summary>
		/// Set the clear color
		/// </summary>
		/// <param name="r">red component of clear color</param>
		/// <param name="g">green component of clear color</param>
		/// <param name="b">blue component of clear color</param>
		inline static void SetClearColor(float r, float g, float b) { s_RC->ISetClearColor(r, g, b); }
		/// <summary>
		/// clear the screen
		/// </summary>
		inline static void Clear() { s_RC->IClear(); }
		/// <summary>
		/// Draw the current VertexArray. Does not bind the VertexArray passed to it.
		/// </summary>
		/// <param name="vertexArray"></param>
		inline static void Draw(VertexArrayRef vertexArray) { s_RC->IDraw(vertexArray); }

		/// <summary>
		/// Draw the current VertexArray using lines. Does not bind the VertexArray passed to it.
		/// </summary>
		/// <param name="vertexArray"></param>
		inline static void DrawLines(VertexArrayRef vertexArray) { s_RC->IDrawLines(vertexArray); }

	protected:
		/// <summary>
		/// Protected SetClearColor, for underlying implementation to override
		/// </summary>
		/// <param name="r">red component of clear color</param>
		/// <param name="g">green component of clear color</param>
		/// <param name="b">blue component of clear color</param>
		virtual void ISetClearColor(float r, float g, float b) = 0;
		/// <summary>
		/// Protected Clear, for underlying implementation to override
		/// </summary>
		virtual void IClear() = 0;
		/// <summary>
		/// Protected Draw, for underlying implementation to override
		/// </summary>
		/// <param name="vertexArray"></param>
		virtual void IDraw(VertexArrayRef vertexArray) = 0;

		/// <summary>
		/// Protected DrawLines, for underlying implementation to override
		/// </summary>
		/// <param name="vertexArray"></param>
		virtual void IDrawLines(VertexArrayRef vertexArray) = 0;
	private:
		/// <summary>
		/// the unique pointer to the underlying RenderCommand instance.
		/// </summary>
		static std::unique_ptr<RenderCommand> s_RC;
	};

}