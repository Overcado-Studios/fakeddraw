#include "pch.h"
#include "framework.h"
#include <ddraw.h>
#include <d3d.h>
#include "fakeddraw.h"
#include "faked3d.h"
#include "vtable.h"
#include "d3d11_func.h"
#include "nvdebug.h"


#define GUARD( ptr, ret ) if( !ptr ) return ret
#define _GETREF( type ) ((type*) This->reserved)->RefCount
#define _INCREF( type ) ((type*) This->reserved)->RefCount++
#define _DECREF( type, func ) ((type*) This->reserved)->RefCount--; \
						if( ((type*) This->reserved)->RefCount < 1 ) { func(&This); }
#define _RETREF( type ) return ((type*) This->reserved)->RefCount;
#define _ACCESS( type, ptr ) ((type*) (ptr)->reserved)
#define ACCESS( type ) ((type*) (This)->reserved)

#define LOGUNIMPL_F		{ static bool once=true; if(once){ DISPDBG_FP(0, "Not implemented!"); NVASSERT(0,"Not implemented"); once=false; } return E_FAIL; }
#define LOGUNIMPL(r)	{ static bool once=true; if(once){ DISPDBG_FP(0, "Not implemented!"); NVASSERT(0,"Not implemented"); once=false; } return r; }

extern NVDebug nvdebug;


struct D3DPrivate
{
	ULONG RefCount;
	D3D11* pD3DContext;
};

struct D3DDevicePrivate
{
	ULONG RefCount;
	D3D11* pD3DContext;
};

struct D3DVertexBufferPrivate
{
	ULONG RefCount;
};


HRESULT PVT_Direct3DFakeCreate( LPVOID* ppvObj, D3D11* d3d )
{
	/* Sanity check */
	if( !ppvObj )
		return E_INVALIDARG;

	/* TODO: Actually use GUIDs */

	/* Allocate fake D3D interface and initialize Vtable for C */
	(*ppvObj ) = new IDirect3DFake;
	if( !(*ppvObj ) )
		return E_OUTOFMEMORY;

	IDirect3DFake** lplpD3D = (IDirect3DFake**) ppvObj;
	if( !D3DVtableCreate( lplpD3D ) )
		return E_OUTOFMEMORY;

	(*lplpD3D)->reserved = new D3DPrivate;
	if( !(*lplpD3D)->reserved )
		return E_OUTOFMEMORY;

	memset( (*lplpD3D)->reserved, 0, sizeof( D3DPrivate ) );

	_ACCESS( D3DPrivate, (*lplpD3D ) )->pD3DContext = d3d;
	_ACCESS( D3DPrivate, (*lplpD3D) )->RefCount = 1;

	return D3D_OK;
}

HRESULT PVT_Direct3DDeviceFakeCreate( LPVOID* ppvObj, D3D11* d3d )
{
	/* Sanity check */
	if( !ppvObj )
		return E_INVALIDARG;

	/* TODO: Actually use GUIDs */

	/* Allocate fake D3DDevice interface and initialize Vtable for C */
	(*ppvObj ) = new IDirect3DDeviceFake;
	if( !(*ppvObj ) )
		return E_OUTOFMEMORY;

	IDirect3DDeviceFake** lplpD3DDevice = (IDirect3DDeviceFake**) ppvObj;
	if( !D3DDeviceVtableCreate( lplpD3DDevice ) )
		return E_OUTOFMEMORY;

	(*lplpD3DDevice)->reserved = new D3DDevicePrivate;
	if( !(*lplpD3DDevice)->reserved )
		return E_OUTOFMEMORY;

	memset( (*lplpD3DDevice)->reserved, 0, sizeof( D3DDevicePrivate ) );

	_ACCESS( D3DDevicePrivate, (*lplpD3DDevice) )->pD3DContext = d3d;
	_ACCESS( D3DDevicePrivate, (*lplpD3DDevice) )->RefCount = 1;

	return D3D_OK;
}

VOID PVT_IDirect3DFake_Uninitialize( IDirect3DFake** This )
{
	/* Free private data */
	delete _ACCESS( D3DPrivate, (*This) );

	/* Delete the Vtable (for C) */
	D3DVtableDelete( This );
}

VOID PVT_IDirect3DDeviceFake_Uninitialize( IDirect3DDeviceFake** This )
{
	/* Free private data */
	delete _ACCESS( D3DPrivate, (*This) );

	/* Delete the Vtable (for C) */
	D3DDeviceVtableDelete( This );
}

VOID PVT_IDirect3DVertexBufferFake_Uninitialize( IDirect3DVertexBufferFake** This )
{
	/* Free private data */
	delete _ACCESS( D3DPrivate, (*This) );

	/* Delete the Vtable (for C) */
	D3DVertexBufferVtableDelete( This );
}


/*
 * IDirect3DFake functions
 */

HRESULT WINAPI IDirect3DFake::QueryInterface(  REFIID riid, LPVOID FAR* ppvObj )
{
	return IDirect3DFake_QueryInterface( this, riid, ppvObj );
}

