#pragma once
#include "tarapch.h"
#include "Tara/Input/Event.h"



namespace Tara {
	/// <summary>
	/// Window class is a platform-independant window interface
	/// </summary>
	class Window {
		/// <summary>
		/// Window reference
		/// </summary>
		using WindowRef = std::shared_ptr<Window>;
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {};
		/// <summary>
		/// Get the width of the window
		/// </summary>
		/// <returns>the width as unsigned int</returns>
		virtual uint32_t GetWidth() = 0;
		/// <summary>
		/// Get the height of the window
		/// </summary>
		/// <returns></returns>
		virtual uint32_t GetHeight() = 0;
		/// <summary>
		/// Update the window
		/// </summary>
		virtual void OnUpdate() = 0;
		/// <summary>
		/// Set the event callback function
		/// </summary>
		/// <param name="callback">TATA_BIND_FN(ClassName::FunctionName) function</param>
		virtual void SetNativeEventCallback(const EventCallbackFn& callback) = 0;
		/// <summary>
		/// Set the VSync enabled/disabled
		/// </summary>
		/// <param name="enabled">bool true to enable VSync, false to disable</param>
		virtual void SetVSync(bool enabled) = 0;
		/// <summary>
		/// Get if VSync is enabled
		/// </summary>
		/// <returns>true if VSync is on, false otherwise</returns>
		virtual bool GetVSync() = 0;
		/// <summary>
		/// Get a void pointer to native window. Used for platform-dependant stuff
		/// </summary>
		/// <returns>void* pointer</returns>
		virtual void* GetNativeWindow() const = 0; //native window reference
		/// <summary>
		/// Swap the window buffers
		/// </summary>
		virtual void SwapBuffers() = 0;
		
		/// <summary>
		/// Create a Window
		/// implment in platform-dependant window class
		/// </summary>
		/// <param name="width">window width</param>
		/// <param name="height">window height</param>
		/// <param name="title">window title</param>
		/// <returns>Window reference</returns>
		static WindowRef Create(uint32_t width, uint32_t height, const std::string& title); 
	};

	/// <summary>
	/// Reference to a Window class is a platform-independant window interface
	/// </summary>
	using WindowRef = std::shared_ptr<Window>;
}