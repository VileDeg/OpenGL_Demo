#include "pch.h"
#include "Editor.h"
#include "scenes/TestScene.h"
#include "scene/SceneSerializer.h"
#include "platform/WindowsUtils.h"
#include "ui/EditorUI.h"

namespace Crave
{
    namespace Editor
    {
        namespace
        {
            constexpr const char* PROJECT_NAME = "CAVERN";
            constexpr const unsigned WINDOW_WIDTH = 1920;
            constexpr const unsigned WINDOW_HEIGHT = 1080;

            constexpr const float    DOCKSPACE_MIN_PANEL_WIDTH = 340.f;

            Ref<Framebuffer> m_ViewportFramebuffer{};
            Ref<Camera>      m_Camera{};
            Ref<TestScene>   m_ActiveScene{};
        }

        void Editor::Init()
        {
            Log::Init();
            Input::Init();

            Window::Open(WINDOW_WIDTH, WINDOW_HEIGHT, PROJECT_NAME);
            m_Camera = CreateRef<Camera>(glm::vec3(0.f, 15.f, 30.f));
            Window::SetCamera(m_Camera);

            m_ViewportFramebuffer = CreateRef<Framebuffer>(Framebuffer::Config{ 
                true, std::initializer_list<Texture::Config>{
                { Texture::Type::RGBA, Texture::Target::Texture2D, Texture::MMFilter::Linear, Texture::WrapMode::ClampToEdge },
                { Texture::Type::Integer, Texture::Target::Texture2D, Texture::MMFilter::Linear, Texture::WrapMode::ClampToEdge },
                { Texture::Type::DepthNStencil, Texture::Target::Texture2D }
            } });
            m_ViewportFramebuffer->Invalidate({ WINDOW_WIDTH, WINDOW_HEIGHT });

            Renderer::Init(m_ViewportFramebuffer, WINDOW_WIDTH, WINDOW_HEIGHT);
            ImguiLayer::Init(Window::Handle(), "CavernEditor");
            GeoData::Init();

            m_ActiveScene = CreateRef<TestScene>(m_Camera);

            EditorUI::Init(m_ViewportFramebuffer, m_Camera, m_ActiveScene);
        }

        void Editor::Run()
        {
            while (Window::IsOpen())
            {
                Window::GLFWPollEvents();
                if (Window::Paused())
                    continue;

                Window::OnUpdate();

                m_ViewportFramebuffer->Bind();
                Renderer::Clear(GLBuffer::Color | GLBuffer::Depth | GLBuffer::Stencil);
                m_ViewportFramebuffer->ClearIntAttachment(-1);


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
    }
}