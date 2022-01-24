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
	

	/*****************************************************************
	 *                       Rendering Constants                     *
	 *****************************************************************/

	static bool s_InitializedQuadDraw = false;
	static bool s_InitializedDepthDraw = false;

	VertexArrayRef Renderer::s_QuadArray = nullptr;
	ShaderRef Renderer::s_QuadShader = nullptr;
	uint32_t Renderer::s_MaxTextures = 16;
	std::vector<Renderer::QuadGroup> Renderer::s_QuadGroups;
	
	ShaderRef Renderer::s_MeshDepthShader = nullptr;
	ShaderRef Renderer::s_MeshDepthPanoramicShader = nullptr;
	
	RenderBackend Renderer::s_RenderBackend = RenderBackend::OpenGl;
	RenderSceneData Renderer::s_SceneData = { nullptr, nullptr };
	
	RenderTarget2DRef Renderer::s_GBuffer = nullptr;
	RenderTarget2DRef Renderer::s_PostProcessBufferA = nullptr;
	RenderTarget2DRef Renderer::s_PostProcessBufferB = nullptr;
	RenderTarget2DRef Renderer::s_FakeDepth2D = nullptr;
	RenderTargetCubemapRef Renderer::s_FakeDepthCubemap = nullptr;
	StaticMeshRef Renderer::s_ScreenQuadMesh = nullptr;
	StaticMeshRef Renderer::s_LightSphereMesh = nullptr;

	/*****************************************************************
	 *                       Rendering Functions                     *
	 *****************************************************************/

	void Renderer::BeginScene(const CameraRef camera)
	{
		//save camera and target for future commands and queue execution
		s_SceneData.camera = camera;
		s_SceneData.target = camera->GetRenderTarget();
		s_SceneData.light = nullptr;
		s_SceneData.renderPass = RenderPass::Standard;
		

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

	void Tara::Renderer::BeginModelDepthScene(const LightBaseRef light)
	{
		s_SceneData.camera = nullptr;
		s_SceneData.target = light->GetDepthTarget();
		s_SceneData.light = light;
		s_SceneData.renderPass = RenderPass::ModelDepth;

		if (!s_InitializedDepthDraw) {
			LoadMeshDepthShader();
			s_InitializedDepthDraw = true;
		}

		if (s_SceneData.target) {
			s_SceneData.target->RenderTo(true);
		}
		RenderCommand::EnableFrontfaceCulling(true);
		RenderCommand::EnableDepthTesting(true);
		RenderCommand::Clear();
		//Don't queue these draws, for now
	}

	void Renderer::EndScene()
	{
		if (s_SceneData.renderPass == RenderPass::Standard) {
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
		else {
			//LOG_S(ERROR) << "Renderer::EndScene not supported for ModelDepth render passes yet";
			if (s_SceneData.target) {
				s_SceneData.target->RenderTo(false);
			}
			s_SceneData.camera = nullptr;
			s_SceneData.light = nullptr;
			s_SceneData.target = nullptr;

			RenderCommand::EnableFrontfaceCulling(false);
		}
	}

	void Renderer::Draw(VertexArrayRef vertexArray, ShaderRef shader, Transform transform)
	{
		if (s_SceneData.renderPass != RenderPass::Standard) {
			return;
		}
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
		if (s_SceneData.renderPass != RenderPass::Standard) {
			return;
		}
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
		if (s_SceneData.renderPass != RenderPass::Standard) {
			return;
		}
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
		if (s_SceneData.renderPass != RenderPass::Standard) {
			return;
		}
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

	void Renderer::Light(const LightBaseRef& light)
	{
		if (s_SceneData.renderPass != RenderPass::Standard) {
			return;
		}
		s_SceneData.lights.push_back(light);
	}

	void Renderer::StaticMesh(const Transform& transform, const StaticMeshRef& mesh, const std::vector<MaterialBaseRef>& materials)
	{
		if (s_SceneData.renderPass == RenderPass::Standard) {
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
		else {
			
			auto& vertexArrays = mesh->GetVertexArrays();
			auto lightData = s_SceneData.light->GetLightData();
			ShaderRef shader = nullptr;
			//if (s_SceneData.light->DepthIsPanoramic()) {
			//	s_MeshDepthPanoramicShader->Bind();
			//	shader = s_MeshDepthPanoramicShader;
			//}
			//else {
			s_MeshDepthShader->Bind();
			shader = s_MeshDepthShader;
			//}
			
			if (shader->ValidUniform("u_MatrixModel")) {
				shader->Send("u_MatrixModel", transform.GetTransformMatrix());
			}
			if (shader->ValidUniform("u_CameraPositionWS")) {
				shader->Send("u_CameraPositionWS", lightData.Position);
			}
			if (shader->ValidUniform("u_FarClipPlane")) {
				shader->Send("u_FarClipPlane", lightData.MaxRadius);
			}
			
			if (s_SceneData.light->DepthIsPanoramic()) {
					glm::mat4 proj = s_SceneData.light->GetLightProjectionMatrix();
					std::vector<glm::mat4> viewProjMatricies;
					viewProjMatricies.reserve(6);
					viewProjMatricies.push_back(proj * glm::lookAt((glm::vec3)lightData.Position, (glm::vec3)(lightData.Position) + glm::vec3{ 1.0f, 0.0f, 0.0f}, { 0.0f,-1.0f, 0.0f}));
					viewProjMatricies.push_back(proj * glm::lookAt((glm::vec3)lightData.Position, (glm::vec3)(lightData.Position) + glm::vec3{-1.0f, 0.0f, 0.0f}, { 0.0f,-1.0f, 0.0f}));
					viewProjMatricies.push_back(proj * glm::lookAt((glm::vec3)lightData.Position, (glm::vec3)(lightData.Position) + glm::vec3{ 0.0f, 1.0f, 0.0f}, { 0.0f, 0.0f, 1.0f}));
					viewProjMatricies.push_back(proj * glm::lookAt((glm::vec3)lightData.Position, (glm::vec3)(lightData.Position) + glm::vec3{ 0.0f,-1.0f, 0.0f}, { 0.0f, 0.0f,-1.0f}));
					viewProjMatricies.push_back(proj * glm::lookAt((glm::vec3)lightData.Position, (glm::vec3)(lightData.Position) + glm::vec3{ 0.0f, 0.0f, 1.0f}, { 0.0f,-1.0f, 0.0f}));
					viewProjMatricies.push_back(proj * glm::lookAt((glm::vec3)lightData.Position, (glm::vec3)(lightData.Position) + glm::vec3{ 0.0f, 0.0f,-1.0f}, { 0.0f,-1.0f, 0.0f}));
					
					
				for (int i = 0; i < mesh->GetArrayCount(); i++) {
					vertexArrays[i]->Bind();
					for(int j=0;j<6;j++){
						std::dynamic_pointer_cast<RenderTargetCubemap>(s_SceneData.target)->SetTargetedFace((RenderTargetCubemap::Face)j);
						if (shader->ValidUniform("u_MatrixViewProjection")){
							shader->Send("u_MatrixViewProjection", viewProjMatricies[j]);
						}
						RenderCommand::Draw(vertexArrays[i]);
					}
				}
				std::dynamic_pointer_cast<RenderTargetCubemap>(s_SceneData.target)->SetTargetFaceAll();
			}
			else {
				if (shader->ValidUniform("u_MatrixViewProjection")) {
					shader->Send("u_MatrixViewProjection", s_SceneData.light->GetLightProjectionMatrix());
				}
				for (int i = 0; i < mesh->GetArrayCount(); i++) {
					vertexArrays[i]->Bind();
					RenderCommand::Draw(vertexArrays[i]);
				}
			}

			//u_FarClipPlane
			
			vertexArrays[vertexArrays.size()-1]->Unbind(); //Leaving this bound could cause manual VertexBuffer / IndexBuffer creation to effect this vertexArray, so, unbind it.

		}
	}

	void Renderer::RenderQuads()
	{
		if (s_QuadGroups.size() > 0){
			RenderCommand::EnableDeferred(false);
			RenderCommand::EnableBackfaceCulling(false);
			if (s_SceneData.camera->GetProjectionType() == Camera::ProjectionType::Screen) {
				RenderCommand::EnableDepthTesting(false);
			}
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

	static bool s_LoadedSceneParts = false;
	void Renderer::SceneRender()
	{
		RenderCommand::StopQueue();
		RenderCommand::EnableDepthTesting(true);
		auto& lightingMat = s_SceneData.camera->GetLightingMaterial();
		auto& ppMaterials = s_SceneData.camera->GetPostProcessMaterials();

		//load all the assets nesecary to render
		if (!s_LoadedSceneParts){
			LoadSceneParts();
			s_LoadedSceneParts = true;
		}

		glm::vec2 finalSize;
		if (s_SceneData.target) {
			finalSize = { s_SceneData.target->GetWidth(), s_SceneData.target->GetHeight() };
		}
		else {
			auto window = Application::Get()->GetWindow();
			finalSize = { window->GetWidth(), window->GetHeight() };
		}
		//setup post process RenderTargets

		//check for resize. YES, this is BAD if the engine is rendering to a bunch of different-sized rendertargets every frame.
		if (finalSize.x != s_GBuffer->GetWidth() || finalSize.y != s_GBuffer->GetHeight()) {
			s_PostProcessBufferA->SetSize(finalSize.x, finalSize.y);
		}
		
		//check for resize. YES, this is BAD if the engine is rendering to a bunch of different-sized rendertargets every frame.
		if (finalSize.x != s_GBuffer->GetWidth() || finalSize.y != s_GBuffer->GetHeight()) {
			s_PostProcessBufferB->SetSize(finalSize.x, finalSize.y);
		}
		//check for resize. YES, this is BAD if the engine is rendering to a bunch of different-sized rendertargets every frame.
		if (finalSize.x != s_GBuffer->GetWidth() || finalSize.y != s_GBuffer->GetHeight()) {
			s_GBuffer->SetSize(finalSize.x, finalSize.y);
		}
		

		if (lightingMat) {
			RenderCommand::SetBlendMode(RenderBlendMode::REPLACE);

			s_GBuffer->RenderTo(true);
			RenderCommand::Clear();
			RenderCommand::EnableBackfaceCulling(true);
			RenderCommand::EnableDeferred(true);
			RenderCommand::ExecuteQueue();

			//End rendering to GBuffer, render to passed rendertarget, screen, or postprocess buffer A
			if (ppMaterials.size() > 0) {
				s_PostProcessBufferA->RenderTo(true);
			}
			else{
				if (s_SceneData.target) {
					s_SceneData.target->RenderTo(true);
				}
				else {
					s_GBuffer->RenderTo(false);
				}
			}
			RenderCommand::Clear();
			

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
				shader->Send("u_TargetSize", finalSize);
			}
			if (shader->ValidUniform("u_CameraPositionWS")) {
				shader->Send("u_CameraPositionWS", s_SceneData.camera->GetPosition());
			}
			if (shader->ValidUniform("u_CameraForwardVector")) {
				shader->Send("u_CameraForwardVector", s_SceneData.camera->GetRotation().GetForwardVector());
			}
			if (shader->ValidUniform("u_CameraNearClipPlane")) {
				shader->Send("u_CameraNearClipPlane", s_SceneData.camera->GetNearClipPlane());
			}
			if (shader->ValidUniform("u_CameraFarClipPlane")) {
				shader->Send("u_CameraFarClipPlane", s_SceneData.camera->GetFarClipPlane());
			}

			//deal with lights
			
			RenderCommand::SetBlendMode(RenderBlendMode::ADD);
			RenderCommand::EnableDepthTesting(false);
			float originalFarPlane = s_SceneData.camera->GetFarClipPlane();
			if (originalFarPlane != 1.0f) {
				s_SceneData.camera->SetFarClipPlane(originalFarPlane * 2);
			}

			for (auto& light : s_SceneData.lights) {
				//auto& light = s_SceneData.lights[i];
				auto lightData = light->GetLightData();
				//uniform int u_LightCount;
				if (shader->ValidUniform("u_LightPosition")) {
					shader->Send("u_LightPosition", lightData.Position);
				}
				if (shader->ValidUniform("u_LightForwardVector")) {
					shader->Send("u_LightForwardVector", lightData.ForwardVector);
				}
				if (shader->ValidUniform("u_LightColor")) {
					shader->Send("u_LightColor", (glm::vec3)lightData.Color);
				}
				if (shader->ValidUniform("u_LightType")) {
					shader->Send("u_LightType", (int32_t)lightData.Type);
				}
				if (shader->ValidUniform("u_LightIntensity")) {
					shader->Send("u_LightIntensity", lightData.Intensity);
				}
				if (shader->ValidUniform("u_LightParam1")) {
					shader->Send("u_LightParam1", lightData.Custom1);
				}
				if (shader->ValidUniform("u_LightParam2")) {
					shader->Send("u_LightParam2", lightData.Custom2);
				}
				if (shader->ValidUniform("u_LightRadius")) {
					if (lightData.MaxRadius < 0) { //invert if less than 0
						shader->Send("u_LightRadius", -lightData.MaxRadius);
					}
					else {
						shader->Send("u_LightRadius", lightData.MaxRadius);
					}
				}
				if (light->ShouldDrawDepth()){
					if (shader->ValidUniform("u_LightProjectionMatrix")) {
						shader->Send("u_LightProjectionMatrix", light->GetLightProjectionMatrix());
					}
					if (light->DepthIsPanoramic()) {
						if (shader->ValidUniform("u_LightDepthMapPanoramic")) {
							light->GetDepthTarget()->ImplBind(5, 0);
							shader->Send("u_LightDepthMapPanoramic", 5);
						}
						if (shader->ValidUniform("u_LightDepthMapPlanar")) {
							s_FakeDepth2D->ImplBind(6, 0);
							shader->Send("u_LightDepthMapPlanar", 6);
						}
					}
					else {
						if (shader->ValidUniform("u_LightDepthMapPanoramic")) {
							s_FakeDepthCubemap->ImplBind(5, 0);
							shader->Send("u_LightDepthMapPanoramic", 5);
						}
						if (shader->ValidUniform("u_LightDepthMapPlanar")) {
							light->GetDepthTarget()->ImplBind(6, 0);
							shader->Send("u_LightDepthMapPlanar", 6);
						}
					}
					if (shader->ValidUniform("u_LightDepthMapSize")) {
						shader->Send("u_LightDepthMapSize", (float)light->GetDepthTarget()->GetWidth());
					}
				}
				else {
					if (shader->ValidUniform("u_LightDepthMapPanoramic")) {
						s_FakeDepthCubemap->ImplBind(5, 0);
						shader->Send("u_LightDepthMapPanoramic", 5);
					}
					if (shader->ValidUniform("u_LightDepthMapPlanar")) {
						s_FakeDepth2D->ImplBind(6, 0);
						shader->Send("u_LightDepthMapPlanar", 6);
					}
					if (shader->ValidUniform("u_LightDepthMapSize")) {
						shader->Send("u_LightDepthMapSize", 1.0f);
					}
				}

				if (lightData.MaxRadius <= 0 || lightData.MaxRadius == std::numeric_limits<float>::infinity()) {
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
						shader->Send("u_MatrixModel", Transform(lightData.Position, Rotator::FromForwardVector(lightData.ForwardVector), { lightData.MaxRadius}).GetTransformMatrix());
					}
					
					RenderCommand::Draw(vertArray);
					vertArray->ImplUnbind();
				}
			}
			if (originalFarPlane != 1.0f) {
				s_SceneData.camera->SetFarClipPlane(originalFarPlane);
			}
			
			//copy depth, and implicitly set the s_SceneData.target as targeted RenderBuffer
			if (ppMaterials.size() > 0) {
				//set the PostProcess renderTarget 
				s_GBuffer->BlitDepthToOther(s_PostProcessBufferA);
			}
			else {
				s_GBuffer->BlitDepthToOther(s_SceneData.target);
			}
		}
		else {
			if (ppMaterials.size() > 0) {
				//set the PostProcess renderTarget 
				s_PostProcessBufferA->RenderTo(true);
			}
			else {
				//set the camera target as the targeted rendertarget
				if (s_SceneData.target) {
					s_SceneData.target->RenderTo(true);
				}
			}

			//discard the deferred queue
			RenderCommand::EnableDeferred(true);
			RenderCommand::DiscardQueue();
		}
		
		//execute forward rendering (so much simpler)
		RenderCommand::SetBlendMode(RenderBlendMode::NORMAL);
		RenderCommand::EnableDepthTesting(true);
		RenderCommand::EnableDeferred(false);
		RenderCommand::ExecuteQueue();

		//Post-processing!
		if (ppMaterials.size() > 0) {
			RenderCommand::SetBlendMode(RenderBlendMode::REPLACE);
			RenderCommand::EnableDepthTesting(false);
			bool renderingToA = false;
			auto vertArray = s_ScreenQuadMesh->GetVertexArrays()[0];

			for (int i = 0; i < ppMaterials.size(); i++) {
				if (!ppMaterials[i]) {
					continue;
				}
				//setup the current target
				if(i < ppMaterials.size()-1){
					//flipflop on which is rendered to and which is rendered from
					if (renderingToA) {
						s_PostProcessBufferA->RenderTo(true);
					}
					else {
						s_PostProcessBufferB->RenderTo(true);
					}
				}
				else {
					//render to screen on last pass
					if (s_SceneData.target) {
						s_SceneData.target->RenderTo(true);
					}
					else {
						s_PostProcessBufferB->RenderTo(false);
					}
				}
				RenderCommand::Clear();

				//use the material, and set up the uniforms
				ppMaterials[i]->Use();
				auto& shader = ppMaterials[i]->GetShader();

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
				if (renderingToA) {
					s_PostProcessBufferB->ImplBind(5, 0);
				}
				else {
					s_PostProcessBufferA->ImplBind(5, 0);
				}
				shader->Send("u_FinalColorSampler", 5);

				//these might be optimized away if the specific material does not use them
				if (shader->ValidUniform("u_TargetSize")) {
					shader->Send("u_TargetSize", finalSize);
				}
				if (shader->ValidUniform("u_CameraPositionWS")) {
					shader->Send("u_CameraPositionWS", s_SceneData.camera->GetPosition());
				}
				if (shader->ValidUniform("u_CameraForwardVector")) {
					shader->Send("u_CameraForwardVector", s_SceneData.camera->GetRotation().GetForwardVector());
				}
				if (shader->ValidUniform("u_CameraNearClipPlane")) {
					shader->Send("u_CameraNearClipPlane", s_SceneData.camera->GetNearClipPlane());
				}
				if (shader->ValidUniform("u_CameraFarClipPlane")) {
					shader->Send("u_CameraFarClipPlane", s_SceneData.camera->GetFarClipPlane());
				}

				
				vertArray->ImplBind(0, 0);
				//send identity matricies
				if (shader->ValidUniform("u_MatrixViewProjection")) {
					shader->Send("u_MatrixViewProjection", glm::mat4(1.0f));
				}
				if (shader->ValidUniform("u_MatrixModel")) {
					shader->Send("u_MatrixModel", glm::mat4(1.0f));
				}
				RenderCommand::Draw(vertArray);

				//switch which is being rendered to
				renderingToA = !renderingToA;
			}
			vertArray->ImplUnbind();

			
			
		}

		//unset the target RenderTarget
		//if target is nullptr, then it should be fine.
		if (s_SceneData.target) {
			s_SceneData.target->RenderTo(false);
		}
	}



	void Renderer::LoadSceneParts()
	{
		if (s_ScreenQuadMesh == nullptr) {
			//load the ScreenQuad (if unloaded)
			MeshMaker mm(MeshMaker::Mode::QUADS);
			mm.Vertex(-1, -1, 0); mm.TextureCoord(0, 0); mm.Color(1, 1, 1, 1);
			mm.Vertex(-1, 1, 0); mm.TextureCoord(0, 1); mm.Color(1, 1, 1, 1);
			mm.Vertex(1, 1, 0); mm.TextureCoord(1, 1); mm.Color(1, 1, 1, 1);
			mm.Vertex(1, -1, 0); mm.TextureCoord(1, 0); mm.Color(1, 1, 1, 1);
			s_ScreenQuadMesh = StaticMesh::Create({ mm.GetMeshPart() }, "__ScreenQuadMesh__");
		}
		if (s_LightSphereMesh == nullptr) {
			auto sphere = MeshPart::UnitSphere();
			sphere.FlipWindings();
			s_LightSphereMesh = StaticMesh::Create({ sphere }, "__LightSphereMesh__");
		}

		glm::vec2 FinalSize;
		if (s_SceneData.target) {
			FinalSize = { s_SceneData.target->GetWidth(), s_SceneData.target->GetHeight() };
		}
		else {
			auto window = Application::Get()->GetWindow();
			FinalSize = { window->GetWidth(), window->GetHeight() };
		}

		if (s_GBuffer == nullptr) {
			s_GBuffer = RenderTarget2D::Create(FinalSize.x, FinalSize.y, 5, RenderTarget::InternalType::FLOAT, false, "__gBuffer__");
		}
		if (s_PostProcessBufferA == nullptr) {
			s_PostProcessBufferA = RenderTarget2D::Create(FinalSize.x, FinalSize.y, 1, RenderTarget::InternalType::FLOAT, false, "__PostProcessBufferA__");
		}
		if (s_PostProcessBufferB == nullptr) {
			s_PostProcessBufferB = RenderTarget2D::Create(FinalSize.x, FinalSize.y, 1, RenderTarget::InternalType::FLOAT, false, "__PostProcessBufferB__");
		}
		if (s_FakeDepth2D == nullptr) {
			s_FakeDepth2D = RenderTarget2D::Create(1, 1, 0, RenderTarget::InternalType::FLOAT, true, "__FakeDepth2d__");
		}
		if (s_FakeDepthCubemap == nullptr) {
			s_FakeDepthCubemap = RenderTargetCubemap::Create(1, 1, 0, RenderTarget::InternalType::FLOAT, "__FakeDepthCubemap__");
		}
	}
}