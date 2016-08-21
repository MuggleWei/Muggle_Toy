#include "image_utils_rgbe.h"
#include "str_utils.h"
#include "str_convert.h"

#define RGBE_BUF 512

NS_MUGGLE_BEGIN

bool RGBE_ReadPixels(FILE *fp, float *data, int numpixels)
{
	unsigned char rgbe[4];

	while (numpixels-- > 0)
	{
		if (fread(rgbe, sizeof(rgbe), 1, fp) < 1)
		{
			return false;
		}

		ImageUtils_RGBE_To_Float(&data[0], &data[1], &data[2], rgbe);
		data += 3;
	}
	return true;
}

void ImageUtils_RGBE_GetHeaderKeyValue(char* buf, char** key, char** value)
{
	char* p = buf;
	while (*p != '=' && *p != '\0')
	{
		++p;
	}

	if (*p == '\0')
	{
		MWARNING(0, "Failed in parse rgbe file header");
	}
	else
	{
		*p = '\0';
		*key = buf;
		++p;
		MG_SKIP_BLANK(p);
		*value = p;
		MG_SKIP_TO_LINE_END(p);
		*p = '\0';
	}
}

bool ImageUtils_RGBE_ReadHeader(void* file_handle, RGBE_Header* p_header)
{
	char buf[RGBE_BUF];

	// ensure file hand is valid
	FILE* fp = (FILE*)file_handle;
	if (fp == nullptr)
	{
		MWARNING(0, "Can't read a empty file handle");
		return false;
	}	

	// ensure RGBE header pointer
	if (p_header == nullptr)
	{
		MWARNING(0, "header info pointer is null");
		return false;
	}

	// set REGE header information default value
	p_header->format = eRGBE_bit_rle_rgbe;
	p_header->exposure = 1.0f;
	p_header->gamma = 1.0f;

	// check RADIANCE identifier
	if (fgets(buf, RGBE_BUF, fp) == nullptr)
	{
		MWARNING(0, "Failed parse rgbe file, can't read RADIANCE identifier");
		return false;
	}
	if (strncmp(buf, "#?RADIANCE", 10) != 0)
	{
		MWARNING(0, "Failed parse rgbe file, can't find RADIANCE identifier");
		return false;
	}

	// parse header info
	char *key, *value;
	while (true)
	{
		if (fgets(buf, RGBE_BUF, fp) == nullptr)
		{
			MWARNING(0, "Failed in parse rgbe file header");
			return false;
		}
		else
		{
			if (buf[0] == '\n')
			{
				break;
			}
			else if (buf[0] == '#')
			{
				// comment, do nothing
			}
			else
			{
				ImageUtils_RGBE_GetHeaderKeyValue(buf, &key, &value);
				if (strcmp(key, "FORMAT") == 0)
				{
					if (strcmp(value, "32-bit_rle_rgbe") == 0)
					{
						p_header->format = eRGBE_bit_rle_rgbe;
					}
					else if (strcmp(value, "32-bit_rle_xyze") == 0)
					{
						p_header->format = eRGBE_bit_rle_xyze;
					}
					else
					{
						MWARNING(0, "Unknown RGBE file format");
					}
				}
				else if (strcmp(key, "EXPOSURE") == 0)
				{
					float exposure_val = 1.0f;
					if (sscanf(value, "%f\n", &exposure_val) != EOF)
					{
						p_header->exposure = exposure_val;
					}
					else
					{
						MWARNING(0, "Invalid rgbe exposure value");
					}
				}
				else if ((strcmp(key, "GAMMA") == 0) || (strcmp(key, "COLORCORR") == 0))
				{
					float gamma = 1.0f;
					if (sscanf(value, "%f\n", &gamma) != EOF)
					{
						p_header->gamma = gamma;
					}
					else
					{
						MWARNING(0, "Invalid rgbe gamma value");
					}
				}
			}
		}
	}

	// parse Resolution String
	if (fgets(buf, RGBE_BUF, fp) == nullptr)
	{
		MWARNING(0, "Failed in parse rgbe resolution string");
		return false;
	}
	else
	{
		char* words[4];
		int word_num;
		StrUtils_SplitLineToWords(buf, words, &word_num, 4);
		if (word_num == 4 && 
			strlen(words[0]) == 2 && 
			strlen(words[2]) == 2 &&
			(words[0][0] == '+' || words[0][0] == '-') &&
			(words[2][0] == '+' || words[2][0] == '-') &&
			(words[0][1] == 'X' || words[0][1] == 'Y') &&
			(words[2][1] == 'X' || words[2][1] == 'Y')
			)
		{
			p_header->resolution_dir = (words[0][0] << 24) | (words[0][1] << 16) | (words[2][0] << 8) | words[2][1];
			if (!StrConvert_atoi(words[1], &p_header->height))
			{
				MWARNING(0, "Invalid rgbe resolution string");
				return false;
			}
			if (!StrConvert_atoi(words[3], &p_header->width))
			{
				MWARNING(0, "Invalid rgbe resolution string");
				return false;
			}			
		}
		else
		{
			MWARNING(0, "Invalid rgbe resolution string");
			return false;
		}
	}

	return true;
}
void* ImageUtils_RGBE_ReadData(void* file_handle, RGBE_Header* p_header)
{
	unsigned char rgbe[4], *scanline_buffer, *ptr, *ptr_end;
	int i, count;
	unsigned char buf[2];

	int scanline_width = p_header->width;
	int num_scanlines = p_header->height;

	FILE* fp = (FILE*)file_handle;

	float* pixels = (float*)malloc(sizeof(float) * 3 * scanline_width * num_scanlines);
	float* data = pixels;

	if ((scanline_width < 8) || (scanline_width > 0x7fff))
	{
		bool ret = RGBE_ReadPixels(fp, data, scanline_width * num_scanlines);
		if (ret)
		{
			return data;
		}
		else
		{
			goto fail;
		}
	}
		
	scanline_buffer = nullptr;
	while (num_scanlines > 0) 
	{
		if (fread(rgbe, sizeof(rgbe), 1, fp) < 1) 
		{
			free(scanline_buffer);
			goto fail;
		}
		if ((rgbe[0] != 2) || (rgbe[1] != 2) || (rgbe[2] & 0x80)) 
		{
			ImageUtils_RGBE_To_Float(&data[0], &data[1], &data[2], rgbe);
			data += 3;
			free(scanline_buffer);
			bool ret = RGBE_ReadPixels(fp, data, scanline_width*num_scanlines - 1);
			if (ret)
			{
				return data;
			}
			else
			{
				goto fail;
			}
		}
		if ((((int)rgbe[2]) << 8 | rgbe[3]) != scanline_width) 
		{
			free(scanline_buffer);
			goto fail;
		}
		if (scanline_buffer == nullptr)
		{
			scanline_buffer = (unsigned char *)malloc(sizeof(unsigned char) * 4 * scanline_width);
		}			
		if (scanline_buffer == nullptr)
		{
			goto fail;
		}		

		ptr = &scanline_buffer[0];
		for (i = 0; i<4; i++) 
		{
			ptr_end = &scanline_buffer[(i + 1)*scanline_width];
			while (ptr < ptr_end) 
			{
				if (fread(buf, sizeof(buf[0]) * 2, 1, fp) < 1) 
				{
					free(scanline_buffer);
					goto fail;
				}
				if (buf[0] > 128) 
				{
					count = buf[0] - 128;
					if ((count == 0) || (count > ptr_end - ptr)) 
					{
						free(scanline_buffer);
						goto fail;
					}
					while (count-- > 0)
					{
						*ptr++ = buf[1];
					}						
				}
				else 
				{
					count = buf[0];
					if ((count == 0) || (count > ptr_end - ptr)) 
					{
						free(scanline_buffer);
						goto fail;
					}
					*ptr++ = buf[1];
					if (--count > 0) 
					{
						if (fread(ptr, sizeof(*ptr)*count, 1, fp) < 1) 
						{
							free(scanline_buffer);
							goto fail;
						}
						ptr += count;
					}
				}
			}
		}
		for (i = 0; i<scanline_width; i++) 
		{
			rgbe[0] = scanline_buffer[i];
			rgbe[1] = scanline_buffer[i + scanline_width];
			rgbe[2] = scanline_buffer[i + 2 * scanline_width];
			rgbe[3] = scanline_buffer[i + 3 * scanline_width];
			ImageUtils_RGBE_To_Float(&data[0], &data[1], &data[2], rgbe);
			data += 3;
		}
		num_scanlines--;
	}
	free(scanline_buffer);

	return pixels;

fail:
	free(pixels);
	MWARNING(0, "Failed in parse hdr file");
	return nullptr;
}

void ImageUtils_Float_To_RGBE(unsigned char rgbe[4], float red, float green, float blue)
{
	float v;
	int e;

	v = red;
	if (green > v) v = green;
	if (blue > v) v = blue;
	if (v < 1e-32) 
	{
		rgbe[0] = rgbe[1] = rgbe[2] = rgbe[3] = 0;
	}
	else 
	{
		v = frexp(v, &e) * 256.0 / v;
		rgbe[0] = (unsigned char)(red * v);
		rgbe[1] = (unsigned char)(green * v);
		rgbe[2] = (unsigned char)(blue * v);
		rgbe[3] = (unsigned char)(e + 128);
	}
}
void ImageUtils_RGBE_To_Float(float* r, float* g, float* b, unsigned char rgbe[4])
{
	float f;

	if (rgbe[3]) 
	{
		f = ldexp(1.0, rgbe[3] - (int)(128 + 8));
		*r = rgbe[0] * f;
		*g = rgbe[1] * f;
		*b = rgbe[2] * f;
	}
	else
	{
		*r = *g = *b = 0.0;
	}
		
}

NS_MUGGLE_END