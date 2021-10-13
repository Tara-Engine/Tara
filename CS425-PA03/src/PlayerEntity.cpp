#include "PlayerEntity.h"
#include "Tara/Renderer/Renderer.h"
#include "Tara/Input/Input.h"
#include "Tara/Input/Mappings.h"

const static float MOVEMENT_DISTANCE = 16 * 4; //size of 1 tile;

PlayerEntity::PlayerEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name, Tara::SpriteRef sprite)
	: SpriteEntity(parent, owningLayer, transform, name, sprite), m_Target(transform.Position), m_Origin(transform.Position), m_Timer(0), m_Traveling(false)
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

	//apply movement directly to position
	Tara::Transform  t = GetWorldTransform();
	if (m_Traveling) {
		LOG_S(INFO) << "Traveling!";
		m_Timer += deltaTime;
		if (m_Timer >= m_MaxTime) {
			m_Traveling = false;
			m_Timer = 0;
			t.Position = m_Target;
			m_Origin = m_Target;
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
	if (m_Traveling) { return false; }

	auto key = e.getKey();
	Tara::Vector dir = { 0,0,0 };
	if (key == TARA_KEY_S || key == TARA_KEY_DOWN) {
		dir.y = -1;
	}
	else if (key == TARA_KEY_W || key == TARA_KEY_UP) {
		dir.y = 1;
	}
	else if (key == TARA_KEY_A || key == TARA_KEY_LEFT) {
		dir.x = -1;
	}
	else if (key == TARA_KEY_D || key == TARA_KEY_RIGHT) {
		dir.x = 1;
	}
	else {
		return false;
	}
	dir *= MOVEMENT_DISTANCE;
	const auto t = GetWorldTransform();
	m_Origin = t.Position;
	m_Target = t.Position + dir;
	m_Traveling = true;
	LOG_S(INFO) << "Key pressed event handled: " << (char)key;
	return true;
}

void PlayerEntity::SetTarget(Tara::Vector target, float travelTime)
{
	m_Origin = GetWorldTransform().Position;
	m_Target = target;
	m_MaxTime = travelTime;
	m_Traveling = true;
}
