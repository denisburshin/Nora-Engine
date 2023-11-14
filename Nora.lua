include "Submodules.lua"

workspace "Nora Engine"
    architecture "x64"
    configurations
    {
        "Debug",
        "Release",
        "Profile"
    }

    flags { "MultiProcessorCompile" }

outdir = "%{cfg.buildcfg}-x64"

group "Submodules"
    include "Submodules/glfw/GLFW.lua"
group ""

include "Engine/Engine.lua"
include "Editor/Editor.lua"