HRESULT WINAPI IDirect3DFake_QueryInterface( IDirect3DFake* This, REFIID riid, LPVOID FAR* ppvObj )
{
	return E_NOINTERFACE;
}

ULONG WINAPI IDirect3DFake::AddRef()
{
	return IDirect3DFake_AddRef(this);
}

ULONG WINAPI IDirect3DFake_AddRef( IDirect3DFake* This )
{
	GUARD( This, 0 );
	_INCREF( D3DPrivate );
	_RETREF( D3DPrivate );
}

ULONG WINAPI IDirect3DFake::Release()
{
	return IDirect3DFake_Release(this);
}

ULONG WINAPI IDirect3DFake_Release( IDirect3DFake* This )
{
	GUARD( This, 0 );
	ULONG ref = _GETREF( D3DPrivate );
	_DECREF( D3DPrivate, PVT_IDirect3DFake_Uninitialize );
	_RETREF( D3DPrivate );
}

HRESULT WINAPI IDirect3DFake::CreateDevice( REFCLSID rclsid, IDirectDrawSurfaceFake* lpDDS, IDirect3DDeviceFake** lplpD3DDevice )
{
	return IDirect3DFake_CreateDevice( this, rclsid, lpDDS, lplpD3DDevice );
}

HRESULT WINAPI IDirect3DFake_CreateDevice( IDirect3DFake* This, REFCLSID rclsid, IDirectDrawSurfaceFake* lpDDS, IDirect3DDeviceFake** lplpD3DDevice ) 
{ 
	GUARD( This, E_FAIL );
	return PVT_Direct3DDeviceFakeCreate( (LPVOID*) lplpD3DDevice, ACCESS(D3DPrivate)->pD3DContext );
}

HRESULT WINAPI IDirect3DFake::CreateVertexBuffer(LPD3DVERTEXBUFFERDESC lpVBDesc, IDirect3DVertexBufferFake** lplpD3DVertexBuffer, DWORD dwFlags )
{
	return IDirect3DFake_CreateVertexBuffer( this, lpVBDesc, lplpD3DVertexBuffer, dwFlags );
}

HRESULT WINAPI IDirect3DFake_CreateVertexBuffer( IDirect3DFake* This, LPD3DVERTEXBUFFERDESC lpVBDesc, IDirect3DVertexBufferFake** lplpD3DVertexBuffer, DWORD dwFlags ) { LOGUNIMPL_F; }

HRESULT WINAPI IDirect3DFake::EnumDevices( LPD3DENUMDEVICESCALLBACK7 lpEnumDevicesCallback, LPVOID lpUserArg )
{
	return IDirect3DFake_EnumDevices( this, lpEnumDevicesCallback, lpUserArg );
}

HRESULT WINAPI IDirect3DFake_EnumDevices( IDirect3DFake* This, LPD3DENUMDEVICESCALLBACK7 lpEnumDevicesCallback, LPVOID lpUserArg ) { LOGUNIMPL_F; }

HRESULT WINAPI IDirect3DFake::EnumZBufferFormats( REFCLSID riidDevice, LPD3DENUMPIXELFORMATSCALLBACK lpEnumCallback, LPVOID lpContext ) 
{
	return IDirect3DFake_EnumZBufferFormats( this, riidDevice, lpEnumCallback, lpContext );
}

HRESULT WINAPI IDirect3DFake_EnumZBufferFormats( IDirect3DFake* This, REFCLSID riidDevice, LPD3DENUMPIXELFORMATSCALLBACK lpEnumCallback, LPVOID lpContext ) 
{ 
	GUARD( This, E_FAIL );

	/* TODO: Enumerate depth stencil formats */
	DDPIXELFORMAT ddpf;
	::ZeroMemory( &ddpf, sizeof( ddpf ) );
	ddpf.dwSize = sizeof( DDPIXELFORMAT );
	ddpf.dwZBufferBitDepth = 24;
	ddpf.dwStencilBitDepth = 8;
	ddpf.dwFlags = DDPF_ZBUFFER;

	if( lpEnumCallback( &ddpf, lpContext ) == D3DENUMRET_CANCEL )
		return D3D_OK;
	
	return D3D_OK; 
}

HRESULT WINAPI IDirect3DFake::EvictManagedTextures()
{
	return IDirect3DFake_EvictManagedTextures(this);
}

HRESULT WINAPI IDirect3DFake_EvictManagedTextures( IDirect3DFake* This ) { LOGUNIMPL_F; }


/*
 * IDirect3DDeviceFake functions
 */

HRESULT WINAPI IDirect3DDeviceFake::QueryInterface( REFIID riid, LPVOID FAR* ppvObj )
{
	return IDirect3DDeviceFake_QueryInterface( this, riid, ppvObj );
}

HRESULT WINAPI IDirect3DDeviceFake_QueryInterface( IDirect3DDeviceFake* This, REFIID riid, LPVOID FAR* ppvObj )
{
	return E_NOINTERFACE;
}

ULONG WINAPI IDirect3DDeviceFake::AddRef()
{
	return IDirect3DDeviceFake_AddRef(this);
}

