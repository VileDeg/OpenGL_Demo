#include "pch.h"
#include "TestModelImport.h"

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

        float cutOff;
        float outerCutOff;
    } LightParams;

    static glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

    static Object floorBox({vertices, sizeof(vertices), 36, 3, 3, 0, 2},
        {}, Shader("material_vert.shader", "plainWhite_frag.shader"), glm::vec3(0.0f, -10.0f, 0.0f));

    static Shader modelSh("model_vert.shader", "model_frag.shader");

    //static std::string base = "res/models/deccer-cubes-main/";

    TestModelImport::TestModelImport()
        :
        //m_Model("deccer-cubes-main/SM_Deccer_Cubes.gltf"),
        
        m_Model("backpack/backpack.obj"),
        //m_Model("backpack/backpack.obj"),
        m_Camera(glm::vec3(0.0f, 0.0f, 10.0f))
        //m_LightSource({ vertices, sizeof(vertices), 36, 3, 3, 0, 2 },
            //{}, "material_vert.shader", "plainWhite_frag.shader", lightPos)
    {
        
       
        inputManager.SetCamera(&m_Camera);
        context.HideCursor();
        
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

	void TestModelImport::OnRender()
	{
        
        //Shader& contSh = m_Container.GetShader();
        modelSh.Bind();

        modelSh.setFloat3("light.position",  m_Camera.Position());
        /*modelSh.setFloat3("light.direction", m_Camera.Front());
        modelSh.setFloat ("light.cutOff",    LightParams.cutOff);
        modelSh.setFloat ("light.outerCutOff", LightParams.outerCutOff);*/
        modelSh.setFloat3("viewPos",         m_Camera.Position());

        modelSh.setFloat3("light.ambient",   LightParams.ambient);
        modelSh.setFloat3("light.diffuse",   LightParams.diffuse);
        modelSh.setFloat3("light.specular",  LightParams.specular);
        modelSh.setFloat ("light.constant",  LightParams.constant);
        modelSh.setFloat ("light.linear",    LightParams.linear);
        modelSh.setFloat ("light.quadratic", LightParams.quadratic);

        modelSh.setFloat("material.shininess", 32.0f);

        
        modelSh.setMat4f("u_ProjMat", m_Camera.GetProjMat());
        modelSh.setMat4f("u_ViewMat", m_Camera.GetViewMat());
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f)); // translate it down so it's at the center of the scene
        //model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));	// it's a bit too big for our scene, so scale it down
        modelSh.setMat4f("u_ModelMat", model);

        m_Model.Draw(modelSh);

        floorBox.WatchedBy(m_Camera);
        
        floorBox.DrawNoIndex();

        m_Camera.SetSpeed(m_CamSpeed);
        //m_LightSource.WatchedBy(m_Camera);

        /*contSh.Bind();
        m_DiffuseTexture.Bind(0);
        m_SpecularTexture.Bind(1);*/

        //Renderer renderer;
        
        //for (int i = 0; i < m_ObjPositions.size(); ++i)
        //{
        //    m_Container.MoveTo(m_ObjPositions[i]);
        //    //float angle = 20.0f * (i+1);
        //    float angle = 50.0f;
        //    m_Container.Rotate((float)glfwGetTime() * glm::radians(angle),
        //        glm::vec3(1.0f, 0.3f, 0.5f));
        //    //m_LightSource.MoveTo(LightParams.lightPos);
        //    
        //    m_Container.DrawNoIndex();
        //    //m_LightSource.DrawNoIndex();
        //}
	}

	void TestModelImport::OnImGuiRender()
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
