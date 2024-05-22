#pragma once
#include "Platform/Vulkan/Config.h"

#include <vulkan/vulkan.hpp>

namespace Nora
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphics_family;

		bool IsComplete() const;
	};

	class Device
	{
	public:
		Device(const vk::UniqueInstance& instance);
	private:
		vk::PhysicalDevice PickPhysicalDevice(const std::vector<vk::PhysicalDevice> devices) const;
		uint32_t RateDevice(const vk::PhysicalDevice& device) const;
		QueueFamilyIndices FindQueueFamilyIndices() const;
	private:
		vk::PhysicalDevice device_;
		QueueFamilyIndices family_indices_;
	};
}


