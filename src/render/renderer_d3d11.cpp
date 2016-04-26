#include "renderer_d3d11.h"

/////////////
// LINKING //
/////////////
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

NS_MUGGLE_BEGIN

DXGI_FORMAT MG_IMG_FMT_TO_DXGI[ImageFormat::Enum::Max] =
{
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_R8G8B8A8_UNORM,
	DXGI_FORMAT_R16G16B16A16_FLOAT
};

Renderer_D3D11::Renderer_D3D11()
{
	m_render_type = RenderType::Enum::D3D11;

	m_swap_chain = nullptr;
	m_device = nullptr;
	m_device_context = nullptr;	
	m_render_target_view = nullptr;
	m_depth_stencil_view = nullptr;
	m_depth_stencil_buffer = nullptr;
	m_depth_stencil_state = nullptr;
	m_raster_state = nullptr;
	m_dxgi_format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	m_feature_level = D3D_FEATURE_LEVEL_11_0;
}
Renderer_D3D11::~Renderer_D3D11()
{}

bool Renderer_D3D11::Initialize(const RenderInitParameter& init_param)
{
	MLOG("## Initialize d3d11\n");

	// initialize parent member variable
	Renderer::Initialize(init_param);

	// initialize member variable
	m_dxgi_format = MG_IMG_FMT_TO_DXGI[init_param.rt_format];
	
	// retrieve hardware information
	if (!RetrieveHardwareInfo())
	{
		MASSERT_MSG(0, "Something wrong in the retrieve hardware information!");
	}

	// initialize device and swap chain
	if (!InitDeviceAndSwapChain())
	{
		MASSERT_MSG(0, "Failed in d3d11 initialize device and swap chain!");
		return false;
	}

	// initialize back buffer
	if (!InitBackBuffer())
	{
		MASSERT_MSG(0, "Failed in d3d11 initialize back buffer!");
		return false;
	}

	// initialize depth stencil
	if (!InitDepthStencil())
	{
		MASSERT_MSG(0, "Failed in d3d11 initialize depth stencil!");
		return false;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline
	m_device_context->OMSetRenderTargets(1, &m_render_target_view, m_depth_stencil_view);

	// initialize raster
	if (!InitRaster())
	{
		MASSERT_MSG(0, "Failed in d3d11 initialize raster!");
		return false;
	}

	// initialize viewport
	if (!InitViewport())
	{
		MASSERT_MSG(0, "Failed in d3d11 initialize viewport!");
		return false;
	}

	return true;
}
void Renderer_D3D11::Destroy()
{
	MLOG("## Destroy d3d11\n");

	if (m_swap_chain)
	{
		m_swap_chain->SetFullscreenState(false, nullptr);
	}

	if (m_raster_state)
	{
		m_raster_state->Release();
		m_raster_state = nullptr;
	}

	if (m_depth_stencil_view)
	{
		m_depth_stencil_view->Release();
		m_depth_stencil_view = nullptr;
	}

	if (m_depth_stencil_state)
	{
		m_depth_stencil_state->Release();
		m_depth_stencil_state = nullptr;
	}

	if (m_depth_stencil_buffer)
	{
		m_depth_stencil_buffer->Release();
		m_depth_stencil_buffer = nullptr;
	}

	if (m_render_target_view)
	{
		m_render_target_view->Release();
		m_render_target_view = nullptr;
	}

	if (m_device_context)
	{
		m_device_context->Release();
		m_device_context = nullptr;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = nullptr;
	}

	if (m_swap_chain)
	{
		m_swap_chain->Release();
		m_swap_chain = nullptr;
	}
}
void Renderer_D3D11::BeginScene()
{
	m_device_context->ClearRenderTargetView(m_render_target_view, &m_clear_color[0]);
	m_device_context->ClearDepthStencilView(m_depth_stencil_view, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
void Renderer_D3D11::EndScene()
{
	if (m_vsync)
	{
		m_swap_chain->Present(1, 0);
	}
	else
	{
		m_swap_chain->Present(0, 0);
	}
}

void Renderer_D3D11::setShader(const ShaderProgram* shader_program)
{
	// TODO: 
}

bool Renderer_D3D11::RetrieveHardwareInfo()
{
	MLOG("## retrieve hardware render information: \n");

	// clear video card information
	m_video_card_infos.clear();

	HRESULT result;
	IDXGIFactory* factory;

	// create dx graphics interface factory
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		MASSERT_MSG(0, "Failed in create dxgi factory");
		return false;
	}

	// enumerate adapter
	UINT i = 0;
	IDXGIAdapter * adapter;
	DXGI_ADAPTER_DESC adapter_desc;
	while (factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		VideoCardInfo video_card_info;
		memset(&video_card_info, 0, sizeof(VideoCardInfo));

		// get adapter description
		result = adapter->GetDesc(&adapter_desc);
		if (FAILED(result))
		{
			MASSERT_MSG(0, "Failed in get adapter description");
			adapter->Release();
			adapter = nullptr;
			continue;
		}

		// get video card information
		video_card_info.memory_size = (int)(adapter_desc.DedicatedVideoMemory / 1024 / 1024);
		size_t desc_len;
		errno_t error = wcstombs_s(&desc_len, video_card_info.desc, MAX_VideoCardDescLen, adapter_desc.Description, MAX_VideoCardDescLen);
		if (error != 0)
		{
			MASSERT_MSG(0, "Failed in get adapter description string");
			adapter->Release();
			adapter = nullptr;
			continue;
		}
		video_card_info.monitor_num = 0;

		// enumerate adapter output
		UINT j = 0;
		IDXGIOutput * adapter_output;
		DXGI_OUTPUT_DESC adapter_output_desc;
		while (adapter->EnumOutputs(i, &adapter_output) != DXGI_ERROR_NOT_FOUND)
		{
			MonitorInfo monitor_info;
			memset(&monitor_info, 0, sizeof(MonitorInfo));

			// get number of adapter output display mode list
			unsigned int num_modes;
			result = adapter_output->GetDisplayModeList(m_dxgi_format, DXGI_ENUM_MODES_INTERLACED, &num_modes, NULL);
			if (FAILED(result))
			{
				MLOG("## an adapter output can't match current dxgi format");
				adapter_output->Release();
				adapter_output = nullptr;
				continue;
			}

			// create adapter output display mode list
			DXGI_MODE_DESC* display_mode_list = new DXGI_MODE_DESC[num_modes];
			if (!display_mode_list)
			{
				MASSERT_MSG(0, "Failed in create adapter output display mode list");
				adapter_output->Release();
				adapter_output = nullptr;
				continue;
			}

			// get display mode list
			result = adapter_output->GetDisplayModeList(m_dxgi_format, DXGI_ENUM_MODES_INTERLACED, &num_modes, display_mode_list);
			if (FAILED(result))
			{
				MASSERT_MSG(0, "Failed in get adapter output display mode list");
				delete[] display_mode_list;
				display_mode_list = nullptr;
				adapter_output->Release();
				adapter_output = nullptr;
				continue;
			}

			// find one match win width and height 
			for (i = 0; i < num_modes; i++)
			{
				if (display_mode_list[i].Width == (unsigned int)m_width &&
					display_mode_list[i].Height == (unsigned int)m_height)
				{
					monitor_info.refresh_rate_numerator = display_mode_list[i].RefreshRate.Numerator;
					monitor_info.refresh_rate_denominator = display_mode_list[i].RefreshRate.Denominator;
					break;
				}
			}

			// get monitor description
			adapter_output->GetDesc(&adapter_output_desc);
			WideCharToMultiByte(CP_UTF8, 0, adapter_output_desc.DeviceName, -1, monitor_info.name, MG_MAX_PATH, NULL, FALSE);
			monitor_info.width = abs(adapter_output_desc.DesktopCoordinates.right - adapter_output_desc.DesktopCoordinates.left);
			monitor_info.height = abs(adapter_output_desc.DesktopCoordinates.top - adapter_output_desc.DesktopCoordinates.bottom);

			// delete display mode list
			delete[] display_mode_list;
			display_mode_list = nullptr;

			// Release the adapter output
			adapter_output->Release();
			adapter_output = nullptr;

			// store monitor information
			++video_card_info.monitor_num;
			video_card_info.monitors[j] = monitor_info;

			++j;
		}

		// Release the adapter
		adapter->Release();
		adapter = nullptr;

		// store video card information
		m_video_card_infos.push_back(video_card_info);
		++i;
	}

	// Release the factory
	factory->Release();
	factory = nullptr;

	// log all video card information
	if (m_video_card_infos.size() == 0)
	{
		return false;
	}
	MLOG("## Video card information: \n");
	for (i = 0; i < m_video_card_infos.size(); ++i)
	{
		MLOG("## Video card No.%u: \n", i);
		MLOG("## description: %s\n", m_video_card_infos[i].desc);
		MLOG("## memory size: %d\n", m_video_card_infos[i].memory_size);
		MLOG("## connecting monitor number: %d\n", m_video_card_infos[i].monitor_num);
		for (size_t j = 0; j < m_video_card_infos[i].monitor_num; ++j)
		{
			const MonitorInfo& monitor_ref = m_video_card_infos[i].monitors[j];
			MLOG("## monitor No.%u\n", j);
			MLOG("## monitor name: %s\n", monitor_ref.name);
			MLOG("## monitor screen: %d x %d\n", monitor_ref.width, monitor_ref.height);
			MLOG("## monitor refresh rational: %d %d\n", monitor_ref.refresh_rate_numerator, monitor_ref.refresh_rate_denominator);
		}
	}

	return true;
}
bool Renderer_D3D11::InitDeviceAndSwapChain()
{
	HRESULT result;

	// fill out swap chain description
	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	FilloutSwapChainDesc(swap_chain_desc);

	// create device and swap chain
	UINT flags = 0;
#if MUGGLE_DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, &m_feature_level, 1,
		D3D11_SDK_VERSION, &swap_chain_desc, &m_swap_chain, &m_device, NULL, &m_device_context);
	if (FAILED(result))
	{
		MASSERT_MSG(0, "Failed in D3D11CreateDeviceAndSwapChain");
		return false;
	}

	return true;
}
bool Renderer_D3D11::InitBackBuffer()
{
	HRESULT result;
	ID3D11Texture2D* ptr_back_buffer;

	result = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&ptr_back_buffer);
	if (FAILED(result))
	{
		return false;
	}

	result = m_device->CreateRenderTargetView(ptr_back_buffer, NULL, &m_render_target_view);
	if (FAILED(result))
	{
		return false;
	}

	ptr_back_buffer->Release();
	ptr_back_buffer = nullptr;

	return true;
}
bool Renderer_D3D11::InitDepthStencil()
{
	HRESULT result;

	// fill out depth buffer description
	D3D11_TEXTURE2D_DESC depth_buffer_desc;
	FilloutDepthBufferDesc(depth_buffer_desc);

	// create depth stencil buffer
	result = m_device->CreateTexture2D(&depth_buffer_desc, NULL, &m_depth_stencil_buffer);
	if (FAILED(result))
	{
		MASSERT_MSG(0, "Failed in create depth stencil buffer");
		return false;
	}

	// fill out depth stencil description
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
	FilloutDepthStencilDesc(depth_stencil_desc);

	// create depth stencil state
	result = m_device->CreateDepthStencilState(&depth_stencil_desc, &m_depth_stencil_state);
	if (FAILED(result))
	{
		MASSERT_MSG(0, "Failed in create depth stencil state");
		return false;
	}

	// Sets the depth-stencil state of the output-merger stage
	m_device_context->OMSetDepthStencilState(m_depth_stencil_state, 1);

	// fill out depth stencil view description
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
	FilloutDepthStencilViewDesc(depth_stencil_view_desc);

	// create depth stencil view
	result = m_device->CreateDepthStencilView(m_depth_stencil_buffer, &depth_stencil_view_desc, &m_depth_stencil_view);
	if (FAILED(result))
	{
		MASSERT_MSG(0, "Failed in create depth stencil view");
		return false;
	}

	return true;
}
bool Renderer_D3D11::InitRaster()
{
	HRESULT result;
	D3D11_RASTERIZER_DESC raster_desc;
	raster_desc.AntialiasedLineEnable = false;
	raster_desc.CullMode = D3D11_CULL_BACK;
	raster_desc.DepthBias = 0;
	raster_desc.DepthBiasClamp = 0.0f;
	raster_desc.DepthClipEnable = true;
	raster_desc.FillMode = D3D11_FILL_SOLID;
	raster_desc.FrontCounterClockwise = false;
	raster_desc.MultisampleEnable = false;
	raster_desc.ScissorEnable = false;
	raster_desc.SlopeScaledDepthBias = 0.0f;

	result = m_device->CreateRasterizerState(&raster_desc, &m_raster_state);
	if (FAILED(result))
	{
		MASSERT_MSG(0, "Failed in d3d11 CreateRasterizerState");
		return false;
	}

	m_device_context->RSSetState(m_raster_state);

	return true;
}
bool Renderer_D3D11::InitViewport()
{
	D3D11_VIEWPORT viewport;
	viewport.Width = (float)m_width;
	viewport.Height = (float)m_height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	m_device_context->RSSetViewports(1, &viewport);

	return true;
}

