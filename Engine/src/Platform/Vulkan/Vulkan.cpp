#include "NoraPrecompiled.h"
#include "Vulkan.h"
#include "Platform/Vulkan/Instance.h"
#include "Platform/Vulkan/Device.h"

#include "Engine/Core/Logger.h"

namespace Nora
{
	struct VulkanContext
	{
		Instance instance;
		std::unique_ptr<Device> device;


		VulkanContext()
		{
			device = std::make_unique<Device>(instance.GetInstance());
		}
	};

	Vulkan::Vulkan()
	{
		Init();
	}

	void Vulkan::Init()
	{
		context_ = std::make_unique<VulkanContext>();
	}

	void Vulkan::SetViewport(uint32_t width, uint32_t height)
	{
	}
}
