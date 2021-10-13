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

		//make a texture asset
		auto textureUVChecker = Tara::Texture2D::Create("assets/UV_Checker.png");
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

		auto sprite = Tara::Sprite::Create(textureParticle4x1, 4, 1, "ParticleSprite");
		sprite->CreateAnimationSequence("default", 0, 3, 1.0f);
		
		for (int i=0;i<SPRITE_MAX;i++){
			Tara::Transform t;
			t.Scale = { 0.2f, 0.2f, 0.2f };
			t.Position = {

				//Interpolation Test
				//xpos,
				//Tara::Lerp<float>(extent.Top, extent.Bottom, (float)i /( (float)SPRITE_MAX-1)),
				
				//Randomness Test
				Tara::MapRange((float)(rd() % 1000),0,1000,extent.Left,extent.Right-0.2),
				Tara::MapRange((float)(rd() % 1000),0,1000,extent.Bottom,extent.Top-0.2),
				
				//that z value
				0
			};
			xpos += delta;
			auto spriteEntity = Tara::SpriteEntity::Create(dmce, weak_from_this(), t, "sprite", sprite);
			spriteEntity->PlayAnimation("default");
			//spriteEntity->SetCurrentFrame(rd() % 4);
			
		}
		

		//Noise test

		//Tara::Noise noise(21, 0.05f, 1.0f, 0.5f, 2);
		/*
		for (int x = 0; x < 20; x++) {
			for (int y = 0; y < 20; y++) {
				if (noise(x, y) > 0.0) {
					Tara::Transform t;
					t.Scale = { 0.2f, 0.2f, 0.2f };
					t.Position = {
						//map x and y
						Tara::MapRange((float)(x),0,19,extent.Left,extent.Right - 0.2),
						Tara::MapRange((float)(y),0,19,extent.Bottom,extent.Top - 0.2),

						//that z value
						0
					};
					auto sprite = Tara::SpriteEntity::Create(dmce, weak_from_this(), t, "sprite");
					sprite->SetTexture(m_Texture);
				}
			}
		}
		*/
		m_Player = TControlableEntity::Create(Tara::EntityNoRef(), weak_from_this(), TRANSFORM_DEFAULT, "player");
		m_Player->SetColor({ 0.0f, 1.0f, 0.0f, 0.25f });
		m_Player->SetSpeed(1.5f);

		//attach camera to player
		m_Player->AddChild(m_Camera);


		
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

		Tara::Renderer::EndScene();
	}
	
	virtual void OnEvent(Tara::Event& e) override {
		//LOG_S(INFO) << "TestingLayer::OnEvent!";
		Tara::EventFilter filter(e);
		filter.Call<Tara::MouseButtonPressEvent>(TARA_BIND_FN(TestingLayer::OnMousePressedEvent));
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
	
	
	//Tara::CameraRef m_Camera;
	
	float m_PlayerSpeed = 1.0f;

	Tara::CameraEntityRef m_Camera;
	std::shared_ptr<TControlableEntity> m_Player;
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