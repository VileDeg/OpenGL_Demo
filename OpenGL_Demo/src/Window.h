#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "input/Keybind.h"
#include "Camera.h"

class Window
{
private:
    struct WindowParams
    {
        //static std::vector<KeybindDelayed*>* keybinds;
        std::vector<Keybind> keys;
        Camera* camera = nullptr;
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


    const int Width() { return m_Params.width; }
    const int Height() { return m_Params.height; }
    Camera::ScreenParams GetDimensions() const {
        return { m_Params.width, m_Params.height }; 
    }
    const float DeltaTime() const { return m_DeltaTime; }

    bool CursorVisible() { return m_Params.cursorVisible; }
    void HideCursor() { 
        glfwSetInputMode(m_WindowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        m_Params.cursorVisible = false;
    }
    void ShowCursor() { 
        glfwSetInputMode(m_WindowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        m_Params.cursorVisible = true;
    }
    void Close()
    {
        glfwSetWindowShouldClose(m_WindowHandle, true);
    }
    void SetKeybinds(std::unordered_map<KeyActionType, Keybind>& kbs);
    
private:
    void SetCameraCallbacks(bool cameraActive);
    void ProcessCameraInput() const;
    void CalcDeltaTime();
    
private:
    GLFWwindow* m_WindowHandle;
    WindowParams m_Params;
    std::string m_Name;
    float m_DeltaTime;
    float m_LastFrame;
    std::vector<Keybind> m_CamKeys;

    //static std::vector<KeybindCamera*>* s_CameraKeybinds;
    
private:
    static void s_fbSizeCallback(GLFWwindow* window, int width, int height);
    static void s_cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void s_scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void s_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
};

