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

	auto basicTexture = Tara::Texture2D::Create("assets/Widget_Base.png");
	auto wallTexture = Tara::Texture2D::Create("assets/wall.png");
	//make a basic shader
	/*
	std::unordered_map<Tara::Shader::TargetStage, std::string> shaderSources;
	shaderSources[Tara::Shader::TargetStage::Vertex] = "assets/basic.vertex.glsl";
	shaderSources[Tara::Shader::TargetStage::Pixel] = "assets/basic.fragment.glsl";
	m_Shader = Tara::Shader::Create("BasicShader", Tara::Shader::SourceType::TextFiles, shaderSources);
	*/

	/*
	const static std::string materialSource = R"V0G0N(
	uniform vec4 tintColor;
	vec4 diffuse(){
		return v_Color * tintColor;
	}
	)V0G0N";
	*/
	auto unlitMat = Tara::Material::Create(Tara::MaterialType::UNLIT, "assets/material1.glsl", Tara::Shader::SourceType::TextFiles, "UnlitMaterial");
	unlitMat->SetVector4Paramater("tintColor", glm::vec4(0, 0, 1, 1));

	m_Material = Tara::Material::Create(Tara::MaterialType::LIT, "assets/material3deferred.glsl", Tara::Shader::SourceType::TextFiles, "BasicMaterial");
	m_Material->SetVector4Paramater("tintColor", glm::vec4(1, 1, 1, 1));
	m_Material->SetScalarParamater("roughnessValue", 1.0f);
	m_Material->SetTextureParmamter("diffuseTexture", basicTexture);

	auto litMat2 = m_Material->CreateInstance("BaseMaterialInstance");
	litMat2->SetVector4Paramater("tintColor", glm::vec4(0, 1, 1, 1));
	litMat2->SetScalarParamater("roughnessValue", 0.1f);
	litMat2->SetTextureParmamter("diffuseTexture", wallTexture);

	//light material
	auto lightMaterial = Tara::Material::Create(Tara::MaterialType::LIGHTING, "assets/materialLightingPhong.glsl", Tara::Shader::SourceType::TextFiles, "LightingMaterial");
	m_Camera->GetCamera()->SetLightingMaterial(lightMaterial);
	
	
	//make a MeshPart for rendering a cube
	
	//make a good scene for lighting tests
	Tara::MeshMaker planeMaker(Tara::MeshMaker::Mode::QUADS);
	//{0,1,0,  0, 1, 0, 1,1,1,1, 0,1}, {1,1,0,  0, 1, 0, 1,1,1,1, 1,1}, {1,1,1,  0, 1, 0, 1,1,1,1, 1,0}, {0,1,1,  0, 1, 0, 1,1,1,1, 0,0}, //Top(+Y)
	planeMaker.Vertex({ 0,0,0 }); planeMaker.TextureCoord(glm::vec2(0,1)); planeMaker.Color({1,1,1,1});
	planeMaker.Vertex({ 1,0,0 }); planeMaker.TextureCoord(glm::vec2(1,1)); planeMaker.Color({1,1,1,1});
	planeMaker.Vertex({ 1,0,1 }); planeMaker.TextureCoord(glm::vec2(1,0)); planeMaker.Color({1,1,1,1});
	planeMaker.Vertex({ 0,0,1 }); planeMaker.TextureCoord(glm::vec2(0,0)); planeMaker.Color({1,1,1,1});
	Tara::MeshPart planePart = planeMaker.GetMeshPart();
	planePart.CalculateNormals();
	planePart.Transform(Tara::Transform({-16,0,-16}, {0,0,0}, {32,32,32}).GetTransformMatrix());
	auto planeMesh = Tara::StaticMesh::Create({ planePart }, "PlaneMesh");
	auto planeObject = Tara::CreateEntity<Tara::StaticMeshEntity>(Tara::EntityNoRef(), weak_from_this(), Tara::Transform({ 0,-8.5,0 }, { 0,0,0 }, { 10,10,10 }), planeMesh, litMat2, "StaticMesh Entity - Plane");
	
	
	Tara::MeshPart invertedCube = Tara::MeshPart::UnitCube();
	invertedCube.Transform(Tara::Transform({-8,-8,-8}, {0,0,0}, {16,16,16}).GetTransformMatrix());
	invertedCube.FlipTriangles();
	auto invertedCubeMesh = Tara::StaticMesh::Create({ invertedCube }, "InvertedCubeMesh");
	auto invertedCubeObject = Tara::CreateEntity<Tara::StaticMeshEntity>(Tara::EntityNoRef(), weak_from_this(), Tara::Transform({ 0,0,0 }, { 0,0,0 }, { 1,1,1 }), invertedCubeMesh, m_Material, "Static Mesh Entity");

	Tara::MeshPart cube = Tara::MeshPart::UnitCube();
	auto cubeMesh = Tara::StaticMesh::Create({ cube }, "CubeMesh");
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
			}, {1,1,1}
		), cubeMesh, m_Material, "StaticMeshEntity1");
	}

	Tara::PointLightEntity::SetEditorLogo(Tara::Texture2D::Create("assets/PointLightLogo.png"));

	auto light1 = Tara::CreateEntity<Tara::PointLightEntity>(Tara::EntityNoRef(), weak_from_this(), Tara::Transform({   0,0,0 }, { 0,0,0 }, { 1,1,1 }), Tara::Vector(1, 1, 1), 1, "LightEntity");
	auto light2 = Tara::CreateEntity<Tara::PointLightEntity>(Tara::EntityNoRef(), weak_from_this(), Tara::Transform({  12,0,0 }, { 0,0,0 }, { 1,1,1 }), Tara::Vector(1, 1, 1), 1, "LightEntity");
	auto light3 = Tara::CreateEntity<Tara::PointLightEntity>(Tara::EntityNoRef(), weak_from_this(), Tara::Transform({ -12,0,0 }, { 0,0,0 }, { 1,1,1 }), Tara::Vector(1, 1, 1), 1, "LightEntity");

	light1->SetDrawingEditorLogo(true);
	light2->SetDrawingEditorLogo(true);
	light3->SetDrawingEditorLogo(true);

	m_Light = light1;


	//create a unit sphere
	auto sphereMesh = Tara::StaticMesh::Create({Tara::MeshPart::UnitSphere()}, "SphereMesh");
	auto sphereObject = Tara::CreateEntity<Tara::StaticMeshEntity>(
		Tara::EntityNoRef(), weak_from_this(), 
		Tara::Transform({ 0,-2,0 }, { 0,0,0 }, { 1,1,1 }), 
		sphereMesh, m_Material, "Static Mesh Entity"
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
	
	auto text = Tara::CreateEntity<Tara::UITextEntity>(list, PARENT_LAYER, font, "Text Entity");
	text->SetSnapRules(Tara::UISnapRule::CENTER_HORIZONTAL | Tara::UISnapRule::CENTER_VERTICAL);
	text->SetText("Light Strength: ?");
	text->SetTextSize(32);

	Tara::CreateComponent<Tara::LambdaComponent>(text, LAMBDA_BEGIN_PLAY_DEFAULT,
		[light1](Tara::LambdaComponent* self, float deltaTime) {
			std::stringstream ss;
			ss << "Light Strength: " << light1->GetLightIntensity();
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
	Tara::CreateComponent<Tara::LambdaComponent>(buttonLess, LAMBDA_BEGIN_PLAY_DEFAULT,LAMBDA_UPDATE_DEFAULT,
		[light1, light2, light3](Tara::LambdaComponent* self, Tara::Event& e) {
			Tara::EventFilter filter(e);
			filter.Call<Tara::UIToggleEvent>([light1, light2, light3](Tara::UIToggleEvent& ee) {
					float intensity = light1->GetLightIntensity();
					if (intensity <= 1) {
						intensity /= 2;
					}
					else {
						intensity -= 1;
					}
					light1->SetLightIntensity(intensity);
					light2->SetLightIntensity(intensity);
					light3->SetLightIntensity(intensity);
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
		[light1, light2, light3](Tara::LambdaComponent* self, Tara::Event& e) {
			Tara::EventFilter filter(e);
			filter.Call<Tara::UIToggleEvent>([light1, light2, light3](Tara::UIToggleEvent& ee) {
					float intensity = light1->GetLightIntensity();
					if (intensity <= 1) {
						intensity *= 2;
					}
					else {
						intensity += 1;
					}
					light1->SetLightIntensity(intensity);
					light2->SetLightIntensity(intensity);
					light3->SetLightIntensity(intensity);
					return true;
				});
		});

	auto textMore = Tara::CreateEntity<Tara::UITextEntity>(buttonMore, PARENT_LAYER, font, "Text Entity");
	textMore->SetSnapRules(Tara::UISnapRule::CENTER_HORIZONTAL | Tara::UISnapRule::CENTER_VERTICAL);
	textMore->SetText(">>");
	textMore->SetTextSize(32);

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

void ModelBuildLayer::Deactivate()
{
	
}

void ModelBuildLayer::Draw(float deltaTime)
{
	//endable depth testing
	Tara::RenderCommand::EnableDepthTesting(true);
	Tara::RenderCommand::EnableBackfaceCulling(true);

	/*
	Tara::Renderer::BeginScene(m_Camera->GetCamera());

	auto& vas = m_Mesh->GetVertexArrays();
	for (auto& va : vas) {
		Tara::Renderer::Draw(va, m_Shader, TRANSFORM_DEFAULT);
	}

	Tara::Renderer::EndScene();
	*/

	//
	Layer::Draw(deltaTime);
}

void ModelBuildLayer::OnEvent(Tara::Event& e)
{
	Layer::OnEvent(e);

	Tara::EventFilter filter(e);
	filter.Call<Tara::KeyPressEvent>([this](Tara::KeyPressEvent& ee) {
		if (ee.GetKey() == TARA_KEY_T){
			m_Light->SetLightIntensity(m_Light->GetLightIntensity() + 1);
			LOG_S(INFO) << "Light Level: " << m_Light->GetLightIntensity();
		}
		if (ee.GetKey() == TARA_KEY_G) {
			m_Light->SetLightIntensity(m_Light->GetLightIntensity() - 1);
			LOG_S(INFO) << "Light Level: " << m_Light->GetLightIntensity();
		}
		return false;
	});
}

