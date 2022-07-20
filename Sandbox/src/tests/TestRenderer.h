#pragma once
#include "Test.h"

#include "OpenGL_Demo.h"

namespace test
{
    class TestRenderer : public Test
    {
    public:
        TestRenderer(Window& window);
        ~TestRenderer() {}

        void OnUpdate(float deltaTime, Camera& camera) override;
        //void OnRender() override {}
        void OnImGuiRender() override;
    private:
        void SetLightParams();
    private:
        Ref<Scene> m_Scene;
        Mesh m_CubeMesh;
        Mesh* m_BrickwallMesh;
        Entity m_Cubes[1000];
        Entity m_LightCubes[3];
        Entity m_Room;
        Entity m_Brickwalls[6];
        Entity m_WorldCenter;
        std::vector<Entity> m_ImportedModels;
        std::vector<Model> m_Models;
        
        //Camera m_Camera;
        float m_CamSpeed;
        glm::vec3 m_LightPositions[3];

        Light m_DirLightParams;
        Light m_SpotLightParams;
        Light m_PointLightParams;
    };
}
