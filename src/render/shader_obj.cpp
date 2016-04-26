#include "shader_obj.h"
#include "base/file_utility.h"
#include "renderer.h"
#include "glad/glad.h"

NS_MUGGLE_BEGIN

const GLenum GLShaderStageType[ShaderStageType::Enum::Max] =
{
	GL_VERTEX_SHADER,
	GL_FRAGMENT_SHADER,
	GL_COMPUTE_SHADER
};

ShaderObj::ShaderObj()
{
	gl_shader_handle = 0;
	dx_shader_ptr = nullptr;
}
ShaderObj::~ShaderObj()
{
	MASSERT_MSG(gl_shader_handle == 0 && dx_shader_ptr == nullptr,
		"Must use Destroy() function to release shader object resource before destructor");
}

bool ShaderObj::Initialize(
	Renderer* renderer,
	const char* file_name,
	const char* entry_point,
	ShaderStageType::Enum shader_stage_type_in,
	ShaderType::Enum shader_type_in)
{
	shader_stage_type = shader_stage_type_in;
	shader_type = shader_type_in;

	switch (shader_type)
	{
		case ShaderType::Enum::GLSL:
		{
			MASSERT_MSG(
				renderer->getType() == RenderType::Enum::OpenGL ||
				renderer->getType() == RenderType::Enum::Vulkan
				, "%s can't use %s shader type"
				, RenderType::EnumToString(renderer->getType()), ShaderType::EnumToString(shader_type));
			return Initialzie_GLSL(renderer, file_name, entry_point, shader_stage_type, shader_type);
		}break;
		default:
		{
			MASSERT_MSG(0, "Not support shader type");
			return false;
		}break;
	}

	return true;
}
void ShaderObj::Destroy()
{
	switch (shader_type)
	{
		case ShaderType::GLSL:
		{
			if (gl_shader_handle != 0)
			{
				glDeleteShader(gl_shader_handle);
				gl_shader_handle = 0;
			}
		}break;
		case ShaderType::HLSL:
		{
			MASSERT_MSG(0, "To be continued...");
		}break;
		case ShaderType::Cg:
		{
			MASSERT_MSG(0, "To be continued...");
		}break;
	}
}

bool ShaderObj::Initialzie_GLSL(Renderer* renderer,
	const char* file_name,
	const char* entry_point,
	ShaderStageType::Enum shader_stage_type,
	ShaderType::Enum shader_type)
{
	// create shader object
	GLuint shader_handle = glCreateShader(GLShaderStageType[shader_stage_type]);
	if (shader_handle == 0)
	{
		MASSERT_MSG(0, "Failed in create shader %s : %s", file_name, entry_point);
		return false;
	}

	// get shader source code
	char file_path_buffer[MG_MAX_PATH];
	File_GetAbsolutePath(file_name, file_path_buffer);
	char* shader_source_code = nullptr;
	long size = 0;
	if (!File_Read(file_path_buffer, &shader_source_code, &size))
	{
		MASSERT_MSG(0, "Failed in read shader source code from file %s", file_name);
		return false;
	}

	// copy source code into shader object
	const GLchar* code_array[] = { shader_source_code };
	glShaderSource(shader_handle, 1, code_array, NULL);

	// compile shader
	glCompileShader(shader_handle);

	// free shader source code
	free(shader_source_code);

	// verify the compilation status
	GLint gl_result;
	glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &gl_result);
	if (GL_FALSE == gl_result)
	{
		MLOG("Shader compilation failed : %s\n", file_name);
		GLint log_len;
		glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &log_len);
		if (log_len > 0)
		{
			char *log = (char*)malloc(log_len);
			GLsizei written;
			glGetShaderInfoLog(shader_handle, log_len, &written, log);
			MLOG("Shader log:\n%s", log);
			free(log);
		}
		MASSERT_MSG(0, "Shader compilation failed : %s\n", file_name);
		return false;
	}

	// init shader object
	gl_shader_handle = shader_handle;
	shader_type = shader_type;
	shader_stage_type = shader_stage_type;

	return true;
}

ShaderObj* CreateShaderObj(
	Renderer* renderer,
	const char* file_name,
	const char* entry_point,
	ShaderStageType::Enum shader_stage_type,
	ShaderType::Enum shader_type)
{
	ShaderObj* shader_obj = new ShaderObj;
	if (shader_obj == nullptr)
	{
		MASSERT_MSG(0, "Failed in create shader object");
		return nullptr;
	}

	if (!shader_obj->Initialize(renderer, file_name, entry_point, shader_stage_type, shader_type))
	{
		shader_obj->Destroy();
		delete shader_obj;
		MASSERT_MSG(0, "Failed in initialize shader object");
		return nullptr;
	}

	return shader_obj;
}

NS_MUGGLE_END