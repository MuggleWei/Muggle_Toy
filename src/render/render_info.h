#ifndef __MUGGLE_RENDER_INFO_H__
#define __MUGGLE_RENDER_INFO_H__

#include "base/base.h"

NS_MUGGLE_BEGIN

#define FOREACH_ENUM_RENDER_TYPE(_) \
	_(Vulkan) \
	_(OpenGL) \
	_(D3D11)
ENUM_STRUCT(RenderType, FOREACH_ENUM_RENDER_TYPE)

#define FOREACH_ENUM_IMAGE_FORMAT(_) \
	_(IMAGE_FMT_UNKNOWN) \
	_(IMAGE_FMT_RGBA8_UNORM) \
	_(IMAGE_FMT_RGBA16_F)
ENUM_STRUCT(ImageFormat, FOREACH_ENUM_IMAGE_FORMAT)

struct RenderInitParameter
{
	void* hWnd;
	bool full_screen;
	bool vsync;
	int win_width;
	int win_height;
	ImageFormat::Enum rt_format;
};

enum eHardwareInfo
{
	MAX_VideoCardDescLen = 128,
	MAX_MonitorNameLen = 512,
	MAX_MonitorPreVideoCard = 8,
};

struct MonitorInfo
{
	int refresh_rate_numerator;
	int refresh_rate_denominator;
	char name[MAX_MonitorNameLen];
	int width;
	int height;
};

struct VideoCardInfo
{
	int memory_size;
	char desc[MAX_VideoCardDescLen];
	MonitorInfo monitors[MAX_MonitorPreVideoCard];
	int monitor_num;
};

NS_MUGGLE_END

#endif