#include "tarapch.h"
#include "RenderCommand.h"
#include "Tara/Renderer/Renderer.h"
#include "Tara/Renderer/VertexArray.h"
#include "Tara/Renderer/Bindable.h"
#include "Tara/Renderer/Texture.h"
#include "Tara/Core/Application.h"
#include "Tara/Renderer/MaterialBase.h"

//platform-specific includes
//REMEMBER: this SHOULD be wrapped in #ifdefs and whatnot!
//#include "glad/glad.h"
//#include "GLFW/glfw3.h"

#include "Platform/OpenGL/OpenGLRenderCommand.h"
namespace Tara {
	//default::uninitialized pointer
	std::unique_ptr<RenderCommand> RenderCommand::s_RC = std::unique_ptr<RenderCommand>();
	std::list<RenderCommand::DrawType> RenderCommand::s_DrawTypeStack;
	std::vector<RenderCommand::Command> RenderCommand::s_CommandQueueForward;
	std::vector<RenderCommand::Command> RenderCommand::s_CommandQueueDeferred;
	bool RenderCommand::s_EnqueingCommands = false;
	bool RenderCommand::s_CurrentModeDeferred = false;
	RenderTargetRef RenderCommand::s_GBuffer = nullptr;
	VertexArrayRef RenderCommand::s_ScreenQuad = nullptr;
	MaterialBaseRef RenderCommand::s_LightingMaterial = nullptr;

	//RenderCommand inititalization
	void RenderCommand::Init()
	{
		switch (Renderer::GetRenderBackend()) {
		case RenderBackend::OpenGl: {
			//add opengl ponter creation
			s_RC = std::make_unique<OpenGLRenderCommand>();
			break;
		}
		case RenderBackend::None: ABORT_F("Renderbackend::None not supported!");
		}
		s_DrawTypeStack.push_front({ RenderDrawType::Triangles, false });
	}

	void RenderCommand::BeginQueue()
	{
		s_EnqueingCommands = true;
		s_CurrentModeDeferred = false;
	}

	void RenderCommand::ExecuteQueue(const RenderSceneData& sceneData)
	{
		s_EnqueingCommands = false; //MUST do first, or some will try and enqueue while running this
		
		
		if (s_LightingMaterial) {
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

			//render deferred queue
			for (auto& command : s_CommandQueueDeferred) {
				ExecuteCommand(command);
			}
			
			//End rendering to GBuffer, blit its depth to the passed RenderTarget
			if (sceneData.target) {
				sceneData.target->RenderTo(true);
			}
			else {
				s_GBuffer->RenderTo(false);
			}
			//render the quad
			if (!s_ScreenQuad) {
				//load the ScreenQuad
				//{0,0,0,  0, 0,-1, 1,1,1,1, 1,0}, {1,0,0,  0, 0,-1, 1,1,1,1, 0,0}, {1,1,0,  0, 0,-1, 1,1,1,1, 0,1}, {0,1,0,  0, 0,-1, 1,1,1,1, 1,1}
				static float verts[]{
					-1.0f, -1.0f, 0.0f, 0.0f, //Bottom left
					-1.0f, 1.0f,  0.0f, 1.0f,  //Bottom right
					1.0f, 1.0f,   1.0f, 1.0f,   //Top right
					1.0f, -1.0f,  1.0f, 0.0f,  //Top left
				};
				static uint32_t indices[]{
					0, 1, 2, 2, 3, 0
				};
				VertexBufferRef vb = VertexBuffer::Create(verts, sizeof(verts) / sizeof(float));
				vb->SetLayout({ 
					{Shader::Datatype::Float2, "Position", false}, 
					{Shader::Datatype::Float2, "UV", false}
				});
				IndexBufferRef ib = IndexBuffer::Create(indices, 6);
				s_ScreenQuad = VertexArray::Create();
				s_ScreenQuad->AddVertexBuffer(vb);
				s_ScreenQuad->SetIndexBuffer(ib);
			}

			//render the screen quad with light material
			s_LightingMaterial->Use();
			/*
			uniform sampler2D u_ColorMetallicSampler;
			uniform sampler2D u_SpecularRoughnessSampler;
			uniform sampler2D u_EmissiveAOSampler;
			uniform sampler2D u_WorldSpaceNormalSampler;
			uniform sampler2D u_WorldSpacePositionSampler;
			*/
			auto& shader = s_LightingMaterial->GetShader();
			
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
			//these might be optimized away
			if (shader->ValidUniform("u_CameraPositionWS")) {
				shader->Send("u_CameraPositionWS", sceneData.camera->GetPosition());
			}
			if (shader->ValidUniform("u_CameraForwardVector")) {
				shader->Send("u_CameraForwardVector", sceneData.camera->GetRotation().GetForwardVector());
			}
			
			s_ScreenQuad->ImplBind(0,0); //non-cached version
			RenderCommand::Draw(s_ScreenQuad);

			//copy depth
			s_GBuffer->BlitDepthToOther(sceneData.target);

		}
		else {
			LOG_S(WARNING) << "No lighting material given to RenderCommand, thus, no deferred rendering will take place";
			if (sceneData.target) {
				sceneData.target->RenderTo(true);
			}
		}

		//execute the forward queue
		for (auto& command : s_CommandQueueForward) {
			ExecuteCommand(command);
		}

		//clear the queues, but leave the size
		auto size = s_CommandQueueForward.size();
		s_CommandQueueForward.clear();
		s_CommandQueueForward.reserve(size);

		size = s_CommandQueueDeferred.size();
		s_CommandQueueDeferred.clear();
		s_CommandQueueDeferred.reserve(size);

		//unset the target RenderTarget
		//if target is nullptr, then it should be fine.
		if (sceneData.target){
			sceneData.target->RenderTo(false);
		}
	}

