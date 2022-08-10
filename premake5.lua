workspace "OpenGL_Demo"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "OpenGL_Demo/vendor/GLFW/include"
IncludeDir["GLAD"] = "OpenGL_Demo/vendor/GLAD/include"
IncludeDir["ImGui"] = "OpenGL_Demo/vendor/imgui"
IncludeDir["GLM"] = "OpenGL_Demo/vendor/GLM"
IncludeDir["entt"] = "OpenGL_Demo/vendor/entt"
IncludeDir["assimp"] = "OpenGL_Demo/vendor/assimp/include"
--IncludeDir["yaml_cpp"] = "OpenGL_Demo/vendor/yaml-cpp/include"
IncludeDir["boost"] = "C:/Program Files/boost/boost_1_79_0"
IncludeDir["ImGuizmo"] = "OpenGL_Demo/vendor/ImGuizmo"
IncludeDir["cereal"] = "OpenGL_Demo/vendor/cereal"

group "Dependencies"
	include "OpenGL_Demo/vendor/GLFW"
	include "OpenGL_Demo/vendor/GLAD"
	include "OpenGL_Demo/vendor/ImGui"

group ""

project "OpenGL_Demo"
	location "OpenGL_Demo"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "OpenGL_Demo/src/pch.cpp"
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp"
		
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/stb_image",

		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.cereal}",
		"%{IncludeDir.ImGuizmo}"
	}

	links 
	{ 
		"GLFW",
		"GLAD",
		"ImGui",
		"OpenGL_Demo/vendor/assimp/lib/assimp-vc143-mt.lib",
		"opengl32.lib"
	}
	
	filter "files:vendor/ImGuizmo/**.cpp"
	flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "Sandbox/src/pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		--"OpenGL_Demo/vendor/spdlog/include",
		"OpenGL_Demo/src",
		"OpenGL_Demo/vendor",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.cereal}"
	}

	links
	{
		"OpenGL_Demo"
	}

	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"