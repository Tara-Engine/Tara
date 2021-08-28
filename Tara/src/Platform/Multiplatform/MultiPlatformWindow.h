#pragma once
#include "Tara/Core/Window.h"


struct GLFWwindow;

namespace Tara {

	class MultiPlatformWindow : public Window {
	public:
		MultiPlatformWindow(uint32_t width, uint32_t height, const std::string& title);
		virtual ~MultiPlatformWindow() override;

		virtual uint32_t GetWidth() override { return m_Data.Width; }
		virtual uint32_t GetHeight() override { return m_Data.Height; }
		virtual void OnUpdate() override;
		virtual void SetNativeEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		virtual void SetVSync(bool enabled) override;
		virtual bool GetVSync() override { return m_Data.VSync; }
		virtual void* GetNativeWindow() const override { return m_WindowHandle; }

	private:
		GLFWwindow* m_WindowHandle;
		
		//this is done so all data can be passed as a blob to the GLFW window events via pointer.
		struct WindowData {
			std::string Title;
			uint32_t Width, Height;
			bool VSync;
			EventCallbackFn EventCallback;
		};
		
		WindowData m_Data;
	};

}