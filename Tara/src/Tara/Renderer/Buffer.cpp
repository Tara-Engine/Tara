#include "tarapch.h"
#include "Tara/Renderer/Renderer.h"
#include "Buffer.h"

//platfrom-specific
#include "Platform/OpenGL/OpenGLBuffer.h"
namespace Tara{

    VertexBufferRef VertexBuffer::Create(const float* vertices, uint32_t count)
    {
        switch (Renderer::GetRenderBackend()) {
        case RenderBackend::OpenGl : return std::make_shared<OpenGLVertexBuffer>(vertices, count);

        case RenderBackend::None: ABORT_F("Renderbackend::None not supported!");
        }
        return VertexBufferRef();
    }

    void VertexBuffer::SetData(const float* data, uint32_t count)
    {
        RenderCommand::SetVertexBufferData(std::dynamic_pointer_cast<VertexBuffer>(shared_from_this()), data, count);
    }

    /*
    VertexBufferRef VertexBuffer::Create(void* vertices, uint32_t size)
    {
        switch (Renderer::GetRenderBackend()) {
        case RenderBackend::OpenGl: return std::make_shared<OpenGLVertexBuffer>(vertices, size);

        case RenderBackend::None: ABORT_F("Renderbackend::None not supported!");
        }
        return VertexBufferRef();
    }
    */

    IndexBufferRef IndexBuffer::Create(const uint32_t* indices, uint32_t count)
    {
        switch (Renderer::GetRenderBackend()) {
        case RenderBackend::OpenGl: return std::make_shared<OpenGLIndexBuffer>(indices, count);

        case RenderBackend::None: ABORT_F("Renderbackend::None not supported!");
        }
        return IndexBufferRef();
    }


    BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
        : m_Elements(elements)
    {
        uint32_t offset = 0;
        for (auto& element : m_Elements) {
            element.Offset = offset;
            offset += element.Size;
        }
        m_Stride = offset;
    }
    
}
