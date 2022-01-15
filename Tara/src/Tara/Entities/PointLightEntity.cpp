#include "tarapch.h"
#include "PointLightEntity.h"
#include "Tara/Renderer/Renderer.h"

namespace Tara{

	Texture2DRef PointLightEntity::s_EditorLogo = nullptr;

	PointLightEntity::PointLightEntity(EntityNoRef parent, LayerNoRef owningLayer, Transform transform, Vector lightColor, float lightIntensity, const std::string& name)
		: Entity(parent, owningLayer, transform, name), 
		LightBase(lightColor, lightIntensity, 1, LightType::POINT),
		m_DrawingEditorLogo(false)
	{
		SetLightRadiusFromIntensity();
	}

	void PointLightEntity::OnDraw(float deltaTime)
	{
		Renderer::Light(std::dynamic_pointer_cast<PointLightEntity>(shared_from_this()));
		if (s_EditorLogo && m_DrawingEditorLogo && Renderer::GetCurrentSceneData().renderPass == RenderPass::Standard) {
			auto camPos = Renderer::GetCurrentSceneData().camera->GetPosition();
			Vector forward = (GetWorldPosition() - camPos).Normalize();
			Transform transform = Transform(GetWorldPosition() - Vector{ 0.5, 0.5, 0.5 }, Rotator::FromForwardVector(forward), { 1,1,1 });
			Renderer::Quad(transform, { 1,1,1,1 }, s_EditorLogo);
		}
	}

	LightData PointLightEntity::GetLightData()
	{
		return LightData(GetWorldPosition(), GetLightColor(), GetLightIntensity(), GetLightRadius(), GetLightType());
	}



}
