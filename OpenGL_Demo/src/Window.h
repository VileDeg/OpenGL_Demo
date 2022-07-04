#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "renderer/Buffer.h"
#include "shader/Shader.h"
#include "input/Keybind.h"
#include "Camera.h"

class Window
{
private:
    struct WindowParams
    {
        static std::vector<KeybindDelayed*>* keybinds;
        Camera* camera;
        int width;
        int height;
        float cursorX;
        float cursorY;
        /*float offsetX;
        float offsetY;*/
        bool cursorVisible;
    };
public:
    Window(GLFWwindow* handle, const std::string& name);
    ~Window() {}

    GLFWwindow* Handle() { return m_WindowHandle; }
    void SetCamera(Camera* cam);
    void OnUpdate();
    void SetDelayedKeybinds(std::vector<KeybindDelayed*>* kbs)
    {
        WindowParams::keybinds = kbs;
    }
    void SetCameraKeybinds(std::vector<KeybindCamera*>* kbs)
    {
        s_CameraKeybinds = kbs;
    }
    
private:
    void SetCameraCallbacks(bool cameraActive);
    void ProcessCameraInput() const;
    void CalcDeltaTime()
    {
        static float m_LastFrame = 0.0f;
        float currentFrame = glfwGetTime();
        m_DeltaTime = currentFrame - m_LastFrame;
        m_LastFrame = currentFrame;
    }
private:
    GLFWwindow* m_WindowHandle;
    WindowParams m_Params;
    std::string m_Name;
    float m_DeltaTime;
    float m_LastFrame;

    static std::vector<KeybindCamera*>* s_CameraKeybinds;
private:
    static void s_fbSizeCallback(GLFWwindow* window, int width, int height);
    static void s_cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void s_scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void s_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
};

