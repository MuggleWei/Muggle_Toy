#include "renderer.h"
#include "renderer_opengl.h"
#if MG_ENABLE_DX11
#include "renderer_d3d11.h"
#endif

NS_MUGGLE_BEGIN

Renderer* CreateRenderer(RenderType::Enum render_type)
{
	switch (render_type)
	{
		case RenderType::Enum::OpenGL:
		{
			return new Renderer_OpenGL();
		}break;
#if MG_ENABLE_DX11
		case RenderType::Enum::D3D11:
		{
			return new Renderer_D3D11();
		}break;
#endif
		default:
		{
			MASSERT_MSG(0, "RenderType %s does not support", RenderType::EnumToString(render_type));
		}break;
	}

	return nullptr;
}

NS_MUGGLE_END