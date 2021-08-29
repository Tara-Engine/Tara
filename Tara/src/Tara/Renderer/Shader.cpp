#include "tarapch.h"
#include "Shader.h"

//platform-specific includes
#include "Platform/OpenGL/OpenGLShader.h"

namespace Tara {

    uint32_t Tara::Shader::GetDatatypeSize(Shader::Datatype type)
    {
        switch (type) {
        case Datatype::None:    return 0;
        case Datatype::Float:   return 4;
        case Datatype::Float2:  return 8;
        case Datatype::Float3:  return 12;
        case Datatype::Float4:  return 16;
        case Datatype::Int:     return 4;
        case Datatype::Int2:    return 8;
        case Datatype::Int3:    return 12;
        case Datatype::Int4:    return 16;
        case Datatype::Mat3:    return 36;
        case Datatype::Mat4:    return 64;
        case Datatype::Bool:    return 1; //TODO: confirm size is actually 1!
        }
        return 0;
    }

    uint32_t Shader::GetDatatypeCount(Shader::Datatype type)
    {
        switch (type) {
        case Datatype::None:    return 0;
        case Datatype::Float:   return 1;
        case Datatype::Float2:  return 2;
        case Datatype::Float3:  return 3;
        case Datatype::Float4:  return 4;
        case Datatype::Int:     return 1;
        case Datatype::Int2:    return 2;
        case Datatype::Int3:    return 3;
        case Datatype::Int4:    return 4;
        case Datatype::Mat3:    return 9;
        case Datatype::Mat4:    return 16;
        case Datatype::Bool:    return 1;
        }
        return 0;
    }
    ShaderRef Shader::Create(Shader::SourceType type, const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        switch (Renderer::GetRenderBackend()) {
        case RenderBackend::OpenGl: return std::make_shared<OpenGLShader>(type, vertexSrc, fragmentSrc);

        case RenderBackend::None: ABORT_F("Renderbackend::None not supported!");
        }
        return ShaderRef();
    }
}