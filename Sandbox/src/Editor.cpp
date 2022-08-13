#include "pch.h"
#include "Editor.h"
#include "scenes/TestScene.h"
#include "scene/SceneSerializer.h"
#include "platform/WindowsUtils.h"
#include "ui/EditorUI.h"

namespace Editor
{
    namespace
    {
        constexpr const unsigned WINDOW_WIDTH = 1600;
        constexpr const unsigned WINDOW_HEIGHT = 900;
        constexpr const float    DOCKSPACE_MIN_PANEL_WIDTH = 340.f;

        Ref<Camera>    m_Camera{};
        Ref<TestScene> m_ActiveScene{};
    }

    void Editor::Init()
    {
        Window::Open(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL_Demo");

        Renderer::Init(WINDOW_WIDTH, WINDOW_HEIGHT);
        ImguiLayer::Init(Window::Handle(), "Sandbox");
        GeoData::Init();

        m_Camera = CreateRef<Camera>(glm::vec3(0.f, 15.f, 30.f));
        Window::SetCamera(m_Camera);

        m_ActiveScene = CreateRef<TestScene>(m_Camera);

        EditorUI::Init(m_Camera, m_ActiveScene);
    }

    void Editor::Run()
    {
        while (Window::IsOpen())
        {
            Window::GLFWPollEvents();
            if (Window::Paused())
                continue;

            Window::OnUpdate();

            Renderer::GetMainFB()->Bind();
            Renderer::Clear(GLBuffer::Color | GLBuffer::Depth | GLBuffer::Stencil);
            Renderer::GetMainFB()->ClearIntAttachment(-1);
           

            m_ActiveScene->OnUpdate(Window::DeltaTime());

            EditorUI::Render();

            Window::GLFWSwapBuffers();
        }
    }

    void Editor::SaveSceneAs()
    {
        std::optional<std::string> filepath = WinUtils::SaveFile("Game Scene (*.json)\0*.json\0");
        if (filepath)
        {
            SceneSerializer ss(m_ActiveScene);
            ss.SaveScene(*filepath);
        }
    }

    void Editor::LoadScene()
    {
        std::optional<std::string> filepath = WinUtils::OpenFile("Game Scene (*.json)\0*.json\0");
        if (filepath)
        {
            SceneSerializer ss(m_ActiveScene);
            ss.LoadScene(*filepath);
        }
    }

  /*  const Camera& GetCamera()
    {
        return m_Camera;
    }

    const Scene& GetActiveScene()
    {
        return m_ActiveScene;
    }*/
}