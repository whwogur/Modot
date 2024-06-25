workspace "Yeti"
	architecture "x64"
	startproject "Client"

	configurations
	{
		"Debug",
		"Release"
	}

project "Engine"
	location "Project/Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("External/Library/Engine")
	objdir ("%{cfg.architecture}/{cfg.buildcfg}")

	files
	{
		"Project/Engine/**.h",
		"Project/Engine/**.cpp"
	}

	filter "system:windows"
		systemversion "latest"
		defines
		{
			"PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "_NDEBUG"
		runtime "Release"
		optimize "on"

project "Client"
	location "Project/Client"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	dependson { "Engine" }
	links { "Engine" }

	targetdir ("OutputFile/bin")
	objdir ("%{cfg.architecture}/{cfg.buildcfg}")

	files
	{
		"Project/Client/**.h",
		"Project/Client/**.cpp"
	}

	includedirs
	{
		"Project/Engine",
        "External/spdlog/include"
	}

	filter "system:windows"
		systemversion "latest"
		defines
		{
			"PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "_NDEBUG"
		runtime "Release"
		optimize "on"
