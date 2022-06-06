workspace "VoxWorld"
	configurations { "Debug", "Release" }
	location ".."

	-- Global settings
	system "Windows"
	systemversion "latest"
	architecture "x86"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"
	defines { 
		"_CRT_SECURE_NO_WARNINGS", 
		"_CONSOLE"
	}

	-- VoxEngine include directories also needed by users
	includedirs {
		"%{wks.location}/Dependencies/glm",
		"%{wks.location}/Dependencies/entt/include",
		"%{wks.location}/Dependencies/glad",
		"%{wks.location}/Dependencies/GLFW/include",
		"%{wks.location}/Dependencies/libpng/include",
		"%{wks.location}/Dependencies/zlib/include",
		"%{wks.location}/Dependencies/FMT/include",
	}

	-- Global configuration settings
	filter "configurations:Debug"
		defines { 
			"DEBUG", 
			"_DEBUG",
		}
		symbols "On"

	filter "configurations:Release"
		defines { 
			"NDEBUG",
		}
		optimize "On"

	-- End of configurations filters
	filter {}

-- Projects
-- VOXENGINE
project "VoxEngine"
	location "../VoxEngine"
	kind "StaticLib"
	files {
		"%{wks.location}/%{prj.name}/**.cpp",
		"%{wks.location}/%{prj.name}/**.c",
		"%{wks.location}/%{prj.name}/**.hpp",
		"%{wks.location}/%{prj.name}/**.h",
		"%{wks.location}/Dependencies/glad/*.c",
	}
	includedirs {
		"%{prj.location}/src",
		"%{wks.location}/VoxEngine/voxpch",
	}
	
	filter "configurations:Debug"
		targetname "voxengine_s_d"
		targetdir "../bin/%{prj.name}/debug"
		objdir "../bin-int/%{prj.name}/debug"

	filter "configurations:Release"
		targetname "voxengine_s"
		targetdir "../bin/%{prj.name}/release"
		objdir "../bin-int/%{prj.name}/release"

	filter {}

	pchheader "voxpch.h"
	pchsource "../VoxEngine/voxpch/voxpch.cpp"


-- SANDBOX
project "Sandbox"
	location "../Sandbox"
	kind "ConsoleApp"
	files {
		"%{wks.location}/%{prj.name}/**.cpp",
		"%{wks.location}/%{prj.name}/**.c",
		"%{wks.location}/%{prj.name}/**.hpp",
		"%{wks.location}/%{prj.name}/**.h",
		"%{wks.location}/VoxEngine/voxpch/**"
	}
	includedirs {
		"%{prj.location}/src",
		"%{wks.location}/VoxEngine/voxpch",
		"%{wks.location}/VoxEngine/src"
	}
	-- Library links
	libdirs {
		"%{wks.location}/Dependencies/GLFW/lib",
		"%{wks.location}/Dependencies/libpng/lib",
		"%{wks.location}/Dependencies/zlib/lib",
		"%{wks.location}/Dependencies/FMT/lib",
	}
	links {
		"glfw3.lib",
		"libpng16_static.lib",
		"zlibstatic.lib",
	}
	links {
		"VoxEngine",
	}

	filter "configurations:Debug"
		targetname "sandbox_d"
		targetdir "../bin/%{prj.name}/debug"
		debugdir "../bin/%{prj.name}/debug"
		objdir "../bin-int/%{prj.name}/debug"
		links {
			"fmtd.lib"
		}

	filter "configurations:Release"
		targetname "sandbox"
		targetdir "../bin/%{prj.name}/release"
		objdir "../bin-int/%{prj.name}/release"
		links {
			"fmt.lib"
		}		
	filter {}

	pchheader "voxpch.h"
	pchsource "../VoxEngine/voxpch/voxpch.cpp"


workspace "VoxWorld"
	startproject "Sandbox"
	filter "files:**.c"
		flags "NoPCH"
	filter {}
		