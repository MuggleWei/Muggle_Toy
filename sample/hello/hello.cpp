#include "hello.h"
#include "utility/string_utils.h"

void PreInit_()
{
	MLOG("Hello dll PreInit");
}
void PostInit_()
{
	MLOG("Hello dll PostInit");
}

void Hello::Print()
{
	MLOG("Hello Print");
}