#include "NoraPrecompiled.h"
#include "VulkanContext.h"

#include "Engine/Core/Application.h"

#include <GLFW/glfw3.h>
#include <limits>
#include <ranges>

namespace Nora
{
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

	VulkanContext::VulkanContext()
	{
		VULKAN_HPP_DEFAULT_DISPATCHER.init();
		instance_ = CreateInstance();
		VULKAN_HPP_DEFAULT_DISPATCHER.init(*instance_);
		debug_messenger_ = CreateDebugMessenger();
		surface_ = CreateSurface();
		physical_device_ = PickPhysicalDevice();
		logical_device_ = CreateLogicalDevice();

		InitializeQueues();

		swap_chain_ = CreateSwapChain();

		images_ = logical_device_->getSwapchainImagesKHR(*swap_chain_);

		NORA_ASSERT(images_.size() > 0);

		InitializeImageViews();
	}

	vk::UniqueInstance VulkanContext::CreateInstance() const
	{
		vk::ApplicationInfo app_info("Nora Engine", VK_MAKE_VERSION(0, 1, 0),
			"No Engine", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_0);

		if (!IsRequestedLayersAvailable(config.instance_layers))
			throw std::runtime_error("Requested vulkan layer not available");
		
		if (!IsRequestedExtensionsAvailable(config.instance_extensions))
			throw std::runtime_error("Requested vulkan extension not available");

		const auto debug_info = CreateDebugInfo();

		return vk::createInstanceUnique(vk::InstanceCreateInfo().
			setPApplicationInfo(&app_info).
			setPEnabledLayerNames(config.instance_layers).
			setPEnabledExtensionNames(config.instance_extensions).
			setPNext(config.enable_validation ? &debug_info : nullptr));
	}

	bool VulkanContext::IsRequestedLayersAvailable(const std::vector<const char*> requested) const
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

	bool VulkanContext::IsRequestedExtensionsAvailable(const std::vector<const char*>& requested) const
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

	vk::UniqueDebugUtilsMessengerEXT VulkanContext::CreateDebugMessenger() const
	{
		if (!config.enable_validation)
			return {};

		return instance_->createDebugUtilsMessengerEXTUnique(CreateDebugInfo());
	}

	vk::UniqueSurfaceKHR VulkanContext::CreateSurface() const
	{
		const auto window = Application::Get().GetWindow().GetNativeWindow();

		VkSurfaceKHR surface;
		if (glfwCreateWindowSurface(*instance_, static_cast<GLFWwindow*>(window), nullptr, &surface))
			throw std::runtime_error("Failed to create window surface!");

		return vk::UniqueSurfaceKHR(surface, *instance_);
	}

	vk::PhysicalDevice VulkanContext::PickPhysicalDevice() const
	{
		const auto devices = instance_->enumeratePhysicalDevices();

		if (devices.empty())
			throw std::runtime_error("Empty list of GPU's!");

		std::multimap<uint32_t, vk::PhysicalDevice> ratings;

		for (const auto& device : devices)
			if (IsDeviceSuitable(device))
				ratings.insert({ RateDevice(device), device});

		if (ratings.rbegin()->first > 0)
			return ratings.rbegin()->second;

		throw std::runtime_error("Suitable GPU not found!");
	}

	vk::UniqueDevice VulkanContext::CreateLogicalDevice() const
	{
		const auto priority = 1.0f;
		const auto queue_family = FindQueueFamilyIndices(physical_device_);

		std::unordered_set<uint32_t> queue_families { *queue_family.graphics_family, *queue_family.presentation_family };

		std::vector<vk::DeviceQueueCreateInfo> queue_info;
		for (const auto queue_family_index : queue_families)
		{
			queue_info.push_back(vk::DeviceQueueCreateInfo().setQueueFamilyIndex(queue_family_index).
				setQueueCount(1).
				setQueuePriorities(priority));
		}

		vk::DeviceCreateInfo create_info(vk::DeviceCreateInfo().
			setPEnabledExtensionNames(config.physical_device_extensions).
			setQueueCreateInfos(queue_info));

		return physical_device_.createDeviceUnique(create_info);
	}

	vk::UniqueSwapchainKHR VulkanContext::CreateSwapChain()
	{
		const auto details = QuerySwapChainDetails(physical_device_);

		format_ = PickSwapSurfaceFormat(details.formats);
		extent_ = PickSwapExtent(details.capabilities);
		
		const auto present_mode = PickSwapPresentMode(details.present_modes);

		uint32_t max_image_count = details.capabilities.minImageCount + 1;

		if (details.capabilities.maxImageCount > 0 && max_image_count > details.capabilities.maxImageCount)
			max_image_count = details.capabilities.maxImageCount;

		vk::SwapchainCreateInfoKHR create_info(vk::SwapchainCreateInfoKHR().
			setSurface(*surface_).
			setMinImageCount(max_image_count).
			setImageFormat(format_.format).
			setImageColorSpace(format_.colorSpace).
			setImageExtent(extent_).
			setImageArrayLayers(1).
			setImageUsage(vk::ImageUsageFlagBits::eColorAttachment));

		const auto indices = FindQueueFamilyIndices(physical_device_);

		std::array<uint32_t, 2> queue_family_indices = { *indices.graphics_family, *indices.presentation_family };

		if (*indices.graphics_family != *indices.presentation_family)
			create_info.setImageSharingMode(vk::SharingMode::eConcurrent).
			setQueueFamilyIndices(queue_family_indices);
		else
			create_info.setImageSharingMode(vk::SharingMode::eExclusive);

		create_info.setPreTransform(details.capabilities.currentTransform).
			setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque).
			setClipped(vk::True);

