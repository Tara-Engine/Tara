#include "tarapch.h"
#include "Tara/Renderer/Renderer.h"
#include "Tara/Renderer/RenderCommand.h"
//#include "Tara/Renderer/VertexArray.h"
//#include "Tara/Renderer/Shader.h"
//#include "Tara/Math/Types.h"

namespace Tara {
	//default to OpenGL renderbackend
	RenderBackend Renderer::s_RenderBackend = RenderBackend::OpenGl;
	Renderer::SceneData Renderer::s_SceneData = { nullptr };

	void Renderer::BeginScene(const CameraRef camera)
	{
		s_SceneData.camera = camera;
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
}