#include "TOrthoCameraControllerComponent.h"

TOrthoCameraControllerComponent::TOrthoCameraControllerComponent(Tara::EntityNoRef parent, const std::string& name)
	: Tara::Component(parent, name)
{}

TOrthoCameraControllerComponent::~TOrthoCameraControllerComponent()
{
	LOG_S(INFO) << "Controller component destroyed!";
}

TOrthoCameraControllerComponentRef TOrthoCameraControllerComponent::Create(Tara::EntityNoRef parent, const std::string & name)
{
	auto comp = std::make_shared<TOrthoCameraControllerComponent>(parent, name);
	Register(comp);
	comp->ListenForEvents(true);
	return comp;
}

void TOrthoCameraControllerComponent::OnUpdate(float deltaTime)
{
	Tara::Vector offset = { 0.0f,0.0f,0.0f };
	if (Tara::Input::Get()->IsKeyPressed(TARA_KEY_S)) {
		offset.y -= 1;
	}
	if (Tara::Input::Get()->IsKeyPressed(TARA_KEY_W)) {
		offset.y += 1;
	}
	if (Tara::Input::Get()->IsKeyPressed(TARA_KEY_A)) {
		offset.x -= 1;
	}
	if (Tara::Input::Get()->IsKeyPressed(TARA_KEY_D)) {
		offset.x += 1;
	}

	auto t = GetParent().lock()->GetRelativeTransform();
	t.Position += offset * deltaTime * m_Speed;
	GetParent().lock()->SetRelativeTransform(t);
}

void TOrthoCameraControllerComponent::OnEvent(Tara::Event& e)
{
	//LOG_S(INFO) << "Event called!";
	Tara::EventFilter filter(e);
	filter.Call<Tara::KeyPressEvent>(TARA_BIND_FN(TOrthoCameraControllerComponent::OnKeyPressEvent));
}

bool TOrthoCameraControllerComponent::OnKeyPressEvent(Tara::KeyPressEvent& e)
{
	if (e.getKey() == TARA_KEY_Q) {
		LOG_S(INFO) << "Removing controller from parent";
		GetParent().lock()->RemoveComponentByRef(shared_from_this());
	}
	return false;
}


