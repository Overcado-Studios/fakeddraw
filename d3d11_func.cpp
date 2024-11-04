#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <ddraw.h>
#include <d3dcompiler.h>

//#include <d3d.h>

typedef struct _D3DVIEWPORT7 {
    DWORD       dwX;
    DWORD       dwY;            /* Viewport Top left */
    DWORD       dwWidth;
    DWORD       dwHeight;       /* Viewport Dimensions */
    FLOAT		dvMinZ;         /* Min/max of clip Volume */
    FLOAT       dvMaxZ;
} D3DVIEWPORT7, *LPD3DVIEWPORT7;

#include "d3d11_func.h"
#include "nvdebug.h"

#define D3D11_SURF_FLAG_FRONTBUFFER		0x1
#define D3D11_SURF_FLAG_BACKBUFFER		0x2
#define D3D11_SURF_FLAG_TEXTURE			0x4
#define D3D11_SURF_FLAG_ZBUFFER			0x8

ComPtr<ID3D11VertexShader> D3D11Func_CreateVertexShader(D3D11** ppd3d, const std::wstring& fileName, ComPtr<ID3DBlob>& vertexShaderBlob);
ComPtr<ID3D11PixelShader> D3D11Func_CreatePixelShader(D3D11** ppd3d, const std::wstring& fileName);

bool	D3D11Func_ShaderManager_Init(D3D11** ppd3d);
bool	D3D11Func_ShaderManager_Shutdown(D3D11** ppd3d);

bool	D3D11Func_CompileShader(const std::wstring& fileName,
	const std::string& entryPoint,
	const std::string& profile, ComPtr<ID3DBlob>& shaderBlob);
bool	D3D11Func_CreateVertexShaderInputLayout(D3D11** ppd3d);
bool	D3D11Func_CreateVertexBuffer(D3D11** ppd3d);


struct VertexProperties
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 color;
};

struct D3D11
{
	ID3D11Device*			device;
	ID3D11DeviceContext*	context;
	IDXGISwapChain*			swapchain;

	IDirectDraw7*	ddraw;
	GUID*			ddrawguid;
	HWND			hwnd;
	D3D11_VIEWPORT	vp;

	// default (we can refactor pipeline to use this shader later, but for now this stays)
	ComPtr<ID3D11VertexShader> testVS = nullptr;
	ComPtr<ID3D11PixelShader> testPS = nullptr;
	ComPtr<ID3DBlob> testVSBlob = nullptr;
	ComPtr<ID3D11InputLayout> testInputLayout = nullptr;
	ComPtr<ID3D11Buffer> testTriangleVertices = nullptr;
};

struct D3D11Surface
{
	ID3D11Texture2D*		texture;
	IDXGISurface1*			surface;
	ID3D11RenderTargetView*	rtv;
	D3D11*					parent_context;
	DWORD					flags;
	DDSURFACEDESC2			ddsd;
};

HRESULT D3D11Func_Initialize( D3D11** ppd3d )
{
	HRESULT hr;

	if( !ppd3d )
		return E_INVALIDARG;

	/* Allocate context struct */
	(*ppd3d) = new D3D11;
	if( !(*ppd3d) )
		return E_OUTOFMEMORY;

	/* Use the real IDirectDraw7 interface for things related to vblank timing */
	/* TODO: We could probably use WDDM directly instead, but for now this should work */
	/* TODO: Use user supplied GUID */
	hr = DirectDrawCreateEx( /*(*ppd3d)->ddrawguid*/ NULL, (void**) &(*ppd3d)->ddraw, IID_IDirectDraw7, NULL );
	if( FAILED( hr ) )
		return hr;

	return S_OK;
}

void D3D11Func_Uninitialize( D3D11** ppd3d )
{
	/* Uninitialize real DirectDraw7 */
	if( (*ppd3d)->ddraw )
		(*ppd3d)->ddraw->Release();

	delete (*ppd3d);
}

