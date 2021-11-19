#include "tarapch.h"
#include "TilemapEntity.h"
#include "Tara/Renderer/Renderer.h"
#include "nlohmann/json.hpp"
#include "Tara/Core/Script.h"
#include <fstream>

namespace Tara {
	Tara::TileChunk::TileChunk()
		: Tiles(nullptr)
	{
		Tiles = new uint32_t[(uint64_t)WIDTH * WIDTH];
		memset(Tiles, uint32_t(0), ((uint64_t)WIDTH * WIDTH * 4));
	}

	TileChunk::TileChunk(TileChunk&& old) noexcept
	{
		Tiles = old.Tiles;
		old.Tiles = nullptr;
	}

	TileChunk::~TileChunk()
	{
		if (Tiles) {
			delete[] Tiles;
			//Tiles = nullptr;
		}
	}

	uint32_t TileChunk::GetTile(int32_t x, int32_t y)
	{
		//making the assumption Tiles is still valid
		if (x < WIDTH && x >= 0 && y < WIDTH && y >= 0) {
			return Tiles[x * WIDTH + y];
		}
		else {
			LOG_S(ERROR) << "TileChunk tried to access tile outside its range!";
			return 0;
		}
	}

	void TileChunk::SetTile(int32_t x, int32_t y, uint32_t tileID)
	{
		//making the assumption Tiles is still valid
		if (x < WIDTH && x >= 0 && y < WIDTH && y >= 0) {
			Tiles[x * WIDTH + y] = tileID;
		}
		else {
			LOG_S(ERROR) << "TileChunk tried to access tile outside its range!";
		}
	}


	TileLayer::~TileLayer()
	{
		for (auto& kv : m_Chunks) {
			if (kv.second) {
				delete kv.second;
				kv.second = nullptr;
			}
		}
	}

	uint32_t TileLayer::GetTile(int32_t x, int32_t y)
	{
		auto idxX = TilemapEntity::ToChunkIndex(x);
		auto idxY = TilemapEntity::ToChunkIndex(y);
		glm::ivec2 chunkIndex{ idxX.first, idxY.first };
		auto iter = m_Chunks.find(chunkIndex);
		if (iter != m_Chunks.end()) {
			//we have a valid chunk
			//return the tileID from it.
			return iter->second->GetTile(idxX.second, idxY.second);
		}
		else {
			//not a loaded chunk, so its entirely empty.
			return 0; 
		}
	}

	void TileLayer::SetTile(int32_t x, int32_t y, uint32_t tileID)
	{
		auto idxX = TilemapEntity::ToChunkIndex(x);
		auto idxY = TilemapEntity::ToChunkIndex(y);
		glm::ivec2 chunkIndex{ idxX.first, idxY.first };
		auto& iter = m_Chunks.find(chunkIndex);
		if (iter != m_Chunks.end()) {
			//we have a valid chunk
			//return the tileID from it.
			iter->second->SetTile(idxX.second, idxY.second, tileID);
			//if this is an empty tile, check if all tiles are 0, if they are, then remove that chunk
			if (!tileID) {
				bool allEmpty = true;
				for (int i = 0; i < TileChunk::WIDTH * TileChunk::WIDTH; i++) {
					if (iter->second->Tiles[i]) {
						//a valid tile is found
						allEmpty = false;
						break;
					}
				}
				if (allEmpty) {
					TileChunk* chunk = iter->second;
					m_Chunks.erase(iter);
					delete chunk;
				}
			}
		}
		else {
			//not a loaded chunk, so its entirely empty.
			//make a new chunk there. Unless, of course, the tileID is 0
			if (tileID) {
				TileChunk* chunk = new TileChunk();
				chunk->SetTile(idxX.second, idxY.second, tileID);
				//make sure to Move the chunk into the index, so no copying of the 4kb blob of memory on the heap
				m_Chunks.insert_or_assign(chunkIndex, chunk);
			}
		}
	}


	TilemapEntity::TilemapEntity(EntityNoRef parent, LayerNoRef owningLayer, std::initializer_list<TilesetRef> tilesets, Transform transform, const std::string& name)
		: Entity(parent, owningLayer, transform, name),
		m_Bounds(0.0f,0.0f,0.0f,0.0f,0.0f,0.0f)
	{
		m_Tilesets = tilesets;
		PushLayer(); //create a default layer
	}

