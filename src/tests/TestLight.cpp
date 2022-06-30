#include "pch.h"
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


    static struct
    {
        glm::vec3 lightPos;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        float constant;
        float linear;
        float quadratic;

        float shininess;
    } LightParams;

    static glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

    TestLight::TestLight()
        : m_Camera(glm::vec3(0.0f, 0.0f, 3.0f)),
        m_Container({ vertices, sizeof(vertices), 36, 3, 3, 0, 2 },
            {}, Shader("material_vert.shader", "pointLight_frag.shader")),
        m_LightSource({ vertices, sizeof(vertices), 36, 3, 3, 0, 2 },
            {}, Shader("material_vert.shader", "plainWhite_frag.shader"), lightPos),
        m_DiffuseTexture("container2.png"), m_SpecularTexture("container2_specular.png")
    {
        {
            m_ObjPositions.emplace_back(glm::vec3(1.f, 1.0f, 1.f));
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
       
        inputManager.SetCamera(&m_Camera);
        context.HideCursor();
        
        {
            LightParams.lightPos = lightPos;
            LightParams.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
            LightParams.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
            LightParams.specular = glm::vec3(1.f, 1.f, 1.f);

            LightParams.constant = 1.0f;
            LightParams.linear = 0.09f;
            LightParams.quadratic = 0.032f;

            LightParams.shininess = 32.0f;
        }
        
        m_LightSource.Scale(glm::vec3(0.2f));

        Shader& contSh = m_Container.GetShader();
        contSh.Bind();

        contSh.setInt("material.diffuse", 0);
        contSh.setInt("material.specular", 1);
	}

	void TestLight::OnRender()
	{
        
        Shader& contSh = m_Container.GetShader();
        contSh.Bind();

        contSh.setFloat3("light.position", m_LightSource.Position());
        contSh.setFloat3("viewPos", m_Camera.Position());

        contSh.setFloat3("light.ambient", LightParams.ambient);
        contSh.setFloat3("light.diffuse", LightParams.diffuse);
        contSh.setFloat3("light.specular", LightParams.specular);
        contSh.setFloat("light.constant", LightParams.constant);
        contSh.setFloat("light.linear", LightParams.linear);
        contSh.setFloat("light.quadratic", LightParams.quadratic);

        contSh.setFloat("material.shininess", 32.0f);

        m_Container.WatchedBy(m_Camera);
        m_LightSource.WatchedBy(m_Camera);

        contSh.Bind();
        m_DiffuseTexture.Bind(0);
        m_SpecularTexture.Bind(1);

        Renderer renderer;
        
        for (int i = 0; i < m_ObjPositions.size(); ++i)
        {
            m_Container.MoveTo(m_ObjPositions[i]);
            //float angle = 20.0f * (i+1);
            float angle = 50.0f;
            m_Container.Rotate((float)glfwGetTime() * glm::radians(angle),
                glm::vec3(1.0f, 0.3f, 0.5f));
            m_LightSource.MoveTo(LightParams.lightPos);
            
            m_Container.DrawNoIndex();
            m_LightSource.DrawNoIndex();
        }
	}

	void TestLight::OnImGuiRender()
	{
        /*float w = context.Width();
        float h = context.Height();
        ImGui::SetNextWindowPos(ImVec2(w * 0.75f, h * 0.75f));
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);*/
        static float min = 0.0f;
        static float maxPos = 30.f;
        static float maxParams = 10.0f;
        static float maxAtten = 1.0f;
        static float val = 0.0f;
        ImGui::SliderFloat3("Light Position", &LightParams.lightPos[0], min, maxPos);
        
        ImGui::SliderFloat("Ambient",         &LightParams.ambient[0],  min, maxParams);
        LightParams.ambient = glm::vec3(LightParams.ambient.x);
        ImGui::SliderFloat("Diffuse",         &LightParams.diffuse[0],  min, maxParams);
        LightParams.diffuse = glm::vec3(LightParams.diffuse.x);
        ImGui::SliderFloat("Specular",        &LightParams.specular[0], min, maxParams);
        LightParams.specular = glm::vec3(LightParams.specular.x);
        
        ImGui::SliderFloat ("Constant",       &LightParams.constant,  min, maxAtten  );
        ImGui::SliderFloat ("Linear",         &LightParams.linear,    min, maxAtten  );
        ImGui::SliderFloat ("Quadratic",      &LightParams.quadratic, min, maxAtten  );

        ImGui::SliderFloat ("Shininess",      &LightParams.shininess, min, 256.0f);
	}
}
