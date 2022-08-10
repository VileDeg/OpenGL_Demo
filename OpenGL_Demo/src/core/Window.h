#pragma once

#include "renderer/Camera.h"
#include "input/Input.h"

struct GLFWwindow;

class Window
{
public:
    struct WindowParams
    {
        bool paused = false;
        //std::vector<Keybind> keys;
        //Ref<Camera> camera = nullptr;
        int width;
        int height;
        float cursorX;
        float cursorY;
        bool cursorVisible;
    };
public:
    static GLFWwindow* Open(const unsigned width, const unsigned height, const std::string& name);

    static GLFWwindow* Handle() { return m_Handle; }
    static void SetCamera(Ref<Camera> cam);
    static void OnUpdate();

    static const bool Paused() { return m_Params.paused; }
    static const int Width() { return m_Params.width; }
    static const int Height() { return m_Params.height; }

    static const float DeltaTime() { return m_DeltaTime; }

    static bool KeyPressed(Key keyCode);
    static bool IsOpen();
    static bool CursorVisible() { return m_Params.cursorVisible; }
    static void HideCursor();
    static void ShowCursor();
    static void Close();
    static void SetKeybinds();

    static void GLFWTerminate();
    static void GLFWSwapBuffers();
    static void GLFWPollEvents();
    
private:
    static void ProcessCameraInput();
    static void CalcDeltaTime();
    static void SetGlobalSettings();
    static void SetCamKey(Key keyCode, KeyEvent eventType, Keybind::actionType func);

private:
    static GLFWwindow* m_Handle;
    static Ref<Camera> m_Camera;
    static WindowParams m_Params;
    static std::string m_Name;
    static float m_DeltaTime;
    static float m_LastFrame;
    static std::vector<Keybind> m_CamKeys;

    static bool m_GladInit;
private:
    static void s_fbSizeCallback   (GLFWwindow* window, int width, int height);
    static void s_iconifyCallback  (GLFWwindow* window, int minimized);

    static void s_cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void s_scrollCallback   (GLFWwindow* window, double xoffset, double yoffset);
    static void s_keyCallback      (GLFWwindow* window, int key, int scancode, int action, int mode);
    static void s_DebugMessageCallback(unsigned source,
        unsigned type, unsigned id, unsigned severity,
        int length, const char* message, const void* userParam);
};

