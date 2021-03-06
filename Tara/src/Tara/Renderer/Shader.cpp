#include "tarapch.h"
#include "Shader.h"
#include "Tara/Renderer/Renderer.h"
#include "Tara/Asset/AssetLibrary.h"

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
    ShaderRef Shader::Create(const std::string& name, Shader::SourceType type, const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        LOG_S(WARNING) << "Warning: Shader::Create() with a vertex and fragment is depriciated! Use the version with a map from TargetStage to string instead!";
        ShaderRef ref;
        switch (Renderer::GetRenderBackend()) {
        case RenderBackend::OpenGl: {
            std::unordered_map<TargetStage, std::string>  sources;
            sources[TargetStage::Vertex] = vertexSrc;
            sources[TargetStage::Pixel] = fragmentSrc;
            ref = std::make_shared<OpenGLShader>(name, type, sources);
            break;
        }

        case RenderBackend::None: ABORT_F("RenderBackend::None not supported!");
        }
        AssetLibrary::Get()->RegisterAsset(ref);
        return ref;
    }
    ShaderRef Shader::Create(const std::string& name, Shader::SourceType type, std::unordered_map<TargetStage, std::string> sources)
    {
        ShaderRef ref;
        switch (Renderer::GetRenderBackend()) {
        case RenderBackend::OpenGl: ref = std::make_shared<OpenGLShader>(name, type, sources); break;

        case RenderBackend::None: ABORT_F("RenderBackend::None not supported!");
        }
        AssetLibrary::Get()->RegisterAsset(ref);
        return ref;
    }
}