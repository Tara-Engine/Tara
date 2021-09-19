#include "TColorRectEntity.h"

TColorRectEntity::TColorRectEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name)
	: Entity(parent, owningLayer, transform, name), m_Color(1.0f, 1.0f, 1.0f, 1.0f)
{}

std::shared_ptr<TColorRectEntity> TColorRectEntity::Create(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name)
{
	std::shared_ptr<TColorRectEntity> newEntity = std::make_shared<TColorRectEntity>(parent, owningLayer, transform, name);
	Tara::Entity::Register(newEntity);
	return newEntity;
}

TColorRectEntity::~TColorRectEntity()
{
}

void TColorRectEntity::OnUpdate(float deltaTime)
{
}

void TColorRectEntity::OnDraw(float deltaTime)
{
	//LOG_S(INFO) << "TColorRectEntity draw call!";
	Tara::Renderer::Quad(m_Color, GetWorldTransform());
	Tara::BoundingBox box = GetSpecificBoundingBox();
	Tara::Renderer::DrawBoundingBox(box, { 0.5f, 0.5f, 0.5f, 1.0f });
	box = GetFullBoundingBox();
	Tara::Renderer::DrawBoundingBox(box, { 1.0f, 1.0f, 1.0f, 1.0f });
}
