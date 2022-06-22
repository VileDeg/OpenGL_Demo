#pragma once

#include <string>

#include "glad/glad.h"
#include "GLFW/glfw3.h"


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
    
    void SetFBSizeCallback(fb_size_callback);

    inline const float DeltaTime() { return m_DeltaTime; }
    inline void SetDeltaTime(float dt) { m_DeltaTime = dt; }
    //float m_LastFrame;
    float m_DeltaTime;

    GLContext(const GLContext&)      = delete;
    void operator=(const GLContext&) = delete;
private:
    GLContext( const unsigned width, const unsigned height,
        const std::string& name, fb_size_callback callback);

    void Init();
    void OpenWindow();
    void SetGlobalSettings();
    //void DeltaTimer();

    static void default_fb_size_callback(GLFWwindow* window, int width, int height);

    
    

    int m_SCR_WIDTH;
    int m_SCR_HEIGHT;
    std::string m_WINDOW_NAME;
    GLFWwindow* m_WINDOW;
    fb_size_callback m_FBSIZE_CALLBACK;
};

