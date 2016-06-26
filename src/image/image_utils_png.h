#ifndef __MUGGLE_IMAGE_UTILS_PNG_H__
#define __MUGGLE_IMAGE_UTILS_PNG_H__

#include "base.h"
#include "lpng/png.h"
#include "zlib/zlib.h"

NS_MUGGLE_BEGIN

enum ePNGStatus
{
	ePNG_OK = 0,
	ePNG_BadSignature = 1,
	ePNG_ReadFailed = 2,
	ePNG_OutMemory = 4,
};

int ImageUtils_Png_Init(void* file_handle, png_structp* p_png_ptr, png_infop* p_info_ptr);
void ImageUtils_Png_Destroy(png_structp png_ptr, png_infop info_ptr);

NS_MUGGLE_END

#endif