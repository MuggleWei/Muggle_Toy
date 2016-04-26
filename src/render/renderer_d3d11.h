#ifndef __MUGGLE_RENDERER_D3D11_H__
#define __MUGGLE_RENDERER_D3D11_H__

#include "base/base.h"
#include "render/renderer.h"

//////////////
// INCLUDES //
//////////////
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>

#include <vector>

NS_MUGGLE_BEGIN

class Renderer_D3D11 : public Renderer
{
public:
	MG_DLL Renderer_D3D11();
	MG_DLL ~Renderer_D3D11();

	MG_DLL virtual bool Initialize(const RenderInitParameter& init_param) override;
	MG_DLL virtual void Destroy() override;
	MG_DLL virtual void BeginScene() override;
	MG_DLL virtual void EndScene() override;

	MG_DLL virtual void setShader(const ShaderProgram* shader_program) override;

protected:
	bool RetrieveHardwareInfo();
	bool InitDeviceAndSwapChain();
	bool InitBackBuffer();
	bool InitDepthStencil();
	bool InitRaster();
	bool InitViewport();

	void FilloutSwapChainDesc(DXGI_SWAP_CHAIN_DESC& swap_chain_desc);
	void FilloutDepthBufferDesc(D3D11_TEXTURE2D_DESC& depth_buffer_desc);
	void FilloutDepthStencilDesc(D3D11_DEPTH_STENCIL_DESC& depth_stencil_desc);
	void FilloutDepthStencilViewDesc(D3D11_DEPTH_STENCIL_VIEW_DESC& depth_stencil_view_desc);

protected:
	IDXGISwapChain* m_swap_chain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_device_context;
	ID3D11RenderTargetView* m_render_target_view;
	ID3D11DepthStencilView* m_depth_stencil_view;
	ID3D11Texture2D* m_depth_stencil_buffer;
	ID3D11DepthStencilState* m_depth_stencil_state;
	ID3D11RasterizerState* m_raster_state;
	DXGI_FORMAT m_dxgi_format;
	D3D_FEATURE_LEVEL m_feature_level;

	std::vector<VideoCardInfo> m_video_card_infos;
};

NS_MUGGLE_END

#endif