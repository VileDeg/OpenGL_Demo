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

        void OnUpdate(float deltaTime) override;
        void OnRender() override {}
        void OnImGuiRender() override;
    private:
        void SetLightParams();
    private:
        Ref<Scene> m_Scene;
        Mesh m_CubeMesh;
        Entity m_Cubes[1000];
        Entity m_LightCubes[3];
        Entity m_Room;
        //Object m_Cube;
        //Object m_LightCube;
        Camera m_Camera;
        float m_CamSpeed;
        glm::vec3 m_LightPositions[3];

        Light m_DirLightParams;
        Light m_SpotLightParams;
        Light m_PointLightParams;
    };
}
