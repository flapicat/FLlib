workspace "FLlib"
	architecture "x64"
    startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["spdlog"] = "vendor/spdlog/include"
IncludeDir["GLFW"] = "vendor/GLFW/include"
IncludeDir["glad"] = "vendor/glad/include"
IncludeDir["glm"] = "vendor/glm"

group "Dependencies"
	include "vendor/GLFW"
	include "vendor/glad"
group ""

project "FLlib"
	location "FLlib"
	kind "StaticLib"
	language "C++"
	cppdialect "C++23"
	staticruntime "on"
	buildoptions "/utf-8"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.glm}",
	}
	links

	{
		"GLFW",
		"glad",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines { "RELEASE", "NDEBUG" }
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++23"
	staticruntime "on"
    buildoptions { "/utf-8" }

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"FLlib/src",
		"%{prj.name}/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.glm}",
	}

	links{
		"FLlib",
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		defines { "RELEASE", "NDEBUG" }
		runtime "Release"
		optimize "on"