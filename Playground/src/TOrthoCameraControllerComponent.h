#pragma once
#include <Tara.h>

class TOrthoCameraControllerComponent;
using TOrthoCameraControllerComponentRef = std::shared_ptr<TOrthoCameraControllerComponent>;


class TOrthoCameraControllerComponent : public Tara::Component {
public:
	
	TOrthoCameraControllerComponent(Tara::EntityNoRef parent, const std::string& name);

	virtual ~TOrthoCameraControllerComponent();

	inline virtual void OnBeginPlay() override { ListenForEvents(true);} //enable window event listening

	virtual void OnUpdate(float deltaTime) override;

	virtual void OnEvent(Tara::Event& e) override;

	bool OnKeyPressEvent(Tara::KeyPressEvent& e);

	inline void SetSpeed(float speed) { m_Speed = speed; }

	inline float GetSpeed() const { return m_Speed; }

private:
	float m_Speed = 1;
};
