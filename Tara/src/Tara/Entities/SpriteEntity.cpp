#include "tarapch.h"
#include "SpriteEntity.h"

SpriteEntity::SpriteEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name)
	: Entity(parent, owningLayer, transform, name), m_Texture(nullptr)
{}

std::shared_ptr<SpriteEntity> SpriteEntity::Create(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name)
{
	std::shared_ptr<SpriteEntity> newEntity = std::make_shared<SpriteEntity>(parent, owningLayer, transform, name);
	Tara::Entity::Register(newEntity);
	return newEntity;
}

SpriteEntity::~SpriteEntity()
{}

void SpriteEntity::OnUpdate(float deltaTime)
{}

void SpriteEntity::OnDraw(float deltaTime)
{
	if (m_Texture)
		Tara::Renderer::Quad(m_Texture, m_Transform);
}

void SpriteEntity::OnEvent(Tara::Event& e)
{
	Tara::EventFilter filter(e);
	filter.Call<Tara::OverlapEvent>(TARA_BIND_FN(SpriteEntity::OnOverlapEvent));
}

bool SpriteEntity::OnOverlapEvent(Tara::OverlapEvent& e)
{
	LOG_S(INFO) << "Entity '" << GetName() << "' Overlaped with '" << e.GetOther()->GetName() << " Penetration: " << e.GetPenetration();
	return false;
}