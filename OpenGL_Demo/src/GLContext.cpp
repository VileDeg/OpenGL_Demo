#include "pch.h"
#include "GLContext.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

std::unique_ptr<GLContext> GLContext::s_Instance = nullptr;


GLContext::GLContext()

{
    Init();

    SetKeybinds();
}

GLContext& GLContext::Create()
{
    if (s_Instance != nullptr)
        return Get();

    s_Instance.reset(new GLContext);
    return *s_Instance;
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

    if (!m_GladInit)
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }
    }
    
    m_Window.reset(new Window(handle, name));
    m_Window->SetKeybinds(m_Keybinds);
    return handle;
}

void GLContext::Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

   
}


void GLContext::SetKeybinds()
{
    m_Keybinds.insert({ KeyActionType::CameraForward  , Keybind(GLFW_KEY_W, GLFW_PRESS)      });
    m_Keybinds.insert({ KeyActionType::CameraBackward , Keybind(GLFW_KEY_S, GLFW_PRESS)      });
    m_Keybinds.insert({ KeyActionType::CameraRight    , Keybind(GLFW_KEY_D, GLFW_PRESS)      });
    m_Keybinds.insert({ KeyActionType::CameraLeft     , Keybind(GLFW_KEY_A, GLFW_PRESS)      });
    m_Keybinds.insert({ KeyActionType::WindowClose    , Keybind(GLFW_KEY_ESCAPE, GLFW_PRESS) });
    m_Keybinds.insert({ KeyActionType::CursorToggle   , Keybind(GLFW_KEY_C, GLFW_PRESS)      });
}

void GLContext::Clear(std::bitset<3> bufferBits) const
{
    int buffers = 0;
    if (bufferBits[0])
    {
        buffers |= GL_COLOR_BUFFER_BIT;
    }
    if (bufferBits[1])
    {
        buffers |= GL_DEPTH_BUFFER_BIT;
    }
    if (bufferBits[2])
    {
        buffers |= GL_STENCIL_BUFFER_BIT;
    }
    glClear(buffers);
}
void GLContext::Terminate() const
{
    glfwTerminate();
}
void GLContext::SetClearColors(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}
void GLContext::SwapBuffers() const { glfwSwapBuffers(m_Window->Handle()); }
void GLContext::PollEvents() const { glfwPollEvents(); }
