#include "tarapch.h"
#include "RenderCommand.h"

//platform-specific includes
//REMEMBER: this SHOULD be wrapped in #ifdefs and whatnot!
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Platform/OpenGL/OpenGLRenderCommand.h"
namespace Tara {
	//default::uninitialized pointer
	std::unique_ptr<RenderCommand> RenderCommand::s_RC = std::unique_ptr<RenderCommand>();
	
	//RenderCommand inititalization
	void RenderCommand::Init()
	{
		switch (Renderer::GetRenderBackend()) {
		case RenderBackend::OpenGl: {
			//OpenGL context stuff
			gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(&OpenGLRenderCommand::GLError, (void*)0);

			//TODO: add opengl ponter creation
			s_RC = std::make_unique<OpenGLRenderCommand>();

			break;
		}
		case RenderBackend::None: ABORT_F("Renderbackend::None not supported!");
		}
	}




	
}