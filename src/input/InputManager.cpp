#include "InputManager.h"
#include "GLContext.h"


static void default_mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

}

InputManager& InputManager::getTnstance(cursor_pos_callback callback)
{
    static InputManager INSTANCE(callback);
    return INSTANCE;
}

InputManager::InputManager(cursor_pos_callback callback)
{
    if (callback == NULL)
        m_CursorPosCallback = default_mouse_callback;
    else
        m_CursorPosCallback = callback;
}   

void InputManager::SetGlobalSettings()
{
    GLContext& context = GLContext::getTnstance();
    glfwSetInputMode(context.Window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
