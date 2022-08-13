#include "pch.h"
#include "TestScene.h"
#include "renderer/MeshManager.h"

static glm::vec3 s_LightPos = { 0.0f, 0.0f, 7.5f };
void TestScene::SetLightParams()
{
    glm::vec3 ambient = glm::vec3(0.4f);
    glm::vec3 diffuse = glm::vec3(0.8f);
    glm::vec3 specular = glm::vec3(1.f);

    float constant = 0.3f * 0.5f;
    float linear = 0.09f * 0.25f;
    float quadratic = 0.032f * 0.5f;

    float cutOff = 20.f;

    {
        m_PointLightParams.type = 1; //Point
        m_PointLightParams.position = s_LightPos;
        m_PointLightParams.ambient = ambient;
        m_PointLightParams.diffuse = diffuse;
        m_PointLightParams.specular = specular;

        m_PointLightParams.constant = constant;
        m_PointLightParams.linear = linear;
        m_PointLightParams.quadratic = quadratic;
    }
}

static int num = 2;
TestScene::TestScene(Ref<Camera> camera)
    : m_Camera(camera),
    m_CamSpeed(6.f)
{
    //Window::HideCursor();

    SetLightParams();
   
    m_CubeMesh = MeshManager::GetMesh({ Primitive::Cube,
        {
            { TexType::Diffuse,  {"container2.png"         } },
            { TexType::Specular, {"container2_specular.png"} }
        }});
    m_BrickwallMesh = MeshManager::GetMesh({ Primitive::Plane,
        {
            { TexType::Diffuse, {"brickwall.jpg"       } },
            { TexType::Normal,  {"brickwall_normal.jpg"} }
        }});
    
    m_WorldCenter = CreateEntity("WorldCenter");
    m_WorldCenter.AddComponent<ModelComponent>(m_CubeMesh, false);
    m_WorldCenter.GetComponent<ModelComponent>().mis[0].Color = { 1.f, 1.f, 1.f, 1.f };
    m_WorldCenter.GetComponent<TransformComponent>().ScaleF(0.2f);

    float h = num - 1;
    for (int i = 0; i < num; ++i)
    {
        for (int j = 0; j < num; ++j)
        {
            for (int k = 0; k < num; ++k)
            {
                int ind = i * num * num + j * num + k;
                m_Cubes[ind] = CreateEntity("Cube" + std::to_string(ind));
                m_Cubes[ind].AddComponent<ModelComponent>(m_CubeMesh);
                m_Cubes[ind].GetComponent<TransformComponent>().
                    Position = glm::vec3(i * 2.f - h, j * 2.f - h, k * 2.f - h);
            }
        }
    }

    
#if 0
    float scale = 15.f;
    for (int i = 0; i < 6; ++i)
    {
        m_Brickwalls[i] = CreateEntity("Brickwall" + std::to_string(i));
        m_Brickwalls[i].AddComponent<ModelComponent>(m_BrickwallMesh);

        m_Brickwalls[i].GetComponent<TransformComponent>().ScaleF(scale);
    }

    const glm::vec3 up{ 0.f, 1.f, 0.f };
    const glm::vec3 right{ 1.f, 0.f, 0.f };

    {
        m_Brickwalls[0].GetComponent<TransformComponent>().Position = { 0.f, 0.f, -scale };

        m_Brickwalls[1].GetComponent<TransformComponent>().Position = { 0.f, 0.f, scale };
        m_Brickwalls[1].GetComponent<TransformComponent>().RotateTo(180.f, up);

        m_Brickwalls[2].GetComponent<TransformComponent>().Position = { -scale, 0.f, 0.f };
        m_Brickwalls[2].GetComponent<TransformComponent>().RotateTo(90.f, up);

        m_Brickwalls[3].GetComponent<TransformComponent>().Position = { scale, 0.f, 0.f };
        m_Brickwalls[3].GetComponent<TransformComponent>().RotateTo(-90.f, up);

        m_Brickwalls[4].GetComponent<TransformComponent>().Position = { 0.f, -scale, 0.f };
        m_Brickwalls[4].GetComponent<TransformComponent>().RotateTo(-90.f, right);

        m_Brickwalls[5].GetComponent<TransformComponent>().Position = { 0.f, scale, 0.f };
        m_Brickwalls[5].GetComponent<TransformComponent>().RotateTo(90.f, right);
    }
#endif
    m_LightCube = CreateEntity("LightCube");
    m_LightCube.AddComponent<ModelComponent>(m_CubeMesh, false);

    m_LightCube.GetComponent<ModelComponent>().mis[0].Color = { 1.f, 1.f, 1.f, 1.f };

    m_LightCube.GetComponent<TransformComponent>().Position = s_LightPos;
    m_LightCube.GetComponent<TransformComponent>().ScaleF(0.2f);

    m_LightCube.AddComponent<LightComponent>(m_PointLightParams, true);
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
    Renderer::BeginScene(m_Camera, LightOn ? 1 : 0, CastShadows);

    auto& view = m_Registry.view<TagComponent, TransformComponent, LightComponent>();
    for (auto& entity : view)
    {
        auto& [tag, tr, light] = view.get(entity);
        if (tag.Tag == "LightCube" && RotateLight)
        {
            tr.RotateAroundPoint(RotPoint, LightRotSpeed * deltaTime, RotAxis);
        }
    }

    /*if (RotateLight)
    {
        auto& tr = m_LightCube.GetComponent<TransformComponent>();
        tr.RotateAroundPoint(RotPoint, 150.f * deltaTime, RotAxis);
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