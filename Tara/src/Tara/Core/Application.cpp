#include "tarapch.h"
#include "Tara/Core/Application.h"
#include "Tara/Input/Input.h"
#include <iostream>



//TODO: Remove from this file
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Tara {
	
	
	//TODO: move to GLcontext
	void GLError(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
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
		case GL_DEBUG_SOURCE_API:				{ss << "API usage"; break; }
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		{ss << "Window System"; break; }
		case GL_DEBUG_SOURCE_SHADER_COMPILER:	{ss << "Shader Compiler"; break; }
		case GL_DEBUG_SOURCE_THIRD_PARTY:		{ss << "third-party"; break; }
		case GL_DEBUG_SOURCE_APPLICATION:		{ss << "Application-generated"; break; }
		case GL_DEBUG_SOURCE_OTHER:				{ss << "Unknown"; break; }
		}
		ss << "] Type:[";
		switch (type) {
		case GL_DEBUG_TYPE_ERROR:				{ss << "API error"; break; }
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: {ss << "Depricated Behavior"; break; }
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	{ss << "Undefined Behavior"; break; }
		case GL_DEBUG_TYPE_PORTABILITY:			{ss << "non-portable code"; break; }
		case GL_DEBUG_TYPE_PERFORMANCE:			{ss << "possible performance issue"; break; }
		case GL_DEBUG_TYPE_MARKER:				{ss << "stream annotation"; break; }
		case GL_DEBUG_TYPE_PUSH_GROUP:			{ss << "Group Push"; break; }
		case GL_DEBUG_TYPE_POP_GROUP:			{ss << "Group Pop"; break; }
		case GL_DEBUG_TYPE_OTHER:				{ss << "Unknown"; break; }
		}
		ss << "] ID: (" << id << ")[";
		switch (id) {
		case GL_NO_ERROR:						{ss << "No Error?"; break; }
		case GL_INVALID_ENUM:					{ss << "Invalid Enum"; break; }
		case GL_INVALID_VALUE:					{ss << "Invalid Value"; break; }
		case GL_INVALID_OPERATION:				{ss << "Invalid Operation"; break; }
		case GL_INVALID_FRAMEBUFFER_OPERATION:	{ss << "Invalid Framebuffer Operation"; break; }
		case GL_OUT_OF_MEMORY:					{ss << "Out of Memory"; break; }
		case GL_STACK_OVERFLOW:					{ss << "Stack Overflow"; break; }
		case GL_STACK_UNDERFLOW:				{ss << "Stack Underflow"; break; }
		default:								{ss << "Not an Error"; }
		}
		ss << "] Message: ";
		VLOG_S(level) << ss.str() << message;
	}


	Application::Application()
		: m_Running(true)
	{
		//loguru::init();
		//Initialize loguru. MUST happen relatively early, thus, first time application is initialized.
		loguru::add_file("all.log", loguru::Append, loguru::Verbosity_MAX);
		loguru::add_file("session.log", loguru::Truncate, loguru::Verbosity_INFO);
		loguru::g_stderr_verbosity = loguru::Verbosity_INFO;
		loguru::set_fatal_handler([](const loguru::Message& message) {
			throw std::runtime_error(std::string(message.prefix) + message.message);
		});

	}

	Application::~Application()
	{	
		LOG_S(INFO) << "Application Shutting Down...";
		loguru::shutdown();
	}

	void Application::Init(uint32_t x, uint32_t y, const std::string& title)
	{
		m_Window = Window::Create(x, y, title);
		m_Window->SetNativeEventCallback(BIND_EVENT_FN(Application::EventCallback));
		Input::Init(m_Window);

		//TODO: remove
		//OpenGL context stuff
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(&GLError, (void*)0);



		//create a vertex array
		m_VertexArray = VertexArray::Create();
		m_VertexArray->Bind();

		//vertex buffer
		float vertecies[3 * (3+4)] = {
			-0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.0f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		};
		m_VertexBuffer = VertexBuffer::Create(vertecies, sizeof(vertecies)/sizeof(float));
		
		m_VertexBuffer->SetLayout({
			{Shader::Datatype::Float3, "position"},
			{Shader::Datatype::Float4, "color"}
			});

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		
		
		//Index Buffer
		uint32_t indecies[3] = {
			0, 1, 2
		};
		m_IndexBuffer = IndexBuffer::Create(indecies, sizeof(indecies) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
		

		m_Shader = Shader::Create(
			Shader::SourceType::TextFiles, 
			"assets/basic.vertex.glsl", 
			"assets/basic.fragment.glsl"
		);
		m_Shader->Bind();

	}


	void Application::Run()
	{
		//LOG_S(INFO) << "Test log!";

		while (m_Running) {
			try {
				PollEvents();
				Update();
				Render();
				
			}
			catch (std::exception e) {
				std::cerr << e.what() << std::endl;
				m_Running = false;
				exit(-1);
			}
		} 
	}

	void Application::Update()
	{
		m_Window->OnUpdate();
	}

	void Application::PollEvents()
	{
		//TODO implement this
		Input::Update();
	}

	void Application::Render()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glBindVertexArray(m_VertexArrayID);
		//LOG_S(INFO) << "Before VertexArray::Bind() for draw";
		m_VertexArray->Bind();
		m_Shader->Bind();
		//uint32_t count = m_IndexBuffer->GetCount();
		glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
		
		//TODO implement this
	}

	bool Application::EventCallback(Event& e)
	{
		//TODO: remove from here and add to InputManager static class
		if (e.GetEventClass() == EventClass::WindowClose) {
			m_Running = false;
		}
		//LOG_S(INFO) << e;
		return false;
	}


}