#include "InputManager.h"
#include "GLContext.h"


InputManager& InputManager::getInstance()
{
    static InputManager INSTANCE;
    return INSTANCE;
}

void InputManager::default_mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    static bool firstMouseUse = true;

    InputManager& inp = InputManager::getInstance();
    inp.m_CursorX = 0.0f;
    inp.m_CursorY = 0.0f;

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
 

    inp.m_Camera->ProcessMouseMovement(xoffset, yoffset);
}

template<typename T>
inline void InputManager::RegisterKeybind(int keyId)
{
    m_Keybinds.push_back(new T(keyId));
}



void InputManager::ProcessInput() const
{
    for (auto keyBind : m_Keybinds)
    {
        if (glfwGetKey(m_Context.Window(), keyBind->GlId()) == GLFW_PRESS)
            keyBind->OnPress(*m_Camera);
    }
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
    m_CursorPosCallback = default_mouse_callback;
}   

void InputManager::SetGlobalSettings()
{
    GLContext& context = GLContext::getTnstance();
    glfwSetInputMode(context.Window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
