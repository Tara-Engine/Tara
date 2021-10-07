
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

		//make our camera
		auto cameraEntity = Tara::CameraEntity::Create(Tara::EntityNoRef(), weak_from_this(), Tara::Camera::ProjectionType::Ortographic, TRANSFORM_DEFAULT, "camera");
		cameraEntity->SetOrthographicExtent((float)WIDTH / 100.0f); // the /100 is used to counteract the scale of the player
		//cameraEntity->SetOrthographicExtent(8.0f);
		m_Camera = std::dynamic_pointer_cast<Tara::OrthographicCamera>(cameraEntity->GetCamera());
		//m_Camera = std::make_shared<Tara::OrthographicCamera>((float)WIDTH);

		//very important to initialize first!
		RoomManager::Get()->Init(Tara::EntityNoRef(), weak_from_this());
		RoomManager::Get()->LoadRoomTextures();

		RoomManager::Get()->AddRoom(0, 0,  DOORSTATE_DOWN | DOORSTATE_RIGHT, 1);
		RoomManager::Get()->AddRoom(0, -1, DOORSTATE_UP   | DOORSTATE_RIGHT, 2);
		RoomManager::Get()->AddRoom(1, 0,  DOORSTATE_DOWN | DOORSTATE_LEFT , 3);
		RoomManager::Get()->AddRoom(1, -1, DOORSTATE_UP   | DOORSTATE_LEFT , 4);

		//create the player
		auto player = Tara::PlayerEntity::Create(Tara::EntityNoRef(), weak_from_this(), { {0,0,0},{0,0,0}, {100,100,1} }, "player", nullptr);
		player->AddChild(cameraEntity);
		
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