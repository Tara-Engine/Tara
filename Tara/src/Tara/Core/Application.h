#pragma once
#include "Tara/Core/Window.h"

namespace Tara {

	class Application {
	private:
		Application();
	
	public:
		static Application* Get() {
			static Application app;
			return &app;
		}
		//singleton stuff
		Application(Application const&) = delete;
		void operator=(Application const&) = delete;

		virtual ~Application();

		void Init(uint32_t x, uint32_t y, const std::string& title);

		void Run(); //NOT virtual!

		void Update();
		void PollEvents();
		void Render();

		bool EventCallback(Event& e);
	protected:
		bool m_Running;
		WindowRef m_Window;
	};

}