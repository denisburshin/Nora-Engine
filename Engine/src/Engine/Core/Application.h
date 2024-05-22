#pragma once
#include "Window.h"

int main();

namespace Nora
{
	class Application final
	{
	public:
		Application();

		~Application();

		void Run();

		static Application& Get();

		Window& GetWindow();

	private:
		std::unique_ptr<Window> window_;
	private:
		static Application* s_application_;
		friend int ::main();
	};

	Application* CreateApplication();
}

