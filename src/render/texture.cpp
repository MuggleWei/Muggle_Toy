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

Texture* Texture::Load(const char* file_name, TextureType::Enum tex_type)
{
	Image* image = Image::Load(file_name);
	if (image == nullptr)
	{
		MWARNING(0, "Can't load image: %s\n", file_name);
		return nullptr;
	}

	Texture* tex = new Texture();
	tex->m_image = image;
	tex->m_type = tex_type;
	tex->GenGPUInfo();

	return tex;
}
Texture* Texture::LoadCube(const char* file_name[eCubeImage::CI_Max])
{
	Texture* tex = new Texture();
	for (int i = 0; i < eCubeImage::CI_Max; ++i)
	{
		tex->m_cube_image[i] = Image::Load(file_name[i]);
	}
	tex->m_type = TextureType::Enum::Cube;
	tex->GenGPUInfo();

	return tex;
}

Texture::Texture()
{
	gl_texture_id = GL_INVALID_VALUE;
	m_image = nullptr;
	for (int i = 0; i < CI_Max; ++i)
	{
		m_cube_image[i] = nullptr;
	}
}
Texture::~Texture()
{
	if (gl_texture_id != GL_INVALID_VALUE)
	{
		glDeleteTextures(1, &gl_texture_id);
	}

	if (m_image != nullptr)
	{
		delete(m_image);
	}
	for (int i = 0; i < eCubeImage::CI_Max; ++i)
	{
		if (m_cube_image[i] != nullptr)
		{
			delete(m_cube_image[i]);
		}
	}
}

uint32_t Texture::getHandle()
{
	return gl_texture_id;
}
TextureType::Enum Texture::getType()
{
	return m_type;
}

void Texture::GenGPUInfo()
{
	if (m_image == nullptr && 
		(m_cube_image[CI_PositiveX] == nullptr || m_cube_image[CI_NegativeX] == nullptr ||
		m_cube_image[CI_PositiveY] == nullptr || m_cube_image[CI_NegativeY] == nullptr ||
		m_cube_image[CI_PositiveZ] == nullptr || m_cube_image[CI_NegativeZ] == nullptr
		))
	{
		MWARNING(0, "Can't generate texture info for gpu, cause image is empyt");
		return;
	}

	if (gl_texture_id != GL_INVALID_VALUE)
	{
		glDeleteTextures(1, &gl_texture_id);
		gl_texture_id = GL_INVALID_VALUE;
	}

	if (m_type == TextureType::Single)
	{
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
	else
	{
		if (m_image != nullptr)
		{
			MASSERT_MSG(0, "To be continued...");
		}
		else
		{
			// Create texture and bind
			GLuint textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
			gl_texture_id = textureID;

			GLuint targets[] = {
				GL_TEXTURE_CUBE_MAP_POSITIVE_X,
				GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
				GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
				GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
				GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
				GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
			};
			for (int i = 0; i < eCubeImage::CI_Max; ++i)
			{
				// m_cube_image[i]->CorrectTexCoord(true);
				GLint texture_format = ColorTypeToGLInternalFormat(m_cube_image[i]->getColorType());
				glTexImage2D(targets[i], 0, texture_format,
					m_cube_image[i]->getWidth(), m_cube_image[i]->getHeight(),
					0, texture_format, GL_UNSIGNED_BYTE, m_cube_image[i]->getData());
			}

			// TODO: this need to be config in texture member variable
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			// unbind
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
	}
}

NS_MUGGLE_END