#include "tarapch.h"
#include "MultiPlatformInput.h"
#include "GLFW/glfw3.h"

namespace Tara {
    MultiPlatformInput::MultiPlatformInput()
        : Input(), m_WindowHandle(nullptr) {}


    bool MultiPlatformInput::IsKeyPressed(int32_t key) const
    {
        return (glfwGetKey(m_WindowHandle, key) == GLFW_PRESS);
    }

    bool MultiPlatformInput::IsMouseDown(int32_t button) const
    {
        return (glfwGetMouseButton(m_WindowHandle, button) == GLFW_PRESS);
    }

    std::pair<uint32_t, uint32_t> MultiPlatformInput::GetMousePos() const
    {
        double xPos, yPos;
        glfwGetCursorPos(m_WindowHandle, &xPos, &yPos);
        return std::pair<uint32_t, uint32_t>((uint32_t)xPos, (uint32_t)yPos);
    }

    void MultiPlatformInput::Initialize(WindowRef ref)
    {
        if (m_WindowHandle == nullptr) {
            m_WindowHandle = (GLFWwindow*)ref->GetNativeWindow();
        }
        else {
            ABORT_F("Trying to initialize the Input system twice!");
        }
    };

    std::unique_ptr<Input> Tara::Input::Create()
    {
        return std::make_unique<MultiPlatformInput>();
    }
}
