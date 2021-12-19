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
	std::unordered_map<Tara::Shader::TargetStage, std::string> shaderSources;
	shaderSources[Tara::Shader::TargetStage::Vertex] = "assets/basic.vertex.glsl";
	shaderSources[Tara::Shader::TargetStage::Pixel] = "assets/basic.fragment.glsl";
	m_Shader = Tara::Shader::Create("BasicShader", Tara::Shader::SourceType::TextFiles, shaderSources);


	//make a MeshPart for rendering a cube
	
	/*
	*/

	Tara::MeshMaker mm(Tara::MeshMaker::Mode::TRIANGLE_STRIP);
	
	const int sides = 16;
	for (int i = 0; i <= sides; i++) {
		Tara::Vector v = Tara::Rotator(0, 0, (360.0f / sides) * (i % sides)).RotateVector({ 0, 0, 1 });
		mm.Vertex(v);
		mm.Vertex(v + Tara::Vector{0, 1, 0});
	}
	
	//mm.Vertex(0, 0, 0); mm.TextureCoord(0, 0);
	//mm.Vertex(0, 0, 0); mm.TextureCoord(0, 0);
	//mm.Vertex(0, 0, 0); mm.TextureCoord(0, 0);
	//mm.Vertex(0, 0, 0); mm.TextureCoord(0, 0);

	
	
	Tara::MeshPart part = mm.GetMeshPart();
	//Tara::MeshPart part = Tara::MeshPart::UnitCube();
	
	part.CalculateNormals();
	//part.FlipWindings();
	//part.FlipNormals();

	m_Mesh = Tara::StaticMesh::Create({ part }, "CubeMesh");
}

void ModelBuildLayer::Deactivate()
{
}

void ModelBuildLayer::Draw(float deltaTime)
{
	//endable depth testing
	Tara::RenderCommand::EnableDepthTesting(true);
	//Tara::RenderCommand::EnableBackfaceCulling(true);

	Tara::Renderer::BeginScene(m_Camera->GetCamera());

	auto& vas = m_Mesh->GetVertexArrays();
	for (auto& va : vas) {
		Tara::Renderer::Draw(va, m_Shader, TRANSFORM_DEFAULT);
	}

	Tara::Renderer::EndScene();
	//
	Layer::Draw(deltaTime);
}

