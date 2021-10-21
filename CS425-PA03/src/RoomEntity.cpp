#include "RoomEntity.h"
#include "RoomManager.h"

RoomEntity::RoomEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, uint32_t doorState, uint32_t perm, Tara::Transform transform, std::string name, Tara::SpriteRef sprite)
	: Tara::SpriteEntity(parent, owningLayer, transform, name, sprite), m_DoorState(doorState), m_Perm(perm)
{
}





