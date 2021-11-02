#pragma once
#include <Tara.h>

class EditorCameraControllerComponent : public Tara::Component {
public:
	EditorCameraControllerComponent(Tara::EntityNoRef parent, float speed = 1.0f, const std::string& name = "EditorCameraControllerComponent");

	virtual void OnBeginPlay() override;

	virtual void OnUpdate(float deltaTime) override;

	virtual void OnEvent(Tara::Event& e) override;

	inline float GetSpeed() const { return m_Speed; }

	inline void SetSpeed(float speed) { m_Speed = speed; }

	inline float GetSensitivity() const { return m_MouseSensitivity; }

	inline void SetSensitivity(float sens) { m_MouseSensitivity = sens; }

private:
	float m_Speed;
	float m_MouseSensitivity;
};