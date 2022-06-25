#include "tarapch.h"
#include "Texture.h"
#include "Tara/Renderer/Renderer.h"
#include "Tara/Asset/AssetLibrary.h"

//platform-dependant
#include "Platform/OpenGL/OpenGLTexture2D.h"
#include "Platform/OpenGL/OpenGLTextureCubemap.h"


namespace Tara{
    Texture::Filtering Texture::s_DefaultTextureFiltering = Texture::Filtering::Nearest;
    Texture::Wrapping Texture::s_DefaultTextureWrapping = Texture::Wrapping::Mirror;

    Texture2DRef Texture2D::Create(const std::string& path, const std::string& name)
    {
        std::string lName = name;
        if (name == "") { lName = GetAssetNameFromPath(path); }
        Texture2DRef ref;
        ref = AssetLibrary::Get()->GetAssetIf<Texture2D>(lName);
        if (ref == nullptr){
            try {
                switch (Renderer::GetRenderBackend()) { //GetAssetNameFromPath(path)
                case RenderBackend::OpenGl: ref = std::make_shared<OpenGLTexture2D>(path, lName); break;

                case RenderBackend::None: ABORT_F("Renderbackend::None not supported!");
                }
                AssetLibrary::Get()->RegisterAsset(ref);
            }
            catch (std::exception e) {
                LOG_S(ERROR) << "File Loading Error: " << e.what();
                return nullptr;
            }
        }
        return ref;
    }


    Texture2DRef Texture2D::Create(const uint8_t* bytes, uint32_t width, uint32_t height, uint32_t bytesPerPixel, const std::string& name)
    {
        Texture2DRef ref;
        ref = AssetLibrary::Get()->GetAssetIf<Texture2D>(name);
        if (ref == nullptr) {
            switch (Renderer::GetRenderBackend()) { //GetAssetNameFromPath(path)
            case RenderBackend::OpenGl: ref = std::make_shared<OpenGLTexture2D>(bytes, width, height, bytesPerPixel, name); break;

            case RenderBackend::None: ABORT_F("Renderbackend::None not supported!");
            }
            AssetLibrary::Get()->RegisterAsset(ref);
        }
        return ref;
    }

    TextureCubemapRef TextureCubemap::CreateHDRI(const std::string& path, uint32_t size, const std::string& name)
    {
        std::string lName = name;
        if (name == "") { lName = GetAssetNameFromPath(path); }
        TextureCubemapRef ref;
        ref = AssetLibrary::Get()->GetAssetIf<TextureCubemap>(lName);
        if (ref == nullptr) {
            try {
                switch (Renderer::GetRenderBackend()) { //GetAssetNameFromPath(path)
                case RenderBackend::OpenGl: ref = std::make_shared<OpenGLTextureCubemap>(path, size, lName); break;

                case RenderBackend::None: ABORT_F("Renderbackend::None not supported!");
                }
                AssetLibrary::Get()->RegisterAsset(ref);
            }
            catch (std::exception e) {
                LOG_S(ERROR) << "File Loading Error: " << e.what();
                return nullptr;
            }
        }
        return ref;
    }

}
