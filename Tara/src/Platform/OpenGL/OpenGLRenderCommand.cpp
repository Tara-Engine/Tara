#include "tarapch.h"
#include "OpenGLRenderCommand.h"
#include "Tara/Renderer/VertexArray.h"

#include "glad/glad.h"

//remove this, make window independant!!
#include "GLFW/glfw3.h"
namespace Tara{
	OpenGLRenderCommand::OpenGLRenderCommand()
		: m_DrawMode(GL_TRIANGLES)
	{
		//load glad functions
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		LOG_S(INFO) << "OpenGL Loaded:";
		LOG_S(INFO) << "OpenGL Version: " << glGetString(GL_VERSION);
		LOG_S(INFO) << "OpenGL Renderer: " << glGetString(GL_RENDERER);
		LOG_S(INFO) << "OpenGL Vendor: " << glGetString(GL_VENDOR);

		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(&OpenGLRenderCommand::GLError, (void*)0);
		//blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRenderCommand::ISetClearColor(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);
	}

	void OpenGLRenderCommand::ISetDrawType(RenderDrawType drawType, bool wireframe)
	{
		switch (drawType) {
			case RenderDrawType::Keep:			{break; }//do nothing
			case RenderDrawType::Points:		{m_DrawMode = GL_POINTS; break; }
			case RenderDrawType::Lines:			{m_DrawMode = GL_LINES; break; }
			case RenderDrawType::Triangles:		{m_DrawMode = GL_TRIANGLES; break; }
			case RenderDrawType::LineStrip:		{m_DrawMode = GL_LINE_STRIP; break; }
			case RenderDrawType::LineLoop:		{m_DrawMode = GL_LINE_LOOP; break; }
			case RenderDrawType::TriangleStrip: {m_DrawMode = GL_TRIANGLE_STRIP; break; }
			case RenderDrawType::TriangleFan:	{m_DrawMode = GL_TRIANGLE_FAN; break; }
		}
		if (wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	void OpenGLRenderCommand::IClear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //TODO: Add depth buffer and everything else needed!
	}

	void OpenGLRenderCommand::IDraw(VertexArrayRef vertexArray)
	{
		glDrawElements(m_DrawMode, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
	}

	void OpenGLRenderCommand::IDrawCount(uint32_t count)
	{
		glDrawArrays(m_DrawMode, 0, count);
	}

	uint32_t OpenGLRenderCommand::IGetMaxTextureSlotsPerShader()
	{
		int32_t textureUnits;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
		return (uint32_t)textureUnits;
	}

	void OpenGLRenderCommand::IEnableDepthTesting(bool enable)
	{
		if (enable) {
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
		}
		else {
			glDisable(GL_DEPTH_TEST);
		}
	}

	void OpenGLRenderCommand::IEnableBackfaceCulling(bool enable)
	{
		if (enable) {
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CW); //This is to match DirectX!
		}
		else {
			glDisable(GL_CULL_FACE);
		}
	}

	void OpenGLRenderCommand::GLError(
		uint32_t source,
		uint32_t type,
		uint32_t id,
		uint32_t severity,
		int32_t length,
		const char* message,
		const void* userParam
	) {
		int level = 9;
		switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:			level = loguru::Verbosity_ERROR; break;
		case GL_DEBUG_SEVERITY_MEDIUM:			level = loguru::Verbosity_WARNING; break;
		case GL_DEBUG_SEVERITY_LOW:				level = loguru::Verbosity_INFO; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:	level = loguru::Verbosity_2; break;
		}

		std::stringstream ss;
		ss << "OpenGL Error: Source:[";
		switch (source) {
		case GL_DEBUG_SOURCE_API: {ss << "API usage"; break; }
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: {ss << "Window System"; break; }
		case GL_DEBUG_SOURCE_SHADER_COMPILER: {ss << "Shader Compiler"; break; }
		case GL_DEBUG_SOURCE_THIRD_PARTY: {ss << "third-party"; break; }
		case GL_DEBUG_SOURCE_APPLICATION: {ss << "Application-generated"; break; }
		case GL_DEBUG_SOURCE_OTHER: {ss << "Unknown"; break; }
		}
		ss << "] Type:[";
		switch (type) {
		case GL_DEBUG_TYPE_ERROR: {ss << "API error"; break; }
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: {ss << "Depricated Behavior"; break; }
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: {ss << "Undefined Behavior"; break; }
		case GL_DEBUG_TYPE_PORTABILITY: {ss << "non-portable code"; break; }
		case GL_DEBUG_TYPE_PERFORMANCE: {ss << "possible performance issue"; break; }
		case GL_DEBUG_TYPE_MARKER: {ss << "stream annotation"; break; }
		case GL_DEBUG_TYPE_PUSH_GROUP: {ss << "Group Push"; break; }
		case GL_DEBUG_TYPE_POP_GROUP: {ss << "Group Pop"; break; }
		case GL_DEBUG_TYPE_OTHER: {ss << "Unknown"; break; }
		}
		ss << "] ID: (" << id << ")[";
		switch (id) {
		case GL_NO_ERROR: {ss << "No Error?"; break; }
		case GL_INVALID_ENUM: {ss << "Invalid Enum"; break; }
		case GL_INVALID_VALUE: {ss << "Invalid Value"; break; }
		case GL_INVALID_OPERATION: {ss << "Invalid Operation"; break; }
		case GL_INVALID_FRAMEBUFFER_OPERATION: {ss << "Invalid Framebuffer Operation"; break; }
		case GL_OUT_OF_MEMORY: {ss << "Out of Memory"; break; }
		case GL_STACK_OVERFLOW: {ss << "Stack Overflow"; break; }
		case GL_STACK_UNDERFLOW: {ss << "Stack Underflow"; break; }
		default: {ss << "Not an Error"; }
		}
		ss << "] Message: ";
		VLOG_S(level) << ss.str() << message;
	}

}
