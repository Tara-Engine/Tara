
#include <random>
#include <Tara.h>
#include "PawnEntity.h"
#include "RoomManager.h"
#include "PlayerControllerComponent.h"
#include "EnemyControllerComponent.h"

//initial screen size
const static int WIDTH = 1200;
const static int HEIGHT = 700;

const static int32_t MAP_SIZE = 5;
//copied from RoomManager.cpp
static const uint32_t ROOMSIZE_PX = 144; //pixel size of original room images
static const float ROOM_SCALE = ROOMSIZE_PX * 4.0f;

//world regen func
void Regenerate(Tara::LayerNoRef layer);

//The layer that everything is living in. Layers are seperate, ie, there is no collision or overlaps between them. So, everything lives on one.
class GameLayer : public Tara::Layer {
public:
	//we don't do anything here because the OpenGL context is not guaranteed to be made yet
	GameLayer()
	{}

	~GameLayer() {
		Deactivate();
		LOG_S(INFO)<< "Layer destructor called.";
	}

	//this function is called when the layer is loaded to the screen. OpenGL context is guaranteed to be made, and engine systems initialized
	virtual void Activate() override {
		SCOPE_PROFILE("LayerLoading");
		//set BG color
		Tara::RenderCommand::SetClearColor({0.0f,0.0f,0.0f});
		//set default texture filtering mode
		Tara::Texture::SetDefaultTextureFiltering(Tara::Texture::Filtering::Nearest);

		//very important to initialize first!
		RoomManager::Get()->Init(Tara::EntityNoRef(), weak_from_this());
		RoomManager::Get()->LoadRoomTextures();
		auto path = RoomManager::Get()->Generate(time(0), MAP_SIZE, MAP_SIZE, 30);
		
		//textures and sprites
		m_DeathTexture = Tara::Texture2D::Create("assets/DeathLogo.png", "deathLogo");
		auto playerSprite = Tara::AssetLibrary::Get()->GetAssetIf<Tara::Sprite>("playerSprite");
		if (!playerSprite){ //only do this if the sprite does not exist in the AssetLibrary (ie, this only happens once)
			auto playerTexture = Tara::Texture2D::Create("assets/Character_8x4.png", "playerTexture");
			playerSprite = Tara::Sprite::Create(playerTexture, 8, 4, "playerSprite");
			//load animations
			playerSprite->CreateAnimationSequence("idle_down",  0,  3,  2.0f);
			playerSprite->CreateAnimationSequence("walk_down",  4,  7,  5.0f);
			playerSprite->CreateAnimationSequence("idle_right", 8,  11, 2.0f);
			playerSprite->CreateAnimationSequence("walk_right", 12, 15, 5.0f);
			playerSprite->CreateAnimationSequence("idle_left",  16, 19, 2.0f);
			playerSprite->CreateAnimationSequence("walk_left",  20, 23, 5.0f);
			playerSprite->CreateAnimationSequence("idle_up",    24, 27, 2.0f);
			playerSprite->CreateAnimationSequence("walk_up",    28, 31, 5.0f);
		}
		auto enemySprite = Tara::AssetLibrary::Get()->GetAssetIf<Tara::Sprite>("enemySprite");
		if (!enemySprite) {
			auto enemyTexture = Tara::Texture2D::Create("assets/Particle_10x5.png", "enemyTexture");
			enemySprite = Tara::Sprite::Create(enemyTexture, 10, 5, "enemySprite");
			//load animations
			enemySprite->CreateAnimationSequence("walk_down",    0, 9, 15.0f);
			enemySprite->CreateAnimationSequence("walk_right", 10, 19, 15.0f);
			enemySprite->CreateAnimationSequence("walk_left",  20, 29, 15.0f);
			enemySprite->CreateAnimationSequence("walk_up",  30, 39, 15.0f);
			enemySprite->CreateAnimationSequence("idle_down",  40, 43, 15.0f);
			enemySprite->CreateAnimationSequence("idle_right",  40, 43, 15.0f);
			enemySprite->CreateAnimationSequence("idle_left",  40, 43, 15.0f);
			enemySprite->CreateAnimationSequence("idle_up",  40, 43, 15.0f);
		}


		//make the player entity
		int32_t originY = (*path.begin()) % MAP_SIZE;
		int32_t originX = (*path.begin() - originY) / MAP_SIZE;

		glm::vec2 playerPos = RoomManager::RoomCoordToWorldCoord({ originX, -originY });
		m_Player = Tara::CreateEntity<PawnEntity>(Tara::EntityNoRef(), weak_from_this(), TRANSFORM_2D(playerPos.x, playerPos.y,0,16*4,16*4), "player", playerSprite);
		
		//make our camera
		auto cameraEntity = Tara::CreateEntity<Tara::CameraEntity>(
			m_Player, weak_from_this(),
			Tara::Camera::ProjectionType::Ortographic, 
			TRANSFORM_2D(0.5f,0.5f,0,1,1),
			"camera"
		);
		cameraEntity->SetOrthographicExtent((float)Tara::Application::Get()->GetWindow()->GetWidth()); 
		cameraEntity->SetUseWorldScale(false); //ignore parent scale for view matrix.
		m_Camera = std::dynamic_pointer_cast<Tara::OrthographicCamera>(cameraEntity->GetCamera());
		m_Player->SetUpdateChildrenFirst(false);
		//m_Player->AddChild(cameraEntity);
		
		//Make a copy of this for the AIControllerComponent. But only leave 1 uncommented
		Tara::CreateComponent<PlayerControllerComponent>(m_Player, "playerController");
		//m_Player->SetImmortal(true);//test immortality
		

		//Enemy
		auto rd = std::random_device();
		while (!m_Enemy) {
			int rx = rd() % MAP_SIZE;
			int ry = -(rd() % MAP_SIZE);
			auto room = RoomManager::Get()->GetRoom(rx, ry);
			if (room && !Tara::PowOfTwo(room->GetDoorState())) { //the room is real and not a dead end
				glm::vec2 pos = RoomManager::RoomCoordToWorldCoord({ rx, ry });
				m_Enemy = Tara::CreateEntity<PawnEntity>(Tara::EntityNoRef(), weak_from_this(), TRANSFORM_2D(pos.x, pos.y, 0, 16 * 4, 16 * 4), "player", enemySprite);
				m_Enemy->PlayAnimation("all");
				m_Enemy->SetImmortal(true);
				Tara::CreateComponent<EnemyControllerComponent>(m_Enemy, "enemyController");
			}
		}
	}

