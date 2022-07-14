#include "pch.h"
#include "OpenGL_Demo.h"
#include "TestRenderer.h"

namespace test
{
    void test::TestRenderer::SetLightParams()
    {
        glm::vec3 ambient = glm::vec3(0.1f);
        glm::vec3 diffuse = glm::vec3(0.8f);
        glm::vec3 specular = glm::vec3(1.f);

        float constant = 0.3f / 2;
        float linear = 0.09f / 2;
        float quadratic = 0.032f / 2;

        float cutOff = 20.f;
        //float outerCutOff = 12.5f;
            
        m_LightPositions[0] = { 0.0f, 0.0f, 7.5f };
        m_LightPositions[1] = { 10.0f, -3.0f, 0.0 };
        
        
        {
            m_DirLightParams.type = 0; //Dir
            m_DirLightParams.direction = glm::vec3(1.f, -1.f, 0.f);
            m_DirLightParams.ambient   = ambient;
            m_DirLightParams.diffuse   = diffuse;
            m_DirLightParams.specular  = specular;
        }

        {
            m_SpotLightParams.type = 2; //Spot
            m_SpotLightParams.position    = m_LightPositions[1];
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
            m_PointLightParams.type = 1; //Point
            m_PointLightParams.position  = m_LightPositions[0];
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
        m_Camera(window, glm::vec3(0.f, 10.f, 15.f)),
        m_Scene(CreateRef<Scene>()),
        m_CubeMesh(GeoData::GetData(Primitive::Cube).data,
            GeoData::GetData(Primitive::Cube).size, 
            GeoData::GetData(Primitive::Cube).count,
            { 
                {TexType::Diffuse,  "container2.png"         },
                {TexType::Specular, "container2_specular.png"} 
            }),
        m_CamSpeed(8.0f)
    {
        m_Window.SetCamera(&m_Camera);

        SetLightParams();

        /*m_Cubes[0] = m_Scene->CreateEntity("Cube" + std::to_string(0));
        m_Cubes[0].AddComponent<MeshComponent>(&m_CubeMesh);
        m_Cubes[0].GetComponent<TransformComponent>().
            TranslateTo(glm::vec3(0.f, 0.f, 0.f));*/

        int num = 10;
        int h = num / 2;
        for (int j = 0; j < num; j += 2)
        {
            for (int i = 0; i < num; i += 2)
            {
                for (int k = 0; k < num; k += 2)
                {
                    m_Cubes[i + j + k] = m_Scene->CreateEntity("Cube" + std::to_string(i));
                    m_Cubes[i + j + k].AddComponent<MeshComponent>(&m_CubeMesh);
                    m_Cubes[i + j + k].GetComponent<TransformComponent>().
                        TranslateTo(glm::vec3(i - h, k - h, j - h));
                }
            }
        }
        /*for (int i = 0; i < 1; i++)
        {
            m_LightCubes[i] = m_Scene->CreateEntity("LightCube" + std::to_string(i));
            m_LightCubes[i].AddComponent<MeshComponent>(&m_CubeMesh);

            m_LightCubes[i].GetComponent<MeshComponent>().HasTextures = false;
            m_LightCubes[i].GetComponent<MeshComponent>().Color = { 1.f, 1.f, 1.f, 1.f };

            m_LightCubes[i].GetComponent<TransformComponent>().TranslateTo(m_LightPositions[i]);
            m_LightCubes[i].GetComponent<TransformComponent>().ScaleTo(0.2f);
        }*/
        m_LightCubes[0] = m_Scene->CreateEntity("LightCube0");
        m_LightCubes[0].AddComponent<MeshComponent>(&m_CubeMesh);
                     
        m_LightCubes[0].GetComponent<MeshComponent>().HasTextures = false;
        m_LightCubes[0].GetComponent<MeshComponent>().Color = { 1.f, 1.f, 1.f, 1.f };
                     
        m_LightCubes[0].GetComponent<TransformComponent>().TranslateTo(m_LightPositions[0]);
        m_LightCubes[0].GetComponent<TransformComponent>().ScaleTo(0.2f);

        m_LightCubes[0].AddComponent<LightComponent>(m_PointLightParams, true);
        /*m_LightCubes[0] = m_Scene->CreateEntity("LightCube0");
        m_LightCubes[0].AddComponent<LightComponent>(m_DirLightParams);
        m_LightCubes[1].AddComponent<LightComponent>(m_SpotLightParams);
        m_LightCubes[2].AddComponent<LightComponent>(m_PointLightParams);*/
        m_Room = m_Scene->CreateEntity("Room");
        auto& mesh = m_Room.AddComponent<MeshComponent>(&m_CubeMesh);
        mesh.FlipNormals = true;
        m_Room.GetComponent<TransformComponent>().ScaleTo(35.f);

    }

    static float DeltaTime = 0.f;
    static float LightRotSpeed = 100.f;
    glm::vec3 RotAxis = { 0.f, 1.0f, 0.f };

    void TestRenderer::OnUpdate(float deltaTime)
    {
        DeltaTime = deltaTime;
        Renderer::BeginScene(m_Camera);

        auto& tr = m_LightCubes[0].GetComponent<TransformComponent>();

        glm::quat quatRot = glm::angleAxis(glm::radians(LightRotSpeed * deltaTime), glm::normalize(RotAxis));
        glm::mat4x4 matRot = glm::mat4_cast(quatRot);
        
        tr.Transform = matRot * tr.Transform;
        //tr = glm::rotate(tr.Transform, glm::radians(LightRotSpeed * deltaTime), glm::vec3(0.f, 1.0f, 0.f));

        m_Scene->OnUpdate(deltaTime);

        Renderer::EndScene();
        m_Camera.SetSpeed(m_CamSpeed);
    }

    void TestRenderer::OnImGuiRender()
    {
        ImGui::Begin("LevelMenu");
        static float min = 0.0f;
        static int x, y, z;
        ImGui::SliderFloat("Camera Speed", &m_CamSpeed, min, 10.0f);
        ImGui::SliderFloat("Light Rot Speed", &LightRotSpeed, min, 300.f);
        //ImGui::SliderFloat3("Rotation Axis", glm::value_ptr(RotAxis), min, 1.0f);
        /*static glm::vec3 lightPos = m_LightPositions[0];
        ImGui::SliderFloat3("Light Position", glm::value_ptr(lightPos), -5.f, 5.f);*/
        //m_LightCubes[0].GetComponent<TransformComponent>().TranslateTo(lightPos);
        ImGui::Text("Camera Pos:\n%.2f %.2f %.2f",
            m_Camera.Position().x, m_Camera.Position().y, m_Camera.Position().z);
        ImGui::Text("Frame Rate: %.0f", 1 / DeltaTime);
       
        ImGui::End();
    }
}
