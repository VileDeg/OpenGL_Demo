#pragma once


struct GLFWwindow;

class ImguiLayer
{
public:
	static void Init(GLFWwindow* window, const char* projectName);

	static void Begin();
	static void End(float width, float height);
	static void Shutdown();

	static void SetDarkThemeColors();
};