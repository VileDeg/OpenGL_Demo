#include "pch.h"
#include "Window.h"




Window::Window(GLFWwindow* handle, const std::string& name)
    :m_WindowHandle(handle), m_Name(name)
{
    glfwGetWindowSize(handle, &m_Params.width, &m_Params.height);
    glfwSetWindowUserPointer(handle, (void*)&m_Params);
    glfwSetFramebufferSizeCallback(handle, s_fbSizeCallback);
   
    glfwSetKeyCallback(handle, s_keyCallback);
}

void Window::OnUpdate()
{
    CalcDeltaTime();
    if (m_Params.camera)
    {
        if (!m_Params.cursorVisible)
            ProcessCameraInput();
        m_Params.camera->OnUpdate();
    }
}

void Window::ProcessCameraInput() const
{
    for (auto key : m_CamKeys)
    {
        if (glfwGetKey(m_WindowHandle, key.GlId()) == key.GlType())
            key.Exec();
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
    glfwSetCursorPosCallback(m_WindowHandle, s_cursorPosCallback);
    glfwSetScrollCallback(m_WindowHandle, s_scrollCallback);
}

void Window::SetKeybinds(std::unordered_map<KeyActionType, Keybind>& kbs)
{
    /*kbs[KeyActionType::CameraForward].BindAction(
        std::bind([=](Camera* cam) {cam->MoveForward(); },
            m_Params.camera));*/
    kbs[KeyActionType::CameraForward].BindAction(
        [&](){ m_Params.camera->MoveForward (&m_DeltaTime); });
    kbs[KeyActionType::CameraBackward].BindAction(
        [&](){ m_Params.camera->MoveBackward(&m_DeltaTime); });
    kbs[KeyActionType::CameraRight].BindAction(
        [&]() { m_Params.camera->MoveRight  (&m_DeltaTime); });
    kbs[KeyActionType::CameraLeft].BindAction(
        [&]() { m_Params.camera->MoveLeft   (&m_DeltaTime); });
    kbs[KeyActionType::WindowClose].BindAction(
        [&]() {if (!m_Params.cursorVisible)
                ShowCursor();
            else
                Close();});
    kbs[KeyActionType::CursorToggle].BindAction(
            [&](){if (m_Params.cursorVisible)
                    HideCursor();
                else
                    ShowCursor();});

    for (auto& [type, keybind] : kbs)
    {
        if (type > KeyActionType::Camera)
            m_CamKeys.push_back(keybind);
        else if (type < KeyActionType::Camera)
            m_Params.keys.push_back(keybind);
    }
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

    for (auto kb : params.keys)
    {
        if (key == kb.GlId() && action == kb.GlType())
            kb.Exec();
    }
}