--[[
This lua code file is used for premake5, a build tool, similar to cmake.
[https://premake.github.io/]
Download premake, and run the command:
/path/to/premake/executable/premake5 vs2019
to generate visual studios 2019 project files.
If another version of visual studios is desired, use the appropreate options in premake.
--]]

workspace("Tara")
	architecture("x64")
	configurations{"Debug", "Release"}
	startproject("Playground")
	

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--standard includes
IncludeDir = {
	glad = "Vendor/glad/include",
	glfw = "Vendor/glfw/include",
	loguru = "Vendor/loguru/include",
	glm = "Vendor/glm",
	stb = "Vendor/stb",
	json = "Vendor/json/single_include"
}
--"sdl" = "Vendor/sdl/include",
group("dependancies")
	include("Vendor/glad")
	include("Vendor/loguru")



group("")


project("Tara")
	location("Tara")
	kind("StaticLib")
	language("C++")
	cppdialect("C++17") --prefered C++19, but premake does not support that yet
	staticruntime("Off")
	
	targetdir("bin/"..outputdir.."/%{prj.name}")
	objdir("bin/int/"..outputdir.."/%{prj.name}")
	
	pchheader("tarapch.h")
	pchsource("%{prj.name}/src/tarapch.cpp")
	
	files({
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.cpp",
		--"Vendor/glad/src/glad.c",
		--"Vendor/loguru/src/loguru.cpp",
		
	})
	
	includedirs({
		"%{prj.name}/src",
		"%{IncludeDir.glad}",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.loguru}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.json}",
	})
	
	libdirs({
		"Vendor/glfw/lib-vc2019", --glfw
	})
	
	links({
		"glfw3",
		"glad",
		"loguru",
	})
	
	defines({
		"LOGURU_WITH_STREAMS=1"
	})
	
	filter("system:Windows")
		system("windows")
		systemversion("latest")
		
		--defines({})
		
		links({
			"opengl32.lib"
		})
		
	filter("")
	
	filter("configurations:Debug")
		runtime("Debug")
		defines("DEBUG")
		symbols("On")
		
	filter("configurations:Release")
		runtime("Release")
		defines("NDBUG")
		optimize("On")
	
	filter("")



project("Playground")
	location("Playground")
	kind("ConsoleApp")
	language("C++")
	cppdialect("C++17") --prefered C++19, but premake does not support that yet
	staticruntime("Off")
	
	targetdir("bin/"..outputdir.."/%{prj.name}")
	objdir("bin/int/"..outputdir.."/%{prj.name}")
	
	files({
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.cpp"
	})
	
	includedirs({
		"%{prj.name}/src",
		"Tara/src",
		"%{IncludeDir.glad}",
		"%{IncludeDir.loguru}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.json}",
	})
	
	libdirs({
		"Vendor/glfw/lib-vc2019", --glfw
	})
	
	links({
		"Tara",
		"glfw3",
		"glad",
		"loguru",
	})
	
	filter("system:Windows")
		system("windows")
		systemversion("latest")
		
		defines({})
		
		links({
			"opengl32.lib"
		})
		
	filter("")
	
	filter("configurations:Debug")
		runtime("Debug")
		defines("DEBUG")
		symbols("On")
		
	filter("configurations:Release")
		runtime("Release")
		defines("NDBUG")
		optimize("On")
	
	filter("")