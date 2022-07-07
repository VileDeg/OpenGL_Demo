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
        /*{
            for (auto x : m_Cubes)
                delete x;
        }*/

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        Object m_Cube;
        Camera m_Camera;
        float m_CamSpeed;
        /*static constexpr const int CUBE_COUNT = 4;

        std::vector<Object*> m_Cubes;
        Object m_OutlineCube;
        Object m_FloorBox;*/
    };
}
