#include "RoomEntity.h"
#include "RoomManager.h"

RoomEntity::RoomEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name, Tara::SpriteRef sprite, uint32_t doorState, uint32_t perm)
	: Tara::SpriteEntity(parent, owningLayer, transform, name, sprite), m_DoorState(doorState), m_Perm(perm)
{
}

RoomEntityRef RoomEntity::Create(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, uint32_t doorState, uint32_t perm, Tara::Transform transform, std::string name)
{
	if (perm == 0) {
		perm = rand() % MAX_ROOM_PERMS + 1;
	}
	RoomEntityRef newEntity = std::make_shared<RoomEntity>(parent, owningLayer, transform, name, RoomManager::RoomTextureFromDoorsAndPerm(doorState, perm), doorState, perm);
	//must be done outside of constructor because 
	//you have to have it fully constructed before getting a shared ptr
	Entity::Register(newEntity);
	return newEntity;
}


/*************************************************
*				TODO SECTION					 *
**************************************************/


