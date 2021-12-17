#pragma once
#include <Tara.h>


REFTYPE(PawnEntity)
NOREFTYPE(PawnEntity)

/// <summary>
/// The Pawn is also a subclass of SpriteEntity
/// </summary>
class PawnEntity : public Tara::SpriteEntity {
	enum class Direction : uint8_t {
		UP, DOWN, LEFT, RIGHT
	};

public:
	/// <summary>
	/// Construct a new pawn entity
	/// </summary>
	/// <param name="parent">the parent entity</param>
	/// <param name="owningLayer">the owning layer</param>
	/// <param name="transform">the local transform (optional)</param>
	/// <param name="name">the name (optional)</param>
	/// <param name="sprite">sprite (optional)</param>
	PawnEntity(Tara::EntityNoRef parent, Tara::LayerNoRef owningLayer, Tara::Transform transform = TRANSFORM_DEFAULT, std::string name = "PawnEntity", Tara::SpriteRef sprite = nullptr);

	virtual ~PawnEntity() {};
	
	/// <summary>
	/// Called every frame in the Update stage. 
	/// </summary>
	/// <param name="deltaTime"></param>
	virtual void OnUpdate(float deltaTime) override;

	/// <summary>
	/// Set the target for the Pawn
	/// </summary>
	/// <param name="target">the new target</param>
	/// <param name="travelTime">the time to reach it</param>
	void SetTarget(Tara::Vector target, float travelTime = 0.25f);

	/// <summary>
	/// get if the Pawn is alive
	/// </summary>
	/// <returns></returns>
	inline bool GetAlive() const { return m_Alive; }

	/// <summary>
	/// Set if the Pawn is alive. if false and the Pawn is immortal, it will have no effect
	/// </summary>
	/// <param name="alive"></param>
	inline void SetAlive(bool alive) { m_Alive = alive || m_Immortal; }

	/// <summary>
	/// get if the Pawn is immortal
	/// </summary>
	/// <returns></returns>
	inline bool GetImmortal() const { return m_Immortal; }

	/// <summary>
	/// Set if the Pawn is immortal. If true, will automatically make alive.
	/// </summary>
	/// <param name="alive"></param>
	inline void SetImmortal(bool immortal) { m_Immortal = immortal; m_Alive |= immortal; }

	/// <summary>
	/// Get the pawn's direction
	/// </summary>
	/// <returns></returns>
	inline Direction GetDirection() const { return m_Direction; }

	/// <summary>
	/// Set the pawn's direction
	/// </summary>
	/// <param name="dir"></param>
	inline void SetDirection(Direction dir) { m_Direction = dir; }

	/// <summary>
	/// Set Pawn back to spawn
	/// </summary>
	void Respawn();
	 
	/// <summary>
	/// Check if pawn is traveling
	/// </summary>
	/// <returns></returns>
	inline bool GetTraveling() const { return m_Traveling; }

	/// <summary>
	/// Kill the pawn
	/// </summary>
	void Kill();

public:

	void __SCRIPT__SetTarget(sol::table vector, float time = 0.25f);
	void __SCRIPT__SetDirection(std::string dir);
	std::string __SCRIPT__GetDirection() const;


	/// <summary>
	/// Register the lua type
	/// </summary>
	/// <param name="lua"></param>
	static void RegisterLuaType(sol::state& lua);

private:
	Tara::Vector m_Target, m_Origin, m_Spawn;
	float m_Timer;
	float m_MaxTime = 0.25f; // 1/4 second
	Direction m_Direction;
	bool m_Traveling;
	bool m_Alive;
	bool m_Immortal;
};
