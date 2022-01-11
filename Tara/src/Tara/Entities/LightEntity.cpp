#include "tarapch.h"
#include "LightEntity.h"
#include "Tara/Renderer/Renderer.h"

namespace Tara{
	LightEntity::LightEntity(EntityNoRef parent, LayerNoRef owningLayer, Transform transform, Vector lightColor, float lightIntensity, const std::string& name)
		: Entity(parent, owningLayer, transform, name), 
		LightBase(lightColor, lightIntensity, LightType::POINT)
	{}

	void LightEntity::OnDraw(float deltaTime)
	{
		Renderer::Light(GetLightData());
	}

	LightData LightEntity::GetLightData()
	{
		return LightData(GetWorldPosition(), GetLightColor(), GetLightIntensity(), GetLightType());
	}



}
