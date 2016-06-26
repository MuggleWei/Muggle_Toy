#ifndef __MUGGLE_FILE_UTILITY_H__
#define __MUGGLE_FILE_UTILITY_H__

#include "base/base.h"

EXTERN_C_BEGIN

/*
 *  Get current process path
 *  @file_path: out path represent current process path, use MG_MAX_PATH char buffer
 */
MG_DLL void File_GetProcessPath(char* file_path);
MG_DLL bool File_IsExist(const char* file_path);
MG_DLL bool File_IsAbsolutePath(const char* file_path);

MG_DLL bool File_GetAbsolutePath(const char* in_file_name, char* out_file_path);
MG_DLL bool File_GetDirectory(const char* file_path, char* dir);
MG_DLL bool File_Read(const char* file_path, char** ptr_bytes, long* ptr_num);

MG_DLL void* File_GetHandle(const char* file_path, const char* mode);
MG_DLL void File_CloseHandle(void* file_handle);

EXTERN_C_END

#endif