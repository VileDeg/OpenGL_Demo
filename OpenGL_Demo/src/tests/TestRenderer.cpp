#include "pch.h"
#include "TestRenderer.h"


#include "imgui.h"

#include "math_headers.h"
#include "Window.h"
#include "renderer/Renderer.h"

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

    //static struct
    //{
    //    bool enableOutline = true;
    //    bool enableRotation = false;
    //    int rotationSpeed = 1;
    //    glm::vec3 rotationAxis = glm::vec3(0.f, 1.f, 0.f);
    //    int outlineBorderWidth = 1;
    //} s_configs;



    //Shader("material_vert.shader", "color_frag.shader")
    TestRenderer::TestRenderer(Window& window)
        : Test(window),
        m_Camera(window, glm::vec3(0.0f, 0.0f, 10.0f)),
        m_Cube(Primitive::Cube, "container2.png"),
        m_CamSpeed(8.0f)
        //m_OutlineCube(Primitive::Cube)
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

      
        Renderer::Init();
    }



    void TestRenderer::OnRender()
    {

    }
    void TestRenderer::OnUpdate(float deltaTime)
    {
        Renderer::BeginScene(m_Camera);

       /* m_Cube.TranslateTo({ 0.f, 0.f, 0.f });
        m_Cube.DrawColor({ 1.f,1.f,1.f,1.f });*/
        //m_Cube.Translate({0.f,0.f,0.1f*deltaTime});
        
        //m_Cube.TranslateTo({5.f, 0.f, 5.f});
        m_Cube.Draw();
           
    
   

        
        m_Camera.SetSpeed(m_CamSpeed);
        Renderer::EndScene();
    }

    void TestRenderer::OnImGuiRender()
    {
        //ImGui::ShowDemoWindow();
        ImGui::Begin("LevelMenu");
        static float min = 0.0f;
        ImGui::SliderFloat("Camera Speed", &m_CamSpeed, min, 10.0f);
        ImGui::End();
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
