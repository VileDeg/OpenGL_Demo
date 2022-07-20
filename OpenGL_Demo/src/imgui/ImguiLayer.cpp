#include "pch.h"
#include "ImguiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.cpp"
#include "backends/imgui_impl_glfw.cpp"
#include "GLFW/glfw3.h"



	void ImguiLayer::Init(GLFWwindow* window)
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 130"); //410
	}
	void ImguiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	//void ImguiLayer::OnUpdate()
	//{
	//	// Note: Switch this to true to enable dockspace
	//	static bool dockspaceOpen = true;
	//	static bool opt_fullscreen_persistant = true;
	//	bool opt_fullscreen = opt_fullscreen_persistant;
	//	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	//	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	//	// because it would be confusing to have two docking targets within each others.
	//	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	//	if (opt_fullscreen)
	//	{
	//		ImGuiViewport* viewport = ImGui::GetMainViewport();
	//		ImGui::SetNextWindowPos(viewport->Pos);
	//		ImGui::SetNextWindowSize(viewport->Size);
	//		ImGui::SetNextWindowViewport(viewport->ID);
	//		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	//		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	//		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	//		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	//	}

	//	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	//	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
	//		window_flags |= ImGuiWindowFlags_NoBackground;

	//	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	//	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
	//	// all active windows docked into it will lose their parent and become undocked.
	//	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
	//	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	//	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	//	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	//	ImGui::PopStyleVar();

	//	if (opt_fullscreen)
	//		ImGui::PopStyleVar(2);

	//	// DockSpace
	//	ImGuiIO& io = ImGui::GetIO();
	//	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	//	{
	//		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	//		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	//	}

	//	if (ImGui::BeginMenuBar())
	//	{
	//		if (ImGui::BeginMenu("File"))
	//		{
	//			// Disabling fullscreen would allow the window to be moved to the front of other windows, 
	//			// which we can't undo at the moment without finer window depth/z control.
	//			//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

	//			//if (ImGui::MenuItem("Exit")) Application::Get().Close();
	//			ImGui::EndMenu();
	//		}

	//		ImGui::EndMenuBar();
	//	}

	//	/*ImGui::Begin("Settings");

	//	auto stats = Hazel::Renderer2D::GetStats();
	//	ImGui::Text("Renderer2D Stats:");
	//	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	//	ImGui::Text("Quads: %d", stats.QuadCount);
	//	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	//	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	//	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	//	ImGui::End();*/

	//	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	//	ImGui::Begin("Viewport");
	//	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	//	if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
	//	{
	//		m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
	//		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

	//		m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
	//	}
	//	uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
	//	ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	//	ImGui::End();
	//	ImGui::PopStyleVar();

	//	ImGui::End();
	//}

	void ImguiLayer::End(float width, float height)
	{
		ImGuiIO& io = ImGui::GetIO();

		io.DisplaySize = ImVec2(width, height);

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

	}
	void ImguiLayer::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