ULONG WINAPI IDirect3DDeviceFake_AddRef( IDirect3DDeviceFake* This )
{
	GUARD( This, 0 );
	_INCREF( D3DDevicePrivate );
	_RETREF( D3DDevicePrivate );
}

ULONG WINAPI IDirect3DDeviceFake::Release()
{
	return IDirect3DDeviceFake_Release(this);
}

ULONG WINAPI IDirect3DDeviceFake_Release( IDirect3DDeviceFake* This )
{
	GUARD( This, 0 );
	ULONG ref = _GETREF( D3DDevicePrivate );
	_DECREF( D3DDevicePrivate, PVT_IDirect3DDeviceFake_Uninitialize );
	_RETREF( D3DDevicePrivate );
}

HRESULT WINAPI IDirect3DDeviceFake::ApplyStateBlock( DWORD dwBlockHandle )
{
	return IDirect3DDeviceFake_ApplyStateBlock( this, dwBlockHandle );
}

HRESULT WINAPI IDirect3DDeviceFake_ApplyStateBlock( IDirect3DDeviceFake* This,   DWORD dwBlockHandle ) LOGUNIMPL_F 

HRESULT WINAPI IDirect3DDeviceFake::BeginScene()
{
	return IDirect3DDeviceFake_BeginScene(this);
}

HRESULT WINAPI IDirect3DDeviceFake_BeginScene( IDirect3DDeviceFake* This )
{
	/* Nothing to do here */
	return D3D_OK;
}

HRESULT WINAPI IDirect3DDeviceFake::BeginStateBlock()
{
	return IDirect3DDeviceFake_BeginStateBlock(this);
}

HRESULT WINAPI IDirect3DDeviceFake_BeginStateBlock( IDirect3DDeviceFake* This ) LOGUNIMPL_F

HRESULT WINAPI IDirect3DDeviceFake::CaptureStateBlock( DWORD dwBlockHandle )
{
	return IDirect3DDeviceFake_CaptureStateBlock( this, dwBlockHandle );
}

HRESULT WINAPI IDirect3DDeviceFake_CaptureStateBlock( IDirect3DDeviceFake* This,  DWORD dwBlockHandle ) LOGUNIMPL_F

HRESULT WINAPI IDirect3DDeviceFake::CreateStateBlock( D3DSTATEBLOCKTYPE d3dsbType, LPDWORD lpdwBlockHandle )
{
	return IDirect3DDeviceFake_CreateStateBlock( this, d3dsbType, lpdwBlockHandle );
}

HRESULT WINAPI IDirect3DDeviceFake_CreateStateBlock( IDirect3DDeviceFake* This, D3DSTATEBLOCKTYPE d3dsbType, LPDWORD lpdwBlockHandle ) { LOGUNIMPL_F; }

HRESULT WINAPI IDirect3DDeviceFake::Clear( DWORD dwCount, LPD3DRECT lpRects, DWORD dwFlags, DWORD dwColor, D3DVALUE dvZ, DWORD dwStencil )
{
	return IDirect3DDeviceFake_Clear( this, dwCount, lpRects, dwFlags, dwColor, dvZ, dwStencil );
}

HRESULT WINAPI IDirect3DDeviceFake_Clear( IDirect3DDeviceFake* This,  DWORD     dwCount,     LPD3DRECT lpRects,   
  DWORD     dwFlags,     DWORD     dwColor,     D3DVALUE  dvZ,       
  DWORD     dwStencil  ) LOGUNIMPL_F

HRESULT WINAPI IDirect3DDeviceFake::ComputeSphereVisibility( LPD3DVECTOR lpCenters,         
  LPD3DVALUE  lpRadii,             DWORD       dwNumSpheres,      
  DWORD       dwFlags,             LPDWORD     lpdwReturnValues   )
{
	return IDirect3DDeviceFake_ComputeSphereVisibility( this, lpCenters, lpRadii, dwNumSpheres, dwFlags, lpdwReturnValues );
}

HRESULT WINAPI IDirect3DDeviceFake_ComputeSphereVisibility( IDirect3DDeviceFake* This,   LPD3DVECTOR lpCenters,         
  LPD3DVALUE  lpRadii,             DWORD       dwNumSpheres,      
  DWORD       dwFlags,             LPDWORD     lpdwReturnValues   ) LOGUNIMPL_F 

HRESULT WINAPI IDirect3DDeviceFake::DeleteStateBlock( DWORD dwBlockHandle )
{
	return IDirect3DDeviceFake_DeleteStateBlock( this, dwBlockHandle );
}

HRESULT WINAPI IDirect3DDeviceFake_DeleteStateBlock ( IDirect3DDeviceFake* This,  DWORD dwBlockHandle  ) LOGUNIMPL_F 

HRESULT WINAPI IDirect3DDeviceFake::DrawIndexedPrimitive( D3DPRIMITIVETYPE d3dptPrimitiveType,  
  DWORD  dwVertexTypeDesc,                LPVOID lpvVertices,                   
  DWORD  dwVertexCount,                   LPWORD lpwIndices,                    
  DWORD  dwIndexCount,                    DWORD  dwFlags                        
) 
{
	return IDirect3DDeviceFake_DrawIndexedPrimitive( this, d3dptPrimitiveType, dwVertexTypeDesc, lpvVertices, dwVertexCount, lpwIndices, dwIndexCount, dwFlags );
}

