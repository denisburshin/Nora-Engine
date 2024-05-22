#pragma once
#include "Engine/Renderer/RendererImpl.h"

namespace Nora
{
	struct VulkanContext;

	class Vulkan : public RendererImpl
	{
	public:
		Vulkan();
		virtual ~Vulkan() = default;

		void Init() override;
		void SetViewport(uint32_t width, uint32_t height) override;
	private:
		std::unique_ptr<VulkanContext> context_;
	};
}


