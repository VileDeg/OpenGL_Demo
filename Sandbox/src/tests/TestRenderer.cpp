#include "pch.h"
#include "TestRenderer.h"
#include "imgui/imgui.h"

#include "math_headers.h"
#include "Window.h"

//#include "..\..\OpenGL_Demo\vendor\GLAD\include\glad\glad.h"

namespace test
{
    void test::TestRenderer::SetLightParams()
    {
        glm::vec3 ambient = glm::vec3(0.3f);
        glm::vec3 diffuse = glm::vec3(0.8f);
        glm::vec3 specular = glm::vec3(1.f);

        float constant = 0.3f / 2;
        float linear = 0.09f / 2;
        float quadratic = 0.032f / 2;

        float cutOff = 20.f;
        //float outerCutOff = 12.5f;
            
        m_LightPositions[0] = { 10.0f, -3.0f, 0.0 };
        m_LightPositions[1] = { 0.0f, 5.0f, 0.0f };
        
        
        {
            m_DirLightParams.direction = glm::vec3(1.f, -1.f, 0.f);
            m_DirLightParams.ambient   = ambient;
            m_DirLightParams.diffuse   = diffuse;
            m_DirLightParams.specular  = specular;
        }

        {
            m_SpotLightParams.position    = m_LightPositions[0];
            m_SpotLightParams.direction   = glm::vec3(-1.0f, 0.0f, 0.0f);;
            m_SpotLightParams.ambient     = ambient;
            m_SpotLightParams.diffuse     = diffuse;
            m_SpotLightParams.specular    = specular;
                                          
            m_SpotLightParams.constant    = constant;
            m_SpotLightParams.linear      = linear;
            m_SpotLightParams.quadratic   = quadratic;

            m_SpotLightParams.cutOff      = glm::cos(glm::radians(cutOff));
            m_SpotLightParams.outerCutOff = glm::cos(glm::radians(cutOff+5));
        }

        {
            m_PointLightParams.position  = m_LightPositions[1];
            m_PointLightParams.ambient   = ambient;
            m_PointLightParams.diffuse   = diffuse;
            m_PointLightParams.specular  = specular;

            m_PointLightParams.constant  = constant;
            m_PointLightParams.linear    = linear;
            m_PointLightParams.quadratic = quadratic;
        }
    }

    TestRenderer::TestRenderer(Window& window)
        : Test(window),
        m_Camera(window, glm::vec3(0.0f, 0.0f, 10.0f)),
        m_Cube(Primitive::Cube, "container2.png", "container2_specular.png"),
        m_LightCube(Primitive::Cube),
        m_CamSpeed(8.0f)
    {

        m_Window.SetCamera(&m_Camera);

        SetLightParams();

        {
            using Type = Renderer::LightType;
            Renderer::UploadLightData(Type::Directional, &m_DirLightParams);
            Renderer::UploadLightData(Type::Spot, &m_SpotLightParams);
            Renderer::UploadLightData(Type::Point, &m_PointLightParams);
        }

        m_LightCube.ScaleTo(0.2f);
    }

    static float DeltaTime = 0.f;

    void TestRenderer::OnUpdate(float deltaTime)
    {
        DeltaTime = deltaTime;
        Renderer::BeginScene(m_Camera);

            {
                /*Renderer::UpdateLightData(Renderer::LightType::Spot,
                    glm::value_ptr(m_Camera.Position()), 12, 0);

                Renderer::UpdateLightData(Renderer::LightType::Spot,
                    glm::value_ptr(m_Camera.Front())   , 12, 16);*/
                /*m_LightSSBO->Upload(glm::value_ptr(m_Camera.Position()), 12, 0 );
                m_LightSSBO->Upload(glm::value_ptr(m_Camera.Front()   ), 12, 16);*/
                //m_LightUbo->Upload(glm::value_ptr(m_Camera.Position()), 16, 32);
            }
            
            int num = 10;
            int h = num / 2;
            for (int j = 0; j < num; j += 2)
            {
                for (int i = 0; i < num; i += 2)
                {
                    for (int k = 0; k < num; k += 2)
                    {
                        m_Cube.TranslateTo(glm::vec3(i-h, k- h, j- h));
                        m_Cube.DrawSpecular();
                    }
                }
            }
            
            m_LightCube.TranslateTo(m_LightPositions[0]);
            m_LightCube.DrawColor(glm::vec4(1.0f, 0.f, 0.f, 1.f));
            m_LightCube.TranslateTo(m_LightPositions[1]);
            m_LightCube.DrawColor(glm::vec4(0.0f, 1.f, 0.f, 1.f));
            
            
            Renderer::DrawSkybox();

            m_Camera.SetSpeed(m_CamSpeed);

        Renderer::EndScene();
    }

    void TestRenderer::OnImGuiRender()
    {
        ImGui::Begin("LevelMenu");
        static float min = 0.0f;
        static int x, y, z;
        ImGui::SliderFloat("Camera Speed", &m_CamSpeed, min, 10.0f);
        ImGui::LabelText("Frame Rate", "%f", 1 / DeltaTime);
       
        ImGui::End();
    }
}
