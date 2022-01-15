#include "tarapch.h"
#include "RenderTarget.h"
#include "Tara/Renderer/Renderer.h"
#include "Tara/Asset/AssetLibrary.h"

//platform-dependant
#include "Platform/OpenGL/OpenGlRenderTarget.h"

namespace Tara {

    RenderTargetRef RenderTarget::Create(uint32_t width, uint32_t height, uint32_t colorTargets, InternalType type, bool depthIsTexture, const std::string& name)
    {
        RenderTargetRef ref;
        ref = AssetLibrary::Get()->GetAssetIf<RenderTarget>(name);
        if (ref == nullptr) {
            switch (Renderer::GetRenderBackend()) { //GetAssetNameFromPath(path)
            case RenderBackend::OpenGl: ref = std::make_shared<OpenGLRenderTarget>(width, height, colorTargets, type, depthIsTexture, name); break;

            case RenderBackend::None: ABORT_F("Renderbackend::None not supported!");
            }
            AssetLibrary::Get()->RegisterAsset(ref);
        }
        return ref;
    }
    RenderTargetRef RenderTarget::Create(uint32_t width, uint32_t height, const std::string& name)
    {
        return Create(width, height, 1, InternalType::UINT8, false, name);
    }

    void RenderTarget::RenderTo(bool render)
    {
        RenderCommand::RenderToTartet(std::dynamic_pointer_cast<RenderTarget>(shared_from_this()), render);
    }


}