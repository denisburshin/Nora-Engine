#pragma once
#include "Engine/Renderer/RendererImpl.h"

#include "VulkanContext.h"

namespace Nora
{
	class Vulkan : public RendererImpl
	{
	public:
		Vulkan() = default;
		virtual ~Vulkan() = default;	

		void Init() override;
		void SetViewport(uint32_t width, uint32_t height) override;
	private:
		VulkanContext context_;
	};
}