HRESULT WINAPI IDirect3DDeviceFake_DrawIndexedPrimitive( IDirect3DDeviceFake* This,  D3DPRIMITIVETYPE d3dptPrimitiveType,  
  DWORD  dwVertexTypeDesc,                LPVOID lpvVertices,                   
  DWORD  dwVertexCount,                   LPWORD lpwIndices,                    
  DWORD  dwIndexCount,                    DWORD  dwFlags                        
) LOGUNIMPL_F

HRESULT WINAPI IDirect3DDeviceFake::DrawIndexedPrimitiveStrided( D3DPRIMITIVETYPE d3dptPrimitiveType,  
  DWORD  dwVertexTypeDesc,              
  LPD3DDRAWPRIMITIVESTRIDEDDATA lpVertexArray,  
  DWORD  dwVertexCount,                   LPWORD lpwIndices,                    
  DWORD  dwIndexCount,                    DWORD  dwFlags                        
) 
{
	return IDirect3DDeviceFake_DrawIndexedPrimitiveStrided( this, d3dptPrimitiveType, dwVertexTypeDesc, lpVertexArray, dwVertexCount, lpwIndices, dwIndexCount, dwFlags );
}

HRESULT WINAPI IDirect3DDeviceFake_DrawIndexedPrimitiveStrided( IDirect3DDeviceFake* This,  D3DPRIMITIVETYPE d3dptPrimitiveType,  
  DWORD  dwVertexTypeDesc,              
  LPD3DDRAWPRIMITIVESTRIDEDDATA lpVertexArray,  
  DWORD  dwVertexCount,                   LPWORD lpwIndices,                    
  DWORD  dwIndexCount,                    DWORD  dwFlags                        
) LOGUNIMPL_F

HRESULT WINAPI IDirect3DDeviceFake::DrawIndexedPrimitiveVB( D3DPRIMITIVETYPE        d3dptPrimitiveType,
  IDirect3DVertexBufferFake* lpd3dVertexBuffer,  DWORD  dwStartVertex,
  DWORD  dwNumVertices,  LPWORD lpwIndices,  DWORD  dwIndexCount,
  DWORD  dwFlags )
{
	return IDirect3DDeviceFake_DrawIndexedPrimitiveVB( this,  d3dptPrimitiveType, lpd3dVertexBuffer, dwStartVertex, dwNumVertices, lpwIndices, dwIndexCount, dwFlags );
}

HRESULT WINAPI IDirect3DDeviceFake_DrawIndexedPrimitiveVB( IDirect3DDeviceFake* This,  D3DPRIMITIVETYPE        d3dptPrimitiveType,
  IDirect3DVertexBufferFake* lpd3dVertexBuffer,  DWORD  dwStartVertex,
  DWORD  dwNumVertices,  LPWORD lpwIndices,  DWORD  dwIndexCount,
  DWORD  dwFlags) LOGUNIMPL_F

HRESULT WINAPI IDirect3DDeviceFake::DrawPrimitive( D3DPRIMITIVETYPE dptPrimitiveType,  
  DWORD  dwVertexTypeDesc,              LPVOID lpvVertices,                 
  DWORD  dwVertexCount,                 DWORD  dwFlags                      )
{
	return IDirect3DDeviceFake_DrawPrimitive( this, dptPrimitiveType, dwVertexTypeDesc, lpvVertices, dwVertexCount, dwFlags );
}

HRESULT WINAPI IDirect3DDeviceFake_DrawPrimitive( IDirect3DDeviceFake* This,  D3DPRIMITIVETYPE dptPrimitiveType,  
  DWORD  dwVertexTypeDesc,              LPVOID lpvVertices,                 
  DWORD  dwVertexCount,                 DWORD  dwFlags                      ) LOGUNIMPL_F

HRESULT WINAPI IDirect3DDeviceFake::DrawPrimitiveStrided( D3DPRIMITIVETYPE dptPrimitiveType,  
  DWORD  dwVertexTypeDesc,            
  LPD3DDRAWPRIMITIVESTRIDEDDATA lpVertexArray,  
  DWORD  dwVertexCount,                 DWORD  dwFlags                      )
{
	return IDirect3DDeviceFake_DrawPrimitiveStrided( this, dptPrimitiveType, dwVertexTypeDesc, lpVertexArray, dwVertexCount, dwFlags );
}

HRESULT WINAPI IDirect3DDeviceFake_DrawPrimitiveStrided( IDirect3DDeviceFake* This,  D3DPRIMITIVETYPE dptPrimitiveType,  
  DWORD  dwVertexTypeDesc,            
  LPD3DDRAWPRIMITIVESTRIDEDDATA lpVertexArray,  
  DWORD  dwVertexCount,                 DWORD  dwFlags                      ) LOGUNIMPL_F

