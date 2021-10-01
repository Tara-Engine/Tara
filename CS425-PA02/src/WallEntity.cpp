#include "tarapch.h"
#include "WallEntity.h"
#include "Tara/Renderer/Renderer.h"

namespace Tara {

	WallEntity::WallEntity(EntityNoRef parent, LayerNoRef owningLayer, Transform transform, std::string name)
		: SpriteEntity(parent, owningLayer, transform, name, nullptr)
	{}

	WallEntity::WallEntity(EntityNoRef parent, LayerNoRef owningLayer, Transform transform, std::string name, Tara::Texture2DRef texture)
		: SpriteEntity(parent, owningLayer, transform, name, texture)
	{}


	std::shared_ptr<WallEntity> WallEntity::Create(EntityNoRef parent, LayerNoRef owningLayer, Transform transform, std::string name, Tara::Texture2DRef texture)
	{
		std::shared_ptr<WallEntity> newEntity = std::make_shared<WallEntity>(parent, owningLayer, transform, name, texture);
		//must be done outside of constructor because 
		//you have to have it fully constructed before getting a shared ptr
		Entity::Register(newEntity);
		return newEntity;
	}

}