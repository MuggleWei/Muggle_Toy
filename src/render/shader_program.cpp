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

	return true;
}

NS_MUGGLE_END