HRESULT WINAPI IDirect3DDeviceFake::DrawPrimitiveVB( D3DPRIMITIVETYPE d3dptPrimitiveType, IDirect3DVertexBufferFake* lpd3dVertexBuffer,  DWORD dwStartVertex, DWORD dwNumVertices,  DWORD dwFlags )
{
	return IDirect3DDeviceFake_DrawPrimitiveVB( this, d3dptPrimitiveType,lpd3dVertexBuffer, dwStartVertex, dwNumVertices, dwFlags );
}

HRESULT WINAPI IDirect3DDeviceFake_DrawPrimitiveVB( IDirect3DDeviceFake* This,   D3DPRIMITIVETYPE        d3dptPrimitiveType,
  IDirect3DVertexBufferFake* lpd3dVertexBuffer,  DWORD dwStartVertex,
  DWORD dwNumVertices,  DWORD dwFlags) LOGUNIMPL_F 

HRESULT WINAPI IDirect3DDeviceFake::EndScene()
{
	return IDirect3DDeviceFake_EndScene(this);
}

HRESULT WINAPI IDirect3DDeviceFake_EndScene( IDirect3DDeviceFake* This )
{
	/* Nothing to do here */
	return D3D_OK;
}

HRESULT WINAPI IDirect3DDeviceFake::EndStateBlock( LPDWORD lpdwBlockHandle )
{
	return IDirect3DDeviceFake_EndStateBlock( this,  lpdwBlockHandle );
}

HRESULT WINAPI IDirect3DDeviceFake_EndStateBlock( IDirect3DDeviceFake* This,   LPDWORD lpdwBlockHandle ) LOGUNIMPL_F 

HRESULT WINAPI IDirect3DDeviceFake::EnumTextureFormats( LPD3DENUMPIXELFORMATSCALLBACK lpd3dEnumPixelProc, LPVOID lpArg )
{
	return IDirect3DDeviceFake_EnumTextureFormats( this, lpd3dEnumPixelProc, lpArg );
}

HRESULT WINAPI IDirect3DDeviceFake_EnumTextureFormats( IDirect3DDeviceFake* This,  LPD3DENUMPIXELFORMATSCALLBACK lpd3dEnumPixelProc, LPVOID lpArg ) LOGUNIMPL_F

HRESULT WINAPI IDirect3DDeviceFake::GetCaps( LPD3DDEVICEDESC7 lpD3DDevDesc )
{
	return IDirect3DDeviceFake_GetCaps( this, lpD3DDevDesc );
}

HRESULT WINAPI IDirect3DDeviceFake_GetCaps( IDirect3DDeviceFake* This,  LPD3DDEVICEDESC7 lpD3DDevDesc ) LOGUNIMPL_F

HRESULT WINAPI IDirect3DDeviceFake::GetClipPlane( DWORD dwIndex,   D3DVALUE* pPlaneEquation )
{
	return IDirect3DDeviceFake_GetClipPlane( this, dwIndex, pPlaneEquation );
}

HRESULT WINAPI IDirect3DDeviceFake_GetClipPlane( IDirect3DDeviceFake* This,   DWORD dwIndex,   D3DVALUE* pPlaneEquation ) LOGUNIMPL_F 

HRESULT WINAPI IDirect3DDeviceFake::GetClipStatus( LPD3DCLIPSTATUS lpD3DClipStatus ) 
{
	return IDirect3DDeviceFake_GetClipStatus( this, lpD3DClipStatus );
}

HRESULT WINAPI IDirect3DDeviceFake_GetClipStatus( IDirect3DDeviceFake* This,  LPD3DCLIPSTATUS lpD3DClipStatus ) LOGUNIMPL_F

HRESULT WINAPI IDirect3DDeviceFake::GetDirect3D( IDirect3DFake** lplpD3D )
{
	return IDirect3DDeviceFake_GetDirect3D( this, lplpD3D );
}

HRESULT WINAPI IDirect3DDeviceFake_GetDirect3D( IDirect3DDeviceFake* This,  IDirect3DFake** lplpD3D ) LOGUNIMPL_F

HRESULT WINAPI IDirect3DDeviceFake::GetInfo( DWORD  dwDevInfoID,   LPVOID pDevInfoStruct,   DWORD  dwSize )
{
	return IDirect3DDeviceFake_GetInfo( this, dwDevInfoID, pDevInfoStruct, dwSize );
}

HRESULT WINAPI IDirect3DDeviceFake_GetInfo( IDirect3DDeviceFake* This,   DWORD  dwDevInfoID,   LPVOID pDevInfoStruct,   DWORD  dwSize ) LOGUNIMPL_F 

HRESULT WINAPI IDirect3DDeviceFake::GetLight( DWORD dwLightIndex,   LPD3DLIGHT7 lpLight )
{
	return IDirect3DDeviceFake_GetLight( this, dwLightIndex, lpLight );
}

HRESULT WINAPI IDirect3DDeviceFake_GetLight( IDirect3DDeviceFake* This,   DWORD dwLightIndex,   LPD3DLIGHT7 lpLight ) LOGUNIMPL_F 

