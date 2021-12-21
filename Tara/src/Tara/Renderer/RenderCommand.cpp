#include "tarapch.h"
#include "RenderCommand.h"
#include "Tara/Renderer/Renderer.h"
#include "Tara/Renderer/VertexArray.h"
#include "Tara/Renderer/Bindable.h"

//platform-specific includes
//REMEMBER: this SHOULD be wrapped in #ifdefs and whatnot!
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Platform/OpenGL/OpenGLRenderCommand.h"
namespace Tara {
	//default::uninitialized pointer
	std::unique_ptr<RenderCommand> RenderCommand::s_RC = std::unique_ptr<RenderCommand>();
	std::list<RenderCommand::DrawType> RenderCommand::m_DrawTypeStack;

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
		m_DrawTypeStack.push_front({ RenderDrawType::Triangles, false });
	}

	void RenderCommand::PushDrawType(RenderDrawType drawType, bool wireframe)
	{
		DrawType t = { drawType, wireframe };
		if (drawType == RenderDrawType::Keep) {
			t.Type = m_DrawTypeStack.begin()->Type;
		}
		m_DrawTypeStack.push_front(t);
		s_RC->ISetDrawType(drawType, wireframe);
	}

	void RenderCommand::PopDrawType()
	{
		m_DrawTypeStack.pop_front();
		if (m_DrawTypeStack.size() > 0){
			auto t = *(m_DrawTypeStack.begin());
			s_RC->ISetDrawType(t.Type, t.WireFrame);
		}
	}

	void RenderCommand::Bind(BindableRef ref, int a, int b)
	{
		ref->ImplBind(a, b);
	}




	
}