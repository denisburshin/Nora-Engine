#include "NoraPrecompiled.h"
#include "Windows.h"

namespace Nora::Windows
{
	static void ErrorCallback(int code, const char* message)
	{
		NORA_ERROR(message);
	}

	Window::Window()
		: window_(nullptr)
	{
		glfwSetErrorCallback(ErrorCallback);
		
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

	std::vector<const char*> Window::GetRequiredExtensions() const
	{
		uint32_t count = 0;
		auto extensions_ptr = glfwGetRequiredInstanceExtensions(&count);

		return { extensions_ptr, extensions_ptr + count };
	}
}