HRESULT WINAPI IDirect3DDeviceFake::GetLightEnable(  DWORD dwLightIndex,  BOOL* pbEnable )
{
	return IDirect3DDeviceFake_GetLightEnable( this, dwLightIndex, pbEnable );
}

HRESULT WINAPI IDirect3DDeviceFake_GetLightEnable( IDirect3DDeviceFake* This,   DWORD dwLightIndex,  BOOL* pbEnable ) LOGUNIMPL_F 

HRESULT WINAPI IDirect3DDeviceFake::GetMaterial( LPD3DMATERIAL7 lpMaterial ) 
{
	return IDirect3DDeviceFake_GetMaterial( this, lpMaterial );
}

HRESULT WINAPI IDirect3DDeviceFake_GetMaterial( IDirect3DDeviceFake* This,   LPD3DMATERIAL7 lpMaterial ) LOGUNIMPL_F 

HRESULT WINAPI IDirect3DDeviceFake::GetRenderState( D3DRENDERSTATETYPE dwRenderStateType,  LPDWORD lpdwRenderState )
{
	return IDirect3DDeviceFake_GetRenderState( this, dwRenderStateType, lpdwRenderState );
}

HRESULT WINAPI IDirect3DDeviceFake_GetRenderState( IDirect3DDeviceFake* This,  D3DRENDERSTATETYPE dwRenderStateType,  LPDWORD lpdwRenderState ) LOGUNIMPL_F

HRESULT WINAPI IDirect3DDeviceFake::GetRenderTarget( IDirectDrawSurfaceFake** lplpRenderTarget  ) 
{
	return IDirect3DDeviceFake_GetRenderTarget( this, lplpRenderTarget  );
}

HRESULT WINAPI IDirect3DDeviceFake_GetRenderTarget( IDirect3DDeviceFake* This,  IDirectDrawSurfaceFake** lplpRenderTarget  ) LOGUNIMPL_F

/*HRESULT WINAPI IDirect3DDeviceFake::GetStateData( DWORD dwState,   LPVOID* lplpStateData )
{
	return IDirect3DDeviceFake_GetStateData( this, dwState, lplpStateData );
}*/

HRESULT WINAPI IDirect3DDeviceFake_GetStateData( IDirect3DDeviceFake* This,   DWORD dwState,   LPVOID* lplpStateData ) LOGUNIMPL_F

HRESULT WINAPI IDirect3DDeviceFake::GetTexture( DWORD dwStage,  IDirectDrawSurfaceFake** lplpTexture )
{
	return IDirect3DDeviceFake_GetTexture( this, dwStage, lplpTexture );
}

HRESULT WINAPI IDirect3DDeviceFake_GetTexture( IDirect3DDeviceFake* This,   DWORD dwStage,  IDirectDrawSurfaceFake** lplpTexture ) LOGUNIMPL_F 

HRESULT WINAPI IDirect3DDeviceFake::GetTextureStageState( DWORD   dwStage, D3DTEXTURESTAGESTATETYPE dwState,  LPDWORD lpdwValue )
{
	return IDirect3DDeviceFake_GetTextureStageState( this, dwStage, dwState, lpdwValue );
}

HRESULT WINAPI IDirect3DDeviceFake_GetTextureStageState( IDirect3DDeviceFake* This,  DWORD   dwStage, D3DTEXTURESTAGESTATETYPE dwState,  LPDWORD lpdwValue ) LOGUNIMPL_F 

HRESULT WINAPI IDirect3DDeviceFake::GetTransform( D3DTRANSFORMSTATETYPE dtstTransformStateType,  LPD3DMATRIX lpD3DMatrix )
{
	return IDirect3DDeviceFake_GetTransform( this, dtstTransformStateType, lpD3DMatrix );
}

HRESULT WINAPI IDirect3DDeviceFake_GetTransform( IDirect3DDeviceFake* This,  D3DTRANSFORMSTATETYPE dtstTransformStateType,  LPD3DMATRIX lpD3DMatrix ) LOGUNIMPL_F

HRESULT WINAPI IDirect3DDeviceFake::GetViewport( LPD3DVIEWPORT7 lpViewport )
{
	return IDirect3DDeviceFake_GetViewport( this, lpViewport );
}

HRESULT WINAPI IDirect3DDeviceFake_GetViewport( IDirect3DDeviceFake* This,  LPD3DVIEWPORT7 lpViewport ) LOGUNIMPL_F 

HRESULT WINAPI IDirect3DDeviceFake::LightEnable( DWORD dwLightIndex,  BOOL bEnable )
{
	return IDirect3DDeviceFake_LightEnable( this, dwLightIndex, bEnable );
}

HRESULT WINAPI IDirect3DDeviceFake_LightEnable( IDirect3DDeviceFake* This,   DWORD dwLightIndex,  BOOL bEnable ) LOGUNIMPL_F 

HRESULT WINAPI IDirect3DDeviceFake::Load( IDirectDrawSurfaceFake* lpDestTex,   LPPOINT lpDestPoint, IDirectDrawSurfaceFake* lpSrcTex, LPRECT lprcSrcRect, DWORD dwFlags )
{
	return IDirect3DDeviceFake_Load( this, lpDestTex, lpDestPoint, lpSrcTex, lprcSrcRect, dwFlags );
}

