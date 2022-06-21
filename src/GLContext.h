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
    
    const float DeltaTime();

    GLContext(const GLContext&)      = delete;
    void operator=(const GLContext&) = delete;
private:
    GLContext( const unsigned width, const unsigned height,
        const std::string& name, fb_size_callback callback);

    void Init();
    void OpenWindow();
    void SetGlobalSettings();

    float m_DeltaTime;
    float m_LastFrame;

    int m_SCR_WIDTH;
    int m_SCR_HEIGHT;
    std::string m_WINDOW_NAME;
    GLFWwindow* m_WINDOW;
    fb_size_callback m_FBSIZE_CALLBACK;
};