	TilemapEntity::~TilemapEntity()
	{
		//cleanup remaining cell metadata
		int cnt = 0;
		for (auto& kv : m_CellMetadata) {
			if (kv.second) {
				delete kv.second;
				kv.second = nullptr;
				cnt++;
			}
		}
		LOG_S(INFO) << "TilemapEntity destructor running. Metadata chunks cleaned: " << cnt;
	}

	uint32_t TilemapEntity::GetTile(int32_t x, int32_t y, int32_t layer)
	{
		if (layer >= 0 && layer < m_Layers.size()) {
			return (m_Layers[layer].GetTile(x, y) - 1); //if Tile is 0, then it becomes NO_TILE
		}
		else {
			return NO_TILE;
		}
	}

	void TilemapEntity::SwapTile(int32_t x, int32_t y, int32_t layer, uint32_t tileID)
	{
		if (layer >= 0 && layer < m_Layers.size()) {
			m_Layers[layer].SetTile(x, y, tileID + 1); //if tileID is NO_TILE, it becomes 0.
			if (tileID + 1) {
				//if we are setting a tile, make sure to update the bounds
				if (x < m_Bounds.x) {
					//Offset Width so it stays the same pos
					m_Bounds.Width += m_Bounds.x - x;
					//move bounds X
					m_Bounds.x = x;
				}
				if (x >= m_Bounds.x + m_Bounds.Width) {
					//increase Width to encompass the whole thing. Width will alwawys be one greater than last index in X
					m_Bounds.Width = (x - m_Bounds.x) + 1;
				}

				if (y < m_Bounds.y) {
					//offset Height so it stays the same pos
					m_Bounds.Height += m_Bounds.y - y;
					//move bounds Y
					m_Bounds.y = y;
				}
				if (y >= m_Bounds.y + m_Bounds.Height) {
					//increase Height to encompass the whole thing. Height will alwawys be one greater than last index in Y
					m_Bounds.Height = (y - m_Bounds.y) + 1;
				}
			}
		}
		else {
			LOG_S(ERROR) << "Attempted to set a tile in a nonexistant tilemap layer. tilemap layers must be explicitly created!";
		}
	}

	inline void TilemapEntity::PushLayer()
	{
		m_Layers.push_back(std::move(TileLayer{}));
	}

	void TilemapEntity::FillFromJson(const std::string& path)
	{
		std::ifstream file(path);
		nlohmann::json json;
		file >> json;
		nlohmann::json jLayers = json["layers"];

		bool isInfFile = json["infinite"].get<bool>();

		m_Layers.clear(); //clear current data
		for (auto& jLayer : jLayers) {
			//make a new layuer
			PushLayer();
			
			if (isInfFile) {

				for (auto& jChunk : jLayer["chunks"]) {
					//read the data out of json
					std::vector<uint32_t> data = jChunk["data"].get<std::vector<uint32_t>>();
					int32_t width = jChunk["width"].get<int32_t>();
					//height data is irrelevent
					//int32_t height = jLayer["height"].get<int32_t>();
					int32_t originX = jChunk["x"].get<int32_t>();
					int32_t originY = jChunk["y"].get<int32_t>();

					//put the data into the layer. Its aready numerically formatted.
					TileLayer& tLayer = m_Layers[m_Layers.size() - 1];
					for (int i = 0; i < data.size(); i++) {
						int32_t x = i % width + originX;
						int32_t y = -(i / width + originY);
						tLayer.SetTile(x, y, data[i]);
					}
				}
			}
			else {
				//read the data out of json
				std::vector<uint32_t> data = jLayer["data"].get<std::vector<uint32_t>>();
				int32_t width = jLayer["width"].get<int32_t>();
				//height data is irrelevent
				int32_t height = jLayer["height"].get<int32_t>();
				int32_t originX = jLayer["x"].get<int32_t>();
				int32_t originY = jLayer["y"].get<int32_t>();

				//put the data into the layer. Its aready numerically formatted.
				TileLayer& tLayer = m_Layers[m_Layers.size() - 1];
				for (int i = 0; i < data.size(); i++) {
					int32_t x = i % width + originX;
					int32_t y = (height - 1) - (i / width + originY);
					tLayer.SetTile(x, y, data[i]);
				}
			}
		}

	}

	void TilemapEntity::GiveCellMetadata(glm::ivec3 pos, void* metaData)
	{
		auto& iter = m_CellMetadata.find(pos);
		if (iter != m_CellMetadata.end()) {
			//there is already metadata there. We own it
			if (iter->second) {
				delete (iter->second);
				iter->second = nullptr;
			}
		}
		//insert new metadata
		m_CellMetadata.insert_or_assign(pos, metaData);
	}