HRESULT WINAPI IDirect3DDeviceFake_Load( IDirect3DDeviceFake* This,   IDirectDrawSurfaceFake* lpDestTex,   LPPOINT lpDestPoint, IDirectDrawSurfaceFake* lpSrcTex, LPRECT lprcSrcRect, DWORD dwFlags ) LOGUNIMPL_F

HRESULT WINAPI IDirect3DDeviceFake::MultiplyTransform(  D3DTRANSFORMSTATETYPE dtstTransformStateType, LPD3DMATRIX lpD3DMatrix )
{
	return IDirect3DDeviceFake_MultiplyTransform( this, dtstTransformStateType, lpD3DMatrix );
}

HRESULT WINAPI IDirect3DDeviceFake_MultiplyTransform( IDirect3DDeviceFake* This,  D3DTRANSFORMSTATETYPE dtstTransformStateType, LPD3DMATRIX lpD3DMatrix ) LOGUNIMPL_F

HRESULT WINAPI IDirect3DDeviceFake::PreLoad( IDirectDrawSurfaceFake* lpddsTexture )
{
	return IDirect3DDeviceFake_PreLoad( this, lpddsTexture );
}

HRESULT WINAPI IDirect3DDeviceFake_PreLoad( IDirect3DDeviceFake* This,  IDirectDrawSurfaceFake* lpddsTexture ) LOGUNIMPL_F

HRESULT WINAPI IDirect3DDeviceFake::SetClipPlane( DWORD dwIndex,   D3DVALUE* pPlaneEquation )
{
	return IDirect3DDeviceFake_SetClipPlane( this, dwIndex, pPlaneEquation );
}

HRESULT WINAPI IDirect3DDeviceFake_SetClipPlane( IDirect3DDeviceFake* This,   DWORD dwIndex,   D3DVALUE* pPlaneEquation ) LOGUNIMPL_F 

HRESULT WINAPI IDirect3DDeviceFake::SetClipStatus( LPD3DCLIPSTATUS lpD3DClipStatus  ) 
{
	return IDirect3DDeviceFake_SetClipStatus( this, lpD3DClipStatus );
}

HRESULT WINAPI IDirect3DDeviceFake_SetClipStatus( IDirect3DDeviceFake* This,  LPD3DCLIPSTATUS lpD3DClipStatus  ) LOGUNIMPL_F

HRESULT WINAPI IDirect3DDeviceFake::SetLight( DWORD dwLightIndex,   LPD3DLIGHT7 lpLight )
{
	return IDirect3DDeviceFake_SetLight( this, dwLightIndex, lpLight );
}

HRESULT WINAPI IDirect3DDeviceFake_SetLight( IDirect3DDeviceFake* This,   DWORD dwLightIndex,   LPD3DLIGHT7 lpLight ) LOGUNIMPL_F

HRESULT WINAPI IDirect3DDeviceFake::SetMaterial( LPD3DMATERIAL7 lpMaterial ) 
{
	return IDirect3DDeviceFake_SetMaterial( this, lpMaterial );
}

HRESULT WINAPI IDirect3DDeviceFake_SetMaterial( IDirect3DDeviceFake* This,   LPD3DMATERIAL7 lpMaterial ) LOGUNIMPL_F 

HRESULT WINAPI IDirect3DDeviceFake::SetRenderState(  D3DRENDERSTATETYPE dwRenderStateType, DWORD dwRenderState )
{
	return IDirect3DDeviceFake_SetRenderState( this, dwRenderStateType, dwRenderState );
}

HRESULT WINAPI IDirect3DDeviceFake_SetRenderState( IDirect3DDeviceFake* This,  D3DRENDERSTATETYPE dwRenderStateType, DWORD dwRenderState ) LOGUNIMPL_F


HRESULT WINAPI IDirect3DDeviceFake::SetRenderTarget( IDirectDrawSurfaceFake* lpNewRenderTarget, DWORD dwFlags )
{
	return IDirect3DDeviceFake_SetRenderTarget( this, lpNewRenderTarget, dwFlags );
}

HRESULT WINAPI IDirect3DDeviceFake_SetRenderTarget( IDirect3DDeviceFake* This,  IDirectDrawSurfaceFake* lpNewRenderTarget, DWORD dwFlags ) LOGUNIMPL_F

/*HRESULT WINAPI IDirect3DDeviceFake::SetStateData( DWORD dwState,   LPVOID lpStateData )
{
	return IDirect3DDeviceFake_SetStateData( this, dwState, lpStateData );
}*/

HRESULT WINAPI IDirect3DDeviceFake_SetStateData( IDirect3DDeviceFake* This,   DWORD dwState,   LPVOID lpStateData ) LOGUNIMPL_F

HRESULT WINAPI IDirect3DDeviceFake::SetTexture( DWORD dwStage, IDirectDrawSurfaceFake* lpTexture ) 
{
	return IDirect3DDeviceFake_SetTexture( this, dwStage, lpTexture );
}

