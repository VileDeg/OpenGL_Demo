#include "Editor.h"
#include "scenes/TestScene.h"
#include "scene/SceneSerializer.h"
#include "platform/WindowsUtils.h"

Editor::Editor()
{
    m_Context = &GLContext::Create();
    GLFWwindow* windowHandle = m_Context->OpenWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL_Demo");
    m_Window = &m_Context->GetWindow();

    Renderer::Init(WINDOW_WIDTH, WINDOW_HEIGHT);
    ImguiLayer::Init(windowHandle, "Sandbox");
    GeoData::Init();

    m_Camera = CreateRef<Camera>(*m_Window, glm::vec3(0.f, 0.f, 10.f));
    m_Window->SetCamera(m_Camera);

    m_ActiveScene = CreateRef<TestScene>(*m_Window, *m_Camera);
    m_SceneHierarchyPanel.SetContext(m_ActiveScene);
}

void Editor::Run()
{
    while (m_Window->IsOpen())
    {
        m_Context->PollEvents();
        if (m_Window->Paused())
            continue;

        m_Context->UpdateWindows();

        Renderer::SetClearColor(0.049f, 0.0f, 0.1f, 1.f);
        Renderer::Clear(3);
        
        m_ActiveScene->OnUpdate(m_Window->DeltaTime());
        
        UIRender();

        m_Context->SwapBuffers();
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
                m_Window->Close();
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
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
    ImGui::End();
    ImGui::PopStyleVar();
}

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
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
    
        UIDrawMenuBar();

        m_SceneHierarchyPanel.OnImGuiRender();

        UIDrawViewport();

    ImGui::End();

    m_ActiveScene->OnImGuiRender();

    ImguiLayer::End(m_Window->Width(), m_Window->Height());
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

    m_Context->Terminate();
}