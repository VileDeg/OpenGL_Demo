#pragma once

#include "renderer/Camera.h"
#include "input/Input.h"

struct GLFWwindow;

namespace Window
{
    struct WindowParams
    {
        bool paused = false;
        int width;
        int height;
        float cursorX;
        float cursorY;
        bool cursorVisible;
        bool scrollingLocked;
    };

    GLFWwindow* Open(const unsigned width, const unsigned height, const std::string& name);

    GLFWwindow* Handle();

    void SetCamera(Ref<Camera> cam);
    void OnUpdate();

    void SetScrollingLocked(bool locked);

    bool Paused();
    int Width();
    int Height();

    float DeltaTime();

    bool KeyPressed(Key keyCode);
    bool IsOpen();
    bool CursorVisible();
    void HideCursor();
    void ShowCursor();
    void Close();
    void SetKeybinds();

    void GLFWTerminate();
    void GLFWSwapBuffers();
    void GLFWPollEvents();
    
    void CalcDeltaTime();
};
