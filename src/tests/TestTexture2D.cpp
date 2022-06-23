#include "TestTexture2D.h"
#include "imgui/imgui.h"

#include "math_headers.h"


namespace test
{
	TestTexture2D::TestTexture2D()
		: m_TranslationA{ 200, 200, 0 }, m_TranslationB{ 400, 200, 0 },
		m_Proj(glm::ortho(0.f, 800.f, 0.f, 600.f, -1.f, 1.f)), m_View(glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0)))
	{

		float vertices[] = {
			// positions          // colors           // texture coords
			 -50.f, -50.f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // top right
			 50.f, -50.f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
			 50.f, 50.f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // bottom left
			 -50.f, 50.f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
		};
		unsigned int indices[] = {
			0, 1, 2, // first triangle
			2, 3, 0  // second triangle
		};

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		m_Shader = std::make_unique<Shader>("vertex.shader", "fragment.shader");
		m_VAO = std::make_unique<VAO>();
		m_VBO = std::make_unique<VBO>(vertices, sizeof(vertices));
		VertexLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		m_VAO->AddBuffer(*m_VBO, layout);
		
		m_IndexBuffer = std::make_unique<EBO>(indices, sizeof(indices) / sizeof(unsigned));

		m_Texture = std::make_unique<Texture>("cherno_logo.jpg");

		m_Shader->Bind();
		m_Shader->setInt("texture1", 0);
	}

	TestTexture2D::~TestTexture2D()
	{

	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{

	}

	void TestTexture2D::OnRender()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Renderer renderer;

		m_Texture->Bind();

		{
			glm::mat4 model = glm::translate(glm::mat4(1.f), m_TranslationA);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->setMat4f("u_mvp", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.f), m_TranslationB);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->setMat4f("u_mvp", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("TranslationA", &m_TranslationA.x, 0.0f, 800.0f);
		ImGui::SliderFloat3("TranslationB", &m_TranslationB.x, 0.0f, 800.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}