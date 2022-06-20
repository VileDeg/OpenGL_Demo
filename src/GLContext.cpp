#include "GLContext.h"

#include <iostream>

GLContext& GLContext::getTnstance(const unsigned width, const unsigned height,
    const std::string& name, fb_size_callback callback)
{
    static GLContext INSTANCE(width, height, name, callback);
    return INSTANCE;   
}

static void default_fb_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

const float GLContext::DeltaTime()
{
    float currentFrame = glfwGetTime();
    m_DeltaTime = currentFrame - m_LastFrame;
    m_LastFrame = currentFrame;
    return m_DeltaTime;
}

GLContext::GLContext(const unsigned width, const unsigned height,
    const std::string& name, fb_size_callback callback)
    : m_SCR_WIDTH(width), m_SCR_HEIGHT(height), m_WINDOW_NAME(name)
{
    Init();
    OpenWindow();
    SetGlobalSettings();

    if (callback == NULL)
        m_FBSIZE_CALLBACK = default_fb_size_callback;
    else
        m_FBSIZE_CALLBACK = callback;
}

void GLContext::Init()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

void GLContext::OpenWindow()
{
    // glfw window creation
   // --------------------
    m_WINDOW = glfwCreateWindow(m_SCR_WIDTH, m_SCR_HEIGHT, m_WINDOW_NAME.c_str(), NULL, NULL);
    if (m_WINDOW == NULL)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
    }
    glfwMakeContextCurrent(m_WINDOW);
    glfwSetFramebufferSizeCallback(m_WINDOW, m_FBSIZE_CALLBACK);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
}

void GLContext::SetGlobalSettings()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glEnable(GL_DEPTH_TEST);

    
}
