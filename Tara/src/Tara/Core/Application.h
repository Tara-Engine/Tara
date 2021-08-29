#pragma once
#include "Tara/Core/Window.h"
#include "Tara/Core/Scene.h"

//TEMP
#include "Tara/Renderer/Shader.h"
#include "Tara/Renderer/Buffer.h"
#include "Tara/Renderer/VertexArray.h"

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

		inline bool IsRunning() const { return m_Running; }

		inline const WindowRef& GetWindow() const { return m_Window; }
		inline const SceneRef& GetScene() const { return m_Scene; }

		bool EventCallback(Event& e);
	private:
		bool m_Running;
		WindowRef m_Window;
		SceneRef m_Scene;
	};

}