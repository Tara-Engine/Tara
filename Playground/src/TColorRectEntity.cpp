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

void TColorRectEntity::OnEvent(Tara::Event& e)
{
	Tara::EventFilter filter(e);
	filter.Call<Tara::OverlapEvent>(TARA_BIND_FN(TColorRectEntity::OnOverlapEvent));
}

bool TColorRectEntity::OnOverlapEvent(Tara::OverlapEvent& e)
{
	//LOG_S(INFO) << "Entity '" << GetName() << "' Overlaped with '" << e.GetOther()->GetName();
	return false;
}


TControlableEntity::TControlableEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name)
	:TColorRectEntity(parent, owningLayer, transform, name), m_Speed(1.0f)
{
}

std::shared_ptr<TControlableEntity> TControlableEntity::Create(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name)
{
	std::shared_ptr<TControlableEntity> newEntity = std::make_shared<TControlableEntity>(parent, owningLayer, transform, name);
	Tara::Entity::Register(newEntity);
	return newEntity;
}

TControlableEntity::~TControlableEntity()
{
}

void TControlableEntity::OnUpdate(float deltaTime)
{
	Tara::Vector offset = { 0.0f,0.0f,0.0f };
	if (Tara::Input::Get()->IsKeyPressed(TARA_KEY_S)) {
		offset.y -= 1;
	}
	if (Tara::Input::Get()->IsKeyPressed(TARA_KEY_W)) {
		offset.y += 1;
	}
	if (Tara::Input::Get()->IsKeyPressed(TARA_KEY_A)) {
		offset.x -= 1;
	}
	if (Tara::Input::Get()->IsKeyPressed(TARA_KEY_D)) {
		offset.x += 1;
	}

	auto t = GetRelativeTransform();
	t.Position += offset * deltaTime * m_Speed;
	SetTransform(t);
}
