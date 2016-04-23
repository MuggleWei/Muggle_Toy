#include "renderer.h"
#include "renderer_opengl.h"
#if MG_PLATFORM_WINDOWS
#include "renderer_d3d11.h"
#endif

NS_MUGGLE_BEGIN

Renderer* CreateRenderer(RenderType::Enum render_type)
{
	switch (render_type)
	{
#if MG_PLATFORM_WINDOWS
		case RenderType::Enum::OpenGL:
		{
			return new Renderer_OpenGL();
		}break;
		case RenderType::Enum::D3D11:
		{
			return new Renderer_D3D11();
		}break;
#endif
	}

	return nullptr;
}

NS_MUGGLE_END