#pragma once
#include <Tara.h>

//Some macros to help when creating a room entity
//can be or'ed together
#define DOORSTATE_UP 0x01
#define DOORSTATE_DOWN 0x02
#define DOORSTATE_LEFT 0x04
#define DOORSTATE_RIGHT 0x08
#define DOORSTATE_ALL 0x0f

//number of permutations for each room
#define MAX_ROOM_PERMS 4

//No namespcace, as this is part of the game, not the engine


class RoomEntity : public Tara::SpriteEntity {
public:
	using RoomEntityRef = std::shared_ptr<RoomEntity>;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="parent">The parent</param>
	/// <param name="owningLayer">The owning layer</param>
	/// <param name="doorState">bitfield of the door state</param>
	/// <param name="perm">permutation number (1-4)</param>
	/// <param name="transform">the local transform (optional)</param>
	/// <param name="name">the name (optional)</param>
	/// <param name="sprite">the sprite (optional)</param>
	RoomEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, uint32_t doorState, uint32_t perm, Tara::Transform transform = TRANSFORM_DEFAULT, std::string name = "RoomEntity", Tara::SpriteRef sprite = nullptr);
	
	virtual ~RoomEntity() {}	

	/// <summary>
	/// Get the door state
	/// </summary>
	/// <returns></returns>
	inline uint32_t GetDoorState() const { return m_DoorState; }
	
	/// <summary>
	/// Match a given state to the door state. Does not invert the other state
	/// </summary>
	/// <param name="otherState">the state to match</param>
	/// <returns>true if they match for at least one door, false otherwise</returns>
	inline bool MatchDoorState(uint32_t otherState) const { return m_DoorState & otherState; }

	/// <summary>
	/// Hard match a given state to the door state. Does not invert the other state
	/// </summary>
	/// <param name="otherState">the state to match</param>
	/// <returns>true if the states match for all doors, false otherwise</returns>
	inline bool HardMatchDoorState(uint32_t otherState) const { return m_DoorState == otherState; }

	/// <summary>
	/// Set the door state. Does not change texture.
	/// </summary>
	/// <param name="state">the new state</param>
	inline void SetDoorState(uint32_t state) { m_DoorState = state; }

	/// <summary>
	/// Get the room permutation
	/// </summary>
	/// <returns>the permutation</returns>
	inline uint32_t GetPerm() const { return m_Perm; }

	/// <summary>
	/// Set the room permutation. Does not change texture
	/// </summary>
	/// <param name="perm">the new permutation</param>
	inline void SetPerm(uint32_t perm) { m_Perm = perm; }
	
public:
	//lua stuff
	static void RegisterLuaType(sol::state& lua);


private:
	uint32_t m_DoorState;
	uint32_t m_Perm;
};

using RoomEntityRef = std::shared_ptr<RoomEntity>;