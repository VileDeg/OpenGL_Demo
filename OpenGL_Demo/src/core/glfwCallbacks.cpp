#include "pch.h"
#include "Window.h"
#include <GLFW/glfw3.h>
#include "input/Input.h"

void Window::s_fbSizeCallback(GLFWwindow* window, int width, int height)
{
    /*WindowParams& params =
        *reinterpret_cast<WindowParams*>(glfwGetWindowUserPointer(window));*/

    //glViewport(0, 0, width, height);
    m_Params.width = width;
    m_Params.height = height;
}

void Window::s_iconifyCallback(GLFWwindow* window, int minimized)
{
    /*WindowParams& params =
        *reinterpret_cast<WindowParams*>(glfwGetWindowUserPointer(window));*/

    if (minimized)
    {
        m_Params.paused = true;
    }
    else
    {
        m_Params.paused = false;
    }
}

void Window::s_cursorPosCallback(GLFWwindow* window, double xposIn, double yposIn)
{
    /*WindowParams& params =
        *reinterpret_cast<WindowParams*>(glfwGetWindowUserPointer(window));*/

    if (!m_Camera)
        return;
    static bool firstMouseUse = true;
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);


    if (firstMouseUse)
    {
        m_Params.cursorX = xpos;
        m_Params.cursorY = ypos;
        firstMouseUse = false;
    }

    float xoffset = xpos - m_Params.cursorX;
    float yoffset = m_Params.cursorY - ypos;

    m_Params.cursorX = xpos;
    m_Params.cursorY = ypos;

    if (!m_Params.cursorVisible)
        m_Camera->ProcessMouseMovement(xoffset, yoffset);
    else
        firstMouseUse = true;
}

void Window::s_scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    /*WindowParams& params =
        *reinterpret_cast<WindowParams*>(glfwGetWindowUserPointer(window));*/

    if (!m_Camera)
        return;
    m_Camera->ProcessMouseScroll(static_cast<float>(yoffset));
}

void Window::s_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    /*WindowParams& params =
        *reinterpret_cast<WindowParams*>(glfwGetWindowUserPointer(window));*/

    Input::ProcessInput(key, action);

   /* for (auto kb : params.keys)
    {
        if (key == kb.GlId() && action == kb.GlType())
            kb.Exec();
    }*/
}

#define GL_DEBUG_SEVERITY_NOTIFICATION 0x826B
#define GL_DEBUG_TYPE_ERROR            0x824C

void Window::s_DebugMessageCallback(unsigned source,
    unsigned type, unsigned id, unsigned severity,
    int length, const char* message, const void* userParam)
{
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
        return;
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
}