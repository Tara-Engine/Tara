#pragma once
#include <Tara.h>

//this should not be here ... 


/// <summary>
/// The Player is also a subclass of SpriteEntity
/// </summary>
class PawnEntity : public Tara::SpriteEntity {
	enum class Direction : uint8_t {
		UP, DOWN, LEFT, RIGHT
	};

public:
	//construtor
	PawnEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform, std::string name, Tara::SpriteRef sprite);

	/// <summary>
	/// Create a new Player
	/// </summary>
	/// <param name="parent">parent entity</param>
	/// <param name="owningLayer">owning layer</param>
	/// <param name="transform">initial transform</param>
	/// <param name="name">entity name</param>
	/// <param name="texture">entity texture</param>
	/// <returns>a ref to the particle</returns>
	static std::shared_ptr<PawnEntity> Create(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform = TRANSFORM_DEFAULT, std::string name = "SpriteEntity", Tara::SpriteRef sprite = nullptr);

	virtual ~PawnEntity() {};
	
	/// <summary>
	/// Called every frame in the Update stage. 
	/// The player polls for keys and uses them to move.
	/// Because it polls, there is no need to listen for events
	/// </summary>
	/// <param name="deltaTime"></param>
	virtual void OnUpdate(float deltaTime) override;

	/// <summary>
	/// Set the target for the player
	/// </summary>
	/// <param name="target">the new target</param>
	/// <param name="travelTime">the time to reach it</param>
	void SetTarget(Tara::Vector target, float travelTime = 0.25f);

	/// <summary>
	/// get if the player is alive
	/// </summary>
	/// <returns></returns>
	inline bool GetAlive() const { return m_Alive; }

	/// <summary>
	/// Set if the player is alive
	/// </summary>
	/// <param name="alive"></param>
	inline void SetAlive(bool alive) { m_Alive = alive; }

	/// <summary>
	/// Set player back to spawn
	/// </summary>
	void Respawn();
	 
	virtual bool GetTraveling() const { return m_Traveling; }

private:
	Tara::Vector m_Target, m_Origin, m_Spawn;
	float m_Timer;
	float m_MaxTime = 0.25f; // 1/4 second
	bool m_Traveling = false;
	Direction m_Direction;
	bool m_Alive = true;
};
