#include "texture.h"
#include "glad/glad.h"
#include "image/image.h"

NS_MUGGLE_BEGIN

int32_t ColorTypeToGLInternalFormat(ImageColorType::Enum color_type)
{
	int32_t gl_internal_format = 0;

	switch (color_type)
	{
	case ImageColorType::Enum::RGB:
	{
		gl_internal_format = GL_RGB;
	}break;
	case ImageColorType::Enum::RGBA:
	{
		gl_internal_format = GL_RGBA;
	}break;
	default:
	{
		MERROR(0, "to be continued...");
	}break;
	}

	return gl_internal_format;
}

Texture* Texture::Load(const char* file_name)
{
	Image* image = Image::Load(file_name);
	if (image == nullptr)
	{
		MWARNING(0, "Can't load image: %s\n", file_name);
		return nullptr;
	}

	Texture* tex = new Texture();
	tex->m_image = image;
	tex->GenGPUInfo();

	return tex;
}

Texture::Texture()
{
	gl_texture_id = GL_INVALID_VALUE;
	m_image = nullptr;
}
Texture::~Texture()
{
	if (gl_texture_id != 0)
	{
		glDeleteTextures(1, &gl_texture_id);
	}

	if (m_image != nullptr)
	{
		delete(m_image);
	}
}

uint32_t Texture::getHandle()
{
	return gl_texture_id;
}

void Texture::GenGPUInfo()
{
	if (gl_texture_id != GL_INVALID_VALUE)
	{
		glDeleteTextures(1, &gl_texture_id);
		gl_texture_id = GL_INVALID_VALUE;
	}

	if (m_image == nullptr)
	{
		MWARNING(0, "Can't generate texture info for gpu, cause image is empyt");
		return;
	}

	// correct texture coordinate
	m_image->CorrectTexCoord(true);

	// Create texture and bind
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	gl_texture_id = textureID;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	GLint texture_format = ColorTypeToGLInternalFormat(m_image->getColorType());

	glTexImage2D(GL_TEXTURE_2D, 0, texture_format,
		m_image->getWidth(), m_image->getHeight(), 
		0, texture_format, GL_UNSIGNED_BYTE, m_image->getData());

	// TODO: this need to be config in texture member variable
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// unbind
	glBindTexture(GL_TEXTURE_2D, 0);
}

NS_MUGGLE_END