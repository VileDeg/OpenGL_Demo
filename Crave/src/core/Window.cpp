#include "pch.h"
#include "Window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

namespace Crave
{
    namespace Window
    {
        namespace
        {
            void s_fbSizeCallback(GLFWwindow* window, int width, int height);
            void s_iconifyCallback(GLFWwindow* window, int minimized);

            void s_cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
            void s_scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
            void s_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
            void s_DebugMessageCallback(unsigned source,
                unsigned type, unsigned id, unsigned severity,
                int length, const char* message, const void* userParam);

            struct WindowParams
            {
                bool paused = false;
                int width = 800;
                int height = 600;
                float cursorX{};
                float cursorY{};
                bool cursorVisible = true;
                bool scrollingLocked = false;
                int vsync = 1;
            };

            GLFWwindow* m_Handle{};
            WindowParams         m_Params{};
            Ref<Camera>          m_Camera{};
            std::string          m_Name{};
            float                m_DeltaTime{};
            float                m_LastFrame{};
            //std::vector<Keybind> m_CamKeys{};
            bool                 m_GladInit = false;
            float                m_MouseScrollSensivity = 6.f;
        }

        GLFWwindow* Open(const unsigned width, const unsigned height, const std::string& name)
        {
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
            auto monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);

            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

            GLFWwindow* handle = glfwCreateWindow(mode->width, mode->height, name.c_str(), NULL, NULL);

            //GLFWwindow* handle = glfwCreateWindow(width, height, name.c_str(), glfwGetPrimaryMonitor(), NULL);
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
            m_Params.vsync = 1;

            SetKeybinds();

            glfwSwapInterval(m_Params.vsync);

            //glfwSetFramebufferSizeCallback(handle, s_fbSizeCallback);
            glfwSetKeyCallback(handle, s_keyCallback);
            glfwSetWindowIconifyCallback(handle, s_iconifyCallback);

            glfwSetCursorPosCallback(m_Handle, s_cursorPosCallback);
            glfwSetScrollCallback(m_Handle, s_scrollCallback);

            return handle;
        }

        void OnUpdate()
        {
            CalcDeltaTime();
            m_Camera->OnUpdate();
        }

        void CalcDeltaTime()
        {
            static float m_LastFrame = 0.0f;
            float currentFrame = glfwGetTime();
            m_DeltaTime = currentFrame - m_LastFrame;
            m_LastFrame = currentFrame;
        }

        GLFWwindow* Handle() { return m_Handle; }

        bool Paused() { return m_Params.paused; }
        int Width() { return m_Params.width; }
        int Height() { return m_Params.height; }

        float DeltaTime() { return m_DeltaTime; }

        bool CursorVisible() { return m_Params.cursorVisible; }

        void SetCamera(Ref<Camera> cam)
        {
            m_Camera = cam;
        }

        bool KeyPressed(Input::Key keyCode)
        {
            return glfwGetKey(m_Handle, static_cast<int>(keyCode)) == GLFW_PRESS;
        }

        bool IsOpen()
        {
            return !glfwWindowShouldClose(m_Handle);
        }

        void HideCursor() {
            glfwSetInputMode(m_Handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            m_Params.cursorVisible = false;
        }

        void ShowCursor() {
            glfwSetInputMode(m_Handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            m_Params.cursorVisible = true;
        }

        void SetScrollingLocked(bool locked)
        {
            m_Params.scrollingLocked = locked;
        }

        void Close()
        {
            glfwSetWindowShouldClose(m_Handle, true);
        }

        void SetKeybinds()
        {
            Input::KeybindBindAction(Input::KeybindName::CloseWindow, [&]() { Close(); });
        }

        void GLFWTerminate()
        {
            glfwTerminate();
        }

        void GLFWSwapBuffers()
        {
            glfwSwapBuffers(m_Handle);
        }

        void GLFWPollEvents()
        {
            glfwPollEvents();
        }

        namespace
        {
            void s_fbSizeCallback(GLFWwindow* window, int width, int height)
            {
                m_Params.width = width;
                m_Params.height = height;
            }

            void s_iconifyCallback(GLFWwindow* window, int minimized)
            {
                m_Params.paused = (bool)minimized;
            }

            void s_cursorPosCallback(GLFWwindow* window, double xposIn, double yposIn)
            {
                if (!m_Camera)
                    return;
                static bool firstMouseUse = true;
                float xpos = static_cast<float>(xposIn);
                float ypos = static_cast<float>(yposIn);


                if (firstMouseUse)
                {
                    m_Params.cursorX = xpos;
                    m_Params.cursorY = ypos;
                    firstMouseUse = false;
                }

                float xoffset = xpos - m_Params.cursorX;
                float yoffset = m_Params.cursorY - ypos;

                m_Params.cursorX = xpos;
                m_Params.cursorY = ypos;

                //!m_Params.cursorVisible && 
                if (glfwGetMouseButton(m_Handle, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS ||
                    glfwGetKey(m_Handle, GLFW_KEY_LEFT_ALT) || glfwGetKey(m_Handle, GLFW_KEY_RIGHT_ALT))
                    m_Camera->ProcessMouseMovement(xoffset, yoffset);
                else if (glfwGetMouseButton(m_Handle, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
                {
                    m_Camera->MoveLeft(xoffset * DeltaTime());
                    m_Camera->MoveDown(yoffset * DeltaTime());
                }
                /*else
                    firstMouseUse = true;*/
            }

            void s_scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
            {
                if (!m_Camera || m_Params.scrollingLocked)
                    return;
                //m_Camera->ProcessMouseScroll((float)yoffset * m_MouseScrollSensivity * DeltaTime());
                m_Camera->MoveForward(yoffset * m_MouseScrollSensivity * DeltaTime());
            }

            void s_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
            {
                Input::ProcessInput(key, action);
            }

#define GL_DEBUG_SEVERITY_NOTIFICATION 0x826B
#define GL_DEBUG_TYPE_ERROR            0x824C

            void s_DebugMessageCallback(unsigned source,
                unsigned type, unsigned id, unsigned severity,
                int length, const char* message, const void* userParam)
            {
                if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
                    return;
                fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
                    (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
                    type, severity, message);
            }
        }
    }
}