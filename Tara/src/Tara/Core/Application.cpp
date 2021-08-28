#include "tarapch.h"
#include "Tara/Core/Application.h"
#include <iostream>

namespace Tara {
	Application::Application()
		: m_Running(true)
	{
		//loguru::init();
		//Initialize loguru. MUST happen relatively early, thus, first time application is initialized.
		loguru::add_file("log_all.txt", loguru::Append, loguru::Verbosity_MAX);
		loguru::add_file("log.txt", loguru::Truncate, loguru::Verbosity_INFO);
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
	}


	void Application::Run()
	{
		LOG_S(INFO) << "Test log!";

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
	}

	void Application::Render()
	{
		//TODO implement this
	}

	bool Application::EventCallback(Event& e)
	{
		//TODO: remove from here and add to InputManager static class
		if (e.GetEventClass() == EventClass::WindowClose) {
			m_Running = false;
		}
		LOG_S(INFO) << e;
		return false;
	}


}