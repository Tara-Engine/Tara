/*
A Tara application, for testing purposes.


*/
#include <Tara.h>

//testing types
#include "TColorRectEntity.h"

/// <summary>
/// Testing Layer
/// </summary>
class TestingLayer : public Tara::Layer {
public:
	/// <summary>
	/// Constructor
	/// </summary>
	TestingLayer() {}
	
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
		m_Camera = std::make_shared<Tara::OrthographicCamera>(8.0f);

		//new TColorRectEntity(Tara::EntityNoRef(), weak_from_this(), { {0.5,0,0}, {0,0,0}, {1,1,1} });

		if (weak_from_this().lock() == nullptr) {
			LOG_S(ERROR) << "weak_from_this is null!";
		}

		auto entity = TColorRectEntity::Create(Tara::EntityNoRef(), weak_from_this(), Tara::Transform{ {0.5,0.5,0}, {0,0,0}, {1,1,1}});
		entity->SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });
	}
	
	virtual void Deactivate() override {
		LOG_S(INFO) << "Testing Layer Deactivated!";
	}

	virtual void Update(float deltaTime) override{
		
		//call super update function
		Tara::Layer::Update(deltaTime);

		//move camera
		Tara::Vector offset = { 0.0f,0.0f,0.0f };
		if (Tara::Input::Get()->IsKeyPressed(TARA_KEY_S)) {
			offset.y -= 1;
		}
		if (Tara::Input::Get()->IsKeyPressed(TARA_KEY_W)) {
			offset.y += 1;
		}
		if (Tara::Input::Get()->IsKeyPressed(TARA_KEY_A)) {
			offset.x -= 1;
		}
		if (Tara::Input::Get()->IsKeyPressed(TARA_KEY_D)) {
			offset.x += 1;
		}

		m_Camera->SetPosition(m_Camera->GetPosition() + (offset * (m_PlayerSpeed * deltaTime)));
	}
	
	virtual void Draw(float deltaTime) override{
		Tara::Renderer::BeginScene(m_Camera);
		
		/*anatomy of a transform:
		* { { pos as vec3}, {euler angle rot as rotator}, {scale as vec3} }
		*/

		//draw a textured quad
		Tara::Renderer::Quad(m_Texture, { {-0.5f,-0.5f,0.0f}, {0,0,0}, {1,1,1} });

		//call super draw func
		Tara::Layer::Draw(deltaTime);

		Tara::Renderer::EndScene();
	}
	
	virtual void OnEvent(Tara::Event& e) override {}

private:
	//TESTING STUFF
	Tara::Texture2DRef m_Texture;
	Tara::CameraRef m_Camera;
	float m_PlayerSpeed = 1.0f;
};


int main(int argc, char** argv) {
	Tara::Application::Get()->Init(1200, 700, "Tara Playground Application!");
	//add layers to scene...
	Tara::Application::Get()->GetScene()->PushLayer(std::make_shared<TestingLayer>());
	//run
	Tara::Application::Get()->Run();
	return 0;
}