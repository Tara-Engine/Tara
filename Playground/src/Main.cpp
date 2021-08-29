/*
A Tara application, for testing purposes.


*/
#include <Tara.h>

class TestingLayer : public Tara::Layer {
public:
	TestingLayer() {}
	~TestingLayer() {
		Deactivate();
	}

	virtual void Activate() {
		LOG_S(INFO) << "Testing Layer Activated!";

		m_VertexArray = Tara::VertexArray::Create();
		m_VertexArray->Bind();

		//vertex buffer
		float vertecies[3 * (3 + 4)] = {
			-0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.0f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		};
		Tara::VertexBufferRef VBRef = Tara::VertexBuffer::Create(vertecies, sizeof(vertecies) / sizeof(float));

		VBRef->SetLayout({
			{Tara::Shader::Datatype::Float3, "position"},
			{Tara::Shader::Datatype::Float4, "color"}
			});

		m_VertexArray->AddVertexBuffer(VBRef);


		//Index Buffer
		uint32_t indecies[3] = {
			0, 1, 2
		};
		Tara::IndexBufferRef IBRef = Tara::IndexBuffer::Create(indecies, sizeof(indecies) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(IBRef);


		m_Shader = Tara::Shader::Create(
			Tara::Shader::SourceType::TextFiles,
			"assets/basic.vertex.glsl",
			"assets/basic.fragment.glsl"
		);
		m_Shader->Bind();

	}
	
	virtual void Deactivate() {
		LOG_S(INFO) << "Testing Layer Deactivated!";
	}

	virtual void Update() {}
	
	virtual void Draw() {
		m_VertexArray->Bind();
		m_Shader->Bind();
		Tara::RenderCommand::Draw(m_VertexArray);
	}
	
	
	virtual void OnEvent(Tara::Event& e) {}

private:
	//TESTING STUFF
	Tara::VertexArrayRef m_VertexArray;
	Tara::ShaderRef m_Shader;
};


int main(int argc, char** argv) {
	Tara::Application::Get()->Init(1200, 700, "Tara Playground Application!");
	//add layers to scene...
	Tara::Application::Get()->GetScene()->PushLayer(new TestingLayer());

	Tara::Application::Get()->Run();
	return 0;
}