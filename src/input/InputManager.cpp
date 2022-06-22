#include "InputManager.h"
#include "GLContext.h"
#include <iostream>

InputManager& InputManager::getInstance()
{
    static InputManager INSTANCE;
    return INSTANCE;
}

void InputManager::default_cursor_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    InputManager& inp = InputManager::getInstance();
    if (!inp.m_Camera)
        return;
    static bool firstMouseUse = true;
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
   
    
    //inp.m_CursorX = 0.0f;
    //inp.m_CursorY = 0.0f;

    if (firstMouseUse)
    {
        inp.m_CursorX = xpos;
        inp.m_CursorY = ypos;
        firstMouseUse = false;
    }

    float xoffset = xpos - inp.m_CursorX;
    float yoffset = inp.m_CursorY - ypos; // reversed since y-coordinates go from bottom to top

    inp.m_CursorX = xpos;
    inp.m_CursorY = ypos;
    //std::cout << "Cursor x: " << xpos << ", y: " << ypos << '\n';

    inp.m_Camera->ProcessMouseMovement(xoffset, yoffset); 
    inp.xoffset = xoffset;
    inp.yoffset = yoffset;
}

void InputManager::default_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Camera* cam = InputManager::getInstance().m_Camera;
    if (!cam)
        return;
    cam->ProcessMouseScroll(static_cast<float>(yoffset));
}



void InputManager::ProcessInput() const
{
    for (auto keyBind : m_Keybinds)
    {
        if (glfwGetKey(m_Context.Window(), keyBind->GlId()) == GLFW_PRESS)
            keyBind->OnPress();
    }
}

void InputManager::SetCursorCallback(mouse_callback cpcb)
{
    if (cpcb != NULL)
        m_CursorPosCallback = cpcb;
    else
        m_CursorPosCallback = default_cursor_callback;
    glfwSetCursorPosCallback(m_Context.Window(), m_CursorPosCallback);
}

void InputManager::SetScrollCallback(mouse_callback sccb)
{
    if (sccb != NULL)
        m_ScrollCallback = sccb;
    else
        m_ScrollCallback = default_scroll_callback;
    glfwSetScrollCallback(m_Context.Window(), m_ScrollCallback);
}

void InputManager::HideCursor()
{
    GLContext& context = GLContext::getTnstance();
    glfwSetInputMode(context.Window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

InputManager::~InputManager()
{
    for (auto keyBind : m_Keybinds)
    {
        delete keyBind;
    }
}

InputManager::InputManager()
    : m_Context(GLContext::getTnstance()), m_CursorX(m_Context.Width()/2.0f),
    m_CursorY(m_Context.Height() / 2.0f)
{
    SetCursorCallback(default_cursor_callback);
    SetScrollCallback(default_scroll_callback);
    SetGlobalSettings();
}   

void InputManager::SetGlobalSettings()
{
    
}
