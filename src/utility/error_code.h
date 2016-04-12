#ifndef __MUGGLE_ERROR_CODE_H__
#define __MUGGLE_ERROR_CODE_H__

#include "base/base.h"

NS_MUGGLE_BEGIN

struct ErrorCode
{
	enum EnumCategory
	{
		Unknown = 0x00000000,
		Application = 0x00010000,
	};
	
	enum EnumApplicationError
	{
		App_New = 0x0,
		App_Init = 0x1,
		App_CustomMain = 0x2,
	};
};

NS_MUGGLE_END

#endif