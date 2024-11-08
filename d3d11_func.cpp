#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <ddraw.h>
#include <d3dcompiler.h>
#include <vector>

//#include <d3d.h>

// forward declarations
struct D3D11Pipeline;
enum PipelineStage;
struct VertexProperties;
struct D3D11Surface;

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

// some important enumerations
enum
{
	SAMPLER_STATE_COUNT = D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT,
	RESOURCE_STATE_COUNT = D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT,
	CONSTANT_BUFFER_COUNT = D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNT
};

enum PipelineStage
{
	PIPELINE_STAGE_VERTEX,
	PIPELINE_STAGE_PIXEL,
	PIPELINE_STAGE_COMPUTE,
	PIPELINE_STAGE_COUNT
};

struct VertexProperties
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 color;
	DirectX::XMFLOAT2 uv;
};

struct D3D11Surface
{
	ID3D11Texture2D* texture;
	IDXGISurface1* surface;
	ID3D11RenderTargetView* rtv;
	ID3D11ShaderResourceView* srv;
	D3D11* parent_context;
	DWORD						flags;
	DDSURFACEDESC2				ddsd;
};

enum PipelineShaderType
{
	PIPELINE_SHADER_FILE,		// shader came from vs.hlsl / ps.hlsl
	PIPELINE_SHADER_SRC,		// shader came from an ascii string
	PIPELINE_SHADER_BLOB		// shader precompiled
};

struct D3D11Pipeline
{
	ComPtr<ID3D11VertexShader> vertexShader = nullptr;
	ComPtr<ID3D11PixelShader> pixelShader = nullptr;
	ComPtr<ID3DBlob> vertexShaderBlob = nullptr;
	ComPtr<ID3DBlob> pixelShaderBlob = nullptr;
	ComPtr<ID3D11InputLayout> inputLayout = nullptr;
	ComPtr<ID3D11Buffer> vertexBuffer = nullptr;

	int	 vertexCount;

	ComPtr<ID3D11SamplerState>				samplerStates[PIPELINE_STAGE_COUNT][SAMPLER_STATE_COUNT];
	ComPtr<ID3D11ShaderResourceView>		resourceStates[PIPELINE_STAGE_COUNT][RESOURCE_STATE_COUNT];		// make this abit better to handle non-texture resources?
	ComPtr<ID3D11Buffer>					constantBuffers[PIPELINE_STAGE_COUNT][RESOURCE_STATE_COUNT];	
};

struct D3D11
{
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	IDXGISwapChain* swapchain;

	IDirectDraw7* ddraw;
	GUID* ddrawguid;
	HWND			hwnd;
	D3D11_VIEWPORT	vp;

	D3D11Pipeline	defaultBlitPipeline;

	// Default Samplers
	ComPtr<ID3D11SamplerState> linearSamplerState;
};


struct ShaderConstants
{
	DirectX::XMFLOAT4 tint;				
	DirectX::XMFLOAT4 sampleParameters; // xy width height in normalized space
};

// Private Member Functions
ComPtr<ID3D11VertexShader> D3D11Func_CreateVertexShader(D3D11** ppd3d, const std::wstring& pData, ComPtr<ID3DBlob>& vertexShaderBlob, PipelineShaderType type);
ComPtr<ID3D11PixelShader> D3D11Func_CreatePixelShader(D3D11** ppd3d, const std::wstring& pData, ComPtr<ID3DBlob>& pixelShaderBlob, PipelineShaderType type);


