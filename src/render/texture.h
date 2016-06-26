#ifndef __MUGGLE_TEXTURE_H__
#define __MUGGLE_TEXTURE_H__

#include "base.h"
#include "image/image.h"

NS_MUGGLE_BEGIN

class Texture
{
public:
	MG_DLL static Texture* Load(const char* file_name);

	MG_DLL Texture();
	MG_DLL virtual ~Texture();

	MG_DLL uint32_t getHandle();

protected:
	void GenGPUInfo();

protected:
	union
	{
		uint32_t gl_texture_id;
	};

	muggle::Image* m_image;
};

NS_MUGGLE_END

#endif