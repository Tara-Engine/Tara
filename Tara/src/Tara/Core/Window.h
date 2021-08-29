#pragma once
#include "tarapch.h"
#include "Tara/Input/Event.h"



namespace Tara {

	//platform-independant window interface
	class Window {
		using WindowRef = std::shared_ptr<Window>;
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {};
		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetHeight() = 0;
		virtual void OnUpdate() = 0;
		virtual void SetNativeEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool GetVSync() = 0;
		virtual void* GetNativeWindow() const = 0; //native window reference
		virtual void SwapBuffers() = 0;

		//implment in platform-dependant window class
		static WindowRef Create(uint32_t width, uint32_t height, const std::string& title); 
	};

	using WindowRef = std::shared_ptr<Window>;
}