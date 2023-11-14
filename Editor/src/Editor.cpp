#include "Nora.h"
#include "Engine/Core/Entry.h"

Nora::Application* Nora::CreateApplication()
{
	return new Application;
}