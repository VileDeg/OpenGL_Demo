#include "InputManager.h"
#include "GLContext.h"


static void default_mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

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
            keyBind->OnPress();
    }
}



InputManager& InputManager::getTnstance(cursor_pos_callback callback)
{
    static InputManager INSTANCE(callback);
    return INSTANCE;
}

InputManager::~InputManager()
{
    for (auto keyBind : m_Keybinds)
    {
        delete keyBind;
    }
}

InputManager::InputManager(cursor_pos_callback callback)
    : m_Context(GLContext::getTnstance()), m_CursorX(m_Context.Width()/2.0f),
    m_CursorY(m_Context.Height() / 2.0f), m_CursorPosCallback(callback)
{
    if (m_CursorPosCallback == NULL)
        m_CursorPosCallback = default_mouse_callback;
}   

void InputManager::SetGlobalSettings()
{
    GLContext& context = GLContext::getTnstance();
    glfwSetInputMode(context.Window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
