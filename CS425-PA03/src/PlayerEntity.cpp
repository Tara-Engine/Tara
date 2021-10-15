#include "PlayerEntity.h"

//#include "Tara/Renderer/Renderer.h"
//#include "Tara/Input/Input.h"
//#include "Tara/Input/Mappings.h"

#include "RoomManager.h"

const static float MOVEMENT_DISTANCE = 16 * 4; //size of 1 tile;

PlayerEntity::PlayerEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name, Tara::SpriteRef sprite)
	: SpriteEntity(parent, owningLayer, transform, name, sprite), 
	m_Target(transform.Position), m_Origin(transform.Position), m_Timer(0), 
	m_Traveling(false), m_Direction(Direction::DOWN)
{}


std::shared_ptr<PlayerEntity> PlayerEntity::Create(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name, Tara::SpriteRef sprite)
{
	std::shared_ptr<PlayerEntity> newEntity = std::make_shared<PlayerEntity>(parent, owningLayer, transform, name, sprite);
	//must be done outside of constructor because 
	//you have to have it fully constructed before getting a shared ptr
	Entity::Register(newEntity);
	newEntity->ListenForEvents(true); //listen for window events
	return newEntity;
}

void PlayerEntity::OnUpdate(float deltaTime)
{
	Tara::SpriteEntity::OnUpdate(deltaTime); //call the super OnUppdate function (for animation reasons)
	const static std::string qualifiers[] = { "up", "down", "left", "right" };

	//apply movement directly to position
	Tara::Transform  t = GetWorldTransform();
	if (m_Traveling) {
		m_Timer += deltaTime;
		if (m_Timer >= m_MaxTime) {
			m_Traveling = false;
			m_Timer = 0;
			t.Position = m_Target;
			m_Origin = m_Target;
			PlayAnimation(std::string("idle_") + qualifiers[(uint8_t)m_Direction]);

			//get room and check for death
			auto pos = glm::vec2(GetWorldPosition());
			auto roomPos = RoomManager::WorldCoordToRoomCoord(pos);
			auto centered = RoomManager::IsCentered(pos);
			auto room = RoomManager::Get()->GetRoom(roomPos.x, roomPos.y);
			if (room && centered.first && centered.second) {
				if (room->GetPerm() == 4 && !Tara::PowOfTwo(room->GetDoorState())) {
					LOG_S(INFO) << "Player has Died!";
					SetAlive(false);
				}
			}
		}
		else {
			t.Position = Tara::CubicInterp<Tara::Vector>(m_Origin, m_Target, m_Timer / m_MaxTime);
		}
		
	}
	SetWorldTransform(t);
}

void PlayerEntity::OnEvent(Tara::Event& e)
{
	//LOG_S(INFO) << "Event received: " << e;
	Tara::EventFilter filter(e);
	filter.Call<Tara::KeyPressEvent>(TARA_BIND_FN(PlayerEntity::OnKeyPressEvent));
}

bool PlayerEntity::OnKeyPressEvent(Tara::KeyPressEvent& e)
{
	if (m_Traveling || !m_Alive) { return false; }
	auto pos = glm::vec2(GetWorldPosition());
	auto key = e.getKey();
	Tara::Vector dir = { 0,0,0 };
	auto roomPos = RoomManager::WorldCoordToRoomCoord(pos);
	auto centered = RoomManager::IsCentered(pos);
	auto room = RoomManager::Get()->GetRoom(roomPos.x, roomPos.y);
	
	if (key == TARA_KEY_E) {
		LOG_S(INFO) << "CURRENT ROOM: {" << roomPos.x << "," << roomPos.y << "}";
	}
	
	if (key == TARA_KEY_S || key == TARA_KEY_DOWN) {
		if (centered.first && (!room || !centered.second|| room->GetDoorState() & DOORSTATE_DOWN)) {
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
	dir *= MOVEMENT_DISTANCE * 4.5;
	const auto t = GetWorldTransform();
	SetTarget(t.Position + dir, 0.25 * 4.5);
	//LOG_S(INFO) << "Key pressed event handled: " << (char)key;
	return true;
}

void PlayerEntity::SetTarget(Tara::Vector target, float travelTime)
{
	//LOG_S(INFO) << "Traveling! {" << target.x << "," << target.y << "}";
	m_Origin = GetWorldTransform().Position;
	m_Target = target;
	m_MaxTime = travelTime;
	m_Traveling = true;

	//get direction
	auto delta = m_Target - m_Origin;
	if (abs(delta.x) > abs(delta.y)) {
		if (delta.x > 0) {
			m_Direction = Direction::RIGHT;
		}
		else {
			m_Direction = Direction::LEFT;
		}
	}
	else {
		if (delta.y > 0) {
			m_Direction = Direction::UP;
		}
		else {
			m_Direction = Direction::DOWN;
		}
	}

	//update animation
	const static std::string qualifiers[] = {"up", "down", "left", "right"};
	PlayAnimation(std::string("walk_") + qualifiers[(uint8_t)m_Direction]);
}
