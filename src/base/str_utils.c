#include "str_utils.h"

char* StrUtils_AllocByDiff(const char* p_start, const char* p_end)
{
	char* buf = NULL;
	ptrdiff_t diff = p_end - p_start + 1;
	buf = (char*)malloc((size_t)diff + 1);
	memcpy(buf, p_start, (size_t)diff);
	buf[(size_t)diff] = '\0';

	return buf;
}