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
        //SetLightParams(s_LightBrightness);

        m_CubeMesh = MeshManager::GetPrimitiveMesh({ Primitive::Cube,
            {
                { Mesh::TexType::Diffuse,  {"container2.png"         } },
                { Mesh::TexType::Specular, {"container2_specular.png"} }
            } });
        m_BrickwallMesh = MeshManager::GetPrimitiveMesh({ Primitive::Plane,
            {
                { Mesh::TexType::Diffuse, {"brickwall.jpg"       } }
                //{ Mesh::TexType::Normal,  {"brickwall_normal.jpg"} }
            } });

        m_WorldCenter = CreateEntity("WorldCenter");
        m_WorldCenter.AddComponent<MeshInstance>(m_CubeMesh, false);
        m_WorldCenter.GetComponent<MeshInstance>().Color = { 1.f, 1.f, 1.f, 1.f };
        m_WorldCenter.GetComponent<Transform>().ScaleF(0.2f);
#if 0
        float h = num - 1;
        for (int i = 0; i < num; ++i)
        {
            for (int j = 0; j < num; ++j)
            {
                for (int k = 0; k < num; ++k)
                {
                    int ind = i * num * num + j * num + k;
                    m_Cubes[ind] = CreateEntity("Cube" + std::to_string(ind));
                    m_Cubes[ind].AddComponent<MeshInstance>(m_CubeMesh);
                    m_Cubes[ind].GetComponent<Transform>().Position =
                        glm::vec3(i * 2.f - h, j * 2.f - h, k * 2.f - h);
                }
            }
        }
#endif

#if 0
        float scale = 30.f;
        float halfScale = scale * 0.5f;
        int total
        for (int i = 0; i < 6; ++i)
        {
            m_Brickwalls[i] = CreateEntity("Brickwall" + std::to_string(i));
            m_Brickwalls[i].AddComponent<MeshInstance>(m_BrickwallMesh);
            m_Brickwalls[i].GetComponent<Transform>().ScaleF(scale);
        }

        const glm::vec3 up{ 0.f, 1.f, 0.f };
        const glm::vec3 right{ 1.f, 0.f, 0.f };

        {
            m_Brickwalls[0].GetComponent<Transform>().Position = { 0.f, 0.f, -halfScale };

            m_Brickwalls[1].GetComponent<Transform>().Position = { 0.f, 0.f, halfScale };
            m_Brickwalls[1].GetComponent<Transform>().RotateTo(180.f, up);

            m_Brickwalls[2].GetComponent<Transform>().Position = { -halfScale, 0.f, 0.f };
            m_Brickwalls[2].GetComponent<Transform>().RotateTo(90.f, up);

            m_Brickwalls[3].GetComponent<Transform>().Position = { halfScale, 0.f, 0.f };
            m_Brickwalls[3].GetComponent<Transform>().RotateTo(-90.f, up);

            m_Brickwalls[4].GetComponent<Transform>().Position = { 0.f, -halfScale, 0.f };
            m_Brickwalls[4].GetComponent<Transform>().RotateTo(-90.f, right);

            m_Brickwalls[5].GetComponent<Transform>().Position = { 0.f, halfScale, 0.f };
            m_Brickwalls[5].GetComponent<Transform>().RotateTo(90.f, right);
        }
#endif
#if 0
        float scale = 10.f;
        float halfScale = scale * 0.5f + 10.f;
        for (int i = 0; i < 5; ++i)
        {
            m_Brickwalls[i] = CreateEntity("Brickwall" + std::to_string(i));
            m_Brickwalls[i].AddComponent<MeshInstance>(m_CubeMesh);

            m_Brickwalls[i].GetComponent<Transform>().ScaleF(scale);
        }

        const glm::vec3 up{ 0.f, 1.f, 0.f };
        const glm::vec3 right{ 1.f, 0.f, 0.f };

        {
            m_Brickwalls[0].GetComponent<Transform>().Position = { 0.f, halfScale, 0.f };
            m_Brickwalls[0].GetComponent<Transform>().RotateTo(90.f, right);

            m_Brickwalls[1].GetComponent<Transform>().Position = { 0.f, 0.f, -halfScale };
            m_Brickwalls[1].GetComponent<Transform>().RotateTo(180.f, up);

            m_Brickwalls[2].GetComponent<Transform>().Position = { -halfScale, 0.f, 0.f };
            m_Brickwalls[2].GetComponent<Transform>().RotateTo(90.f, up);

            m_Brickwalls[3].GetComponent<Transform>().Position = { halfScale, 0.f, 0.f };
            m_Brickwalls[3].GetComponent<Transform>().RotateTo(-90.f, up);

            m_Brickwalls[4].GetComponent<Transform>().Position = { 0.f, -halfScale, 0.f };
            m_Brickwalls[4].GetComponent<Transform>().RotateTo(-90.f, right);

            /*m_Brickwalls[5].GetComponent<Transform>().Position = { 0.f, halfScale, 0.f };
            m_Brickwalls[5].GetComponent<Transform>().RotateTo(90.f, right);*/
        }
#endif

