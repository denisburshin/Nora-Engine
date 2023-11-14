#pragma once
#include "Engine/Renderer/RendererImpl.h"

#ifdef NORA_PLATFORM_WINDOWS
	#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include <vulkan/vulkan.hpp>

namespace Nora
{

	struct VulkanContext
	{
		vk::Instance instance;
		vk::Device device;
		vk::SurfaceKHR surface;
	};

	class Vulkan : public RendererImpl
	{
	public:
		Vulkan() = default;
		virtual ~Vulkan();

		void Init() override;
		void SetViewport(uint32_t width, uint32_t height) override;
	private:
		vk::Instance CreateInstance();
		vk::PhysicalDevice GetPhysicalDevice();

		bool IsRequestedLayersAvailable(const std::vector<const char*> requested) const;
	private:
		VulkanContext context_;
	};
}


