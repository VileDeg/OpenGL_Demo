#include "pch.h"
#include "TestObject3D.h"

#include "GLFW/glfw3.h"
#include "imgui/imgui.h"

#include "math_headers.h"


namespace test
{
	TestObject3D::TestObject3D()
	{
        float vertices[] = {
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };


        {
            m_ObjPositions.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f));
            m_ObjPositions.emplace_back(glm::vec3(2.0f, 5.0f, -15.0f));
            m_ObjPositions.emplace_back(glm::vec3(-1.5f, -2.2f, -2.5f));
            m_ObjPositions.emplace_back(glm::vec3(-3.8f, -2.0f, -12.3f));
            m_ObjPositions.emplace_back(glm::vec3(2.4f, -0.4f, -3.5f));
            m_ObjPositions.emplace_back(glm::vec3(-1.7f, 3.0f, -7.5f));
            m_ObjPositions.emplace_back(glm::vec3(1.3f, -2.0f, -2.5f));
            m_ObjPositions.emplace_back(glm::vec3(1.5f, 2.0f, -2.5f));
            m_ObjPositions.emplace_back(glm::vec3(1.5f, 0.2f, -1.5f));
            m_ObjPositions.emplace_back(glm::vec3(-1.3f, 1.0f, -1.5f));
        }



		m_Shader = std::make_unique<Shader>("test_object3D_vert.shader", "test_object3D_frag.shader");
		m_VAO = std::make_unique<VAO>();
		m_VBO = std::make_unique<VBO>(vertices, sizeof(vertices));
		VertexLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		m_VAO->AddBuffer(*m_VBO, layout);
		
		//m_IndexBuffer = std::make_unique<EBO>(indices, sizeof(indices) / sizeof(unsigned));

		m_Texture = std::make_unique<Texture>("awesomeface.png");

		m_Shader->Bind();
		m_Shader->setInt("texture1", 0);
        m_Texture->Bind();
	}

	TestObject3D::~TestObject3D()
	{

	}

	void TestObject3D::OnUpdate(float deltaTime)
	{

	}

	void TestObject3D::OnRender()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Renderer renderer;

        {
            m_Shader->Bind();

            glm::mat4 identity = glm::mat4(1.0f);
//            glm::mat4 scale = glm::scale(identity, glm::vec3(0.5f));


            m_View = glm::translate(identity, glm::vec3(0.0f, 0.0f, -3.0f));
            m_Proj = glm::perspective(glm::radians(45.0f), (float)800/600, 0.1f, 100.0f);

            m_Shader->setMat4f("view", m_View);
            m_Shader->setMat4f("proj", m_Proj);
            //renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
            glBindVertexArray(m_VAO->Id());
            for (int i = 0; i < m_ObjPositions.size(); ++i)
            {
                m_Model = glm::translate(identity, m_ObjPositions[i]);
                float angle = 20.0f * (i+1);
                m_Model = glm::rotate(m_Model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                m_Shader->setMat4f("model", m_Model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

        }
	}

	void TestObject3D::OnImGuiRender()
	{
		/*ImGui::SliderFloat3("TranslationA", &m_TranslationA.x, 0.0f, 800.0f);
		ImGui::SliderFloat3("TranslationB", &m_TranslationB.x, 0.0f, 800.0f);*/
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
