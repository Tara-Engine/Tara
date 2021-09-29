#include "ParticleEntity.h"
#include "WallEntity.h"
#include "PlayerEntity.h"

#define MIN_DIST 50
#define MAX_DIST 200
#define FORCE 50.0f

ParticleEntity::ParticleEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name, Tara::Texture2DRef texture)
	:Tara::SpriteEntity(parent, owningLayer, transform, name, texture), m_Velocity(0,0,0)
{
	Tara::Texture2DRef tex = GetTexture();
	if (tex) {
		m_BaseTextureName = tex->GetAssetName();
	}
	else {
		m_BaseTextureName = "";
	}
	m_GlowTextureName = "";
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

	Tara::Vector acceleration = { 0,0,0 };
	for (auto particle : list) {
		if (particle.get() == this) { continue; } //dont consider ourself
		auto otherT = particle->GetWorldTransform();
		Tara::Vector otherPos = otherT.Position + (otherT.Scale / 2.0f);
		float dist = glm::distance(center, otherPos);
		if (dist < MIN_DIST) {
			//move away from it!
			acceleration += glm::normalize(otherPos - center) * -(FORCE * FORCE) * 1.0f/(dist/(float)MIN_DIST);
		}
		else if (dist > MAX_DIST) {
			//move towards it!
			acceleration += glm::normalize(otherPos - center) * FORCE;
		}
	}
	acceleration = acceleration / (float)(list.size() - 1);

	m_Velocity += acceleration * deltaTime; //set for use in collision events
	auto localTransform = GetRelativeTransform();
	localTransform.Position += m_Velocity * deltaTime;
	
	//bounce off of sides of window
	auto window =  Tara::Application::Get()->GetWindow();
	float width = ((float)window->GetWidth() / 2.0f);
	float height = ((float)window->GetHeight() / 2.0f);
	if (localTransform.Position.x < -width ) {
		if (m_Velocity.x < 0) {
			m_Velocity.x *= -1;
		}
		localTransform.Position.x = -width;
	}
	else if (localTransform.Position.x > width - localTransform.Scale.x) {
		if (m_Velocity.x > 0) {
			m_Velocity.x *= -1;
		}
		localTransform.Position.x = width - localTransform.Scale.x;
	}

	if (localTransform.Position.y < -height) {
		if (m_Velocity.y < 0) {
			m_Velocity.y *= -1;
		}
		localTransform.Position.y = -height;
	}
	else if (localTransform.Position.y > height - localTransform.Scale.y) {
		if (m_Velocity.y > 0) {
			m_Velocity.y *= -1;
		}
		localTransform.Position.y = height - localTransform.Scale.y;
	}

	SetTransform(localTransform); //update position

	//glow stuff
	if (m_IsGlowing && !m_HitPlayerRecently) {
		//unset glow
		m_IsGlowing = false;
		auto newTex = Tara::AssetLibrary::Get()->GetAssetIf<Tara::Texture2D>(m_BaseTextureName);
		if (newTex) {
			SetTexture(newTex);
		}
	}
	//reset this
	m_HitPlayerRecently = false;
}


//deal with overlaps
bool ParticleEntity::OnOverlapEvent(Tara::OverlapEvent& e)
{
	std::shared_ptr<Tara::WallEntity> wall = std::dynamic_pointer_cast<Tara::WallEntity>(e.GetOther());
	if (wall) {
		//colliding with a wall
		auto wt = GetWorldTransform();
		//LOG_S(INFO) << "Particle overlap wall! << POS: {x=" << wt.Position.x << ",y=" << wt.Position.y << ",z=" << wt.Position.z << "}";

		//Get how much velocity is along the normal
		float velAlongNorm = glm::dot(m_Velocity, e.GetNormal());
		
		//if (velAlongNorm <= 0) {
		//if moving towards each other, then change velocity to move away
		float restitution = 1.0f;//bounciness
		float impulseScalar = -(1 + restitution) * velAlongNorm;
		//ideally, this would be devided by the two inverse masses. 
		//But the wall has inf mass, as the particle has a mass of 1, 
		//so, it would be dividing by 1, so it can be skipped

		m_Velocity -= e.GetNormal() * (1+restitution) * velAlongNorm;
		//}
		if (e.GetNormal().z != 0) {
			LOG_S(INFO) << "Overlap detected in Z!";
		}


		//positional correction
		auto t = GetRelativeTransform();
		t.Position -= e.GetNormal() * (e.GetPenetration() * 0.8f);
		t.Position.z = 0;
		SetTransform(t);

		return true;
	}
	
	std::shared_ptr<Tara::PlayerEntity> player = std::dynamic_pointer_cast<Tara::PlayerEntity>(e.GetOther());
	if (player) {
		m_IsGlowing = true;
		m_HitPlayerRecently = true;
		auto newTex = Tara::AssetLibrary::Get()->GetAssetIf<Tara::Texture2D>(m_GlowTextureName);
		if (newTex) {
			SetTexture(newTex);
		}

		return true;
	}
	
	return false;
}
