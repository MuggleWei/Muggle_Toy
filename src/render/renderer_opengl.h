#ifndef __MUGGLE_RENDERER_OPENGL_H__
#define __MUGGLE_RENDERER_OPENGL_H__

#include "base/base.h"
#include "render/renderer.h"

NS_MUGGLE_BEGIN

class Renderer_OpenGL : public Renderer
{
public:
	MG_DLL Renderer_OpenGL();
	MG_DLL ~Renderer_OpenGL();

	MG_DLL virtual bool Initialize(const RenderInitParameter& init_param) override;
	MG_DLL virtual void Destroy() override;
	MG_DLL virtual void BeginScene() override;
	MG_DLL virtual void EndScene() override;

	MG_DLL virtual void setClearColor(const vec4f& color) override;

protected:
	bool ExtensionSupported(const char* extension_name);

#if MG_PLATFORM_WINDOWS
	bool Initialize_Windows(const RenderInitParameter& init_param);
	void Destroy_Windows();
#else
#endif

protected:
#if MG_PLATFORM_WINDOWS
	void* m_hdc;
	void* m_hglrc;
	void* wglSwapIntervalEXT;
#endif
};

NS_MUGGLE_END

#endif