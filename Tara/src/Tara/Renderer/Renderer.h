#pragma once
#include "tarapch.h"

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

	private:
		/// <summary>
		/// Stored rendering backend
		/// </summary>
		static RenderBackend s_RenderBackend;

	};

}