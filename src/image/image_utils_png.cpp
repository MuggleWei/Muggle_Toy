#include "image_utils_png.h"
#include <stdio.h>

NS_MUGGLE_BEGIN

int ImageUtils_Png_Init(void* file_handle, png_structp* p_png_ptr, png_infop* p_info_ptr)
{
	FILE* fp = (FILE*)file_handle;
	MERROR(fp, "Can't read a empty file handle");

	// check signature
	uint8_t sig[8];
	fread(sig, 1, 8, fp);
	if (!png_check_sig(sig, 8))
	{
		return ePNG_BadSignature;
	}

	// create png structure
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		return ePNG_OutMemory;
	}

	// create info structure
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) 
	{
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return ePNG_OutMemory;
	}

	/* setjmp() must be called in every function that calls a PNG-reading
	* libpng function */
	if (setjmp(png_jmpbuf(png_ptr))) 
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return ePNG_ReadFailed;
	}

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);  /* we already read the 8 signature bytes */

	png_read_info(png_ptr, info_ptr);  /* read all PNG info up to image data */

	*p_png_ptr = png_ptr;
	*p_info_ptr = info_ptr;

	return ePNG_OK;
}
void ImageUtils_Png_Destroy(png_structp png_ptr, png_infop info_ptr)
{
	MERROR(png_ptr && info_ptr, "Can't destroy empty pointer");
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

NS_MUGGLE_END