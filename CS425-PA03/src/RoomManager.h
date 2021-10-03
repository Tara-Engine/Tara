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
	RoomEntityRef GetRoomAt(int32_t x, int32_t y);
	
	void Generate(); 

private:
	//Some datastructure to hold the rooms

};