project("glad")
	kind "StaticLib"
	language "C"
	staticruntime "Off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	
	files({
		"inlcude/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c",
	})
	
	includedirs({
		"include"
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