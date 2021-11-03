#include "tarapch.h"
#include "MultiPlatformWindow.h"
#include "GLFW/glfw3.h"

//#include "GLFW/glfw3.h"

namespace Tara {

	static bool TARA_GLFW_INTITIALIZED = false;
	
	static void TARA_GLFW_Error_Callback(int errcode, const char* desc) {
		LOG_S(ERROR) << "GLFW ERROR[" << errcode << "]: " << desc;
	}

	MultiPlatformWindow::MultiPlatformWindow(uint32_t width, uint32_t height, const std::string& title)
	{
		m_Data.Width = width;
		m_Data.Height = height;
		m_Data.Title = title;
		m_Data.VSync = true;
		if (!TARA_GLFW_INTITIALIZED) {
			bool result = (bool)glfwInit();
			DCHECK_F(result, "GLFW failed to intialize properly!");
			glfwSetErrorCallback(TARA_GLFW_Error_Callback);
			TARA_GLFW_INTITIALIZED = true;
		}
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
		m_WindowHandle = glfwCreateWindow((int)width, (int)height, title.c_str(), NULL, NULL);
		DCHECK_NOTNULL_F(m_WindowHandle, "GLFW Window failed to be created properly!");
		glfwMakeContextCurrent(m_WindowHandle);
		glfwSetWindowUserPointer(m_WindowHandle, &m_Data);
		SetVSync(true);

		//Create Event Handles.....

		//window closed:
		glfwSetWindowCloseCallback(m_WindowHandle, [](GLFWwindow* window) 
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent e;
				data.EventCallback(e);
			});

		glfwSetWindowSizeCallback(m_WindowHandle, [](GLFWwindow* window, int width, int height)
			{
				//TODO: move to Renderer
				glViewport(0, 0, width, height);
				
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;
				WindowResizeEvent e(width, height);
				data.EventCallback(e);
			});

		glfwSetKeyCallback(m_WindowHandle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				//KeyPressedEvent / KeyReleasedEvent
				int repeat = 0;
				switch (action) {
				case GLFW_REPEAT: {
					repeat = 1;
				}
				// fallthrough
				case GLFW_PRESS: {
					KeyPressEvent e(key, mods, repeat);
					data.EventCallback(e);
					break;
				}
				case GLFW_RELEASE: {
					KeyReleaseEvent e(key, mods);
					data.EventCallback(e);
					break;
				}
				}
			});

		glfwSetCharCallback(m_WindowHandle, [](GLFWwindow* window, unsigned int character)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				//KeyTypedEvent
				KeyTypeEvent e(character);
				data.EventCallback(e);
			});

		glfwSetMouseButtonCallback(m_WindowHandle, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				//MousePressedEvent / MouseReleasedEvent
				switch (action) {
				case GLFW_PRESS : {
					MouseButtonPressEvent e(button);
					data.EventCallback(e);
					break;
				}
				case GLFW_RELEASE: {
					MouseButtonReleaseEvent e(button);
					data.EventCallback(e);
					break;
				}
				}
			});

		glfwSetCursorPosCallback(m_WindowHandle, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				//MouseMovedEvent
				MouseMoveEvent e((float)xPos, (float)yPos);
				data.EventCallback(e);
			});

		glfwSetScrollCallback(m_WindowHandle, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				//MouseScrolledEvent
				MouseScrollEvent e((float)xOffset, (float)yOffset);
				data.EventCallback(e);
			});


	}

	MultiPlatformWindow::~MultiPlatformWindow()
	{
		//somehow, change to be if this is the LAST window of the application
		LOG_S(INFO) << "Window Object Destroyed!";
		glfwDestroyWindow(m_WindowHandle);
		glfwTerminate();
	}

	void MultiPlatformWindow::OnUpdate()
	{
		glfwPollEvents();
	}

	void MultiPlatformWindow::SetVSync(bool enabled)
	{
		if (enabled) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}
		m_Data.VSync = enabled;
	}

	void MultiPlatformWindow::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

	float MultiPlatformWindow::GetLastFrameTime() const
	{
		return (float)glfwGetTime();
	}


	//call this to creat a window.
	WindowRef Window::Create(uint32_t width, uint32_t height, const std::string& title)
	{
		return std::make_shared<MultiPlatformWindow>(width, height, title);
	}
}

