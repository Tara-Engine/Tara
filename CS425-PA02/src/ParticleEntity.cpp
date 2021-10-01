#include "ParticleEntity.h"
#include "WallEntity.h"
#include "PlayerEntity.h"

//some settings for the fun forces
#define MIN_DIST 50
#define MAX_DIST 200
#define FORCE 50.0f

ParticleEntity::ParticleEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name, Tara::Texture2DRef texture)
	:Tara::SpriteEntity(parent, owningLayer, transform, name, texture), m_Velocity(0,0,0)
{
	//if a default texture is supplied, get its asset name
	Tara::Texture2DRef tex = GetTexture();
	if (tex) {
		m_BaseTextureName = tex->GetAssetName();
	}
	else {
		m_BaseTextureName = "";
	}
	m_GlowTextureName = "";
}

//standard creation
std::shared_ptr<ParticleEntity> ParticleEntity::Create(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name, Tara::Texture2DRef texture)
{
	ParticleEntityRef newEntity = std::make_shared<ParticleEntity>(parent, owningLayer, transform, name, texture);
	//must be done outside of constructor because 
	//you have to have it fully constructed before getting a shared ptr
	Tara::Entity::Register(newEntity); 
	return newEntity;
}

//update
void ParticleEntity::OnUpdate(float deltaTime)
{
	//get all the other particles in a specific radius
	auto t = GetWorldTransform();
	Tara::Vector center = t.Position + (t.Scale / 2.0f);
	//get all particles in a radius around ourselves
	auto list = GetOwningLayer().lock()->GetAllEntitiesOfClassInRadius<ParticleEntity>(center, 400);

	//use acceleration-based movement 
	//(thus, no static locking into stable position like in vel-based movement)
	Tara::Vector acceleration = { 0,0,0 };
	for (auto particle : list) {
		if (particle.get() == this) { continue; } //dont consider ourself
		
		//get the distance between particles
		auto otherT = particle->GetWorldTransform();
		Tara::Vector otherPos = otherT.Position + (otherT.Scale / 2.0f);
		float dist = glm::distance(center, otherPos);
		
		if (dist < MIN_DIST) {
			//move away from it!
			//also, make the force stronger as we get closer. 
			acceleration += glm::normalize(otherPos - center) * -(FORCE * FORCE) * 1.0f/(dist/(float)MIN_DIST);
		}
		else if (dist > MAX_DIST) {
			//move towards it!
			acceleration += glm::normalize(otherPos - center) * FORCE;
		}
	}
	//acceleration averaging
	acceleration = acceleration / (float)(list.size() - 1);

	//apply acceleration
	m_Velocity += acceleration * deltaTime;
	auto localTransform = GetRelativeTransform();
	localTransform.Position += m_Velocity * deltaTime;
	
	//bounce off of sides of window
	auto window =  Tara::Application::Get()->GetWindow();
	float width = ((float)window->GetWidth() / 2.0f);
	float height = ((float)window->GetHeight() / 2.0f);
	
	//LEFT
	if (localTransform.Position.x < -width ) {
		if (m_Velocity.x < 0) {
			//set velocity reflected by that axis so it can be used in collision events
			m_Velocity.x *= -1; 
		}
		//set the transform back inside
		localTransform.Position.x = -width;
	}
	//RIGHT
	else if (localTransform.Position.x > width - localTransform.Scale.x) {
		if (m_Velocity.x > 0) {
			m_Velocity.x *= -1;
		}
		localTransform.Position.x = width - localTransform.Scale.x;
	}
	//BOTTOM
	if (localTransform.Position.y < -height) {
		if (m_Velocity.y < 0) {
			m_Velocity.y *= -1;
		}
		localTransform.Position.y = -height;
	}
	//TOP
	else if (localTransform.Position.y > height - localTransform.Scale.y) {
		if (m_Velocity.y > 0) {
			m_Velocity.y *= -1;
		}
		localTransform.Position.y = height - localTransform.Scale.y;
	}
	//update position
	SetTransform(localTransform); 


	//Deal with disabling glow texture after end player overlap
	//if we are glowing and did not overlap the player since last frame
	if (m_IsGlowing && !m_HitPlayerRecently) { 
		//unset glow
		m_IsGlowing = false;
		auto newTex = Tara::AssetLibrary::Get()->GetAssetIf<Tara::Texture2D>(m_BaseTextureName);
		if (newTex) {
			SetTexture(newTex);
		}
	}
	//reset the hit player. If overlap player event happends, will be set to true.
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
		
		if (velAlongNorm <= 0) {
			//if moving towards each other, then change velocity to move away
			float restitution = 1.0f;//bounciness
			float impulseScalar = -(1 + restitution) * velAlongNorm;
			//ideally, this would be devided by the two inverse masses. 
			//But the wall has inf mass, as the particle has a mass of 1, 
			//so, it would be dividing by 1, so it can be skipped

			m_Velocity -= e.GetNormal() * (1+restitution) * velAlongNorm;
		}
		if (e.GetNormal().z != 0) {
			LOG_S(INFO) << "Overlap detected in Z!";
		}

		//positional correction
		auto t = GetRelativeTransform();
		t.Position -= e.GetNormal() * (e.GetPenetration() * 0.8f);
		t.Position.z = 0;
		SetTransform(t);

		return true;//event is dealt with
	}
	
	std::shared_ptr<Tara::PlayerEntity> player = std::dynamic_pointer_cast<Tara::PlayerEntity>(e.GetOther());
	if (player) {
		//overlapping player
		m_HitPlayerRecently = true;
		//if not glowing, glow
		if (!m_IsGlowing){
			m_IsGlowing = true;
			auto newTex = Tara::AssetLibrary::Get()->GetAssetIf<Tara::Texture2D>(m_GlowTextureName);
			if (newTex) {
				SetTexture(newTex);
			}
		}
		return true; //event is dealt with
	}
	
	return false;//event is not dealt with, somethign else could try and deal with it.
} 