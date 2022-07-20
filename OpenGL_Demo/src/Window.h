#pragma once

#include "input/Keybind.h"
#include "Camera.h"

struct GLFWwindow;

class Window
{
private:
    struct WindowParams
    {
        bool paused = false;
        std::vector<Keybind> keys;
        Ref<Camera> camera = nullptr;
        int width;
        int height;
        float cursorX;
        float cursorY;
        bool cursorVisible;
    };
public:
    Window(GLFWwindow* handle, const std::string& name);
    ~Window() {}

    GLFWwindow* Handle() { return m_WindowHandle; }
    void SetCamera(Ref<Camera> cam);
    void OnUpdate();

    const bool Paused() const { return m_Params.paused; }
    const int Width() { return m_Params.width; }
    const int Height() { return m_Params.height; }

    const float DeltaTime() const { return m_DeltaTime; }

    bool IsOpen() const;
    bool CursorVisible() { return m_Params.cursorVisible; }
    void HideCursor();
    void ShowCursor();
    void Close();
    void SetKeybinds(std::unordered_map<KeyActionType, Keybind>& kbs);
    
private:
    void SetCameraCallbacks(bool cameraActive);
    void ProcessCameraInput() const;
    void CalcDeltaTime();
    void SetGlobalSettings();

private:
    GLFWwindow* m_WindowHandle;
    WindowParams m_Params;
    std::string m_Name;
    float m_DeltaTime = 0.0f;
    float m_LastFrame = 0.0f;
    std::vector<Keybind> m_CamKeys;


    //static std::vector<KeybindCamera*>* s_CameraKeybinds;
    
private:
    static void s_fbSizeCallback   (GLFWwindow* window, int width, int height);
    static void s_iconifyCallback  (GLFWwindow* window, int minimized);

    static void s_cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void s_scrollCallback   (GLFWwindow* window, double xoffset, double yoffset);
    static void s_keyCallback      (GLFWwindow* window, int key, int scancode, int action, int mode);
};

