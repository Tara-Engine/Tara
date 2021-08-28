project("loguru")
	kind "StaticLib"
	language "C++"
	staticruntime "Off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	
	files({
		"include/loguru/loguru.hpp",
		"src/loguru.cpp",
	})
	
	includedirs({
		"include/loguru"
	})
	
	defines({
		"LOGURU_WITH_STREAMS=1"
	})
	
	filter ("configurations:Debug")
		runtime "Debug"
		symbols "on"

	filter ("configurations:Release")
		runtime "Release"
		optimize "on"
	

	filter("")

	filter("system:windows")
		systemversion("latest")
	
	filter("system:Linux")
		systemversion("latest")