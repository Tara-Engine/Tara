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

    glm::vec2 MultiPlatformInput::GetMousePos() const
    {
        double xPos, yPos;
        glfwGetCursorPos(m_WindowHandle, &xPos, &yPos);
        return glm::vec2{ (float)xPos, (float)yPos };
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
