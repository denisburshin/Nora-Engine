#include "NoraPrecompiled.h"
#include "Device.h"

namespace Nora
{
	Device::Device(const vk::UniqueInstance& instance)
	{
		device_ = PickPhysicalDevice(instance->enumeratePhysicalDevices());
		const auto info = device_.getProperties();
		NORA_INFO("Picked device: {}", static_cast<std::string_view>(info.deviceName));
	}

	vk::PhysicalDevice Device::PickPhysicalDevice(const std::vector<vk::PhysicalDevice> devices) const
	{
		if (devices.empty())
			throw std::runtime_error("Empty list of GPU's!");

		std::multimap<uint32_t, vk::PhysicalDevice> ratings;

		for (const auto& device : devices)
			ratings.insert({ RateDevice(device), device });

		if (ratings.rbegin()->first > 0)
			return ratings.rbegin()->second;

		throw std::runtime_error("Suitable GPU not found!");
	}

	uint32_t Device::RateDevice(const vk::PhysicalDevice& device) const
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

	QueueFamilyIndices Device::FindQueueFamilyIndices() const
	{
		uint32_t index = 0;
		for (const auto& properties : device_.getQueueFamilyProperties())
		{
			if (properties.queueFlags & vk::QueueFlagBits::eGraphics)
				return { index };

			++index;
		}
		return { };
	}

	bool QueueFamilyIndices::IsComplete() const
	{
		return graphics_family.has_value();
	}
}