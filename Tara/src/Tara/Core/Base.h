#pragma once

/*
some future - proofing for the engine,
if I decide to support multiple platforms
*/

#ifdef _WIN32
	#ifdef _WIN64
		#define TARA_PLATFORM_WINDOWS
	#else
		#error "x86 builds not supported!"
	#endif
#elif defined(__APPLE__) || defined (__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS sumulator not supported"
	#elif TARGET_OS_IPHONE
		#define TARA_PLATFORM_IOS
		#error "IOS not currently Supported"
	#elif TARGET_OS_MAC
		#define TARA_PLATFORM_MAC
		#error "MacOS not currently Supported"
	#else
		#error "Unknown Apple Platform"
	#endif
#elif defined(__ANDROID__)
	#define TARA_PLATFORM_ANDROID
	#error "Android not currently Supported"
#elif defined(__linux__)
	#define TARA_PLATFORM_LINUX
	#pragma message ("Warning! Linux builds untested. If a segfault occurs on execution, your OpenGL version may not be high enough.")
#endif

/*
Various other core utilities
that should be in every file
*/

//used to easily generate bitfields
#define BIT(x) (1 << x)

//used to bind event functions
#define TARA_BIND_FN(x) std::bind(&x, this, std::placeholders::_1)

//defining reftypes
#define REFTYPE(name) class name; \
	using name##Ref = std::shared_ptr<##name>;

#define NOREFTYPE(name) using name##NoRef = std::weak_ptr<##name>;