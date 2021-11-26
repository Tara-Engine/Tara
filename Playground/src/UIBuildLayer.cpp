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
	base->SetBorder(0, 0, 0, 0);

	auto list = Tara::CreateEntity<Tara::UIListEntity>(base, weak_from_this(), "UIListEntity");
	//list->SetDirecton(Tara::UIListEntity::Direction::Horizontal);
	list->SetSnapRules(Tara::UISnapRule::TOP | Tara::UISnapRule::LEFT);
	list->SetSpacing(10, 10);

	auto vis = Tara::CreateEntity<Tara::UIVisualEntity>(list, weak_from_this(), m_Patch, "UIVisualEntity 1");
	vis->SetSnapRules(Tara::UISnapRule::TOP | Tara::UISnapRule::LEFT);
	vis->SetBorderFromPatch();
	vis->SetOffsets(0, 1000, 0, 50);
	//vis->SetTint({1, 1, 1, 0.5});

	auto vis2 = Tara::CreateEntity<Tara::UIVisualEntity>(list, weak_from_this(), m_Patch, "UIVisualEntity 2");
	vis2->SetSnapRules(Tara::UISnapRule::TOP |Tara::UISnapRule::LEFT );
	vis2->SetTint({ 0.7, 0.7, 1, 1 });
	vis2->SetBorderFromPatch();
	
	auto vis3 = Tara::CreateEntity<Tara::UIVisualEntity>(list, weak_from_this(), m_Patch, "UIVisualEntity 3");
	vis3->SetSnapRules(Tara::UISnapRule::TOP | Tara::UISnapRule::LEFT);
	vis3->SetTint({ 0.7, 0.7, 1, 1 });
	vis3->SetBorderFromPatch();
	vis3->SetOffsets(0, 0, 0, 50);

	auto vis4 = Tara::CreateEntity<Tara::UIVisualEntity>(list, weak_from_this(), m_Patch, "UIVisualEntity 2");
	vis4->SetSnapRules(Tara::UISnapRule::TOP | Tara::UISnapRule::LEFT);
	vis4->SetTint({ 1, 0.7, 0.7, 1 });
	vis4->SetBorderFromPatch();

	auto vis5 = Tara::CreateEntity<Tara::UIVisualEntity>(list, weak_from_this(), m_Patch, "UIVisualEntity 3");
	vis5->SetSnapRules(Tara::UISnapRule::TOP | Tara::UISnapRule::LEFT);
	vis5->SetTint({ 1, 0.7, 0.7, 1 });
	vis5->SetBorderFromPatch();
	//vis2->SetOffsets(50, 150, 0, 75);
}

void UIBuildLayer::Draw(float deltaTime)
{
	Layer::Draw(deltaTime);
	
	//Tara::Renderer::BeginScene(m_SceneCamera->GetCamera());

	//Tara::Renderer::Patch(TRANSFORM_2D(0, 0, 0, 500, 300), m_Patch);

	//Tara::Renderer::EndScene();
}

