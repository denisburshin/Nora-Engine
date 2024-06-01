#include "NoraPrecompiled.h"
#include "Config.h"

#include "Engine/Core/Application.h"

#include <vulkan/vulkan.hpp>

namespace Nora
{
	Config::Config()
	{

#ifdef NORA_DEBUG
		enable_validation = true;
#else
		enable_validation = false;
#endif

		if (enable_validation)
			instance_layers = { "VK_LAYER_KHRONOS_validation" };

		instance_extensions = Application::Get().GetWindow().GetRequiredExtensions();

		if (enable_validation)
			instance_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		physical_device_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	}
}

