#include "pch.h"
#include "Editor.h"
#include "scenes/TestScene.h"
#include "scene/SceneSerializer.h"
#include "platform/WindowsUtils.h"

#include <ImGuizmo.h>
#include <glm/gtx/matrix_decompose.hpp>

Editor::Editor()
{
    Window::Open(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL_Demo");

    Renderer::Init(WINDOW_WIDTH, WINDOW_HEIGHT);
    ImguiLayer::Init(Window::Handle(), "Sandbox");
    GeoData::Init();

    m_Camera = CreateRef<Camera>(glm::vec3(0.f, 0.f, 10.f));
    Window::SetCamera(m_Camera);

    m_ActiveScene = CreateRef<TestScene>(*m_Camera);
    m_SceneHierarchyPanel.SetContext(m_ActiveScene);

    Input::SetKeybind(Key::Q, KeyEvent::Press, [&]() {
            if (Window::CursorVisible())
                m_GizmoType = -1;
        });
    Input::SetKeybind(Key::W, KeyEvent::Press, [&]() {
            if (Window::CursorVisible())
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
        });
    Input::SetKeybind(Key::E, KeyEvent::Press, [&]() {
            if (Window::CursorVisible())
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
        });
    Input::SetKeybind(Key::R, KeyEvent::Press, [&]() {
            if (Window::CursorVisible())
                m_GizmoType = ImGuizmo::OPERATION::SCALE;
        });
}

void Editor::Run()
{
    while (Window::IsOpen())
    {
        Window::GLFWPollEvents();
        if (Window::Paused())
            continue;

        Window::OnUpdate();

        Renderer::SetClearColor(0.049f, 0.0f, 0.1f, 1.f);
        Renderer::Clear(3);
        
        m_ActiveScene->OnUpdate(Window::DeltaTime());
        
        UIRender();

        Window::GLFWSwapBuffers();
    }
}

void Editor::UIDrawMenuBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save Scene"))
                SaveSceneAs();

            if (ImGui::MenuItem("Load Scene"))
                LoadScene();

            if (ImGui::MenuItem("Exit"))
                Window::Close();
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void Editor::UIDrawGizmos()
{
    Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();

    if (selectedEntity && m_GizmoType != -1)
    {
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();

        float windowWidth = (float)ImGui::GetWindowWidth();
        float windowHeight = (float)ImGui::GetWindowHeight();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

        // Camera
        const glm::mat4& cameraProjection = m_Camera->GetProjMat();
        glm::mat4 cameraView = m_Camera->GetViewMat();

        // Entity transform
        auto& tc = selectedEntity.GetComponent<TransformComponent>();
        glm::mat4 transform = tc.GetTransform();

        // Snapping
        bool snap = Window::KeyPressed(Key::LeftCtrl);
        float snapValue = 0.5f; // Snap to 0.5m for translation/scale
        // Snap to 45 degrees for rotation
        if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
            snapValue = 45.0f;

        float snapValues[3] = { snapValue, snapValue, snapValue };

        ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
            (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
            nullptr, snap ? snapValues : nullptr);

        if (ImGuizmo::IsUsing())
        {
            glm::vec3 scale;
            glm::quat rotation;
            glm::vec3 translation;
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose(transform, scale, rotation, translation, skew, perspective);

            glm::vec3 deltaRotation = glm::degrees(glm::eulerAngles(rotation)) - tc.EulerAngles;

            tc.Position = translation;
            tc.Quaternion = rotation;
            tc.EulerAngles += deltaRotation;

            tc.Quaternion = rotation;
            tc.Scale = scale;
        }
    }
}

void Editor::UIDrawViewport()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Viewport");

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    unsigned textureID = 0;
    textureID = Renderer::GetFBColorAttachmentID();
    if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
    {
        Renderer::SetRenderImageSize(
            (unsigned)viewportPanelSize.x, (unsigned)viewportPanelSize.y);

        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        m_Camera->UpdateProjMat(viewportPanelSize.x, viewportPanelSize.y);
    }
    ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y },
        ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

    // Gizmos
    UIDrawGizmos();

    ImGui::End();
    ImGui::PopStyleVar();
}

static float s_DockWindowMinSizeX = 300.f;
void Editor::UIRender()
{
    ImguiLayer::Begin();

    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    static bool p_open = true;
    ImGui::Begin("DockSpace Demo", &p_open, window_flags);

        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = s_DockWindowMinSizeX;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = minWinSizeX;
    
        UIDrawMenuBar();

        m_SceneHierarchyPanel.OnImGuiRender();

        UIDrawViewport();

    ImGui::End();

    m_ActiveScene->OnImGuiRender();

    ImguiLayer::End(Window::Width(), Window::Height());
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

Editor::~Editor()
{
    ImguiLayer::Shutdown();
    Renderer::Shutdown();

    Window::GLFWTerminate();
}