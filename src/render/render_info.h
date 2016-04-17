#ifndef __MUGGLE_RENDER_INFO_H__
#define __MUGGLE_RENDER_INFO_H__

#include "base/base.h"

NS_MUGGLE_BEGIN

#define FOREACH_ENUM_RENDER_TYPE(_) \
	_(OpenGL) \
	_(D3D11)
ENUM_STRUCT(RenderType, FOREACH_ENUM_RENDER_TYPE)

NS_MUGGLE_END

#endif