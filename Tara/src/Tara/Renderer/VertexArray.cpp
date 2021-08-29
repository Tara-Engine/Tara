#include "tarapch.h"
#include "VertexArray.h"

//platform-specific
#include "Platform/OpenGL/OpenGLVertexArray.h"
namespace Tara {

    VertexArrayRef Tara::VertexArray::Create()
    {
        switch (Renderer::GetRenderBackend()) {
        case RenderBackend::OpenGl: return std::make_shared<OpenGLVertexArray>();

        case RenderBackend::None: ABORT_F("Renderbackend::None not supported!");
        }
        return VertexArrayRef();
    }

}
