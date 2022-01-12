#include "tarapch.h"
#include "AmbientLightEntity.h"
#include "Tara/Renderer/Renderer.h"

namespace Tara{
	Texture2DRef AmbientLightEntity::s_EditorLogo = nullptr;

	AmbientLightEntity::AmbientLightEntity(EntityNoRef parent, LayerNoRef owningLayer, Transform transform, Vector lightColor, float lightIntensity, const std::string& name)
		: Entity(parent, owningLayer, transform, name),
		LightBase(lightColor, lightIntensity, 0, LightType::AMBIENT),
		m_DrawingEditorLogo(false)
	{}

	void AmbientLightEntity::OnDraw(float deltaTime)
	{
		Renderer::Light(GetLightData());
		if (s_EditorLogo && m_DrawingEditorLogo) {
			auto camPos = Renderer::GetCurrentSceneData().camera->GetPosition();
			Vector forward = (GetWorldPosition() - camPos).Normalize();
			Transform transform = Transform(GetWorldPosition() - Vector{ 0.5, 0.5, 0.5 }, Rotator::FromForwardVector(forward), { 1,1,1 });
			Renderer::Quad(transform, { 1,1,1,1 }, s_EditorLogo);
		}
	}

	LightData AmbientLightEntity::GetLightData()
	{
		return LightData(GetWorldPosition(), GetLightColor(), GetLightIntensity(), GetLightRadius(), GetLightType());
	}


}
