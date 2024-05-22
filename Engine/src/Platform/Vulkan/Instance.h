#pragma once
#include "Platform/Vulkan/Config.h"

#include <vulkan/vulkan.hpp>

namespace Nora
{
	class Instance
	{
	public:
		Instance();

		const vk::UniqueInstance& GetInstance() const
		{
			return instance_;
		}

	private:
		vk::UniqueInstance CreateInstance() const;
		vk::UniqueDebugUtilsMessengerEXT CreateDebugMessenger() const;

		std::vector<const char*> GetRequiredExtensions() const;
		std::vector<const char*> GetRequiredLayers() const;

		bool IsRequestedLayersAvailable(const std::vector<const char*> requested) const;
		bool IsRequestedExtensionsAvailable(const std::vector<const char*>& requested) const;



		//template <class T, class U, class Visitor>
		//T FilterBySupport(T requested, U supported, const Visitor& visitor) const;
	private:
		vk::UniqueInstance instance_;
		vk::UniqueDebugUtilsMessengerEXT debug_messenger_;
	};

	//template <class T, class U, class Visitor>
	//T Instance::FilterBySupport(T requested, U supported, const Visitor& visitor) const
	//{
	//	std::unordered_set<std::string_view> supported_set;
	//	for (const auto& value : supported)
	//		supported_set.insert(visitor(value));
	//
	//
	//
	//}
}

