#include "shader_program_glsl.h"
#include "shader_obj.h"
#include "glad/glad.h"

NS_MUGGLE_BEGIN

const char* ConvertGLTypeToString(GLenum type)
{
	switch (type)
	{
	case GL_FLOAT:
	{
		return "float";
	}break;
	case GL_FLOAT_VEC2:
	{
		return "vec2";
	}break;
	case GL_FLOAT_VEC3:
	{
		return "vec3";
	}break;
	case GL_FLOAT_VEC4:
	{
		return "vec4";
	}break;
	case GL_DOUBLE:
	{
		return "double";
	}break;
	case GL_DOUBLE_VEC2:
	{
		return "dvec2";
	}break;
	case GL_DOUBLE_VEC3:
	{
		return "dvec3";
	}break;
	case GL_DOUBLE_VEC4:
	{
		return "dvec4";
	}break;
	case GL_INT:
	{
		return "int";
	}break;
	case GL_INT_VEC2:
	{
		return "ivec2";
	}break;
	case GL_INT_VEC3:
	{
		return "ivec3";
	}break;
	case GL_INT_VEC4:
	{
		return "ivec4";
	}break;
	case GL_UNSIGNED_INT:
	{
		return "unsigned int";
	}break;
	case GL_UNSIGNED_INT_VEC2:
	{
		return "uvec2";
	}break;
	case GL_UNSIGNED_INT_VEC3:
	{
		return "uvec3";
	}break;
	case GL_UNSIGNED_INT_VEC4:
	{
		return "uvec4";
	}break;
	case GL_BOOL:
	{
		return "bool";
	}break;
	case GL_BOOL_VEC2:
	{
		return "bvec2";
	}break;
	case GL_BOOL_VEC3:
	{
		return "bvec3";
	}break;
	case GL_BOOL_VEC4:
	{
		return "bvec4";
	}break;
	case GL_FLOAT_MAT2:
	{
		return "mat2";
	}break;
	case GL_FLOAT_MAT3:
	{
		return "mat3";
	}break;
	case GL_FLOAT_MAT4:
	{
		return "mat4";
	}break;
	case GL_FLOAT_MAT2x3:
	{
		return "mat2x3";
	}break;
	case GL_FLOAT_MAT2x4:
	{
		return "mat2x4";
	}break;
	case GL_FLOAT_MAT3x2:
	{
		return "mat3x2";
	}break;
	case GL_FLOAT_MAT3x4:
	{
		return "mat3x4";
	}break;
	case GL_FLOAT_MAT4x2:
	{
		return "mat4x2";
	}break;
	case GL_FLOAT_MAT4x3:
	{
		return "mat4x3";
	}break;
	case GL_DOUBLE_MAT2:
	{
		return "dmat2";
	}break;
	case GL_DOUBLE_MAT3:
	{
		return "dmat3";
	}break;
	case GL_DOUBLE_MAT4:
	{
		return "dmat4";
	}break;
	case GL_DOUBLE_MAT2x3:
	{
		return "dmat2x3";
	}break;
	case GL_DOUBLE_MAT2x4:
	{
		return "dmat2x4";
	}break;
	case GL_DOUBLE_MAT3x2:
	{
		return "dmat3x2";
	}break;
	case GL_DOUBLE_MAT3x4:
	{
		return "dmat3x4";
	}break;
	case GL_DOUBLE_MAT4x2:
	{
		return "dmat4x2";
	}break;
	case GL_DOUBLE_MAT4x3:
	{
		return "dmat4x3";
	}break;
	case GL_SAMPLER_1D:
	{
		return "sampler1D";
	}break;
	case GL_SAMPLER_2D:
	{
		return "sampler2D";
	}break;
	case GL_SAMPLER_3D:
	{
		return "sampler3D";
	}break;
	case GL_SAMPLER_CUBE:
	{
		return "samplerCube";
	}break;
	case GL_SAMPLER_1D_SHADOW:
	{
		return "sampler1DShadow";
	}break;
	case GL_SAMPLER_2D_SHADOW:
	{
		return "sampler2DShadow";
	}break;
	case GL_SAMPLER_1D_ARRAY:
	{
		return "sampler1DArray";
	}break;
	case GL_SAMPLER_2D_ARRAY:
	{
		return "sampler2DArray";
	}break;
	case GL_SAMPLER_1D_ARRAY_SHADOW:
	{
		return "sampler1DArrayShadow";
	}break;
	case GL_SAMPLER_2D_ARRAY_SHADOW:
	{
		return "sampler2DArrayShadow";
	}break;
	case GL_SAMPLER_2D_MULTISAMPLE:
	{
		return "sampler2DMS";
	}break;
	case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
	{
		return "sampler2DMSArray";
	}break;
	case GL_SAMPLER_CUBE_SHADOW:
	{
		return "samplerCubeShadow";
	}break;
	case GL_SAMPLER_BUFFER:
	{
		return "samplerBuffer";
	}break;
	case GL_SAMPLER_2D_RECT:
	{
		return "sampler2DRect";
	}break;
	case GL_SAMPLER_2D_RECT_SHADOW:
	{
		return "sampler2DRectShadow";
	}break;
	case GL_INT_SAMPLER_1D:
	{
		return "isampler1D";
	}break;
	case GL_INT_SAMPLER_2D:
	{
		return "isampler2D";
	}break;
	case GL_INT_SAMPLER_3D:
	{
		return "isampler3D";
	}break;
	case GL_INT_SAMPLER_CUBE:
	{
		return "isamplerCube";
	}break;
	case GL_INT_SAMPLER_1D_ARRAY:
	{
		return "isampler1DArray";
	}break;
	case GL_INT_SAMPLER_2D_ARRAY:
	{
		return "isampler2DArray";
	}break;
	case GL_INT_SAMPLER_2D_MULTISAMPLE:
	{
		return "isampler2DMS";
	}break;
	case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
	{
		return "isampler2DMSArray";
	}break;
	case GL_INT_SAMPLER_BUFFER:
	{
		return "isamplerBuffer";
	}break;
	case GL_INT_SAMPLER_2D_RECT:
	{
		return "isampler2DRect";
	}break;
	case GL_UNSIGNED_INT_SAMPLER_1D:
	{
		return "usampler1D";
	}break;
	case GL_UNSIGNED_INT_SAMPLER_2D:
	{
		return "usampler2D";
	}break;
	case GL_UNSIGNED_INT_SAMPLER_3D:
	{
		return "usampler3D";
	}break;
	case GL_UNSIGNED_INT_SAMPLER_CUBE:
	{
		return "usamplerCube";
	}break;
	case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
	{
		return "usampler2DArray";
	}break;
	case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
	{
		return "usampler2DArray";
	}break;
	case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
	{
		return "usampler2DMS";
	}break;
	case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
	{
		return "usampler2DMSArray";
	}break;
	case GL_UNSIGNED_INT_SAMPLER_BUFFER:
	{
		return "usamplerBuffer";
	}break;
	case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
	{
		return "usampler2DRect";
	}break;
	case GL_IMAGE_1D:
	{
		return "image1D";
	}break;
	case GL_IMAGE_2D:
	{
		return "image2D";
	}break;
	case GL_IMAGE_3D:
	{
		return "image3D";
	}break;
	case GL_IMAGE_2D_RECT:
	{
		return "image2DRect";
	}break;
	case GL_IMAGE_CUBE:
	{
		return "imageCube";
	}break;
	case GL_IMAGE_BUFFER:
	{
		return "imageBuffer";
	}break;
	case GL_IMAGE_1D_ARRAY:
	{
		return "image1DArray";
	}break;
	case GL_IMAGE_2D_ARRAY:
	{
		return "image2DArray";
	}break;
	case GL_IMAGE_2D_MULTISAMPLE:
	{
		return "image2DMS";
	}break;
	case GL_IMAGE_2D_MULTISAMPLE_ARRAY:
	{
		return "image2DMSArray";
	}break;
	case GL_INT_IMAGE_1D:
	{
		return "iimage1D";
	}break;
	case GL_INT_IMAGE_2D:
	{
		return "iimage2D";
	}break;
	case GL_INT_IMAGE_3D:
	{
		return "iimage3D";
	}break;
	case GL_INT_IMAGE_2D_RECT:
	{
		return "iimage2DRect";
	}break;
	case GL_INT_IMAGE_CUBE:
	{
		return "iimageCube";
	}break;
	case GL_INT_IMAGE_BUFFER:
	{
		return "iimageBuffer";
	}break;
	case GL_INT_IMAGE_1D_ARRAY:
	{
		return "iimage1DArray";
	}break;
	case GL_INT_IMAGE_2D_ARRAY:
	{
		return "iimage2DArray";
	}break;
	case GL_INT_IMAGE_2D_MULTISAMPLE:
	{
		return "iimage2DMS";
	}break;
	case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
	{
		return "iimage2DMSArray";
	}break;
	case GL_UNSIGNED_INT_IMAGE_1D:
	{
		return "uimage1D";
	}break;
	case GL_UNSIGNED_INT_IMAGE_2D:
	{
		return "uimage2D";
	}break;
	case GL_UNSIGNED_INT_IMAGE_3D:
	{
		return "uimage3D";
	}break;
	case GL_UNSIGNED_INT_IMAGE_2D_RECT:
	{
		return "uimage2DRect";
	}break;
	case GL_UNSIGNED_INT_IMAGE_CUBE:
	{
		return "uimageCube";
	}break;
	case GL_UNSIGNED_INT_IMAGE_BUFFER:
	{
		return "uimageBuffer";
	}break;
	case GL_UNSIGNED_INT_IMAGE_1D_ARRAY:
	{
		return "uimage1DArray";
	}break;
	case GL_UNSIGNED_INT_IMAGE_2D_ARRAY:
	{
		return "uimage2DArray";
	}break;
	case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE:
	{
		return "uimage2DMS";
	}break;
	case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
	{
		return "uimage2DMSArray";
	}break;
	case GL_UNSIGNED_INT_ATOMIC_COUNTER:
	{
		return "atomic_uint";
	}break;
	default:
	{
		return "";
	}break;
	}

	return "";
}

