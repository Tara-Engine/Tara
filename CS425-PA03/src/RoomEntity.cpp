#include "RoomEntity.h"
#include "RoomManager.h"

RoomEntity::RoomEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, uint32_t doorState, uint32_t perm, Tara::Transform transform, std::string name, Tara::SpriteRef sprite)
	: Tara::SpriteEntity(parent, owningLayer, transform, name, sprite), m_DoorState(doorState), m_Perm(perm)
{
}

void RoomEntity::RegisterLuaType(sol::state& lua)
{
	sol::usertype<RoomEntity> type = lua.new_usertype<RoomEntity>("RoomEntity", sol::base_classes, sol::bases<Tara::Entity, Tara::SpriteEntity>());
	CONNECT_METHOD(RoomEntity, GetDoorState);
	CONNECT_METHOD(RoomEntity, MatchDoorState);
	CONNECT_METHOD(RoomEntity, HardMatchDoorState);
	CONNECT_METHOD(RoomEntity, SetDoorState);
	CONNECT_METHOD(RoomEntity, GetPerm);
	CONNECT_METHOD(RoomEntity, SetPerm);
}





