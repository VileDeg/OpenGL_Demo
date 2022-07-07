#pragma once

#include "Window.h"

class GLContext
{
public:
	static GLContext& Get() { return *s_Instance; }

	static GLContext& Create();
	GLFWwindow* OpenWindow(const unsigned width = 800, const unsigned height = 600,
		const std::string& name = "Window");

	void Clear(std::bitset<3> bufferBits) const;
	void Terminate() const;
	void SetClearColors(float r, float g, float b, float a);
	void SwapBuffers() const;
	void PollEvents() const;
	void UpdateWindows()
	{
		m_Window->OnUpdate();
	}

	Window& GetWindow() const { return *m_Window; }

	~GLContext() {}

	GLContext(const GLContext&) = delete;
	void operator=(const GLContext&) = delete;
private:
	void Init();
	void SetKeybinds();

	/*void UpdateWindowDelayedKeybinds();
	void UpdateWindowCameraKeybinds();*/

	template<typename T>
    inline void RegisterDelayedKeybind(int keyId)
    {
        m_DelayedKeybinds.push_back(new T(keyId));
		//UpdateWindowDelayedKeybinds();
    }
    template<typename T>
    inline void RegisterCameraKeybind(int keyId)
    {
        m_CameraKeybinds.push_back(new T(keyId));
		//UpdateWindowCameraKeybinds();
    }

	GLContext();
	
private:
	/*std::vector<KeybindDelayed*> m_DelayedKeybinds;
	std::vector<KeybindCamera*>  m_CameraKeybinds;*/
	bool m_GladInit = false;
	std::unordered_map<KeyActionType, Keybind> m_Keybinds;
	std::shared_ptr<Window> m_Window = nullptr;
	static std::unique_ptr<GLContext> s_Instance;
};