ShaderProgramGLSL::ShaderProgramGLSL()
{
	m_handle = 0;
	m_linked = false;
}
ShaderProgramGLSL::~ShaderProgramGLSL()
{
	MERROR(m_handle == 0,
		"Must use Destroy() function to release shader program resource before destructor");
}

bool ShaderProgramGLSL::Initialize()
{
	m_handle = glCreateProgram();
	if (m_handle == 0)
	{
		MERROR(0, "Failed creating program object");
		return false;
	}

	return true;
}
bool ShaderProgramGLSL::isLinked()
{
	return m_linked;
}
uint32_t ShaderProgramGLSL::getHandle()
{
	return m_handle;
}

bool ShaderProgramGLSL::Attach(const ShaderObj* shader_obj)
{
	MASSERT(m_handle != 0);
	glAttachShader(m_handle, shader_obj->gl_shader_handle);

	return true;
}
bool ShaderProgramGLSL::Link()
{
	glLinkProgram(m_handle);

	// verify the link status
	GLint status;
	glGetProgramiv(m_handle, GL_LINK_STATUS, &status);
	if (GL_FALSE == status)
	{
		MLOG("Failed to link shader program!\n");
		GLint log_len;
		glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &log_len);
		if (log_len > 0)
		{
			char *log = (char*)malloc(log_len);
			GLsizei written;
			glGetProgramInfoLog(m_handle, log_len, &written, log);
			MLOG("Program log: \n%s", log);
			free(log);
		}
		MASSERT_MSG(0, "gl shader link error");
		return false;
	}

	// retrieve information
	RetrieveInfo();

