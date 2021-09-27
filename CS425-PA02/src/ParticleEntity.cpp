#include "ParticleEntity.h"

ParticleEntity::ParticleEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name, Tara::Texture2DRef texture)
	:Tara::SpriteEntity(parent, owningLayer, transform, name, texture)
{

}

std::shared_ptr<ParticleEntity> ParticleEntity::Create(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name, Tara::Texture2DRef texture)
{
	ParticleEntityRef newEntity = std::make_shared<ParticleEntity>(parent, owningLayer, transform, name, texture);
	Tara::Entity::Register(newEntity);
	return newEntity;
}
