#pragma once
#include "tarapch.h"
#include "Tara/Renderer/Shader.h"
#include "Tara/Renderer/VertexArray.h"
#include "Tara/Renderer/Camera.h"
#include "Tara/Math/Types.h"
#include "Tara/Renderer/Texture.h"
#include "Tara/Math/BoundingBox.h"

namespace Tara {

	/// <summary>
	/// A enum of the types of rendering backends available
	/// </summary>
	enum class RenderBackend {
		None = 0,
		OpenGl
	};

	/// <summary>
	/// Renderer
	/// Mostly static class that is used for rendering things in worldspace. 
	/// This includes any 2d or 3d world components, but not UI
	/// UI rendering should be done with Renderer2D <NOT MADE YET> that renders in screenspace
	/// </summary>
	class Renderer {
	public:
		/// <summary>
		/// Get the rendering backend
		/// </summary>
		/// <returns>the rendering backend</returns>
		static RenderBackend GetRenderBackend() { return s_RenderBackend; }

		/// <summary>
		/// Begin a scene to render with a given camera
		/// </summary>
		/// <param name="camera">the camera to render with</param>
		static void BeginScene(const CameraRef camera);

		/// <summary>
		/// End the current scene
		/// </summary>
		static void EndScene();

		/// <summary>
		/// draw a arbitrary vertex array
		/// </summary>
		/// <param name="vertexArray">the vertex array to draw</param>
		/// <param name="shader">the shader to draw with</param>
		/// <param name="transform">where the vertex array is</param>
		static void Draw(VertexArrayRef vertexArray, ShaderRef shader, Transform transform);

		/// <summary>
		/// draw a arbitrary vertex array
		/// </summary>
		/// <param name="vertexArray">the vertex array to draw</param>
		/// <param name="shader">the shader to draw with</param>
		/// <param name="transform">where the vertex array is</param>
		static void DrawLines(VertexArrayRef vertexArray, ShaderRef shader, Transform transform);

		/// <summary>
		/// draw a texture on a 1x1 quad
		/// </summary>
		/// <param name="texture">the texture to draw</param>
		/// <param name="transform">the transform of the quad</param>
		static void Quad(Texture2DRef texture, Transform transform);

		/// <summary>
		/// draw a colored 1x1 quad
		/// </summary>
		/// <param name="color">the color</param>
		/// <param name="transform">the transform of the quad</param>
		static void Quad(glm::vec4 color, Transform transform);

		/// <summary>
		/// Draw a bounding box
		/// </summary>
		/// <param name="box">the box to draw</param>
		/// <param name="color">the color to draw it in</param>
		static void DrawBoundingBox(const BoundingBox& box, glm::vec4 color = { 0,0,0,1 });

	private:

		/// <summary>
		/// Structure that holds data about the current scene.
		/// </summary>
		struct SceneData {
			CameraRef camera;
		};

		/// <summary>
		/// Stored rendering backend
		/// </summary>
		static RenderBackend s_RenderBackend;
		
		/// <summary>
		/// Stored scene data.
		/// </summary>
		static SceneData s_SceneData;

		static VertexArrayRef s_QuadArray;
		static VertexArrayRef s_BoxArray;
		static ShaderRef s_TextureQuadShader;
		static ShaderRef s_ColorQuadShader;
	};

}