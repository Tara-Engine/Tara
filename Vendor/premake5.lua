
project("loguru")
	location("loguru")
	kind("StaticLib")
	language("C++")
	staticruntime("Off")

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/int/" .. outputdir .. "/%{prj.name}")
	
	
	files({
		"loguru/loguru.hpp",
		"loguru/loguru.cpp",
	})
	
	includedirs({
		"loguru"
	})
	
	defines({
		"LOGURU_WITH_STREAMS=1"
	})
	
	--PLATFORM
	filter("system:windows")
		systemversion("latest")
	
	filter("system:Linux")
		systemversion("latest")
	
	filter("")
	
	
	--CONFIGURATION
	filter ("configurations:Debug")
		runtime "Debug"
		defines("DEBUG")
		symbols "on"

	filter ("configurations:Release")
		runtime "Release"
		defines("NDBUG")
		optimize "on"
	
	

	filter("")

	



project("glfw")
	location("glfw")
	kind("StaticLib")
	language("C")
	staticruntime("Off")
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/int/" .. outputdir .. "/%{prj.name}")
	
	files({
		"glfw/include/GLFW/glfw3.h",
		"glfw/include/GLFW/glfw3native.h",
		"glfw/src/glfw_config.h",
		"glfw/src/context.c",
		"glfw/src/init.c",
		"glfw/src/input.c",
		"glfw/src/monitor.c",
		"glfw/src/vulkan.c",
		"glfw/src/window.c"
	})
	
	
	--PLATFORM
	filter("system:windows")
		systemversion("latest")

		files({
			"glfw/src/win32_init.c",
			"glfw/src/win32_joystick.c",
			"glfw/src/win32_monitor.c",
			"glfw/src/win32_time.c",
			"glfw/src/win32_thread.c",
			"glfw/src/win32_window.c",
			"glfw/src/wgl_context.c",
			"glfw/src/egl_context.c",
			"glfw/src/osmesa_context.c"
		})

		defines({ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		})
		
	filter("system:linux")
		pic("On")
		systemversion("latest")
		
		files({
			"glfw/src/x11_init.c",
			"glfw/src/x11_monitor.c",
			"glfw/src/x11_window.c",
			"glfw/src/xkb_unicode.c",
			"glfw/src/posix_time.c",
			"glfw/src/posix_thread.c",
			"glfw/src/glx_context.c",
			"glfw/src/egl_context.c",
			"glfw/src/osmesa_context.c",
			"glfw/src/linux_joystick.c"
		})

		defines({
			"_GLFW_X11"
		})
		
	filter("")
	
	--CONFIGURATION
	filter ("configurations:Debug")
		runtime "Debug"
		defines("DEBUG")
		symbols "on"

	filter ("configurations:Release")
		runtime "Release"
		defines("NDBUG")
		optimize "on"
	
		
	filter("")


project("glad")
	location("glad")
	kind "StaticLib"
	language "C"
	staticruntime "Off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/int/" .. outputdir .. "/%{prj.name}")
	
	
	files({
		"glad/inlcude/glad/glad.h",
		"glad/include/KHR/khrplatform.h",
		"glad/src/glad.c",
	})
	
	includedirs({
		"glad/include"
	})
	
	filter ("configurations:Debug")
		runtime "Debug"
		defines("DEBUG")
		symbols "on"

	filter ("configurations:Release")
		runtime "Release"
		defines("NDBUG")
		optimize "on"
	
	

	filter("")

	filter("system:windows")
		systemversion("latest")
	
	filter("system:Linux")
		systemversion("latest")


project("lua")
	location("lua")
	kind("StaticLib")
	language("C")
	staticruntime("Off")
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/int/" .. outputdir .. "/%{prj.name}")
	
	
	files({
		"lua/lapi.c",
		"lua/lauxlib.c",
		"lua/lbaselib.c",
		"lua/lcode.c",
		"lua/lcorolib.c",
		"lua/lctype.c",
		"lua/ldblib.c",
		"lua/ldebug.c",
		"lua/ldo.c",
		"lua/ldump.c",
		"lua/lfunc.c",
		"lua/lgc.c",
		"lua/linit.c",
		"lua/liolib.c",
		"lua/llex.c",
		"lua/lmathlib.c",
		"lua/lmem.c",
		"lua/loadlib.c",
		"lua/lobject.c",
		"lua/lopcodes.c",
		"lua/loslib.c",
		"lua/lparser.c",
		"lua/lstate.c",
		"lua/lstring.c",
		"lua/lstrlib.c",
		"lua/ltable.c",
		"lua/ltablib.c",
		"lua/ltests.c",
		"lua/ltm.c",
		"lua/lundump.c",
		"lua/lutf8lib.c",
		"lua/lvm.c",
		"lua/lzio.c",
	})
	
	includedirs({
		"lua"
	})
	
	filter ("configurations:Debug")
		runtime "Debug"
		defines("DEBUG")
		symbols "on"

	filter ("configurations:Release")
		runtime "Release"
		defines("NDBUG")
		optimize "on"

	filter("")

	filter("system:windows")
		systemversion("latest")
	
	filter("system:Linux")
		systemversion("latest")