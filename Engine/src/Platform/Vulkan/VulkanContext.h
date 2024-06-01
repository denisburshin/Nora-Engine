#pragma once
#include "Platform/Vulkan/Config.h"

#include <vulkan/vulkan.hpp>

namespace Nora
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphics_family;
		std::optional<uint32_t> presentation_family;

		bool IsComplete() const;
	};

	struct SwapChainDetails
	{
		vk::SurfaceCapabilitiesKHR capabilities;
		std::vector<vk::SurfaceFormatKHR> formats;
		std::vector<vk::PresentModeKHR> present_modes;

		bool IsSupported() const;
	};

	class VulkanContext
	{
	public:
		VulkanContext();
	private:
		vk::UniqueInstance CreateInstance() const;
		vk::UniqueDebugUtilsMessengerEXT CreateDebugMessenger() const;
		vk::UniqueSurfaceKHR CreateSurface() const;
		vk::PhysicalDevice PickPhysicalDevice() const;
		vk::UniqueDevice CreateLogicalDevice() const;
		vk::UniqueSwapchainKHR CreateSwapChain();

		bool IsRequestedLayersAvailable(const std::vector<const char*> requested) const;
		bool IsRequestedExtensionsAvailable(const std::vector<const char*>& requested) const;

		bool IsDeviceSuitable(const vk::PhysicalDevice& device) const;
		uint32_t RateDevice(const vk::PhysicalDevice& device) const;
		bool CheckDeviceExtensionsSupport(const vk::PhysicalDevice& device, const std::vector<const char*> extensions) const;
		QueueFamilyIndices FindQueueFamilyIndices(const vk::PhysicalDevice& device) const;

		void InitializeQueues();

		SwapChainDetails QuerySwapChainDetails(const vk::PhysicalDevice& device) const;
		vk::SurfaceFormatKHR PickSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats) const;
		vk::PresentModeKHR PickSwapPresentMode(const std::vector<vk::PresentModeKHR>& modes) const;
		vk::Extent2D PickSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) const;

		void InitializeImageViews();

	private:
		//Handles
		vk::UniqueInstance instance_;
		vk::UniqueDebugUtilsMessengerEXT debug_messenger_;
		vk::UniqueSurfaceKHR surface_;
		vk::PhysicalDevice physical_device_;
		vk::UniqueDevice logical_device_;
		vk::UniqueSwapchainKHR swap_chain_;

		//Queues
		vk::Queue graphics_queue_;
		vk::Queue presentation_queue_;

		//Images
		std::vector<vk::Image> images_;
		std::vector<vk::UniqueImageView> image_views_;

		//TODO: Maybe abstract to SwapChain struct
		vk::SurfaceFormatKHR format_;
		vk::Extent2D extent_;

		Config config;
	};
}


