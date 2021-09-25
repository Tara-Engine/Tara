#pragma once
#include "Tara/Entities/SpriteEntity.h"

namespace Tara {
	class PlayerEntity : public SpriteEntity {

	public:
		PlayerEntity(EntityNoRef parent, LayerNoRef owningLayer, Tara::Transform transform, std::string name);
		PlayerEntity(EntityNoRef parent, LayerNoRef owningLayer, Tara::Transform transform, std::string name, Texture2DRef texture);

		static std::shared_ptr<PlayerEntity> Create(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform = TRANSFORM_DEFAULT, std::string name = "SpriteEntity", Tara::Texture2DRef texture = nullptr);

	public:
		virtual ~PlayerEntity() {};

		virtual void OnUpdate(float deltaTime) override;

		//virtual void OnDraw(float deltaTime) override; //No need to override

		//virtual void OnEvent(Tara::Event& e) override; //No need to override
	};
}