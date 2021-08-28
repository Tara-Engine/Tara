#pragma once
#include "Tara/Input/Input.h"
#include "Tara/Core/Window.h"

//#include "Platform/Multiplatform/MultiPlatformWindow.h"
struct GLFWwindow;

namespace Tara {

	class MultiPlatformInput : public Input {
	public:
		MultiPlatformInput();

		virtual bool IsKeyPressed(int32_t key) const override;
		virtual bool IsMouseDown(int32_t button) const override;
		//MousePos
		virtual std::pair<uint32_t, uint32_t>GetMousePos() const override;

	private:
		
		//TODO: instead of initializing, add a window to listen to!
		void Initialize(WindowRef ref) override;

	private:
		GLFWwindow* m_WindowHandle;
	};

}