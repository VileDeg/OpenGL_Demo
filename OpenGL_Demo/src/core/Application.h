#pragma once

class Application
{
public:
	Application();
	~Application();
	void Run();
	
private:
	bool m_Running;

};

// To be defined in CLIENT
Application* CreateApplication();