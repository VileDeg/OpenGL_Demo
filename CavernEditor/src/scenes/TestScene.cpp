#include "pch.h"
#include "TestScene.h"
#include "renderer/MeshManager.h"

namespace Crave
{
    using namespace Component;

    static glm::vec3 s_PointLightPos = { 0.0f, 0.0f, 7.5f };
    static glm::vec3 s_DirLightPos = { -20.f, 20.f, 0.f };
    static glm::vec3 s_SpotLightPos = { 7.5f, 0.f, 0.f };
 
    static glm::ivec2 s_SpotLightsGrid = {3, 3};
    static int s_SpotLightsTotal = s_SpotLightsGrid.x * s_SpotLightsGrid.y;
    static int num = 2;
    TestScene::TestScene(Ref<Camera> camera)
        : m_Camera(camera),

        m_CamSpeed(6.f)
    {
        m_CubeMesh = MeshManager::GetPrimitiveMesh({ Primitive::Cube,
            {
                { Mesh::TexType::Diffuse,  {"container2.png"         } },
                { Mesh::TexType::Specular, {"container2_specular.png"} }
            } });
        m_BrickwallMesh = MeshManager::GetPrimitiveMesh({ Primitive::Plane,
            {
                { Mesh::TexType::Diffuse, {"brickwall.jpg"       } }
            } });

        m_WorldCenter = CreateEntity("WorldCenter");
        m_WorldCenter.AddComponent<MeshInstance>(m_CubeMesh, false);
        m_WorldCenter.GetComponent<MeshInstance>().Color = { 1.f, 1.f, 1.f, 1.f };
        m_WorldCenter.GetComponent<Transform>().ScaleF(0.2f);


#if 1
        float wallscale = 10.f;
        {
            float yoffset = -7.5f;
            float halfscale = wallscale * 0.5f + 10.f;
            glm::vec3 pos{};
            int total = s_SpotLightsTotal;
            int gridx = s_SpotLightsGrid.x;
            int gridy = s_SpotLightsGrid.y;
            float step = wallscale;
            glm::vec3 cubeoffset = { 0.f, 0.f, -step / 4 };
            glm::vec3 lowerleft = { -step * (gridx - 1) / 2.f, yoffset, -step * (gridy - 1) / 2.f };

            for (int i = 0; i < total; ++i)
            {
                glm::vec3 pos = lowerleft + cubeoffset + glm::vec3(step * (i % gridy), 0.f, step * (i / gridx));
                auto cube = CreateEntity("Cube_" + std::to_string(i));
                cube.AddComponent<MeshInstance>(m_CubeMesh);
                cube.GetComponent<Transform>().Position = pos;
            }
            float floorscale = 40.f;
            float floorhaflsc = floorscale * 0.5f;
            auto floor = CreateEntity("Floor");
            floor.AddComponent<MeshInstance>(m_CubeMesh);
            float floory = yoffset - floorhaflsc - wallscale * 0.5f;
            floor.GetComponent<Transform>().Position = glm::vec3(0.f, floory, 0.f);
            floor.GetComponent<Transform>().ScaleF(floorscale);
        }
#endif
#if 1
        {
            float halfscale = wallscale * 0.5f + 10.f;
            glm::vec3 pos{};
            int total = s_SpotLightsTotal;
            int gridx = s_SpotLightsGrid.x;
            int gridy = s_SpotLightsGrid.y;
            float step = wallscale;
            glm::vec3 walloffset = { 0.f, 0.f, -step / 2 };
            glm::vec3 lowerleft = { -step * (gridx - 1) / 2.f, -7.5f, -step * (gridy - 1) / 2.f };

            for (int i = 0; i < total; ++i)
            {
                glm::vec3 pos = lowerleft + walloffset + glm::vec3(step * (i % gridy), 0.f, step * (i / gridx));
                auto brickwall = CreateEntity("Brickwall_" + std::to_string(i));
                brickwall.AddComponent<MeshInstance>(m_BrickwallMesh);
                brickwall.GetComponent<Transform>().ScaleF(wallscale);
                brickwall.GetComponent<Transform>().Position = pos;
            }
        }
#endif

#if 1
        {
            glm::vec3 pos{};
            int total = s_SpotLightsTotal;
            int gridx = s_SpotLightsGrid.x;
            int gridy = s_SpotLightsGrid.y;
            float step = wallscale;
            glm::vec3 lowerleft = { -step * (gridx - 1) / 2.f, -7.5f, -step * (gridy - 1) / 2.f };
            for (int i = 0; i < total; ++i)
            {
                glm::vec3 pos = lowerleft + glm::vec3(step * (i % gridy), 0.f, step * (i / gridx));
                Entity pl = CreateEntity("PointLight_" + std::to_string(i));
                pl.AddComponent<MeshInstance>(m_CubeMesh, false);

                pl.GetComponent<MeshInstance>().Color = { 1.f, 1.f, 1.f, 1.f };

                pl.GetComponent<Transform>().Position = pos;
                pl.GetComponent<Transform>().ScaleF(0.2f);

                auto& l = pl.AddComponent<Light>(LightType::Point, true);
                l.Data.color = { 0.f, 0.2f, 0.8f };
                l.Data.brightness *= 3.f;
            }
        }
#endif
   
    }

    static float DeltaTime = 0.f;
    static bool  RotateLight = false;
    static bool  LightOn = true;
    static bool  CastShadows = true;
    static float LightRotSpeed = 150.f;

    static float PlaneAngle = 0.f;
    static glm::vec3 PlaneAxis = { 0.f, 1.f, 0.f };
    glm::vec3 RotPoint = { 0.f, 0.f, 0.f };
    glm::vec3 RotAxis = { 0.f, 1.0f, 0.f };

    void TestScene::OnUpdate(float deltaTime)
    {
        DeltaTime = deltaTime;
        Renderer::BeginScene(m_Camera, CastShadows);


        Scene::OnUpdate(deltaTime);

        Renderer::EndScene();
        m_Camera->SetSpeed(m_CamSpeed);
    }

    void TestScene::OnImGuiRender(ImGuiWindowFlags panelFlags)
    {
        ImGui::Begin("LevelMenu", (bool*)0, panelFlags);

        static float min = 0.0f;
        static int x, y, z;
        ImGui::SliderFloat("Camera Speed", &m_CamSpeed, min, 10.0f);
        ImGui::Checkbox("Cast Shadows", &CastShadows);
        ImGui::Checkbox("Rotate Light", &RotateLight);
        ImGui::SliderFloat("Light Rot Speed", &LightRotSpeed, min, 300.f);
        ImGui::SliderFloat3("Light Rot Point", glm::value_ptr(RotPoint), -15.f, 15.f);

        ImGui::End();

        MeshManager::OnImGuiRender(panelFlags);

        ImGui::Begin("Stats", (bool*)0, panelFlags);

        ImGui::Text("Number of entities: ");
        ImGui::SameLine();
        ImGui::Text(std::to_string(m_NumOfEntities).c_str());
        ImGui::Text("Frame Rate:	     ");
        ImGui::SameLine();
        ImGui::Text(std::to_string(1 / DeltaTime).c_str());

        ImGui::End();
    }

    TestScene::~TestScene()
    {

    }
}