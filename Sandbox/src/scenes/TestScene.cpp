#include "TestScene.h"

void TestScene::SetLightParams()
{
    glm::vec3 ambient = glm::vec3(0.4f);
    glm::vec3 diffuse = glm::vec3(0.8f);
    glm::vec3 specular = glm::vec3(1.f);

    float constant = 0.3f * 0.5f;
    float linear = 0.09f * 0.25f;
    float quadratic = 0.032f * 0.5f;

    float cutOff = 20.f;
    //float outerCutOff = 12.5f;

    m_LightPositions[0] = { 0.0f, 0.0f, 7.5f };
    //m_LightPositions[1] = { 10.0f, -3.0f, 0.0 };
    //
    //
    //{
    //    m_DirLightParams.type = 0; //Dir
    //    m_DirLightParams.direction = glm::vec3(1.f, -1.f, 0.f);
    //    m_DirLightParams.ambient   = ambient;
    //    m_DirLightParams.diffuse   = diffuse;
    //    m_DirLightParams.specular  = specular;
    //}

    //{
    //    m_SpotLightParams.type = 2; //Spot
    //    m_SpotLightParams.position    = m_LightPositions[1];
    //    m_SpotLightParams.direction   = glm::vec3(-1.0f, 0.0f, 0.0f);;
    //    m_SpotLightParams.ambient     = ambient;
    //    m_SpotLightParams.diffuse     = diffuse;
    //    m_SpotLightParams.specular    = specular;
    //                                  
    //    m_SpotLightParams.constant    = constant;
    //    m_SpotLightParams.linear      = linear;
    //    m_SpotLightParams.quadratic   = quadratic;

    //    m_SpotLightParams.cutOff      = glm::cos(glm::radians(cutOff));
    //    m_SpotLightParams.outerCutOff = glm::cos(glm::radians(cutOff+5));
    //}

    {
        m_PointLightParams.type = 1; //Point
        m_PointLightParams.position = m_LightPositions[0];
        m_PointLightParams.ambient = ambient;
        m_PointLightParams.diffuse = diffuse;
        m_PointLightParams.specular = specular;

        m_PointLightParams.constant = constant;
        m_PointLightParams.linear = linear;
        m_PointLightParams.quadratic = quadratic;
    }
}

static float QuadVertices[84];

static void CalcPlaneVertices()
{
    // positions
    glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
    glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
    glm::vec3 pos3(1.0f, -1.0f, 0.0f);
    glm::vec3 pos4(1.0f, 1.0f, 0.0f);
    // texture coordinates
    glm::vec2 uv1(0.0f, 1.0f);
    glm::vec2 uv2(0.0f, 0.0f);
    glm::vec2 uv3(1.0f, 0.0f);
    glm::vec2 uv4(1.0f, 1.0f);
    // normal vector
    glm::vec3 nm(0.0f, 0.0f, 1.0f);

    // calculate tangent/bitangent vectors of both triangles
    glm::vec3 tangent1, bitangent1;
    glm::vec3 tangent2, bitangent2;
    // triangle 1
    // ----------
    glm::vec3 edge1 = pos2 - pos1;
    glm::vec3 edge2 = pos3 - pos1;
    glm::vec2 deltaUV1 = uv2 - uv1;
    glm::vec2 deltaUV2 = uv3 - uv1;

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

    bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

    // triangle 2
    // ----------
    edge1 = pos3 - pos1;
    edge2 = pos4 - pos1;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


    bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


    float quadVertices[84] = {
        // positions            // normal         // texcoords  // tangent                          // bitangent
        pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
        pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
        pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

        pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
        pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
        pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
    };
    for (int i = 0; i < 84; ++i)
    {
        QuadVertices[i] = quadVertices[i];
    }
}

