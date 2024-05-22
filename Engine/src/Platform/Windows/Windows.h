#pragma once
#include "Engine/Core/Window.h"
#include <GLFW/glfw3.h>

namespace Nora::Windows
{
	class Window final : public Nora::Window 
	{
	public:
		Window();
		~Window();

		bool ShouldClose() const;
		void OnUpdate() const override;

		std::vector<const char*> GetRequiredExtensions() const override;

	private:
		GLFWwindow* window_;
	};
}


