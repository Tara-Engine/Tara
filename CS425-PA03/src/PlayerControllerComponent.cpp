#include "PlayerControllerComponent.h"
#include "PawnEntity.h"
#include "RoomManager.h"

//world regen func
void Regenerate(Tara::LayerNoRef layer);

const static float MOVEMENT_DISTANCE = 16 * 4; //size of 1 tile;

PlayerControllerComponent::PlayerControllerComponent(Tara::EntityNoRef parent, const std::string& name)
	: Tara::Component(parent, name)
{}

PlayerControllerComponentRef PlayerControllerComponent::Create(Tara::EntityNoRef parent, const std::string & name)
{
	PlayerControllerComponentRef component = std::make_shared<PlayerControllerComponent>(parent, name);
	Register(component);
	return component;
}

void PlayerControllerComponent::OnEvent(Tara::Event& e)
{
	Tara::EventFilter filter(e);
	filter.Call<Tara::KeyPressEvent>(TARA_BIND_FN(PlayerControllerComponent::OnKeyPressEvent));

}

void PlayerControllerComponent::OnUpdate(float deltaTime)
{
	if (!m_SendingToNextLevel){
		auto parent = std::dynamic_pointer_cast<PawnEntity>(GetParent().lock()); //OK becase will be cleaned up and end of func call
		if (parent && !parent->GetTraveling()) {
			auto pos = glm::vec2(parent->GetWorldPosition());
			auto roomPos = RoomManager::WorldCoordToRoomCoord(pos);
			auto centered = RoomManager::IsCentered(pos);
			auto room = RoomManager::Get()->GetRoom(roomPos.x, roomPos.y);
			if (room && centered.first && centered.second && room->GetPerm() == 4 && Tara::PowOfTwo(room->GetDoorState())) {
				//regenerate!
				m_SendingToNextLevel = true;
				Tara::After([](Tara::LayerNoRef layer) {Regenerate(layer); }, 1.0, parent->GetOwningLayer());
			}
		}
	}
}

bool PlayerControllerComponent::OnKeyPressEvent(Tara::KeyPressEvent& e)
{
	auto parent = std::dynamic_pointer_cast<PawnEntity>(GetParent().lock()); //OK becase will be cleaned up and end of func call
	if (!parent) { return false; }

	if (parent->GetTraveling() || !parent->GetAlive()) { return false; }
	auto pos = glm::vec2(parent->GetWorldPosition());
	auto key = e.getKey();
	Tara::Vector dir = { 0,0,0 };
	auto roomPos = RoomManager::WorldCoordToRoomCoord(pos);
	auto centered = RoomManager::IsCentered(pos);
	auto room = RoomManager::Get()->GetRoom(roomPos.x, roomPos.y);

	if (key == TARA_KEY_E) {
		LOG_S(INFO) << "CURRENT ROOM: {" << roomPos.x << "," << roomPos.y << "}";
	}

	if (key == TARA_KEY_S || key == TARA_KEY_DOWN) {
		if (centered.first && (!room || !centered.second || room->GetDoorState() & DOORSTATE_DOWN)) {
			dir.y = -1;
		}
	}
	else if (key == TARA_KEY_W || key == TARA_KEY_UP) {
		if (centered.first && (!room || !centered.second || room->GetDoorState() & DOORSTATE_UP)) {
			dir.y = 1;
		}
	}
	else if (key == TARA_KEY_A || key == TARA_KEY_LEFT) {
		if (centered.second && (!room || !centered.first || room->GetDoorState() & DOORSTATE_LEFT)) {
			dir.x = -1;
		}
	}
	else if (key == TARA_KEY_D || key == TARA_KEY_RIGHT) {
		if (centered.second && (!room || !centered.first || room->GetDoorState() & DOORSTATE_RIGHT)) {
			dir.x = 1;
		}
	}
	else {
		return false;
	}
	if (dir.x == 0 && dir.y == 0) {
		//skip movement
		return true;
	}
	dir *= MOVEMENT_DISTANCE * 4.5; //move 4.5 tiles
	parent->SetTarget(parent->GetWorldPosition() + dir, 0.25 * 4.5);
	//LOG_S(INFO) << "Key pressed event handled: " << (char)key;
	return true;
}

