#include "PawnEntity.h"

//#include "Tara/Renderer/Renderer.h"
//#include "Tara/Input/Input.h"
//#include "Tara/Input/Mappings.h"




const static float MOVEMENT_DISTANCE = 16 * 4; //size of 1 tile;

PawnEntity::PawnEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name, Tara::SpriteRef sprite)
	: SpriteEntity(parent, owningLayer, transform, name, sprite), 
	m_Target(transform.Position), m_Origin(transform.Position), m_Spawn(transform.Position),
	m_Timer(0), m_Traveling(false), m_Direction(Direction::DOWN)
{}

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

void PawnEntity::Kill()
{
	LOG_S(INFO) << "Pawn \"" << GetName() << "\" has Died!";
	SetAlive(false);
	if (!GetImmortal()) { //only respawn if not immortal
		Tara::After(std::bind(&PawnEntity::Respawn, this), 1.0f, this);
	}
}

void PawnEntity::__SCRIPT__SetTarget(sol::table vector, float time)
{
	if (vector.valid()) {
		Tara::Vector v;
		v.x = vector.get_or("x", 0.0f);
		v.y = vector.get_or("y", 0.0f);
		v.z = vector.get_or("z", 0.0f);
		SetTarget(v, time);
	}
	else {
		LOG_S(ERROR) << "Lua: PawnEntity::SetTarget needs a vector as first paramater";
	}
}
//UP, DOWN, LEFT, RIGHT
void PawnEntity::__SCRIPT__SetDirection(std::string dir)
{
	Direction d = Direction::UP;
	if (dir == "UP") {
		d = Direction::UP;
	}
	else if (dir == "DOWN") {
		d = Direction::DOWN;
	}
	else if (dir == "LEFT") {
		d = Direction::LEFT;
	}
	else if (dir == "RIGHT") {
		d = Direction::RIGHT;
	}
	SetDirection(d);
}

std::string PawnEntity::__SCRIPT__GetDirection() const
{
	switch (GetDirection()) {
	case Direction::UP:		return "UP";
	case Direction::DOWN:	return "DOWN";
	case Direction::LEFT:	return "LEFT";
	case Direction::RIGHT:	return "RIGHT";
	}
}

void PawnEntity::RegisterLuaType(sol::state& lua)
{
	sol::usertype<PawnEntity> type = lua.new_usertype<PawnEntity>("PawnEntity", sol::base_classes, sol::bases<Tara::Entity, Tara::SpriteEntity>()); 
	CONNECT_FUNCTION(PawnEntity, GetAlive);
	CONNECT_FUNCTION(PawnEntity, SetAlive);
	CONNECT_FUNCTION(PawnEntity, GetImmortal);
	CONNECT_FUNCTION(PawnEntity, SetImmortal);
	CONNECT_FUNCTION(PawnEntity, Respawn);
	CONNECT_FUNCTION(PawnEntity, GetTraveling);
	CONNECT_FUNCTION(PawnEntity, Kill);
	CONNECT_FUNCTION_OVERRIDE(PawnEntity, SetTarget);
	CONNECT_FUNCTION_OVERRIDE(PawnEntity, GetDirection);
	CONNECT_FUNCTION_OVERRIDE(PawnEntity, SetDirection);
}
