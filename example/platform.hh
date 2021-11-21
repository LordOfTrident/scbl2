#ifndef __PLATFORM_HH_HEADER_GUARD__
#define __PLATFORM_HH_HEADER_GUARD__

#if defined(WIN32) or defined(_WIN32) or defined(__WIN32__) or defined(__NT__)
// Windows
#	define PLATFORM_WINDOWS
#	ifdef _WIN64
#		define PLATFORM_WINDOWS_64
#	else
#		define PLATFORM_WINDOWS_32
#	endif
#elif defined(__APPLE__)
// Apple
#	define PLATFORM_APPLE
#	include <TargetConditionals.h>
#	if defined(TARGET_IPHONE_SIMULATOR)
#		define PLATFORM_IOS_SIMULATOR
#	elif define(TARGET_OS_MACCATALYST)
#		define PLATFORM_MAC_CATALYST
#	elif defined(TARGET_OS_IPHONE)
#		define PLATFORM_IOS
#	elif TARGET_OS_MAC
#		define PLATFORM_MACOS
#	else
#		define PLATFORM_APPLE_UNKNOWN
#	endif
#elif defined(__ANDROID__)
// Android
#	define PLATFORM_ANDROID
#elif defined(__linux__) or defined(__gnu_linux__) or defined(linux)
// Linux
#	define PLATFORM_LINUX
#elif defined(__unix__) or defined(unix)
// Unix
#	define PLATFORM_UNIX
#else
// Unknown
#	define PLATFORM_UNKNOWN
#endif

#endif // __PLATFORM_HH_HEADER_GUARD__
