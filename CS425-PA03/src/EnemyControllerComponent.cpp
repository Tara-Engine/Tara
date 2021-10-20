#include "EnemyControllerComponent.h"
#include "PawnEntity.h"
#include "RoomManager.h"

const static float MOVEMENT_DISTANCE = 16 * 4; //size of 1 tile

EnemyControllerComponent::EnemyControllerComponent(Tara::EntityNoRef parent, const std::string& name)
	: Tara::Component(parent, name)
{}

EnemyControllerComponentRef EnemyControllerComponent::Create(Tara::EntityNoRef parent, const std::string & name)
{
	EnemyControllerComponentRef component = std::make_shared<EnemyControllerComponent>(parent, name);
	Register(component);
	return component; 
}


void EnemyControllerComponent::OnUpdate(float deltaTime)
{
	auto parent = std::dynamic_pointer_cast<PawnEntity>(GetParent().lock()); //OK becase will be cleaned up and end of func call
	if (parent && !parent->GetTraveling()) {
		//choose a new destination
		auto roomCords = RoomManager::WorldCoordToRoomCoord(glm::vec2(parent->GetWorldPosition()));
		auto room = RoomManager::Get()->GetRoom(roomCords.x, roomCords.y);
		auto doorState = room->GetDoorState();
		uint32_t dirBit = 0;
		while (!(dirBit & doorState)) {
			dirBit = 1 << (rand() % 4); //should pick a random bit
		}
		Tara::Vector dir = { 0,0,0 };
		switch (dirBit) {
		case DOORSTATE_UP: {
			dir.y = 1;
			break;
		}
		case DOORSTATE_DOWN: {
			dir.y = -1;
			break;
		}
		case DOORSTATE_LEFT: {
			dir.x = -1;
			break;
		}
		case DOORSTATE_RIGHT: {
			dir.x = 1;
			break;
		}
		}
		if (dir.x != 0 || dir.y != 0) {
			dir *= MOVEMENT_DISTANCE * 9; //move 9 tiles, room center to room center
			parent->SetTarget(parent->GetWorldPosition() + dir, 0.25 * 9);
		}
	}
}

