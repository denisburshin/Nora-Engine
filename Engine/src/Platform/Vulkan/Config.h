#pragma once

#ifdef NORA_PLATFORM_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1

namespace Nora
{
	struct Config
	{
		std::vector<const char*> instance_layers;
		std::vector<const char*> instance_extensions;
		std::vector<const char*> physical_device_extensions;

		bool enable_validation = false;

		Config();
	};

}