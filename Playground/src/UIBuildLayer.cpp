#include "UIBuildLayer.h"

UIBuildLayer::UIBuildLayer()
	: Tara::Layer()
{}

UIBuildLayer::~UIBuildLayer()
{}

void UIBuildLayer::Activate()
{
	

	auto textureUVChecker = Tara::Texture2D::Create("assets/Button_Normal.png");
	m_Patch = Tara::Patch::Create(textureUVChecker, "ButtonPatch");
	m_Patch->SetBorderPixels(16, 16, 16, 16);

	m_SceneCamera = Tara::CreateEntity<Tara::CameraEntity>(
		Tara::EntityNoRef(), weak_from_this(),
		Tara::Camera::ProjectionType::Screen,
		TRANSFORM_DEFAULT,
		"camera"
	);

	SetLayerCamera(m_SceneCamera);

	auto base = Tara::CreateEntity<Tara::UIBaseEntity>(Tara::EntityNoRef(), weak_from_this(), "UIBaseEntity");
	base->SetBorder(10, 20, 5, 30);
	auto vis = Tara::CreateEntity<Tara::UIVisualEntity>(base, weak_from_this(), m_Patch, "UIVisualEntity 1");
	vis->SetSnapRules(Tara::UISnapRule::RIGHT | Tara::UISnapRule::CENTER_HORIZONTAL | Tara::UISnapRule::BOTTOM | Tara::UISnapRule::CENTER_VERTICAL);
	vis->SetBorderFromPatch();

	auto vis2 = Tara::CreateEntity<Tara::UIVisualEntity>(vis, weak_from_this(), m_Patch, "UIVisualEntity 2");
	vis2->SetSnapRules(Tara::UISnapRule::TOP | Tara::UISnapRule::LEFT);
	vis2->SetTint({ 0.7, 0.7, 1, 1 });
	vis2->SetBorderFromPatch();
	vis2->SetOffsets(50, 150, 0, 75);
}

void UIBuildLayer::Draw(float deltaTime)
{
	Layer::Draw(deltaTime);
	
	//Tara::Renderer::BeginScene(m_SceneCamera->GetCamera());

	//Tara::Renderer::Patch(TRANSFORM_2D(0, 0, 0, 500, 300), m_Patch);

	//Tara::Renderer::EndScene();
}

