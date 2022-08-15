#include "pch.h"
#include "EditorUI.h"
#include "Editor.h"

#include <ImGuizmo.h>
#include <glm/gtx/matrix_decompose.hpp>

using namespace Component;

namespace EditorUI
{
	namespace
	{
		constexpr const float DOCKSPACE_MIN_PANEL_WIDTH = 340.f;

        Ref<Framebuffer> m_ViewportFramebuffer;

        Ref<Camera> m_Camera;
        Ref<Scene> m_ActiveScene;
		int m_GizmoType = -1;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        glm::vec2 m_ViewportBounds[2]{};
        bool m_ViewportFocused = false;
        bool m_ViewportHovered = false;

        Entity m_HoveredEntity{};

		SceneHierarchyPanel m_SceneHierarchyPanel;

        ImGuiWindowFlags m_PanelFlags = ImGuiWindowFlags_None;
        bool m_PanelsMovable = false;
        bool m_DisplayControls = false;

        void SetKeybinds()
        {
            Input::SetKeybind("Gizmo None", Key::Q, KeyEvent::Press, [&]() {
                    m_GizmoType = -1;
                });
            Input::SetKeybind("Gizmo Translate", Key::W, KeyEvent::Press, [&]() {
                    m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
                });
            Input::SetKeybind("Gizmo Rotate", Key::E, KeyEvent::Press, [&]() {
                    m_GizmoType = ImGuizmo::OPERATION::ROTATE;
                });
            Input::SetKeybind("Gizmo Scale", Key::R, KeyEvent::Press, [&]() {
                    m_GizmoType = ImGuizmo::OPERATION::SCALE;
                });
        }

        void UIDrawMenuBar()
        {
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Save Scene"))
                        Editor::SaveSceneAs();

                    if (ImGui::MenuItem("Load Scene"))
                        Editor::LoadScene();

                    if (ImGui::MenuItem("Exit"))
                        Window::Close();
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Dockspace"))
                {
                    if (ImGui::MenuItem("Movable Panels", NULL, &m_PanelsMovable))
                    {
                        if (m_PanelsMovable)
                            m_PanelFlags &= ~ImGuiWindowFlags_NoMove;
                        else
                            m_PanelFlags |= ImGuiWindowFlags_NoMove;
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Controls"))
                {
                    ImGui::MenuItem("Display Controls", NULL, &m_DisplayControls);
                    
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }
        }

        void UIDrawGizmos()
        {
            Entity selectedEntity = m_ActiveScene->SelectedEntity();

            if (selectedEntity && m_GizmoType != -1)
            {
                ImGuizmo::SetOrthographic(false);
                ImGuizmo::SetDrawlist();

                ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

                // Camera
                const glm::mat4& cameraProjection = m_Camera->GetProjMat();
                glm::mat4 cameraView = m_Camera->GetViewMat();

                // Entity transform
                auto& tc = selectedEntity.GetComponent<Transform>();
                glm::mat4 transform = tc.GetTransformNoParent();

                // Snapping
                bool snap = Window::KeyPressed(Key::LeftCtrl);
                float snapValue = 0.5f; // Snap to 0.5m for translation/scale
                // Snap to 45 degrees for rotation
                if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
                    snapValue = 45.0f;

                float snapValues[3] = { snapValue, snapValue, snapValue };

                ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                    (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::WORLD, glm::value_ptr(transform),
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

        void UIDrawViewport()
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

            ImGui::Begin("Viewport", (bool*)0, m_PanelFlags);

                auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
                auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
                auto viewportOffset = ImGui::GetWindowPos();
                m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
                m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };
                glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
                m_ViewportFocused = ImGui::IsWindowFocused();
                m_ViewportHovered = ImGui::IsWindowHovered();

            //ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            unsigned textureID = 0;
            textureID = m_ViewportFramebuffer->GetColorAttachmentId(0);
            if (m_ViewportSize != *((glm::vec2*)&viewportSize))
            {
                m_ViewportFramebuffer->Invalidate(viewportSize.x, viewportSize.y);

                m_ViewportSize = { viewportSize.x, viewportSize.y };

                m_Camera->UpdateProjMat(viewportSize.x, viewportSize.y);
            }
            ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y },
                ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

            // Gizmos
            UIDrawGizmos();

            //Mouse picking
            auto [mx, my] = ImGui::GetMousePos();
            mx -= m_ViewportBounds[0].x;
            my -= m_ViewportBounds[0].y;
            
            my = viewportSize.y - my;
            int mouseX = (int)mx;
            int mouseY = (int)my;

            if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
            {
                m_ViewportFramebuffer->Bind();
                int pixelData = m_ViewportFramebuffer->ReadPixelInt(mouseX, mouseY);
                m_ViewportFramebuffer->Unbind();
                
                m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
                if (Input::IsMouseButtonDown(MouseButton::Left))
                {
                    if (m_ViewportHovered && !ImGuizmo::IsOver())
                        m_ActiveScene->SelectEntity(m_HoveredEntity);
                }
            }

            ImGui::End();
            ImGui::PopStyleVar();
        }

        
	}

    void EditorUI::Init(Ref<Framebuffer> viewportFB, Ref<Camera> camera, Ref<Scene> activeScene)
    {
        m_ViewportFramebuffer = viewportFB;
        m_Camera = camera;
        m_ActiveScene = activeScene;
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
        SetKeybinds();
    }

    void EditorUI::Render()
    {
        ImguiLayer::Begin();

        //ImGui::ShowDemoWindow();
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
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
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

        ImGui::Begin("DockSpace", &p_open, window_flags);


        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);


        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        //Set min panel width
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = DOCKSPACE_MIN_PANEL_WIDTH;

        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = minWinSizeX;

        UIDrawMenuBar();

        //Prevent camera Z movement if viewport is not hovered.
        Window::SetScrollingLocked(!m_ViewportHovered);
            
        m_SceneHierarchyPanel.OnImGuiRender(m_PanelFlags);

        UIDrawViewport();

        ImGui::End();

        m_ActiveScene->OnImGuiRender(m_PanelFlags);

        if (m_DisplayControls)
            Input::UIDisplayControlsInfo(&m_DisplayControls, m_PanelFlags);

        //ImGui::ShowDemoWindow();

        ImguiLayer::End(Window::Width(), Window::Height());
    }
}