HRESULT WINAPI IDirect3DDeviceFake_SetTexture( IDirect3DDeviceFake* This, DWORD dwStage, IDirectDrawSurfaceFake* lpTexture ) LOGUNIMPL_F 

HRESULT WINAPI IDirect3DDeviceFake::SetTextureStageState( DWORD dwStage, D3DTEXTURESTAGESTATETYPE dwState, DWORD dwValue )
{
	return IDirect3DDeviceFake_SetTextureStageState( this, dwStage, dwState, dwValue );
}

HRESULT WINAPI IDirect3DDeviceFake_SetTextureStageState( IDirect3DDeviceFake* This, DWORD dwStage, D3DTEXTURESTAGESTATETYPE dwState, DWORD dwValue ) LOGUNIMPL_F 

HRESULT WINAPI IDirect3DDeviceFake::SetTransform( D3DTRANSFORMSTATETYPE dtstTransformStateType, LPD3DMATRIX lpD3DMatrix )
{
	return IDirect3DDeviceFake_SetTransform( this, dtstTransformStateType, lpD3DMatrix );
}

HRESULT WINAPI IDirect3DDeviceFake_SetTransform( IDirect3DDeviceFake* This,  D3DTRANSFORMSTATETYPE dtstTransformStateType, LPD3DMATRIX lpD3DMatrix ) LOGUNIMPL_F

HRESULT WINAPI IDirect3DDeviceFake::SetViewport( LPD3DVIEWPORT7 lpViewport )
{
	return IDirect3DDeviceFake_SetViewport( this, lpViewport );
}

HRESULT WINAPI IDirect3DDeviceFake_SetViewport( IDirect3DDeviceFake* This,  LPD3DVIEWPORT7 lpViewport ) 
{
	GUARD(This, E_FAIL);
	return D3D11Func_SetViewport( ACCESS(D3DDevicePrivate)->pD3DContext, lpViewport );
}

HRESULT WINAPI IDirect3DDeviceFake::ValidateDevice( LPDWORD lpdwPasses ) 
{
	return IDirect3DDeviceFake_ValidateDevice( this, lpdwPasses );
}

HRESULT WINAPI IDirect3DDeviceFake_ValidateDevice( IDirect3DDeviceFake* This,  LPDWORD lpdwPasses ) LOGUNIMPL_F


/*
 * IDirect3DVertexBufferFake functions
 */
HRESULT WINAPI IDirect3DVertexBufferFake_QueryInterface( IDirect3DVertexBufferFake* This, REFIID riid, LPVOID FAR* ppvObj )
{
	return E_NOINTERFACE;
}

ULONG WINAPI IDirect3DVertexBufferFake_AddRef( IDirect3DVertexBufferFake* This )
{
	GUARD( This, 0 );
	_INCREF( D3DVertexBufferPrivate );
	_RETREF( D3DVertexBufferPrivate );
}

ULONG WINAPI IDirect3DVertexBufferFake_Release( IDirect3DVertexBufferFake* This )
{
	GUARD( This, 0 );
	ULONG ref = _GETREF( D3DVertexBufferPrivate );
	_DECREF( D3DVertexBufferPrivate, PVT_IDirect3DVertexBufferFake_Uninitialize );
	_RETREF( D3DVertexBufferPrivate );
}

HRESULT WINAPI IDirect3DVertexBufferFake_GetVertexBufferDesc( IDirect3DVertexBufferFake* This,  LPD3DVERTEXBUFFERDESC lpVBDesc ) LOGUNIMPL_F

HRESULT WINAPI IDirect3DVertexBufferFake_Lock( IDirect3DVertexBufferFake* This, DWORD   dwFlags,    LPVOID* lplpData,   LPDWORD lpdwSize  ) LOGUNIMPL_F
HRESULT WINAPI IDirect3DVertexBufferFake_Optimize( IDirect3DVertexBufferFake* This,  LPDIRECT3DDEVICE7 lpD3DDevice,  DWORD dwFlags ) LOGUNIMPL_F 
HRESULT WINAPI IDirect3DVertexBufferFake_ProcessVertices( IDirect3DVertexBufferFake* This,  DWORD dwVertexOp,   DWORD dwDestIndex, 
  DWORD dwCount,   LPDIRECT3DVERTEXBUFFER7 lpSrcBuffer,   DWORD dwSrcIndex, 
  LPDIRECT3DDEVICE7       lpD3DDevice,   DWORD dwFlags) LOGUNIMPL_F
HRESULT WINAPI IDirect3DVertexBufferFake_ProcessVerticesStrided( IDirect3DVertexBufferFake* This,  DWORD dwVertexOp,   DWORD dwDestIndex, 
  DWORD dwCount,   LPD3DDRAWPRIMITIVESTRIDEDDATA lpVertexArray, 
  DWORD dwSrcIndex,   LPDIRECT3DDEVICE7 lpD3DDevice,   DWORD dwFlags) LOGUNIMPL_F
HRESULT WINAPI IDirect3DVertexBufferFake_Unlock( IDirect3DVertexBufferFake* This ) LOGUNIMPL_F


