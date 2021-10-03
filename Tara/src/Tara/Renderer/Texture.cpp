#include "tarapch.h"
#include "Texture.h"
#include "Tara/Renderer/Renderer.h"
#include "Tara/Asset/AssetLibrary.h"

//platform-dependant
#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace Tara{
    Texture2DRef Texture2D::Create(const std::string& path, const std::string& name)
    {
        std::string lName = name;
        if (name == "") { lName = GetAssetNameFromPath(path); }
        Texture2DRef ref;
        switch (Renderer::GetRenderBackend()) { //GetAssetNameFromPath(path)
        case RenderBackend::OpenGl: ref = std::make_shared<OpenGLTexture2D>(path, lName); break;

        case RenderBackend::None: ABORT_F("Renderbackend::None not supported!");
        }
        AssetLibrary::Get()->RegisterAsset(ref);
        return ref;
    }
}
