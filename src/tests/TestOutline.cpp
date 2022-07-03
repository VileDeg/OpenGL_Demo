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


     
    static struct
    {
        bool enableOutline = true;
        bool enableRotation = false;
        int rotationSpeed = 1;
        glm::vec3 rotationAxis = glm::vec3(0.f, 1.f, 0.f);
        int outlineBorderWidth = 1;
    } s_configs;

    

#define OUTLINE(x)\
    do {\
        if (s_configs.enableOutline)\
        {\
            x\
        }\
    } while (0);

#define N_OUTLINE(x)\
    do {\
        if (!s_configs.enableOutline)\
        {\
            x\
        }\
    } while (0);

    static Cube outlineCube(Shader("material_vert.shader", "color_frag.shader"));
    
    
    
    TestOutline::TestOutline()
        :  m_Camera(glm::vec3(0.0f, 0.0f, 10.0f)),
        m_FloorBox(Shader("material_vert.shader", "color_frag.shader"),
            glm::vec3(0.f, -60.f, 0.f))
    {

        inputManager.SetCamera(&m_Camera);
        context.HideCursor();
        
        
        for (int i = 0; i < CUBE_COUNT; i++)
        {
            m_Cubes.emplace_back(new Cube("container2.png", "container2_specular.png",
                Shader("cube_vf.shader"), glm::vec3(3.f*i, 0.f, 3.f*i)));
        }
        

        {
            LightParams.lightPos = m_Camera.Position();
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
        
        m_FloorBox.GetShader().Bind();
        m_FloorBox.GetShader().setFloat4("u_Color", glm::vec4(0.92f, 0.86f, 0.68f, 0.8));
        m_FloorBox.Scale(100.f);
        m_CamSpeed = m_Camera.Speed();

        
        OUTLINE(
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            glEnable(GL_STENCIL_TEST);
            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            )
        outlineCube.GetShader().Bind();
        outlineCube.GetShader().setFloat4("u_Color", glm::vec4(1.f, 0.f, 0.f, 1.f));
        outlineCube.TranslateTo(glm::vec3(5.f));
        
	}



	void TestOutline::OnRender()
	{
        OUTLINE(glStencilMask(0x00);)
        m_FloorBox.WatchedBy(m_Camera);
        m_FloorBox.Draw();
        N_OUTLINE(
            outlineCube.WatchedBy(m_Camera);
            outlineCube.Draw();
            
        )

        for (auto& cube : m_Cubes)
        {
            Shader& cubeSh = cube->GetShader();
            cubeSh.Bind();

            cubeSh.setFloat3("light.position", m_Camera.Position());
            cubeSh.setFloat3("light.direction", m_Camera.Front());
            cubeSh.setFloat("light.cutOff", LightParams.cutOff);
            cubeSh.setFloat("light.outerCutOff", LightParams.outerCutOff);
            cubeSh.setFloat3("viewPos", m_Camera.Position());

            cubeSh.setFloat3("light.ambient", LightParams.ambient);
            cubeSh.setFloat3("light.diffuse", LightParams.diffuse);
            cubeSh.setFloat3("light.specular", LightParams.specular);
            cubeSh.setFloat("light.constant", LightParams.constant);
            cubeSh.setFloat("light.linear", LightParams.linear);
            cubeSh.setFloat("light.quadratic", LightParams.quadratic);

            cubeSh.setFloat("material.shininess", 32.0f);

            OUTLINE
            (
                
                glStencilFunc(GL_ALWAYS, 1, 0xFF);
                glStencilMask(0xFF);
            )
            cube->WatchedBy(m_Camera);
            /*if (s_configs.enableRotation)
            {
                cube->Rotate(1.f * s_configs.rotationSpeed * GLContext::getTnstance().DeltaTime(),
                    s_configs.rotationAxis);
            }*/
            cube->Draw();
            OUTLINE
            (
                glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
                glStencilMask(0x00);
                glDisable(GL_DEPTH_TEST);
                
                outlineCube.ScaleTo((1.f + 0.1f * s_configs.outlineBorderWidth));
                outlineCube.TranslateTo(cube->GetPosition());
                if (s_configs.enableRotation)
                {
                    outlineCube.Rotate(1.f*s_configs.rotationSpeed*GLContext::getTnstance().DeltaTime(),
                        s_configs.rotationAxis);
                }
                outlineCube.WatchedBy(m_Camera);
                outlineCube.Draw();
                /*outlineCube.ReportPosition();
                outlineCube.ReportScale();*/
                glStencilMask(0xFF);
                glStencilFunc(GL_ALWAYS, 1, 0xFF);
                glEnable(GL_DEPTH_TEST);
            )
            
        }
        

        m_Camera.SetSpeed(m_CamSpeed);
        
	}

	void TestOutline::OnImGuiRender()
	{
       
        static float min = 0.0f;

        ImGui::SliderFloat("Camera Speed", &m_CamSpeed, min, 10.0f);
        ImGui::Checkbox   ("Outline ON",   &s_configs.enableOutline         );
        if (s_configs.enableOutline)
        {
            ImGui::Checkbox ("Rotation ON", &s_configs.enableRotation);
            ImGui::SliderInt("Border Width", &s_configs.outlineBorderWidth, 0, 10);
            if (s_configs.enableRotation)
            {
                ImGui::SliderInt("Rotation Speed", &s_configs.rotationSpeed, 0, 10);
                ImGui::SliderFloat3("Rotation Axis", glm::value_ptr(s_configs.rotationAxis), 0.0f, 45.f);
            }
        }
	}
}
