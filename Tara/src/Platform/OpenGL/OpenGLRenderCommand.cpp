#include "tarapch.h"
#include "OpenGLRenderCommand.h"

#include "glad/glad.h"
namespace Tara{
	void OpenGLRenderCommand::ISetClearColor(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);
	}

	void OpenGLRenderCommand::IClear()
	{
		glClear(GL_COLOR_BUFFER_BIT); //TODO: Add depth buffer and everything else needed!
	}

	void OpenGLRenderCommand::IDraw(VertexArrayRef vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
	}


}
