#include "BatchTestLayer.h"
#include <glad/glad.h>



BatchTestLayer::BatchTestLayer()
	 /*: m_Player(nullptr)*/
{}

void BatchTestLayer::Activate()
{
	LOG_S(INFO) << "Testing Layer Activated!";

	//make a texture asset
	m_Texture = Tara::Texture2D::Create("assets/UV_Checker.png");
	m_Texture2 = Tara::Texture2D::Create("assets/wall.png");
	m_Texture3 = Tara::Texture2D::Create("assets/Particle.png");

	m_Camera = Tara::CreateEntity<Tara::CameraEntity>(Tara::EntityNoRef(), weak_from_this(), Tara::Camera::ProjectionType::Ortographic, TRANSFORM_DEFAULT, "camera");
	m_Camera->SetOrthographicExtent(8.0f);
	std::shared_ptr<Tara::OrthographicCamera> camera = std::dynamic_pointer_cast<Tara::OrthographicCamera>(m_Camera->GetCamera());

	//m_Player = TControlableEntity::Create(Tara::EntityNoRef(), weak_from_this(), TRANSFORM_DEFAULT, "player");
	//m_Player->SetColor({ 0.0f, 1.0f, 0.0f, 0.25f });
	//m_Player->SetSpeed(1.5f);

	//attach camera to player
	//m_Player->AddChild(m_Camera);



	//create a special shader
	std::unordered_map < Tara::Shader::TargetStage, std::string> sources;
	sources[Tara::Shader::TargetStage::Vertex] = "assets/quad.vertex.glsl";
	sources[Tara::Shader::TargetStage::Geometry] = "assets/quad.geom.glsl";
	sources[Tara::Shader::TargetStage::Pixel] = "assets/quad.frag.glsl";
	m_GShader = Tara::Shader::Create("geomQuadShader", Tara::Shader::SourceType::TextFiles, sources);

	m_Quads.push_back(QuadData(Tara::Transform( { 0, 0, 0},{0,0,0},{ 1.0f,  1.0f,  1.0f} ), { 0.0f,0.0f }, { 1.0f,1.0f }, { 1,0,0,1 }, -1.0f));
	m_Quads.push_back(QuadData(Tara::Transform( {-2, 0, 0},{0,0,0},{ 0.5f,  0.75f, 1.0f} ), { 0.0f,0.0f }, { 1.0f,1.0f }, { 1,1,1,1 },  0.0f));
	m_Quads.push_back(QuadData(Tara::Transform( { 1,-2, 0},{0,0,0},{ 0.75f, 0.75f, 1.0f} ), { 0.0f,0.0f }, { 1.0f,1.0f }, { 1,1,1,1 },  1.0f));
	m_Quads.push_back(QuadData(Tara::Transform( {-1,-2, 0},{0,0,0},{ 1.0f,  1.0f,  1.0f} ), { 0.0f,0.0f }, { 1.0f,1.0f }, { 1,1,1,1 },  1.0f));
	m_Quads.push_back(QuadData(Tara::Transform( { 2, 1, 0},{0,0,0},{ 1.0f,  1.0f,  1.0f} ), { 0.0f,0.0f }, { 1.0f,1.0f }, { 1,1,1,1 },  2.0f));
	

	//m_Quads.push_back(QuadData({ 0,0 }, { 1,1 }, { 0.0f,0.0f }, { 1.0f,1.0f }, { 1,0,0,1 }, -1.0f));
	//m_Quads.push_back(QuadData({ -2,0 }, { 0.5,0.75 }, { 0.0f,0.0f }, { 1.0f,1.0f }, { 1,1,1,1 }, 0.0f));
	//m_Quads.push_back(QuadData({ 1,-2 }, { 0.75,0.75 }, { 0.0f,0.0f }, { 1.0f,1.0f }, { 1,1,1,1 }, 1.0f));
	//m_Quads.push_back(QuadData({ -1,2 }, { 1.0,1.0 }, { 0.0f,0.0f }, { 1.0f,1.0f }, { 1,1,1,1 }, 1.0f));
	//m_Quads.push_back(QuadData({ 2,1 }, { 1.0,1.0 }, { 0.0f,0.0f }, { 1.0f,1.0f }, { 1,1,1,1 }, 2.0f));

	//create quad points array
	m_QuadPoints = Tara::VertexArray::Create();
	m_QuadPoints->Bind();

	
	int floatInQuadData = sizeof(QuadData) / sizeof(float); //18
	auto quadVertecies = Tara::VertexBuffer::Create((float*)m_Quads.data(), m_Quads.size() * floatInQuadData);

	quadVertecies->Bind();
	quadVertecies->SetLayout({
		{Tara::Shader::Datatype::Float3, "a_Position", false},
		{Tara::Shader::Datatype::Float3, "a_Rotation", false},
		{Tara::Shader::Datatype::Float3, "a_Scale", false},
		{Tara::Shader::Datatype::Float2, "a_UVmin", false},
		{Tara::Shader::Datatype::Float2, "a_UVmax", false},
		{Tara::Shader::Datatype::Float4, "a_Color", false},
		{Tara::Shader::Datatype::Float,  "a_TextureIndex", false}
		});
	m_QuadPoints->AddVertexBuffer(quadVertecies);


}



void BatchTestLayer::Draw(float deltaTime)
{
	Tara::Renderer::BeginScene(m_Camera->GetCamera());

	m_Texture->Bind(0);
	m_Texture2->Bind(1);
	m_Texture3->Bind(2);
	m_GShader->Bind();
	m_GShader->Send("u_MatrixViewProjection", m_Camera->GetCamera()->GetViewProjectionMatrix());
	m_GShader->Send("u_MatrixModel", glm::mat4(1.0f));
	
	//int textures[] = { 0, 1, 2 };
	//m_GShader->Send("u_Textures", textures, 3);
	
	m_GShader->Send("u_Texture0", 0);
	m_GShader->Send("u_Texture1", 1);
	m_GShader->Send("u_Texture2", 2);

	m_QuadPoints->Bind();
	//glPointSize(10);
	glDrawArrays(GL_POINTS, 0, m_Quads.size());


	Tara::Renderer::EndScene();
}

void BatchTestLayer::OnEvent(Tara::Event& e)
{
	Tara::EventFilter filter(e);
	filter.Call<Tara::WindowResizeEvent>(TARA_BIND_FN(BatchTestLayer::OnWindowResizeEvent));
}

bool BatchTestLayer::OnWindowResizeEvent(Tara::WindowResizeEvent& e) {
	m_Camera->SetOrthographicExtent(8.0f);
	return false;
}