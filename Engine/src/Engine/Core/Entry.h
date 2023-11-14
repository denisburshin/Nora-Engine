#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Core/Application.h"

extern Nora::Application* Nora::CreateApplication();

int main()
{
	NORA_INFO("Application startup.");
	auto app = Nora::CreateApplication();
	
	app->Run();

	NORA_INFO("Application stop.");
	delete app;
}
