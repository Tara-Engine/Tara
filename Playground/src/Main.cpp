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

		auto entity = TColorRectEntity::Create(Tara::EntityNoRef(), weak_from_this(), Tara::Transform{ {1,0,0}, {0,0,0}, {1,1,1}}, "YellowSquare");
		entity->SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });
		m_Parent1 = entity;

		entity = TColorRectEntity::Create(Tara::EntityNoRef(), weak_from_this(), Tara::Transform{ {-2,0,0}, {0,0,0}, {1,1,1} }, "RedSquare");
		entity->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
		m_Parent2 = entity;

		entity = TColorRectEntity::Create(m_Parent1, weak_from_this(), Tara::Transform{ {0.5,-0.5,0}, {0,0,0}, {0.5,0.5,1} }, "BlueSquare");
		entity->SetColor({ 0.0f, 0.0f, 1.0f, 1.0f });
		m_Child1 = entity;

		entity = TColorRectEntity::Create(m_Child1, weak_from_this(), Tara::Transform{ {0.5,-0.5,0}, {0,0,0}, {0.5,0.5,1} }, "CyanSquare");
		entity->SetColor({ 0.0f, 1.0f, 1.0f, 1.0f });
		m_Child2 = entity;
	}
	
	virtual void Deactivate() override {
		LOG_S(INFO) << "Testing Layer Deactivated!";
	}

	virtual void Update(float deltaTime) override {

		//call super update function
		Tara::Layer::Update(deltaTime);

		//rotation of parent2
		m_TimeCounter += deltaTime;
		float alpha = (sinf(m_TimeCounter) + 1) / 2;
		auto t = m_Parent2->GetRelativeTransform();
		t.Rotation.Roll = (45.0f * alpha);
		m_Parent2->SetTransform(t);

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

		glm::vec3 val = { 0,0,1 };
		

		m_Camera->SetPosition(m_Camera->GetPosition() + (offset * (m_PlayerSpeed * deltaTime)));
	}
	
	virtual void Draw(float deltaTime) override{
		Tara::Renderer::BeginScene(m_Camera);
		//LOG_S(INFO) << "Scene Begun!";
		
		/*anatomy of a transform:
		* { { pos as vec3}, {euler angle rot as rotator}, {scale as vec3} }
		*/

		//create and draw a debug bounding box
		//Tara::BoundingBox box({ 0, 0, 0 }, { 1, 1, 1 });
		//Tara::Renderer::DrawBoundingBox(box, { 1.0f, 1.0f, 1.0f, 1.0f });

		//draw a textured quad
		Tara::Renderer::Quad(m_Texture, { {-0.5f,-0.5f,0.0f}, {0,0,0}, {1,1,1} });

		//call super draw func
		Tara::Layer::Draw(deltaTime);
		//LOG_S(INFO) << "Scene Ended";
		Tara::Renderer::EndScene();
	}
	
	virtual void OnEvent(Tara::Event& e) override {
		//LOG_S(INFO) << "TestingLayer::OnEvent!";
		Tara::EventFilter filter(e);
		filter.Call<Tara::MouseButtonPressEvent>(TARA_BIND_FN(TestingLayer::OnMousePressedEvent));
	}

	bool OnMousePressedEvent(Tara::MouseButtonPressEvent& e) {
		//
		if (e.getButton() == TARA_MOUSE_BUTTON_1) {
			if (m_Child1->GetParent().lock() == m_Parent1) {
				m_Child1->SwapParent(m_Parent2);
			}
			else {
				m_Child1->SwapParent(m_Parent1);
			}
		}
		else if (e.getButton() == TARA_MOUSE_BUTTON_2) {
			
			//LOG_S(INFO) << "Mouse Button Pressed!";
			auto entity = m_Child1->RemoveChildByName("CyanSquare");
			if (!entity) {
				LOG_S(INFO) << "Child not found";
				m_Child1->AddChild(m_Child2);
			}
			else {
				LOG_S(INFO) << "Child found";
			}
		}
		else if (e.getButton() == TARA_MOUSE_BUTTON_3) {
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
	Tara::CameraRef m_Camera;
	float m_PlayerSpeed = 1.0f;

	Tara::EntityRef m_Parent1;
	Tara::EntityRef m_Parent2;
	Tara::EntityRef m_Child1;
	Tara::EntityRef m_Child2;

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