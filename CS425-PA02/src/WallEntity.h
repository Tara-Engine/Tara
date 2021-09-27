#pragma once
#include "Tara/Entities/SpriteEntity.h"

namespace Tara {
	class WallEntity : public SpriteEntity {

	public:
		WallEntity(EntityNoRef parent, LayerNoRef owningLayer, Tara::Transform transform, std::string name);
		WallEntity(EntityNoRef parent, LayerNoRef owningLayer, Tara::Transform transform, std::string name, Texture2DRef texture);

		static std::shared_ptr<WallEntity> Create(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform = TRANSFORM_DEFAULT, std::string name = "SpriteEntity", Tara::Texture2DRef texture = nullptr);

	public:
		virtual ~WallEntity() {};

		//virtual void OnUpdate(float deltaTime) override; //No need to override

		//virtual void OnDraw(float deltaTime) override; //No need to override

		//virtual void OnEvent(Tara::Event& e) override; //No need to override
	};
}