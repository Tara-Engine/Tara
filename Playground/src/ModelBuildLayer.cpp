#include "ModelBuildLayer.h"
#include "EditorCameraControllerComponent.h"



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
	m_Material = Tara::Material::Create(Tara::MaterialType::LIT, "assets/material2deferred.glsl", Tara::Shader::SourceType::TextFiles, "BasicMaterial");
	m_Material->SetVector4Paramater("tintColor", glm::vec4(1, 1, 1, 1));

	//light material
	const static std::string lightingMaterialSource = R"V0G0N(
	vec4 color(){
		return vec4(Diffuse, 1);
	}
	)V0G0N";
	auto lightMaterial = Tara::Material::Create(Tara::MaterialType::LIGHTING, "assets/materialLightingCamera.glsl", Tara::Shader::SourceType::TextFiles, "LightingMaterial");
	Tara::RenderCommand::SetLightingMaterial(lightMaterial);
	//make a MeshPart for rendering a cube
	
	/*
	*/

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
		if (ee.GetKey() == TARA_KEY_G){
			this->m_Material->SetVector4Paramater("tintColor", glm::vec4(1, 1, 0, 1));
		}
		return false;
	});
}

