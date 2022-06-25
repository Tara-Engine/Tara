#include "ModelBuildLayer.h"
#include "EditorCameraControllerComponent.h"
#include <random>


ModelBuildLayer::ModelBuildLayer()
	:Tara::Layer()
{
}

ModelBuildLayer::~ModelBuildLayer()
{
}

void ModelBuildLayer::Activate()
{
	//camera creation
	m_Camera = Tara::CreateEntity<Tara::CameraEntity>(Tara::EntityNoRef(), weak_from_this(),
		Tara::Camera::ProjectionType::Perspective, Tara::Transform{ {0.5,0.5,3},{0,0,0},{1,1,1} });
	m_Camera->SetPerspectiveFOV(90);
	SetLayerCamera(m_Camera);
	Tara::CreateComponent<EditorCameraControllerComponent>(m_Camera, 1.0f, "EditorCameraControllerComponent");
	std::dynamic_pointer_cast<Tara::PerspectiveCamera>(m_Camera->GetCamera())->SetFarClipPlane(500);

	m_ScreenCamera  = Tara::CreateEntity<Tara::CameraEntity>(Tara::EntityNoRef(), weak_from_this(), 
		Tara::Camera::ProjectionType::Screen, TRANSFORM_DEFAULT);

	/*
	auto& window = Tara::Application::Get()->GetWindow();
	m_DepthTarget = Tara::RenderTarget::Create(window->GetWidth(), window->GetHeight(), 0, Tara::RenderTarget::InternalType::FLOAT, true, "DepthTarget");
	m_Camera->SetRenderTarget(m_DepthTarget);
	*/

	//Scene setup stuff
	{
		//materials
		auto basicTexture = Tara::Texture2D::Create("assets/Widget_Base.png");
		auto wallTexture = Tara::Texture2D::Create("assets/wall.png");

		auto unlitMat = Tara::Material::Create(Tara::MaterialType::UNLIT, "assets/material1.glsl", Tara::Shader::SourceType::TextFiles, "UnlitMaterial");
		unlitMat->SetVector4Parameter("tintColor", glm::vec4(0, 0, 1, 1));

		auto litMat = Tara::Material::Create(Tara::MaterialType::LIT, "assets/material3deferred.glsl", Tara::Shader::SourceType::TextFiles, "BasicMaterial");
		litMat->SetVector4Parameter("tintColor", glm::vec4(1, 1, 1, 1));
		litMat->SetScalarParameter("roughnessValue", 1.0f);
		litMat->SetTextureParameter("diffuseTexture", basicTexture);

		auto litMat2 = litMat->CreateInstance("BaseMaterialInstance");
		litMat2->SetVector4Parameter("tintColor", glm::vec4(1, 1, 1, 1));
		litMat2->SetScalarParameter("roughnessValue", 0.1f);
		litMat2->SetTextureParameter("diffuseTexture", wallTexture);

		//light material
		auto lightMaterial = Tara::Material::Create(Tara::MaterialType::LIGHTING, "assets/materialLightingBRDF.glsl", Tara::Shader::SourceType::TextFiles, "LightingMaterial");
		m_Camera->GetCamera()->SetLightingMaterial(lightMaterial);


		//TesterSingleColor
		
		//auto m_Material = Tara::Material::Create(Tara::MaterialType::LIT, "assets/Material/TesterSingleColor.glsl", Tara::Shader::SourceType::TextFiles, "TesterMaterial");
		//m_Material->SetParameter("metallicValue", 0.5f);
		//m_Material->SetParameter("roughnessValue", 0.5f);

		auto testerMat = Tara::Material::Create(Tara::MaterialType::LIT, "assets/Material/Tester.glsl", Tara::Shader::SourceType::TextFiles, "TesterMaterial");
		testerMat->SetTextureParameter("diffuseTexture", Tara::Texture2D::Create("assets/Material/Tester_Diffuse.png"));
		testerMat->SetTextureParameter("emissiveTexture", Tara::Texture2D::Create("assets/Material/Tester_Emissive.png"));
		testerMat->SetTextureParameter("normalTexture", Tara::Texture2D::Create("assets/Material/Tester_Normal.png"));
		testerMat->SetTextureParameter("roughnessTexture", Tara::Texture2D::Create("assets/Material/Tester_Roughness.png"));
		testerMat->SetTextureParameter("metallicTexture", Tara::Texture2D::Create("assets/Material/Tester_Metallic.png"));
		testerMat->SetTextureParameter("ambientOcclusionTexture", Tara::Texture2D::Create("assets/Material/Tester_AmbientOcclusion.png"));
		m_Material = testerMat;

		auto postProcess1 = Tara::Material::Create(Tara::MaterialType::POSTPROCESS, "assets/postProcess1.glsl", Tara::Shader::SourceType::TextFiles, "PostProcessMaterial1");

		auto cubemaptexture = Tara::TextureCubemap::CreateHDRI("assets/Helipad_GoldenHour/LA_Downtown_Helipad_GoldenHour_8k.jpg", 512);
		auto panoramicMat = Tara::Material::Create(Tara::MaterialType::LIT, "assets/Material/panoramic.glsl", Tara::Shader::SourceType::TextFiles, "PanoramicMateial");
		panoramicMat->SetTextureParameter("cubemap", cubemaptexture);

		m_Camera->GetCamera()->AddPostProcessMaterial(postProcess1);
		
		//scene setup (meshes, lights)

		//make a MeshPart for rendering a cube

		//make a good scene for lighting tests
		Tara::MeshMaker planeMaker(Tara::MeshMaker::Mode::QUADS);
		//{0,1,0,  0, 1, 0, 1,1,1,1, 0,1}, {1,1,0,  0, 1, 0, 1,1,1,1, 1,1}, {1,1,1,  0, 1, 0, 1,1,1,1, 1,0}, {0,1,1,  0, 1, 0, 1,1,1,1, 0,0}, //Top(+Y)
		planeMaker.Vertex({ 0,0,0 }); planeMaker.TextureCoord(glm::vec2(0, 1)); planeMaker.Color({ 1,1,1,1 });
		planeMaker.Vertex({ 1,0,0 }); planeMaker.TextureCoord(glm::vec2(1, 1)); planeMaker.Color({ 1,1,1,1 });
		planeMaker.Vertex({ 1,0,1 }); planeMaker.TextureCoord(glm::vec2(1, 0)); planeMaker.Color({ 1,1,1,1 });
		planeMaker.Vertex({ 0,0,1 }); planeMaker.TextureCoord(glm::vec2(0, 0)); planeMaker.Color({ 1,1,1,1 });
		Tara::MeshPart planePart = planeMaker.GetMeshPart();
		planePart.CalculateNormals();
		planePart.Transform(Tara::Transform({ -16,0,-16 }, { 0,0,0 }, { 32,32,32 }).GetTransformMatrix());
		auto planeMesh = Tara::StaticMesh::Create({ planePart }, "PlaneMesh");
		auto planeObject = Tara::CreateEntity<Tara::StaticMeshEntity>(Tara::EntityNoRef(), weak_from_this(), Tara::Transform({ 0,-8.5,0 }, { 0,0,0 }, { 10,10,10 }), planeMesh, litMat2, "StaticMesh Entity - Plane");


		Tara::MeshPart invertedCube = Tara::MeshPart::UnitCube();
		invertedCube.Transform(Tara::Transform({ -8,-8,-8 }, { 0,0,0 }, { 16,16,16 }).GetTransformMatrix());
		invertedCube.FlipTriangles();
		auto invertedCubeMesh = Tara::StaticMesh::Create({ invertedCube }, "InvertedCubeMesh");
		//auto invertedCubeObject = Tara::CreateEntity<Tara::StaticMeshEntity>(Tara::EntityNoRef(), weak_from_this(), Tara::Transform({ 0,0,0 }, { 0,0,0 }, { 1,1,1 }), invertedCubeMesh, litMat, "Static Mesh Entity");

		Tara::MeshPart cube = Tara::MeshPart::UnitCube();
		auto cubeMesh = Tara::StaticMesh::Create({ cube }, "CubeMesh");
		
		auto sphereMesh = Tara::StaticMesh::Create({ Tara::MeshPart::UnitSphere() }, "SphereMesh");
		auto sphereEntity = Tara::CreateEntity<Tara::StaticMeshEntity>(Tara::EntityNoRef(), weak_from_this(), Tara::Transform({-4, 10, -4}, {0,0,0}, {5,5,5}), sphereMesh, panoramicMat, "panoramic Test Entity");
		
		/*
		std::random_device rd;
		for (int i = 0; i < 16; i++) {
			Tara::CreateEntity<Tara::StaticMeshEntity>(Tara::EntityNoRef(), weak_from_this(), Tara::Transform(
				{
					(float)(rd() % 16) - 8,
					(float)(rd() % 16) - 8,
					(float)(rd() % 16) - 8
				}, {
					(float)(rd() % 360) - 180,
					(float)(rd() % 180) - 90,
					(float)(rd() % 360) - 180
				}, { 1,1,1 }
			), cubeMesh, litMat, "StaticMeshEntity1");
		}
		*/

		Tara::PointLightEntity::SetEditorLogo(Tara::Texture2D::Create("assets/PointLightLogo.png"));
		Tara::AmbientLightEntity::SetEditorLogo(Tara::Texture2D::Create("assets/AmbientLightLogo.png"));
		Tara::DirectionalLightEntity::SetEditorLogo(Tara::Texture2D::Create("assets/DirectionallightLogo.png"));
		Tara::SpotLightEntity::SetEditorLogo(Tara::Texture2D::Create("assets/SpotLightLogo.png"));

		//auto light1 = Tara::CreateEntity<Tara::PointLightEntity>(Tara::EntityNoRef(), weak_from_this(), Tara::Transform({ 0,0,0 }, { 0,0,0 }, { 1,1,1 }), Tara::Vector(1, 1, 1), 1, "LightEntity");
		auto light2 = Tara::CreateEntity<Tara::PointLightEntity>(Tara::EntityNoRef(), weak_from_this(), Tara::Transform({ 6,-2,-6 }, { 0,0,0 }, { 1,1,1 }), Tara::Vector(1, 1, 1), 1, "LightEntity");
		auto light3 = Tara::CreateEntity<Tara::PointLightEntity>(Tara::EntityNoRef(), weak_from_this(), Tara::Transform({ -1,1,1 }, { 0,0,0 }, { 1,1,1 }), Tara::Vector(1, 1, 1), 1, "LightEntity");
		
		m_Light = light2;
		auto light4 = Tara::CreateEntity<Tara::AmbientLightEntity>(
			Tara::EntityNoRef(), weak_from_this(), Tara::Transform({ 0,1,0 }, { 0,0,0 }, { 1,1,1 }),
			Tara::Vector(1, 1, 1), 1, "AmbientLightEntity"
			);
		auto light5 = Tara::CreateEntity<Tara::DirectionalLightEntity>(
			Tara::EntityNoRef(), weak_from_this(), Tara::Transform({ 0,7,0 }, { 0,-36,45 }, { 1,1,1 }),
			Tara::Vector(1, 1, 1), 1, "DirectionalLightEntity"
			);
		m_DirectionalLight = light5;

		auto light6 = Tara::CreateEntity<Tara::SpotLightEntity>(
			Tara::EntityNoRef(), weak_from_this(), Tara::Transform({ 0,6,0 }, { 0,-90,0 }, { 1,1,1 }),
			Tara::Vector(0, 0.5, 1), 4, 35, "SpotLightEntity"
			);
		light6->SetSpotlightInnerAngle(25);

		/*
		auto light7 = Tara::CreateEntity<Tara::SpotLightEntity>(
			Tara::EntityNoRef(), weak_from_this(), Tara::Transform({ 4,6,0 }, { 0,-45,90 }, { 1,1,1 }),
			Tara::Vector(0, 1, 0.5), 4, 35, "SpotLightEntity"
			);
		light7->SetSpotlightInnerAngle(25);
		*/

		//m_SpotLight = light6;

		//light1->SetDrawingEditorLogo(true);
		light2->SetDrawingEditorLogo(true);
		light3->SetDrawingEditorLogo(true);
		light4->SetDrawingEditorLogo(true);
		light5->SetDrawingEditorLogo(true);
		light6->SetDrawingEditorLogo(true);
		//light7->SetDrawingEditorLogo(true);


		//m_Light = light1;


		//create a unit sphere
		//auto sphereMesh = Tara::StaticMesh::Create({ Tara::MeshPart::UnitSphere() }, "SphereMesh");
		
		auto CenterObject = Tara::CreateEntity<Tara::StaticMeshEntity>(
			Tara::EntityNoRef(), weak_from_this(),
			Tara::Transform({ 0,-6,0 }, { 0,0,0 }, { 5,5,5 }),
			cubeMesh, m_Material, "Static Mesh Entity"
		);



		//auto sprite = Tara::Sprite::Create(Tara::Texture2D::Create("assets/Widget_Base.png"), 1, 1, "WigetSprite");
		//auto spriteEntity = Tara::CreateEntity<Tara::SpriteEntity>(Tara::EntityNoRef(), weak_from_this(), TRANSFORM_DEFAULT, "sprite", sprite);

		//auto centerCube = Tara::CreateEntity<Tara::StaticMeshEntity>(Tara::EntityNoRef(), weak_from_this(), Tara::Transform({0,0,0}, {0,0,0}, { 0.5,0.5,0.5 }), cubeMesh, m_Material, "StaticMeshEntity1");

		/*
		Tara::CreateComponent<Tara::LambdaComponent>(spriteEntity, LAMBDA_BEGIN_PLAY_DEFAULT,
			[this](Tara::LambdaComponent* self, float deltaTime) {
				Tara::Vector forward = (self->GetParent().lock()->GetWorldPosition() - this->m_Camera->GetWorldPosition()).Normalize();
				LOG_S(INFO) << "Camera Rot: " << this->m_Camera->GetWorldRotation();
				LOG_S(INFO) << "Forward: " << forward << "  Rotation: " << Tara::Rotator::FromForwardVector(forward);
				self->GetParent().lock()->SetWorldRotation(Tara::Rotator::FromForwardVector(forward));
			},
			LAMBDA_EVENT_DEFAULT);
		*/


		//UI Stuff (on a different Layer made here!)
		/*
		{
			Tara::LayerRef UILayer = std::make_shared<Tara::Layer>();
			Tara::Application::Get()->GetScene()->PushOverlay(UILayer);
			auto layerCamera = Tara::CreateEntity<Tara::CameraEntity>(Tara::EntityNoRef(), UILayer, Tara::Camera::ProjectionType::Screen, TRANSFORM_DEFAULT);
			UILayer->SetLayerCamera(layerCamera);

			auto font = Tara::Font::Create("assets/LiberationSans-Regular.ttf", 1024, 96, "arial");

			auto patch = Tara::Patch::Create(Tara::Texture2D::Create("assets/Widget_Base.png"), "PatchWidgetBase");
			patch->SetBorderPixels(2, 2, 2, 2);

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

			auto base = Tara::CreateEntity<Tara::UIBaseEntity>(Tara::EntityNoRef(), UILayer, "UIBaseEntity");
			base->SetBorder(0, 0, 0, 0);

			auto frame = Tara::CreateEntity<Tara::UIFrameEntity>(base, PARENT_LAYER, patchFrame, 28.0f, "Basic Frame");
			frame->SetBorder(frame->GetBorder() + 5.0f);


			auto list = Tara::CreateEntity<Tara::UIListEntity>(frame, PARENT_LAYER, "UIListEntity");
			list->SetSnapRules(Tara::UISnapRule::CENTER_HORIZONTAL | Tara::UISnapRule::CENTER_VERTICAL);
			list->SetSpacing(5, 5);

			{
				auto text = Tara::CreateEntity<Tara::UITextEntity>(list, PARENT_LAYER, font, "Text Entity");
				text->SetSnapRules(Tara::UISnapRule::CENTER_HORIZONTAL | Tara::UISnapRule::CENTER_VERTICAL);
				text->SetText("Material Roughness: ?");
				text->SetTextSize(32);

				Tara::CreateComponent<Tara::LambdaComponent>(text, LAMBDA_BEGIN_PLAY_DEFAULT,
					[m_Material](Tara::LambdaComponent* self, float deltaTime) {
						std::stringstream ss;
						auto param = m_Material->GetParameterValue("roughnessValue");
						ss << "Material Roughness: " << std::get<float>(param.second);
						std::dynamic_pointer_cast<Tara::UITextEntity>(self->GetParent().lock())->SetText(ss.str());
					},
					LAMBDA_EVENT_DEFAULT);

				auto hlist = Tara::CreateEntity<Tara::UIListEntity>(list, PARENT_LAYER, "UIListEntity");
				hlist->SetSnapRules(Tara::UISnapRule::CENTER_HORIZONTAL | Tara::UISnapRule::CENTER_VERTICAL);
				hlist->SetDirecton(Tara::UIListEntity::Direction::Horizontal);
				hlist->SetSpacing(5, 5);
			
			
				auto buttonLess = Tara::CreateEntity<Tara::UIButtonEntity>(hlist, PARENT_LAYER, patchButtonNormal, patchButtonHover, patchButtonClicked, patchButtonDisabled, "baseButton");
				buttonLess->SetSnapRules(Tara::UISnapRule::TOP | Tara::UISnapRule::LEFT);
				buttonLess->SetBorderFromPatch();
				buttonLess->SetTint({ 1, 1, 1, 1 });
				Tara::CreateComponent<Tara::LambdaComponent>(buttonLess, LAMBDA_BEGIN_PLAY_DEFAULT, LAMBDA_UPDATE_DEFAULT,
					[m_Material](Tara::LambdaComponent* self, Tara::Event& e) {
						Tara::EventFilter filter(e);
						filter.Call<Tara::UIToggleEvent>([m_Material](Tara::UIToggleEvent& ee) {
							auto param = m_Material->GetParameterValue("roughnessValue");
							float value = std::get<float>(param.second);
							value -= 0.05;
							value = std::clamp<float>(value, 0.0f, 1.0f);
							m_Material->SetParameter("roughnessValue", value);
							return true;
							});
					});

				auto textLess = Tara::CreateEntity<Tara::UITextEntity>(buttonLess, PARENT_LAYER, font, "Text Entity");
				textLess->SetSnapRules(Tara::UISnapRule::CENTER_HORIZONTAL | Tara::UISnapRule::CENTER_VERTICAL);
				textLess->SetText("<<");
				textLess->SetTextSize(32);

				auto buttonMore = Tara::CreateEntity<Tara::UIButtonEntity>(hlist, PARENT_LAYER, patchButtonNormal, patchButtonHover, patchButtonClicked, patchButtonDisabled, "baseButton");
				buttonMore->SetSnapRules(Tara::UISnapRule::TOP | Tara::UISnapRule::LEFT);
				buttonMore->SetBorderFromPatch();
				buttonMore->SetTint({ 1, 1, 1, 1 });
				Tara::CreateComponent<Tara::LambdaComponent>(buttonMore, LAMBDA_BEGIN_PLAY_DEFAULT, LAMBDA_UPDATE_DEFAULT,
					[m_Material](Tara::LambdaComponent* self, Tara::Event& e) {
						Tara::EventFilter filter(e);
						filter.Call<Tara::UIToggleEvent>([m_Material](Tara::UIToggleEvent& ee) {
							auto param = m_Material->GetParameterValue("roughnessValue");
							float value = std::get<float>(param.second);
							value += 0.05;
							value = std::clamp<float>(value, 0.0f, 1.0f);
							m_Material->SetParameter("roughnessValue", value);
							return true;
							});
					});

				auto textMore = Tara::CreateEntity<Tara::UITextEntity>(buttonMore, PARENT_LAYER, font, "Text Entity");
				textMore->SetSnapRules(Tara::UISnapRule::CENTER_HORIZONTAL | Tara::UISnapRule::CENTER_VERTICAL);
				textMore->SetText(">>");
				textMore->SetTextSize(32);
			}


			{
				auto text = Tara::CreateEntity<Tara::UITextEntity>(list, PARENT_LAYER, font, "Text Entity");
				text->SetSnapRules(Tara::UISnapRule::CENTER_HORIZONTAL | Tara::UISnapRule::CENTER_VERTICAL);
				text->SetText("Material Metallic: ?");
				text->SetTextSize(32);

				Tara::CreateComponent<Tara::LambdaComponent>(text, LAMBDA_BEGIN_PLAY_DEFAULT,
					[m_Material](Tara::LambdaComponent* self, float deltaTime) {
						std::stringstream ss;
						auto param = m_Material->GetParameterValue("metallicValue");
						ss << "Material Metallic: " << std::get<float>(param.second);
						std::dynamic_pointer_cast<Tara::UITextEntity>(self->GetParent().lock())->SetText(ss.str());
					},
					LAMBDA_EVENT_DEFAULT);

				auto hlist = Tara::CreateEntity<Tara::UIListEntity>(list, PARENT_LAYER, "UIListEntity");
				hlist->SetSnapRules(Tara::UISnapRule::CENTER_HORIZONTAL | Tara::UISnapRule::CENTER_VERTICAL);
				hlist->SetDirecton(Tara::UIListEntity::Direction::Horizontal);
				hlist->SetSpacing(5, 5);

				auto buttonLess = Tara::CreateEntity<Tara::UIButtonEntity>(hlist, PARENT_LAYER, patchButtonNormal, patchButtonHover, patchButtonClicked, patchButtonDisabled, "baseButton");
				buttonLess->SetSnapRules(Tara::UISnapRule::TOP | Tara::UISnapRule::LEFT);
				buttonLess->SetBorderFromPatch();
				buttonLess->SetTint({ 1, 1, 1, 1 });
				Tara::CreateComponent<Tara::LambdaComponent>(buttonLess, LAMBDA_BEGIN_PLAY_DEFAULT, LAMBDA_UPDATE_DEFAULT,
					[m_Material](Tara::LambdaComponent* self, Tara::Event& e) {
						Tara::EventFilter filter(e);
						filter.Call<Tara::UIToggleEvent>([m_Material](Tara::UIToggleEvent& ee) {
							auto param = m_Material->GetParameterValue("metallicValue");
							float value = std::get<float>(param.second);
							value -= 0.05;
							value = std::clamp<float>(value, 0.0f, 1.0f);
							m_Material->SetParameter("metallicValue", value);
							return true;
							});
					});

				auto textLess = Tara::CreateEntity<Tara::UITextEntity>(buttonLess, PARENT_LAYER, font, "Text Entity");
				textLess->SetSnapRules(Tara::UISnapRule::CENTER_HORIZONTAL | Tara::UISnapRule::CENTER_VERTICAL);
				textLess->SetText("<<");
				textLess->SetTextSize(32);

				auto buttonMore = Tara::CreateEntity<Tara::UIButtonEntity>(hlist, PARENT_LAYER, patchButtonNormal, patchButtonHover, patchButtonClicked, patchButtonDisabled, "baseButton");
				buttonMore->SetSnapRules(Tara::UISnapRule::TOP | Tara::UISnapRule::LEFT);
				buttonMore->SetBorderFromPatch();
				buttonMore->SetTint({ 1, 1, 1, 1 });
				Tara::CreateComponent<Tara::LambdaComponent>(buttonMore, LAMBDA_BEGIN_PLAY_DEFAULT, LAMBDA_UPDATE_DEFAULT,
					[m_Material](Tara::LambdaComponent* self, Tara::Event& e) {
						Tara::EventFilter filter(e);
						filter.Call<Tara::UIToggleEvent>([m_Material](Tara::UIToggleEvent& ee) {
							auto param = m_Material->GetParameterValue("metallicValue");
							float value = std::get<float>(param.second);
							value += 0.05;
							value = std::clamp<float>(value, 0.0f, 1.0f);
							m_Material->SetParameter("metallicValue", value);
							return true;
							});
					});

				auto textMore = Tara::CreateEntity<Tara::UITextEntity>(buttonMore, PARENT_LAYER, font, "Text Entity");
				textMore->SetSnapRules(Tara::UISnapRule::CENTER_HORIZONTAL | Tara::UISnapRule::CENTER_VERTICAL);
				textMore->SetText(">>");
				textMore->SetTextSize(32);
			}
		}
		*/

		//Comment
		/*
		Tara::MeshMaker mm(Tara::MeshMaker::Mode::QUADS);
		mm.SetCombineRule(Tara::MeshMaker::CombineRules::IF_SECOND_DEFAULT);

		const int sides = 16;
		float colorR = 0;
		for (int i = 0; i < sides; i++) {
			Tara::Vector v = Tara::Rotator(0, 0, (360.0f / sides) * (i % sides)).RotateVector({ 0, 0, 1 });
			Tara::Vector v2 = Tara::Rotator(0, 0, (360.0f / sides) * ((i+1) % sides)).RotateVector({ 0, 0, 1 });
			mm.Vertex(v);
			mm.Vertex(v + Tara::Vector{0, 1, 0});
			mm.Vertex(v2 + Tara::Vector{ 0, 1, 0 });	mm.Color(1, 1 - colorR, 1 - colorR, 1);
			mm.Vertex(v2);								mm.Color(1, 1 - colorR, 1 - colorR, 1);
			colorR += (1.0f / sides);
		}

		mm.SetCombineRule(Tara::MeshMaker::CombineRules::NEVER); //mark these edges sharp
		mm.SetMode(Tara::MeshMaker::Mode::TRIANGLE_FAN);
		mm.Vertex(0, 0, 0);
		for (int i = 0; i <= sides; i++) {
			Tara::Vector v = Tara::Rotator(0, 0, (360.0f / sides) * (i % sides)).RotateVector({ 0, 0, 1 });
			mm.Vertex(v);
			//mm.Normal(0, -1, 0);
		}

		mm.SetMode(Tara::MeshMaker::Mode::TRIANGLE_FAN);
		mm.Vertex(0, 1, 0);
		for (int i = sides; i >= 0; i--) {
			Tara::Vector v = Tara::Rotator(0, 0, (360.0f / sides) * (i % sides)).RotateVector({ 0, 0, 1 });
			mm.Vertex(v + Tara::Vector{ 0, 1, 0 });
			//mm.Normal(0, 1, 0);
		}

		//mm.Vertex(0, 0, 0); mm.TextureCoord(0, 0);
		//mm.Vertex(0, 0, 0); mm.TextureCoord(0, 0);
		//mm.Vertex(0, 0, 0); mm.TextureCoord(0, 0);
		//mm.Vertex(0, 0, 0); mm.TextureCoord(0, 0);


		Tara::MeshPart part = mm.GetMeshPart();


		//Tara::MeshPart part = Tara::MeshPart::UnitCube();

		part.CalculateNormals();

		part.Transform(Tara::Transform({ 0 }, { 0,0,0 }, { 1,2,1 }).GetTransformMatrix());
		//part.FlipWindings();
		//part.FlipNormals();

		m_Mesh = Tara::StaticMesh::Create({ part }, "CubeMesh");


		auto sme = Tara::CreateEntity<Tara::StaticMeshEntity>(Tara::EntityNoRef(), weak_from_this(), Tara::Transform({0,0,0}, {0,0,0}, {1,1,1}), m_Mesh, m_Material, "Static Mesh Entity");
		*/
	}



}

