#include "tarapch.h"
#include "Tara/Core/Application.h"
#include "Tara/Input/Input.h"
#include <iostream>

#include "Tara/Renderer/RenderCommand.h"

//TODO: Remove from this file
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Tara {

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
		m_Scene = SceneRef();	//manually cause these to be replaced
		m_Window = WindowRef();	//early, so they exit first!
		LOG_S(INFO) << "Application Shutting Down...";
		loguru::shutdown();
	}

	void Application::Init(uint32_t x, uint32_t y, const std::string& title)
	{
		m_Window = Window::Create(x, y, title);
		m_Window->SetNativeEventCallback(BIND_EVENT_FN(Application::EventCallback));
		Input::Init(m_Window);

		RenderCommand::Init();

		//make a default scene
		m_Scene = std::make_shared<Scene>();
	}


	void Application::Run()
	{
		//default application main loop
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
		//update the window first, to cause all event states to update
		m_Window->OnUpdate();
		//then, update the scene
		m_Scene->Update();
	}

	void Application::PollEvents()
	{
		//first, update the Input manager
		Input::Update();
		//TODO implement this
		//Then, poll it for any events, and pass to the scene
	}

	void Application::Render()
	{
		//clear window
		RenderCommand::SetClearColor(0.0f, 0.0f, 0.0f);
		RenderCommand::Clear();
		//draw scene
		m_Scene->Draw();
		//swap buffers
		m_Window->SwapBuffers();
	}

	//TODO: remove from here and add to InputManager static class
	bool Application::EventCallback(Event& e)
	{
		if (e.GetEventClass() == EventClass::WindowClose) {
			m_Running = false;
		}
		m_Scene->OnEvent(e);
		return false;
	}


}