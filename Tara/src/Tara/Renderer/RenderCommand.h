#pragma once
#include "Tara/Renderer/Renderer.h"
#include "Tara/Renderer/VertexArray.h"

namespace Tara {
	

	/// <summary>
	/// An Enum of the drawing types available to the render backend
	/// </summary>
	enum class RenderDrawType {
		/// <summary>
		/// Keep the current drawtype
		/// </summary>
		Keep, 
		/// <summary>
		/// Draw points at each vertex
		/// </summary>
		Points,
		/// <summary>
		/// Draw lines between each pair of vertecies
		/// </summary>
		Lines,
		/// <summary>
		/// Draw triangles with each set of three vertecies
		/// </summary>
		Triangles,
		/// <summary>
		/// Draw lines connecting the vertecies in drawing order
		/// </summary>
		LineStrip,
		/// <summary>
		/// Draw lines connecting the vertecies in drawing order, and close the shape
		/// </summary>
		LineLoop,
		/// <summary>
		/// Draw a triangle strip
		/// </summary>
		TriangleStrip,
		/// <summary>
		/// Draw a triangle fan
		/// </summary>
		TriangleFan
	};

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
		/// Push a new draw type for future draws
		/// </summary>
		/// <param name="drawType">the new draw type</param>
		/// <param name="wireframe">if wireframe should be turned on (false by default)</param>
		static void PushDrawType(RenderDrawType drawType, bool wireframe = false);
		
		/// <summary>
		/// Pop the current drawType for future draws
		/// </summary>
		static void PopDrawType();

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
		/// Draw the current VertexArray, but a specific count of vertecies instead of via an IndexBuffer. 
		/// Does not bind the VertexArray passed to it.
		/// </summary>
		/// <param name="vertexArray"></param>
		inline static void DrawCount(uint32_t count) { s_RC->IDrawCount(count); }

		/// <summary>
		/// Gets the max number of textures that can be sent to the fragment shader.
		/// </summary>
		/// <returns>that number.</returns>
		inline static uint32_t GetMaxTextureSlotsPerShader() {return s_RC->IGetMaxTextureSlotsPerShader();}
	
		/// <summary>
		/// Enable and Disable depth testing in the rendering system. Should be disabled for 2D.
		/// </summary>
		/// <param name="enable"></param>
		inline static void EnableDepthTesting(bool enable) { s_RC->IEnableDepthTesting(enable); }

		inline static void EnableBackfaceCulling(bool enable) { s_RC->IEnableBackfaceCulling(enable); }

	protected:
		/// <summary>
		/// Protected SetClearColor, for underlying implementation to override
		/// </summary>
		/// <param name="r">red component of clear color</param>
		/// <param name="g">green component of clear color</param>
		/// <param name="b">blue component of clear color</param>
		virtual void ISetClearColor(float r, float g, float b) = 0;

		/// <summary>
		/// Protected SetDrawType, for underlying implementation to override
		/// </summary>
		/// <param name="drawType">the drawtype</param>
		/// <param name="wireframe">if wireframe should be turned on</param>
		virtual void ISetDrawType(RenderDrawType drawType, bool wireframe) = 0;

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
		/// Protected DrawCount, for underlying implementation to override
		/// </summary>
		/// <param name="vertexArray"></param>
		virtual void IDrawCount(uint32_t count) = 0;

		/// <summary>
		/// Protected GetMaxTextureSlotsPerShader, for underlying implementation to override
		/// </summary>
		virtual uint32_t IGetMaxTextureSlotsPerShader() = 0;

		/// <summary>
		/// Protected EnableDepthTesting for underlying implementation to override
		/// </summary>
		/// <param name="enable"></param>
		virtual void IEnableDepthTesting(bool enable) = 0;

		/// <summary>
		/// Protected EnableBackfaceCulling for underlying implementation to override
		/// </summary>
		/// <param name="enable"></param>
		virtual void IEnableBackfaceCulling(bool enable) = 0;
	private:

		struct DrawType {
			RenderDrawType Type;
			bool WireFrame;
		};

		/// <summary>
		/// the unique pointer to the underlying RenderCommand instance.
		/// </summary>
		static std::unique_ptr<RenderCommand> s_RC;
		static std::list<DrawType> m_DrawTypeStack;
	};

}