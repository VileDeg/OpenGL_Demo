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
    GLContext::getTnstance().SetViewport(width, height);
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
//void GLContext::DrawCursorSetup()
//{
//
//    m_CursorData.shader = new Shader("cursor_vert.shader", "cursor_frag.shader");
//    m_CursorData.vao = new VAO;
//    m_CursorData.vbo = new VBO(quad2D, sizeof(quad2D), 4);
//    static VertexLayout layout;
//    layout.Push<float>(3);
//    m_CursorData.vao->AddBuffer(*m_CursorData.vbo, layout);
//    
//    m_CursorData.data = quad2D;
//}
//
//void GLContext::DrawCursor()
//{
//    
//    Object cursorObj({ quad2D, sizeof(quad2D), 4, 3, 0, 0, 2 }, { "awesomeface.png" }, "cursor_vert.shader", "cursor_frag.shader");
//    //Object obj({quad2D, sizeof(quad2D), 8, 2, 0, 0, 0}, {}, vertShaderPath)
//
//    cursorObj.SetProjMat(glm::ortho(0, m_SCR_WIDTH, 0, m_SCR_HEIGHT));
//    cursorObj.DrawNoIndex();
//
//    /*m_CursorData.shader->Bind();
//    m_CursorData.vao->Bind();
//    float x, y;
//    InputManager::getInstance().GetCursor(x, y);
//    m_CursorData.shader->setFloat("cursorX", x);
//    m_CursorData.shader->setFloat("cursorY", y);
//    m_CursorData.shader->setFloat("cursorSize", 20.0f);
//    glDrawArrays(GL_TRIANGLES, 0, m_CursorData.vao->Count());*/
//    std::cout << "Drawing cursor.\n";
//}

void GLContext::SetParams(const unsigned width, const unsigned height, const std::string& name, fb_size_callback callback)
{
    m_SCR_WIDTH = width;
    m_SCR_HEIGHT = height;
    m_WINDOW_NAME = name;
    SetFBSizeCallback(callback);
}



GLContext::GLContext(const unsigned width, const unsigned height,
    const std::string& name, fb_size_callback callback)
    : m_SCR_WIDTH(width), m_SCR_HEIGHT(height), m_WINDOW_NAME(name),
    m_CursorData{ nullptr, nullptr, nullptr, nullptr }, m_CursorVisible(true)
{
    Init();
    OpenWindow();
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
