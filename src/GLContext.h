#pragma once

#include <string>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "VAO.h"
#include "shader/Shader.h"

class GLContext
{
private:
    
    using fb_size_callback = void (*)(GLFWwindow* window, int width, int height);
public:

    static GLContext& getTnstance(const unsigned width = 800, const unsigned height = 600,
        const std::string& name = "Window", fb_size_callback callback = NULL);
    inline const int Width() const { return m_SCR_WIDTH; }
    inline const int Height() const { return m_SCR_HEIGHT; }
    inline const std::string& Name() const { return m_WINDOW_NAME; }
    inline GLFWwindow* Window() const { return m_WINDOW; }
    inline void SetViewport(const int width, const int height)
        { m_SCR_WIDTH = width; m_SCR_HEIGHT = height; }
    
    void OpenWindow();
    void SetGlobalSettings();
    void SetFBSizeCallback(fb_size_callback);
    void DrawCursor();


    inline void HideCursor() { glfwSetInputMode(m_WINDOW, GLFW_CURSOR, GLFW_CURSOR_DISABLED); m_CursorVisible = false; }
    inline void ShowCursor() { glfwSetInputMode(m_WINDOW, GLFW_CURSOR, GLFW_CURSOR_NORMAL); m_CursorVisible = true; }
    inline bool CursorVisible() { return m_CursorVisible; }
    inline const float DeltaTime() { return m_DeltaTime; }
    inline void SetDeltaTime(float dt) { m_DeltaTime = dt; }
    inline void SetWindowSize(const int width, const int height)
    { 
        glfwSetWindowSize(m_WINDOW, width, height);
        m_SCR_WIDTH = width;
        m_SCR_HEIGHT = height;
    }
    inline void SetWindowTitle(const char* title)
    {
        glfwSetWindowTitle(m_WINDOW, title);
        m_WINDOW_NAME = title;   
    }

    void SetParams(const unsigned width = 800, const unsigned height = 600,
        const std::string& name = "Window", fb_size_callback callback = NULL);

    static void default_fb_size_callback(GLFWwindow* window, int width, int height);

    GLContext(const GLContext&)      = delete;
    void operator=(const GLContext&) = delete;
private:
    GLContext( const unsigned width, const unsigned height,
        const std::string& name, fb_size_callback callback);

    void DrawCursorSetup();
    void Init();
    
    //void DeltaTimer();

    

    double m_CursorX;
    double m_CursorY;
    bool m_CursorVisible;
    float m_DeltaTime;
    int m_SCR_WIDTH;
    int m_SCR_HEIGHT;
    std::string m_WINDOW_NAME;
    GLFWwindow* m_WINDOW;
    fb_size_callback m_FBSIZE_CALLBACK;

    struct CursorData
    {
        VAO vao;
        Shader shader;
        const float* data;
    };
    CursorData m_CursorData;
};

