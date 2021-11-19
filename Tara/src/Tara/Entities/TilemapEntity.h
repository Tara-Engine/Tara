#pragma once
#include "Tara/Core/Layer.h"
#include "Tara/Core/Entity.h"
#include "Tara/Asset/Tileset.h"
#include "Tara/Math/Extensions.h" //hashing for glm types


namespace Tara {

	REFTYPE(TilemapEntity);
	NOREFTYPE(TilemapEntity);

	/// <summary>
	/// TileChunk is a 32*32 chunk of tiles (1024 tiles)
	/// It heap-allocates its tile data
	/// </summary>
	struct TileChunk {
		/// <summary>
		/// Static width
		/// </summary>
		const static int32_t WIDTH = 32;

		/// <summary>
		/// pointer to the tile data
		/// </summary>
		uint32_t* Tiles;
		
		/// <summary>
		/// default constructor
		/// </summary>
		TileChunk();

		/// <summary>
		/// move constructor. Invalidates the old chunk
		/// </summary>
		/// <param name="old"></param>
		TileChunk(TileChunk&& old) noexcept;

		/// <summary>
		/// deleted copy constructor
		/// </summary>
		/// <param name="src"></param>
		TileChunk(const TileChunk& src) = delete;
		
		inline TileChunk operator=(TileChunk&& old) {
			return TileChunk(std::move(old));
		}

		///destructor
		~TileChunk();

		/// <summary>
		/// Raw get a tile from the chunk
		/// Coordinates are relative to the chunk itself
		/// </summary>
		/// <param name="x">the x coord</param>
		/// <param name="y">the y coord</param>
		/// <returns>the tile id</returns>
		uint32_t GetTile(int32_t x, int32_t y);

		/// <summary>
		/// Raw set a tile in the chunk
		/// Coordinates are relative to the chunk itself
		/// </summary>
		/// <param name="x">the x coord</param>
		/// <param name="y">the y coord</param>
		/// <param name="tileID">the new tile id</param>
		void SetTile(int32_t x, int32_t y, uint32_t tileID);
	};


	/// <summary>
	/// A single layer in a tilemap
	/// </summary>
	class TileLayer {
		friend class TilemapEntity; //so the tilemap can get private memebers
	public:
		TileLayer() = default;

		TileLayer(TileLayer&& old) 
			: m_Chunks(std::move(old.m_Chunks))
		{}

		~TileLayer();

		/// <summary>
		/// Raw get a tile from the chunk
		/// </summary>
		/// <param name="x">the x coord</param>
		/// <param name="y">the y coord</param>
		/// <returns>the tile id</returns>
		uint32_t GetTile(int32_t x, int32_t y);

		/// <summary>
		/// Raw set a tile in the chunk
		/// </summary>
		/// <param name="x">the x coord</param>
		/// <param name="y">the y coord</param>
		/// <param name="tileID">the new tile id</param>
		void SetTile(int32_t x, int32_t y, uint32_t tileID);

	private:
		std::unordered_map<glm::ivec2, TileChunk*> m_Chunks;
		bool m_Colliding = false;
	};


	/// <summary>
	/// An entity that is an infinite, multi-layered tilemap
	/// </summary>
	class TilemapEntity : public Entity {
	public:
		/// <summary>
		/// When getting and setting a tileID, NO_TILE is the tileID for a blank spot
		/// </summary>
		const static uint32_t NO_TILE = 0xFFFFFFFF;

		/// <summary>
		/// Constructor for TilemapEntity. Use CreateEntity to make
		/// </summary>
		/// <param name="parent"></param>
		/// <param name="owningLayer"></param>
		/// <param name="tilesets"></param>
		/// <param name="transform"></param>
		/// <param name="name"></param>
		TilemapEntity(
			EntityNoRef parent, LayerNoRef owningLayer, std::initializer_list<TilesetRef> tilesets, 
			Transform transform = TRANSFORM_DEFAULT, const std::string& name = "TilemapEntity"
		);

		virtual ~TilemapEntity();