void ModelBuildLayer::Deactivate()
{
	
}

void ModelBuildLayer::Draw(float deltaTime)
{
	/*
	//Tara::RenderCommand::EnableBackfaceCulling(true);
	//LOG_S(INFO) << "Target Size" << m_SpotLight->GetDepthTarget()->GetWidth() << ", " << m_SpotLight->GetDepthTarget()->GetHeight();
	Tara::Renderer::BeginModelDepthScene(m_SpotLight);
	uint32_t cameraBits = m_Camera->GetCamera()->GetRenderFilterBits();
	for (auto entity : GetEntityList()) {
		if (entity) {
			entity->Draw(deltaTime, cameraBits);
		}
	}
	Tara::Renderer::EndScene();
	*/

	Layer::Draw(deltaTime);
	

	//some fake UI
	/*
	Tara::Renderer::BeginScene(m_ScreenCamera->GetCamera());
	Tara::RenderCommand::EnableDepthTesting(false);
	Tara::RenderCommand::EnableBackfaceCulling(false);
	auto& window = Tara::Application::Get()->GetWindow();
	glm::vec2 size = glm::vec2((float)window->GetWidth(), (float)window->GetHeight());
	Tara::Renderer::Quad(
		TRANSFORM_2D(size.x - 200, 200, 0, 150, -150),
		{ 1,1,1,1 }, std::dynamic_pointer_cast<Tara::RenderTarget2D>(m_DirectionalLight->GetDepthTarget())
	);

	Tara::Renderer::EndScene();
	*/
}

void ModelBuildLayer::OnEvent(Tara::Event& e)
{
	Layer::OnEvent(e);
	Tara::EventFilter filter(e);
	filter.Call<Tara::KeyPressEvent>([this](Tara::KeyPressEvent& ee) {
		if (ee.GetKey() == TARA_KEY_R) {
			LOG_S(INFO) << "Camera Transform" << m_Camera->GetWorldTransform();
		}
		if (ee.GetKey() == TARA_KEY_T) {
			//m_Camera->SetWorldPosition(m_Light->GetWorldPosition());
		}
		return false;
	});
}

