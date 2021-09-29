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

	inline virtual void OnEvent(Tara::Event& e) override {
		Tara::EventFilter filter(e);
		filter.Call<Tara::OverlapEvent>(TARA_BIND_FN(ParticleEntity::OnOverlapEvent));
	} 

	bool OnOverlapEvent(Tara::OverlapEvent& e);

	inline void SetBaseTextureName(std::string name) { m_BaseTextureName = name; }
	inline void SetGlowTextureName(std::string name) { m_GlowTextureName = name; }
private:
	Tara::Vector m_Velocity;
	std::string m_BaseTextureName;
	std::string m_GlowTextureName;
	bool m_HitPlayerRecently = false;
	bool m_IsGlowing = false;
};

using ParticleEntityRef = std::shared_ptr<ParticleEntity>;