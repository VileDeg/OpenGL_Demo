#include "pch.h"
#include "TestScene.h"
#include "renderer/MeshManager.h"

namespace Crave
{
    using namespace Component;

    static glm::vec3 s_PointLightPos = { 0.0f, 0.0f, 7.5f };
    static glm::vec3 s_DirLightPos = { -20.f, 20.f, 0.f };
    static glm::vec3 s_SpotLightPos = { 7.5f, 0.f, 0.f };
    void TestScene::SetLightParams(float brightness)
    {
        glm::vec3 ambient = glm::vec3(0.1f);
        glm::vec3 diffuse = glm::vec3(0.8f);
        glm::vec3 specular = glm::vec3(1.f);

        float constant = 1.f / brightness;
        float linear = 0.09f / brightness;
        float quadratic = 0.032f / brightness;

        float cutOff = 12.5f;

        {
            m_LightData[LightType::Point].type = LightType::Point;

            m_LightData[LightType::Point].position = s_PointLightPos;
            //m_LightData[LightType::Point].ambient = ambient;
            m_LightData[LightType::Point].ambient = glm::vec3(0.f);
            m_LightData[LightType::Point].diffuse = diffuse;
            m_LightData[LightType::Point].specular = specular;

            m_LightData[LightType::Point].constant = constant;
            m_LightData[LightType::Point].linear = linear;
            m_LightData[LightType::Point].quadratic = quadratic;
        }
        {
            m_LightData[LightType::Directional].type = LightType::Directional;

            m_LightData[LightType::Directional].position = s_DirLightPos;

            glm::mat4 lightView = glm::lookAt(m_LightData[LightType::Directional].position,
                glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));

            m_LightData[LightType::Directional].projViewMat = Renderer::GetDirLightProjMat() * lightView;
            
            m_LightData[LightType::Directional].ambient = ambient;
            m_LightData[LightType::Directional].diffuse = diffuse;
            m_LightData[LightType::Directional].specular = specular;

            //Direction is not used by shader! position - FragPos is used to get direction
            //m_LightData[LightType::Directional].direction = {1.f, -1.f, 0.f}; //down + right
        }
        {
            m_LightData[LightType::Spot].type = LightType::Spot;

            m_LightData[LightType::Spot].position = s_SpotLightPos;
            m_LightData[LightType::Spot].direction = { -1.f, 0.f, 0.f };

            glm::vec3 pos = m_LightData[LightType::Spot].position;
            glm::vec3 dir = m_LightData[LightType::Spot].direction;
            glm::vec3 front = pos + glm::normalize(dir);
            glm::mat4 lightView = glm::lookAt(pos, front, glm::vec3(0.f, 1.f, 0.f));

            m_LightData[LightType::Spot].projViewMat = Renderer::GetSpotLightProjMat() * lightView;

            m_LightData[LightType::Spot].ambient = ambient;
            //m_LightData[LightType::Spot].ambient = glm::vec3(0.f);
            m_LightData[LightType::Spot].diffuse = diffuse;
            m_LightData[LightType::Spot].specular = specular;

            m_LightData[LightType::Spot].constant = constant;
            m_LightData[LightType::Spot].linear = linear;
            m_LightData[LightType::Spot].quadratic = quadratic;

            m_LightData[LightType::Spot].cutOff = glm::cos(glm::radians(cutOff));
            m_LightData[LightType::Spot].outerCutOff = glm::cos(glm::radians(cutOff+5.f));
        }
    }

    static float s_LightBrightness = 5.f;
    static int num = 2;
    TestScene::TestScene(Ref<Camera> camera)
        : m_Camera(camera),

        m_CamSpeed(6.f)
    {
        SetLightParams(s_LightBrightness);

        m_CubeMesh = MeshManager::GetPrimitiveMesh({ Primitive::Cube,
            {
                { Mesh::TexType::Diffuse,  {"container2.png"         } },
                { Mesh::TexType::Specular, {"container2_specular.png"} }
            } });
        m_BrickwallMesh = MeshManager::GetPrimitiveMesh({ Primitive::Plane,
            {
                { Mesh::TexType::Diffuse, {"brickwall.jpg"       } },
                { Mesh::TexType::Normal,  {"brickwall_normal.jpg"} }
            } });

        m_WorldCenter = CreateEntity("WorldCenter");
        m_WorldCenter.AddComponent<MeshInstance>(m_CubeMesh, false);
        m_WorldCenter.GetComponent<MeshInstance>().Color = { 1.f, 1.f, 1.f, 1.f };
        m_WorldCenter.GetComponent<Transform>().ScaleF(0.2f);

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


#if 1
        float scale = 30.f;
        float halfScale = scale * 0.5f;
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
#if 1
        {
            m_PointLight = CreateEntity("PointLight");
            m_PointLight.AddComponent<MeshInstance>(m_CubeMesh, false);

            m_PointLight.GetComponent<MeshInstance>().Color = { 1.f, 1.f, 1.f, 1.f };

            m_PointLight.GetComponent<Transform>().Position = s_PointLightPos;
            m_PointLight.GetComponent<Transform>().ScaleF(0.2f);

            m_PointLight.AddComponent<Light>(m_LightData[LightType::Point], true);
        }
#endif
#if 1
        {
            m_DirLight = CreateEntity("DirLight");
            m_DirLight.AddComponent<MeshInstance>(m_CubeMesh, false);

            m_DirLight.GetComponent<MeshInstance>().Color = { 1.f, 1.f, 1.f, 1.f };

            m_DirLight.GetComponent<Transform>().Position = s_DirLightPos;
            m_DirLight.GetComponent<Transform>().ScaleF(0.2f);

            m_DirLight.AddComponent<Light>(m_LightData[LightType::Directional], true);
        }
#endif
#if 1
        {
            m_SpotLight = CreateEntity("SpotLight");
            m_SpotLight.AddComponent<MeshInstance>(m_CubeMesh, false);

            m_SpotLight.GetComponent<MeshInstance>().Color = { 1.f, 1.f, 1.f, 1.f };

            m_SpotLight.GetComponent<Transform>().Position = s_SpotLightPos;
            m_SpotLight.GetComponent<Transform>().ScaleF(0.2f);

            m_SpotLight.AddComponent<Light>(m_LightData[LightType::Spot], true);
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
        ImGui::SliderFloat("Light Brightness", &s_LightBrightness, min, 10.f);
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