#if MUGGLE_DEBUG
	OutputInformation();
#endif

	m_linked = true;

	return true;
}
bool ShaderProgramGLSL::Use()
{
	if (m_handle == 0 || m_linked == false)
	{
		MERROR(0, "Failed use unlinked shader");
		return false;
	}
	glUseProgram(m_handle);
	return true;
}
void ShaderProgramGLSL::Destroy()
{
	MWARNING(m_handle != 0, "Can't destroy an invalid gl shader program");
	if (m_handle != 0)
	{
		glDeleteProgram(m_handle);
		m_handle = 0;
		m_linked = false;
	}
}

void ShaderProgramGLSL::OutputInformation() 
{
	MLOG("=============================\n");
	MLOG("Shader Program info:\n");

	OutputActiveUniforms();
	OutputActiveAttribs();
	OutputActiveUniformBlocks();
}
void ShaderProgramGLSL::OutputActiveUniforms()
{
	for (auto &var : m_attrib_map)
	{
		MLOG("Attribute: [location %d] %s %s;\n",
			var.second.location, ConvertGLTypeToString(var.second.type), var.first.c_str());
	}
	
}
void ShaderProgramGLSL::OutputActiveAttribs() 
{
	for (auto &var : m_uniform_map)
	{
		MLOG("Uniform: [location %d] %s %s;\n",
			var.second.location, ConvertGLTypeToString(var.second.type), var.first.c_str());
	}
}
void ShaderProgramGLSL::OutputActiveUniformBlocks() 
{
	for (auto &uniform_block : m_uniform_block_map)
	{
		MLOG("Uniform block: [index %d] %s\n", uniform_block.second.index, uniform_block.first.c_str());
		MLOG("{\n");
		for (auto &var : uniform_block.second.var_info_map)
		{
			MLOG("\t%s %s;\n", ConvertGLTypeToString(var.second.type), var.first.c_str());
		}
		MLOG("};\n");
	}
}

