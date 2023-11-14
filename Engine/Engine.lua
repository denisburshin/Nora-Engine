project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "Off"

    targetdir ("bin/" .. outdir)
    objdir ("bin-int/" .. outdir)

    pchheader "NoraPrecompiled.h"
    pchsource "src/NoraPrecompiled.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "src",
        "%{Headers.GLFW}",
        "%{Headers.Vulkan}"
    }

    links 
    {
        "GLFW",
        "%{Library.Vulkan}"
    }

    defines
    {
        "GLFW_INCLUDE_NONE"
    }

    filter "system:Windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "NORA_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
        defines "NORA_RELEASE"
		runtime "Release"
		optimize "on"
        
    filter "configurations:Profile"
        defines "NORA_PROFILE"
		runtime "Release"
		optimize "on"