		return logical_device_->createSwapchainKHRUnique(create_info);
	}

	bool VulkanContext::IsDeviceSuitable(const vk::PhysicalDevice& device) const
	{
		NORA_ASSERT(FindQueueFamilyIndices(device).IsComplete());
		NORA_ASSERT(CheckDeviceExtensionsSupport(device, config.physical_device_extensions));
		NORA_ASSERT(QuerySwapChainDetails(device).IsSupported());

		//return FindQueueFamilyIndices(device).IsComplete()
		//	&& CheckDeviceExtensionsSupport(device, config.physical_device_extensions)
		//	&& QuerySwapChainDetails(device).IsSupported();

		return true;
	}

	uint32_t VulkanContext::RateDevice(const vk::PhysicalDevice& device) const
	{
		const auto features = device.getFeatures();
		const auto properties = device.getProperties();
		const auto memory_properties = device.getMemoryProperties();

		uint32_t rating = 0;
		rating += (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) * 1000;
		rating += properties.limits.maxImageDimension2D;
		rating += memory_properties.memoryHeapCount;

		return features.geometryShader * rating;
	}

	bool VulkanContext::CheckDeviceExtensionsSupport(const vk::PhysicalDevice& device, const std::vector<const char*> extensions) const
	{
		const auto available_extensions = device.enumerateDeviceExtensionProperties() | std::views::transform(&vk::ExtensionProperties::extensionName);

		std::unordered_set<std::string> extensions_set(extensions.cbegin(), extensions.cend());
	
		for (const auto& available_extension : available_extensions)
			extensions_set.erase(available_extension);

		NORA_ASSERT(extensions_set.empty());

		return extensions_set.empty();
	}

	QueueFamilyIndices VulkanContext::FindQueueFamilyIndices(const vk::PhysicalDevice& device) const
	{
		QueueFamilyIndices indices;

		uint32_t index = 0;
		for (const auto& properties : device.getQueueFamilyProperties())
		{
			if (properties.queueFlags & vk::QueueFlagBits::eGraphics)
				indices.graphics_family = index;

			if (device.getSurfaceSupportKHR(index, *surface_))
				indices.presentation_family = index;
		}

		return indices;
	}

	void VulkanContext::InitializeQueues()
	{
		const auto queue_families = FindQueueFamilyIndices(physical_device_);

		graphics_queue_ = logical_device_->getQueue(*queue_families.graphics_family, 0);
		presentation_queue_ = logical_device_->getQueue(*queue_families.presentation_family, 0);
	}

	SwapChainDetails VulkanContext::QuerySwapChainDetails(const vk::PhysicalDevice& device) const
	{
		SwapChainDetails details;

		details.capabilities = device.getSurfaceCapabilitiesKHR(*surface_);
		details.formats = device.getSurfaceFormatsKHR(*surface_);
		details.present_modes = device.getSurfacePresentModesKHR(*surface_);

		return details;
	}

	vk::SurfaceFormatKHR VulkanContext::PickSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats) const
	{
		for (const auto& format : formats)
			if (format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
				return format;

		return formats[0];
	}

	vk::PresentModeKHR VulkanContext::PickSwapPresentMode(const std::vector<vk::PresentModeKHR>& modes) const
	{
		for (const auto& mode : modes)
			if (mode == vk::PresentModeKHR::eMailbox)
				return mode;

		return vk::PresentModeKHR::eFifo;
	}

	vk::Extent2D VulkanContext::PickSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) const
	{
		if (capabilities.currentExtent.width != (std::numeric_limits<uint32_t>::max)())
			return capabilities.currentExtent;

		const auto [width, height] = Application::Get().GetWindow().GetFramebufferSize();

		vk::Extent2D extent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

		extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return extent;
	}

	void VulkanContext::InitializeImageViews()
	{
		image_views_.resize(images_.size());

		for (int i = 0; i < images_.size(); ++i)
		{
			vk::ImageViewCreateInfo create_info(vk::ImageViewCreateInfo().
				setImage(images_[i]).
				setViewType(vk::ImageViewType::e2D).
				setFormat(format_.format).
				setSubresourceRange({ vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 }));

			image_views_[i] = logical_device_->createImageViewUnique(create_info);
		}
	}

	bool QueueFamilyIndices::IsComplete() const
	{
		return graphics_family.has_value() && presentation_family.has_value();
	}

	bool SwapChainDetails::IsSupported() const
	{
		return !formats.empty() && !present_modes.empty();
	}
}