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
#include "PawnEntity.h"
#include "TOrthoCameraControllerComponent.h"
//#include "BatchTestLayer.h"
#include "EditorCameraControllerComponent.h"
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


		font = Tara::Font::Create("assets/arial.ttf", 1024, 96, "arial");

		//make a texture asset
		auto textureUVChecker = Tara::Texture2D::Create("assets/UV_Checker.png");
		auto textureDirArrows = Tara::Texture2D::Create("assets/DirArrows.png");
		textureDirArrows->SetFiltering(Tara::Texture::Filtering::Linear);

		auto textureParticle4x1 = Tara::Texture2D::Create("assets/Particle_4x1.png");
		//auto textureParticle2x2 = Tara::Texture2D::Create("assets/Particle_2x2.png");
		//auto textureParticle1x4 = Tara::Texture2D::Create("assets/Particle_1x4.png");

		//the number here is width in world units the screen will be wide. Has no effect on window size.
		//to make in 1:1 with pixels, set it to the width of the window.
		
		//m_Camera = Tara::CameraEntity::Create(Tara::EntityNoRef(), weak_from_this(), Tara::Camera::ProjectionType::Ortographic, TRANSFORM_DEFAULT, "camera");
		m_Camera = Tara::CreateEntity<Tara::CameraEntity>(
			Tara::EntityNoRef(), weak_from_this(), 
			Tara::Camera::ProjectionType::Perspective, 
			Tara::Transform{ {0.0f, 0.0f, 10.0f},{0.0f,0.0f,180.0f},{1.0f} }, 
			"camera"
		);

		m_Camera->SetPerspectiveFOV(45.0f);
		//m_Camera->SetOrthographicExtent(8.0f);
		//std::shared_ptr<Tara::OrthographicCamera> camera = std::dynamic_pointer_cast<Tara::OrthographicCamera>(m_Camera->GetCamera());
		//std::shared_ptr<Tara::OrthographicCamera> camera = std::make_shared<Tara::OrthographicCamera>(8.0f);
		//m_Camera = camera;

		//new TColorRectEntity(Tara::EntityNoRef(), weak_from_this(), { {0.5,0,0}, {0,0,0}, {1,1,1} });

		if (weak_from_this().lock() == nullptr) {
			LOG_S(ERROR) << "weak_from_this is null!";
		}

		
		//auto dmce = Tara::DynamicMultiChildEntity::Create(Tara::EntityNoRef(), weak_from_this());
		auto dmce = Tara::CreateEntity<Tara::DynamicMultiChildEntity>(Tara::EntityNoRef(), weak_from_this());

		//auto extent = camera->GetExtent();
		//std::random_device rd;
		//float delta = (extent.Left - extent.Right)/(float)SPRITE_MAX, xpos = extent.Right;

		auto sprite = Tara::Sprite::Create(textureDirArrows, 1, 1, "ParticleSprite");
		//sprite->CreateAnimationSequence("default", 0, 3, 1.0f);
		
		//m_TempSpriteEntity = Tara::SpriteEntity::Create(dmce, weak_from_this(), TRANSFORM_DEFAULT, "sprite", sprite);
		m_TempSpriteEntity = Tara::CreateEntity<PawnEntity>(dmce, weak_from_this(), TRANSFORM_DEFAULT, "sprite", sprite);
		//spriteEntity->PlayAnimation("default");
		m_TempSpriteEntity->SetFlip(SPRITE_FLIP_H | SPRITE_FLIP_V);

		
		//m_Player = TColorRectEntity::Create(Tara::EntityNoRef(), weak_from_this(), TRANSFORM_DEFAULT, "player");
		m_Player = Tara::CreateEntity<TColorRectEntity>(Tara::EntityNoRef(), weak_from_this(), TRANSFORM_DEFAULT, "player");
		m_Player->SetColor({ 0.0f, 1.0f, 0.0f, 0.25f });
		
		auto textEntity = Tara::CreateEntity<Tara::TextEntity>(Tara::EntityNoRef(), weak_from_this(), font, "Great day, isn't it?", TRANSFORM_2D(-1, 0, 0, 1, 1));
		textEntity->SetColor({ 1,0,0,1 });
		textEntity->SetRelativePosition(Tara::Vector{ -1, 0, -1 });

		//auto player = TControlableEntity::Create(Tara::EntityNoRef(), weak_from_this(), TRANSFORM_DEFAULT, "player");
		//player->SetColor({ 0.0f, 1.0f, 0.0f, 0.25f });
		//player->SetSpeed(1.5f);
		//m_Player = player;

		//attach camera to player
		//m_Player->AddChild(m_Camera);
		m_Camera->AddChild(m_Player);

		//Controller Component
		auto controller = Tara::CreateComponent<EditorCameraControllerComponent>(m_Camera, 1.0f, "CameraController");
		//controller->SetSpeed(1.5f);
		
		
		auto luaComponent1 = Tara::CreateComponent<Tara::ScriptComponent>(m_Player, "assets/Component1.lua", "LuaComponent1");
		auto luaComponent2 = Tara::CreateComponent<Tara::ScriptComponent>(m_TempSpriteEntity, "assets/Component2.lua", "LuaComponent2");
	}
	
	virtual void Deactivate() override {
		LOG_S(INFO) << "Testing Layer Deactivated!";
	}

	virtual void Update(float deltaTime) override {
		//SCOPE_PROFILE("Update");
		//Tara::Timer timer("UpdateTimer"); 
		//call super update function
		Tara::Layer::Update(deltaTime);

		if (Tara::Input::Get()->IsMouseDown(TARA_MOUSE_BUTTON_1)) {
			//test the whole camera ray thing
			auto ray = m_Camera->GetCamera()->GetRayFromScreenCoordinate(Tara::Input::Get()->GetMouseX(), Tara::Input::Get()->GetMouseY());
			//since ortho, only origin is nesecary
			//set the m_TempSpriteEntity location
			m_TempSpriteEntity->SetWorldPosition(ray.first + (ray.second * 5));
		}
	}
	
	virtual void Draw(float deltaTime) override{
		Tara::Renderer::BeginScene(m_Camera->GetCamera());
		
		Tara::Layer::Draw(deltaTime);

		Tara::Renderer::Text(TRANSFORM_2D(1, 1, 0, 1, 1), "Hello World", font);
		//Tara::Renderer::Quad(TRANSFORM_2D(1, 1, 0, 2, 2), { 1,1,1,1 }, font->GetTexture());
		
		
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
		
		//if (m_TempSpriteEntity){
		//	m_TempSpriteEntity->Destroy();
		//	m_TempSpriteEntity.reset();
		//}
		return false;
	}

	void test(std::shared_ptr<TColorRectEntity> player) {
		player->SetColor({ 0.0f, 0.0f, 1.0f, 1.0f });
		LOG_S(INFO) << "After function executed";
	}

	bool OnMousePressedEvent(Tara::MouseButtonPressEvent& e) {
		if (e.GetButton() == TARA_MOUSE_BUTTON_3) {
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
	
	Tara::FontRef font;
	//Tara::CameraRef m_Camera;
	
	float m_PlayerSpeed = 1.0f;

	Tara::CameraEntityRef m_Camera;
	std::shared_ptr<TColorRectEntity> m_Player;
	Tara::SpriteEntityRef m_TempSpriteEntity;
	float m_TimeCounter = 0;

	Tara::ShaderRef m_GShader;
	Tara::VertexArrayRef m_QuadPoints;
};


int main(int argc, char** argv) {
	Tara::Script::Get()->SetDefaultLibraryPath("../Tara/lua");
	Tara::Application::Get()->Init(1200, 700, "Tara Playground Application!");
	//init stuff we have to do
	Tara::Script::RegisterType<PawnEntity>("PawnEntity"); //register PawnEntity

	//add layers to scene...
	//Tara::Application::Get()->GetScene()->PushLayer(std::make_shared<BatchTestLayer>());
	Tara::Application::Get()->GetScene()->PushLayer(std::make_shared<TestingLayer>());
	//run
	Tara::Application::Get()->Run();
	return 0;
}