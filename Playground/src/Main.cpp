/*
A Tara application, for testing purposes.
*/

#include <random>

#include <Tara.h>

/*
OpenGL include - BAD - used for developing new features ONLY
*/
#include "glad/glad.h"

//testing types
#include "TColorRectEntity.h"
#include "TOrthoCameraControllerComponent.h"
#include "BatchTestLayer.h"
#define SPRITE_MAX 100




/// <summary>
/// Testing Layer
/// </summary>
class TestingLayer : public Tara::Layer {
public:
	/// <summary>
	/// Constructor
	/// </summary>
	TestingLayer() 
		: m_Player(nullptr) 
	{}
	
	~TestingLayer() {
		Deactivate();
	}

	/// <summary>
	/// Activattion function override
	/// </summary>
	virtual void Activate() override {
		LOG_S(INFO) << "Testing Layer Activated!";

		//Tara::Texture::SetDefaultTextureFiltering(Tara::Texture::Filtering::Linear);
		Tara::Texture::SetDefaultTextureFiltering(Tara::Texture::Filtering::Nearest);

		//make a texture asset
		auto textureUVChecker = Tara::Texture2D::Create("assets/UV_Checker.png");
		auto textureDirArrows = Tara::Texture2D::Create("assets/DirArrows.png");
		textureDirArrows->SetFiltering(Tara::Texture::Filtering::Linear);

		auto textureParticle4x1 = Tara::Texture2D::Create("assets/Particle_4x1.png");
		//auto textureParticle2x2 = Tara::Texture2D::Create("assets/Particle_2x2.png");
		//auto textureParticle1x4 = Tara::Texture2D::Create("assets/Particle_1x4.png");

		//the number here is width in world units the screen will be wide. Has no effect on window size.
		//to make in 1:1 with pixels, set it to the width of the window.
		
		m_Camera = Tara::CameraEntity::Create(Tara::EntityNoRef(), weak_from_this(), Tara::Camera::ProjectionType::Ortographic, TRANSFORM_DEFAULT, "camera");
		m_Camera->SetOrthographicExtent(8.0f);
		std::shared_ptr<Tara::OrthographicCamera> camera = std::dynamic_pointer_cast<Tara::OrthographicCamera>(m_Camera->GetCamera());
		//std::shared_ptr<Tara::OrthographicCamera> camera = std::make_shared<Tara::OrthographicCamera>(8.0f);
		//m_Camera = camera;

		//new TColorRectEntity(Tara::EntityNoRef(), weak_from_this(), { {0.5,0,0}, {0,0,0}, {1,1,1} });

		if (weak_from_this().lock() == nullptr) {
			LOG_S(ERROR) << "weak_from_this is null!";
		}

		
		auto dmce = Tara::DynamicMultiChildEntity::Create(Tara::EntityNoRef(), weak_from_this());

		auto extent = camera->GetExtent();
		std::random_device rd;
		float delta = (extent.Left - extent.Right)/(float)SPRITE_MAX, xpos = extent.Right;

		auto sprite = Tara::Sprite::Create(textureDirArrows, 1, 1, "ParticleSprite");
		//sprite->CreateAnimationSequence("default", 0, 3, 1.0f);
		
		auto spriteEntity = Tara::SpriteEntity::Create(dmce, weak_from_this(), TRANSFORM_DEFAULT, "sprite", sprite);
		//spriteEntity->PlayAnimation("default");
		spriteEntity->SetFlip(SPRITE_FLIP_H | SPRITE_FLIP_V);

		
		m_Player = TColorRectEntity::Create(Tara::EntityNoRef(), weak_from_this(), TRANSFORM_DEFAULT, "player");
		m_Player->SetColor({ 0.0f, 1.0f, 0.0f, 0.25f });
		
		
		
		//auto player = TControlableEntity::Create(Tara::EntityNoRef(), weak_from_this(), TRANSFORM_DEFAULT, "player");
		//player->SetColor({ 0.0f, 1.0f, 0.0f, 0.25f });
		//player->SetSpeed(1.5f);
		//m_Player = player;

		//attach camera to player
		m_Player->AddChild(m_Camera);

		//Controller Component
		auto controller = TOrthoCameraControllerComponent::Create(spriteEntity, "PlayerController");
		controller->SetSpeed(1.5f);
		

		//Some simple drawing texts
		std::unordered_map <Tara:: Shader::TargetStage, std::string> map;
		map[Tara::Shader::TargetStage::Vertex] = "assets/basic.vertex.glsl";
		map[Tara::Shader::TargetStage::Pixel] = "assets/basic.fragment.glsl";
		m_SimpleShader = Tara::Shader::Create("simpleShader", Tara::Shader::SourceType::TextFiles, map);

		m_VertexArray = Tara::VertexArray::Create();

		float verts[] = {
			1.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f
		};
		auto vBuf = Tara::VertexBuffer::Create(verts, 9);
		vBuf->SetLayout({
			{Tara::Shader::Datatype::Float3, "a_Position", false}
		});
		m_VertexArray->AddVertexBuffer(vBuf);

		uint32_t indecies[] = {
			0, 1, 2
		};
		auto iBuf = Tara::IndexBuffer::Create(indecies, 3);
		m_VertexArray->SetIndexBuffer(iBuf);
	}
	
