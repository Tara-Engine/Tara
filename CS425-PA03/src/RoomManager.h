#pragma once
#include <Tara.h>
#include <RoomEntity.h>
// ^ We get the door macros from here
//the reason we don't have them in this file is because they are needed in both files.



//no namespace, not part of the tara lib

class RoomManager {

private:
	RoomManager(); 

public:

	//these are deleted to prevent copying the singleton.
	RoomManager(RoomManager const&) = delete;
	void operator=(RoomManager const&) = delete;

	/// <summary>
	/// Initialize the manager
	/// </summary>
	/// <param name="defaultParent">the default parent for rooms</param>
	/// <param name="defaultLayer">the default layer for rooms</param>
	void Init(Tara::EntityNoRef defaultParent, Tara::LayerNoRef defaultLayer);

	/// <summary>
	/// Singleton Getter
	/// </summary>
	/// <returns>the singleton instance</returns>
	static RoomManager* Get();

	/// <summary>
	/// Internal helper function to convert the door states bitflag and perm number to a string for file loading and
	/// asset manager querying
	/// </summary>
	/// <param name="doorStates"></param>
	/// <param name="perm"></param>
	/// <returns></returns>
	static std::string StatesAndPermToString(uint32_t doorStates, uint32_t perm);

	/// <summary>
	/// Invert a door state, so it can work for adjacent room matching. Should only be performed on one state at a time (ie, no or'ing together states)
	/// </summary>
	/// <param name="state">the state</param>
	/// <returns>the inverted state</returns>
	static uint32_t InvertDoorState(uint32_t state);

	/// <summary>
	/// Get the appropreate texture from  the doorstate and permutation
	/// </summary>
	/// <param name="doorState"></param>
	/// <param name="perm"></param>
	/// <returns></returns>
	static Tara::SpriteRef RoomTextureFromDoorsAndPerm(uint32_t doorState, uint32_t perm);

	/// <summary>
	/// Turn world coordinates into room coordinates
	/// </summary>
	/// <param name="worldCoord"></param>
	/// <returns></returns>
	static glm::ivec2 WorldCoordToRoomCoord(glm::vec2 worldCoord);

	/// <summary>
	/// turn room coordinates into world coordinates
	/// The coordinates are centered. on the room, for sprites
	/// </summary>
	/// <param name="roomCoord"></param>
	/// <returns></returns>
	static glm::vec2 RoomCoordToWorldCoord(glm::ivec2 roomCoord);

	/// <summary>
	/// check if a world position is centered
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	static std::pair<bool, bool> IsCentered(glm::vec2 pos);

	

	/// <summary>
	/// Load the textures for all room permutations;
	/// </summary>
	void LoadRoomTextures();

	/// <summary>
	/// Add a room to the map
	/// </summary>
	/// <param name="x">the x location in room graph</param>
	/// <param name="y">the y location in room graph</param>
	/// <param name="doors">the doors bitflag</param>
	/// <param name="perm">permutation index (0 for random)</param>
	/// <returns>true if the room was added, false if it could not be</returns>
	bool AddRoom(int32_t x, int32_t y, uint32_t doors, uint32_t perm);


	/// <summary>
	/// Get the room at a specific coordiante
	/// </summary>
	/// <param name="x">the x coordinate</param>
	/// <param name="y">the y coordinate</param>
	/// <returns>the room. May be nullptr</returns>
	RoomEntityRef GetRoom(int32_t x, int32_t y);
	

	std::list<int32_t> Generate(uint32_t seed, int32_t width, int32_t height, int32_t steps); 

	/// <summary>
	/// clear all rooms
	/// </summary>
	void ClearAll();

private:
	
	//callable struct for hashing pairs
	struct pair_hash{
		template <class T1, class T2>
		std::size_t operator() (const std::pair<T1, T2>& pair) const {
			return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
		}
	};

	std::unordered_map<std::pair<uint32_t,uint32_t>, RoomEntityRef, pair_hash> m_Rooms;

	Tara::EntityNoRef m_DefaultParent;
	Tara::LayerNoRef m_DefaultLayer;


};

