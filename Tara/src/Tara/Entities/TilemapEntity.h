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

		virtual ~TilemapEntity() {

		}

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
		inline uint32_t GetTile(glm::ivec3 pos) { GetTile(pos.x, pos.y, pos.z); };

		/// <summary>
		/// Get a tile in the map.
		/// </summary>
		/// <param name="pos">the coordinates of the tile</param>
		/// <returns>the tileID</returns>
		inline uint32_t GetTile(Vector pos) { GetTile((int32_t)pos.x, (int32_t)pos.y, (int32_t)pos.z); }

		/// <summary>
		/// Set a tile in the map
		/// </summary>
		/// <param name="x">the x coordinate of the tile</param>
		/// <param name="y">the y coordinate of the tile</param>
		/// <param name="layer">the layer coordinate of the tile</param>
		/// <param name="tileID">the new tileID</param>
		inline void SetTile(int32_t x, int32_t y, int32_t layer, uint32_t tileID) { SwapTile(x, y, layer, tileID); }

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
		inline int32_t GetLayerCount() const { return m_Layers.size(); }

		/// <summary>
		/// add a new layer to the map
		/// </summary>
		inline void PushLayer();
		
		void OnDraw(float deltaTime) override;

	public:
		/// <summary>
		/// Get the chunk index and index into the chunk based off of a mapspace index. (to do this for coords, call onece for x, again for y)
		/// </summary>
		/// <param name="index">the worlspace index</param>
		/// <returns>pair: (chunk index, index into chunk)</returns>
		static std::pair<int32_t, int32_t> ToChunkIndex(int32_t index);

		
	private:
		std::vector<TilesetRef> m_Tilesets;
		std::vector<TileLayer> m_Layers; //TileLayer is stack, not pointer, cause its only the size of an unordered_list. 
	};


}

