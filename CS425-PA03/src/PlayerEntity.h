#pragma once
#include <Tara.h>

//this should not be here ... 


/// <summary>
/// The Player is also a subclass of SpriteEntity
/// </summary>
class PlayerEntity : public Tara::SpriteEntity {

public:
	//construtor
	PlayerEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name, Tara::SpriteRef sprite);

	/// <summary>
	/// Create a new Player
	/// </summary>
	/// <param name="parent">parent entity</param>
	/// <param name="owningLayer">owning layer</param>
	/// <param name="transform">initial transform</param>
	/// <param name="name">entity name</param>
	/// <param name="texture">entity texture</param>
	/// <returns>a ref to the particle</returns>
	static std::shared_ptr<PlayerEntity> Create(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform = TRANSFORM_DEFAULT, std::string name = "SpriteEntity", Tara::SpriteRef sprite = nullptr);

	virtual ~PlayerEntity() {};
	
	/// <summary>
	/// Called every frame in the Update stage. 
	/// The player polls for keys and uses them to move.
	/// Because it polls, there is no need to listen for events
	/// </summary>
	/// <param name="deltaTime"></param>
	virtual void OnUpdate(float deltaTime) override;

	//virtual void OnDraw(float deltaTime) override; //No need to override, handled by SpriteEntity

	/// <summary>
	/// An event is delivered to the entity
	/// </summary>
	/// <param name="e"></param>
	virtual void OnEvent(Tara::Event& e) override;

	/// <summary>
	/// A key press event
	/// </summary>
	/// <param name="e"></param>
	/// <returns></returns>
	bool OnKeyPressEvent(Tara::KeyPressEvent& e);

	/// <summary>
	/// Set the target for the player
	/// </summary>
	/// <param name="target">the new target</param>
	/// <param name="travelTime">the time to reach it</param>
	void SetTarget(Tara::Vector target, float travelTime = 0.25f);

private:
	Tara::Vector m_Target, m_Origin;
	float m_Timer;
	float m_MaxTime = 0.25f; // 1/4 second
	bool m_Traveling = false;
};
