#ifndef __HELLO_H__
#define __HELLO_H__

#include "base/base.h"

EXTERN_C_BEGIN

MG_DLL void PreInit_();
MG_DLL void PostInit_();

EXTERN_C_END

class MG_DLL Hello
{
public:
	void Print();
};

#endif