#include "tarapch.h"
#include "Tara/Renderer/Renderer.h"

namespace Tara {
	//default to OpenGL renderbackend
	RenderBackend Renderer::s_RenderBackend = RenderBackend::OpenGl;
}