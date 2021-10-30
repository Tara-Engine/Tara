#pragma once
#include "tarapch.h"
//All mappings should be added when using this file
#include "Tara/Input/Mappings.h"
#include "Tara/Core/Window.h"

//TODO: add cursor mode changing (normal, disabled, hidden) 
//[https://www.glfw.org/docs/latest/input_guide.html#cursor-mode]

namespace Tara {

	//Active Input manager. Polling ONLY
	
	/// <summary>
	/// Input is an active input manager, for polling 
	/// </summary>
	class Input {
	public:
		/// <summary>
		/// Singleton Getter
		/// </summary>
		/// <returns>a raw pointer to the single Input instance</returns>
		static Input* Get() {
			static std::unique_ptr<Input> input = Input::Create();
			return &(*input); //hacky way of getting non-owning raw pointer for unique pointer
		}
		

		//TODO: replace the Init function with a function to add a window.
		/// <summary>
		/// Initialize the Input manager with a window. 
		/// </summary>
		/// <param name="ref"></param>
		inline static void Init(WindowRef ref) {
			Get()->Initialize(ref);
		}
		/// <summary>
		/// Update the Input Manager. 
		/// Automatically called from the Application automatically every frame.
		/// </summary>
		inline static void Update() {
			Get()->m_PrevMousePos = Get()->GetMousePos();
		}
		
		//singleton stuff
		Input(Input const&) = delete;
		void operator=(Input const&) = delete;

		virtual ~Input() {
			LOG_S(INFO) << "Input Object Destroyed!";
		}

		/// <summary>
		/// Get if a specific key is currently presed
		/// </summary>
		/// <param name="key">the key value. Use Terra macros</param>
		/// <returns>boolean true if pressed</returns>
		virtual bool IsKeyPressed(int32_t key) const = 0;
		/// <summary>
		/// Get if a mouse button is currently pressed
		/// </summary>
		/// <param name="button">the button value. Use Terra Macros</param>
		/// <returns>boolean true if pressed</returns>
		virtual bool IsMouseDown(int32_t button) const = 0;
		/// <summary>
		/// Get the current mouse pos as a pair
		/// </summary>
		/// <returns>mouse pos as a pair</returns>
		virtual glm::vec2 GetMousePos() const = 0;
		/// <summary>
		/// Get the current mouse X pos
		/// </summary>
		/// <returns>current mouse X pos as unsigned int</returns>
		inline float GetMouseX() const { auto p = GetMousePos(); return p.x; }
		/// <summary>
		/// Get the current mouse Y pos
		/// </summary>
		/// <returns>current mouse Y pos as unsigned int</returns>
		inline float GetMouseY() const { auto p = GetMousePos(); return p.y; }
		/// <summary>
		/// Get the previous mouse pos as a pair
		/// </summary>
		/// <returns>previous mouse pos as a pair</returns>
		inline glm::vec2 GetPrevMousePos() const { return m_PrevMousePos; }
		/// <summary>
		/// Get the previous mouse X pos
		/// </summary>
		/// <returns>previous mouse X pos as unsigned int</returns>
		inline float GetPrevMouseX() const { return m_PrevMousePos.x; }
		/// <summary>
		/// Get the previous mouse Y pos
		/// </summary>
		/// <returns>previous mouse Y pos as unsigned int</returns>
		inline float GetPrevMouseY() const { return m_PrevMousePos.y; }
		/// <summary>
		/// Get the change in mouse position last frame (mouse delta) as a pair
		/// </summary>
		/// <returns>mouse delta as a pair</returns>
		inline glm::vec2 GetMouseDelta() const { 
			auto p = GetMousePos(); 
			return p - m_PrevMousePos;
		}
		/// <summary>
		/// Get the change in mouse X position last frame
		/// </summary>
		/// <returns>mouse delta X as a unsigned int</returns>
		inline float GetMouseDeltaX() const { auto p = GetMouseDelta(); return p.x; }
		/// <summary>
		/// Get the change in mouse Y position last frame
		/// </summary>
		/// <returns>mouse delta Y as a unsigned int</returns>
		inline float GetMouseDeltaY() const { auto p = GetMouseDelta(); return p.y; }

		//TODO: add gamepad stuff here. Eventually
	protected:
		virtual void Initialize(WindowRef ref) = 0;
		Input() {
			LOG_S(INFO) << "Input Object Created!";
		}
	
	private:
		/// <summary>
		/// creation of implementation-specific version
		/// implemented in platform-specific file
		/// </summary>
		/// <returns>a unique pointer to the inplementation-specific object</returns>
		static std::unique_ptr<Input> Create(); 
	
	private:
		/// <summary>
		/// a pair to store prev mouse pos
		/// </summary>
		glm::vec2 m_PrevMousePos;
	};

}