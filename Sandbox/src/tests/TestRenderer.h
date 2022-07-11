#pragma once
#include "Test.h"

#include "geometry/Object.h"


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
        Object m_Cube;
        Object m_LightCube;
        Camera m_Camera;
        float m_CamSpeed;
        glm::vec3 m_LightPositions[3];
        DirectionalLight m_DirLightParams;
        Spotlight m_SpotLightParams;
        PointLight m_PointLightParams;
    };
}
