#include "NoraPrecompiled.h"
#include "Vulkan.h"

#include "Engine/Core/Logger.h"

namespace Nora
{
	void Vulkan::Init()
	{
		context_.instance = CreateInstance();
		GetPhysicalDevice();
	}

	void Vulkan::SetViewport(uint32_t width, uint32_t height)
	{
		
	}

	vk::Instance Vulkan::CreateInstance()
	{
		vk::ApplicationInfo app_info("Nora Engine", VK_MAKE_VERSION(0, 1, 0),
			"No Engine", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_0);

		std::vector<const char*> extensions = 
		{
			VK_KHR_SURFACE_EXTENSION_NAME,
			VK_KHR_WIN32_SURFACE_EXTENSION_NAME
		};

		vk::InstanceCreateInfo info;
		info.pApplicationInfo = &app_info;

		// Enable validation layers for debug build
#ifdef NORA_DEBUG
		const std::vector<const char*> layers = 
		{
			"VK_LAYER_KHRONOS_validation"
		};

		if (!IsRequestedLayersAvailable(layers))
			throw std::runtime_error("Requested vulkan layer not available");

		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		info.enabledLayerCount = static_cast<uint32_t>(layers.size());
		info.ppEnabledLayerNames = layers.data();
#endif

		info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		info.ppEnabledExtensionNames = extensions.data();

		return vk::createInstance(info);
	}

	vk::PhysicalDevice Vulkan::GetPhysicalDevice()
	{
		for (const auto& device : context_.instance.enumeratePhysicalDevices())
		{
			auto props = device.getProperties();
		}

		return nullptr;
	}

	bool Vulkan::IsRequestedLayersAvailable(const std::vector<const char*> requested) const
	{
		auto layers_props = vk::enumerateInstanceLayerProperties();
		
		for (const auto request : requested)
			if (!std::any_of(layers_props.begin(), layers_props.end(),
				[request](const auto& props)
				{
					return strcmp(request, props.layerName);
				}))
				return false;

		return true;
	}

	Vulkan::~Vulkan()
	{
		context_.instance.destroy();
	}
}
