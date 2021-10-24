#pragma once
#include <Tara.h>
#include "RoomEntity.h"
#include "PawnEntity.h"

REFTYPE(EnemyControllerComponent)
//NOREFTYPE(EnemyControllerComponent)

class EnemyControllerComponent : public Tara::Component {
public:
	EnemyControllerComponent(Tara::EntityNoRef parent, const std::string& name, PawnEntityNoRef player);

	virtual ~EnemyControllerComponent() {}

	virtual void OnUpdate(float deltaTime) override;

	Tara::Vector GetAStarDirection(int32_t x, int32_t y);

private:
	PawnEntityNoRef m_Player;
	Tara::Vector m_LastDir;

	struct ZComparator
	{
		bool operator() (const glm::vec3& a, const glm::vec3& b)
		{
			return a.z > b.z;
		}
	};

	struct vec_hash {
		std::size_t operator() (const glm::vec2& a) const {
			return std::hash<float>()(a.x) ^ std::hash<float>()(a.y);
		}
	};
};