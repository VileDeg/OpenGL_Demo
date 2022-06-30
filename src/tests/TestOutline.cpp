#include "pch.h"
#include "TestOutline.h"

#include "GLFW/glfw3.h"
#include "imgui/imgui.h"

#include "math_headers.h"
#include "GLContext.h"
#include "input/InputManager.h"


namespace test
{
    static GLContext& context = GLContext::getTnstance();
    static InputManager& inputManager = InputManager::getInstance();


    


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

        float cutOff;
        float outerCutOff;
    } LightParams;

    static glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

    static Cube floorBox(Shader("material_vert.shader", "plainWhite_frag.shader"),
        glm::vec3(0.0f, -10.0f, 0.0f));

    static Shader cubeShader("model_vert.shader", "model_frag.shader");

    static constexpr Object::TextureData CUBE_TDATA
    {
        "container2.png",
        "container2_specular.png"
    };

    //m_Cubes{
    /*Cube(CUBE_TDATA, "cube_vf.shader", glm::vec3(3.f, 0.f, 3.f)),
        Cube(CUBE_TDATA, "cube_vf.shader", glm::vec3(-3.f, 0.f, 3.f)),
        Cube(CUBE_TDATA, "cube_vf.shader", glm::vec3(3.f, 0.f, -3.f)),
        Cube(CUBE_TDATA, "cube_vf.shader", glm::vec3(-3.f, 0.f, -3.f))
    },*/
    TestOutline::TestOutline()
        :  m_Camera(glm::vec3(0.0f, 0.0f, 10.0f))
        
    {

        inputManager.SetCamera(&m_Camera);
        context.HideCursor();
        
        {
            for (int i = 0; i < CUBE_COUNT; i++)
            {
                m_Cubes.emplace_back(new Cube(CUBE_TDATA, "cube_vf.shader", glm::vec3(3.f, 0.f, 3.f)));
            }
        }

        {
            LightParams.lightPos = lightPos;
            LightParams.ambient = glm::vec3(1.f);
            LightParams.diffuse = glm::vec3(1.0f);
            LightParams.specular = glm::vec3(1.f);
            LightParams.shininess = 32.0f;
            
            LightParams.constant = 1.0f;
            LightParams.linear = 0.09f;
            LightParams.quadratic = 0.032f;

            LightParams.cutOff = glm::cos(glm::radians(12.5f));
            LightParams.outerCutOff = glm::cos(glm::radians(17.5f));
        }
        
        floorBox.Scale({100.f, 0.0f, 100.f });
        //m_LightSource.Scale(glm::vec3(0.2f));
        m_CamSpeed = m_Camera.Speed();
        
        /*Shader& contSh = m_Container.GetShader();
        contSh.Bind();

        contSh.setInt("material.diffuse", 0);
        contSh.setInt("material.specular", 1);*/
	}

	void TestOutline::OnRender()
	{
        
        Shader& cubeSh = m_Cubes[0]->GetShader();
        cubeSh.Bind();

        cubeSh.setFloat3("light.position",  m_Camera.Position());
        /*cubeSh.setFloat3("light.direction", m_Camera.Front());
        cubeSh.setFloat ("light.cutOff",    LightParams.cutOff);
        cubeSh.setFloat ("light.outerCutOff", LightParams.outerCutOff);*/
        cubeSh.setFloat3("viewPos",         m_Camera.Position());

        cubeSh.setFloat3("light.ambient",   LightParams.ambient);
        cubeSh.setFloat3("light.diffuse",   LightParams.diffuse);
        cubeSh.setFloat3("light.specular",  LightParams.specular);
        cubeSh.setFloat ("light.constant",  LightParams.constant);
        cubeSh.setFloat ("light.linear",    LightParams.linear);
        cubeSh.setFloat ("light.quadratic", LightParams.quadratic);

        cubeSh.setFloat("material.shininess", 32.0f);

        for (auto& cube : m_Cubes)
        {
            cube->WatchedBy(m_Camera);
            cube->DrawNoIndex();
        }

        floorBox.WatchedBy(m_Camera);
        
        floorBox.DrawNoIndex();

        m_Camera.SetSpeed(m_CamSpeed);
        
	}

	void TestOutline::OnImGuiRender()
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
        //ImGui::SliderFloat3("Light Position", &LightParams.lightPos[0], min, maxPos);
        ImGui::SliderFloat("Camera Speed", &m_CamSpeed, min, 10.0f);
        ImGui::SliderFloat("Ambient",         &LightParams.ambient[0],  min, maxParams);
        LightParams.ambient = glm::vec3(LightParams.ambient.x);
        ImGui::SliderFloat("Diffuse",         &LightParams.diffuse[0],  min, maxParams);
        LightParams.diffuse = glm::vec3(LightParams.diffuse.x);
        ImGui::SliderFloat("Specular",        &LightParams.specular[0], min, maxParams);
        LightParams.specular = glm::vec3(LightParams.specular.x);
        ImGui::SliderFloat("Shininess", &LightParams.shininess, min, 256.0f);
        
        ImGui::SliderFloat ("Constant",       &LightParams.constant,  min, maxAtten  );
        ImGui::SliderFloat ("Linear",         &LightParams.linear,    min, maxAtten  );
        ImGui::SliderFloat ("Quadratic",      &LightParams.quadratic, min, maxAtten  );

        /*static float cutOff = LightParams.cutOff;
        ImGui::SliderFloat("CutOff", &cutOff, min, 180.0f);
        LightParams.cutOff = cutOff;
        static float outerCutOff = LightParams.outerCutOff;
        ImGui::SliderFloat("OuterCutOff", &outerCutOff, min, 180.0f);
        LightParams.outerCutOff = outerCutOff;*/
	}
}
