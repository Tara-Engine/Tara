--[[
This lua code file is used for premake5, a build tool, similar to cmake.
[https://premake.github.io/]
Download premake, and run the command:
/path/to/premake/executable/premake5 vs2019
to generate visual studios 2019 project files.
If another version of visual studios is desired, use the appropreate options in premake.



When using Tara as a submodule (as you should do),
make sure to defune Tara_Included to some non-nil value before including it.
You can also define outputdir to be some path for the binaries, but do not need to.

Tara_IncludeDir is defined with key/value pairs to all Tara's dependancies, RELATIVE TO THIS FOLDER
Tara_Links is defined as a list of all the dependency projects for a project using Tara.
--]]


local make_workspace = true
if (Tara_Included ~= nil) then
	make_workspace = false
end

if (make_workspace) then
workspace("Tara")
	architecture("x64")
	configurations{"Debug", "Release"}
	startproject("Playground")
end	

if (outputdir == nil) then
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
end

--standard includes
Tara_IncludeDir = {
	glad = "Vendor/glad/include",
	glfw = "Vendor/glfw/include",
	loguru = "Vendor/loguru",
	glm = "Vendor/glm",
	stb = "Vendor/stb",
	json = "Vendor/json/single_include",
	lua = "Vendor/lua",
	sol = "Vendor/sol2/include",
}

--standard links for anything using Tara.
Tara_Links = {
	"Tara",
	"glfw",
	"glad",
	"loguru",
	"lua",
}

--include Tara's dependancies
group("dependancies")
	include("Vendor")
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
	})
	
	includedirs({
		"%{prj.name}/src",
		"%{Tara_IncludeDir.glad}",
		"%{Tara_IncludeDir.glfw}",
		"%{Tara_IncludeDir.loguru}",
		"%{Tara_IncludeDir.glm}",
		"%{Tara_IncludeDir.stb}",
		"%{Tara_IncludeDir.json}",
		"%{Tara_IncludeDir.lua}",
		"%{Tara_IncludeDir.sol}",
	})
	
	
	links({
		"glfw",
		"glad",
		"loguru",
		"lua",
	})
	
	defines({
		"LOGURU_WITH_STREAMS=1",
		"SOL_NO_EXCEPTIONS=1",
	})
	
	filter("system:Windows")
		system("windows")
		systemversion("latest")
		
		--defines({})
		
		links({
			"opengl32.lib"
		})
		
	filter("system:linux")
		system("linux")
		systemversion("latest")
		toolset("clang")
		links({
			"GL",
			"X11", 
			"dl", 
			"pthread", 
			"m", 
			"z",
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


if (make_workspace) then

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
		"%{Tara_IncludeDir.glad}",
		"%{Tara_IncludeDir.loguru}",
		"%{Tara_IncludeDir.glm}",
		"%{Tara_IncludeDir.stb}",
		"%{Tara_IncludeDir.json}",
		"%{Tara_IncludeDir.lua}",
		"%{Tara_IncludeDir.sol}",
	})
	
	
	links(Tara_Links)
	
	filter("system:Windows")
		system("windows")
		systemversion("latest")
		
		defines({})
		
		links({
			"opengl32.lib"
		})
	
	filter("system:linux")
		system("linux")
		systemversion("latest")
		toolset("clang")
		links({
			"GL",
			"X11", "dl", "pthread", "m", "z", --"Xtest", "Xfixes"
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

end