#include "tarapch.h"
#include "Texture.h"
#include "Tara/Renderer/Renderer.h"
#include "Tara/Asset/AssetLibrary.h"

//platform-dependant
#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace Tara{
    Texture2DRef Texture2D::Create(const std::string& path)
    {
        Texture2DRef ref;
        switch (Renderer::GetRenderBackend()) {
        case RenderBackend::OpenGl: ref = std::make_shared<OpenGLTexture2D>(path); break;

        case RenderBackend::None: ABORT_F("Renderbackend::None not supported!");
        }
        AssetLibrary::Get()->RegisterAsset(ref);
        return ref;
    }
}
