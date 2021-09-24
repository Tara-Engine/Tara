/*
A Tara application, for testing purposes.
*/

#include <random>

#include <Tara.h>

//testing types
#include "TColorRectEntity.h"

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
		m_Texture = Tara::Texture2D::Create("assets/UV_Checker.png");

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
		for (int i=0;i<SPRITE_MAX;i++){
			Tara::Transform t;
			t.Scale = { 0.2f, 0.2f, 0.2f };
			t.Position = {
				Tara::MapRange((float)(rd() % 1000),0,1000,extent.Left,extent.Right-0.2),
				Tara::MapRange((float)(rd() % 1000),0,1000,extent.Bottom,extent.Top-0.2),
				0
			};

			auto sprite = Tara::SpriteEntity::Create(dmce, weak_from_this(), t, "sprite");
			sprite->SetTexture(m_Texture);
		}

		m_Player = TControlableEntity::Create(Tara::EntityNoRef(), weak_from_this(), TRANSFORM_DEFAULT, "player");
		m_Player->SetColor({ 0.0f, 1.0f, 0.0f, 1.0f });
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
		
		/*anatomy of a transform:
		* { { pos as vec3}, {euler angle rot as rotator}, {scale as vec3} }
		*/

		//call super draw func
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
	Tara::Texture2DRef m_Texture;
	
	//Tara::CameraRef m_Camera;
	
	float m_PlayerSpeed = 1.0f;

	Tara::CameraEntityRef m_Camera;
	std::shared_ptr<TControlableEntity> m_Player;
	float m_TimeCounter = 0;
};


int main(int argc, char** argv) {
	Tara::Application::Get()->Init(1200, 700, "Tara Playground Application!");
	//add layers to scene...
	Tara::Application::Get()->GetScene()->PushLayer(std::make_shared<TestingLayer>());
	//run
	Tara::Application::Get()->Run();
	return 0;
}