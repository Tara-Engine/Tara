#include "tarapch.h"
#include "RenderCommand.h"
#include "Tara/Renderer/Renderer.h"
#include "Tara/Renderer/VertexArray.h"
#include "Tara/Renderer/Bindable.h"
#include "Tara/Renderer/Texture.h"

//platform-specific includes
//REMEMBER: this SHOULD be wrapped in #ifdefs and whatnot!
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Platform/OpenGL/OpenGLRenderCommand.h"
namespace Tara {
	//default::uninitialized pointer
	std::unique_ptr<RenderCommand> RenderCommand::s_RC = std::unique_ptr<RenderCommand>();
	std::list<RenderCommand::DrawType> RenderCommand::s_DrawTypeStack;
	std::vector<RenderCommand::Command> RenderCommand::s_CommandQueue;
	bool RenderCommand::s_EnqueingCommands = false;

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
	}

	void RenderCommand::ExecuteQueue()
	{
		s_EnqueingCommands = false; //MUST do first, or some will try and enqueue while running this
		for (auto& command : s_CommandQueue) {
			switch (command.Type) {
			case CommandType::CLEAR : {
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
		auto size = s_CommandQueue.size();
		s_CommandQueue.clear();
		s_CommandQueue.reserve(size);
	}

	void RenderCommand::Clear()
	{
		if (s_EnqueingCommands) {
			Command c{ CommandType::CLEAR };
			s_CommandQueue.push_back(c);
		}
		else {
			s_RC->IClear();
		}
	}

	void RenderCommand::Draw(VertexArrayRef vertexArray)
	{
		if (s_EnqueingCommands) {
			Command c{ CommandType::DRAW, CommandFormDraw{vertexArray} };
			s_CommandQueue.push_back(c);
		}
		else{ 
			s_RC->IDraw(vertexArray); 
		}
	}

	void RenderCommand::DrawCount(uint32_t count)
	{
		if (s_EnqueingCommands) {
			Command c{ CommandType::DRAW_COUNT, CommandFormDrawCount{count} };
			s_CommandQueue.push_back(c);
		}
		else { 
			s_RC->IDrawCount(count); 
		}
	}

	void RenderCommand::SetClearColor(float r, float g, float b)
	{
		if (s_EnqueingCommands) {
			Command c{ CommandType::SET_CLEAR_COLOR, CommandFormSetClearColor{r,g,b} };
			s_CommandQueue.push_back(c);
		}else { 
			s_RC->ISetClearColor(r, g, b); 
		}
	}

	void RenderCommand::PushDrawType(RenderDrawType drawType, bool wireframe)
	{
		if (s_EnqueingCommands) {
			Command c{ CommandType::PUSH_DRAW_TYPE , DrawType{drawType, wireframe} };
			s_CommandQueue.push_back(c);
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
			s_CommandQueue.push_back(c);
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
			s_CommandQueue.push_back(c);
		}else { 
			s_RC->IEnableDepthTesting(enable); 
		}
	}

	void RenderCommand::EnableBackfaceCulling(bool enable)
	{
		if (s_EnqueingCommands) {
			Command c{ CommandType::ENABLE_BACKFACE_CULLING, CommandFormBool{enable} };
			s_CommandQueue.push_back(c);
		}
		else { 
			s_RC->IEnableBackfaceCulling(enable); 
		}
	}

	void RenderCommand::Bind(BindableRef ref, int a, int b)
	{
		if (s_EnqueingCommands) {
			Command c{ CommandType::BIND, CommandFormBind{ref, a, b} };
			s_CommandQueue.push_back(c);
		}else{
			ref->ImplBind(a, b);
		}
	}

	void RenderCommand::SendUniform(ShaderRef shader, const std::string& name, const Uniform& data)
	{
		if (s_EnqueingCommands) {
			Command c{ CommandType::UNIFORM, CommandFormUniform{shader, name, data} };
			s_CommandQueue.push_back(c);
		}else{
			shader->ImplSend(name, data);
		}
	}

	void RenderCommand::RenderToTartet(RenderTargetRef ref, bool render)
	{
		if (s_EnqueingCommands) {
			Command c{ CommandType::RENDER_TO_TARGET, CommandFormRenderToTarget{ref, render} };
			s_CommandQueue.push_back(c);
		}
		else {
			ref->ImplRenderTo(render);
		}
	}

	
	

}