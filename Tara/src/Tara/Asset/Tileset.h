#pragma once
#include "Tara/Asset/Asset.h"
#include "Tara/Renderer/Texture.h"
#include "Tara/Math/Types.h"
#include <any>

namespace Tara {
	REFTYPE(Tileset)

	/// <summary>
	/// A tileset asset
	/// Manages getting the UVs and data for a tile
	/// </summary>
	class Tileset : public Asset {
	public:
		/// <summary>
		/// Construct a raw tilemap. Use static Create function, not raw construction
		/// </summary>
		/// <param name="texture">the base texture to use</param>
		/// <param name="tileWidth">the width (pixels) of a tile</param>
		/// <param name="tileHeight">the height (pixels) of a tile</param>
		/// <param name="spacing">the spacing (pixels) between tiles</param>
		/// <param name="margin">the spacing (pixels) between an edge tile and the image border</param>
		/// <param name="name">the asset name</param>
		Tileset(Tara::Texture2DRef texture, float tileWidth, float tileHeight, float spacing, float margin, const std::string& name);

		/// <summary>
		/// Load a tileset from a JSON file. We use the format exported by Tiled. Use static Create function, not raw construction
		/// </summary>
		/// <param name="filePath">the path to the tiled file</param>
		/// <param name="name">the asset name</param>
		Tileset(const std::string& filePath, const std::string& name);

		/// <summary>
		/// destructor.
		/// </summary>
		virtual ~Tileset();

		/// <summary>
		/// Create a tileset from scratch.
		/// </summary>
		/// <param name="texture">the base texture to use</param>
		/// <param name="tileWidth">the width (pixels) of a tile</param>
		/// <param name="tileHeight">the height (pixels) of a tile</param>
		/// <param name="spacing">the spacing (pixels) between tiles</param>
		/// <param name="margin">the spacing (pixels) between an edge tile and the image border</param>
		/// <param name="name">the asset name</param>
		/// <returns>a reference to the new tileset</returns>
		static TilesetRef Create(Tara::Texture2DRef texture, float tileWidth, float tileHeight, float spacing, float margin, const std::string& name);

		/// <summary>
		/// Create a tileset by loading a JSON file (as exported by Tiled)
		/// </summary>
		/// <param name="filePath"></param>
		/// <param name="name"></param>
		/// <returns></returns>
		static TilesetRef Create(const std::string& filePath, const std::string& name);

		/// <summary>
		/// Get the min and max UVs for a particular tile ID. 
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		std::pair<glm::vec2, glm::vec2> GetTileUVs(uint32_t index);

		
		/// <summary>
		/// Get the texture of the set.
		/// </summary>
		/// <returns></returns>
		inline const Texture2DRef& GetTexture() const { return m_Texture; }

		/// <summary>
		/// set the texture of the set
		/// </summary>
		/// <param name="tex"></param>
		inline void SetTexture(Texture2DRef& tex) { m_Texture = tex; }

		/// <summary>
		/// Get the full tile count
		/// </summary>
		/// <returns></returns>
		inline uint32_t GetTileCount() { return m_TileCount; }

		/// <summary>
		/// Get the margin between the edge tiles and the image border
		/// </summary>
		/// <returns></returns>
		inline float GetMargin() const { return m_Margin; }

		/// <summary>
		/// Set the margin between the edge tiles and the image border
		/// </summary>
		/// <param name="margin"></param>
		inline void SetMargin(float margin) { m_Margin = margin; UpdateTileCount(); }

		/// <summary>
		/// Get the spacing between tiles internally
		/// </summary>
		/// <returns></returns>
		inline float GetSpacing() const { return m_Spacing; }

		/// <summary>
		/// Set the spacing between tiles internally
		/// </summary>
		/// <param name="spacing"></param>
		inline void SetSpacing(float spacing) { m_Spacing = spacing, UpdateTileCount(); }

		/// <summary>
		/// Get the width of a tile
		/// </summary>
		/// <returns></returns>
		inline float GetTileWidth() const { return m_TileWidth; }

		/// <summary>
		/// Set the width of a tile
		/// </summary>
		/// <param name="width"></param>
		inline void SetTileWidth(float width) { m_TileWidth = width; UpdateTileCount(); }

		/// <summary>
		/// Get the height of a tile
		/// </summary>
		/// <returns></returns>
		inline float GetTileHeight() const { return m_TileHeight; }

		/// <summary>
		/// Set the height of a tile
		/// </summary>
		/// <param name="height"></param>
		inline void SetTileHeight(float height) { m_TileHeight = height; UpdateTileCount(); }

		/// <summary>
		/// Get the number of tiles across the image (columns)
		/// </summary>
		/// <returns></returns>
		uint32_t GetTileCountX() const { return (uint32_t)floorf((m_Texture->GetWidth() - (m_Margin * 2) ) / (m_TileWidth + m_Spacing)); }
		
		/// <summary>
		/// Get the number of tiles down the image (rows)
		/// </summary>
		/// <returns></returns>
		uint32_t GetTileCountY() const { return (uint32_t)floorf((m_Texture->GetHeight() - (m_Margin * 2) ) / (m_TileHeight + m_Spacing)); }

		/// <summary>
		/// Give a certain tile ID some metadata.
		/// </summary>
		/// <param name="index">the tile index</param>
		/// <param name="metaData">pointer to the metadata. Should be heap-allocated</param>
		void SetTileMetadata(uint32_t index, const std::any& metaData);

		/// <summary>
		/// Get a pointer to the metadata associated with a tile ID.
		/// </summary>
		/// <param name="index">the tile index</param>
		/// <returns></returns>
		const std::any& GetTileMetadata(uint32_t index);

		/// <summary>
		/// Wipe the metadata associated with an index.
		/// </summary>
		/// <param name="index"></param>
		void WipeTileMetadata(uint32_t index);

	protected:
		inline void UpdateTileCount() { m_TileCount = GetTileCountX() * GetTileCountY(); }

	private:
		Texture2DRef m_Texture;
		uint32_t m_TileCount;
		float m_Margin;
		float m_Spacing;
		float m_TileWidth;
		float m_TileHeight;
		std::string m_FilePath;
		std::unordered_map<unsigned int, std::any> m_TileMetadata;
	};
}
