#pragma once
#include "tarapch.h"

namespace Tara {

	enum class RenderBackend {
		None = 0,
		OpenGl
	};

	class Renderer {
	public:
		static RenderBackend GetRenderBackend() { return s_RenderBackend; }

	private:
		static RenderBackend s_RenderBackend;

	};

}