void Renderer_D3D11::FilloutSwapChainDesc(DXGI_SWAP_CHAIN_DESC& swap_chain_desc)
{
	ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));
	swap_chain_desc.BufferCount = 1;					// single back buffer
	swap_chain_desc.BufferDesc.Width = m_width;			// win width and height
	swap_chain_desc.BufferDesc.Height = m_height;
	swap_chain_desc.BufferDesc.Format = m_dxgi_format;	// dxgi format

	// refresh rate numerator and denominator
	if (m_vsync && m_video_card_infos.size() > 0 && m_video_card_infos[0].monitor_num > 0)
	{
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = m_video_card_infos[0].monitors[0].refresh_rate_numerator;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = m_video_card_infos[0].monitors[0].refresh_rate_denominator;
	}
	else
	{
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = 0;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	}
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// usage of the back buffer
	swap_chain_desc.OutputWindow = (HWND)m_hWnd;					// window handle
	swap_chain_desc.SampleDesc.Count = 1;							// turn multisampling off
	swap_chain_desc.SampleDesc.Quality = 0;
	// whether or not full screen
	if (m_full_screen)
	{
		swap_chain_desc.Windowed = false;
	}
	else
	{
		swap_chain_desc.Windowed = true;
	}

	// set the scan line ordering and scaling to unspecified
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;			// discard the back buffer contents after presenting
	swap_chain_desc.Flags = 0;										// have no advanced flags
}
void Renderer_D3D11::FilloutDepthBufferDesc(D3D11_TEXTURE2D_DESC& depth_buffer_desc)
{
	ZeroMemory(&depth_buffer_desc, sizeof(D3D11_TEXTURE2D_DESC));
	depth_buffer_desc.Width = m_width;
	depth_buffer_desc.Height = m_height;
	depth_buffer_desc.MipLevels = 1;
	depth_buffer_desc.ArraySize = 1;
	depth_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_buffer_desc.SampleDesc.Count = 1;
	depth_buffer_desc.SampleDesc.Quality = 0;
	depth_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_buffer_desc.CPUAccessFlags = 0;
	depth_buffer_desc.MiscFlags = 0;
}
void Renderer_D3D11::FilloutDepthStencilDesc(D3D11_DEPTH_STENCIL_DESC& depth_stencil_desc)
{
	ZeroMemory(&depth_stencil_desc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depth_stencil_desc.DepthEnable = true;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;
	depth_stencil_desc.StencilEnable = true;
	depth_stencil_desc.StencilReadMask = 0xFF;
	depth_stencil_desc.StencilWriteMask = 0xFF;
	depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
}
void Renderer_D3D11::FilloutDepthStencilViewDesc(D3D11_DEPTH_STENCIL_VIEW_DESC& depth_stencil_view_desc)
{
	ZeroMemory(&depth_stencil_view_desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depth_stencil_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_stencil_view_desc.Texture2D.MipSlice = 0;
}


NS_MUGGLE_END