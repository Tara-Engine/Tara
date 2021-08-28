#include "tarapch.h"
#include "Tara/Core/Application.h"
#include "Tara/Input/Input.h"
#include <iostream>

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
		LOG_S(INFO) << "Application Shutting Down...";
		loguru::shutdown();
	}

	void Application::Init(uint32_t x, uint32_t y, const std::string& title)
	{
		m_Window = Window::Create(x, y, title);
		m_Window->SetNativeEventCallback(BIND_EVENT_FN(Application::EventCallback));
		Input::Init(m_Window);
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
		if (Input::Get()->IsKeyPressed(TARA_KEY_W)) {
			LOG_S(INFO) << "W key is pressed!";
		}

		if (Input::Get()->IsMouseDown(TARA_MOUSE_BUTTON_LEFT)) {
			LOG_S(INFO) << "Left mouse button pressed!";
		}
		auto pos = Input::Get()->GetMousePos();
		if (pos.first < 100 && pos.second < 100) {
			LOG_S(INFO) << "Mouse in upper right 100x100 area!";
		}

		m_Window->OnUpdate();
	}

	void Application::PollEvents()
	{
		//TODO implement this
		Input::Update();
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
		//LOG_S(INFO) << e;
		return false;
	}


}