#include "shader_program.h"
#include "shader_obj.h"
#include "glad/glad.h"

NS_MUGGLE_BEGIN

ShaderProgram::ShaderProgram()
{
	gl_handle = 0;
}
ShaderProgram::~ShaderProgram()
{
	MASSERT_MSG(gl_handle == 0,
		"Must use Destroy() function to release shader program resource before destructor");
}

bool ShaderProgram::Initialize()
{
	gl_handle = glCreateProgram();
	if (gl_handle == 0)
	{
		MASSERT_MSG(0, "Failed creating program object");
		return false;
	}

	return true;
}
void ShaderProgram::Destroy()
{
	if (gl_handle != 0)
	{
		glDeleteProgram(gl_handle);
		gl_handle = 0;
	}
}

void ShaderProgram::Attach(const ShaderObj* shader_obj)
{
	MASSERT(gl_handle != 0);
	glAttachShader(gl_handle, shader_obj->gl_shader_handle);
}
bool ShaderProgram::Link()
{
	glLinkProgram(gl_handle);

	// verify the link status
	GLint status;
	glGetProgramiv(gl_handle, GL_LINK_STATUS, &status);
	if (GL_FALSE == status)
	{
		MLOG("Failed to link shader program!\n");
		GLint log_len;
		glGetProgramiv(gl_handle, GL_INFO_LOG_LENGTH, &log_len);
		if (log_len > 0)
		{
			char *log = (char*)malloc(log_len);
			GLsizei written;
			glGetProgramInfoLog(gl_handle, log_len, &written, log);
			MLOG("Program log: \n%s", log);
			free(log);
		}
		MASSERT_MSG(0, "gl shader link error");
		return false;
	}

#if MUGGLE_DEBUG
	OutputInformation();
#endif

	return true;
}

bool ShaderProgram::OutputInformation()
{
	/*
	if (gl_handle == 0)
	{
		return false;
	}

	GLint n_attribs, max_len_attris = 0;
	GLint n_uniforms, max_len_uniforms = 0;
	GLint n_uniform_blocks, max_len_uniform_blocks = 0;
	glGetProgramiv(gl_handle, GL_ACTIVE_ATTRIBUTES, &n_attribs);
	glGetProgramiv(gl_handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_len_attris);
	glGetProgramiv(gl_handle, GL_ACTIVE_UNIFORMS, &n_uniforms);
	glGetProgramiv(gl_handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_len_uniforms);
	glGetProgramiv(gl_handle, GL_ACTIVE_UNIFORM_BLOCKS, &n_uniform_blocks);
	glGetProgramiv(gl_handle, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &max_len_uniform_blocks);

	GLsizei max_length = max_len_attris > max_len_uniforms ? max_len_attris : max_len_uniforms;
	max_length = max_length > max_len_uniform_blocks ? max_length : max_len_uniform_blocks;
	GLchar* name = (GLchar*)malloc(max_length+1);

	GLint size, location;
	GLsizei length;
	GLenum type;
	GLint active_uniform_in_block;
	MLOG("=============================\n");
	MLOG("Shader Program info:\n");
	for (GLint i = 0; i < n_attribs; ++i)
	{
		glGetActiveAttrib(
			gl_handle, (GLuint)i, 
			max_length, &length,
			&size, &type, name);
		location = glGetAttribLocation(gl_handle, name);
		MLOG("Attribute: [location %d] %s %s;\n", location, ConvertGLTypeToString(type), name);
	}
	for (GLint i = 0; i < n_uniforms; ++i)
	{
		glGetActiveUniform(
			gl_handle, (GLuint)i,
			max_length, &length,
			&size, &type, name);
		location = glGetUniformLocation(gl_handle, name);
		if (location == -1)
		{
			continue;
		}
		MLOG("Uniforms: [location %d] %s %s;\n", location, ConvertGLTypeToString(type), name);
	}
	for (GLint i = 0; i < n_uniform_blocks; ++i)
	{
		glGetActiveUniformBlockName(
			gl_handle, (GLuint)i,
			max_length, &length,
			name);
		glGetActiveUniformBlockiv(gl_handle, (GLuint)i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &active_uniform_in_block);
		MLOG("Uniform block: [index %d] %s\n", i, name);
		MLOG("{\n");
		if (active_uniform_in_block > 0)
		{
			GLint* indices = (GLint*)malloc(active_uniform_in_block * sizeof(GLint));
			glGetActiveUniformBlockiv(gl_handle, (GLuint)i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, indices);
			for (GLint j = 0; j < active_uniform_in_block; ++j)
			{
				glGetActiveUniform(
					gl_handle, (GLuint)j,
					max_length, &length,
					&size, &type, name);
				MLOG("\t%s %s;\n", ConvertGLTypeToString(type), name);
			}
			free(indices);
		}
		MLOG("};\n");
	}

	free(name);
	*/
	return true;
}

NS_MUGGLE_END