	void RenderCommand::Clear()
	{
		if (s_EnqueingCommands) {
			Command c{ CommandType::CLEAR };
			PushCommand(c);
		}
		else {
			s_RC->IClear();
		}
	}

	void RenderCommand::Draw(VertexArrayRef vertexArray)
	{
		if (s_EnqueingCommands) {
			Command c{ CommandType::DRAW, CommandFormDraw{vertexArray} };
			PushCommand(c);
		}
		else{ 
			s_RC->IDraw(vertexArray); 
		}
	}

	void RenderCommand::DrawCount(uint32_t count)
	{
		if (s_EnqueingCommands) {
			Command c{ CommandType::DRAW_COUNT, CommandFormDrawCount{count} };
			PushCommand(c);
		}
		else { 
			s_RC->IDrawCount(count); 
		}
	}

	void RenderCommand::SetClearColor(float r, float g, float b)
	{
		if (s_EnqueingCommands) {
			Command c{ CommandType::SET_CLEAR_COLOR, CommandFormSetClearColor{r,g,b} };
			PushCommand(c);
		}else { 
			s_RC->ISetClearColor(r, g, b); 
		}
	}

	void RenderCommand::PushDrawType(RenderDrawType drawType, bool wireframe)
	{
		if (s_EnqueingCommands) {
			Command c{ CommandType::PUSH_DRAW_TYPE , DrawType{drawType, wireframe} };
			PushCommand(c);
		}
		else {
			DrawType t = { drawType, wireframe };
			if (drawType == RenderDrawType::Keep) {
				t.Type = s_DrawTypeStack.begin()->Type;
			}
			s_DrawTypeStack.push_front(t);
			s_RC->ISetDrawType(drawType, wireframe);
		}
	}

	void RenderCommand::PopDrawType()
	{
		if (s_EnqueingCommands) {
			Command c{ CommandType::POP_DRAW_TYPE };
			PushCommand(c);
		}
		else {
			s_DrawTypeStack.pop_front();
			if (s_DrawTypeStack.size() > 0){
				auto t = *(s_DrawTypeStack.begin());
				s_RC->ISetDrawType(t.Type, t.WireFrame);
			}
		}
	}

