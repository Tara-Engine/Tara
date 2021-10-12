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
	ShaderRef Renderer::s_QuadShader = nullptr;
	uint32_t Renderer::s_MaxTextures = 16;
	std::vector<Renderer::QuadGroup> Renderer::s_QuadGroups;

	void Renderer::BeginScene(const CameraRef camera)
	{
		s_SceneData.camera = camera;

		//if the Renderer has not initialized its quad related stuff, do it
		if (!s_InitializedQuadDraw){
			s_InitializedQuadDraw = true;

			//Get the max texture slots per shader
			s_MaxTextures = RenderCommand::GetMaxTextureSlotsPerShader();

			//create vertex array
			s_QuadArray = VertexArray::Create();
			
			//add null vertex data to array and set layout
			VertexBufferRef vb_ref = VertexBuffer::Create(nullptr, 0);
			vb_ref->SetLayout({
				{Tara::Shader::Datatype::Float3, "a_Position", false},
				{Tara::Shader::Datatype::Float3, "a_Rotation", false},
				{Tara::Shader::Datatype::Float3, "a_Scale", false},
				{Tara::Shader::Datatype::Float2, "a_UVmin", false},
				{Tara::Shader::Datatype::Float2, "a_UVmax", false},
				{Tara::Shader::Datatype::Float4, "a_Color", false},
				{Tara::Shader::Datatype::Float,  "a_TextureIndex", false}
			});
			s_QuadArray->AddVertexBuffer(vb_ref);

			//shaders
			//defined in seperate file for readability here.
			LoadQuadShader();
		}
	}

	void Renderer::EndScene()
	{
		//execute batch rendering
		s_QuadShader->Bind();
		s_QuadShader->Send("u_MatrixViewProjection", s_SceneData.camera->GetViewProjectionMatrix());
		s_QuadArray->Bind();
		for (const auto& group : s_QuadGroups) {
			s_QuadArray->GetVertexBuffers()[0]->SetData((float*)group.Quads.data(), group.Quads.size() * 18); //the 18 is not a "magic number", it is the number of floats in a QuadData struct.
			uint32_t index = 0;
			for (auto texture : group.TextureNames) {
				if (texture) {
					texture->Bind(index);
					s_QuadShader->Send("u_Texture" + std::to_string(index), (int)index);
					index++;
				}
			}
			RenderCommand::DrawPointList(s_QuadArray, group.Quads.size());
			//glDrawArrays(GL_POINTS, 0, m_Quads.size());
		}
		s_QuadGroups.clear();
		//clear camera
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

	void Renderer::Quad(Transform transform, glm::vec4 color, Texture2DRef texture, glm::vec2 minUV, glm::vec2 maxUV)
	{
		//Create the QuadData struct
		QuadData data = {
			transform,
			minUV,
			maxUV,
			color,
			-1 //temp
		};

		//find the slot to enter it into
		bool entered = false;
		for (auto& group : s_QuadGroups) {
			auto iter = std::find(group.TextureNames.begin(), group.TextureNames.end(), texture);
			if (iter == group.TextureNames.end()) {
				if (!texture || group.TextureNames.size() < s_MaxTextures) {
					//this group is not full, and does not contain this texture, (or, the texture is null)
					//so add this texture to the group, and then add this quad to the list
					//after setting its texture index correctly
					if (texture){
						data.TextureIndex = (float)group.TextureNames.size(); //the index about to be filled
						group.TextureNames.push_back(texture); //filled it!
					}
					group.Quads.push_back(data);
					entered = true;
					break;//no need to continue loop
				}
				else {
					//this group is full and does not contain this texture
					continue;
				}
			}
			else {
				//found that texture. Now, add this quad to the list.
				//also modify the quad texture index
				if (texture) {
					uint32_t index = iter - group.TextureNames.begin();
					data.TextureIndex = index; //the index about to be filled
				}
				group.Quads.push_back(data);
				entered = true;
				break; //no need to continue loop
			}
		}
		if (!entered) {
			//no group can take it, or there are none.
			//so, create a new group, add the stuff to it, and push it into the list.
			QuadGroup group;
			if (texture) {
				data.TextureIndex = (float)group.TextureNames.size(); //the index about to be filled
				group.TextureNames.push_back(texture); //filled it!
			}
			group.Quads.push_back(data);
			s_QuadGroups.push_back(group);
		}

	}



	


}