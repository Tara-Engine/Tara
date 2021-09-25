
#include <Tara.h>

class GameLayer : public Tara::Layer {
public:
	GameLayer()
	{}

	~GameLayer() {
		Deactivate();
	}

	virtual void Activate() override {
		m_Camera = std::make_shared<Tara::OrthographicCamera>(8.0f);
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
	Tara::Application::Get()->Init(1200, 700, "CS425 PA02");
	//add layers to scene...
	Tara::Application::Get()->GetScene()->PushLayer(std::make_shared<GameLayer>());
	//run
	Tara::Application::Get()->Run();
	return 0;
}