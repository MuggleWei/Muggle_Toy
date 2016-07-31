#include "image.h"
#include "str_utils.h"
#include "file_utility.h"
#include "image_utils_png.h"

NS_MUGGLE_BEGIN

Image* Image::Load(const char* file_name)
{
	if (StrUtils_EndsWith(file_name, "png") || StrUtils_EndsWith(file_name, "PNG"))
	{
		return LoadImagePng(file_name);
	}
	else if (StrUtils_EndsWith(file_name, "jpg") || StrUtils_EndsWith(file_name, "JPG"))
	{
		return nullptr;
	}

	return nullptr;
}

Image::Image()
{
	memset(m_file_path, 0, MG_MAX_PATH);
	m_data = nullptr;
}
Image::~Image()
{
	if (m_data)
	{
		free(m_data);
	}
}

const char* Image::getFilePath()
{
	return m_file_path;
}
uint32_t Image::getWidth()
{
	return m_width;
}
uint32_t Image::getHeight()
{
	return m_height;
}
ImageColorType::Enum Image::getColorType()
{
	return m_color_type;
}
ImageBitDepth::Enum Image::getBitDepth()
{
	return m_bit_depth;
}
uint32_t Image::getPixelSize()
{
	switch (m_bit_depth)
	{
	case ImageBitDepth::Enum::u8x3:
	{
		return 3;
	}break;
	case ImageBitDepth::Enum::u8x4:
	{
		return 4;
	}break;
	case ImageBitDepth::Enum::u16x3:
	{
		return 6;
	}break;
	case ImageBitDepth::Enum::u16x4:
	{
		return 8;
	}break;
	}

	MERROR(0, "Something must be wrong!");
	return 0;
}
unsigned char* Image::getData()
{
	return m_data;
}

void Image::CorrectTexCoord(bool is_opengl)
{
	if (is_opengl)
	{
		if (m_file_format == ImageFileFormat::Enum::PNG)
		{
			uint32_t rowbytes = m_width * getPixelSize();
			unsigned char* buf = (unsigned char*)malloc(rowbytes * m_height);
			for (int i = 0; i < m_height / 2; ++i)
			{
				memcpy(buf, m_data + i * rowbytes, rowbytes);
				memcpy(m_data + i * rowbytes, m_data + (m_height - i - 1) * rowbytes, rowbytes);
				memcpy(m_data + (m_height - i - 1) * rowbytes, buf, rowbytes);
			}
			free(buf);
		}
	}
}

Image* Image::LoadImagePng(const char* file_name)
{
	// get file handle
	char file_path_buffer[MG_MAX_PATH];
	File_GetAbsolutePath(file_name, file_path_buffer);
	void* file_handle = File_GetHandle(file_path_buffer, "rb");
	if (!file_handle)
	{
		MWARNING(0, "Failed in load image from file %s", file_name);
		return nullptr;
	}

	// png init
	png_structp png_ptr;
	png_infop info_ptr;
	int ret = ImageUtils_Png_Init(file_handle, &png_ptr, &info_ptr);
	if (ret != ePNG_OK)
	{
		File_CloseHandle(file_handle);
		return nullptr;
	}

	// get image information
	png_uint_32 width, height;
	int bit_depth, color_type;
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
		NULL, NULL, NULL);

	// check whether support this color type
	ImageColorType::Enum image_color_type;
	switch (color_type)
	{
	case PNG_COLOR_TYPE_PALETTE:
	{
		image_color_type = ImageColorType::Enum::Palette;
	}break;
	case PNG_COLOR_TYPE_RGB:
	{
		image_color_type = ImageColorType::Enum::RGB;
	}break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
	{
		image_color_type = ImageColorType::Enum::RGBA;
	}break;
	default:
	{
		MWARNING(0, "Don't support this png format in %s, to be continue... \n", file_name);
		ImageUtils_Png_Destroy(png_ptr, info_ptr);
		File_CloseHandle(file_handle);
		return nullptr;
	}
	}

	// check whether support this bit depth
	ImageBitDepth::Enum image_bit_depth;
	switch (bit_depth)
	{
	case 8:
	{
		if (image_color_type == ImageColorType::Enum::RGB)
		{
			image_bit_depth = ImageBitDepth::Enum::u8x3;
		}
		else if (image_color_type == ImageColorType::Enum::RGBA)
		{
			image_bit_depth = ImageBitDepth::Enum::u8x4;
		}
		else
		{
			MWARNING(0, "Don't support this png format in %s, to be continue... \n", file_name);
			ImageUtils_Png_Destroy(png_ptr, info_ptr);
			File_CloseHandle(file_handle);
			return nullptr;
		}
	}break;
	case 16:
	{
		if (image_color_type == ImageColorType::Enum::RGB)
		{
			image_bit_depth = ImageBitDepth::Enum::u16x3;
		}
		else if (image_color_type == ImageColorType::Enum::RGBA)
		{
			image_bit_depth = ImageBitDepth::Enum::u16x4;
		}
		else
		{
			MWARNING(0, "Don't support this png format in %s, to be continue... \n", file_name);
			ImageUtils_Png_Destroy(png_ptr, info_ptr);
			File_CloseHandle(file_handle);
			return nullptr;
		}
	}break;
	}

	// create new image and convert png information
	Image* image = new Image();
	memcpy(image->m_file_path, file_name, strlen(file_path_buffer));
	image->m_width = width;
	image->m_height = height;
	image->m_color_type = image_color_type;
	image->m_bit_depth = image_bit_depth;
	image->m_file_format = ImageFileFormat::Enum::PNG;

	if (image->m_color_type == ImageColorType::Enum::RGB ||
		image->m_color_type == ImageColorType::Enum::RGBA)
	{
		// allocate space
		png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
		image->m_data = (unsigned char*)malloc(rowbytes * height);
		png_bytepp row_pointers = (png_bytepp)malloc(height * sizeof(png_bytep));
		if (image->m_data == nullptr || row_pointers == nullptr)
		{
			MWARNING(0, "Memory is not enought to allocate for png: %s\n", file_name);
			ImageUtils_Png_Destroy(png_ptr, info_ptr);
			File_CloseHandle(file_handle);
			delete(image);
			if (row_pointers != nullptr)
			{
				free(row_pointers);
			}
			return nullptr;
		}

		// read the whole image
		for (int i = 0; i < height; ++i)
		{
			row_pointers[i] = image->m_data + i*rowbytes;
		}
		png_read_image(png_ptr, row_pointers);


		free(row_pointers);
		png_read_end(png_ptr, nullptr);
	}
	else
	{
		MASSERT_MSG(0, "to be continued...");
	}

	// png file destroy
	ImageUtils_Png_Destroy(png_ptr, info_ptr);

	// close file
	File_CloseHandle(file_handle);

	return image;
}

NS_MUGGLE_END