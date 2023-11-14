#include "NoraPrecompiled.h"
#include "Windows.h"

namespace Nora::Windows
{
	static void ErrorCallback(int code, std::string& message)
	{
		NORA_ERROR("GLFW ERROR: " + message);
	}

	Window::Window()
		: window_(nullptr)
	{
		NORA_ASSERT(glfwInit());

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		
		window_ = glfwCreateWindow(1280, 720, "Nora Engine [Vulkan]", nullptr, nullptr);
	}

	Window::~Window()
	{
		glfwDestroyWindow(window_);
		glfwTerminate();
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(window_);
	}

	void Window::OnUpdate() const
	{
		glfwPollEvents();
	}
}