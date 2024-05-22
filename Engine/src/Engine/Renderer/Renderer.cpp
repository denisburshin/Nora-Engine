#include "NoraPrecompiled.h"
#include "Renderer.h"

#include "Platform/Vulkan/Vulkan.h"

namespace Nora
{
	std::unique_ptr<RendererImpl> Renderer::s_renderer_impl_ = nullptr;

	void Renderer::Initialize(const API graphics_api)
	{
		switch (graphics_api)
		{
		case Nora::Renderer::API::OpenGL:
			NORA_ASSERT(false);
			break;
		case Nora::Renderer::API::Vulkan:
			s_renderer_impl_ = std::make_unique<Vulkan>();
			break;
		case Nora::Renderer::API::DirectX:
			NORA_ASSERT(false);
			break;
		case Nora::Renderer::API::None:
			NORA_ASSERT(false);
			break;
		default:
			NORA_ASSERT(false);
			break;
		}
	}

	void Renderer::SetViewport(uint32_t width, uint32_t height)
	{
		s_renderer_impl_->SetViewport(width, height);
	}
}