	virtual void Deactivate() override {
		
	}

	virtual void Update(float deltaTime) override {
		Tara::Layer::Update(deltaTime);
		//deal with player and enemy colliding
		//here, for simplicity. They already both have renferences in this context.
		if (m_Player->GetAlive() && glm::distance(m_Player->GetWorldPosition(), m_Enemy->GetWorldPosition()) < ROOM_SCALE / 9) {
			m_Player->Kill();
		}
	}

	virtual void Draw(float deltaTime) override {
		//begin a scene using our camera, call the parent draw function (which draws the entities) and end the scene
		Tara::Renderer::BeginScene(m_Camera);
		Tara::Layer::Draw(deltaTime);
		//draw the death texture if the player is dead.
		if (!m_Player->GetAlive()) {
			auto t = m_Player->GetWorldTransform();
			t.Scale = { 1000, 454 ,1 };
			t.Position -= t.Scale / 2.0f;
			Tara::Renderer::Quad(t, { 1,1,1,1 }, m_DeathTexture);
		}
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
	std::shared_ptr <PawnEntity> m_Player;
	std::shared_ptr <PawnEntity> m_Enemy;
	Tara::Texture2DRef m_DeathTexture;
};

void Regenerate(Tara::LayerNoRef layer) {
	LOG_S(INFO) << "World Rengeneration!";
	RoomManager::Get()->ClearAll();
	if (layer.lock()) {
		Tara::Application::Get()->GetScene()->RemoveLayer(layer.lock());
	}
	Tara::Application::Get()->GetScene()->PushLayer(std::make_shared<GameLayer>());
}

int main(int argc, char** argv) {
	//Initialize the application
	Tara::Application::Get()->Init(WIDTH, HEIGHT, "CS425 PA03");
	//add layers to scene
	Tara::Application::Get()->GetScene()->PushLayer(std::make_shared<GameLayer>());
	//run
	Tara::Application::Get()->Run();
	return 0;
}