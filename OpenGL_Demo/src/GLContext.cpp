#include "pch.h"
#include "GLContext.h"

GLContext::GLContext()

{
    Init();
    SetKeybinds();
}

GLContext& GLContext::Create()
{
    if (s_Instance != nullptr)
        return Get();

    s_Instance = new GLContext;
}

GLFWwindow* GLContext::OpenWindow(const unsigned width, const unsigned height, const std::string& name)
{
    
    GLFWwindow* handle = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (handle == NULL)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(handle);

    m_Window.reset(new Window(handle, name));
    return handle;
}

void GLContext::Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
}

void GLContext::UpdateWindowDelayedKeybinds()
{
    m_Window->SetDelayedKeybinds(&m_DelayedKeybinds);
}
void GLContext::UpdateWindowCameraKeybinds()
{
    m_Window->SetCameraKeybinds(&m_CameraKeybinds);
}

void GLContext::SetKeybinds()
{
    RegisterCameraKeybind <Keybind_Forward>     (GLFW_KEY_W     );
    RegisterCameraKeybind <Keybind_Backward>    (GLFW_KEY_S     );
    RegisterCameraKeybind <Keybind_Left>        (GLFW_KEY_A     );
    RegisterCameraKeybind <Keybind_Right>       (GLFW_KEY_D     );
    RegisterDelayedKeybind<Keybind_CloseWindow> (GLFW_KEY_ESCAPE);
    RegisterDelayedKeybind<Keybind_ToggleCursor>(GLFW_KEY_C     );
}
