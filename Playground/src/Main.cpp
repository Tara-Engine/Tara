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
		float vertecies[3 * (3 + 4)] = {
			-0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.0f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		};
		*/
		float vertecies[4 * (3 + 2)] = {
			-0.5f, -0.5f,  0.0f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.0f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.0f,  0.0f, 1.0f
		};

		Tara::VertexBufferRef vb_ref = Tara::VertexBuffer::Create(vertecies, sizeof(vertecies) / sizeof(float));

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
		uint32_t indecies[6] = {
			0, 1, 2, 2, 3, 0
		};
		Tara::IndexBufferRef ib_ref = Tara::IndexBuffer::Create(indecies, sizeof(indecies) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(ib_ref);


		Tara::ShaderRef shader = Tara::Shader::Create(
			"s_BasicShader",
			Tara::Shader::SourceType::TextFiles,
			"assets/texture.vertex.glsl",
			"assets/texture.fragment.glsl"
		);
		shader->Bind();

		m_Texture = Tara::Texture2D::Create("assets/UV_Checker.png");
	}
	
	virtual void Deactivate() {
		LOG_S(INFO) << "Testing Layer Deactivated!";
	}

	virtual void Update() {}
	
	virtual void Draw() {
		m_VertexArray->Bind();
		//m_Shader->Bind();
		Tara::ShaderRef shader = Tara::AssetLibrary::Get()->GetAssetIf<Tara::Shader>("s_BasicShader");
		if (shader) { 
			shader->Bind(); 
			shader->Send("u_Texture", 0);
		}
		m_Texture->Bind(0);
		

		Tara::RenderCommand::Draw(m_VertexArray);
	}
	
	
	virtual void OnEvent(Tara::Event& e) {}

private:
	//TESTING STUFF
	Tara::VertexArrayRef m_VertexArray;
	Tara::Texture2DRef m_Texture;
	//Tara::ShaderRef m_Shader;
};


int main(int argc, char** argv) {
	Tara::Application::Get()->Init(1200, 700, "Tara Playground Application!");
	//add layers to scene...
	Tara::Application::Get()->GetScene()->PushLayer(new TestingLayer());
	//run
	Tara::Application::Get()->Run();
	return 0;
}