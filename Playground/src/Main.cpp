/*
A Tara application, for testing purposes.


*/
#include <Tara.h>

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

		m_VertexArray = Tara::VertexArray::Create();
		m_VertexArray->Bind();

		//vertex buffer
		/*
		float vertices[3 * (3 + 4)] = {
			-0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.0f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		};
		*/
		float vertices[4 * (3 + 2)] = {
			-0.5f, -0.5f,  0.0f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.0f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.0f,  0.0f, 1.0f
		};

		Tara::VertexBufferRef vb_ref = Tara::VertexBuffer::Create(vertices, sizeof(vertices) / sizeof(float));

		vb_ref->SetLayout({
			{Tara::Shader::Datatype::Float3, "position"},
			{Tara::Shader::Datatype::Float2, "uv"}
			});

		m_VertexArray->AddVertexBuffer(vb_ref);


		//Index Buffer
		/*
		uint32_t indecies[3] = {
			0, 1, 2
		};
		*/
		uint32_t indices[6] = {
			0, 1, 2, 2, 3, 0
		};
		Tara::IndexBufferRef ib_ref = Tara::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(ib_ref);


		m_Shader = Tara::Shader::Create(
			"s_BasicShader",
			Tara::Shader::SourceType::TextFiles,
			"assets/texture.vertex.glsl",
			"assets/texture.fragment.glsl"
		);
		m_Shader->Bind();

		m_Texture = Tara::Texture2D::Create("assets/UV_Checker.png");

		m_Camera = std::make_shared<Tara::OrthographicCamera>(4.0f);
	}
	
	virtual void Deactivate() {
		LOG_S(INFO) << "Testing Layer Deactivated!";
	}

	virtual void Update() {
		
		//move camera
		glm::vec3 offset = { 0.0f,0.0f,0.0f };
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

		m_Camera->SetPosition(m_Camera->GetPosition() + (offset * m_PlayerSpeed));
	}
	
	virtual void Draw() {
		Tara::Renderer::BeginScene(m_Camera);
		
		m_Shader->Bind();

		m_Shader->Send("u_Texture", 0);
		m_Texture->Bind(0);
		
		Tara::Renderer::Draw(m_VertexArray, m_Shader, { {0,0,0}, {0,0,0}, {1,1,1} });

		Tara::Renderer::EndScene();
	}
	
	
	virtual void OnEvent(Tara::Event& e) {}

private:
	//TESTING STUFF
	Tara::VertexArrayRef m_VertexArray;
	Tara::Texture2DRef m_Texture;
	Tara::ShaderRef m_Shader;
	Tara::CameraRef m_Camera;
	float m_PlayerSpeed = 0.1f;
};


int main(int argc, char** argv) {
	Tara::Application::Get()->Init(1200, 700, "Tara Playground Application!");
	//add layers to scene...
	Tara::Application::Get()->GetScene()->PushLayer(new TestingLayer());
	//run
	Tara::Application::Get()->Run();
	return 0;
}