bool ShaderProgramGLSL::bindAttribLocation(uint32_t location, const char* name)
{
	if (m_handle == 0 || m_linked)
	{ 
		MWARNING(0, "Failed in bind attribute location: %u - %s\n", location, name);
		return false;
	}
	glBindAttribLocation(m_handle, location, name);
	return true;
}
bool ShaderProgramGLSL::bindFragDataLocation(uint32_t location, const char * name)
{
	if (m_handle == 0 || m_linked)
	{
		MWARNING(0, "Failed in bind fragment data location: %u - %s\n", location, name);
		return false;
	}
	glBindFragDataLocation(m_handle, location, name);
	return true;
}

int ShaderProgramGLSL::getUniformLocation(const char* name)
{
	int loc = -1;
	auto it = m_uniform_map.find(name);
	if (it != m_uniform_map.end())
	{
		loc = it->second.location;
	}

	return loc;
}

void ShaderProgramGLSL::setUniform(const char* name, float x, float y, float z)
{
	auto it = m_uniform_map.find(name);
	if (it == m_uniform_map.end())
	{
		MWARNING(0, "Can't find uniform name: %s\n", name);
		return;
	}

	GLSLVarInfo &var = it->second;
	if (var.location >= 0)
	{
		glUniform3f(var.location, x, y, z);
	}
	else
	{
		MWARNING(0, "%s's uniform location is invalid\n", name);
	}
}
void ShaderProgramGLSL::setUniform(const char* name, const muggle::vec2f& v)
{
	auto it = m_uniform_map.find(name);
	if (it == m_uniform_map.end())
	{
		MWARNING(0, "Can't find uniform name: %s\n", name);
		return;
	}

	GLSLVarInfo &var = it->second;
	if (var.location >= 0)
	{
		glUniform2f(var.location, v.x, v.y);
	}
	else
	{
		MWARNING(0, "%s's uniform location is invalid\n", name);
	}
}
void ShaderProgramGLSL::setUniform(const char* name, const muggle::vec3f& v)
{
	auto it = m_uniform_map.find(name);
	if (it == m_uniform_map.end())
	{
		MWARNING(0, "Can't find uniform name: %s\n", name);
		return;
	}

	GLSLVarInfo &var = it->second;
	if (var.location >= 0)
	{
		glUniform3f(var.location, v.x, v.y, v.z);
	}
	else
	{
		MWARNING(0, "%s's uniform location is invalid\n", name);
	}
}
void ShaderProgramGLSL::setUniform(const char* name, const muggle::vec4f& v)
{
	auto it = m_uniform_map.find(name);
	if (it == m_uniform_map.end())
	{
		MWARNING(0, "Can't find uniform name: %s\n", name);
		return;
	}

	GLSLVarInfo &var = it->second;
	if (var.location >= 0)
	{
		glUniform4f(var.location, v.x, v.y, v.z, v.w);
	}
	else
	{
		MWARNING(0, "%s's uniform location is invalid\n", name);
	}
}
void ShaderProgramGLSL::setUniform(const char* name, const muggle::matrix4f& mat4)
{
	auto it = m_uniform_map.find(name);
	if (it == m_uniform_map.end())
	{
		MWARNING(0, "Can't find uniform name: %s\n", name);
		return;
	}

	GLSLVarInfo &var = it->second;
	if (var.location >= 0)
	{
		glUniformMatrix4fv(var.location, 1, GL_TRUE, &mat4.mat[0]);
	}
	else
	{
		MWARNING(0, "%s's uniform location is invalid\n", name);
	}
}
void ShaderProgramGLSL::setUniform(const char* name, float val)
{
	auto it = m_uniform_map.find(name);
	if (it == m_uniform_map.end())
	{
		MWARNING(0, "Can't find uniform name: %s\n", name);
		return;
	}

	GLSLVarInfo &var = it->second;
	if (var.location >= 0)
	{
		glUniform1f(var.location, val);
	}
	else
	{
		MWARNING(0, "%s's uniform location is invalid\n", name);
	}
}
void ShaderProgramGLSL::setUniform(const char* name, int val)
{
	auto it = m_uniform_map.find(name);
	if (it == m_uniform_map.end())
	{
		MWARNING(0, "Can't find uniform name: %s\n", name);
		return;
	}

	GLSLVarInfo &var = it->second;
	if (var.location >= 0)
	{
		glUniform1i(var.location, val);
	}
	else
	{
		MWARNING(0, "%s's uniform location is invalid\n", name);
	}
}
void ShaderProgramGLSL::setUniform(const char* name, bool val)
{
	auto it = m_uniform_map.find(name);
	if (it == m_uniform_map.end())
	{
		MWARNING(0, "Can't find uniform name: %s\n", name);
		return;
	}

	GLSLVarInfo &var = it->second;
	if (var.location >= 0)
	{
		glUniform1i(var.location, val);
	}
	else
	{
		MWARNING(0, "%s's uniform location is invalid\n", name);
	}
}

