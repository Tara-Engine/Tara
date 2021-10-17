#pragma once
#include "Tara/Core/Window.h"
#include "Tara/Core/Scene.h"

//TEMP
#include "Tara/Renderer/Shader.h"
#include "Tara/Renderer/Buffer.h"
#include "Tara/Renderer/VertexArray.h"

namespace Tara {


	struct AfterCallable;


	/// <summary>
	/// Application Class (Singleton)
	/// </summary>
	class Application {
	private:
		/// <summary>
		/// Private Constructor.
		/// </summary>
		Application();
	
	public:
		/// <summary>
		/// Get the singleton Application
		/// </summary>
		/// <returns>Application pointer</returns>
		static Application* Get();
		//singleton stuff
		Application(Application const&) = delete;
		void operator=(Application const&) = delete;

		virtual ~Application();

		/// <summary>
		/// Initialize the application and open a window
		/// </summary>
		/// <param name="x">Window width</param>
		/// <param name="y">Window Height</param>
		/// <param name="title">Window title</param>
		void Init(uint32_t x, uint32_t y, const std::string& title);

		/// <summary>
		/// Run the application main loop
		/// </summary>
		void Run(); //NOT virtual!

		/// <summary>
		/// Update step
		/// </summary>
		void Update(float deltaTime);

		/// <summary>
		/// Poll events step
		/// </summary>
		void PollEvents();

		/// <summary>
		/// Render step
		/// </summary>
		void Render(float deltaTime);

		/// <summary>
		/// Check if application is running
		/// </summary>
		/// <returns></returns>
		inline bool IsRunning() const { return m_Running; }

		/// <summary>
		/// Get the internal Window Reference
		/// </summary>
		/// <returns>Window Reference</returns>
		inline const WindowRef& GetWindow() const { return m_Window; }
		/// <summary>
		/// Get the internal Scene Reference
		/// </summary>
		/// <returns>Scene Reference</returns>
		inline const SceneRef& GetScene() const { return m_Scene; }

		/// <summary>
		/// Event callback function
		/// </summary>
		/// <param name="e">event</param>
		/// <returns>if the event is handled</returns>
		bool EventCallback(Event& e);

		/// <summary>
		/// Get the game delta time
		/// </summary>
		/// <returns>delta time, in seconds</returns>
		inline float GetDeltaTime() const { return m_DeltaTime; }

		void After(AfterCallable* c);

	private:
		bool m_Running;
		WindowRef m_Window;
		SceneRef m_Scene;
		float m_LastFrameTime;
		float m_DeltaTime;
		std::list<AfterCallable*> m_AfterCallableList;
	};

}