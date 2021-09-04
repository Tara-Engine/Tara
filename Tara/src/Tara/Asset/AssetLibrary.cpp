#include "tarapch.h"
#include "AssetLibrary.h"

namespace Tara {
    AssetLibrary::AssetLibrary()
        : m_NextID(0x0)
    {
    }

    AssetLibrary* AssetLibrary::Get()
    {
        static AssetLibrary lib;
        return &lib;
    }

    AssetRef AssetLibrary::GetAsset(const std::string& name)
    {
        auto ast = m_Assets.find(name);
        if (ast != m_Assets.end()) {
            return ast->second;
        }
        else {
            return nullptr;
        }
    }


    bool AssetLibrary::RegisterAsset(AssetRef asset)
    {
        auto ast = m_Assets.find(asset->GetAssetName());
        if (ast != m_Assets.end()) {
            return false;
        }
        m_Assets[asset->GetAssetName()] = asset;
        return true;
    }


    std::string  AssetLibrary::GetNextId()
    {
        std::stringstream ss;
        ss << m_NextID++;
        return ss.str();
    }


    std::string GetAssetNameFromPath(const std::string& path)
    {
        std::string result = path;
        //Make sure to handle both types of slashes, mainly because windows is stupid
        std::replace(result.begin(), result.end(), '\\', '.');
        std::replace(result.begin(), result.end(), '/', '.');
        return result;
    }
}