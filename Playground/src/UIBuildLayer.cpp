#include "UIBuildLayer.h"
#include <sstream>

UIBuildLayer::UIBuildLayer()
	: Tara::Layer()
{}

UIBuildLayer::~UIBuildLayer()
{}

void UIBuildLayer::Activate()
{
	auto font = Tara::Font::Create("assets/LiberationSans-Regular.ttf", 1024, 96, "arial");

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

	//Tara::UIBaseEntity::SetEnableDebugDraw(true);

	auto base = Tara::CreateEntity<Tara::UIBaseEntity>(Tara::EntityNoRef(), weak_from_this(), "UIBaseEntity");
	base->SetBorder(0, 0, 0, 0);

	auto list = Tara::CreateEntity<Tara::UIListEntity>(base, weak_from_this(), "UIListEntity");
	//list->SetDirecton(Tara::UIListEntity::Direction::Horizontal);
	list->SetSnapRules(Tara::UISnapRule::TOP | Tara::UISnapRule::LEFT);
	list->SetSpacing(10, 10);

	auto vis = Tara::CreateEntity<Tara::UIVisualEntity>(list, weak_from_this(), m_Patch, "UIVisualEntity 1");
	vis->SetSnapRules(Tara::UISnapRule::TOP | Tara::UISnapRule::LEFT);
	vis->SetBorderFromPatch();
	//vis->SetOffsets(0, 1000, 0, 50);
	//vis->SetTint({1, 1, 1, 0.5});

	auto text = Tara::CreateEntity<Tara::UITextEntity>(vis, weak_from_this(), font, "Text Entity");
	text->SetSnapRules(Tara::UISnapRule::TOP | Tara::UISnapRule::LEFT);
	text->SetText("Test:\n[    ]\n[\t]");
	text->SetTextSize(32);

	Tara::CreateComponent<Tara::LambdaComponent>(text, LAMBDA_BEGIN_PLAY_DEFAULT, 
		[this](Tara::LambdaComponent* self, float deltaTime) {
			auto screenPos = Tara::Input::Get()->GetMousePos();
			auto worldPos = this->m_SceneCamera->GetRayFromScreenCoordinate(screenPos.x, screenPos.y);
			auto parent = std::dynamic_pointer_cast<Tara::UITextEntity>(self->GetParent().lock());
			std::stringstream ss;
			ss << "Mouse: " << worldPos.first;
			parent->SetText(ss.str());
		}, 
		LAMBDA_EVENT_DEFAULT
	);

	auto vis2 = Tara::CreateEntity<Tara::UIVisualEntity>(list, weak_from_this(), m_Patch, "UIVisualEntity 2");
	vis2->SetSnapRules(Tara::UISnapRule::TOP |Tara::UISnapRule::LEFT );
	vis2->SetTint({ 0.7, 0.7, 1, 1 });
	vis2->SetBorderFromPatch();
	
	auto spacer = Tara::CreateEntity<Tara::UISpacerEntity>(list, weak_from_this());
	spacer->SetSize(50, 50);
	spacer->SetSnapRules(Tara::UISnapRule::TOP | Tara::UISnapRule::LEFT);

	auto vis3 = Tara::CreateEntity<Tara::UIVisualEntity>(spacer, weak_from_this(), m_Patch, "UIVisualEntity 3");
	vis3->SetSnapRules(Tara::UISnapRule::TOP | Tara::UISnapRule::BOTTOM | Tara::UISnapRule::LEFT | Tara::UISnapRule::RIGHT);
	vis3->SetTint({ 1.0, 0.7, 0.7, 1 });
	vis3->SetBorderFromPatch();
	vis3->SetOffsets(0, 0, 0, 10);

	Tara::CreateComponent<Tara::ClickableComponent>(vis3);

	Tara::CreateComponent<Tara::LambdaComponent>(vis3, LAMBDA_BEGIN_PLAY_DEFAULT, LAMBDA_UPDATE_DEFAULT, 
		[](Tara::LambdaComponent* self, Tara::Event& e) {
			if (e.IsInCategory(Tara::EventCategory::EventCategoryUI)) {
				int* clicks = self->Param<int>("clicks");
				if (clicks) {
					(*clicks)++;
				}
				else {
					self->CreateParam<int>("clicks", 1);
					
				}
				LOG_S(INFO) << e.ToString() << "Clicks: " << *self->Param<int>("clicks");
			}
		}
	);
}

void UIBuildLayer::Draw(float deltaTime)
{
	Layer::Draw(deltaTime);
	
	//Tara::Renderer::BeginScene(m_SceneCamera->GetCamera());

	//Tara::Renderer::Patch(TRANSFORM_2D(0, 0, 0, 500, 300), m_Patch);

	//Tara::Renderer::EndScene();
}

