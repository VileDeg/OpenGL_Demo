#include "pch.h"
#include "TestRenderer.h"
#include "imgui/imgui.h"

#include "math_headers.h"
#include "Window.h"

//#include "..\..\OpenGL_Demo\vendor\GLAD\include\glad\glad.h"

namespace test
{
    static Spotlight LightParams;

    TestRenderer::TestRenderer(Window& window)
        : Test(window),
        m_Camera(window, glm::vec3(0.0f, 0.0f, 10.0f)),
        m_Cube(Primitive::Cube, "container2.png", "container2_specular.png"),
        m_LightCube(Primitive::Cube),
        m_CamSpeed(8.0f)
    {

        m_Window.SetCamera(&m_Camera);

        {
            LightParams.position = m_Camera.Position();
            //LightParams.viewPos = m_Camera.Position();
            LightParams.direction = m_Camera.Front();
            LightParams.ambient = glm::vec3(0.3f);
            LightParams.diffuse = glm::vec3(0.9f);
            LightParams.specular = glm::vec3(1.f);

            LightParams.constant = 0.3f/2;
            LightParams.linear = 0.09f/2;
            LightParams.quadratic = 0.032f/2;

            LightParams.cutOff = glm::cos(glm::radians(12.5f));
            LightParams.outerCutOff = glm::cos(glm::radians(17.5f));
        }
        /*m_LightUbo = CreateRef<UBO>(
            "LightParams", (const void*)&LightParams, 5 * 12 + 5 * 4);*/
        //Renderer::UploadSpotlightData(LightParams, m_LightUbo);

        int lightsCount = 1;
        std::size_t dirSize = 12 * 4;
        std::size_t pointSize = 12 * 4 + 4 * 3;
        std::size_t spotSize = 12 * 5 + 4 * 5;
        m_LightSSBO = CreateRef<UBO>(
            "LightData", (const void*)NULL,
            dirSize + spotSize + pointSize * lightsCount);

        

        {
            using Type = Renderer::ShaderType;

            Renderer::SetUniformBuffer(m_LightSSBO, 1, { Type::DiffNSpec });

            //Renderer::SetUniformBuffer(m_LightUbo,  1, { Type::DiffNSpec });
        }
        
        m_LightCube.ScaleTo(0.2f);
    }

    static float DeltaTime = 0.f;

    void TestRenderer::OnUpdate(float deltaTime)
    {
        DeltaTime = deltaTime;
        Renderer::BeginScene(m_Camera);

            {
                m_LightSSBO->Upload(glm::value_ptr(m_Camera.Position()), 12, 0 );
                m_LightSSBO->Upload(glm::value_ptr(m_Camera.Front()   ), 12, 16);
                //m_LightUbo->Upload(glm::value_ptr(m_Camera.Position()), 16, 32);
            }
            
            int num = 10;
            for (int j = 0; j < num; ++j)
            {
                for (int i = 0; i < num; ++i)
                {
                    for (int k = 0; k < num; ++k)
                    {
                        m_Cube.TranslateTo(glm::vec3(i * 2.f, k * 2.f, j * 2.f));
                        m_Cube.DrawSpecular();
                    }
                }
            }
            m_LightCube.TranslateTo(LightParams.position);
            m_LightCube.DrawColor(glm::vec4(1.f));

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
