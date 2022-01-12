#include "tarapch.h"
#include "SpotLightEntity.h"
#include "Tara/Renderer/Renderer.h"

namespace Tara{
	Texture2DRef SpotLightEntity::s_EditorLogo = nullptr;

	SpotLightEntity::SpotLightEntity(EntityNoRef parent, LayerNoRef owningLayer, Transform transform, Vector lightColor, float lightIntensity, float lightAngle, const std::string& name)
		:Entity(parent, owningLayer, transform, name),
		LightBase(lightColor, lightIntensity, 1, LightType::SPOT),
		m_SpotlightAngle(lightAngle),
		m_SpotlightInnerAngle(lightAngle),
		m_DrawingEditorLogo(false)
	{
		SetLightRadiusFromIntensity();
	}

	void SpotLightEntity::OnDraw(float deltaTime)
	{
		Renderer::Light(GetLightData());
		if (s_EditorLogo && m_DrawingEditorLogo) {
			auto camPos = Renderer::GetCurrentSceneData().camera->GetPosition();
			Vector forward = (GetWorldPosition() - camPos).Normalize();
			Transform transform = Transform(GetWorldPosition() - Vector{ 0.5, 0.5, 0.5 }, Rotator::FromForwardVector(forward), { 1,1,1 });
			Renderer::Quad(transform, { 1,1,1,1 }, s_EditorLogo);
		}
	}

	LightData SpotLightEntity::GetLightData()
	{
		float theta = glm::cos(glm::radians(GetSpotlightAngle()));//the cos is so that when we have this value in the shader, we are working with the same units as vector dot products.
		return LightData(
			GetWorldPosition(), GetLightColor(), GetLightIntensity(), GetLightRadius(), GetLightType(),
			GetWorldRotation().GetForwardVector(), 
			theta, 
			glm::cos(glm::radians(GetSpotlightInnerAngle())) - theta //the difference between inner and outer angle
		);
	}
}
