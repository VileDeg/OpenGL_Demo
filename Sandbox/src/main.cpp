#include "OpenGL_Demo.h"

#include "imgui/ImguiLayer.h"
#include "imgui/imgui.h"

#include "tests/TestRenderer.h"


const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

int main()
{
    GLContext& context = GLContext::Create();
    GLFWwindow* windowHandle = context.OpenWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL_Demo");
    Window& window = context.GetWindow();

    Renderer::Init(SCR_WIDTH, SCR_HEIGHT);
    ImguiLayer::Init(windowHandle);

    test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(window,currentTest);
    currentTest = testMenu;

    testMenu->RegisterTest<test::TestRenderer>(window, "TestRenderer");
    testMenu->RunNextTest();

    
    /*test::TestRenderer* testRenderer = new test::TestRenderer(window);
    currentTest = testRenderer;*/
    window.HideCursor();
    while (window.IsOpen())
    {
        context.PollEvents();

        context.UpdateWindows();

        Renderer::SetClearColor(0.049f, 0.0f, 0.1f, 1.f);
        Renderer::Clear(0b111);

        ImguiLayer::Begin();
        
        if (currentTest)
        {
            if (!window.Paused())
            {
                currentTest->OnUpdate(window.DeltaTime());
                currentTest->OnRender();
            }
            

            ImGui::Begin("Menu");
            if (currentTest != testMenu && ImGui::Button("<-"))
            {
                delete currentTest;
                currentTest = testMenu;
                window.ShowCursor();
            }
            currentTest->OnImGuiRender();
            ImGui::End();
        }

        ImguiLayer::End(window.Width(), window.Height());

        context.SwapBuffers();
    }
    delete testMenu;
    if (currentTest != testMenu)
        delete currentTest;

    ImguiLayer::Shutdown();

    context.Terminate();
    return 0;
}