static int num = 2;
TestScene::TestScene(Window& window, Camera& camera)
    : m_Window(window), m_Camera(camera),
    m_CubeMesh(GeoData::GetData(Primitive::Cube).data,
        GeoData::GetData(Primitive::Cube).size,
        GeoData::GetData(Primitive::Cube).count,
        {
            {TexType::Diffuse,  {"container2.png"         }},
            {TexType::Specular, {"container2_specular.png"}}
        }),

    m_CamSpeed(12.f)
{
    m_Window.HideCursor();

    SetLightParams();
    CalcPlaneVertices();
   
    m_BrickwallMesh = new Mesh((const void*)QuadVertices, sizeof(QuadVertices), 84,
        {
            { TexType::Diffuse, { "brickwall.jpg" }},
            { TexType::Normal, {"brickwall_normal.jpg"} }
        });
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

    float scale = 15.f;
    for (int i = 0; i < 6; ++i)
    {
        m_Brickwalls[i] = CreateEntity("Brickwall" + std::to_string(i));
        m_Brickwalls[i].AddComponent<ModelComponent>(*m_BrickwallMesh);

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

    m_LightCubes[0] = CreateEntity("LightCube");
    m_LightCubes[0].AddComponent<ModelComponent>(m_CubeMesh, false);

    m_LightCubes[0].GetComponent<ModelComponent>().mis[0].Color = { 1.f, 1.f, 1.f, 1.f };

    m_LightCubes[0].GetComponent<TransformComponent>().Position = m_LightPositions[0];
    m_LightCubes[0].GetComponent<TransformComponent>().ScaleF(0.2f);

    m_LightCubes[0].AddComponent<LightComponent>(m_PointLightParams, true);
}

static float DeltaTime = 0.f;
static bool  RotateLight = false;
static bool  LightOn = true;
static bool  CastShadows = true;
static float LightRotSpeed = 100.f;

static float PlaneAngle = 0.f;
static glm::vec3 PlaneAxis = { 0.f, 1.f, 0.f };
glm::vec3 RotPoint = { 0.f, 0.f, 0.f };
glm::vec3 RotAxis = { 0.f, 1.0f, 0.f };

void TestScene::OnUpdate(float deltaTime)
{
    DeltaTime = deltaTime;
    Renderer::BeginScene(m_Camera, LightOn ? 1 : 0, CastShadows);

    if (RotateLight)
    {
        auto& tr = m_LightCubes[0].GetComponent<TransformComponent>();
        tr.RotateAroundPoint(RotPoint, 150.f * deltaTime, RotAxis);
    }

    /*for (int i = 0; i < num*num*num; ++i)
    {
        auto& tr = m_Cubes[i].GetComponent<TransformComponent>();
        auto axis = glm::normalize(glm::vec3(1.f, 1.f, 0.f));
        glm::quat quatRot = glm::angleAxis(glm::radians(LightRotSpeed * deltaTime), axis);
        glm::mat4x4 matRot = glm::mat4_cast(quatRot);
        tr.Transform = matRot * tr.Transform;
    }*/

    //m_Brickwall.GetComponent<TransformComponent>().RotateTo(PlaneAngle, PlaneAxis);

    Scene::OnUpdate(deltaTime);

    Renderer::EndScene();
    m_Camera.SetSpeed(m_CamSpeed);
}

void TestScene::OnImGuiRender()
{
    ImGui::Begin("LevelMenu");
    static float min = 0.0f;
    static int x, y, z;
    ImGui::SliderFloat("Camera Speed", &m_CamSpeed, min, 10.0f);
    ImGui::Checkbox("Light On", &LightOn);
    ImGui::Checkbox("Cast Shadows", &CastShadows);
    ImGui::Checkbox("Rotate Light", &RotateLight);
    ImGui::SliderFloat("Light Rot Speed", &LightRotSpeed, min, 300.f);
    ImGui::SliderFloat3("Light Rot Point", glm::value_ptr(RotPoint), -15.f, 15.f);

    ImGui::SliderFloat("Plane Angle", &PlaneAngle, min, 90.f);
    ImGui::SliderFloat3("Plane Axis", glm::value_ptr(PlaneAxis), min, 1.0f);


    ImGui::LabelText("Frame Rate", "%f", 1 / DeltaTime);

    ImGui::End();
}

TestScene::~TestScene()
{

}