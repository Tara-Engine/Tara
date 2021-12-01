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

	m_Patch = Tara::Patch::Create(Tara::Texture2D::Create("assets/Widget_Base.png"), "PatchWidgetBase");
	m_Patch->SetBorderPixels(2, 2, 2, 2);

	auto patchFrame = Tara::Patch::Create(Tara::Texture2D::Create("assets/Frame.png"), "PatchFrame");
	patchFrame->SetBorderPixels(2, 2, 30, 2);


	auto patchButtonNormal = Tara::Patch::Create(Tara::Texture2D::Create("assets/Button_Normal.png"), "PatchButtonNormal");
	patchButtonNormal->SetBorderPixels(5, 5, 5, 5);

	auto patchButtonHover = Tara::Patch::Create(Tara::Texture2D::Create("assets/Button_Hover.png"), "PatchButtonHover");
	patchButtonHover->SetBorderPixels(5, 5, 5, 5);

	auto patchButtonClicked = Tara::Patch::Create(Tara::Texture2D::Create("assets/Button_Clicked.png"), "PatchButtonClicked");
	patchButtonClicked->SetBorderPixels(5, 5, 5, 5);

	auto patchButtonDisabled = Tara::Patch::Create(Tara::Texture2D::Create("assets/Button_Disabled.png"), "PatchButtonDisabled");
	patchButtonDisabled->SetBorderPixels(5, 5, 5, 5);

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

	auto frame = Tara::CreateEntity<Tara::UIFrameEntity>(base, weak_from_this(), patchFrame, 28.0f, "Basic Frame");

	auto list = Tara::CreateEntity<Tara::UIListEntity>(frame, weak_from_this(), "UIListEntity");
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


	//vis 2
	

	
	auto button = Tara::CreateEntity<Tara::UIButtonEntity>(list, weak_from_this(), patchButtonNormal, patchButtonHover, patchButtonClicked, patchButtonDisabled, "baseButton");
	button->SetSnapRules(Tara::UISnapRule::TOP | Tara::UISnapRule::LEFT);
	button->SetBorderFromPatch();
	button->SetTint({ 1, 1, 1, 1 });

	Tara::CreateComponent<Tara::LambdaComponent>(button, LAMBDA_BEGIN_PLAY_DEFAULT, LAMBDA_UPDATE_DEFAULT, 
		[this](Tara::LambdaComponent* self, Tara::Event& e) {
			//LOG_S(INFO) << e.ToString();
			Tara::EventFilter filter(e);
			filter.Call<Tara::UIToggleEvent>([this, self](Tara::UIToggleEvent& ee) {
				int* clickCount = self->Param<int>("clickCount");
				if (!clickCount) {
					self->CreateParam<int>("clickCount", 0);
					clickCount = self->Param<int>("clickCount");
				}
				(*clickCount)++;
				auto parent = self->GetParent().lock();
				if (!parent) { return true; }
				auto disp = parent->GetFirstChildOfType<Tara::UITextEntity>();
				if (!disp) { return true; }
				std::stringstream ss;
				ss << "Clicks: " << *clickCount;
				disp->SetText(ss.str());
				if (*clickCount > 9) {
					auto pparent = std::dynamic_pointer_cast<Tara::UIButtonEntity>(parent);
					if (!pparent) { return true; }
					pparent->SetEnabled(false);
					Tara::After([self, pparent, disp]() {
						pparent->SetEnabled(true);
						(*(self->Param<int>("clickCount"))) = 0;
						disp->SetText("Clicks: 0");
					}, 1);
				}
				return true;
			});
		}
	);
	
	auto sizer = Tara::CreateEntity<Tara::UISpacerEntity>(button, weak_from_this());
	sizer->SetSnapRules(Tara::UISnapRule::TOP | Tara::UISnapRule::LEFT);
	sizer->SetSize({ 150, 25 });

	auto text2 = Tara::CreateEntity<Tara::UITextEntity>(button, weak_from_this(), font, "Text Entity");
	text2->SetSnapRules(Tara::UISnapRule::TOP | Tara::UISnapRule::LEFT);
	text2->SetText("Clicks: 0");
	text2->SetTextSize(32);

	//Vis 3
	
}



