#include "NoraPrecompiled.h"
#include "Application.h"
#include "Engine/Renderer/Renderer.h"

namespace Nora
{
	Application::Application()
	{
		window_ = Window::Create();

		Renderer::Initialize(Renderer::API::Vulkan);
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{		
		while (!window_->ShouldClose())
		{
			window_->OnUpdate();
		}
	}
}
