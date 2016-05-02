#ifndef __MUGGLE_SHADER_PROGRAM_H__
#define __MUGGLE_SHADER_PROGRAM_H__

#include "base/base.h"

NS_MUGGLE_BEGIN

class ShaderObj;

class MG_DLL ShaderProgram
{
public:
	uint32_t gl_handle;

public:
	ShaderProgram();
	~ShaderProgram();

	bool Initialize();
	void Destroy();

	void Attach(const ShaderObj* shader_obj);
	bool Link();

	bool OutputInformation();
};

NS_MUGGLE_END

#endif