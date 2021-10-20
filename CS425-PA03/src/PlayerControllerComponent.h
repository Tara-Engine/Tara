#pragma once
#include <Tara.h>

REFTYPE(PlayerControllerComponent)


class PlayerControllerComponent : public Tara::Component {
public:
	PlayerControllerComponent(Tara::EntityNoRef parent, const std::string& name);

	static PlayerControllerComponentRef Create(Tara::EntityNoRef parent, const std::string& name);

	virtual ~PlayerControllerComponent() {}

	virtual void OnEvent(Tara::Event& e) override;

	virtual void OnUpdate(float deltaTime) override;

	bool OnKeyPressEvent(Tara::KeyPressEvent& e);
private:
	bool m_SendingToNextLevel = false;
};