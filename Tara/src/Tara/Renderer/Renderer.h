#pragma once
#include "tarapch.h"
#include "Tara/Renderer/Shader.h"
#include "Tara/Renderer/VertexArray.h"
#include "Tara/Renderer/Camera.h"
#include "Tara/Math/Types.h"

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

		static void BeginScene(const CameraRef camera);
		static void EndScene();

		static void Draw(VertexArrayRef vertexArray, ShaderRef shader, Transform transform);

	private:

		struct SceneData {
			CameraRef camera;
		};

		/// <summary>
		/// Stored rendering backend
		/// </summary>
		static RenderBackend s_RenderBackend;
		static SceneData s_SceneData;
	};

}