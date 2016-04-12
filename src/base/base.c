#include "base.h"
#if MG_PLATFORM_WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#endif

void ExportFailure(const char* cond, const char* file_name, int line, const char* format, ...)
{
	va_list args;
	va_start(args, format);

	char buf[MG_MAX_PATH];
	vsnprintf(buf, MG_MAX_PATH, format, args);

	MLOG("Assertion failed: %s. %s, %d. %s\n", cond, file_name, line, buf);

#if MG_PLATFORM_WINDOWS
	__debugbreak();
#endif
	abort();

	va_end(args);
}

void SleepFunction(unsigned long ms)
{
#if MG_PLATFORM_WINDOWS
	Sleep(ms);
#else
	usleep((double)(ms) * 1000.0);
#endif
}

void LogFunction(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	
	char buf[MG_MAX_PATH];
	vsnprintf(buf, MG_MAX_PATH, format, args);

#if MG_PLATFORM_WINDOWS
	WCHAR w_buf[MG_MAX_PATH];
	MultiByteToWideChar(CP_UTF8, 0, buf, -1, w_buf, MG_MAX_PATH);
	OutputDebugStringW(w_buf);
	WideCharToMultiByte(CP_ACP, 0, w_buf, -1, buf, MG_MAX_PATH, NULL, FALSE);
	printf("%s", buf);
#else
	printf("%s", buf);
#endif

	va_end(args);
}