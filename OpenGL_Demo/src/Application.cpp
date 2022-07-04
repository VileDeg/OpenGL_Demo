#include "pch.h"

#include "GLContext.h"


#include "imgui.h"

#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "tests/TestOutline.h"


const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

int main()
{
    GLContext& context = GLContext::Create();
    GLFWwindow* window = context.OpenWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL_Demo");
    

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version);

    test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(context.GetWindow(),currentTest);
    currentTest = testMenu;

    testMenu->RegisterTest<test::TestOutline>(context.GetWindow(), "Outline");

    
    while (!glfwWindowShouldClose(window))
    {
        context.PollEvents();
        context.UpdateWindows();


        context.SetClearColors(0.049f, 0.0f, 0.1f, 1.f);
        context.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
        
        if (currentTest)
        {
            currentTest->OnUpdate(context.GetWindow().DeltaTime());
            currentTest->OnRender();
            ImGui::Begin("Test");
            if (currentTest != testMenu && ImGui::Button("<-"))
            {
                delete currentTest;
                currentTest = testMenu;
                context.GetWindow().ShowCursor();
            }
            currentTest->OnImGuiRender();
            ImGui::End();
        }

        

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        context.SwapBuffers();
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
