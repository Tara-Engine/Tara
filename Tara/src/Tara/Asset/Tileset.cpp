#include "tarapch.h"
#include "Tileset.h"
#include "Tara/Asset/AssetLibrary.h"
#include "nlohmann/json.hpp"
#include <fstream>

namespace Tara{
    Tileset::Tileset(Tara::Texture2DRef texture, float tileWidth, float tileHeight, float spacing, float margin, const std::string& name)
        :Asset(name), m_Texture(texture), m_TileWidth(tileWidth), m_TileHeight(tileHeight), m_Spacing(spacing), m_Margin(margin), m_FilePath("")
    {
        m_TileCount = GetTileCountX() * GetTileCountY();
    }

    Tileset::Tileset(const std::string& filePath, const std::string& name)
        : Asset(name), m_FilePath(filePath)
    {
        std::ifstream file(m_FilePath);
        nlohmann::json json;
        file >> json;

        std::string imagePath = json["image"].get<std::string>();
        m_Margin = json["margin"].get<float>();
        m_Spacing = json["spacing"].get<float>();
        m_TileWidth = json["tilewidth"].get<float>();
        m_TileHeight = json["tileheight"].get<float>();
        m_Texture = Tara::Texture2D::Create(imagePath, GetAssetName() + "_Texture");
        m_TileCount = GetTileCountX() * GetTileCountY();

        uint32_t loadTileCount = json["tilecount"].get<uint32_t>();
        if (loadTileCount != m_TileCount) {
            LOG_S(ERROR) << "Issue parsing JSON Tileset file: loaded tile count and calculated tile count do not match!";
        }
        uint32_t loadCols = json["columns"].get<uint32_t>();
        if (loadCols != GetTileCountX()) {
            LOG_S(ERROR) << "Issue parsing JSON Tileset file: loaded tileset columnts and calculated columns do not match!";
        }
    }

    Tileset::~Tileset()
    {
        //cleanup remaining tile metadata
        int cnt = 0;
        for (auto& kv : m_TileMetadata) {
            if (kv.second) {
                delete kv.second;
                kv.second = nullptr;
                cnt++;
            }
        }
        LOG_S(INFO) << "Tileset destructor running. Metadata chunks cleaned: " << cnt;
    }

    TilesetRef Tileset::Create(Tara::Texture2DRef texture, float tileWidth, float tileHeight, float spacing, float margin, const std::string& name)
    {
        auto ref = AssetLibrary::Get()->GetAssetIf<Tileset>(name);
        if (ref == nullptr) {
            ref = std::make_shared<Tileset>(texture, tileWidth, tileHeight, spacing, margin, name);
            AssetLibrary::Get()->RegisterAsset(ref);
        }
        return ref;
    }

    TilesetRef Tileset::Create(const std::string& filePath, const std::string& name)
    {
        auto ref = AssetLibrary::Get()->GetAssetIf<Tileset>(name);
        if (ref == nullptr) {
            ref = std::make_shared<Tileset>(filePath, name);
            AssetLibrary::Get()->RegisterAsset(ref);
        }
        return ref;
    }

    std::pair<glm::vec2, glm::vec2> Tileset::GetTileUVs(uint32_t index)
    {
        if (index >= m_TileCount) {
            LOG_S(WARNING) << "Attempted to get a tile with index " << index << " which is greater than the size of the tileset!";
            return std::make_pair(glm::vec2{ 0,0 }, glm::vec2{ 1,1 });
        }
        uint32_t tilesX = GetTileCountX();
        //get the x, y coords of tile, with y invertex
        float x = (float)(index % tilesX);
        float y = (float)(GetTileCountY() - (index / tilesX) - 1);
        //get the UVs in pixel coords
        glm::vec2 uvMin{ 
            m_Margin + x * (m_TileWidth + m_Spacing),
            m_Margin + y * (m_TileHeight + m_Spacing)
        };
        glm::vec2 uvMax = {
            uvMin.x + m_TileWidth,
            uvMin.y + m_TileHeight
        };
        //get the image size
        glm::vec2 texSize{
            m_Texture->GetWidth(),
            m_Texture->GetHeight()
        };
        //scale pixel coord UVs to be 0-1
        uvMin /= texSize;
        uvMax /= texSize;
        //return
        return std::make_pair(uvMin, uvMax);
    }


    void Tileset::GiveTileMetadata(uint32_t index, void* metaData)
    {
        auto& iter = m_TileMetadata.find(index);
        if (iter != m_TileMetadata.end()) {
            //there is already metadata there. We own it
            if (iter->second) {
                delete (iter->second);
                iter->second = nullptr;
            }
        }
        //insert new metadata
        m_TileMetadata.insert_or_assign(index, metaData);
    }

    void* Tileset::GetTileMetadata(uint32_t index)
    {
        auto& iter = m_TileMetadata.find(index);
        if (iter != m_TileMetadata.end()) {
            //there is metadata there. return it without destrying
            return iter->second;
        }
        else {
            //we have nothing, return nullptr
            return nullptr;
        }
    }

    void* Tileset::TakeTileMetadata(uint32_t index)
    {
        auto& iter = m_TileMetadata.find(index);
        if (iter != m_TileMetadata.end()) {
            //there is metadata there. return it without destrying
            void* data = iter->second;
            m_TileMetadata.erase(index); //remove it from metadata
            return data;
        }
        else {
            //we have nothing, return nullptr
            return nullptr;
        }
    }

}
