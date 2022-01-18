#include "tarapch.h"
#include "RenderTarget.h"
#include "Tara/Renderer/Renderer.h"
#include "Tara/Asset/AssetLibrary.h"

//platform-dependant
#include "Platform/OpenGL/OpenGlRenderTarget2D.h"
#include "Platform/OpenGL/OpenGlRenderTargetCubemap.h"

namespace Tara {

    RenderTarget2DRef RenderTarget2D::Create(uint32_t width, uint32_t height, uint32_t colorTargets, InternalType type, bool depthIsTexture, const std::string& name)
    {
        RenderTarget2DRef ref;
        ref = AssetLibrary::Get()->GetAssetIf<RenderTarget2D>(name);
        if (ref == nullptr) {
            switch (Renderer::GetRenderBackend()) { //GetAssetNameFromPath(path)
            case RenderBackend::OpenGl: ref = std::make_shared<OpenGLRenderTarget2D>(width, height, colorTargets, type, depthIsTexture, name); break;

            case RenderBackend::None: ABORT_F("Renderbackend::None not supported!");
            }
            AssetLibrary::Get()->RegisterAsset(ref);
        }
        return ref;
    }
    RenderTarget2DRef RenderTarget2D::Create(uint32_t width, uint32_t height, const std::string& name)
    {
        return Create(width, height, 1, InternalType::UINT8, false, name);
    }

    void RenderTarget::RenderTo(bool render)
    {
        RenderCommand::RenderToTartet(std::dynamic_pointer_cast<RenderTarget>(shared_from_this()), render);
    }


    RenderTargetCubemapRef RenderTargetCubemap::Create(uint32_t width, uint32_t height, uint32_t colorTargets, InternalType type, const std::string& name)
    {
        RenderTargetCubemapRef ref;
        ref = AssetLibrary::Get()->GetAssetIf<RenderTargetCubemap>(name);
        if (ref == nullptr) {
            switch (Renderer::GetRenderBackend()) { //GetAssetNameFromPath(path)
            case RenderBackend::OpenGl: ref = std::make_shared<OpenGLRenderTargetCubemap>(width, height, colorTargets, type, name); break;

            case RenderBackend::None: ABORT_F("Renderbackend::None not supported!");
            }
            AssetLibrary::Get()->RegisterAsset(ref);
        }
        return ref;
    }

}