HRESULT D3D11Func_CreateDevice( D3D11** ppd3d, HWND hwnd )
{
	HRESULT hr;

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    /* What we're going to do here is initialize the D3D11 device, context and swapchain.  
	   Notice how we're not immediately setting a resolution of some sort because eventually
	   fake DirectDraw will specify these values so we will defer such until then. */

	DWORD flags = D3D11_CREATE_DEVICE_DEBUG;

    scd.BufferCount = 1; 
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;     
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      
    scd.OutputWindow = hwnd;                                
    scd.SampleDesc.Count = 1;                               
    scd.Windowed = TRUE; 
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE | DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    hr = D3D11CreateDeviceAndSwapChain( NULL,
                                  D3D_DRIVER_TYPE_HARDWARE,
                                  NULL,
                                  flags,
                                  NULL,
                                  NULL,
                                  D3D11_SDK_VERSION,
                                  &scd,
                                  &(*ppd3d)->swapchain,
                                  &(*ppd3d)->device,
                                  NULL,
                                  &(*ppd3d)->context );
	if( FAILED( hr ) )
		DISPDBG_FP( 0, "D3D11CreateDeviceAndSwapChain() returned 0x"<<std::hex<<hr );

	return hr;
}

void D3D11Func_ReleaseDevice( D3D11** ppd3d )
{
	if( (*ppd3d)->swapchain )
	{
		(*ppd3d)->swapchain->SetFullscreenState( FALSE, NULL );		/* MUST be in windowed mode or else releasing the swapchain will crash */
		(*ppd3d)->swapchain->Release();
	}
	if( (*ppd3d)->device )
		(*ppd3d)->device->Release();
	if( (*ppd3d)->context )
		(*ppd3d)->context->Release();

	D3D11Func_ShaderManager_Shutdown(ppd3d);
}

// create a simple pipeline
bool D3D11Func_InitializeShaderSystem(D3D11** ppd3d)
{
	bool bRet = D3D11Func_ShaderManager_Init(ppd3d);
	if (!bRet)
		return false;

	bRet = D3D11Func_CreateVertexShaderInputLayout(ppd3d);
	if (!bRet)
		return false;

	bRet = D3D11Func_CreateVertexBuffer(ppd3d);
	if (!bRet)
		return false;

	return false;
}

bool D3D11Func_ShaderManager_Init( D3D11** ppd3d )
{
	(*ppd3d)->testVS = D3D11Func_CreateVertexShader(ppd3d, L"C:\\Projects\\Overcado\\laghaim-front-end\\fakeddraw\\shaders\\Main.vs.hlsl", (*ppd3d)->testVSBlob);

	if ((*ppd3d)->testVS == nullptr)
	{
		return false;
	}

	(*ppd3d)->testPS = D3D11Func_CreatePixelShader(ppd3d, L"C:\\Projects\\Overcado\\laghaim-front-end\\fakeddraw\\shaders\\Main.ps.hlsl");
	if ((*ppd3d)->testVS == nullptr)
	{
		return false;
	}

	return true;
}

bool D3D11Func_ShaderManager_Shutdown(D3D11** ppd3d)
{
	if ((*ppd3d)->testVS)
	{
		(*ppd3d)->testVS->Release();
	}

	if ((*ppd3d)->testPS)
	{
		(*ppd3d)->testPS->Release();
	}

	if ((*ppd3d)->testVSBlob)
	{
		(*ppd3d)->testVSBlob->Release();
	}

	return true;
}

