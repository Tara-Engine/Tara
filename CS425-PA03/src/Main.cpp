
#include <Tara.h>
#include "PlayerEntity.h"
#include "RoomManager.h"

//initial screen size
const static int WIDTH = 1200;
const static int HEIGHT = 700;



//The layer that everything is living in. Layers are seperate, ie, there is no collision or overlaps between them. So, everything lives on one.
class GameLayer : public Tara::Layer {
public:
	//we don't do anything here because the OpenGL context is not guaranteed to be made yet
	GameLayer()
	{}

	~GameLayer() {
		Deactivate();
	}

	//this function is called when the layer is loaded to the screen. OpenGL context is guaranteed to be made, and engine systems initialized
	virtual void Activate() override {
		//set BG color
		Tara::RenderCommand::SetClearColor({0.0f,0.0f,0.0f});

		//very important to initialize first!
		RoomManager::Get()->Init(Tara::EntityNoRef(), weak_from_this());
		RoomManager::Get()->LoadRoomTextures();
		RoomManager::Get()->Generate(time(0), 5, 5, 30);

		//create the player
		//texture and sprite first
		auto playerTexture = Tara::Texture2D::Create("assets/Character_8x4.png", "playerTexture");
		auto playerSprite = Tara::Sprite::Create(playerTexture, 8, 4, "playerSprite");
		//load animations
		playerSprite->CreateAnimationSequence("idle_down",  1,  3,  2.0f);
		playerSprite->CreateAnimationSequence("walk_down",  4,  7,  5.0f);
		playerSprite->CreateAnimationSequence("idle_right", 8,  11, 2.0f);
		playerSprite->CreateAnimationSequence("walk_right", 12, 15, 5.0f);
		playerSprite->CreateAnimationSequence("idle_left",  16, 19, 2.0f);
		playerSprite->CreateAnimationSequence("walk_left",  20, 23, 5.0f);
		playerSprite->CreateAnimationSequence("idle_up",    24, 27, 2.0f);
		playerSprite->CreateAnimationSequence("walk_up",    28, 31, 5.0f);
		//make the player entity
		auto player = PlayerEntity::Create(Tara::EntityNoRef(), weak_from_this(), TRANSFORM_2D(0,0,0,16*4,16*4), "player", playerSprite);
		
		//make our camera
		auto cameraEntity = Tara::CameraEntity::Create(
			player, weak_from_this(), 
			Tara::Camera::ProjectionType::Ortographic, 
			TRANSFORM_2D(0.5f,0.5f,0,1,1),
			"camera"
		);
		cameraEntity->SetOrthographicExtent((float)WIDTH); 
		cameraEntity->SetUseWorldScale(false); //ignore parent scale for view matrix.
		m_Camera = std::dynamic_pointer_cast<Tara::OrthographicCamera>(cameraEntity->GetCamera());
		player->SetUpdateChildrenFirst(false);
		//player->AddChild(cameraEntity);
		
		
	}

	virtual void Deactivate() override {
		
	}

	virtual void Update(float deltaTime) override {
		Tara::Layer::Update(deltaTime);
	}

	virtual void Draw(float deltaTime) override {
		//begin a scene using our camera, call the parent draw function (which draws the entities) and end the scene
		Tara::Renderer::BeginScene(m_Camera);
		Tara::Layer::Draw(deltaTime);
		Tara::Renderer::EndScene();
	}

	//called when an event happens. Has no need to send events to entities in the layer, that happens automatically.
	virtual void OnEvent(Tara::Event& e) override {
		Layer::OnEvent(e); //super call to layer
		Tara::EventFilter filter(e); //create a filter for the type of event
		filter.Call<Tara::WindowResizeEvent>(TARA_BIND_FN(GameLayer::OnWindowResizeEvent));
	}

	//called by OnEvent if the event is a WindowResizeEvent
	bool OnWindowResizeEvent(Tara::WindowResizeEvent& e) {
		m_Camera->SetExtent((float)e.getWidth()); //tell the camera we resized, so it can compensate.
		return false;
	}

private:
	std::shared_ptr<Tara::OrthographicCamera> m_Camera;
	
};


int main(int argc, char** argv) {
	//Initialize the application
	Tara::Application::Get()->Init(WIDTH, HEIGHT, "CS425 PA03");
	//add layers to scene
	Tara::Application::Get()->GetScene()->PushLayer(std::make_shared<GameLayer>());
	//run
	Tara::Application::Get()->Run();
	return 0;
}