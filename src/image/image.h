#ifndef __MUGGLE_IMAGE_H__
#define __MUGGLE_IMAGE_H__

#include "base.h"

NS_MUGGLE_BEGIN

// color type enum struct
#define FOREACH_ENUM_IMAGE_COLOR_TYPE(_) \
	_(RGB) \
	_(RGBA) \
	_(Palette)
ENUM_STRUCT(ImageColorType, FOREACH_ENUM_IMAGE_COLOR_TYPE)

// bit depth enum struct
#define FOREACH_ENUM_IMAGE_BIT_DEPTH(_) \
	_(u8x3) \
	_(u8x4) \
	_(u16x3) \
	_(u16x4) \
	_(fx3)
ENUM_STRUCT(ImageBitDepth, FOREACH_ENUM_IMAGE_BIT_DEPTH)

//
#define FOREACH_ENUM_IMAGE_FILE_FORMAT(_) \
	_(PNG) \
	_(TGA) \
	_(DDS) \
	_(JPG) \
	_(RGBE)
ENUM_STRUCT(ImageFileFormat, FOREACH_ENUM_IMAGE_FILE_FORMAT)

class Image
{
public:
	MG_DLL static Image* Load(const char* file_name);

	MG_DLL Image();
	MG_DLL virtual ~Image();

	MG_DLL const char* getFilePath();
	MG_DLL uint32_t getWidth();
	MG_DLL uint32_t getHeight();
	MG_DLL ImageColorType::Enum getColorType();
	MG_DLL ImageBitDepth::Enum getBitDepth();
	MG_DLL uint32_t getPixelSize();
	MG_DLL unsigned char* getData();

	MG_DLL void CorrectTexCoord(bool is_opengl);

protected:
	static Image* LoadImagePng(const char* file_name);
	static Image* LoadImageHDR(const char* file_name);

protected:
	char m_file_path[MG_MAX_PATH];

	uint32_t m_width;
	uint32_t m_height;

	ImageColorType::Enum m_color_type;
	ImageBitDepth::Enum m_bit_depth;
	ImageFileFormat::Enum m_file_format;

	unsigned char* m_data;
};

NS_MUGGLE_END

#endif