	virtual void Deactivate() override {
		LOG_S(INFO) << "Testing Layer Deactivated!";
	}

	virtual void Update(float deltaTime) override {

		//call super update function
		Tara::Layer::Update(deltaTime);

	}
	
	virtual void Draw(float deltaTime) override{
		Tara::Renderer::BeginScene(m_Camera->GetCamera());
		
		Tara::Layer::Draw(deltaTime);

		//Tara::RenderCommand::PushDrawType(Tara::RenderDrawType::LineStrip, false);
		//Tara::Renderer::Draw(m_VertexArray, m_SimpleShader, TRANSFORM_2D(-1, -1, 0, 1, 1));
		//Tara::RenderCommand::PopDrawType();

		Tara::Renderer::EndScene();
	}
	
	virtual void OnEvent(Tara::Event& e) override {
		//LOG_S(INFO) << "TestingLayer::OnEvent!";
		Layer::OnEvent(e); //parent call


		Tara::EventFilter filter(e);
		filter.Call<Tara::MouseButtonPressEvent>(TARA_BIND_FN(TestingLayer::OnMousePressedEvent));
		filter.Call<Tara::KeyPressEvent>(TARA_BIND_FN(TestingLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Tara::KeyPressEvent& e) {
		//LOG_S(INFO) << "Key pressed called!";
		//Tara::After(std::bind(&TestingLayer::test, this, m_Player), 1.0f);
		return false;
	}

	void test(std::shared_ptr<TColorRectEntity> player) {
		player->SetColor({ 0.0f, 0.0f, 1.0f, 1.0f });
		LOG_S(INFO) << "After function executed";
	}

	bool OnMousePressedEvent(Tara::MouseButtonPressEvent& e) {
		if (e.getButton() == TARA_MOUSE_BUTTON_3) {
			for (auto entity : GetEntityList()) {
				LOG_S(INFO) << entity->GetName() << "{" << (entity->GetParent().lock() == nullptr) << "}";
				entity->DebugLogAllChildren(true, 1);
			}
		}

		return false;
	}

private:
	//TESTING STUFF
	//Tara::Texture2DRef m_Texture;
	//Tara::Texture2DRef m_Texture2;
	
	Tara::ShaderRef m_SimpleShader;
	Tara::VertexArrayRef m_VertexArray;
	
	//Tara::CameraRef m_Camera;
	
	float m_PlayerSpeed = 1.0f;

	Tara::CameraEntityRef m_Camera;
	std::shared_ptr<TColorRectEntity> m_Player;
	float m_TimeCounter = 0;

	Tara::ShaderRef m_GShader;
	Tara::VertexArrayRef m_QuadPoints;
};


int main(int argc, char** argv) {
	Tara::Application::Get()->Init(1200, 700, "Tara Playground Application!");
	//add layers to scene...
	//Tara::Application::Get()->GetScene()->PushLayer(std::make_shared<BatchTestLayer>());
	Tara::Application::Get()->GetScene()->PushLayer(std::make_shared<TestingLayer>());
	//run
	Tara::Application::Get()->Run();
	return 0;
}