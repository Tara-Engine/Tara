
#include <Tara.h>
#include "PlayerEntity.h"
#include "ParticleEntity.h"
#include "WallEntity.h"

//initial screen size
const static int WIDTH = 1200;
const static int HEIGHT = 700;

//size of the particles
const static int PARTICLE_SIZE = 25;

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
		Tara::RenderCommand::SetClearColor({0.0f,0.845f,1.0f});

		//make our camera
		m_Camera = std::make_shared<Tara::OrthographicCamera>((float)WIDTH);

		//load textures
		Tara::Texture2DRef wallTexture = Tara::Texture2D::Create("assets/wall.png");
		Tara::Texture2DRef particleTexture = Tara::Texture2D::Create("assets/Particle.png");
		Tara::Texture2DRef particleGlowTexture = Tara::Texture2D::Create("assets/Particle_glowing.png");
		Tara::Texture2DRef characterTexture = Tara::Texture2D::Create("assets/Character.png");
	
		//create the walls, based off of seeded noise generation
		Tara::Noise wallNoise = Tara::Noise((uint32_t)(time(0)*-1));
		int wallsSpawned = 0;
		while (wallsSpawned < 3) {
			int x = -40 + (rand() % 80);
			int y = -40 + (rand() % 80);
			float val = wallNoise(x, y);
			if ((val + 1) * 50 > rand() % 75) {
				Tara::WallEntity::Create(Tara::EntityNoRef(), weak_from_this(), { {x*8,y*8,0},{0,0,0}, {100,100,1} }, "wall", wallTexture);
				wallsSpawned++;
			}
		}

		//create the player
		auto player = Tara::PlayerEntity::Create(Tara::EntityNoRef(), weak_from_this(), { {0,0,0},{0,0,0}, {100,100,1} }, "player", characterTexture);

		//create an entity to hold the particles.
		//this entity holds all children in an AABB tree, for optimal collisions
		auto dmce = Tara::DynamicMultiChildEntity::Create(Tara::EntityNoRef(), weak_from_this());
		
		//create noise for the particles to spawn according to, like the walls
		Tara::Noise particleNoise = Tara::Noise(time(0));
		srand(time(0));
		int particlesSpawned = 0;
		while (particlesSpawned < 100) {
			int x = -300 + (rand() % 600);
			int y = -300 + (rand() % 600);
			float val = particleNoise(x, y);
			if ((val + 1) * 50 > rand() % 75) { // values above +0.5 are guaranteed spawns
				auto particle = ParticleEntity::Create(dmce/*parent*/, weak_from_this(), {{x,y,0},{0,0,0}, {PARTICLE_SIZE,PARTICLE_SIZE,1}}, "particle", particleTexture);
				particle->SetGlowTextureName(particleGlowTexture->GetAssetName()); //set the asset name for the texture used when overlapping player
				particlesSpawned++;
			}
		}

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
	Tara::Application::Get()->Init(WIDTH, HEIGHT, "CS425 PA02");
	//add layers to scene
	Tara::Application::Get()->GetScene()->PushLayer(std::make_shared<GameLayer>());
	//run
	Tara::Application::Get()->Run();
	return 0;
}