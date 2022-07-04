#include "pch.h"
#include "Window.h"




Window::Window(GLFWwindow* handle, const std::string& name)
    :m_WindowHandle(handle), m_Name(name)
{
    glfwGetWindowSize(handle, &m_Params.width, &m_Params.height);
    glfwSetWindowUserPointer(handle, (void*)&m_Params);
    glfwSetFramebufferSizeCallback(handle, s_fbSizeCallback);

}

void Window::OnUpdate()
{
    CalcDeltaTime();
    ProcessCameraInput();
}

void Window::ProcessCameraInput() const
{
    if (m_Params.cursorVisible)
        return;

    for (auto keyBind : *s_CameraKeybinds)
    {
        if (glfwGetKey(m_WindowHandle, keyBind->GlId()) == keyBind->GlType())
            keyBind->Command();
    }
}

void Window::CalcDeltaTime()
{
    static float m_LastFrame = 0.0f;
    float currentFrame = glfwGetTime();
    m_DeltaTime = currentFrame - m_LastFrame;
    m_LastFrame = currentFrame;
}

void Window::SetCamera(Camera* cam)
{
    if (!m_Params.camera)
        SetCameraCallbacks(cam != nullptr);
    m_Params.camera = cam;
}






void Window::SetCameraCallbacks(bool cameraActive)
{
    if (cameraActive)
    {
        glfwSetCursorPosCallback(m_WindowHandle, s_cursorPosCallback);
        glfwSetScrollCallback(m_WindowHandle, s_scrollCallback);
    }
    else
    {
        glfwSetCursorPosCallback(m_WindowHandle, [](GLFWwindow*,double, double){});
        glfwSetScrollCallback(m_WindowHandle, [](GLFWwindow*, double, double) {});
    }
    
}




void Window::s_fbSizeCallback(GLFWwindow* window, int width, int height)
{
    WindowParams& params = 
        *reinterpret_cast<WindowParams*>(glfwGetWindowUserPointer(window));

    params.width = width;
    params.height = height;
}

void Window::s_cursorPosCallback(GLFWwindow* window, double xposIn, double yposIn)
{
    WindowParams& params =
        *reinterpret_cast<WindowParams*>(glfwGetWindowUserPointer(window));

   /* if (!params.camera)
        return;*/
    static bool firstMouseUse = true;
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);


    if (firstMouseUse)
    {
        params.cursorX = xpos;
        params.cursorY = ypos;
        firstMouseUse = false;
    }

    float xoffset = xpos - params.cursorX;
    float yoffset = params.cursorY - ypos;

    params.cursorX = xpos;
    params.cursorY = ypos;
    
    //params.camera && 
    if (!params.cursorVisible)
        params.camera->ProcessMouseMovement(xoffset, yoffset);
    else
        firstMouseUse = true;

    /*inp.xoffset = xoffset;
    inp.yoffset = yoffset;*/
}



void Window::s_scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    WindowParams& params =
        *reinterpret_cast<WindowParams*>(glfwGetWindowUserPointer(window));

    if (!params.camera)
        return;
    params.camera->ProcessMouseScroll(static_cast<float>(yoffset));
}


void Window::s_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    WindowParams& params =
        *reinterpret_cast<WindowParams*>(glfwGetWindowUserPointer(window));

    for (const auto kb : *params.keybinds)
    {
        if (key == kb->GlId() && action == kb->GlType())
            kb->Command(window, key, scancode, action, mode);
    }
}