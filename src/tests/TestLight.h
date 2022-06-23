#pragma once
#include "Test.h"

#include "Renderer.h"
#include "Texture.h"

#include "Object.h"

#include <memory>
#include <vector>

namespace test
{

    class TestLight : public Test
    {
    public:
        TestLight();
        ~TestLight() {}

        void OnUpdate(float deltaTime) override {}
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        std::unique_ptr<VAO> m_VAO;
        std::unique_ptr<VBO> m_VBO;
        std::unique_ptr<EBO> m_IndexBuffer;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Texture> m_Texture;

        glm::mat4 m_Proj, m_View, m_Model;

        std::vector<glm::vec3> m_ObjPositions;
        Object m_Object;
    };
}
