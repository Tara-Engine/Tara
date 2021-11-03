#include "EditorCameraControllerComponent.h"

EditorCameraControllerComponent::EditorCameraControllerComponent(Tara::EntityNoRef parent, float speed, const std::string& name)
	: Tara::Component(parent, name), m_Speed(speed), m_MouseSensitivity(1.5f)
{}

void EditorCameraControllerComponent::OnBeginPlay()
{
	ListenForEvents(true);
}

void EditorCameraControllerComponent::OnUpdate(float deltaTime)
{
	//movement
	Tara::Vector offset(0.0f);
	Tara::Rotator parentRotation = GetParent().lock()->GetWorldRotation();
	if (Tara::Input::Get()->IsKeyPressed(TARA_KEY_W)) {
		//move forwards
		offset -= parentRotation.GetForwardVector();
	}
	if (Tara::Input::Get()->IsKeyPressed(TARA_KEY_S)) {
		//move backwards
		offset += parentRotation.GetForwardVector();
	}
	if (Tara::Input::Get()->IsKeyPressed(TARA_KEY_A)) {
		//move left
		offset -= parentRotation.GetRightVector();
	}
	if (Tara::Input::Get()->IsKeyPressed(TARA_KEY_D)) {
		//move right
		offset += parentRotation.GetRightVector();
	}
	if (Tara::Input::Get()->IsKeyPressed(TARA_KEY_Q)) {
		//move Up (orthogonal)
		offset -= Tara::Vector(0.0f, 1.0f, 0.0f);
	}
	if (Tara::Input::Get()->IsKeyPressed(TARA_KEY_E)) {
		//move down (orthogonal)
		offset += Tara::Vector(0.0f, 1.0f, 0.0f);
	}
	if (offset != Tara::Vector(0, 0, 0)) {
		offset.Normalize();
		auto pos = GetParent().lock()->GetWorldPosition();
		pos += (offset * m_Speed * deltaTime);
		GetParent().lock()->SetWorldPosition(pos);
	}

	//rotation
	static glm::vec2 prevMousePos = Tara::Input::Get()->GetMousePos();
	if (Tara::Input::Get()->IsMouseDown(TARA_MOUSE_BUTTON_2)) {
		auto rawMouseNow = Tara::Input::Get()->GetMousePos();
		glm::vec2 dMouse = (prevMousePos - rawMouseNow) / m_MouseSensitivity;
		float pitch = parentRotation.Pitch + dMouse.y;
		pitch = (pitch > 90) ? 90 : ((pitch < -90) ? -90 : pitch);
		Tara::Rotator newRot{ 0.0f, pitch, parentRotation.Yaw + dMouse.x };
		LOG_S(INFO) << "Rotation" << newRot;

		GetParent().lock()->SetWorldRotation(newRot);
		prevMousePos = rawMouseNow;
	}
	else {
		prevMousePos = Tara::Input::Get()->GetMousePos();
	}
}

void EditorCameraControllerComponent::OnEvent(Tara::Event& e)
{
	Tara::EventFilter filter(e);
	filter.Call<Tara::MouseScrollEvent>([this](Tara::MouseScrollEvent& ee) {
		if (ee.GetYOffset() > 0) {
			this->SetSpeed(this->GetSpeed() * 1.5f);
		}
		else {
			this->SetSpeed(this->GetSpeed() / 1.5f);
		}
		return false;
	});
}
