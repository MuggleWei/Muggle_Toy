#ifndef __MUGGLE_RENDERER_H__
#define __MUGGLE_RENDERER_H__

#include "base/base.h"
#include "render/render_info.h"
#include "render/shader_obj.h"
#include "math/vector4.h"

NS_MUGGLE_BEGIN

class ShaderProgram;

class Renderer
{
public:
	MG_DLL Renderer()
	{
		m_hWnd = nullptr;
		m_vsync = false;
		m_width = 0;
		m_height = 0;
		m_full_screen = false;
		m_clear_color = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	}
	MG_DLL virtual ~Renderer() {}

	RenderType::Enum getType()
	{
		return m_render_type;
	}

	Renderer(const Renderer&) = delete;
	Renderer(const Renderer&&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(const Renderer&&) = delete;

	MG_DLL virtual bool Initialize(const RenderInitParameter& init_param) = 0
	{
		m_hWnd = init_param.hWnd;
		m_vsync = init_param.vsync;
		m_width = init_param.win_width;
		m_height = init_param.win_height;
		m_full_screen = init_param.full_screen;

		return true;
	}
	MG_DLL virtual void Destroy() = 0;
	MG_DLL virtual void BeginScene() = 0;
	MG_DLL virtual void EndScene() = 0;

	MG_DLL virtual void setClearColor(const vec4f& color)
	{
		m_clear_color = color;
	}

	MG_DLL virtual void setShader(const ShaderProgram* shader_program) = 0;

protected:
	RenderType::Enum m_render_type;

	void* m_hWnd;
	bool m_vsync;
	int m_width;
	int m_height;
	bool m_full_screen;	
	vec4f m_clear_color;

};

MG_DLL Renderer* CreateRenderer(RenderType::Enum render_type);

NS_MUGGLE_END

#endif