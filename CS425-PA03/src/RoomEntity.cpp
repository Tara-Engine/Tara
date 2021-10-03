#include "RoomEntity.h"

RoomEntity::RoomEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name, Tara::Texture2DRef texture, uint32_t doorState, uint32_t perm)
	: Tara::SpriteEntity(parent, owningLayer, transform, name, texture), m_DoorState(doorState), m_Perm(perm)
{
}

RoomEntityRef RoomEntity::Create(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, uint32_t doorState, uint32_t perm, Tara::Transform transform, std::string name)
{
	RoomEntityRef newEntity = std::make_shared<RoomEntity>(parent, owningLayer, transform, name, RoomTextureFromDoorsAndPerm(doorState, perm), doorState, perm);
	//must be done outside of constructor because 
	//you have to have it fully constructed before getting a shared ptr
	Entity::Register(newEntity);
	return newEntity;
}


/*************************************************
*				TODO SECTION					 *
**************************************************/


Tara::Texture2DRef RoomEntity::RoomTextureFromDoorsAndPerm(uint32_t doorState, uint32_t perm)
{
	//TODO: Implement this
	return nullptr;
}
