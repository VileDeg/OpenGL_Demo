#pragma once

#include "Test.h"

#include "Renderer.h"
#include "Texture.h"

#include "Object.h"

namespace test
{

    class TestSpotlight : public Test
    {
    public:
        TestSpotlight();
        ~TestSpotlight() {}

        void OnUpdate(float deltaTime) override {}
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        
        Camera m_Camera;
        Texture m_DiffuseTexture, m_SpecularTexture;
        std::vector<glm::vec3> m_ObjPositions;
        Object m_Container, m_LightSource;
    };
}
