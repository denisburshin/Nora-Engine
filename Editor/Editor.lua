project "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "Off"

	targetdir ("bin/" .. outdir)
    objdir ("bin-int/" .. outdir)
	
    files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Engine/src"
	} 

	links
	{
		"Engine"
	}

    filter "system:windows"
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