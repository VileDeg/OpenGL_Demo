#pragma once
#include "Cavern.h"

namespace Crave
{
    class TestScene : virtual public Scene
    {
    public:
        TestScene(Ref<Camera> camera);
        ~TestScene() override;

        void OnUpdate(float deltaTime) override;
        void OnImGuiRender(ImGuiWindowFlags panelFlags) override;
    private:
        //void SetLightParams(float brightness);
    private:
        Ref<Camera> m_Camera;
        Ref<Mesh> m_CubeMesh;
        Ref<Mesh> m_BrickwallMesh;
        Entity m_Model;
        //Import::Model m_ImportedModel;
        Entity m_Cubes[8];
        Entity m_PointLight;
        Entity m_DirLight;
        Entity m_SpotLight;
        Entity m_Brickwalls[6];
        Entity m_WorldCenter;
        float m_CamSpeed;
        //LightData m_PointLightParams;
        //std::unordered_map<LightType, LightData> m_LightData;
    };
}