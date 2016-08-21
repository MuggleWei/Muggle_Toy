#ifndef __MUGGLE_IMAGE_UTILS_RGBE_H__
#define __MUGGLE_IMAGE_UTILS_RGBE_H__

#include "base.h"

NS_MUGGLE_BEGIN

enum eRGBEFormat
{
	eRGBE_bit_rle_rgbe = 0,
	eRGBE_bit_rle_xyze,
};

enum eRGBEResolutionDir
{
	eRGBE_rd_nypx = (('-' << 24) | ('Y' << 16) | ('+' << 8) | 'X'),		// -Y N +X M
	eRGBE_rd_nynx = (('-' << 24) | ('Y' << 16) | ('-' << 8) | 'X'),		// -Y N -X M
	eRGBE_rd_pynx = (('+' << 24) | ('Y' << 16) | ('-' << 8) | 'X'),		// +Y N -X M
	eRGBE_rd_pypx = (('+' << 24) | ('Y' << 16) | ('+' << 8) | 'X'),		// +Y N +X M
	eRGBE_rd_pxpy = (('+' << 24) | ('X' << 16) | ('+' << 8) | 'Y'),		// +X M +Y N
	eRGBE_rd_nxpy = (('-' << 24) | ('X' << 16) | ('+' << 8) | 'Y'),		// -X M +Y N
	eRGBE_rd_nxny = (('-' << 24) | ('X' << 16) | ('-' << 8) | 'Y'),		// -X M -Y N
	eRGBE_rd_pxny = (('+' << 24) | ('X' << 16) | ('-' << 8) | 'Y'),		// +X M -Y N
};

typedef struct RGBE_Header_tag
{
	int format;			// correspond to eHDRFormat enum
	int resolution_dir;	// resulution direction, correspond to eRGBEResolutionDir enum
	int width;	
	int height;
	float exposure;		// a multiplier that has been applied to all the pixels in the file
	float gamma;		// color correction
}RGBE_Header;

bool ImageUtils_RGBE_ReadHeader(void* file_handle, RGBE_Header* p_header);
void* ImageUtils_RGBE_ReadData(void* file_handle, RGBE_Header* p_header);

void ImageUtils_Float_To_RGBE(unsigned char rgbe[4], float red, float green, float blue);
void ImageUtils_RGBE_To_Float(float* r, float* g, float* b, unsigned char rgbe[4]);

NS_MUGGLE_END

#endif