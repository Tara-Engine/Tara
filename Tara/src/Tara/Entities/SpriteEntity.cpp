#include "tarapch.h"
#include "SpriteEntity.h"
#include "Tara/Renderer/Renderer.h"

namespace Tara{

	SpriteEntity::SpriteEntity(EntityNoRef parent, LayerNoRef owningLayer, Transform transform, std::string name)
		: Entity(parent, owningLayer, transform, name), m_Texture(nullptr)
	{}

	std::shared_ptr<SpriteEntity> SpriteEntity::Create(EntityNoRef parent, LayerNoRef owningLayer, Transform transform, std::string name)
	{
		std::shared_ptr<SpriteEntity> newEntity = std::make_shared<SpriteEntity>(parent, owningLayer, transform, name);
		Entity::Register(newEntity);
		return newEntity;
	}

	void SpriteEntity::OnDraw(float deltaTime)
	{
		if (m_Texture)
			Renderer::Quad(m_Texture, m_Transform);
	}

}