HRESULT D3D11Func_InitPipelineShaders(D3D11** ppd3d, D3D11Pipeline* pipeline, const std::wstring vertexShaderName, const std::wstring pixelShaderName, PipelineShaderType type);
HRESULT D3D11func_CreateDefaultSamplers(D3D11* d3d);
bool	D3D11Func_ShaderManager_Init(D3D11** ppd3d);
bool	D3D11Func_ShaderManager_Shutdown(D3D11** ppd3d);
bool	D3D11Func_ShutdownPipelineResources(D3D11** ppd3d, D3D11Pipeline* pipeline);
bool	D3D11Func_CompileShader(const std::wstring& fileName, const std::string& entryPoint, const std::string& profile, ComPtr<ID3DBlob>& shaderBlob);
bool	D3D11Func_CreateVertexShaderInputLayout(D3D11** ppd3d, D3D11Pipeline* pipeline);
bool	D3D11Func_CreateVertexBuffer(D3D11** ppd3d, D3D11Pipeline* pipeline, VertexProperties* vertices, int vertexCount);

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
	D3D11Func_ShutdownPipelineResources(ppd3d, &(*ppd3d)->defaultBlitPipeline);

	if( (*ppd3d)->swapchain )
	{
		(*ppd3d)->swapchain->SetFullscreenState( FALSE, NULL );		/* MUST be in windowed mode or else releasing the swapchain will crash */
		(*ppd3d)->swapchain->Release();
	}
	if( (*ppd3d)->device )
		(*ppd3d)->device->Release();
	if( (*ppd3d)->context )
		(*ppd3d)->context->Release();

}

// create a simple pipeline
bool D3D11Func_InitializeShaderSystem(D3D11** ppd3d)
{
	bool bRet = D3D11Func_ShaderManager_Init(ppd3d);
	if (!bRet)
		return false;

	return false;
}

