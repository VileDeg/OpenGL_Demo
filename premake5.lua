workspace "Crave"
	architecture "x64"
	startproject "CavernEditor"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Crave/vendor/GLFW/include"
IncludeDir["GLAD"] = "Crave/vendor/GLAD/include"
IncludeDir["ImGui"] = "Crave/vendor/imgui"
IncludeDir["GLM"] = "Crave/vendor/GLM"
IncludeDir["entt"] = "Crave/vendor/entt"
IncludeDir["assimp"] = "Crave/vendor/assimp/include"
IncludeDir["ImGuizmo"] = "Crave/vendor/ImGuizmo"
IncludeDir["stb_image"] = "Crave/vendor/stb_image"
IncludeDir["cereal"] = "Crave/vendor/cereal"
IncludeDir["magic_enum"] = "Crave/vendor/magic_enum"
IncludeDir["spdlog"] = "Crave/vendor/spdlog/include"

group "Dependencies"
	include "Crave/vendor/GLFW"
	include "Crave/vendor/GLAD"
	include "Crave/vendor/ImGui"

group ""

assimplib = "assimp-vc143-mtd"

project "Crave"
	location "Crave"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "Crave/src/pch.cpp"
	
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

		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.cereal}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.magic_enum}",
		"%{IncludeDir.spdlog}"
	}

	links 
	{ 
		"GLFW",
		"GLAD",
		"ImGui",
		"Crave/vendor/assimp/lib/%{assimplib}.lib",
		"opengl32.lib"
	}
	
	filter "files:Crave/vendor/ImGuizmo/**.cpp"
	flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"


project "CavernEditor"
	location "CavernEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "CavernEditor/src/pch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"CavernEditor/src",

		"Crave/src",
		"Crave/vendor",

		"%{IncludeDir.GLM}",
		"%{IncludeDir.entt}",
		--"%{IncludeDir.assimp}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.cereal}",
		"%{IncludeDir.spdlog}"
	}

	links {
		"Crave"
	}
	
	filter "system:windows"
		systemversion "latest"
		postbuildcommands {
			"CopyAssimpDLLToExeFolder.bat %{assimplib}.dll %{cfg.targetdir}"
		}
		
	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"