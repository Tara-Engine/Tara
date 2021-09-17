#include "tarapch.h"
#include "Tara/Renderer/Renderer.h"
#include "Buffer.h"

//platfrom-specific
#include "Platform/OpenGL/OpenGLBuffer.h"
namespace Tara{

    VertexBufferRef VertexBuffer::Create(float* verteces, uint32_t count)
    {
        switch (Renderer::GetRenderBackend()) {
        case RenderBackend::OpenGl : return std::make_shared<OpenGLVertexBuffer>(verteces, count);

        case RenderBackend::None: ABORT_F("Renderbackend::None not supported!");
        }
        return VertexBufferRef();
    }


    IndexBufferRef IndexBuffer::Create(uint32_t* indecies, uint32_t count)
    {
        switch (Renderer::GetRenderBackend()) {
        case RenderBackend::OpenGl: return std::make_shared<OpenGLIndexBuffer>(indecies, count);

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
