#include "NoraPrecompiled.h"
#include "Window.h"

#ifdef NORA_PLATFORM_WINDOWS
	#include "Platform/Windows/Windows.h"
#endif 

namespace Nora
{
	std::unique_ptr<Window> Window::Create()
	{
		#ifdef NORA_PLATFORM_WINDOWS
			return std::make_unique<Windows::Window>();
		#else
			NORA_ASSERT(false, "Unknown platform!");
			return nullptr;
		#endif
	}
}