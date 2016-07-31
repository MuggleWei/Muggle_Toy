#ifndef __MUGGLE_TEXTURE_H__
#define __MUGGLE_TEXTURE_H__

#include "base.h"
#include "image/image.h"

NS_MUGGLE_BEGIN

#define FOREACH_ENUM_TEXTURE_TYPE(_) \
	_(Single) \
	_(Cube)
ENUM_STRUCT(TextureType, FOREACH_ENUM_TEXTURE_TYPE)

class Texture
{
public:
	enum eCubeImage
	{
		CI_PositiveX = 0,
		CI_NegativeX,
		CI_PositiveY,
		CI_NegativeY,
		CI_PositiveZ,
		CI_NegativeZ,
		CI_Max,
	};

public:
	MG_DLL static Texture* Load(const char* file_name, TextureType::Enum tex_type = TextureType::Enum::Single);
	MG_DLL static Texture* LoadCube(const char* file_name[eCubeImage::CI_Max]);

	MG_DLL Texture();
	MG_DLL virtual ~Texture();

	MG_DLL uint32_t getHandle();
	MG_DLL TextureType::Enum getType();

protected:
	void GenGPUInfo();

protected:
	union
	{
		uint32_t gl_texture_id;
	};

	muggle::Image* m_image;
	muggle::Image* m_cube_image[CI_Max];
	TextureType::Enum m_type;
};

NS_MUGGLE_END

#endif