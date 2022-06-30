#pragma once
#include "Test.h"

#include "Object.h"
#include "assimp_import/Model.h"

namespace test
{

    class TestModelImport : public Test
    {
    public:
        TestModelImport();
        ~TestModelImport() {}

        void OnUpdate(float deltaTime) override {}
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        Model m_Model;
        Camera m_Camera;
        float m_CamSpeed;
        //Object m_LightSource;
    };
}
