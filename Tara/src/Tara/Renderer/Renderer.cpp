#include "tarapch.h"
#include "Tara/Renderer/Renderer.h"
#include "Tara/Renderer/RenderCommand.h"
#include "Tara/Math/BoundingBox.h"
#include "Tara/Core/Application.h"
#include "Tara/Utility/MeshMaker.h"
//#include "Tara/Renderer/VertexArray.h"
//#include "Tara/Renderer/Shader.h"
//#include "Tara/Math/Types.h"

namespace Tara {
	//default to OpenGL renderbackend
	RenderBackend Renderer::s_RenderBackend = RenderBackend::OpenGl;
	RenderSceneData Renderer::s_SceneData = { nullptr };
	
	RenderTargetRef Renderer::s_GBuffer = nullptr;
	StaticMeshRef Renderer::s_ScreenQuadMesh = nullptr;
	StaticMeshRef Renderer::s_LightSphereMesh = nullptr;

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
		//save camera and target for future commands and queue execution
		s_SceneData.camera = camera;
		s_SceneData.target = camera->GetRenderTarget();
		

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

		//begin command queue
		RenderCommand::BeginQueue();
	}

	void Renderer::EndScene()
	{
		//render the batched quads
		RenderQuads();
		
		//render the scene, deferred, lighting, etc.
		SceneRender();
		
		//clear Scene Data
		s_SceneData.camera = nullptr;
		s_SceneData.target = nullptr;
		auto size = s_SceneData.lights.size();
		s_SceneData.lights.clear();
		s_SceneData.lights.reserve(size);
	}

	void Renderer::Draw(VertexArrayRef vertexArray, ShaderRef shader, Transform transform)
	{
		vertexArray->Bind();
		shader->Bind();
		if (shader->ValidUniform("u_MatrixViewProjection")) {
			shader->Send("u_MatrixViewProjection", s_SceneData.camera->GetViewProjectionMatrix());
		}
		else {
			if (shader->ValidUniform("u_MatrixView")) {
				shader->Send("u_MatrixView", s_SceneData.camera->GetViewMatrix());
			}
			if (shader->ValidUniform("u_MatrixProjection")) {
				shader->Send("u_MatrixProjection", s_SceneData.camera->GetProjectionMatrix());
			}
		}
		if (shader->ValidUniform("u_MatrixModel")){
			shader->Send("u_MatrixModel", transform.GetTransformMatrix());
		}
		if (shader->ValidUniform("u_CameraPositionWS")) {
			shader->Send("u_CameraPositionWS", s_SceneData.camera->GetPosition());
		}
		if (shader->ValidUniform("u_CameraForwardVector")) {
			shader->Send("u_CameraForwardVector", s_SceneData.camera->GetRotation().GetForwardVector());
		}
		
		RenderCommand::Draw(vertexArray);
		vertexArray->Unbind(); //Leaving this bound could cause manual VertexBuffer / IndexBuffer creation to effect this vertexArray, so, unbind it.
	}

	void Renderer::Quad(const Transform& transform, glm::vec4 color, const Texture2DRef& texture, glm::vec2 minUV, glm::vec2 maxUV)
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
					float index = (float)(iter - group.TextureNames.begin());
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


	void Tara::Renderer::Text(const Transform& transform, const std::string& text, FontRef font, glm::vec4 color)
	{
		Transform t(transform);
		if (s_SceneData.camera->GetProjectionType() != Camera::ProjectionType::Screen) {
			t.Scale.y *= -1;
		}
		std::vector<Transform> transforms;
		std::vector<glm::vec2> uvMin;
		std::vector<glm::vec2> uvMax;
		font->GetTextQuads(text, transforms, uvMin, uvMax);
		for (int i = 0; i < text.size(); i++) {
			Quad(t + transforms[i], color, font->GetTexture(), uvMin[i], uvMax[i]);
		}
	}
	
	void Renderer::Patch(const Transform& transform, const PatchRef& patch, glm::vec4 color)
	{
		Transform offset{ transform.Position, transform.Rotation, {1.0f, 1.0f, transform.Scale.z} };
		auto muv = patch->GetMiddleUVs();
		auto mp = patch->GetMiddleOffsets(glm::vec2(transform.Scale.x, transform.Scale.y));
		auto t = patch->GetTexture();

		float xPos[] = { 0.0f, mp.first.x, mp.second.x, transform.Scale.x };
		//Y is defaulted for when in a ScreenCamera (as that is normal)
		float yPos[] = { transform.Scale.y, transform.Scale.y-mp.first.y, transform.Scale.y-mp.second.y, 0.0f };
		
		float xUV[] = { 0.0f, muv.first.x, muv.second.x, 1.0f };
		float yUV[] = { 0.0f, muv.first.y, muv.second.y, 1.0f };
		
		//when not in screen camera, invert Y
		if (s_SceneData.camera->GetProjectionType() != Camera::ProjectionType::Screen) {
			for (int i = 0; i < 4; i++) {
				yPos[i] = (transform.Scale.y - yPos[i]);
			}
		}
		
		//draw the quads.
		for (int x = 0; x < 3; x++) {
			for (int y = 0; y < 3; y++) {
				Quad(
					offset + TRANSFORM_2D(
						xPos[x], yPos[y],
						0.0f,
						xPos[x + 1] - xPos[x], yPos[y + 1] - yPos[y]),
					color, t,
					{ xUV[x],yUV[y] },
					{ xUV[x + 1],yUV[y + 1] }
				);
			}
		}
	}

	void Renderer::Light(const LightData& light)
	{
		s_SceneData.lights.push_back(light);
	}

	void Renderer::StaticMesh(const Transform& transform, const StaticMeshRef& mesh, const std::vector<MaterialBaseRef>& materials)
	{
		if (materials.size() == 0) {
			LOG_S(WARNING) << "Attemtped to render a static mesh with no materials";
			return;
		}
		int j = 0;
		auto& vertexArrays = mesh->GetVertexArrays();
		for (int i = 0; i < mesh->GetArrayCount(); i++) {
			materials[j]->Use();
			Draw(vertexArrays[i], materials[j]->GetShader(), transform);
			//take into account that the materials list may be shorter than the number of mesh parts
			if (materials.size() > j + 1) {
				j++;
			}
		}
	}

	void Renderer::RenderQuads()
	{
		if (s_QuadGroups.size() > 0){
			RenderCommand::EnableDeferred(false);
			RenderCommand::EnableBackfaceCulling(false);
			RenderCommand::EnableDepthTesting(false);
			//execute batch rendering
			s_QuadShader->Bind();
			s_QuadShader->Send("u_MatrixViewProjection", s_SceneData.camera->GetViewProjectionMatrix());
			s_QuadArray->Bind();
			s_QuadArray->GetVertexBuffers()[0]->Bind();
			for (const auto& group : s_QuadGroups) {
				s_QuadArray->GetVertexBuffers()[0]->SetData((float*)group.Quads.data(), (uint32_t)group.Quads.size() * 18); //the 18 is not a "magic number", it is the number of floats in a QuadData struct.
				uint32_t index = 0;
				for (auto texture : group.TextureNames) {
					if (texture) {
						texture->Bind(index);
						s_QuadShader->Send("u_Texture" + std::to_string(index), (int)index);
						index++;
					}
				}
				s_QuadArray->Bind();
				RenderCommand::PushDrawType(RenderDrawType::Points);
				RenderCommand::DrawCount((uint32_t)group.Quads.size());
				RenderCommand::PopDrawType();
				//glDrawArrays(GL_POINTS, 0, m_Quads.size());
			}
			s_QuadArray->GetVertexBuffers()[0]->Unbind();
			s_QuadArray->Unbind();
			s_QuadShader->Unbind();
			s_QuadGroups.clear();
		}
	}


	void Renderer::SceneRender()
	{
		RenderCommand::StopQueue();
		RenderCommand::EnableDepthTesting(true);
		auto& lightingMat = s_SceneData.camera->GetLightingMaterial();
		if (lightingMat) {
			RenderCommand::SetBlendMode(RenderBlendMode::REPLACE);

			auto window = Application::Get()->GetWindow();
			if (s_GBuffer == nullptr) {
				s_GBuffer = RenderTarget::Create(window->GetWidth(), window->GetHeight(), 5, RenderTarget::InternalType::FLOAT, "gBuffer");
			}
			else {
				//check for resize
				if (window->GetWidth() != s_GBuffer->GetWidth() || window->GetHeight() != s_GBuffer->GetHeight()) {
					s_GBuffer->SetSize(window->GetWidth(), window->GetHeight());
				}
			}
			s_GBuffer->RenderTo(true);
			RenderCommand::Clear();

			RenderCommand::EnableDeferred(true);
			RenderCommand::ExecuteQueue();

			//End rendering to GBuffer, blit its depth to the passed RenderTarget
			if (s_SceneData.target) {
				s_SceneData.target->RenderTo(true);
			}
			else {
				s_GBuffer->RenderTo(false);
			}
			//render the quad
			//VertexArrayRef screenQuad;
			if (!s_ScreenQuadMesh){
				//load the ScreenQuad
				MeshMaker mm(MeshMaker::Mode::QUADS);
				mm.Vertex(-1, -1, 0); mm.TextureCoord(0, 0); mm.Color(1, 1, 1, 1);
				mm.Vertex(-1,  1, 0); mm.TextureCoord(0, 1); mm.Color(1, 1, 1, 1);
				mm.Vertex( 1,  1, 0); mm.TextureCoord(1, 1); mm.Color(1, 1, 1, 1);
				mm.Vertex( 1, -1, 0); mm.TextureCoord(1, 0); mm.Color(1, 1, 1, 1);
				s_ScreenQuadMesh = StaticMesh::Create({ mm.GetMeshPart() }, "__ScreenQuadMesh__");
			}
			if (!s_LightSphereMesh) {
				auto sphere = MeshPart::UnitSphere();
				sphere.FlipWindings();
				s_LightSphereMesh = StaticMesh::Create({ sphere }, "__LightSphereMesh__");
			}

			//render the screen quad with light material
			lightingMat->Use();
			auto& shader = lightingMat->GetShader();

			//These are all guaranteed to be used
			s_GBuffer->ImplBind(0, 0);
			shader->Send("u_ColorMetallicSampler", 0);
			s_GBuffer->ImplBind(1, 1);
			shader->Send("u_SpecularRoughnessSampler", 1);
			s_GBuffer->ImplBind(2, 2);
			shader->Send("u_EmissiveAOSampler", 2);
			s_GBuffer->ImplBind(3, 3);
			shader->Send("u_WorldSpaceNormalSampler", 3);
			s_GBuffer->ImplBind(4, 4);
			shader->Send("u_WorldSpacePositionSampler", 4);

			//these might be optimized away if the specific material does not use them
			if (shader->ValidUniform("u_TargetSize")) {
				glm::vec2 size;
				if (s_SceneData.target) {
					size = glm::vec2(s_SceneData.target->GetWidth(), s_SceneData.target->GetHeight());
				}
				else {
					size = glm::vec2(Application::Get()->GetWindow()->GetWidth(), Application::Get()->GetWindow()->GetHeight());
				}
				shader->Send("u_TargetSize", size);
			}
			if (shader->ValidUniform("u_CameraPositionWS")) {
				shader->Send("u_CameraPositionWS", s_SceneData.camera->GetPosition());
			}
			if (shader->ValidUniform("u_CameraForwardVector")) {
				shader->Send("u_CameraForwardVector", s_SceneData.camera->GetRotation().GetForwardVector());
			}

			//deal with lights
			
			RenderCommand::SetBlendMode(RenderBlendMode::ADD);
			RenderCommand::EnableDepthTesting(false);
			for (auto& light : s_SceneData.lights) {
				//auto& light = s_SceneData.lights[i];
				
				//uniform int u_LightCount;
				if (shader->ValidUniform("u_LightPosition")) {
					shader->Send("u_LightPosition", light.Position);
				}
				if (shader->ValidUniform("u_LightForwardVector")) {
					shader->Send("u_LightForwardVector", light.ForwardVector);
				}
				if (shader->ValidUniform("u_LightColor")) {
					shader->Send("u_LightColor", (glm::vec3)light.Color);
				}
				if (shader->ValidUniform("u_LightType")) {
					shader->Send("u_LightType", (int32_t)light.Type);
				}
				if (shader->ValidUniform("u_LightIntensity")) {
					shader->Send("u_LightIntensity", light.Intensity);
				}
				if (shader->ValidUniform("u_LightParam1")) {
					shader->Send("u_LightParam1", light.Custom1);
				}
				if (shader->ValidUniform("u_LightParam2")) {
					shader->Send("u_LightParam2", light.Custom2);
				}
				if (shader->ValidUniform("u_LightRadius")) {
					shader->Send("u_LightRadius", light.MaxRadius);
				}

				if (light.MaxRadius <= 0 || light.MaxRadius == std::numeric_limits<float>::infinity()) {
					auto vertArray = s_ScreenQuadMesh->GetVertexArrays()[0];
					vertArray->ImplBind(0,0);
					//send identity matricies
					if (shader->ValidUniform("u_MatrixViewProjection")) {
						shader->Send("u_MatrixViewProjection", glm::mat4(1.0f));
					}
					if (shader->ValidUniform("u_MatrixModel")) {
						shader->Send("u_MatrixModel", glm::mat4(1.0f));
					}
					
					RenderCommand::Draw(vertArray);
					vertArray->ImplUnbind();
				}
				else {
					auto vertArray = s_LightSphereMesh->GetVertexArrays()[0];
					vertArray->ImplBind(0, 0);

					if (shader->ValidUniform("u_MatrixViewProjection")) {
						shader->Send("u_MatrixViewProjection", s_SceneData.camera->GetViewProjectionMatrix());
					}
					if (shader->ValidUniform("u_MatrixModel")) {
						shader->Send("u_MatrixModel", Transform(light.Position, Rotator::FromForwardVector(light.ForwardVector), {light.MaxRadius}).GetTransformMatrix());
					}
					
					RenderCommand::Draw(vertArray);
					vertArray->ImplUnbind();
				}
			}

			

			//copy depth
			s_GBuffer->BlitDepthToOther(s_SceneData.target);
		}
		else {
			//LOG_S(WARNING) << "No lighting material in the camera, thus, no deferred rendering will take place";
			if (s_SceneData.target) {
				s_SceneData.target->RenderTo(true);
			}
			//discard the deferred queue
			RenderCommand::EnableDeferred(true);
			RenderCommand::DiscardQueue();
		}
		RenderCommand::SetBlendMode(RenderBlendMode::NORMAL);
		RenderCommand::EnableDeferred(false);
		RenderCommand::ExecuteQueue();

		//unset the target RenderTarget
		//if target is nullptr, then it should be fine.
		if (s_SceneData.target) {
			s_SceneData.target->RenderTo(false);
		}
	}
}