#include "NoraPrecompiled.h"
#include "Application.h"
#include "Engine/Renderer/Renderer.h"

namespace Nora
{
	Application* Application::s_application_ = nullptr;

	Application::Application()
	{
		s_application_ = this;

		window_ = Window::Create();
		
		Renderer::Initialize();
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

	Application& Application::Get()
	{
		return *s_application_;
	}

	Window& Application::GetWindow()
	{
		return *window_;
	}
}
