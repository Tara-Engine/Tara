#include "tarapch.h"
#include "Tara/Core/Application.h"
#include "Tara/Input/Input.h"
#include <iostream>

#include "Tara/Renderer/RenderCommand.h"
#include "Tara/Core/Timer.h"

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

	Application* Application::Get()
	{
		static Application app;
		return &app;
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
		m_Window->SetNativeEventCallback(TARA_BIND_FN(Application::EventCallback));
		Input::Init(m_Window);

		RenderCommand::Init();
		RenderCommand::SetClearColor(0.0f, 0.0f, 0.0f); //Default value
		//make a default scene
		m_Scene = std::make_shared<Scene>();
	}


	void Application::Run()
	{
		//default application main loop
		while (m_Running) {
			try {
				//get deltaTime 
				float time = m_Window->GetLastFrameTime();
				m_DeltaTime = time - m_LastFrameTime;
				m_LastFrameTime = time;

				PollEvents();
				Update(m_DeltaTime);
				Render(m_DeltaTime);
				
			}
			catch (std::exception e) {
				std::cerr << e.what() << std::endl;
				m_Running = false;
				exit(-1);
			}
		} 
	}

	void Application::Update(float deltaTime)
	{
		//update the window first, to cause all event states to update
		m_Window->OnUpdate();
		//then, update the scene
		m_Scene->Update(deltaTime);

		//deal with after functions
		//AfterCallable* c = m_AfterCallableList;
		auto c = m_AfterCallableList.begin();
		while (c != m_AfterCallableList.end()) {
			//LOG_S(INFO) << "Update of callback list item!";
			(*c)->secondsRemaining -= deltaTime;
			if ((*c)->secondsRemaining <= 0.0f) {
				//call and remove
				(*c)->callable.get(); //async wait for. since using deferred, this will be the first time it is called.
				auto ptr = (*c);
				m_AfterCallableList.erase(c++);
				delete ptr;
			}
			else {
				++c;
			}
		}
	}

	void Application::PollEvents()
	{
		//first, update the Input manager
		Input::Update();
		//TODO implement this
		//Then, poll it for any events, and pass to the scene
	}

	void Application::Render(float deltaTime)
	{
		//clear window
		
		RenderCommand::Clear();
		//draw scene
		m_Scene->Draw(deltaTime);
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

	void Application::After(AfterCallable* c)
	{
		m_AfterCallableList.push_back(c);
	}


}