#ifndef __MUGGLE_STR_UTILS_H__
#define __MUGGLE_STR_UTILS_H__

#include "base/base.h"

#define MG_SKIP_BLANK(p) \
while (*p == ' ') \
{ \
	++p; \
}

EXTERN_C_BEGIN

// note: the range is [p_start, p_end], so generate char* is "p_start ... p_end '\0'"
MG_DLL char* StrUtils_AllocByDiff(const char* p_start, const char* p_end);

EXTERN_C_END

#endif