	void* TilemapEntity::GetCellMetadata(glm::ivec3 pos)
	{
		auto& iter = m_CellMetadata.find(pos);
		if (iter != m_CellMetadata.end()) {
			//there is metadata there. return it without destrying
			return iter->second;
		}
		else {
			//we have nothing, return nullptr
			return nullptr;
		}
	}

	void* TilemapEntity::TakeCellMetadata(glm::ivec3 pos)
	{
		auto& iter = m_CellMetadata.find(pos);
		if (iter != m_CellMetadata.end()) {
			//there is metadata there. return it without destrying
			void* data = iter->second;
			m_CellMetadata.erase(pos); //remove it from metadata
			return data;
		}
		else {
			//we have nothing, return nullptr
			return nullptr;
		}
	}

	void* TilemapEntity::GetTileMetadata(int32_t x, int32_t y, int32_t layer)
	{
		uint32_t tile = GetTile(x, y, layer)+1;
		if (tile) {
			tile -= 1;
			for (auto& tileset : m_Tilesets) {
				//if this tile is in the set
				if (tile < tileset->GetTileCount()) {
					//return the metadata
					return tileset->GetTileMetadata(tile);
				}
				else {
					//move the tile index down, and check next tileset
					tile -= tileset->GetTileCount();
				}
			}
		}
		return nullptr;
	}

	void TilemapEntity::WipeCellMetadata(glm::ivec3 pos)
	{
		auto& iter = m_CellMetadata.find(pos);
		if (iter != m_CellMetadata.end()) {
			//there is metadata there. destroy it!
			if (iter->second) {
				delete iter->second;
				iter->second = nullptr;
			}
			m_CellMetadata.erase(pos); //remove slot from metadata
		}
	}




	void TilemapEntity::OnDraw(float deltaTime)
	{
		//get the world transform Once.
		Transform world = GetWorldTransform();
		//for every layer
		for (auto& layer : m_Layers) {
			//for every chunk in layer
			for (auto& kv : layer.m_Chunks) {
				auto& offset = kv.first;
				auto& chunk = kv.second;
				//for every tile in chunk
				for (int i = 0; i < (TileChunk::WIDTH * TileChunk::WIDTH); i++) {
					auto tile = chunk->Tiles[i];
					if (tile) { //ignore empty tiles
						//adjust for 0 being empty tile
						tile -= 1;

						//positional aquiring
						//transforms added so that the offset is relative to the rotation and scale of the parent transform.
						Transform t = world + TRANSFORM_2D(
							(float)(i / TileChunk::WIDTH + offset.x * TileChunk::WIDTH),
							(float)(i % TileChunk::WIDTH + offset.y * TileChunk::WIDTH),
							0,1,1
						); 

						//find the correct tileset and adjust for tileset offsets
						for (auto& tileset : m_Tilesets) {
							//if this tile is in the set
							if (tile < tileset->GetTileCount()) {
								//get the uvs and draw
								auto uvs = tileset->GetTileUVs(tile);
								//since Renderer::Quad is batched, this is actually somewhat efficent. Particularly since its mostly going to be just pushing to a vector.
								Renderer::Quad(t, { 1,1,1,1 }, tileset->GetTexture(), uvs.first, uvs.second);
								break; //we have drawn this tile, exit the "find a tileset" loop
							}
							else {
								//move the tile index down, and check next tileset
								tile -= tileset->GetTileCount();
							}
						}
					}
				}
			}
		}
	}


	bool TilemapEntity::ConfirmOverlap(EntityRef other)
	{
		BoundingBox b = other->GetSpecificBoundingBox();
		//transform corners of bounding box into tilepsace coords.
		b = b / GetWorldTransform();
		int32_t x1 = (int32_t)floorf(b.x) + 1;
		int32_t x2 = x1 + (int32_t)ceilf(b.Width);
		int32_t y1 = (int32_t)floorf(b.y) + 1;
		int32_t y2 = y1 + (int32_t)ceilf(b.Height);
		//check each tile, if a single non-empty one is found, return true
		for (auto& layer : m_Layers) {
			if (layer.m_Colliding){
				for (int32_t x = x1; x <= x2; x++) {
					for (int32_t y = y1; y <= y2; y++) {
						if (layer.GetTile(x, y)) {
							return true;
						}
					}
				}
			}
		}
		//they were all empty
		return false;
	}

	