		/// <summary>
		/// Get a tile in the map.
		/// </summary>
		/// <param name="x">the x coordinate of the tile</param>
		/// <param name="y">the y coordinate of the tile</param>
		/// <param name="layer">the layer coordinate of the tile</param>
		/// <returns>the tileID</returns>
		uint32_t GetTile(int32_t x, int32_t y, int32_t layer);

		/// <summary>
		/// Get a tile in the map.
		/// </summary>
		/// <param name="pos">the coordinates of the tile</param>
		/// <returns>the tileID</returns>
		inline uint32_t GetTile(glm::ivec3 pos) { return GetTile(pos.x, pos.y, pos.z); };

		/// <summary>
		/// Get a tile in the map.
		/// </summary>
		/// <param name="pos">the coordinates of the tile</param>
		/// <returns>the tileID</returns>
		inline uint32_t GetTile(Vector pos) { return GetTile((int32_t)pos.x, (int32_t)pos.y, (int32_t)pos.z); }

		/// <summary>
		/// Set a tile in the map. If there was any metadata for that cell, it is removed.
		/// </summary>
		/// <param name="x">the x coordinate of the tile</param>
		/// <param name="y">the y coordinate of the tile</param>
		/// <param name="layer">the layer coordinate of the tile</param>
		/// <param name="tileID">the new tileID</param>
		inline void SetTile(int32_t x, int32_t y, int32_t layer, uint32_t tileID) { SwapTile(x, y, layer, tileID); WipeCellMetadata(x, y, layer); }

		/// <summary>
		/// Set a tile in the map
		/// </summary>
		/// <param name="pos">the coordinates of the tile</param>
		/// <param name="tileID">the new tileID</param>
		inline void SetTile(glm::ivec3 pos, uint32_t tileID) { SetTile(pos.x, pos.y, pos.z, tileID); }

		/// <summary>
		/// Set a tile in the map
		/// </summary>
		/// <param name="pos">the coordinates of the tile</param>
		/// <param name="tileID">the new tileID</param>
		inline void SetTile(Vector pos, uint32_t tileID) { SetTile((int32_t)pos.x, (int32_t)pos.y, (int32_t)pos.z, tileID); }

		/// <summary>
		/// Sets a tile, but does not change metadata
		/// </summary>
		/// <param name="x">the x coordinate of the tile</param>
		/// <param name="y">the y coordinate of the tile</param>
		/// <param name="layer">the layer coordinate of the tile</param>
		/// <param name="tileID">the new tileID</param>
		void SwapTile(int32_t x, int32_t y, int32_t layer, uint32_t tileID);
		
		/// <summary>
		/// Set a tile in the map, but does not change metadata
		/// </summary>
		/// <param name="pos">the coordinates of the tile</param>
		/// <param name="tileID">the new tileID</param>
		inline void SwapTile(glm::ivec3 pos, uint32_t tileID) { SwapTile(pos.x, pos.y, pos.z, tileID); }

		/// <summary>
		/// Set a tile in the map, but does not change metadata
		/// </summary>
		/// <param name="pos">the coordinates of the tile</param>
		/// <param name="tileID">the new tileID</param>
		inline void SwapTile(Vector pos, uint32_t tileID) { SwapTile((int32_t)pos.x, (int32_t)pos.y, (int32_t)pos.z, tileID); }

		/// <summary>
		/// Get the number of layers
		/// </summary>
		/// <returns></returns>
		inline int32_t GetLayerCount() const { return (int32_t)m_Layers.size(); }

		/// <summary>
		/// add a new layer to the map
		/// </summary>
		inline void PushLayer();
		
		/// <summary>
		/// Fill the tilemap with data from a Tiled json file. Does not check tilesets
		/// </summary>
		/// <param name="path"></param>
		void FillFromJson(const std::string& path);



		//Metadata-related functions

		/// <summary>
		/// Give a certain cell some metadata. The Tilemap now owns the metadata, and is responsible for cleanup!
		/// </summary>
		/// <param name="x">the x location of the cell</param>
		/// <param name="y">the y location of the cell</param>
		/// <param name="layer">the layer of the cell</param>
		/// <param name="metaData">pointer to the metadata. Should be heap-allocated</param>
		inline void GiveCellMetadata(int32_t x, int32_t y, int32_t layer, void* metaData) { GiveCellMetadata(glm::ivec3{ x,y,layer }, metaData); }
		