bool D3D11Func_CompileShader(const std::wstring& fileName,
	const std::string& entryPoint,
	const std::string& profile, ComPtr<ID3DBlob>& shaderBlob)
{
	constexpr UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	ComPtr<ID3DBlob> tempShaderBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;

	HRESULT hr = D3DCompileFromFile(fileName.data(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint.data(), profile.data(), compileFlags, 0, &tempShaderBlob, &errorBlob);

	if (FAILED(hr))
	{
		std::cout << "D3D11: Failed to read shader from file\n";
		if (errorBlob != nullptr)
		{
			std::cout << "D3D11: With message: " <<
				static_cast<const char*>(errorBlob->GetBufferPointer()) << "\n";
		}

		return false;
	}

	shaderBlob = std::move(tempShaderBlob);
	return true;
}


bool D3D11Func_CreateVertexShaderInputLayout(D3D11** ppd3d)
{
	constexpr D3D11_INPUT_ELEMENT_DESC vertexInputLayoutInfo[] = {
		{
			"POSITION",
			0,
			DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			offsetof(VertexProperties, position),
			D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"COLOR",
			0,
			DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			offsetof(VertexProperties,    color),
			D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
	};

	HRESULT hr = (*ppd3d)->device->CreateInputLayout(
		vertexInputLayoutInfo,
		_countof(vertexInputLayoutInfo),
		(*ppd3d)->testVSBlob->GetBufferPointer(),
		(*ppd3d)->testVSBlob->GetBufferSize(),
		&(*ppd3d)->testInputLayout);

	if (FAILED(hr))
	{
		DISPDBG_FP(0, "ERROR: D3D11: Failed to create default vertex input layout" << std::hex << hr);
		return false;
	}

	return true;
}

bool D3D11Func_CreateVertexBuffer(D3D11** ppd3d)
{
	constexpr VertexProperties vertices[] = {
	{  DirectX::XMFLOAT3{ 0.0f, 0.5f, 0.0f }, DirectX::XMFLOAT3{ 0.25f, 0.39f, 0.19f }},
	{  DirectX::XMFLOAT3{ 0.5f, -0.5f, 0.0f }, DirectX::XMFLOAT3{ 0.44f, 0.75f, 0.35f }},
	{  DirectX::XMFLOAT3{ -0.5f, -0.5f, 0.0f }, DirectX::XMFLOAT3{ 0.38f, 0.55f, 0.20f }},
	};
	D3D11_BUFFER_DESC bufferInfo = {};
	bufferInfo.ByteWidth = sizeof(vertices);
	bufferInfo.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE;
	bufferInfo.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA resourceData = {};
	resourceData.pSysMem = vertices;

	HRESULT hr = (*ppd3d)->device->CreateBuffer(
		&bufferInfo,
		&resourceData,
		&(*ppd3d)->testTriangleVertices);

	if (FAILED(hr))
	{
		DISPDBG_FP(0, "ERROR: D3D11: Failed to create triangle vertex buffer" << std::hex << hr);
		return false;
	}

	return true;
}

ComPtr<ID3D11VertexShader> D3D11Func_CreateVertexShader(D3D11** ppd3d, const std::wstring& fileName, ComPtr<ID3DBlob>& vertexShaderBlob)
{
	if (!D3D11Func_CompileShader(fileName, "Main", "vs_5_0", vertexShaderBlob))
	{
		return nullptr;
	}

	ComPtr<ID3D11VertexShader> vertexShader;

	HRESULT hr = (*ppd3d)->device->CreateVertexShader(
		vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize(),
		nullptr,
		&vertexShader);

	if (FAILED(hr))
	{
		DISPDBG_FP(0, "ERROR: D3D11: Failed to compile vertex shader" << std::hex << hr);
		return nullptr;
	}

	return vertexShader;
}

ComPtr<ID3D11PixelShader> D3D11Func_CreatePixelShader(D3D11** ppd3d, const std::wstring& fileName)
{
	ComPtr<ID3DBlob> pixelShaderBlob = nullptr;
	if (!D3D11Func_CompileShader(fileName, "Main", "ps_5_0", pixelShaderBlob))
	{
		return nullptr;
	}

	ComPtr<ID3D11PixelShader> pixelShader;

	HRESULT hr = (*ppd3d)->device->CreatePixelShader(
		pixelShaderBlob->GetBufferPointer(),
		pixelShaderBlob->GetBufferSize(),
		nullptr,
		&pixelShader);

	if (FAILED(hr))
	{
		DISPDBG_FP(0, "ERROR: D3D11: Failed to compile pixel shader" << std::hex << hr);
		return nullptr;
	}

	return pixelShader;
}

HRESULT D3D11Func_SetDisplayMode( D3D11* d3d, int width, int height, int bpp, int refresh_rate, int fullscreen )
{
	HRESULT hr;
	DXGI_MODE_DESC dmd;

#if 1 // Force windowed mode for testing purposes
	fullscreen = 0;
#endif

	dmd.Width = width;
	dmd.Height = height;
	dmd.RefreshRate.Numerator = refresh_rate;
	dmd.RefreshRate.Denominator = 1;
	dmd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	dmd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	dmd.Format  =DXGI_FORMAT_B8G8R8A8_UNORM;

	hr = d3d->swapchain->SetFullscreenState( fullscreen, NULL );
	if( FAILED( hr ) ) return hr;

	hr = d3d->swapchain->ResizeTarget( &dmd );
	if( FAILED( hr ) ) return hr;

#if 1
	hr = d3d->swapchain->ResizeBuffers( 1 /* We're assuming for now */,
									width, height, 
									DXGI_FORMAT_B8G8R8A8_UNORM,	/* TODO: Simulate anything lower than 32-bit? */
									DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE |/* Mode switching and GDI compatibility are a must */
									( fullscreen ? /*DXGI_SWAP_CHAIN_FLAG_FULLSCREEN_VIDEO*/0 : 0 ) );
#endif

	return hr;
}

HRESULT D3D11Func_WaitForVerticalBlank( D3D11* d3d, DWORD dwFlags )
{
	return d3d->ddraw->WaitForVerticalBlank( dwFlags, NULL );
}

HRESULT D3D11Func_GetAvailableVidMem( D3D11* d3d, LPDDSCAPS2 lpDDSCaps2, DWORD* dwTotal, DWORD* dwFree )
{
	/* We have two viable options: Use DDraw directly or actually use DX11.  The case for using the latter
	   is the possibility of the former giving inaccurate results (which I have seen before) but */
#if 1
	return d3d->ddraw->GetAvailableVidMem( lpDDSCaps2, dwTotal, dwFree );
#else
	IDXGIFactory4* pFactory;
	HRESULT hr = CreateDXGIFactory1( __uuidof(IDXGIFactory4), (void**)&pFactory );
	if( FAILED( hr ) )
		return hr;

	/* TODO: Use the actual adapter number instead of defaulting to the default display! */
	IDXGIAdapter3* adapter;
	hr = pFactory->EnumAdapters( 0, reinterpret_cast<IDXGIAdapter**>(&adapter) );
	if( FAILED( hr ) )
	{
		pFactory->Release();
		return hr;
	}

	DXGI_QUERY_VIDEO_MEMORY_INFO videoMemoryInfo = {0};
	hr = adapter->QueryVideoMemoryInfo( 0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &videoMemoryInfo );

	*dwFree = videoMemoryInfo.Budget - videoMemoryInfo.CurrentUsage;
	*dwTotal = videoMemoryInfo.Budget;

	if( pFactory ) pFactory->Release();
	if( adapter ) adapter->Release();

	return hr;
#endif
}

HRESULT D3D11Func_GetScanLine( D3D11* d3d, LPDWORD scanline )
{
	return d3d->ddraw->GetScanLine( scanline );
}

HRESULT D3D11Func_GetVerticalBlankStatus( D3D11* d3d, LPBOOL lpbIsInVB )
{
	return d3d->ddraw->GetVerticalBlankStatus( lpbIsInVB );
}

HRESULT D3D11Func_LazyPresent( D3D11* d3d, D3D11Surface* src, D3D11Surface* dst )
{
	if( ( src->flags & D3D11_SURF_FLAG_BACKBUFFER ) && ( dst->flags & D3D11_SURF_FLAG_FRONTBUFFER ) )
		return D3D11Func_Present( d3d );

	return E_FAIL;
}

HRESULT D3D11Func_Present( D3D11* d3d )
{
	return d3d->swapchain->Present( 0, 0 );
}

HRESULT D3D11Func_CreateSurface( D3D11* d3d, D3D11Surface** ppsurface, DDSURFACEDESC2* pddsd )
{
	if( !pddsd )
		return E_INVALIDARG;

	if( pddsd->dwFlags & DDSD_CAPS )
	{
		(*ppsurface) = new D3D11Surface;
		if( !(*ppsurface) )
			return E_OUTOFMEMORY;

		memset( (*ppsurface), 0, sizeof( D3D11Surface ) );

		/* We'll need this later */
		memmove( &(*ppsurface)->ddsd, pddsd, sizeof( DDSURFACEDESC2 ) );

		/* Frontbuffer creation */
		if( pddsd->ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE )
		{
			(*ppsurface)->flags |= D3D11_SURF_FLAG_FRONTBUFFER;

			/* For now, that's pretty much it.  D3D11 doesn't give us many options for getting access to the 
			   frontbuffer plus it's not commonly needed besides initializing the doublebuffer anyway... */

			//return DD_OK;
		}
		else if( pddsd->ddsCaps.dwCaps & DDSCAPS_BACKBUFFER || /* Fullscreen */
			( pddsd->ddsCaps.dwCaps & DDSCAPS_OFFSCREENPLAIN && pddsd->ddsCaps.dwCaps & DDSCAPS_3DDEVICE ) ) /* Windowed */
		{
			(*ppsurface)->flags |= D3D11_SURF_FLAG_BACKBUFFER;

			HRESULT hr = d3d->swapchain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (void**) &(*ppsurface)->texture );
			if( FAILED( hr ) ) { DISPDBG_FP( 0, "ERROR: IDXGISwapChain::GetBuffer() returned"<<std::hex<<hr ); return hr; }

			hr = d3d->device->CreateRenderTargetView( (*ppsurface)->texture, NULL, &(*ppsurface)->rtv );
			if( FAILED( hr ) ) { DISPDBG_FP( 0, "ERROR: ID3D11Device::CreateRenderTargetView() returned"<<std::hex<<hr ); return hr; }

			hr = d3d->swapchain->GetBuffer( 0, __uuidof(IDXGISurface1), (void**)& (*ppsurface)->surface );
			if( FAILED( hr ) ) { DISPDBG_FP( 0, "ERROR: IDXGISwapChain::GetBuffer() returned"<<std::hex<<hr ); return hr; }

			d3d->context->OMSetRenderTargets( 1, &(*ppsurface)->rtv, NULL );

			//return DD_OK;

			/*if( SUCCEEDED( hr ) )
			{
				hr = g_pSurface1->GetDC(FALSE, &hdc);
				if (SUCCEEDED(hr))
				{
					TextOut(hdc, 50, 50, "GDI Rendering Works!", 20);
					g_pSurface1->ReleaseDC(NULL);
				}
				SafeRelease(g_pSurface1);
			}
			g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);*/
		}
		else if( pddsd->ddsCaps.dwCaps & DDSCAPS_OFFSCREENPLAIN )
		{
			(*ppsurface)->flags |= D3D11_SURF_FLAG_TEXTURE;
		}
		else if( pddsd->ddsCaps.dwCaps & DDSCAPS_ZBUFFER )
		{
			(*ppsurface)->flags |= D3D11_SURF_FLAG_ZBUFFER;
		}
		else
			return E_INVALIDARG;

		return DD_OK;
	}

	return E_INVALIDARG;
}

HRESULT D3D11Func_DeleteSurface( D3D11Surface** ppsurface )
{
	//if( (*ppsurface)->parent_context )
	//	delete (*ppsurface)->parent_context;

	if( (*ppsurface)->rtv )
		(*ppsurface)->rtv->Release();

	if( (*ppsurface)->surface )
		(*ppsurface)->surface->Release();

	if( (*ppsurface)->texture )
		(*ppsurface)->texture->Release();

	delete (*ppsurface);

	return S_OK;
}

HRESULT D3D11Func_SetViewport( D3D11* d3d, D3DVIEWPORT7* vp )
{
	D3D11_VIEWPORT vp11;
	vp11.TopLeftX = vp->dwX;
	vp11.TopLeftY = vp->dwY;
	vp11.Width = vp->dwWidth;
	vp11.Height = vp->dwHeight;
	vp11.MinDepth = vp->dvMinZ;
	vp11.MaxDepth = vp->dvMaxZ;

	d3d->context->RSSetViewports( 1, &vp11 );

	return S_OK; 
}

HRESULT D3D11Func_SetViewport2(D3D11* d3d)
{
	D3D11_VIEWPORT vp11;
	vp11.TopLeftX = 0;
	vp11.TopLeftY = 0;
	vp11.Width = 640;
	vp11.Height = 480;
	vp11.MinDepth = 0;
	vp11.MaxDepth = 1;

	d3d->context->RSSetViewports(1, &vp11);

	return S_OK;
}

HRESULT D3D11Func_ClearRT( D3D11* d3d, DWORD dwColour )
{
	float fC[4];
	fC[0] = float((dwColour>>16)&0xFF)/255.0f;
	fC[1] = float((dwColour>>8)&0xFF)/255.0f;
	fC[2] = float((dwColour)&0xFF)/255.0f;
	fC[3] = float((dwColour>>24)&0xFF)/255.0f;

	ID3D11RenderTargetView* rtv = nullptr;

	d3d->context->OMGetRenderTargets( 1, &rtv, NULL );
	
	if( rtv )
	{
		d3d->context->ClearRenderTargetView( rtv, fC );
		rtv->Release();
	}
	else
		return E_FAIL;

	return S_OK;
}

HRESULT D3D11Func_ClearDS( D3D11* d3d, DWORD flag, float z, DWORD stencil )
{
	ID3D11DepthStencilView* dsv = nullptr;

	d3d->context->OMGetRenderTargets( 1, NULL, &dsv );

	if( dsv )
	{
		d3d->context->ClearDepthStencilView( dsv, (D3D11_CLEAR_FLAG) flag, z, stencil );
		dsv->Release();

		return S_OK;
	}

	return E_FAIL;
}

HRESULT D3D11Func_GetDisplayMode( D3D11* d3d, DDSURFACEDESC2* pddsd )
{
	/* TODO: Might need to do more here... */
	return d3d->ddraw->GetDisplayMode( pddsd );
}

HRESULT D3D11Func_SetInputLayout(D3D11* d3d)
{
	d3d->context->IASetInputLayout(d3d->testInputLayout.Get());

	return S_OK;
}

HRESULT D3D11Func_SetVertexBuffers(D3D11* d3d)
{
	constexpr UINT vertexStride = sizeof(VertexProperties);
	constexpr UINT vertexOffset = 0;

	d3d->context->IASetVertexBuffers(
		0,
		1,
		d3d->testTriangleVertices.GetAddressOf(),
		&vertexStride,
		&vertexOffset);

	d3d->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return S_OK;
}

HRESULT D3D11Func_SetVertexShader(D3D11* d3d)
{
	d3d->context->VSSetShader(
		d3d->testVS.Get(),
		nullptr,
		0);

	return S_OK;
}

HRESULT D3D11Func_SetPixelShader(D3D11* d3d)
{
	d3d->context->PSSetShader(
		d3d->testPS.Get(),
		nullptr,
		0);

	return S_OK;
}

HRESULT D3D11Func_Draw(D3D11* d3d)
{
	d3d->context->Draw(3, 0);

	return E_NOTIMPL;
}


HRESULT D3D11Func_SetRenderTarget(D3D11* d3d, D3D11Surface** ppsurface)
{
	d3d->context->OMSetRenderTargets(1, &(*ppsurface)->rtv, NULL);

	return S_OK;
}

/***********************************************************\
 *            Direct3D11 Surface functionality             *
\***********************************************************/
HRESULT D3D11SurfaceFunc_GetDC( D3D11Surface* surface, HDC* pDC )
{
	return (surface)->surface->GetDC( FALSE, pDC );
}

HRESULT D3D11SurfaceFunc_ReleaseDC( D3D11Surface* surface, HDC* pDC ) 
{
	return surface->surface->ReleaseDC( NULL );
}

HRESULT D3D11SurfaceFunc_GetParentContext( D3D11Surface* surface, D3D11** ppd3d )
{
	(*ppd3d) = surface->parent_context;

	return S_OK;
}

HRESULT D3D11SurfaceFunc_Blt( D3D11* d3d, D3D11Surface* surface,  LPRECT lpDestRect, LPRECT lpSrcRect, DWORD dwFlags, LPDDBLTFX lpDDBltFx )
{
	/* For back surfaces */
	if( surface->ddsd.ddsCaps.dwCaps & DDSCAPS_BACKBUFFER )
	{
		if( !lpDDBltFx )
			return E_FAIL;

		if( dwFlags & DDBLT_COLORFILL )
		{
			DWORD dwC = lpDDBltFx->dwFillColor;

			float fC[4];
			fC[0] = float((dwC>>16)&0xFF)/255.0f;
			fC[1] = float((dwC>>8)&0xFF)/255.0f;
			fC[2] = float((dwC)&0xFF)/255.0f;
			fC[3] = float((dwC>>24)&0xFF)/255.0f;

			/* TODO: lpDestRect can be utilized with ID3D11DeviceContext1::ClearView */

			d3d->context->ClearRenderTargetView( surface->rtv, fC );

			return DD_OK;
		}
	}

	/* Depth buffers are kind of a special case... */
	else if( surface->ddsd.ddsCaps.dwCaps & DDSCAPS_ZBUFFER )
	{
		return E_FAIL;
	}

	/* Do a normal blit */
	else if( surface->ddsd.ddsCaps.dwCaps & DDSCAPS_OFFSCREENPLAIN )
	{
		
	}

	return E_FAIL;
}