#ifndef __MUGGLE_SHADER_OBJECT_H__
#define __MUGGLE_SHADER_OBJECT_H__

#include "base/base.h"
#include "render/render_info.h"

NS_MUGGLE_BEGIN

class Renderer;

class MG_DLL ShaderObj
{
public:
	union
	{
		uint32_t gl_shader_handle;
		void* dx_shader_ptr;
	};

	ShaderType::Enum shader_type;
	ShaderStageType::Enum shader_stage_type;

public:
	ShaderObj();
	~ShaderObj();

	bool Initialize(
		Renderer* renderer,
		const char* file_name,
		const char* entry_point,
		ShaderStageType::Enum shader_stage_type,
		ShaderType::Enum shader_type);
	void Destroy();

protected:
	bool Initialzie_GLSL(Renderer* renderer,
		const char* file_name,
		const char* entry_point,
		ShaderStageType::Enum shader_stage_type,
		ShaderType::Enum shader_type);
};

MG_DLL ShaderObj* CreateShaderObj(
	Renderer* renderer,
	const char* file_name,
	const char* entry_point,
	ShaderStageType::Enum shader_stage_type,
	ShaderType::Enum shader_type);

NS_MUGGLE_END

#endif