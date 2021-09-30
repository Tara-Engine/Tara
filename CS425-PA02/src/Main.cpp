
#include <Tara.h>
#include "PlayerEntity.h"
#include "ParticleEntity.h"
#include "WallEntity.h"

static int WIDTH = 1200;
static int HEIGHT = 700;

static int PARTICLE_SIZE = 25;

class GameLayer : public Tara::Layer {
public:
	GameLayer()
	{}

	~GameLayer() {
		Deactivate();
	}

	virtual void Activate() override {
		m_Camera = std::make_shared<Tara::OrthographicCamera>((float)WIDTH);

		Tara::Texture2DRef wallTexture = Tara::Texture2D::Create("assets/wall.png");
		Tara::Texture2DRef particleTexture = Tara::Texture2D::Create("assets/Particle.png");
		Tara::Texture2DRef particleGlowTexture = Tara::Texture2D::Create("assets/Particle_glowing.png");

		auto player = Tara::PlayerEntity::Create(Tara::EntityNoRef(), weak_from_this(), { {0,0,0},{0,0,0}, {100,100,100} }, "player", wallTexture);
		

		auto dmce = Tara::DynamicMultiChildEntity::Create(Tara::EntityNoRef(), weak_from_this());
		Tara::Noise particleNoise = Tara::Noise(time(0));
		srand(time(0));
		int particlesSpawned = 0;
		while (particlesSpawned < 100) {
			int x = -300 + (rand() % 600);
			int y = -300 + (rand() % 600);
			float val = particleNoise(x, y);
			if ((val + 1) * 50 > rand() % 75) { // values above +0.5 are guaranteed spawns
				auto particle = ParticleEntity::Create(dmce, weak_from_this(), { {x,y,0},{0,0,0}, {PARTICLE_SIZE,PARTICLE_SIZE,1} }, "particle", particleTexture);
				particle->SetGlowTextureName(particleGlowTexture->GetAssetName());
				particlesSpawned++;
			}
		}

		Tara::Noise wallNoise = Tara::Noise((uint32_t)(time(0)*-1));
		int wallsSpawned = 0;
		while (wallsSpawned < 3) {
			int x = -40 + (rand() % 80);
			int y = -40 + (rand() % 80);
			float val = wallNoise(x, y);
			if ((val + 1) * 50 > rand() % 75) {
				Tara::WallEntity::Create(Tara::EntityNoRef(), weak_from_this(), { {x*8,y*8,0},{0,0,0}, {100,100,100} }, "wall", wallTexture);
				wallsSpawned++;
			}
		}

		//ParticleEntity::Create(Tara::EntityNoRef(), weak_from_this(), { {50,0,0},{0,0,0}, {50,50,50} }, "particle", particleTexture);
	}

	virtual void Deactivate() override {
		
	}

	virtual void Update(float deltaTime) override {
		Tara::Layer::Update(deltaTime);
	}

	virtual void Draw(float deltaTime) override {
		Tara::Renderer::BeginScene(m_Camera);

		Tara::Layer::Draw(deltaTime);

		Tara::Renderer::EndScene();
	}

	virtual void OnEvent(Tara::Event& e) override {
		Tara::EventFilter filter(e);
		filter.Call<Tara::WindowResizeEvent>(TARA_BIND_FN(GameLayer::OnWindowResizeEvent));
	}

	bool OnWindowResizeEvent(Tara::WindowResizeEvent& e) {
		m_Camera->SetExtent((float)e.getWidth());
		return false;
	}

private:
	std::shared_ptr<Tara::OrthographicCamera> m_Camera;
	
};


int main(int argc, char** argv) {
	Tara::Application::Get()->Init(WIDTH, HEIGHT, "CS425 PA02");
	//add layers to scene...
	Tara::Application::Get()->GetScene()->PushLayer(std::make_shared<GameLayer>());
	//run
	Tara::Application::Get()->Run();
	return 0;
}