HRESULT D3D11Func_InitPipelineShaders(D3D11** ppd3d, D3D11Pipeline* pipeline, const std::wstring vertexShaderName, const std::wstring pixelShaderName, PipelineShaderType type)
{
	if (!pipeline->vertexShader)
	{
		pipeline->vertexShader = D3D11Func_CreateVertexShader(ppd3d, vertexShaderName, pipeline->vertexShaderBlob, type);
		if (pipeline->vertexShader == nullptr)
		{
			return E_FAIL;
		}
	}
	
	if (!pipeline->pixelShader)
	{
		pipeline->pixelShader = D3D11Func_CreatePixelShader(ppd3d, pixelShaderName, pipeline->pixelShaderBlob, type);
		if (pipeline->pixelShader == nullptr)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

bool D3D11Func_ShutdownPipelineResources(D3D11** ppd3d, D3D11Pipeline* pipeline)
{
	if (pipeline->vertexShader)
	{
		pipeline->vertexShader->Release();
	}

	if (pipeline->pixelShader)
	{
		pipeline->pixelShader->Release();
	}

	if (pipeline->vertexShaderBlob)
	{
		pipeline->vertexShaderBlob->Release();
	}

	if (pipeline->pixelShaderBlob)
	{
		pipeline->pixelShaderBlob->Release();
	}

	if (pipeline->inputLayout)
	{
		pipeline->inputLayout->Release();
	}

	if (pipeline->vertexBuffer)
	{
		pipeline->vertexBuffer->Release();
	}

	for (int i = 0; i < PIPELINE_STAGE_COUNT; i++)
	{
		for (int j = 0; j < SAMPLER_STATE_COUNT; j++)
		{
			ComPtr<ID3D11SamplerState> obj = pipeline->samplerStates[i][j];

			if (obj)
			{
				obj->Release();
			}
		}

		for (int j = 0; j < RESOURCE_STATE_COUNT; j++)
		{
			ComPtr<ID3D11ShaderResourceView> obj = pipeline->resourceStates[i][j];

			if (obj)
			{
				obj->Release();
			}
		}

		for (int j = 0; j < CONSTANT_BUFFER_COUNT; j++)
		{
			ComPtr<ID3D11Buffer> obj = pipeline->constantBuffers[i][j];

			if (obj)
			{
				obj->Release();
			}
		}
	}

	return true;
}

bool D3D11Func_ShaderManager_Init( D3D11** ppd3d )
{
	HRESULT hr = D3D11func_CreateDefaultSamplers(*ppd3d);

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool D3D11Func_ShaderManager_Shutdown(D3D11** ppd3d)
{
	return true;
}

bool D3D11Func_CompileShader(const std::wstring& src,
	const std::string& entryPoint,
	const std::string& profile, ComPtr<ID3DBlob>& shaderBlob, PipelineShaderType type)
{
	if (type == PIPELINE_SHADER_BLOB && shaderBlob)
	{
		// we already have precompiled blob, no need to run the compilation process below.
		return true;
	}
	else if (type == PIPELINE_SHADER_BLOB && !shaderBlob)
	{
		DISPDBG_FP(0, "ERROR: D3D11Func_CompileShader() error: CompileShader called with PIPELINE_SHADER_BLOB, but no data provided.");
		return false;
	}

	constexpr UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	ComPtr<ID3DBlob> tempShaderBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;

	HRESULT hr = S_OK;

	switch (type)
	{
	case PIPELINE_SHADER_FILE:
		hr = D3DCompileFromFile(src.data(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint.data(), profile.data(), compileFlags, 0, &tempShaderBlob, &errorBlob);
		break;
	case PIPELINE_SHADER_SRC:
		hr = D3DCompile(src.data(), src.size(), nullptr, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint.data(), profile.data(), compileFlags, 0, &tempShaderBlob, &errorBlob);
		break;
	};

	if (FAILED(hr))
	{
		DISPDBG_FP(0, "ERROR: D3DCompile() returned" << std::hex << hr);
	}

	if (errorBlob)
	{
		const char* errorMsg = (const char*)errorBlob->GetBufferPointer();
		DISPDBG_FP(0, "Shader Compilation error message : \n" << errorMsg);
	}

	if (FAILED(hr) || errorBlob)
	{
		return false;
	}

	shaderBlob = std::move(tempShaderBlob);
	return true;
}



bool D3D11Func_CreateVertexShaderInputLayout(D3D11** ppd3d, D3D11Pipeline* pipeline)
{
	if (pipeline->inputLayout)
	{
		return true;
	}

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
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,
			0,
			offsetof(VertexProperties,    uv),
			D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
	};

	HRESULT hr = (*ppd3d)->device->CreateInputLayout(
		vertexInputLayoutInfo,
		_countof(vertexInputLayoutInfo),
		pipeline->vertexShaderBlob->GetBufferPointer(),
		pipeline->vertexShaderBlob->GetBufferSize(),
		&pipeline->inputLayout);

	if (FAILED(hr))
	{
		DISPDBG_FP(0, "ERROR: D3D11: Failed to create default vertex input layout" << std::hex << hr);
		return false;
	}

	return true;
}

HRESULT D3D11Func_UpdateConstantBuffer(D3D11** ppd3d, D3D11Pipeline* pipeline, int slot, PipelineStage stage, void* pData, size_t size)
{
	if (!pipeline->constantBuffers[stage][slot])
	{
		DISPDBG_FP(0, "ERROR: D3D11: Tried to update null constant buffer");
		return E_FAIL;
	}

	D3D11_MAPPED_SUBRESOURCE mapped;
	HRESULT hr = (*ppd3d)->context->Map(pipeline->constantBuffers[stage][slot].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	if (FAILED(hr)) { DISPDBG_FP(0, "ERROR: ID3D11DeviceContext::Map() returned" << std::hex << hr); return hr; }

	memcpy(mapped.pData, pData, size);

	(*ppd3d)->context->Unmap(pipeline->constantBuffers[stage][slot].Get(), 0);
}

HRESULT D3D11Func_CreateConstantBuffer(D3D11** ppd3d, D3D11Pipeline * pipeline, int slot, PipelineStage stage, void* pInitialData, size_t size)
{
	// already initialized
	if (pipeline->constantBuffers[stage][slot])
	{
		return S_OK;
	}

	assert(size % 16 == 0);

	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = (UINT)size;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	if (pInitialData)
	{
		D3D11_SUBRESOURCE_DATA resourceData = {};
		resourceData.pSysMem = pInitialData;

		HRESULT hr = (*ppd3d)->device->CreateBuffer(&desc, &resourceData, &pipeline->constantBuffers[stage][slot]);
		if (FAILED(hr) || !pipeline->constantBuffers[stage][slot]) { DISPDBG_FP(0, "ERROR: ID3D11Device::CreateBuffer() returned" << std::hex << hr); return hr; }
	}
	else
	{
		HRESULT hr = (*ppd3d)->device->CreateBuffer(&desc, nullptr, &pipeline->constantBuffers[stage][slot]);
		if (FAILED(hr) || !pipeline->constantBuffers[stage][slot]) { DISPDBG_FP(0, "ERROR: ID3D11Device::CreateBuffer() returned" << std::hex << hr); return hr; }
	}
	

	return S_OK;
}

HRESULT D3D11Func_BindConstantBuffers(D3D11* d3d, D3D11Pipeline* pipeline, PipelineStage pipelineStage, int count)
{
	switch (pipelineStage)
	{
	case PIPELINE_STAGE_VERTEX:
		d3d->context->VSSetConstantBuffers(0, count, pipeline->constantBuffers[pipelineStage]->GetAddressOf());
		break;
	case PIPELINE_STAGE_PIXEL:
		d3d->context->PSSetConstantBuffers(0, count, pipeline->constantBuffers[pipelineStage]->GetAddressOf());
		break;
	case PIPELINE_STAGE_COMPUTE:
		d3d->context->CSSetConstantBuffers(0, count, pipeline->constantBuffers[pipelineStage]->GetAddressOf());
		break;
	}

	return S_OK;
}

bool D3D11Func_CreateVertexBuffer(D3D11** ppd3d, D3D11Pipeline* pipeline, VertexProperties* vertices, int vertexCount)
{
	if (pipeline->vertexBuffer)
	{
		return true;
	}

	if (!vertices)
	{
		return false;
	}

	D3D11_BUFFER_DESC bufferInfo = {};
	bufferInfo.ByteWidth = sizeof(*vertices) * vertexCount;
	bufferInfo.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE;
	bufferInfo.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA resourceData = {};
	resourceData.pSysMem = vertices;

	HRESULT hr = (*ppd3d)->device->CreateBuffer(
		&bufferInfo,
		&resourceData,
		&pipeline->vertexBuffer);

	pipeline->vertexCount = vertexCount;

	if (FAILED(hr))
	{
		DISPDBG_FP(0, "ERROR: D3D11: Failed to create triangle vertex buffer" << std::hex << hr);
		return false;
	}

	return true;
}

ComPtr<ID3D11VertexShader> D3D11Func_CreateVertexShader(D3D11** ppd3d, const std::wstring& pData, ComPtr<ID3DBlob>& vertexShaderBlob, PipelineShaderType type)
{
	if (!D3D11Func_CompileShader(pData, "Main", "vs_5_0", vertexShaderBlob, type))
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
		DISPDBG_FP(0, "ERROR: D3D11: Failed to create vertex shader" << std::hex << hr);
		return nullptr;
	}

	return vertexShader;
}

ComPtr<ID3D11PixelShader> D3D11Func_CreatePixelShader(D3D11** ppd3d, const std::wstring& pData, ComPtr<ID3DBlob>& pixelShaderBlob, PipelineShaderType type)
{
	if (!D3D11Func_CompileShader(pData, "Main", "ps_5_0", pixelShaderBlob, type))
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
		DISPDBG_FP(0, "ERROR: D3D11: Failed to create pixel shader" << std::hex << hr);
		return nullptr;
	}

	return pixelShader;
}

HRESULT D3D11Func_BindResources(D3D11* d3d, D3D11Pipeline* pipeline, PipelineStage pipelineStage, int count)
{
	switch (pipelineStage)
	{
	case PIPELINE_STAGE_VERTEX:
		d3d->context->VSSetShaderResources(0, count, pipeline->resourceStates[pipelineStage]->GetAddressOf());
		break;
	case PIPELINE_STAGE_PIXEL:
		d3d->context->PSSetShaderResources(0, count, pipeline->resourceStates[pipelineStage]->GetAddressOf());
		break;
	case PIPELINE_STAGE_COMPUTE:
		d3d->context->CSSetShaderResources(0, count, pipeline->resourceStates[pipelineStage]->GetAddressOf());
		break;
	}

	return S_OK;
}

HRESULT D3D11Func_BindSamplers(D3D11* d3d, D3D11Pipeline* pipeline, PipelineStage pipelineStage, int count)
{
	switch (pipelineStage)
	{
	case PIPELINE_STAGE_VERTEX:
		d3d->context->VSSetSamplers(0, count, pipeline->samplerStates[pipelineStage]->GetAddressOf());
		break;
	case PIPELINE_STAGE_PIXEL:
		d3d->context->PSSetSamplers(0, count, pipeline->samplerStates[pipelineStage]->GetAddressOf());
		break;
	case PIPELINE_STAGE_COMPUTE:
		d3d->context->CSSetSamplers(0, count, pipeline->samplerStates[pipelineStage]->GetAddressOf());
		break;
	}

	return S_OK;
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

		memset((*ppsurface), 0, sizeof(D3D11Surface));

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

			D3D11_TEXTURE2D_DESC desc2D;
			(*ppsurface)->texture->GetDesc(&desc2D);

			pddsd->dwWidth = desc2D.Width;
			pddsd->dwHeight = desc2D.Height;

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

			D3D11_TEXTURE2D_DESC desc2D;
			desc2D.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // todo: convert 
			desc2D.Width = pddsd->dwWidth;
			desc2D.Height = pddsd->dwHeight;
			desc2D.ArraySize = 1;
			desc2D.MipLevels = 1;
			desc2D.Usage = D3D11_USAGE_DEFAULT;
			desc2D.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc2D.MiscFlags = D3D11_RESOURCE_MISC_GDI_COMPATIBLE;
			desc2D.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
			desc2D.SampleDesc.Count = 1;
			desc2D.SampleDesc.Quality = 0;

			// create texture
			HRESULT hr = d3d->device->CreateTexture2D(&desc2D, nullptr, &(*ppsurface)->texture);
			if (FAILED(hr)) { DISPDBG_FP(0, "ERROR:  ID3D11Device::CreateTexture2D() returned: " << std::hex << hr); return hr; }

			// query surface
			hr = (*ppsurface)->texture->QueryInterface(&(*ppsurface)->surface);
			if (FAILED(hr)) { DISPDBG_FP(0, "ERROR:  ID3D11Device::QueryInterface() returned: " << std::hex << hr); return hr; }

			D3D11_TEXTURE2D_DESC desc2d;
			(*ppsurface)->texture->GetDesc(&desc2d);

			// create rtv
			//D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
			//rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			//rtvDesc.Texture2DArray.MipSlice = 0;
			//rtvDesc.Texture2DArray.ArraySize = 1;
			//rtvDesc.Texture2DArray.FirstArraySlice = 0;
			//rtvDesc.Format = desc2d.Format;

			///hr = d3d->device->CreateRenderTargetView((*ppsurface)->texture, &rtvDesc, &(*ppsurface)->rtv);
			//if (FAILED(hr)) { DISPDBG_FP(0, "ERROR: ID3D11Device::CreateRenderTargetView() returned" << std::hex << hr); return hr; }

			// create srv
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = desc2d.MipLevels;
			srvDesc.Format = desc2D.Format;
			
			hr = d3d->device->CreateShaderResourceView((*ppsurface)->texture, &srvDesc, &(*ppsurface)->srv);
			if (FAILED(hr)) { DISPDBG_FP(0, "ERROR: ID3D11Device::CreateShaderResourceView() returned" << std::hex << hr); return hr; }


			// TESTS
			//HDC pDC;
			//hr = (*ppsurface)->surface->GetDC(FALSE, &pDC);
			//if (FAILED(hr)) { DISPDBG_FP(0, "ERROR: ID3D11Device::GetDC for texture failed() returned" << std::hex << hr); return hr; }
			//(*ppsurface)->surface->ReleaseDC(NULL);


		}
		else if( pddsd->ddsCaps.dwCaps & DDSCAPS_ZBUFFER )
		{
			(*ppsurface)->flags |= D3D11_SURF_FLAG_ZBUFFER;
		}
		else
			return E_INVALIDARG;

		/* We'll need this later */
		memmove(&(*ppsurface)->ddsd, pddsd, sizeof(DDSURFACEDESC2));

		return DD_OK;
	}

	return E_INVALIDARG;
}

