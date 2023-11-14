-- Nora Engine Submodules

VULKAN_SDK = os.getenv("VULKAN_SDK")

Headers = {}
Headers["GLFW"] = "%{wks.location}/Submodules/glfw/include"
Headers["Vulkan"] = "%{VULKAN_SDK}/Include"

Library = {}
Library["Vulkan"] = "%{VULKAN_SDK}/Lib/vulkan-1.lib"