		/// <summary>
		/// Give a certain cell some metadata. The Tilemap now owns the metadata, and is responsible for cleanup!
		/// </summary>
		/// <param name="pos">the cell position</param>
		/// <param name="metaData">pointer to the metadata. Should be heap-allocated</param>
		void GiveCellMetadata(glm::ivec3 pos, void* metaData);

		/// <summary>
		/// Give a certain cell some metadata. The Tilemap now owns the metadata, and is responsible for cleanup!
		/// </summary>
		/// <param name="pos">the cell position</param>
		/// <param name="metaData">pointer to the metadata. Should be heap-allocated</param>
		inline void GiveCellMetadata(Vector pos, void* metaData) { GiveCellMetadata(glm::ivec3{(int)pos.x, (int)pos.y, (int)pos.z}, metaData); }



		/// <summary>
		/// Get a pointer to the metadata associated with a cell.
		/// Callspace does not own the data, it is stilled owned by the tileset.
		/// </summary>
		/// <param name="x">the x location of the cell</param>
		/// <param name="y">the y location of the cell</param>
		/// <param name="layer">the layer of the cell</param>
		/// <returns>a non-owning pointer to the metadata</returns>
		inline void* GetCellMetadata(int32_t x, int32_t y, int32_t layer) { return GetCellMetadata(glm::ivec3{x, y, layer}); }

		/// <summary>
		/// Get a pointer to the metadata associated with a cell.
		/// Callspace does not own the data, it is stilled owned by the tileset.
		/// </summary>
		/// <param name="pos">the cell position</param>
		/// <returns>a non-owning pointer to the metadata</returns>
		void* GetCellMetadata(glm::ivec3 pos);

		/// <summary>
		/// Get a pointer to the metadata associated with a cell.
		/// Callspace does not own the data, it is stilled owned by the tileset.
		/// </summary>
		/// <param name="pos">the cell position</param>
		/// <returns>a non-owning pointer to the metadata</returns>
		inline void* GetCellMetadata(Vector pos) { return GetCellMetadata(glm::ivec3{ (int)pos.x, (int)pos.y, (int)pos.z }); }



		/// <summary>
		/// Get a pointer to the metadata associated with a cell, and clear it from that cell.
		/// That cell will no longer have control of the metadata, the callspace owns it now.
		/// </summary>
		/// <param name="x">the x location of the cell</param>
		/// <param name="y">the y location of the cell</param>
		/// <param name="layer">the layer of the cell</param>
		/// <returns>an owning pointer to the metadata</returns>
		inline void* TakeCellMetadata(int32_t x, int32_t y, int32_t layer) { return TakeCellMetadata(glm::ivec3{ x, y, layer }); }

		/// <summary>
		/// Get a pointer to the metadata associated with a cell, and clear it from that cell.
		/// That cell will no longer have control of the metadata, the callspace owns it now.
		/// </summary>
		/// <param name="pos">the cell position</param>
		/// <returns>a non-owning pointer to the metadata</returns>
		void* TakeCellMetadata(glm::ivec3 pos);

		/// <summary>
		/// Get a pointer to the metadata associated with a cell, and clear it from that cell.
		/// That cell will no longer have control of the metadata, the callspace owns it now.
		/// </summary>
		/// <param name="pos">the cell position</param>
		/// <returns>a non-owning pointer to the metadata</returns>
		inline void* TakeCellMetadata(Vector pos) { return TakeCellMetadata(glm::ivec3{ (int)pos.x, (int)pos.y, (int)pos.z }); }



		/// <summary>
		/// Get the metadata associated with the tileID of a tile in a cell, from what tileset it is a part of.
		/// Callspace does not own the data, it is stilled owned by the tileset.
		/// </summary>
		/// <param name="x">the x location of the cell</param>
		/// <param name="y">the y location of the cell</param>
		/// <param name="layer">the layer of the cell</param>
		/// <returns>a non-owning pointer to the metadata</returns>
		void* GetTileMetadata(int32_t x, int32_t y, int32_t layer);

