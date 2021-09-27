#include "ParticleEntity.h"

#define MIN_DIST 100
#define MAX_DIST 300
#define FORCE 2.0f

ParticleEntity::ParticleEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name, Tara::Texture2DRef texture)
	:Tara::SpriteEntity(parent, owningLayer, transform, name, texture), m_Velocity(0,0,0)
{

}

std::shared_ptr<ParticleEntity> ParticleEntity::Create(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name, Tara::Texture2DRef texture)
{
	ParticleEntityRef newEntity = std::make_shared<ParticleEntity>(parent, owningLayer, transform, name, texture);
	Tara::Entity::Register(newEntity);
	return newEntity;
}


void ParticleEntity::OnUpdate(float deltaTime)
{
	//get all the other particles in a specific radius
	auto t = GetWorldTransform();
	Tara::Vector center = t.Position + (t.Scale / 2.0f);
	auto list = GetOwningLayer().lock()->GetAllEntitiesOfClassInRadius<ParticleEntity>(center, 400);
	//LOG_S(INFO) << "ParticleEntity List Length: " << list.size();

	Tara::Vector vel = { 0,0,0 };
	for (auto particle : list) {
		if (particle.get() == this) { continue; } //dont consider ourself
		auto otherT = particle->GetWorldTransform();
		Tara::Vector otherPos = otherT.Position + (otherT.Scale / 2.0f);
		float dist = glm::distance(center, otherPos);
		if (dist < MIN_DIST) {
			//move away from it!
			vel += glm::normalize(otherPos - center) * -(FORCE * FORCE);
		}
		else if (dist > MAX_DIST) {
			//move towards it!
			vel += glm::normalize(otherPos - center) * FORCE;
		}
	}

	m_Velocity = vel; //set for use in collision events
	auto localTransform = GetRelativeTransform();
	localTransform.Position += vel * deltaTime;
	SetTransform(localTransform); //update position
}
