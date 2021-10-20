#pragma once
#include <Tara.h>

REFTYPE(EnemyControllerComponent)
//NOREFTYPE(EnemyControllerComponent)

class EnemyControllerComponent : public Tara::Component {
public:
	EnemyControllerComponent(Tara::EntityNoRef parent, const std::string& name);

	static EnemyControllerComponentRef Create(Tara::EntityNoRef parent, const std::string& name);

	virtual ~EnemyControllerComponent() {}

	virtual void OnUpdate(float deltaTime) override;
};