		/// <summary>
		/// Get the metadata associated with the tileID of a tile in a cell, from what tileset it is a part of.
		/// Callspace does not own the data, it is stilled owned by the tileset.
		/// </summary>
		/// <param name="pos">the cell position</param>
		/// <returns>a non-owning pointer to the metadata</returns>
		inline void* GetTileMetadata(glm::ivec3 pos) { return GetTileMetadata(pos.x, pos.y, pos.z); } ;

		/// <summary>
		/// Get the metadata associated with the tileID of a tile in a cell, from what tileset it is a part of.
		/// Callspace does not own the data, it is stilled owned by the tileset.
		/// </summary>
		/// <param name="pos">the cell position</param>
		/// <returns>a non-owning pointer to the metadata</returns>
		inline void* GetTileMetadata(Vector pos) { return GetTileMetadata((int)pos.x, (int)pos.y, (int)pos.z); }



		/// <summary>
		/// Wipe the metadata from a cell. The metadata is deleted, and the cell is reverted to a non-metadata state
		/// </summary>
		/// <param name="x">the x location of the cell</param>
		/// <param name="y">the y location of the cell</param>
		/// <param name="layer">the layer of the cell</param>
		inline void WipeCellMetadata(int32_t x, int32_t y, int32_t layer) { WipeCellMetadata(glm::ivec3{ x, y, layer }); }

		/// <summary>
		/// Wipe the metadata from a cell. The metadata is deleted, and the cell is reverted to a non-metadata state
		/// </summary>
		/// <param name="pos">the cell position</param>
		/// <returns>a non-owning pointer to the metadata</returns>
		void WipeCellMetadata(glm::ivec3 pos);

		/// <summary>
		/// Wipe the metadata from a cell. The metadata is deleted, and the cell is reverted to a non-metadata state
		/// </summary>
		/// <param name="pos">the cell position</param>
		/// <returns>a non-owning pointer to the metadata</returns>
		inline void WipeCellMetadata(Vector pos) { WipeCellMetadata(glm::ivec3{ (int)pos.x, (int)pos.y, (int)pos.z }); }



		/// <summary>
		/// Set if a layer is a colliding layer
		/// </summary>
		/// <param name="layer"></param>
		/// <param name="colliding"></param>
		inline void SetLayerColliding(int32_t layer, bool colliding) { if (layer < m_Layers.size()) { m_Layers[layer].m_Colliding = colliding; } }
		
		/// <summary>
		/// Get if a layer is a colliding layer
		/// </summary>
		/// <param name="layer"></param>
		/// <returns></returns>
		inline bool GetLayerColliding(int32_t layer) {if (layer < m_Layers.size()) { return m_Layers[layer].m_Colliding; }else { return false; }}
	public:

		inline virtual BoundingBox GetSpecificBoundingBox() const override { return m_Bounds * GetWorldTransform(); };

		void OnDraw(float deltaTime) override;

		virtual bool ConfirmOverlap(EntityRef other) override;

	public:
		/// <summary>
		/// Get the chunk index and index into the chunk based off of a mapspace index. (to do this for coords, call onece for x, again for y)
		/// </summary>
		/// <param name="index">the worlspace index</param>
		/// <returns>pair: (chunk index, index into chunk)</returns>
		static std::pair<int32_t, int32_t> ToChunkIndex(int32_t index);

	public:
		//Lua stuff
		uint32_t __SCRIPT__GetTile(sol::object a, sol::object b, sol::object c);
		void __SCRIPT__SetTile(sol::object a, sol::object b, sol::object c, sol::object d);
		void __SCRIPT__SwapTile(sol::object a, sol::object b, sol::object c, sol::object d);
		

		static void RegisterLuaType(sol::state& lua);
		
	private:
		std::vector<TilesetRef> m_Tilesets;
		std::vector<TileLayer> m_Layers; //TileLayer is stack, not pointer, cause its only the size of an unordered_list. 
		std::unordered_map<glm::ivec3, void*> m_CellMetadata;
		BoundingBox m_Bounds;
	};


}

