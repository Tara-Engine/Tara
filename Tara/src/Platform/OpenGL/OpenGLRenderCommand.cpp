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

	inline void OpenGLRenderCommand::GLError(
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
