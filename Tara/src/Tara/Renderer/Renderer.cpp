#include "tarapch.h"
#include "Tara/Renderer/Renderer.h"
#include "Tara/Renderer/RenderCommand.h"
#include "Tara/Math/BoundingBox.h"
//#include "Tara/Renderer/VertexArray.h"
//#include "Tara/Renderer/Shader.h"
//#include "Tara/Math/Types.h"

namespace Tara {
	//default to OpenGL renderbackend
	RenderBackend Renderer::s_RenderBackend = RenderBackend::OpenGl;
	Renderer::SceneData Renderer::s_SceneData = { nullptr };


	/*****************************************************************
	 *                    Quad Rendering Constants                   *
	 *****************************************************************/

	static bool s_InitializedQuadDraw = false;

	VertexArrayRef Renderer::s_QuadArray = nullptr;
	VertexArrayRef Renderer::s_BoxArray = nullptr;
	ShaderRef Renderer::s_TextureQuadShader = nullptr;
	ShaderRef Renderer::s_ColorQuadShader = nullptr;

#define VERTEX_ELEMENT_COUNT_QUAD 4 * (2 + 3)
#define VERTEX_INDEX_COUNT_QUAD 6
	
#define VERTEX_ELEMENT_COUNT_BOX 8 * 3
#define VERTEX_INDEX_COUNT_BOX 8 * 3



	void Renderer::BeginScene(const CameraRef camera)
	{
		s_SceneData.camera = camera;

		//if the Renderer has not initialized its quad related stuff, do it
		if (!s_InitializedQuadDraw){
			s_InitializedQuadDraw = true;

			//create vertex array
			s_QuadArray = VertexArray::Create();

			//data
			float verticesQuad[VERTEX_ELEMENT_COUNT_QUAD] = {
				0.0f, 0.0f, 0.0f, 0.0f, 0.0f, //bottom left
				1.0f, 0.0f, 0.0f, 1.0f, 0.0f, //bottom right
				1.0f, 1.0f, 0.0f, 1.0f, 1.0f, //top right
				0.0f, 1.0f, 0.0f, 0.0f, 1.0f  //top left
			};
			uint32_t indicesQuad[VERTEX_INDEX_COUNT_QUAD] = {
					0, 1, 2, 2, 3, 0
			};

			//add vertex data to array and set layout
			VertexBufferRef vb_ref = VertexBuffer::Create(verticesQuad, VERTEX_ELEMENT_COUNT_QUAD);
			vb_ref->SetLayout({
				{Tara::Shader::Datatype::Float3, "position"},
				{Tara::Shader::Datatype::Float2, "uv"}
			});
			s_QuadArray->AddVertexBuffer(vb_ref);

			//add index data to array
			IndexBufferRef ib_ref = IndexBuffer::Create(indicesQuad, VERTEX_INDEX_COUNT_QUAD);
			s_QuadArray->SetIndexBuffer(ib_ref);
			
			s_BoxArray = VertexArray::Create();
			float verticesBox[VERTEX_ELEMENT_COUNT_BOX] = {
				0.0f, 0.0f, 0.0f, //back bottom left
				1.0f, 0.0f, 0.0f, //back bottom right
				1.0f, 1.0f, 0.0f, //back top right
				0.0f, 1.0f, 0.0f, //back top left

				0.0f, 0.0f, 1.0f, //front bottom left
				1.0f, 0.0f, 1.0f, //front bottom right
				1.0f, 1.0f, 1.0f, //front top right
				0.0f, 1.0f, 1.0f, //front top left
			};
			uint32_t indicesBox[VERTEX_INDEX_COUNT_BOX] = {
					0, 1,  1, 2,  2, 3,  3, 0, //back
					0, 4,  1, 5,  2, 6,  3, 7, //sides
					4, 5,  5, 6,  6, 7,  7, 4  //front
			};

			//add vertex data to array and set layout
			vb_ref = VertexBuffer::Create(verticesBox, VERTEX_ELEMENT_COUNT_BOX);
			vb_ref->SetLayout({
				{Tara::Shader::Datatype::Float3, "position"},
			});
			s_BoxArray->AddVertexBuffer(vb_ref);

			//add index data to array
			ib_ref = IndexBuffer::Create(indicesBox, VERTEX_INDEX_COUNT_BOX);
			s_BoxArray->SetIndexBuffer(ib_ref);


			//shaders
			//These are stored as raw source code strings so that they are available even if there are no shader asset files.
			s_TextureQuadShader = Tara::Shader::Create(
				"S_TexturedQuad",
				Tara::Shader::SourceType::Strings,
				R"V0G0N(
				#version 450 core
				layout(location=0) in vec3 a_Position;
				layout(location=1) in vec2 a_UV;
				out vec2 v_UV;
				uniform mat4 u_MatrixViewProjection;
				uniform mat4 u_MatrixModel;
				void main(){
					v_UV = a_UV;
					gl_Position = u_MatrixViewProjection * u_MatrixModel * vec4(a_Position, 1);
				}
				
				)V0G0N",
				R"V0G1N(
				#version 450 core
				layout(location=0)out vec4 color;
				uniform sampler2D u_Texture;
				in vec2 v_UV;
				void main(){
					color = texture(u_Texture, v_UV);
				}
				)V0G1N"
			);


