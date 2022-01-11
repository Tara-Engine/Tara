#pragma once
#include "Tara/Core/Entity.h"
#include "Tara/Core/Layer.h"
#include "Tara/Renderer/Light.h"

namespace Tara {

	REFTYPE(LightEntity);
	NOREFTYPE(LightEntity);

	class LightEntity : public Entity, public LightBase {
	public:

		LightEntity(
			EntityNoRef parent, LayerNoRef owningLayer,
			Transform transform = TRANSFORM_DEFAULT,
			Vector lightColor = { 1,1,1 },
			float lightIntensity = 1.0f,
			const std::string& name = "LightEntity"
		);

		virtual ~LightEntity() = default;

		virtual void OnDraw(float deltaTime) override;

		virtual LightData GetLightData() override;
	};
}