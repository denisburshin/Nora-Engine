#pragma once

namespace Nora
{
	class RendererImpl
	{
	public:
		virtual void SetViewport(uint32_t width, uint32_t height) = 0;
	private:
		virtual void Init() = 0;
	};
}


