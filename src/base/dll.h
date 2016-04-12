#ifndef __MUGGLE_DLL_H__
#define __MUGGLE_DLL_H__

#include "base/base.h"

EXTERN_C_BEGIN

MG_DLL void* DLL_Load(const char* name);
MG_DLL void* Dll_QueryFunc(void* dll, const char* func_name);
MG_DLL bool Dll_Free(void* dll);

EXTERN_C_END

#endif