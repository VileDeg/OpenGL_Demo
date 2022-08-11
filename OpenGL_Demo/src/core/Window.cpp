#include "pch.h"
#include "Window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

GLFWwindow*          Window::m_Handle{};
Window::WindowParams Window::m_Params{};
Ref<Camera>          Window::m_Camera{};
std::string          Window::m_Name{};
float                Window::m_DeltaTime{};
float                Window::m_LastFrame{};
std::vector<Keybind> Window::m_CamKeys{};
bool                 Window::m_GladInit = false;

GLFWwindow* Window::Open(const unsigned width, const unsigned height, const std::string& name)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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
        m_GladInit = true;
#ifdef _DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(s_DebugMessageCallback, nullptr);
#endif
    }

    m_Handle = handle;
    m_Name = name;
    m_Params.width = width;
    m_Params.height = height;

    SetKeybinds();
    SetGlobalSettings();

    //glfwSetFramebufferSizeCallback(handle, s_fbSizeCallback);
    glfwSetKeyCallback(handle, s_keyCallback);
    glfwSetWindowIconifyCallback(handle, s_iconifyCallback);

    glfwSetCursorPosCallback(m_Handle, s_cursorPosCallback);
    glfwSetScrollCallback(m_Handle, s_scrollCallback);

    return handle;
}

void Window::OnUpdate()
{
    CalcDeltaTime();
    if (m_Camera)
    {
        if (!m_Params.cursorVisible)
        {
            ProcessCameraInput();
            m_Camera->OnUpdate();
        }
    }
}

void Window::ProcessCameraInput()
{
    for (auto key : m_CamKeys)
    {
        if (glfwGetKey(m_Handle, key.GlId()) == key.GlType())
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

void Window::SetGlobalSettings()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void Window::SetCamera(Ref<Camera> cam)
{
    m_Camera = cam;
}

bool Window::KeyPressed(Key keyCode)
{
    return glfwGetKey(m_Handle, static_cast<int>(keyCode)) == GLFW_PRESS;
}

bool Window::IsOpen()
{
    return !glfwWindowShouldClose(m_Handle);
}

void Window::HideCursor() {
    glfwSetInputMode(m_Handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    m_Params.cursorVisible = false;
}
void Window::ShowCursor() {
    glfwSetInputMode(m_Handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    m_Params.cursorVisible = true;
}
void Window::Close()
{
    glfwSetWindowShouldClose(m_Handle, true);
}
void Window::SetCamKey(Key keyCode, KeyEvent eventType, Keybind::actionType func)
{
    Keybind kb((int)keyCode, (int)eventType);
    kb.BindAction(func);
    m_CamKeys.push_back(kb);
}
void Window::SetKeybinds()
{
    //Camera controls
    SetCamKey(Key::W, KeyEvent::Press, [&]() {
        m_Camera->MoveForward(m_DeltaTime); });
    SetCamKey(Key::A, KeyEvent::Press, [&]() {
        m_Camera->MoveLeft(m_DeltaTime); });
    SetCamKey(Key::S, KeyEvent::Press, [&]() {
        m_Camera->MoveBackward(m_DeltaTime); });
    SetCamKey(Key::D, KeyEvent::Press, [&]() {
        m_Camera->MoveRight(m_DeltaTime); });
    
    Input::SetKeybind(Key::Esc, KeyEvent::Press, [&]() { //Close window
        if (!m_Params.cursorVisible) ShowCursor();
        else Close(); });
    Input::SetKeybind(Key::C, KeyEvent::Press, [&]() { //Cursor toggle
        if (m_Params.cursorVisible) HideCursor();
        else ShowCursor(); });
}


void Window::GLFWTerminate()
{
    glfwTerminate();
}

void Window::GLFWSwapBuffers()
{
    glfwSwapBuffers(m_Handle);
}

void Window::GLFWPollEvents()
{
    glfwPollEvents();
}