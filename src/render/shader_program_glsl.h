#ifndef __MUGGLE_SHADER_PROGRAM_GLSL_H__
#define __MUGGLE_SHADER_PROGRAM_GLSL_H__

#include "base/base.h"
#include <string>
#include <map>
#include "math/vector2.h"
#include "math/vector3.h"
#include "math/vector4.h"
#include "math/matrix4x4.h"
#include "render/render_info.h"
#include "texture.h"

NS_MUGGLE_BEGIN

class ShaderObj;

struct GLSLVarInfo
{
	uint32_t index;
	int32_t location;
	uint32_t type;
	int32_t size;
	int32_t offset;	// for variable in uniform block
};

class GLSLUniformBlock
{
public:
	uint32_t index;
	uint32_t size;
	std::map<std::string, GLSLVarInfo> var_info_map;
};

struct GLSLSubroutine
{
	uint32_t index;
	ShaderStageType::Enum stage;
};

class ShaderProgramGLSL
{
public:
	MG_DLL ShaderProgramGLSL();
	MG_DLL ~ShaderProgramGLSL();

	MG_DLL bool isLinked();
	MG_DLL uint32_t getHandle();

	MG_DLL bool Initialize();
	MG_DLL bool Attach(const ShaderObj* shader_obj);
	MG_DLL bool Link();
	MG_DLL bool Use();
	MG_DLL void Destroy();

	MG_DLL void OutputInformation();
	MG_DLL void OutputActiveAttribs();
	MG_DLL void OutputActiveUniforms();
	MG_DLL void OutputActiveUniformBlocks();

	MG_DLL bool bindAttribLocation(uint32_t location, const char* name);
	MG_DLL bool bindFragDataLocation(uint32_t location, const char *name);

	MG_DLL int getUniformLocation(const char* name);

	MG_DLL void setUniform(const char* name, float x, float y, float z);
	MG_DLL void setUniform(const char* name, const muggle::vec2f& v);
	MG_DLL void setUniform(const char* name, const muggle::vec3f& v);
	MG_DLL void setUniform(const char* name, const muggle::vec4f& v);
	MG_DLL void setUniform(const char* name, const muggle::matrix3f& mat3);
	MG_DLL void setUniform(const char* name, const muggle::matrix4f& mat4);
	MG_DLL void setUniform(const char* name, float val);
	MG_DLL void setUniform(const char* name, int val);
	MG_DLL void setUniform(const char* name, bool val);	

	MG_DLL void setTexture(const char* name, Texture* tex, int tex_unit);

protected:
	bool RetrieveInfo();
	bool RetrieveAttribInfo();
	bool RetrieveUniformInfo();
	bool RetrieveUniformBlockInfo();
	bool RetrieveSubroutineInfo();

private:
	uint32_t m_handle;			// handle of shader program
	bool m_linked;				// whether or not linked
	std::string	m_log;			// log
	std::map<std::string, GLSLVarInfo>	m_attrib_map;				// map string to attribute variable
	std::map<std::string, GLSLVarInfo>	m_uniform_map;				// map string to uniform variable
	std::map<std::string, GLSLUniformBlock>	m_uniform_block_map;	// map string to uniform block
	std::map<std::string, GLSLSubroutine> m_subroutine_map;			// map string to subroutine
};

NS_MUGGLE_END

#endif 