	void RenderCommand::EnableDepthTesting(bool enable)
	{
		if (s_EnqueingCommands) {
			Command c{ CommandType::ENABLE_DEPTH_TEST, CommandFormBool{enable} };
			PushCommand(c);
		}else { 
			s_RC->IEnableDepthTesting(enable); 
		}
	}

	void RenderCommand::EnableBackfaceCulling(bool enable)
	{
		if (s_EnqueingCommands) {
			Command c{ CommandType::ENABLE_BACKFACE_CULLING, CommandFormBool{enable} };
			PushCommand(c);
		}
		else { 
			s_RC->IEnableBackfaceCulling(enable); 
		}
	}

	void RenderCommand::Bind(BindableRef ref, int a, int b)
	{
		if (s_EnqueingCommands) {
			Command c{ CommandType::BIND, CommandFormBind{ref, a, b} };
			PushCommand(c);
		}else{
			ref->ImplBind(a, b);
		}
	}

	void RenderCommand::SendUniform(ShaderRef shader, const std::string& name, const Uniform& data)
	{
		if (s_EnqueingCommands) {
			Command c{ CommandType::UNIFORM, CommandFormUniform{shader, name, data} };
			PushCommand(c);
		}else{
			shader->ImplSend(name, data);
		}
	}

	void RenderCommand::RenderToTartet(RenderTargetRef ref, bool render)
	{
		if (s_EnqueingCommands) {
			Command c{ CommandType::RENDER_TO_TARGET, CommandFormRenderToTarget{ref, render} };
			PushCommand(c);
		}
		else {
			ref->ImplRenderTo(render);
		}
	}

	
	

	void RenderCommand::PushCommand(Command& c)
	{
		if (s_CurrentModeDeferred) {
			s_CommandQueueDeferred.push_back(c);
		}
		else {
			s_CommandQueueForward.push_back(c);
		}
	}

	void RenderCommand::ExecuteCommand(const Command& command)
	{
		switch (command.Type) {
		case CommandType::CLEAR: {
			s_RC->IClear();
			break;
		}
		case CommandType::DRAW: {
			s_RC->IDraw(std::get<CommandFormDraw>(command.Params).VertexArray);
			break;
		}
		case CommandType::DRAW_COUNT: {
			s_RC->IDrawCount(std::get<CommandFormDrawCount>(command.Params).Count);
			break;
		}
		case CommandType::SET_CLEAR_COLOR: {
			auto color = std::get<CommandFormSetClearColor>(command.Params);
			s_RC->ISetClearColor(color.r, color.g, color.b);
			break;
		}
		case CommandType::PUSH_DRAW_TYPE: {
			auto p = std::get<DrawType>(command.Params);
			PushDrawType(p.Type, p.WireFrame);
			break;
		}
		case CommandType::POP_DRAW_TYPE: {
			PopDrawType();
			break;
		}
		case CommandType::ENABLE_DEPTH_TEST: {
			s_RC->IEnableDepthTesting(std::get<CommandFormBool>(command.Params).Enable);
			break;
		}
		case CommandType::ENABLE_BACKFACE_CULLING: {
			s_RC->IEnableBackfaceCulling(std::get<CommandFormBool>(command.Params).Enable);
			break;
		}
		case CommandType::BIND: {
			auto p = std::get<CommandFormBind>(command.Params);
			p.Bindable->ImplBind(p.a, p.b);
			break;
		}
		case CommandType::UNIFORM: {
			auto p = std::get<CommandFormUniform>(command.Params);
			p.Shader->ImplSend(p.Name, p.uniform);
			break;
		}
		case CommandType::RENDER_TO_TARGET: {
			auto p = std::get<CommandFormRenderToTarget>(command.Params);
			p.Target->ImplRenderTo(p.Render);
			break;
		}
		}
	}

}