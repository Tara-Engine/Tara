#pragma once
#include <Tara.h>


class ParticleEntity : public Tara::SpriteEntity {

public:
	ParticleEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name, Tara::Texture2DRef texture);

	virtual ~ParticleEntity() {}

	static std::shared_ptr<ParticleEntity> Create(
		Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, 
		Tara::Transform transform = TRANSFORM_DEFAULT, 
		std::string name = "ParticleEntity", 
		Tara::Texture2DRef texture = nullptr
	);


	virtual void OnUpdate(float deltaTime) override; //No need to override

	//virtual void OnDraw(float deltaTime) override; //No need to override

	//virtual void OnEvent(Tara::Event& e) override; //No need to override
private:
	Tara::Vector m_Velocity;
};

using ParticleEntityRef = std::shared_ptr<ParticleEntity>;