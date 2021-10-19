#pragma once
#include <Tara.h>

REFTYPE(PlayerControllerComponent)


class PlayerControllerComponent : public Tara::Component {
public:
	PlayerControllerComponent(Tara::EntityNoRef parent, const std::string& name);

	static PlayerControllerComponentRef Create(Tara::EntityNoRef parent, const std::string& name);

	virtual ~PlayerControllerComponent() {}

	virtual void OnEvent(Tara::Event& e) override;

	bool OnKeyPressEvent(Tara::KeyPressEvent& e);
};