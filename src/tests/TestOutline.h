#pragma once
#include "Test.h"

#include "geometry/Cube.h"

namespace test
{

    class TestOutline : public Test
    {
    public:
        TestOutline();
        ~TestOutline()
        {
            for (auto x : m_Cubes)
                delete x;
        }

        void OnUpdate(float deltaTime) override {}
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        static constexpr const int CUBE_COUNT = 4;

        std::vector<Cube*> m_Cubes;
        Camera m_Camera;
        float m_CamSpeed;
    };
}