#if 0
        {
            glm::vec3 pos = s_PointLightPos;
            int total = s_PointLightsGrid.y * s_PointLightsGrid.x;
            for (int i = 0; i < total; ++i)
            {
                
                /*pos.x += i % 2 ? 0.f : 5.f;
                pos.y += i % 2 ? 5.f : 0.f;*/
                glm::vec3 lowerleft = { -5.f * (s_PointLightsGrid.x-1) / 2.f , -5.f * (s_PointLightsGrid.y-1) / 2.f, 7.5f };
                glm::vec3 pos = lowerleft + glm::vec3(5.f * (i % s_PointLightsGrid.x), 5.f * (i / s_PointLightsGrid.x), 0.f );
                Entity pl = CreateEntity("PointLight");
                pl.AddComponent<MeshInstance>(m_CubeMesh, false);

                pl.GetComponent<MeshInstance>().Color = { 1.f, 1.f, 1.f, 1.f };

                pl.GetComponent<Transform>().Position = pos;
                pl.GetComponent<Transform>().ScaleF(0.2f);

                auto& l = pl.AddComponent<Light>(LightType::Point, true);
                l.Data.color = { 1.f, 0.f, 0.f };
                l.Data.brightness *= 5.f;
            }
        }
#endif
#if 0
        {
            m_DirLight = CreateEntity("DirLight");
            m_DirLight.AddComponent<MeshInstance>(m_CubeMesh, false);

            m_DirLight.GetComponent<MeshInstance>().Color = { 1.f, 1.f, 1.f, 1.f };

            m_DirLight.GetComponent<Transform>().Position = s_DirLightPos;
            m_DirLight.GetComponent<Transform>().ScaleF(0.2f);

            auto& l = m_DirLight.AddComponent<Light>(LightType::Directional, true);
            l.Data.brightness = 0.3f;
        }
#endif
#if 1
        float wallscale = 10.f;
        {
            
            float halfscale = wallscale * 0.5f + 10.f;
            glm::vec3 pos{};
            int total = s_SpotLightsTotal;
            int gridx = s_SpotLightsGrid.x;
            int gridy = s_SpotLightsGrid.y;
            float step = wallscale;
            glm::vec3 cubeoffset = { 0.f, 0.f, -step / 4 };
            glm::vec3 lowerleft = { -step * (gridx - 1) / 2.f, -7.5f, -step * (gridy - 1) / 2.f };

            for (int i = 0; i < total; ++i)
            {
                glm::vec3 pos = lowerleft + cubeoffset + glm::vec3(step * (i % gridy), 0.f, step * (i / gridx));
                auto cube = CreateEntity("Cube_" + std::to_string(i));
                cube.AddComponent<MeshInstance>(m_CubeMesh);
                cube.GetComponent<Transform>().Position =
                    pos;
            }
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
                //brickwall.GetComponent<Transform>().RotateTo(90.f, right);
            }

        }
#endif
#if 0
        {
            glm::vec3 pos{};
            int total = s_SpotLightsTotal;
            int gridx = s_SpotLightsGrid.x;
            int gridy = s_SpotLightsGrid.y;
            float step = wallscale;
            glm::vec3 lowerleft = { -step * (gridx - 1) / 2.f, -7.5f, -step * (gridy - 1) / 2.f };
            for (int i = 0; i < total; ++i)
            {

                /*pos.x += i % 2 ? 0.f : step;
                pos.y += i % 2 ? step : 0.f;*/
                glm::vec3 pos = lowerleft + glm::vec3(step * (i % gridy), 0.f, step * (i / gridx));
                Entity pl = CreateEntity("SpotLight_" + std::to_string(i));
                pl.AddComponent<MeshInstance>(m_CubeMesh, false);

                pl.GetComponent<MeshInstance>().Color = { 1.f, 1.f, 1.f, 1.f };

                pl.GetComponent<Transform>().Position = pos;
                pl.GetComponent<Transform>().ScaleF(0.2f);
                pl.GetComponent<Transform>().RotateTo(180.f, {0.f, 1.f, 0.f});

                auto& l = pl.AddComponent<Light>(LightType::Spot, true);
                l.Data.color = { 0.f, 0.f, 1.f };
                l.Data.brightness *= 3.5f;
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

                /*pos.x += i % 2 ? 0.f : step;
                pos.y += i % 2 ? step : 0.f;*/
                glm::vec3 pos = lowerleft + glm::vec3(step * (i % gridy), 0.f, step * (i / gridx));
                Entity pl = CreateEntity("PointLight_" + std::to_string(i));
                pl.AddComponent<MeshInstance>(m_CubeMesh, false);

                pl.GetComponent<MeshInstance>().Color = { 1.f, 1.f, 1.f, 1.f };

                pl.GetComponent<Transform>().Position = pos;
                pl.GetComponent<Transform>().ScaleF(0.2f);
                //pl.GetComponent<Transform>().RotateTo(180.f, { 0.f, 1.f, 0.f });

                auto& l = pl.AddComponent<Light>(LightType::Point, true);
                l.Data.color = { 0.f, 0.f, 1.f };
                l.Data.brightness *= 3.5f;
            }
        }
#endif
       // Entity model = ImportModel("deccer-cubes/SM_Deccer_Cubes_Textured.gltf");
        //model.GetComponent<Transform>().Position = { 5.f, 5.f, 0.f };
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

        //SetLightParams(s_LightBrightness);

       /* auto& view = m_Registry.view<Tag, Transform, Light>();
        for (auto& entity : view)
        {
            auto& [tag, tr, light] = view.get(entity);
            if (tag.String == "DirLight")
            {
                m_LightData[LightType::Directional].direction = -tr.Position;
                light.UploadToSSBO(m_LightData[LightType::Directional]);
                if (RotateLight)
                    tr.RotateAroundPoint(RotPoint, LightRotSpeed * deltaTime, RotAxis);
            }
        }*/

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
        //ImGui::Checkbox("Light On", &LightOn);
        ImGui::Checkbox("Cast Shadows", &CastShadows);
        //ImGui::SliderFloat("Light Brightness", &s_LightBrightness, min, 10.f);
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