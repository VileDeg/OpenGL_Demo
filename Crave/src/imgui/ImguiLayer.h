#pragma once

struct GLFWwindow;

namespace Crave
{
	namespace ImguiLayer
	{
		void Init(GLFWwindow* window, const char* projectName);

		void Begin();
		void End(float width, float height);
		void Shutdown();
	}
}