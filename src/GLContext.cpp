#include "GLContext.h"
#include "input/InputManager.h"

#include "Object.h"

#include <iostream>



GLContext& GLContext::getTnstance(const unsigned width, const unsigned height,
    const std::string& name, fb_size_callback callback)
{
    static GLContext INSTANCE(width, height, name, callback);
    return INSTANCE;   
}

void GLContext::default_fb_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    reinterpret_cast<GLContext*>(glfwGetWindowUserPointer(window))->SetViewport(width, height);
}

void GLContext::SetFBSizeCallback(fb_size_callback fbcb)
{
    if (fbcb != NULL)
        m_FBSIZE_CALLBACK = fbcb;
    else
        m_FBSIZE_CALLBACK = default_fb_size_callback;
    glfwSetFramebufferSizeCallback(m_WINDOW, m_FBSIZE_CALLBACK);
}
static float dist = 200.f;
static float quad2D[20]
{
    -dist, -dist, 0.0f,    0.0f, 0.0f,
    dist, -dist, 0.0f,    1.0f, 0.0f,
    dist, dist, 0.0f,    1.0f, 1.0f, 
    -dist, dist, 0.0f,    0.0f, 1.0f
};                      

void GLContext::SetParams(const unsigned width, const unsigned height, const std::string& name, fb_size_callback callback)
{
    m_SCR_WIDTH = width;
    m_SCR_HEIGHT = height;
    m_WINDOW_NAME = name;
    SetFBSizeCallback(callback);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    GLContext* user = reinterpret_cast<GLContext*>(glfwGetWindowUserPointer(window));
    for (const auto kb : user->Keybinds())
    {
        if (key == kb->GlId() && action == kb->GlType())
            kb->Command(window, key, scancode, action, mode);
    }
}

void GLContext::ProcessInput() const
{
    for (auto keyBind : m_CameraKeybinds)
    {
        if (glfwGetKey(m_WINDOW, keyBind->GlId()) == keyBind->GlType())
            keyBind->Command();
    }
}

GLContext::GLContext(const unsigned width, const unsigned height,
    const std::string& name, fb_size_callback callback)
    : m_SCR_WIDTH(width), m_SCR_HEIGHT(height), m_WINDOW_NAME(name),
    m_CursorData{ nullptr, nullptr, nullptr, nullptr }, m_CursorVisible(true)
{
    Init();
    OpenWindow();
    glfwSetWindowUserPointer(m_WINDOW, reinterpret_cast<void*>(this));
    glfwSetKeyCallback(m_WINDOW, key_callback);
    SetFBSizeCallback(default_fb_size_callback);
    SetGlobalSettings();
    //DrawCursorSetup();
    //DeltaTimer();
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

//void GLContext::DeltaTimer()
//{
//    while (!glfwWindowShouldClose(m_WINDOW))
//    {
//        float currentFrame = glfwGetTime();
//        m_DeltaTime = currentFrame - m_LastFrame;
//        m_LastFrame = currentFrame;
//    }
//}
