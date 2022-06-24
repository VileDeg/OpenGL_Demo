#include "TestLight.h"

#include "GLFW/glfw3.h"
#include "imgui/imgui.h"

#include "math_headers.h"
#include "GLContext.h"
#include "input/InputManager.h"


namespace test
{
    static GLContext& context = GLContext::getTnstance();
    static InputManager& inputManager = InputManager::getInstance();

    //static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    static float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    //static glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    /*static Object container({ vertices, sizeof(vertices), 36, 3, 3, 2, 0 },
        {}, "light_vert.shader", "light_frag.shader");
    static Object lightCube({ vertices, sizeof(vertices), 36, 3, 3, 2, 0 },
        {}, "light_vert.shader", "lightSource_frag.shader", lightPos);

    static Texture containerTex("res/textures/container2.png");*/
    

    TestLight::TestLight()
        : m_Camera(glm::vec3(0.0f, 0.0f, 3.0f)),
        m_Container({ vertices, sizeof(vertices), 36, 3, 3, 2, 0 },
            {}, "light_vert.shader", "light_frag.shader"),
        m_LightSource({ vertices, sizeof(vertices), 36, 3, 3, 2, 0 },
            {}, "light_vert.shader", "lightSource_frag.shader", glm::vec3(1.2f, 1.0f, 2.0f)),
        m_DiffuseTexture("container2.png"), m_SpecularTexture("container2_specular.png")
    {
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
        
        
        m_LightSource.Scale(glm::vec3(0.2f));

        inputManager.SetCamera(&m_Camera);
        inputManager.HideCursor();
	}

	void TestLight::OnRender()
	{

        m_Container.WatchedBy(m_Camera);
        m_LightSource.WatchedBy(m_Camera);
        
        Renderer renderer;

        for (int i = 0; i < m_ObjPositions.size(); ++i)
        {
            m_Container.ResetTransform();
            m_Container.Move(m_ObjPositions[i]);
            float angle = 20.0f * (i+1);
            m_Container.Rotate((float)glfwGetTime() * glm::radians(angle),
                glm::vec3(1.0f, 0.3f, 0.5f));
            
            Shader& contSh = m_Container.GetShader();
            glm::vec3 lightColor(1.0f);
           /* lightColor.x = sin(glfwGetTime() * 2.0f * i * 0.2f);
            lightColor.y = sin(glfwGetTime() * 0.7f * i * 0.2f);
            lightColor.z = sin(glfwGetTime() * 1.3f * i * 0.2f);
            lightColor = glm::clamp(lightColor, 0.2f, 1.0f);*/

            glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
            glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

            contSh.Bind();

            m_DiffuseTexture.Bind(0);
            contSh.setInt("material.diffuse", 0);
            m_SpecularTexture.Bind(1);
            contSh.setInt("material.specular", 1);
            
            
            contSh.setFloat3("light.ambient", ambientColor);
            contSh.setFloat3("light.diffuse", diffuseColor); // darken diffuse light a bit
            contSh.setFloat3("light.specular", 1.0f, 1.0f, 1.0f);
            contSh.setFloat3("viewPos", m_Camera.Position());

            //contSh.setFloat3("material.ambient", 1.0f, 0.5f, 0.31f);
            //contSh.setFloat3("material.diffuse", 1.0f, 0.5f, 0.31f);
            //contSh.setFloat3("material.specular", 0.5f, 0.5f, 0.5f);
            contSh.setFloat("material.shininess", 32.0f);

            m_Container.DrawNoIndex();
            m_LightSource.DrawNoIndex();
        }
	}

	void TestLight::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
