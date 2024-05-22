#pragma once

#include <memory>

namespace Nora
{
	class Window
	{
	public:
		virtual ~Window() = default;

		virtual void OnUpdate() const = 0;
		virtual bool ShouldClose() const = 0;

		virtual std::vector<const char*> GetRequiredExtensions() const = 0;

		static std::unique_ptr<Window> Create();
	};
}