			s_ColorQuadShader = Tara::Shader::Create(
				"S_ColoredQuad",
				Tara::Shader::SourceType::Strings,
				R"V0G0N(
				#version 450 core
				layout(location=0) in vec3 a_Position;
				uniform mat4 u_MatrixViewProjection;
				uniform mat4 u_MatrixModel;
				void main(){
					gl_Position = u_MatrixViewProjection * u_MatrixModel * vec4(a_Position, 1);
				}
				
				)V0G0N",
				R"V0G1N(
				#version 450 core
				layout(location=0)out vec4 color;
				uniform vec4 u_Color;
				void main(){
					color = u_Color;
				}
				)V0G1N"
			);
		}
	}

	void Renderer::EndScene()
	{
		//execute batch rendering
		s_SceneData.camera = nullptr;
	}

	void Renderer::Draw(VertexArrayRef vertexArray, ShaderRef shader, Transform transform)
	{
		vertexArray->Bind();
		shader->Bind();
		shader->Send("u_MatrixViewProjection", s_SceneData.camera->GetViewProjectionMatrix());
		shader->Send("u_MatrixModel", transform.GetTransformMatrix());
		RenderCommand::Draw(vertexArray);
	}

	void Renderer::DrawLines(VertexArrayRef vertexArray, ShaderRef shader, Transform transform)
	{
		vertexArray->Bind();
		shader->Bind();
		shader->Send("u_MatrixViewProjection", s_SceneData.camera->GetViewProjectionMatrix());
		shader->Send("u_MatrixModel", transform.GetTransformMatrix());
		RenderCommand::DrawLines(vertexArray);
	}

	void Renderer::Quad(Texture2DRef texture, Transform transform)
	{
		if (s_SceneData.camera == nullptr) {
			return;
		}
		s_TextureQuadShader->Bind();
		texture->Bind(0);
		s_TextureQuadShader->Send("u_Texture", 0);
		Draw(s_QuadArray, s_TextureQuadShader, transform);
	}

	void Renderer::Quad(glm::vec4 color, Transform transform)
	{
		if (s_SceneData.camera == nullptr) {
			return;
		}
		s_ColorQuadShader->Bind();
		s_ColorQuadShader->Send("u_Color", color);
		Draw(s_QuadArray, s_ColorQuadShader, transform);
	}

	void Renderer::DrawBoundingBox(const BoundingBox& box, glm::vec4 color)
	{
		s_ColorQuadShader->Bind();
		s_ColorQuadShader->Send("u_Color", color);
		s_ColorQuadShader->Send("u_MatrixViewProjection", s_SceneData.camera->GetViewProjectionMatrix());
		s_ColorQuadShader->Send("u_MatrixModel", glm::translate(glm::mat4(1), box.Position) *glm::scale(glm::mat4(1), box.Extent));
		s_BoxArray->Bind();
		RenderCommand::DrawLines(s_BoxArray);
	}


}