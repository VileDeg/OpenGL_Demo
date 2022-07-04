#pragma once

#include "Window.h"

class GLContext
{
public:
	static GLContext& Get() { return *s_Instance; }

	static GLContext& Create();
	GLFWwindow* OpenWindow(const unsigned width = 800, const unsigned height = 600,
		const std::string& name = "Window");

	void Clear(int buffers) const { glClear(buffers); }
	void SetClearColors(float r, float g, float b, float a)
	{
		glClearColor(r,g,b,a);
	}
	void SwapBuffers() const { glfwSwapBuffers(m_Window->Handle()); }
	void PollEvents() const { glfwPollEvents(); }
	void UpdateWindows()
	{
		m_Window->OnUpdate();
	}

	const Window& GetWindow() const { return *m_Window; }

	GLContext(const GLContext&) = delete;
	void operator=(const GLContext&) = delete;
private:
	void Init();
	void SetKeybinds();
	void UpdateWindowDelayedKeybinds();
	void UpdateWindowCameraKeybinds();

	template<typename T>
    inline void RegisterDelayedKeybind(int keyId)
    {
        m_DelayedKeybinds.push_back(new T(keyId));
		UpdateWindowDelayedKeybinds();
    }
    template<typename T>
    inline void RegisterCameraKeybind(int keyId)
    {
        m_CameraKeybinds.push_back(new T(keyId));
		UpdateWindowCameraKeybinds();
    }

	GLContext();
	~GLContext() {}
private:
	std::vector<KeybindDelayed*> m_DelayedKeybinds;
	std::vector<KeybindCamera*>  m_CameraKeybinds;
	std::shared_ptr<Window> m_Window = nullptr;
	static GLContext* s_Instance;
};