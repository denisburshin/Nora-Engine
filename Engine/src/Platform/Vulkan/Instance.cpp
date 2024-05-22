#include "NoraPrecompiled.h"
#include "Instance.h"

#include "Engine/Core/Application.h"

namespace Nora
{

#ifdef NORA_DEBUG
	const bool ENABLE_VALIDATION = true;
#else
	const bool ENABLE_VALIDATION = false;
#endif

	static Logger::Level VulkanSeverityToLogLevel(VkDebugUtilsMessageSeverityFlagBitsEXT severity)
	{
		auto level = Logger::GetInstance().GetCurrentLevel();
		switch (severity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			level = Logger::Level::Trace;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			level = Logger::Level::Info;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			level = Logger::Level::Warning;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			level = Logger::Level::Error;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
			break;
		default:
			break;
		}
		return level;
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebug(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type,
		const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data)
	{
		NORA_LOG(VulkanSeverityToLogLevel(severity), "Vulkan Debug: {}\n", callback_data->pMessage);
		return false;
	}

	static vk::DebugUtilsMessengerCreateInfoEXT CreateDebugInfo()
	{
		return vk::DebugUtilsMessengerCreateInfoEXT({},
			//vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning
			| vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
			vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
			| vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
			| vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
			VulkanDebug);
	}

	Instance::Instance()
	{	
		VULKAN_HPP_DEFAULT_DISPATCHER.init();
		instance_ = CreateInstance();
		VULKAN_HPP_DEFAULT_DISPATCHER.init(*instance_);
		debug_messenger_ = CreateDebugMessenger();
	}

	vk::UniqueInstance Instance::CreateInstance() const
	{
		vk::ApplicationInfo app_info("Nora Engine", VK_MAKE_VERSION(0, 1, 0),
			"No Engine", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_0);

		const auto layers = GetRequiredLayers();
		const auto extensions = GetRequiredExtensions();
		const auto debug_info = CreateDebugInfo();

		return vk::createInstanceUnique(vk::InstanceCreateInfo({}, &app_info,
			static_cast<uint32_t>(layers.size()), layers.data(),
			static_cast<uint32_t>(extensions.size()), extensions.data(),
			ENABLE_VALIDATION ? &debug_info : nullptr));
	}

	std::vector<const char*> Instance::GetRequiredExtensions() const
	{
		auto extensions = Application::Get().GetWindow().GetRequiredExtensions();

		if (ENABLE_VALIDATION)
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		if (!IsRequestedExtensionsAvailable(extensions))
			throw std::runtime_error("Requested vulkan extension not available");

		return extensions;
	}

	std::vector<const char*> Instance::GetRequiredLayers() const
	{
		if (!ENABLE_VALIDATION)
			return {};

		std::vector<const char*> layers = { 
			"VK_LAYER_KHRONOS_validation" 
		};

		if (!IsRequestedLayersAvailable(layers))
			throw std::runtime_error("Requested vulkan layer not available");

		return layers;
	}

	bool Instance::IsRequestedLayersAvailable(const std::vector<const char*> requested) const
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

	bool Instance::IsRequestedExtensionsAvailable(const std::vector<const char*>& requested) const
	{
		const auto supported_extensions = vk::enumerateInstanceExtensionProperties();
		for (const auto& request : requested)
		{
			if (!std::any_of(supported_extensions.begin(), supported_extensions.end(),
				[request](const auto& props)
				{
					return strcmp(request, props.extensionName);
				}))
				return false;
		}
		return true;
	}

	vk::UniqueDebugUtilsMessengerEXT Instance::CreateDebugMessenger() const
	{
		if (!ENABLE_VALIDATION)
			return {};

		return instance_->createDebugUtilsMessengerEXTUnique(CreateDebugInfo());
	}
}

