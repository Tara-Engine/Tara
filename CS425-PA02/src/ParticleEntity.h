#pragma once
#include <Tara.h>


/// <summary>
/// Particle Entity, a subclass of Tara::SpriteEntity
/// </summary>
class ParticleEntity : public Tara::SpriteEntity {

public:
	// Constructor
	ParticleEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name, Tara::Texture2DRef texture);

	virtual ~ParticleEntity() {}

	/// <summary>
	/// Create a new particle
	/// </summary>
	/// <param name="parent">parent entity</param>
	/// <param name="owningLayer">owning layer</param>
	/// <param name="transform">initial transform</param>
	/// <param name="name">entity name</param>
	/// <param name="texture">entity texture</param>
	/// <returns>a ref to the particle</returns>
	static std::shared_ptr<ParticleEntity> Create(
		Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, 
		Tara::Transform transform = TRANSFORM_DEFAULT, 
		std::string name = "ParticleEntity", 
		Tara::Texture2DRef texture = nullptr
	);

	/// <summary>
	/// Called every frame in the Update stage
	/// </summary>
	/// <param name="deltaTime"></param>
	virtual void OnUpdate(float deltaTime) override; 

	
	//virtual void OnDraw(float deltaTime) override; //No need to override, handled by SpriteEntity

	/// <summary>
	/// Called when an event happens
	/// By default, it does not get called for window events
	/// However, an entity can choose to listen for them
	/// </summary>
	/// <param name="e">the event</param>
	inline virtual void OnEvent(Tara::Event& e) override {
		Tara::EventFilter filter(e);
		filter.Call<Tara::OverlapEvent>(TARA_BIND_FN(ParticleEntity::OnOverlapEvent));
	} 

	/// <summary>
	/// Custom overlap event callback
	/// </summary>
	/// <param name="e"></param>
	/// <returns></returns>
	bool OnOverlapEvent(Tara::OverlapEvent& e);

	/// <summary>
	/// Setting asset names for the two textures. Texture2DRef objects are loaded using AssetManager
	/// </summary>
	/// <param name="name">the name of the asset</param>
	inline void SetBaseTextureName(std::string name) { m_BaseTextureName = name; }
	
	/// <summary>
	/// Setting asset names for the two textures. Texture2DRef objects are loaded using AssetManager
	/// </summary>
	/// <param name="name">the name of the asset</param>
	inline void SetGlowTextureName(std::string name) { m_GlowTextureName = name; }
private:
	Tara::Vector m_Velocity;
	std::string m_BaseTextureName;
	std::string m_GlowTextureName;
	bool m_HitPlayerRecently = false;
	bool m_IsGlowing = false;
};

using ParticleEntityRef = std::shared_ptr<ParticleEntity>;