#pragma once
#include "Window.h"

int main();

namespace Nora
{
	class Application
	{
	public:
		Application();

		~Application();

		void Run();
	private:
		std::unique_ptr<Window> window_;
	private:
		static Application* s_application_;
		friend int ::main();
	};

	Application* CreateApplication();
}

