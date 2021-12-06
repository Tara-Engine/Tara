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
	patchFrame->SetBorderPixels(2, 2, 31, 2);


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

	
	/*
	Tara::UIListEntityRef list = Tara::CreateEntity<Tara::UIListEntity>(base, PARENT_LAYER, "UIListEntity");
	//Snap Rules are a bitmask that controls where in its available area it will snap to. There should be at least one vertical and horizontal bit set.
	//multiple bits per axis (ie, TOP | BOTTOM) will cause it to stretch to fill the space.
	list->SetSnapRules(Tara::UISnapRule::CENTER_HORIZONTAL | Tara::UISnapRule::CENTER_VERTICAL);
	//vertical spacing between elements is 25 pixels
	list->SetSpacing(0, 25);

	
	//create the first button. it is a child of the list. It fills its space
	auto button1 = Tara::CreateEntity<Tara::UIButtonEntity>(list, PARENT_LAYER, patchButtonNormal, patchButtonHover, patchButtonClicked, nullptr);
	button1->SetSnapRules(Tara::UISnapRule::TOP | Tara::UISnapRule::LEFT);
	//UIVisualEntity and its subclasses can set its internal border to match the unstretched part of the patch.
	//children will not be drawn over this border.
	button1->SetBorderFromPatch();

	//create a spacer to enforce button size
	//Tara::UISpacerEntityRef spacer1 = Tara::CreateEntity<Tara::UISpacerEntity>(button1, PARENT_LAYER);
	//UISpacerEntity can have its minimum size set
	//spacer1->SetSize(200, 50);


	//create some text for the button. It is centered on its space
	Tara::UITextEntityRef text1 = Tara::CreateEntity<Tara::UITextEntity>(button1, PARENT_LAYER, font);
	text1->SetSnapRules(Tara::UISnapRule::CENTER_HORIZONTAL | Tara::UISnapRule::CENTER_VERTICAL);
	//UITextEntities can have their text, and its pixel size, set.
	text1->SetText("Basic");
	text1->SetTextSize(32);
	*/

	//frame 1
	{
		auto frame = Tara::CreateEntity<Tara::UIFrameEntity>(base, PARENT_LAYER, patchFrame, 28.0f, "Basic Frame");
		frame->SetBorder(frame->GetBorder() + 5.0f);
		
		auto list = Tara::CreateEntity<Tara::UIListEntity>(frame, PARENT_LAYER, "UIListEntity");
		list->SetSnapRules(Tara::UISnapRule::CENTER_HORIZONTAL | Tara::UISnapRule::CENTER_VERTICAL);
		list->SetSpacing(5, 5);

		auto vis = Tara::CreateEntity<Tara::UIVisualEntity>(list, PARENT_LAYER, m_Patch, "UIVisualEntity 1");
		vis->SetSnapRules(Tara::UISnapRule::TOP | Tara::UISnapRule::LEFT);
		vis->SetBorderFromPatch();
		vis->SetTint({1, 0.8, 0.8, 1});

		auto text = Tara::CreateEntity<Tara::UITextEntity>(vis, PARENT_LAYER, font, "Text Entity");
		text->SetSnapRules(Tara::UISnapRule::CENTER_HORIZONTAL | Tara::UISnapRule::CENTER_VERTICAL);
		text->SetText("Test:\n[    ]\n[\t]");
		text->SetTextSize(32);

		
		Tara::CreateComponent<Tara::LambdaComponent>(text, LAMBDA_BEGIN_PLAY_DEFAULT, 
			[this](Tara::LambdaComponent* self, float deltaTime) {
				auto screenPos = Tara::Input::Get()->GetMousePos();
				//auto worldPos = this->m_SceneCamera->GetRayFromScreenCoordinate(screenPos.x, screenPos.y);
				auto parent = std::dynamic_pointer_cast<Tara::UITextEntity>(self->GetParent().lock());
				std::stringstream ss;
				ss << "Mouse: " << screenPos;
				//LOG_S(INFO) << ss.str();
				parent->SetText(ss.str());
			}, 
			LAMBDA_EVENT_DEFAULT
		);

		//vis 2
		auto button = Tara::CreateEntity<Tara::UIButtonEntity>(list, PARENT_LAYER, patchButtonNormal, patchButtonHover, patchButtonClicked, patchButtonDisabled, "baseButton");
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
						}, 3);
					}
					return true;
				});
			}
		);
		
		auto sizer = Tara::CreateEntity<Tara::UISpacerEntity>(button, PARENT_LAYER);
		sizer->SetSnapRules(Tara::UISnapRule::TOP | Tara::UISnapRule::LEFT);
		sizer->SetSize({ 150, 25 });

		auto text2 = Tara::CreateEntity<Tara::UITextEntity>(button, PARENT_LAYER, font, "Text Entity");
		text2->SetSnapRules(Tara::UISnapRule::TOP | Tara::UISnapRule::LEFT);
		text2->SetText("Clicks: 0");
		text2->SetTextSize(32);

		//debug draw button

		auto button2 = Tara::CreateEntity<Tara::UIButtonEntity>(list, PARENT_LAYER, patchButtonNormal, patchButtonHover, patchButtonClicked, patchButtonDisabled, "debugDrawButton");
		button2->SetSnapRules(Tara::UISnapRule::TOP | Tara::UISnapRule::LEFT);
		button2->SetBorderFromPatch();

		Tara::CreateComponent<Tara::LambdaComponent>(button2, LAMBDA_BEGIN_PLAY_DEFAULT, LAMBDA_UPDATE_DEFAULT,
			[this](Tara::LambdaComponent* self, Tara::Event& e) {
				//LOG_S(INFO) << e.ToString();
				Tara::EventFilter filter(e);
				filter.Call<Tara::UIToggleEvent>([this, self](Tara::UIToggleEvent& ee) {
					Tara::UIBaseEntity::SetEnableDebugDraw(!Tara::UIBaseEntity::GetEnableDebugDraw());
					return true;
				});
			}
		);
		auto text3 = Tara::CreateEntity<Tara::UITextEntity>(button2, PARENT_LAYER, font, "Text Entity");
		text3->SetSnapRules(Tara::UISnapRule::TOP | Tara::UISnapRule::LEFT);
		text3->SetText("Toggle Debug Draw");
		text3->SetTextSize(32);
	}

	//Frame 2
	{
		auto frame = Tara::CreateEntity<Tara::UIFrameEntity>(base, PARENT_LAYER, patchFrame, 28.0f, "Basic Frame");
		frame->SetBorder(frame->GetBorder() + 5.0f);

		auto list = Tara::CreateEntity<Tara::UIListEntity>(frame, PARENT_LAYER, "UIListEntity");
		list->SetSnapRules(Tara::UISnapRule::CENTER_HORIZONTAL | Tara::UISnapRule::CENTER_VERTICAL);
		list->SetSpacing(5, 5);

		auto text1 = Tara::CreateEntity<Tara::UITextEntity>(frame, PARENT_LAYER, font, "Text Entity");
		text1->SetSnapRules(Tara::UISnapRule::CENTER_HORIZONTAL | Tara::UISnapRule::CENTER_VERTICAL);
		text1->SetText("testing Text");
		text1->SetTextSize(24);
	}
}

void UIBuildLayer::Draw(float deltaTime)
{
	Layer::Draw(deltaTime);

	//Tara::Renderer::BeginScene(m_SceneCamera->GetCamera());
	//Tara::Renderer::Quad(TRANSFORM_2D(0, 0, 0, 100, 100), glm::vec4(1, 1, 1, 1));
	//Tara::Renderer::EndScene();
}



