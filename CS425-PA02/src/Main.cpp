
#include <Tara.h>
#include "PlayerEntity.h"
#include "ParticleEntity.h"
#include "WallEntity.h"

static int WIDTH = 1200;
static int HEIGHT = 700;

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

		auto player = Tara::PlayerEntity::Create(Tara::EntityNoRef(), weak_from_this(), { {0,0,0},{0,0,0}, {100,100,100} }, "player", wallTexture);
		player->ListenForEvents(true);

		auto particle = ParticleEntity::Create(Tara::EntityNoRef(), weak_from_this(), { {0,0,0},{0,0,0}, {50,50,50} }, "particle", particleTexture);
	}

	virtual void Deactivate() override {
		
	}

	virtual void Update(float deltaTime) override {

	}

	virtual void Draw(float deltaTime) override {
		Tara::Renderer::BeginScene(m_Camera);

		Tara::Layer::Draw(deltaTime);

		Tara::Renderer::EndScene();
	}

	virtual void OnEvent(Tara::Event& e) override {
		
	}

	

private:
	Tara::CameraRef m_Camera;
	
};


int main(int argc, char** argv) {
	Tara::Application::Get()->Init(WIDTH, HEIGHT, "CS425 PA02");
	//add layers to scene...
	Tara::Application::Get()->GetScene()->PushLayer(std::make_shared<GameLayer>());
	//run
	Tara::Application::Get()->Run();
	return 0;
}