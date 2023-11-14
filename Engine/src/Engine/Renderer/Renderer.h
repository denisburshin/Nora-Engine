#pragma once
#include "RendererImpl.h"

namespace Nora
{
	class Renderer
	{
	public:
		enum class API
		{
			OpenGL, Vulkan, DirectX, None
		};
	public:
		static void Initialize(const API graphics_api = API::Vulkan);
		static void SetViewport(uint32_t width, uint32_t height);
	private:
		static std::unique_ptr<RendererImpl> s_renderer_impl_;
	};
};