bool ShaderProgramGLSL::RetrieveInfo()
{
	if (m_handle == 0)
	{
		return false;
	}

	RetrieveAttribInfo();
	RetrieveUniformInfo();
	RetrieveUniformBlockInfo();

	return true;
}
bool ShaderProgramGLSL::RetrieveAttribInfo()
{
	GLint n_nums, max_len;
	glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTES, &n_nums);
	glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_len);
	GLchar* name = (GLchar*)malloc(max_len + 1);

	GLint size, location;
	GLsizei length;
	GLenum type;
	for (GLint i = 0; i < n_nums; ++i)
	{
		glGetActiveAttrib(
			m_handle, (GLuint)i,
			max_len, &length,
			&size, &type, name);
		location = glGetAttribLocation(m_handle, name);

		GLSLVarInfo var_info;
		var_info.index = i;
		var_info.location = location;
		var_info.type = type;
		var_info.size = size;
		var_info.offset = 0;

		m_attrib_map[name] = var_info;
	}

	free(name);

	return true;
}
bool ShaderProgramGLSL::RetrieveUniformInfo()
{
	GLint n_nums, max_len;
	glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &n_nums);
	glGetProgramiv(m_handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_len);
	GLchar* name = (GLchar*)malloc(max_len + 1);

	GLint size, location;
	GLsizei length;
	GLenum type;
	for (GLint i = 0; i < n_nums; ++i)
	{
		glGetActiveUniform(
			m_handle, (GLuint)i,
			max_len, &length,
			&size, &type, name);
		location = glGetUniformLocation(m_handle, name);

		if (location < 0)
		{
			continue;
		}

		GLSLVarInfo var_info;
		var_info.index = i;
		var_info.location = location;
		var_info.type = type;
		var_info.size = size;
		var_info.offset = 0;

		m_uniform_map[name] = var_info;
	}

	free(name);

	return true;
}
bool ShaderProgramGLSL::RetrieveUniformBlockInfo()
{
	GLint n_nums, max_len, max_block_len, max_uniform_len;
	glGetProgramiv(m_handle, GL_ACTIVE_UNIFORM_BLOCKS, &n_nums);
	glGetProgramiv(m_handle, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &max_block_len);
	glGetProgramiv(m_handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_uniform_len);
	max_len = max_block_len > max_uniform_len ? max_block_len : max_uniform_len;

	GLchar* name = (GLchar*)malloc(max_len + 1);

	GLint size;
	GLsizei length;
	GLenum type;
	GLint num_active_uniform_in_block;
	for (GLint i = 0; i < n_nums; ++i)
	{
		// get block uniform information
		glGetActiveUniformBlockName(
			m_handle, (GLuint)i,
			max_len, &length,
			name);
		GLint block_size;
		glGetActiveUniformBlockiv(m_handle, i, GL_UNIFORM_BLOCK_DATA_SIZE, &block_size);

		GLSLUniformBlock uniform_block;
		uniform_block.index = i;
		uniform_block.size = block_size;
		std::string uniform_block_name = name;

		// get block variable information 
		glGetActiveUniformBlockiv(m_handle, (GLuint)i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &num_active_uniform_in_block);
		if (num_active_uniform_in_block > 0)
		{
			GLint* indices = (GLint*)malloc(num_active_uniform_in_block * sizeof(GLint));
			glGetActiveUniformBlockiv(m_handle, (GLuint)i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, indices);
			for (GLint j = 0; j < num_active_uniform_in_block; ++j)
			{
				glGetActiveUniform(
					m_handle, (GLuint)j,
					max_len, &length,
					&size, &type, name);
				GLuint block_var_index;
				glGetUniformIndices(m_handle, 1, (const GLchar**)&name, &block_var_index);
				GLint offset;
				glGetActiveUniformsiv(m_handle, 1, &block_var_index, GL_UNIFORM_OFFSET, &offset);

				GLSLVarInfo var_info;
				var_info.index = block_var_index;
				var_info.location = -1;
				var_info.type = type;
				var_info.size = size;
				var_info.offset = offset;
				uniform_block.var_info_map[name] = var_info;
			}
			free(indices);
		}

		m_uniform_block_map[uniform_block_name] = uniform_block;
	}

	free(name);

	return true;
}


NS_MUGGLE_END