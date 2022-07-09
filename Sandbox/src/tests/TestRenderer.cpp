#include "pch.h"
#include "TestRenderer.h"
#include "imgui/imgui.h"

#include "math_headers.h"
#include "Window.h"

//#include "..\..\OpenGL_Demo\vendor\GLAD\include\glad\glad.h"

namespace test
{
    /*static struct
    {
        glm::vec3 position;
        glm::vec3 direction;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        float constant;
        float linear;
        float quadratic;

        float cutOff;
        float outerCutOff;
    } LightParams;*/

    static Light::SpotLight LightParams;

    TestRenderer::TestRenderer(Window& window)
        : Test(window),
        m_Camera(window, glm::vec3(0.0f, 0.0f, 10.0f)),
        m_Cube(Primitive::Cube, "container2.png", "container2_specular.png"),
        m_LightCube(Primitive::Cube),
        m_CamSpeed(8.0f),
        
        m_CameraUbo(CreateRef<UBO>("ProjViewMat", (const void*)NULL, sizeof(glm::mat4))),
        m_LightUbo(CreateRef<UBO>("LightParams", (const void*)NULL, 112))
    {

        m_Window.SetCamera(&m_Camera);

        {
            LightParams.position = m_Camera.Position();
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
       

        {
            m_CameraUbo->Upload(
                glm::value_ptr(m_Camera.GetProjViewMat()), sizeof(glm::mat4), 0);
        }

        std::size_t sizef = sizeof(float); //4
        std::size_t size4f = sizef * 4; //16
        int offset = 0;
        {
            m_LightUbo->Upload(glm::value_ptr(LightParams.position), size4f, offset);
            offset += size4f;
            m_LightUbo->Upload(glm::value_ptr(LightParams.direction), size4f, offset);
            offset += size4f;
            m_LightUbo->Upload(glm::value_ptr(m_Camera.Position()), size4f, offset);
            offset += size4f;
            m_LightUbo->Upload(glm::value_ptr(LightParams.ambient), size4f, offset);
            offset += size4f;
            m_LightUbo->Upload(glm::value_ptr(LightParams.diffuse), size4f, offset);
            offset += size4f;
            m_LightUbo->Upload(glm::value_ptr(LightParams.specular), size4f - 4, offset);
            offset += size4f - 4;
            m_LightUbo->Upload(&LightParams.constant, sizef, offset);
            offset += sizef;
            m_LightUbo->Upload(&LightParams.linear, sizef, offset);
            offset += sizef;
            m_LightUbo->Upload(&LightParams.quadratic, sizef, offset);
            offset += sizef;
            m_LightUbo->Upload(&LightParams.cutOff, sizef, offset);
            offset += sizef;
            m_LightUbo->Upload(&LightParams.outerCutOff, sizef, offset);
            offset += sizef;
        }

        {
            using Type = Renderer::ShaderType;

            Renderer::SetUniformBuffer(m_CameraUbo, 0, { Type::Diffuse, Type::DiffNSpec, Type::Color });
            Renderer::SetUniformBuffer(m_LightUbo,  1, { Type::DiffNSpec });
        }
        
        m_LightCube.ScaleTo(0.2f);
    }


    static float DeltaTime = 0.f;
    void TestRenderer::OnRender()
    {

    }
    void TestRenderer::OnUpdate(float deltaTime)
    {
        DeltaTime = deltaTime;
        Renderer::BeginScene(m_Camera);

        m_CameraUbo->Upload(
            glm::value_ptr(m_Camera.GetProjViewMat()), sizeof(glm::mat4), 0);
        
        {
            m_LightUbo->Upload(glm::value_ptr(m_Camera.Position()), 16, 0 );
            m_LightUbo->Upload(glm::value_ptr(m_Camera.Front()   ), 16, 16);
            m_LightUbo->Upload(glm::value_ptr(m_Camera.Position()), 16, 32);
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
        //m_Cube.DrawSpecular();
        Renderer::DrawSkybox();

        //m_Camera.SetSpeed(m_CamSpeed);
        Renderer::EndScene();
    }

    void TestRenderer::OnImGuiRender()
    {
        //ImGui::ShowDemoWindow();
        ImGui::Begin("LevelMenu");
        static float min = 0.0f;
        static int x, y, z;
        ImGui::SliderFloat("Camera Speed", &m_CamSpeed, min, 10.0f);
        //ImGui::LabelText("Frame Rate", "%f", 60 / DeltaTime);
        /*ImGui::SliderInt("Light X", &x, min, 10);
        ImGui::SliderInt("Light Y", &y, min, 10);
        ImGui::SliderInt("Light Z", &z, min, 10);*/
        /*ImGui::SliderFloat3("Light Pos", glm::value_ptr(LightParams.position), -5.f, 5.0f);
        LightParams.position.x = (float)((int)(LightParams.position.x * 10.f)/10.f);
        LightParams.position.y= (float)((int)(LightParams.position.y * 10.f) / 10.f);
        LightParams.position.z = (float)((int)(LightParams.position.z * 10.f ) / 10.f);*/
        /*ImGui::SliderFloat("Constant", &LightParams.constant, min, 1.0f);
        ImGui::SliderFloat("Linear",   &LightParams.linear, min, 1.0f);
        ImGui::SliderFloat("Quadratic", &LightParams.quadratic, min, 1.0f);*/
        //LightParams.lightPos = { x,y,z };
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
