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
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        Object m_Cube;
        Camera m_Camera;
        float m_CamSpeed;
    };
}
