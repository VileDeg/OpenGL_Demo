#pragma once
#include "Test.h"

#include "geometry/Object.h"

namespace test
{

    class TestOutline : public Test
    {
    public:
        TestOutline(Window& window);
        ~TestOutline()
        {
            for (auto x : m_Cubes)
                delete x;
        }

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        static constexpr const int CUBE_COUNT = 4;

        std::vector<Object*> m_Cubes;
        Object m_OutlineCube;
        Object m_FloorBox;
        Camera m_Camera;
        float m_CamSpeed;
    };
}
