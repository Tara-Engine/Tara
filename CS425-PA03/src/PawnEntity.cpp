#include "PawnEntity.h"

//#include "Tara/Renderer/Renderer.h"
//#include "Tara/Input/Input.h"
//#include "Tara/Input/Mappings.h"

#include "RoomManager.h"

//world regen func
void Regenerate(Tara::LayerNoRef layer);

const static float MOVEMENT_DISTANCE = 16 * 4; //size of 1 tile;

PawnEntity::PawnEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name, Tara::SpriteRef sprite)
	: SpriteEntity(parent, owningLayer, transform, name, sprite), 
	m_Target(transform.Position), m_Origin(transform.Position), m_Spawn(transform.Position),
	m_Timer(0), m_Traveling(false), m_Direction(Direction::DOWN)
{}


std::shared_ptr<PawnEntity> PawnEntity::Create(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name, Tara::SpriteRef sprite)
{
	std::shared_ptr<PawnEntity> newEntity = std::make_shared<PawnEntity>(parent, owningLayer, transform, name, sprite);
	//must be done outside of constructor because 
	//you have to have it fully constructed before getting a shared ptr
	Entity::Register(newEntity);
	newEntity->ListenForEvents(true); //listen for window events
	return newEntity;
}

void PawnEntity::OnUpdate(float deltaTime)
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
				if (room->GetPerm() == 4){ 
					if (Tara::PowOfTwo(room->GetDoorState())) {
						//regenerate!
						Tara::After([](Tara::LayerNoRef layer) {Regenerate(layer); }, 1.0, GetOwningLayer());
					}
					else{
						LOG_S(INFO) << "Player has Died!";
						SetAlive(false);
						Tara::After(std::bind(&PawnEntity::Respawn, this), 1.0f, this);
					}
				}
			}
		}
		else {
			t.Position = Tara::CubicInterp<Tara::Vector>(m_Origin, m_Target, m_Timer / m_MaxTime);
		}
		
	}
	SetWorldTransform(t);
}


void PawnEntity::SetTarget(Tara::Vector target, float travelTime)
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

void PawnEntity::Respawn()
{
	m_Origin = m_Spawn;
	m_Target = m_Spawn;
	SetWorldPosition(m_Spawn);
	SetAlive(true);
}
