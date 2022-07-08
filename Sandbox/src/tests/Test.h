#pragma once
#include "Window.h"


namespace test
{
	class Test
	{
	public:
		Test(Window& window) : m_Window(window) {}
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	protected:
		Window& m_Window;
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Window& window, Test*& currentTestPointer)
			: Test(window), m_CurrentTest(currentTestPointer) {}
		
		~TestMenu() {}

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(Window& window, const std::string& name)
		{
			std::cout << "Registering test " << name << std::endl;

			m_Tests.push_back(std::make_pair(name, [&]() {return new T(window); }));
		}
	private:
		Test*& m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
	};
}