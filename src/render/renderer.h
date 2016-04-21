#ifndef __MUGGLE_RENDERER_H__
#define __MUGGLE_RENDERER_H__

#include "base/base.h"
#include "render/render_info.h"
#include "math/vector4.h"

NS_MUGGLE_BEGIN

class Renderer
{
public:
	MG_DLL Renderer()
	{
		m_vsync = false;
		m_width = 0;
		m_height = 0;
		m_full_screen = false;
		m_clear_color = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	}
	MG_DLL virtual ~Renderer() {}

	Renderer(const Renderer&) = delete;
	Renderer(const Renderer&&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(const Renderer&&) = delete;

	MG_DLL virtual bool Initialize(const RenderInitParameter& init_param) = 0;
	MG_DLL virtual void Destroy() = 0;
	MG_DLL virtual void BeginScene() = 0;
	MG_DLL virtual void EndScene() = 0;

	MG_DLL void setClearColor(const vec4f& color)
	{
		m_clear_color = color;
	}

protected:
	RenderType::Enum m_render_type;

	bool m_vsync;
	int m_width;
	int m_height;
	bool m_full_screen;	
	vec4f m_clear_color;

};

MG_DLL Renderer* CreateRenderer(RenderType::Enum render_type);

NS_MUGGLE_END

#endif