HRESULT D3D11func_CreateDefaultSamplers(D3D11* d3d)
{
	D3D11_SAMPLER_DESC desc;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.MipLODBias = 0.0f;
	desc.MaxAnisotropy = 16;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.BorderColor[0] = desc.BorderColor[1] = desc.BorderColor[2] = desc.BorderColor[3] = 0.0f;
	desc.MinLOD = 0.0f;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr = d3d->device->CreateSamplerState(&desc, &d3d->linearSamplerState);
	if (FAILED(hr)) { DISPDBG_FP(0, "ERROR:  ID3D11Device::CreateSamplerState() returned: " << std::hex << hr); return hr; }

	D3D_SET_OBJECT_NAME_A(d3d->linearSamplerState, "generic_linear_sampler");
}

HRESULT D3D11Func_DeleteSurface( D3D11Surface** ppsurface )
{
	//if( (*ppsurface)->parent_context )
	//	delete (*ppsurface)->parent_context;

	if( (*ppsurface)->rtv )
		(*ppsurface)->rtv->Release();

	if ((*ppsurface)->srv)
		(*ppsurface)->srv->Release();

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

HRESULT D3D11Func_SetInputLayout(D3D11* d3d, D3D11Pipeline* pipeline)
{
	d3d->context->IASetInputLayout(pipeline->inputLayout.Get());

	return S_OK;
}

HRESULT D3D11Func_SetVertexBuffers(D3D11* d3d, D3D11Pipeline* pipeline)
{
	constexpr UINT vertexStride = sizeof(VertexProperties);
	constexpr UINT vertexOffset = 0;

	d3d->context->IASetVertexBuffers(
		0,
		1,
		pipeline->vertexBuffer.GetAddressOf(),
		&vertexStride,
		&vertexOffset);

	d3d->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return S_OK;
}

HRESULT D3D11Func_SetVertexShader(D3D11* d3d, D3D11Pipeline* pipeline)
{
	d3d->context->VSSetShader(
		pipeline->vertexShader.Get(),
		nullptr,
		0);

	return S_OK;
}

HRESULT D3D11Func_SetPixelShader(D3D11* d3d, D3D11Pipeline* pipeline)
{
	d3d->context->PSSetShader(
		pipeline->pixelShader.Get(),
		nullptr,
		0);

	return S_OK;
}


HRESULT D3D11Func_Draw(D3D11* d3d, D3D11Pipeline* pipeline)
{
	d3d->context->Draw(pipeline->vertexCount, 0);
	//d3d->context->ClearState();
	return S_OK;
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
	return surface->surface->GetDC(FALSE, pDC);
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
		D3D11SurfaceFunc_BltFast(d3d, surface, lpDestRect, lpSrcRect, DDBLTFAST_NOCOLORKEY);
	}

	return E_FAIL;
}

