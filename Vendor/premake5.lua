
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
		symbols "on"

	filter ("configurations:Release")
		runtime "Release"
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
	filter("configurations:Debug")
		runtime "Debug"
		symbols "on"

	filter("configurations:Release")
		runtime "Release"
		optimize "on"
		
	filter("")