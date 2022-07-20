#include "Editor.h"

Editor::Editor()
{
    m_Context = &GLContext::Create();
    GLFWwindow* windowHandle = m_Context->OpenWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL_Demo");
    m_Window = &m_Context->GetWindow();

    Renderer::Init(WINDOW_WIDTH, WINDOW_HEIGHT);
    ImguiLayer::Init(windowHandle);

    m_Camera = CreateRef<Camera>(*m_Window, glm::vec3(0.f, 0.f, 10.f));
    m_Window->SetCamera(m_Camera);
    /*m_Framebuffer = CreateRef<Framebuffer>();
    m_Framebuffer->ResetForRender(WINDOW_WIDTH, WINDOW_HEIGHT);*/
    /*test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(*m_Window, currentTest);
    currentTest = testMenu;

    testMenu->RegisterTest<test::TestRenderer>(window, "TestRenderer");
    testMenu->RunNextTest();*/
    m_RendererTest = CreateScope<test::TestRenderer>(*m_Window);

    //m_Window->HideCursor();
}

void Editor::Run()
{
    while (m_Window->IsOpen())
    {
        m_Context->PollEvents();
        if (m_Window->Paused())
            continue;

        m_Context->UpdateWindows();

        //m_Framebuffer->Bind();
        Renderer::SetClearColor(0.049f, 0.0f, 0.1f, 1.f);
        Renderer::Clear(0b111);
        //m_Framebuffer->Unbind(WINDOW_WIDTH, WINDOW_HEIGHT);
        
        m_RendererTest->OnUpdate(m_Window->DeltaTime(), *m_Camera);
        
        ImGuiRender();

        m_Context->SwapBuffers();
    }
}

void Editor::ImGuiShowViewport()
{
    // Note: Switch this to true to enable dockspace
    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows, 
            // which we can't undo at the moment without finer window depth/z control.
            //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

            if (ImGui::MenuItem("Exit")) m_Window->Close();
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

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
            //m_Framebuffer->ResetForRender(viewportPanelSize.x, viewportPanelSize.y);

            m_Camera->UpdateProjMat(viewportPanelSize.x, viewportPanelSize.y);
        }
        ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y },
            ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::End();
}

void Editor::ImGuiRender()
{
    ImguiLayer::Begin();

    ImGuiShowViewport();

    m_RendererTest->OnImGuiRender();

    ImguiLayer::End(m_Window->Width(), m_Window->Height());
}

Editor::~Editor()
{
    /*delete testMenu;
    if (currentTest != testMenu)
        delete currentTest;*/

    ImguiLayer::Shutdown();
    Renderer::Shutdown();

    m_Context->Terminate();
}