#include <iostream>

#include "GLContext.h"

#include "Renderer.h"
#include "Texture.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestObject3D.h"
#include "tests/TestCamera.h"
#include "tests/TestLight.h"

#include "input/InputManager.h"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

void SetKeybinds()
{
    InputManager& inp = InputManager::getInstance();

    inp.RegisterKeybind<Keybind_Forward>(GLFW_KEY_W);
    inp.RegisterKeybind<Keybind_Backward>(GLFW_KEY_S);
    inp.RegisterKeybind<Keybind_Left>(GLFW_KEY_A);
    inp.RegisterKeybind<Keybind_Right>(GLFW_KEY_D);
    inp.RegisterKeybind<Keybind_CloseWindow>(GLFW_KEY_ESCAPE);
}

float DeltaTimer()
{
    static float lastFrame = 0.0f;
    static float deltaTime = 0.0f;
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    return deltaTime;
}

int main()
{
    GLContext& context = GLContext::getTnstance();
    context.SetWindowSize(SCR_WIDTH, SCR_HEIGHT);
    context.SetWindowTitle("OpenGL Demo");
    /*context.OpenWindow();
    context.SetFBSizeCallback(GLContext::default_fb_size_callback);
    context.SetGlobalSettings();*/
    //context.SetParams(SCR_WIDTH, SCR_HEIGHT, "Omega Engine");
    InputManager& inputManager = InputManager::getInstance();
    SetKeybinds();

    Renderer renderer;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(context.Window(), true);
    const char* glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version);

    test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(currentTest);
    currentTest = testMenu;

    testMenu->RegisterTest<test::TestClearColor>("Clear Color");
    testMenu->RegisterTest<test::TestTexture2D> ("2D Texture");
    testMenu->RegisterTest<test::TestObject3D>  ("3D Object");//, SCR_WIDTH/SCR_HEIGHT
    testMenu->RegisterTest<test::TestCamera>    ("Camera");
    testMenu->RegisterTest<test::TestLight>     ("Light");
    
    
    while (!glfwWindowShouldClose(context.Window()))
    {
        context.SetDeltaTime(DeltaTimer());
        glfwPollEvents();
        inputManager.ProcessInput();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        renderer.Clear();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
        
        if (currentTest)
        {
            currentTest->OnUpdate(0.0f);
            currentTest->OnRender();
            ImGui::Begin("Test");
            if (currentTest != testMenu && ImGui::Button("<-"))
            {
                delete currentTest;
                currentTest = testMenu;
            }
            currentTest->OnImGuiRender();
            ImGui::End();
        }

        //processInput(context.Window());
        

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //glfwSwapInterval(1);
        glfwSwapBuffers(context.Window());
   
        
        //glfwWaitEvents();
    }
    delete testMenu;
    if (currentTest != testMenu)
        delete currentTest;


	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}