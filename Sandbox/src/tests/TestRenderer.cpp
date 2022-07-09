#include "pch.h"
#include "TestRenderer.h"
#include "imgui/imgui.h"

#include "math_headers.h"
#include "Window.h"

//#include "..\..\OpenGL_Demo\vendor\GLAD\include\glad\glad.h"

namespace test
{
    static struct
    {
        glm::vec3 lightPos;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        float constant;
        float linear;
        float quadratic;

        float shininess;

        float cutOff;
        float outerCutOff;
    } LightParams;

    static glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
    //static UBO lightBuffer;
    //unsigned ubo;


    TestRenderer::TestRenderer(Window& window)
        : Test(window),
        m_Camera(window, glm::vec3(0.0f, 0.0f, 10.0f)),
        m_Cube(Primitive::Cube, "container2.png"),
        m_CamSpeed(8.0f),
        m_CameraUbo(CreateRef<UBO>("ProjViewMat", (const void*)NULL, sizeof(glm::mat4)))
    {

        m_Window.SetCamera(&m_Camera);

        {
            LightParams.lightPos = m_Camera.Position();
            LightParams.ambient = glm::vec3(1.f);
            LightParams.diffuse = glm::vec3(1.0f);
            LightParams.specular = glm::vec3(1.f);
            LightParams.shininess = 32.0f;

            LightParams.constant = 1.0f;
            LightParams.linear = 0.09f;
            LightParams.quadratic = 0.032f;

            LightParams.cutOff = glm::cos(glm::radians(12.5f));
            LightParams.outerCutOff = glm::cos(glm::radians(17.5f));
        }

        m_CameraUbo->Upload(
            glm::value_ptr(m_Camera.GetProjViewMat()), sizeof(glm::mat4), 0);
        Renderer::SetUniformBuffer(m_CameraUbo, 0);
    }



    void TestRenderer::OnRender()
    {

    }
    void TestRenderer::OnUpdate(float deltaTime)
    {
        Renderer::BeginScene(m_Camera);

        m_CameraUbo->Upload(
            glm::value_ptr(m_Camera.GetProjViewMat()), sizeof(glm::mat4), 0);

        int num = 10;
        for (int j = 0; j < num; ++j)
        {
            for (int i = 0; i < num; ++i)
            {
                for (int k = 0; k < num; ++k)
                {
                    m_Cube.TranslateTo(glm::vec3(i * 2.f, k * 2.f, j * 2.f));
                    m_Cube.Draw();
                }
            }
        }
        Renderer::DrawSkybox();

        //m_Camera.SetSpeed(m_CamSpeed);
        Renderer::EndScene();
    }

    void TestRenderer::OnImGuiRender()
    {
        //ImGui::ShowDemoWindow();
        /*ImGui::Begin("LevelMenu");
        static float min = 0.0f;
        ImGui::SliderFloat("Camera Speed", &m_CamSpeed, min, 10.0f);
        ImGui::End();*/
        /*static float min = 0.0f;

        
        ImGui::Checkbox   ("Outline ON",   &s_configs.enableOutline         );
        if (s_configs.enableOutline)
        {
            ImGui::Checkbox ("Rotation ON", &s_configs.enableRotation);
            ImGui::SliderInt("Border Width", &s_configs.outlineBorderWidth, 0, 10);
            if (s_configs.enableRotation)
            {
                ImGui::SliderInt("Rotation Speed", &s_configs.rotationSpeed, 0, 10);
                ImGui::SliderFloat3("Rotation Axis", glm::value_ptr(s_configs.rotationAxis), 0.0f, 45.f);
            }
        }*/
    }
}
