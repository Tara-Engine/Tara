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
	#error "Linux not currently Supported"
#endif

/*
Various other core utilities
that should be in every file
*/

//used to easily generate bitfields
#define BIT(x) (1 << x)

//used to bind event functions
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

