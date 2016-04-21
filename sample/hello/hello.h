#ifndef __HELLO_H__
#define __HELLO_H__

#include "base/base.h"

EXTERN_C_BEGIN

MG_DLL void Init();
MG_DLL void Update();
MG_DLL void Render();

EXTERN_C_END

class MG_DLL Hello
{
public:
	void Init();
	void Update();
	void Render();
};

#endif