	std::pair<int32_t, int32_t> TilemapEntity::ToChunkIndex(int32_t index)
	{
		int32_t chunk = index / TileChunk::WIDTH; 
		int32_t cindex = index % TileChunk::WIDTH; 
		if (index < 0) {
			chunk -= 1;
			cindex += TileChunk::WIDTH;
		}
		return std::make_pair(chunk, cindex);
	}

	uint32_t TilemapEntity::__SCRIPT__GetTile(sol::object a, sol::object b, sol::object c)
	{
		if (a.valid()) {
			if (b.valid() && c.valid()) {
				if (
					a.get_type() == sol::type::number && 
					b.get_type() == sol::type::number && 
					c.get_type() == sol::type::number
				) {
					int32_t x = a.as<int32_t>();
					int32_t y = b.as<int32_t>();
					int32_t layer = c.as<int32_t>();
					return GetTile(x, y, layer);
				}//else error below
			}
			else if (!b.valid() && !c.valid()) {
				auto tbl = a.as<sol::table>();
				if (tbl.valid()) {
					Vector v{ tbl };
					return GetTile(v);
				}//else error below
			}//else error below
		}//else error below
		LOG_S(ERROR) << "Lua:: Tilemap::GetTile must take at one paramater (vector) or three paramaters (numbers)";
		return NO_TILE;
	}

	void TilemapEntity::__SCRIPT__SetTile(sol::object a, sol::object b, sol::object c, sol::object d)
	{
		if (a.valid() && b.valid()) {
			if (c.valid() && d.valid()) {
				if (
					a.get_type() == sol::type::number &&
					b.get_type() == sol::type::number &&
					c.get_type() == sol::type::number &&
					d.get_type() == sol::type::number
				) {
					int32_t x = a.as<int32_t>();
					int32_t y = b.as<int32_t>();
					int32_t layer = c.as<int32_t>();
					uint32_t tile = d.as<uint32_t>();
					SetTile(x, y, layer, tile);
					return;
				}//else error below
			}
			else if (!c.valid() && !d.valid()) {
				auto tbl = a.as<sol::table>();
				if (tbl.valid() && b.get_type() == sol::type::number) {
					Vector v{ tbl };
					uint32_t tile = b.as<uint32_t>();
					SetTile(v, tile);
					return;
				}//else error below

			}//else error below
		}
		LOG_S(ERROR) << "Lua:: Tilemap::SetTile must take two paramaters (Vector and number) or four paramaters (four numbers)";
		return;
	}

	void TilemapEntity::__SCRIPT__SwapTile(sol::object a, sol::object b, sol::object c, sol::object d)
	{
		if (a.valid() && b.valid()) {
			if (c.valid() && d.valid()) {
				if (
					a.get_type() == sol::type::number &&
					b.get_type() == sol::type::number &&
					c.get_type() == sol::type::number &&
					d.get_type() == sol::type::number
					) {
					int32_t x = a.as<int32_t>();
					int32_t y = b.as<int32_t>();
					int32_t layer = c.as<int32_t>();
					uint32_t tile = d.as<uint32_t>();
					SwapTile(x, y, layer, tile);
					return;
				}//else error below
			}
			else if (!c.valid() && !d.valid()) {
				auto tbl = a.as<sol::table>();
				if (tbl.valid() && b.get_type() == sol::type::number) {
					Vector v{ tbl };
					uint32_t tile = b.as<uint32_t>();
					SwapTile(v, tile);
					return;
				}//else error below

			}//else error below

		}
		LOG_S(ERROR) << "Lua:: Tilemap::SwapTile must take two paramaters (Vector and number) or four paramaters (four numbers)";
		return;
	}

	void TilemapEntity::RegisterLuaType(sol::state& lua)
	{
		sol::usertype<TilemapEntity> type = lua.new_usertype<TilemapEntity>("TilemapEntity", sol::base_classes, sol::bases<Entity>());
		CONNECT_METHOD(TilemapEntity, GetLayerCount);
		CONNECT_METHOD(TilemapEntity, PushLayer);
		CONNECT_METHOD(TilemapEntity, FillFromJson);
		CONNECT_METHOD_OVERRIDE(TilemapEntity, GetTile);
		CONNECT_METHOD_OVERRIDE(TilemapEntity, SetTile);
		CONNECT_METHOD_OVERRIDE(TilemapEntity, SwapTile);
		/*
		GetTile
		SetTile
		SwapTile
		*/
	}

}
