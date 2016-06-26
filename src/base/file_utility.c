#include "file_utility.h"

#if MG_PLATFORM_WINDOWS

#include <windows.h>

void File_GetProcessPath(char* file_path)
{
	// convert to unicode characters
	WCHAR unicode_buf[MG_MAX_PATH] = { 0 };
	GetModuleFileNameW(NULL, unicode_buf, MG_MAX_PATH);

	// convert to utf8
	WideCharToMultiByte(CP_UTF8, 0, unicode_buf, -1, file_path, MG_MAX_PATH, NULL, FALSE);
}
bool File_IsExist(const char* file_path)
{
	// convert to utf16 characters
	WCHAR unicode_buf[MG_MAX_PATH] = { 0 };
	MultiByteToWideChar(CP_UTF8, 0, file_path, -1, unicode_buf, MG_MAX_PATH);

	// get file attributes
	DWORD attr = GetFileAttributesW(unicode_buf);
	if (attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY))
		return false;
	return true;
}
bool File_IsAbsolutePath(const char* file_path)
{
	size_t len = strlen(file_path);
	if (len > 2 &&
		((file_path[0] >= 'a' && file_path[0] <= 'z') || (file_path[0] >= 'A' && file_path[0] <= 'Z')) &&
		file_path[1] == ':' &&
		(file_path[2] == '/' || file_path[2] == '\\'))
	{
		return true;
	}

	return false;
}

#else

#include <sys/types.h>
#include <unistd.h>

void File_GetProcessPath(char* file_path)
{
	char sz_tmp[32], buf[MG_MAX_PATH];
	ssize_t len;

	sprintf_s(sz_tmp, 31, "/proc/%d/exe", getpid());
	len = readlink(sz_tmp, file_path, MG_MAX_PATH);
	MASSERT_MSG(len >= 0, "something wrong in readlink function");
	if (len >= 0)
		file_path[len] = '\0';
}
bool File_IsExist(const char* file_path)
{
	if (access(file_path, F_OK) != -1)
		return true;

	return false;
}
bool File_IsAbsolutePath(const char* file_path)
{
	size_t len = strlen(file_path);
	if (len > 1 && file_path[0] == '/')
	{
		return true;
	}

	return false;
}

#endif

bool File_GetAbsolutePath(const char* in_file_name, char* out_file_path)
{
	if (File_IsAbsolutePath(in_file_name))
	{
		return true;
	}

	char module_path[MG_MAX_PATH];
	File_GetProcessPath(module_path);

	File_GetDirectory(module_path, out_file_path);
	size_t len = strlen(out_file_path);

	memcpy(&out_file_path[len], in_file_name, strlen(in_file_name) + 1);

	return true;
}
bool File_GetDirectory(const char* file_path, char* dir)
{
	size_t len = strlen(file_path);
	while (len > 0)
	{
		if (file_path[len - 1] == '/' || file_path[len - 1] == '\\')
		{
			break;
		}
		--len;
	}

	if (len < 0)
	{
		return false;
	}

	memmove(dir, file_path, len);
	dir[len] = '\0';

	return true;
}
bool File_Read(const char* file_path, char** ptr_bytes, long* ptr_num)
{
	long file_size;
	size_t read_size;

	// note : why use "rb"
	// if use "r", the return value of ftell and fread maybe not equal
	FILE* fp = fopen(file_path, "rb");
	if (fp == NULL)
	{
		return false;
	}

	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if (file_size == 0)
	{
		*ptr_bytes = NULL;
		*ptr_num = 0;
		return true;
	}

	*ptr_bytes = (char*)malloc(file_size + 1);
	read_size = fread(*ptr_bytes, 1, (size_t)file_size, fp);
	(*ptr_bytes)[file_size] = '\0';
	fclose(fp);

	MASSERT(read_size == file_size);
	*ptr_num = (long)read_size;

	return true;
}

void* File_GetHandle(const char* file_path, const char* mode)
{
	return (void*)fopen(file_path, mode);
}
void File_CloseHandle(void* file_handle)
{
	MASSERT_MSG(file_handle, "Try to close empty file handle");
	fclose(file_handle);
}