#include "TColorRectEntity.h"

TColorRectEntity::TColorRectEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name)
	: Entity(parent, owningLayer, transform, name), m_Color(1.0f, 1.0f, 1.0f, 1.0f)
{}

TColorRectEntity::~TColorRectEntity()
{
}

void TColorRectEntity::OnBeginPlay()
{
	LOG_S(INFO) << "TColorRectEntity OnBeginPlay() called!";
}

void TColorRectEntity::OnUpdate(float deltaTime)
{
}

void TColorRectEntity::OnDraw(float deltaTime)
{
	//LOG_S(INFO) << "TColorRectEntity draw call!";
	Tara::Renderer::Quad(GetWorldTransform(), m_Color);
	//Tara::BoundingBox box = GetSpecificBoundingBox();
	//Tara::Renderer::DrawBoundingBox(box, { 0.5f, 0.5f, 0.5f, 1.0f });
	//box = GetFullBoundingBox();
	//Tara::Renderer::DrawBoundingBox(box, { 1.0f, 1.0f, 1.0f, 1.0f });
}

void TColorRectEntity::OnEvent(Tara::Event& e)
{
	LOG_S(INFO) << e.ToString();
	Tara::EventFilter filter(e);
	filter.Call<Tara::OverlapEvent>(TARA_BIND_FN(TColorRectEntity::OnOverlapEvent));
}

bool TColorRectEntity::OnOverlapEvent(Tara::OverlapEvent& e)
{
	//LOG_S(INFO) << "Entity '" << GetName() << "' Overlaped with '" << e.GetOther()->GetName() << " Penetration: " << e.GetPenetration();
	return false;
}

