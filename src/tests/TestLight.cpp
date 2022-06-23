#include "TestLight.h"

#include "GLFW/glfw3.h"
#include "imgui/imgui.h"

#include "math_headers.h"
#include "GLContext.h"
#include "input/InputManager.h"


namespace test
{
    static GLContext& context = GLContext::getTnstance();
    static InputManager& inputManager = InputManager::getInstance();

    static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    static float vertices[] = {
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    static Texture tex1("res/textures/container.jpg");

    TestLight::TestLight()
        : m_Object({ vertices, sizeof(vertices), 36, 3, 0, 2 }, { tex1 },
            "light_vert.shader", "light_frag.shader")
    {
        {
            m_ObjPositions.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f));
            m_ObjPositions.emplace_back(glm::vec3(2.0f, 5.0f, -15.0f));
            m_ObjPositions.emplace_back(glm::vec3(-1.5f, -2.2f, -2.5f));
            m_ObjPositions.emplace_back(glm::vec3(-3.8f, -2.0f, -12.3f));
            m_ObjPositions.emplace_back(glm::vec3(2.4f, -0.4f, -3.5f));
            m_ObjPositions.emplace_back(glm::vec3(-1.7f, 3.0f, -7.5f));
            m_ObjPositions.emplace_back(glm::vec3(1.3f, -2.0f, -2.5f));
            m_ObjPositions.emplace_back(glm::vec3(1.5f, 2.0f, -2.5f));
            m_ObjPositions.emplace_back(glm::vec3(1.5f, 0.2f, -1.5f));
            m_ObjPositions.emplace_back(glm::vec3(-1.3f, 1.0f, -1.5f));
        }
        
        inputManager.SetCamera(&camera);
        inputManager.HideCursor();
	}

	void TestLight::OnRender()
	{
        m_Object.SetViewMat(camera.GetViewMat());
        m_Object.SetProjMat(camera.GetProjMat());

        for (int i = 0; i < m_ObjPositions.size(); ++i)
        {
            m_Object.ResetTransform();
            m_Object.Move(m_ObjPositions[i]);
            float angle = 20.0f * (i+1);
            m_Object.Rotate((float)glfwGetTime() * glm::radians(angle),
                glm::vec3(1.0f, 0.3f, 0.5f));
                
            m_Object.DrawNoIndex();
        }
	}

	void TestLight::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