HRESULT D3D11SurfaceFunc_BltFast(D3D11* d3d, D3D11Surface* surface, LPRECT lpDestRect, LPRECT lpSrcRect, DWORD dwTrans )
{
	D3DVIEWPORT7 vp;
	vp.dwX = lpDestRect->left;
	vp.dwY = lpDestRect->top;
	vp.dwWidth = lpDestRect->right - lpDestRect->left;
	vp.dwHeight = lpDestRect->bottom - lpDestRect->top;
	vp.dvMinZ = 0;
	vp.dvMaxZ = 1;

	D3D11Func_SetViewport(d3d, &vp);
	D3D11Func_ClearRT(d3d, 0x000000);

	// default vertex coords for a fullscreen quad
	VertexProperties vertices[] = {
	{  DirectX::XMFLOAT3{ -1.f, 1.f, 0.0f }, DirectX::XMFLOAT3{ 0, 1, 0 }, DirectX::XMFLOAT2{ 0, 0 }},
	{  DirectX::XMFLOAT3{ 1.0f, -1.f, 0.0f }, DirectX::XMFLOAT3{ 0, 1, 1 }, DirectX::XMFLOAT2{ 1, 1 }},
	{  DirectX::XMFLOAT3{ -1.0f, -1.0f, 0.0f }, DirectX::XMFLOAT3{ 1, 0, 0 }, DirectX::XMFLOAT2{ 0, 1 }},
	{  DirectX::XMFLOAT3{ -1.f, 1.f, 0.0f }, DirectX::XMFLOAT3{ 0, 1, 0 }, DirectX::XMFLOAT2{ 0, 0 }},
	{  DirectX::XMFLOAT3{ 1.f, 1.f, 0.0f }, DirectX::XMFLOAT3{ 0, 1, 1 }, DirectX::XMFLOAT2{ 1, 0 }},
	{  DirectX::XMFLOAT3{ 1.0f, -1.f, 0.0f }, DirectX::XMFLOAT3{ 0, 1, 1 }, DirectX::XMFLOAT2{ 1, 1 }},
	};

	D3D11_TEXTURE2D_DESC desc;
	surface->texture->GetDesc(&desc);

	float x = float(lpSrcRect->left) / float(desc.Width);
	float y = float(lpSrcRect->top) / float(desc.Height);
	float w = float(lpSrcRect->right - lpSrcRect->left) / float(desc.Width);
	float h = float(lpSrcRect->bottom - lpSrcRect->top) / float(desc.Height);


	D3D11Func_InitPipelineShaders(&d3d, &d3d->defaultBlitPipeline, 
		L"shaders/Main.vs.hlsl",
		L"shaders/Main.ps.hlsl", PIPELINE_SHADER_FILE);

	D3D11Func_CreateVertexShaderInputLayout(&d3d, &d3d->defaultBlitPipeline);
	D3D11Func_CreateVertexBuffer(&d3d, &d3d->defaultBlitPipeline, vertices, ARRAYSIZE(vertices));
	D3D11Func_SetInputLayout(d3d, &d3d->defaultBlitPipeline);
	D3D11Func_SetVertexBuffers(d3d, &d3d->defaultBlitPipeline);
	D3D11Func_SetVertexShader(d3d, &d3d->defaultBlitPipeline);
	D3D11Func_SetPixelShader(d3d, &d3d->defaultBlitPipeline);

	ShaderConstants constants;
	constants.tint = DirectX::XMFLOAT4(1, 1, 1, 1); 
	constants.sampleParameters = DirectX::XMFLOAT4(x, y, w, h);
	D3D11Func_CreateConstantBuffer(&d3d, &d3d->defaultBlitPipeline, 0, PIPELINE_STAGE_PIXEL, &constants, sizeof(ShaderConstants));
	D3D11Func_UpdateConstantBuffer(&d3d, &d3d->defaultBlitPipeline, 0, PIPELINE_STAGE_PIXEL, &constants, sizeof(ShaderConstants));
	D3D11Func_BindConstantBuffers(d3d, &d3d->defaultBlitPipeline, PIPELINE_STAGE_PIXEL, 1);

	// set our samplers and then bind
	d3d->defaultBlitPipeline.samplerStates[PIPELINE_STAGE_PIXEL][0] = d3d->linearSamplerState;
	D3D11Func_BindSamplers(d3d, &d3d->defaultBlitPipeline, PIPELINE_STAGE_PIXEL, 1);

	// set our resources / textures and then bind
	d3d->defaultBlitPipeline.resourceStates[PIPELINE_STAGE_PIXEL][0] = surface->srv;
	D3D11Func_BindResources(d3d, &d3d->defaultBlitPipeline, PIPELINE_STAGE_PIXEL, 1);

	// do the drawing
	D3D11Func_Draw(d3d, &d3d->defaultBlitPipeline);

	// copy method
	/*
	ID3D11RenderTargetView* rtv = nullptr;
	ID3D11Resource* pResource = nullptr;
	d3d->context->OMGetRenderTargets(1, &rtv, NULL);
	rtv->GetResource(&pResource);

	d3d->context->CopySubresourceRegion(pResource, 0, lpDestRect->left, lpDestRect->top, 0, surface->texture, 0, nullptr);
	*/
	return S_OK;
}
