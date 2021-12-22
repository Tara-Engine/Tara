#include "tarapch.h"
#include "Shader.h"
#include "Tara/Renderer/Renderer.h"
#include "Tara/Asset/AssetLibrary.h"
#include "glm/gtc/type_ptr.hpp"

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





    void Shader::Send(const std::string& name, const Uniform& uniform)
    {
        RenderCommand::SendUniform(std::dynamic_pointer_cast<Shader>(shared_from_this()), name, uniform);
    }

    void Shader::Send(const std::string& name, float value)
    {
        UniformData data{ 0 };
        data.Float1 = value;
        Send(name, Uniform{ UniformType::Float1, data });
    }
    void Shader::Send(const std::string& name, const glm::vec2& value)
    {
        UniformData data{ 0 };
        data.Float2 = value;
        Send(name, Uniform{ UniformType::Float2, data });
    }
    void Shader::Send(const std::string& name, const glm::vec3& value)
    {
        UniformData data{ 0 };
        data.Float3 = value;
        Send(name, Uniform{ UniformType::Float3, data });
    }
    void Shader::Send(const std::string& name, const glm::vec4& value)
    {
        UniformData data{ 0 };
        data.Float4 = value;
        Send(name, Uniform{ UniformType::Float4, data });
    }


    void Shader::Send(const std::string& name, int32_t value)
    {
        UniformData data{ 0 };
        data.Int1 = value;
        Send(name, Uniform{ UniformType::Int1, data });
    }
    void Shader::Send(const std::string& name, const glm::ivec2& value)
    {
        UniformData data{ 0 };
        data.Int2 = value;
        Send(name, Uniform{ UniformType::Int2, data });
    }
    void Shader::Send(const std::string& name, const glm::ivec3& value)
    {
        UniformData data{ 0 };
        data.Int3 = value;
        Send(name, Uniform{ UniformType::Int3, data });
    }
    void Shader::Send(const std::string& name, const glm::ivec4& value)
    {
        UniformData data{ 0 };
        data.Int4 = value;
        Send(name, Uniform{ UniformType::Int4, data });
    }


    void Shader::Send(const std::string& name, uint32_t value)
    {
        UniformData data{ 0 };
        data.UInt1 = value;
        Send(name, Uniform{ UniformType::UInt1, data });
    }
    void Shader::Send(const std::string& name, const glm::uvec2& value)
    {
        UniformData data{ 0 };
        data.UInt2 = value;
        Send(name, Uniform{ UniformType::UInt2, data });
    }
    void Shader::Send(const std::string& name, const glm::uvec3& value)
    {
        UniformData data{ 0 };
        data.UInt3 = value;
        Send(name, Uniform{ UniformType::UInt3, data });
    }
    void Shader::Send(const std::string& name, const glm::uvec4& value)
    {
        UniformData data{ 0 };
        data.UInt4 = value;
        Send(name, Uniform{ UniformType::UInt4, data });
    }


    void Shader::Send(const std::string& name, float* value, int count)
    {
        UniformData data{ 0 };
        data.FloatVector = {value, count, 1}; //implicit move after construct
        Send(name, Uniform{ UniformType::Float1Vector, std::move(data) });
    }
    void Shader::Send(const std::string& name, glm::vec2* value, int count)
    {
        UniformData data{ 0 };
        data.FloatVector = { (float*)value, count, 2 };
        Send(name, Uniform{ UniformType::Float2Vector, std::move(data) });
    }
    void Shader::Send(const std::string& name, glm::vec3* value, int count)
    {
        UniformData data{ 0 };
        data.FloatVector = { (float*)value, count, 3 };
        Send(name, Uniform{ UniformType::Float3Vector, std::move(data) });
    }
    void Shader::Send(const std::string& name, glm::vec4* value, int count)
    {
        UniformData data{ 0 };
        data.FloatVector = { (float*)value, count, 4 };
        Send(name, Uniform{ UniformType::Float4Vector, std::move(data) });
    }


    void Shader::Send(const std::string& name, int32_t* value, int count)
    {
        UniformData data{ 0 };
        data.IntVector = { (int32_t*)value, count, 1 };
        Send(name, Uniform{ UniformType::Int1Vector, std::move(data) });
    }
    void Shader::Send(const std::string& name, glm::ivec2* value, int count)
    {
        UniformData data{ 0 };
        data.IntVector = { (int32_t*)value, count, 2 };
        Send(name, Uniform{ UniformType::Int2Vector, std::move(data) });
    }
    void Shader::Send(const std::string& name, glm::ivec3* value, int count)
    {
        UniformData data{ 0 };
        data.IntVector = { (int32_t*)value, count, 3 };
        Send(name, Uniform{ UniformType::Int3Vector, std::move(data) });
    }
    void Shader::Send(const std::string& name, glm::ivec4* value, int count)
    {
        UniformData data{ 0 };
        data.IntVector = { (int32_t*)value, count, 4 };
        Send(name, Uniform{ UniformType::Int4Vector, std::move(data) });
    }


    void Shader::Send(const std::string& name, uint32_t* value, int count)
    {
        UniformData data{ 0 };
        data.UIntVector = { (uint32_t*)value, count, 1 };
        Send(name, Uniform{ UniformType::UInt1Vector, std::move(data) });
    }
    void Shader::Send(const std::string& name, glm::uvec2* value, int count)
    {
        UniformData data{ 0 };
        data.UIntVector = { (uint32_t*)value, count, 2 };
        Send(name, Uniform{ UniformType::UInt2Vector, std::move(data) });
    }
    void Shader::Send(const std::string& name, glm::uvec3* value, int count)
    {
        UniformData data{ 0 };
        data.UIntVector = { (uint32_t*)value, count, 3 };
        Send(name, Uniform{ UniformType::UInt3Vector, std::move(data) });
    }
    void Shader::Send(const std::string& name, glm::uvec4* value, int count)
    {
        UniformData data{ 0 };
        data.UIntVector = { (uint32_t*)value, count, 4 };
        Send(name, Uniform{ UniformType::UInt4Vector, std::move(data) });
    }


    void Shader::Send(const std::string& name, const glm::mat2& value)
    {
        UniformData data{ 0 };
        data.FloatVector = {glm::value_ptr(value), 1, 4 };
        Send(name, Uniform{ UniformType::Matrix2Vector, std::move(data) });
    }
    void Shader::Send(const std::string& name, const glm::mat3& value)
    {
        UniformData data{ 0 };
        data.FloatVector = { glm::value_ptr(value), 1, 9 };
        Send(name, Uniform{ UniformType::Matrix3Vector, std::move(data) });
    }
    void Shader::Send(const std::string& name, const glm::mat4& value)
    {
        UniformData data{ 0 };
        data.FloatVector = { glm::value_ptr(value), 1, 16 };
        Send(name, Uniform{ UniformType::Matrix4Vector, std::move(data) });
    }

    void Shader::Send(const std::string& name, glm::mat2* value, int count)
    {
        UniformData data{ 0 };
        data.FloatVector = { (float*)value, 1, 4 };
        Send(name, Uniform{ UniformType::Matrix2Vector, std::move(data) });
    }
    void Shader::Send(const std::string& name, glm::mat3* value, int count)
    {
        UniformData data{ 0 };
        data.FloatVector = { (float*)value, 1, 9 };
        Send(name, Uniform{ UniformType::Matrix3Vector, std::move(data) });
    }
    void Shader::Send(const std::string& name, glm::mat4* value, int count)
    {
        UniformData data{ 0 };
        data.FloatVector = { (float*)value, 1, 16 };
        Send(name, Uniform{ UniformType::Matrix4Vector, std::move(data) });
    }

    void Shader::Send(const std::string& name, glm::mat2x3* value, int count)
    {
        UniformData data{ 0 };
        data.FloatVector = { (float*)value, 1, 6 };
        Send(name, Uniform{ UniformType::Matrix2x3Vector, std::move(data) });
    }
    void Shader::Send(const std::string& name, glm::mat3x2* value, int count)
    {
        UniformData data{ 0 };
        data.FloatVector = { (float*)value, 1, 6 };
        Send(name, Uniform{ UniformType::Matrix3x2Vector, std::move(data) });
    }

    void Shader::Send(const std::string& name, glm::mat2x4* value, int count)
    {
        UniformData data{ 0 };
        data.FloatVector = { (float*)value, 1, 8 };
        Send(name, Uniform{ UniformType::Matrix2x4Vector, std::move(data) });
    }
    void Shader::Send(const std::string& name, glm::mat4x2* value, int count)
    {
        UniformData data{ 0 };
        data.FloatVector = { (float*)value, 1, 8 };
        Send(name, Uniform{ UniformType::Matrix4x2Vector, std::move(data) });
    }

    void Shader::Send(const std::string& name, glm::mat3x4* value, int count)
    {
        UniformData data{ 0 };
        data.FloatVector = { (float*)value, 1, 12 };
        Send(name, Uniform{ UniformType::Matrix3x4Vector, std::move(data) });
    }
    void Shader::Send(const std::string& name, glm::mat4x3* value, int count)
    {
        UniformData data{ 0 };
        data.FloatVector = { (float*)value, 1, 12 };
        Send(name, Uniform{ UniformType::Matrix4x3Vector, std::move(data) });
    }
}