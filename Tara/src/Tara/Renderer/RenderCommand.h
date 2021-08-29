#pragma once
#include "Tara/Renderer/Renderer.h"
#include "Tara/Renderer/VertexArray.h"

namespace Tara {
	/*
	The RenderCommand class is a (mostly?) static class, providing an interface to a private 
	PlatformRenderCommand instance. Unique Pointer.
	*/
	class RenderCommand {
	public:
		//static stuff
		static void Init();

		inline static void SetClearColor(glm::vec3 color) { s_RC->ISetClearColor(color.r, color.g, color.b); }
		inline static void SetClearColor(glm::vec4 color) { s_RC->ISetClearColor(color.r, color.g, color.b); }
		inline static void SetClearColor(float r, float g, float b) { s_RC->ISetClearColor(r, g, b); }
		inline static void Clear() { s_RC->IClear(); }
		inline static void Draw(VertexArrayRef vertexArray) { s_RC->IDraw(vertexArray); }

	protected:
		//interface stuff for platform-specific subclasses
		virtual void ISetClearColor(float r, float g, float b) = 0;
		virtual void IClear() = 0;
		virtual void IDraw(VertexArrayRef vertexArray) = 0;

	private:

		static void GLError(
			uint32_t source,
			uint32_t type,
			uint32_t id,
			uint32_t severity,
			int32_t length,
			const char* message,
			const void* userParam
		);

	private:
		static std::unique_ptr<RenderCommand> s_RC;
	};

}