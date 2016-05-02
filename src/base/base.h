#ifndef __MUGGLE_BASE_H__
#define __MUGGLE_BASE_H__

// base c header
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <time.h>

// debug and release macro
#if ! defined(NDEBUG)
#define MUGGLE_DEBUG 1
#else
#define MUGGLE_RELEASE 1
#endif

// detect memory leak
#if MG_PLATFORM_WINDOWS && MUGGLE_DEBUG
#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>
#endif

// noexcept
#if MG_PLATFORM_WINDOWS
#if _MSC_VER < 1900
#define _XKEYCHECK_H
#define noexcept
#endif
#endif

// assert
#if MUGGLE_RELEASE
#define MASSERT(x)
#define MASSERT_MSG(x, format, ...)
#define MERROR(x, format, ...)
#define MWARNING(x, format, ...)
#else
#define MASSERT(x) \
do \
{ \
	if (!(x)) \
	{ \
		ExportFailure(#x, __FILE__, __LINE__, "\0");  \
	} \
} while(0)
#define MASSERT_MSG(x, format, ...) \
do \
{ \
	if (!(x)) \
	{ \
		ExportFailure(#x, __FILE__, __LINE__, format, ##__VA_ARGS__);  \
	} \
} while(0)
#define MERROR(x, format, ...) MASSERT_MSG(x, format, ##__VA_ARGS__)
#define MWARNING(x, format, ...) \
do \
{ \
	if (!(x)) \
	{ \
		ExportWarning(#x, __FILE__, __LINE__, format, ##__VA_ARGS__);  \
	} \
} while(0)
#endif

// lib and dll
#if MG_PLATFORM_WINDOWS
#define MG_DLL_EXPORT __declspec(dllexport)
#define MG_DLL_IMPORT __declspec(dllimport)
#if defined(MG_USE_LIB)
#define MG_DLL
#elif defined(MG_USE_DLL)
#define MG_DLL MG_DLL_EXPORT
#else
#define MG_DLL MG_DLL_IMPORT
#endif
#else
#define MG_DLL
#endif

// deprecated macro
// #define MUGGLE_DEPRECATED [[deprecated]] // wait for c++14 support
#if MG_PLATFORM_WINDOWS
	#define MUGGLE_DEPRECATED __declspec(deprecated)
#elif MG_PLATFORM_UNIX || MG_PLATFORM_OSX
	#define MUGGLE_DEPRECATED __attribute__((deprecated))
#else
	#define MUGGLE_DEPRECATED
#endif

// namespace
#ifdef __cplusplus
#define NS_MUGGLE_BEGIN namespace muggle {
#define NS_MUGGLE_END   }
#define USING_NS_MUGGLE using namespace muggle
#else
#define NS_MUGGLE_BEGIN
#define NS_MUGGLE_END  
#define USING_NS_MUGGLE
#endif 

// extern c
#ifdef __cplusplus
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END   }
#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif

// log
#define MLOG(format, ...) LogFunction(format, ##__VA_ARGS__)

// sleep
#define MSleep(ms) SleepFunction(ms)

// safe delete
#define SAFE_DELETE(x) \
do \
{ \
	if (x) \
	{ \
		delete x; \
		x = nullptr; \
	} \
} while (0)

// max length of path
#if MG_PLATFORM_WINDOWS
#define MG_MAX_PATH 260
#else
#define MG_MAX_PATH 1024
#endif

// sprintf_s
#if MG_PLATFORM_WINDOWS
#else
#define sprintf_s(buf, size_in_byte, format, ...) sprintf(buf, format, ##__VA_ARGS__)
#endif

// enum and enum string
#define GENERATE_ENUM(e) e,
#define GENERATE_ENUM_STRING(e) #e,
#define CASE_ENUM_TO_STRING(e) case e: return #e;

#define ENUM_MACRO_DEFINE(enum_name, FOREACH_ENUM) \
enum enum_name \
{ \
	FOREACH_ENUM(GENERATE_ENUM) \
	enum_name##_MAX, \
}; \
extern const char* enum_name##String[enum_name##_MAX];

#define ENUM_MACRO_STRING(enum_name, FOREACH_ENUM) \
const char* enum_name##String[enum_name##_MAX] = { \
	FOREACH_ENUM(GENERATE_ENUM_STRING) \
};

// enum and enum string struct
#ifdef __cplusplus
#define ENUM_STRUCT(struct_name, FOREACH_ENUM) \
struct struct_name \
{ \
	enum Enum \
	{ \
		FOREACH_ENUM(GENERATE_ENUM) \
		Max, \
	}; \
	static const char* EnumToString(Enum index) \
	{ \
		switch (index) \
		{ \
			FOREACH_ENUM(CASE_ENUM_TO_STRING); \
		} \
		MASSERT_MSG(0, "Enumerator index beyond the range"); \
		return ""; \
	} \
	static Enum StringToEnum(const char* str) \
	{ \
		int enum_index; \
		for (enum_index = 0; enum_index < Enum::Max; ++enum_index) \
		{ \
			if (strcmp(str, EnumToString((Enum)enum_index)) == 0) \
			{ \
				return (Enum)enum_index; \
			} \
		} \
		MASSERT_MSG(0, "This string not exist in this enumerator"); \
		return Enum::Max; \
	} \
};
#endif

EXTERN_C_BEGIN

// function
MG_DLL void ExportWarning(const char* cond, const char* file_name, int line, const char* format, ...);
MG_DLL void ExportFailure(const char* cond, const char* file_name, int line, const char* format, ...);
MG_DLL void SleepFunction(unsigned long ms);
MG_DLL void LogFunction(const char *format, ...);

EXTERN_C_END

#endif
