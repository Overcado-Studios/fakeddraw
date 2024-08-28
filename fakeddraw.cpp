// fakeddraw.cpp : Defines the functions for the static library.
//

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

#define LOGUNIMPL_F		{ static bool once=true; if(once){ DISPDBG_FP(0, "Not implemented!"); NVASSERT(0,__func__<<": Not implemented"); once=false; } return E_FAIL; }
#define LOGUNIMPL(r)	{ static bool once=true; if(once){ DISPDBG_FP(0, "Not implemented!"); NVASSERT(0,__func__<<": Not implemented"); once=false; } return r; }

NVDebug nvdebug( 0, "fakeddraw_debug.txt" );

struct DDrawPrivate
{
	ULONG RefCount;
	DWORD dwCooperativeLevel;
	D3D11* pD3DContext;
};

struct DDrawSurfacePrivate
{
	ULONG RefCount;
	D3D11Surface* pSurface;
	D3D11* pParentD3DContext;
	DDSURFACEDESC2 ddsd;
};

struct DDrawClipperPrivate
{
	ULONG RefCount;
	DWORD dwFlags;
	HWND hWnd;
};

struct DDrawPalettePrivate
{
	ULONG RefCount;
	DWORD dwFlags;

	LPPALETTEENTRY	peEntries;
	VOID*			pData;
};

HRESULT WINAPI DirectDrawFakeCreate( GUID FAR *lpGUID, IDirectDrawFake FAR **lplpDD, IUnknownFake FAR *pUnkOuter );
HRESULT WINAPI DirectDrawFakeCreateEx( GUID FAR * lpGuid, LPVOID  *lplpDD, REFIID  iid,IUnknownFake FAR *pUnkOuter );
HRESULT WINAPI DirectDrawFakeCreateClipper( DWORD dwFlags, IDirectDrawClipperFake FAR **lplpDDClipper, IUnknownFake FAR *pUnkOuter );

VOID PVT_IDirectDrawFake_Uninitialize( IDirectDrawFake** This )
{
	/* Uninitialize D3D11 */
	D3D11Func_ReleaseDevice( &_ACCESS( DDrawPrivate, (*This) )->pD3DContext );
	D3D11Func_Uninitialize( &_ACCESS( DDrawPrivate, (*This) )->pD3DContext );

	/* Free private data */
	delete _ACCESS( DDrawPrivate, (*This) );

	/* Delete the Vtable (for C) */
	DDrawVtableDelete( This );
}

VOID PVT_IDirectDrawSurfaceFake_Uninitialize( IDirectDrawSurfaceFake** This )
{
	D3D11Func_DeleteSurface( &_ACCESS( DDrawSurfacePrivate, (*This) )->pSurface );

	delete _ACCESS( DDrawSurfacePrivate, (*This) );

	DDrawSurfaceVtableDelete( This );
}

VOID PVT_IDirectDrawClipperFake_Uninitialize( IDirectDrawClipperFake** This )
{
	delete _ACCESS( DDrawClipperPrivate, (*This) );

	DDrawClipperVtableDelete( This );
}

VOID PVT_IDirectDrawPaletteFake_Uninitialize( IDirectDrawPaletteFake** This )
{
	if( _ACCESS( DDrawPalettePrivate, (*This) )->pData )
		free( _ACCESS( DDrawPalettePrivate, (*This) )->pData );

	if( _ACCESS( DDrawPalettePrivate, (*This) )->peEntries )
		free( _ACCESS( DDrawPalettePrivate, (*This) )->peEntries );

	delete _ACCESS( DDrawPalettePrivate, (*This) );

	DDrawPaletteVtableDelete( This );
}


/*
 * IDirectDrawFake functions 
 */
ULONG WINAPI IDirectDrawFake::AddRef()
{
	return IDirectDrawFake_AddRef( this );
}

ULONG WINAPI IDirectDrawFake_AddRef( IDirectDrawFake* This )
{
	GUARD( This, 0 );
	_INCREF( DDrawPrivate );
	_RETREF( DDrawPrivate );
}

ULONG WINAPI IDirectDrawFake::Release()
{
	ULONG refcount = IDirectDrawFake_Release( this );
	if( refcount < 1 )
		delete this;

	return refcount;
}

ULONG WINAPI IDirectDrawFake_Release( IDirectDrawFake* This )
{
	GUARD( This, 0 );
	ULONG ref = _GETREF( DDrawPrivate );
	_DECREF( DDrawPrivate, PVT_IDirectDrawFake_Uninitialize );
	return ref-1;
}

HRESULT WINAPI IDirectDrawFake::QueryInterface( REFIID riid, LPVOID FAR * ppvObj )
{
	return IDirectDrawFake_QueryInterface( this, riid, ppvObj );
}

HRESULT WINAPI IDirectDrawFake_QueryInterface( IDirectDrawFake* This, REFIID riid, LPVOID FAR * ppvObj )
{
	if( riid == IID_IDirect3D7 )
	{
		extern HRESULT PVT_Direct3DFakeCreate( LPVOID* ppvObj, D3D11* d3d );
		return PVT_Direct3DFakeCreate( ppvObj, ACCESS(DDrawPrivate)->pD3DContext );
	}

	return E_INVALIDARG;
}

HRESULT WINAPI IDirectDrawFake::Compact()
{
	return IDirectDrawFake_Compact(this);
}

HRESULT WINAPI IDirectDrawFake_Compact( IDirectDrawFake* This )
{
	return E_NOTIMPL;
}

HRESULT WINAPI IDirectDrawFake::CreateClipper( DWORD dwFlags, IDirectDrawClipperFake** lplpDDClipper, IUnknownFake* pUnkOuter )
{
	return IDirectDrawFake_CreateClipper( this, dwFlags, lplpDDClipper, pUnkOuter );
}

HRESULT WINAPI IDirectDrawFake_CreateClipper( IDirectDrawFake* This, DWORD dwFlags, IDirectDrawClipperFake** lplpDDClipper, IUnknownFake* pUnkOuter )
{
	return DirectDrawFakeCreateClipper( dwFlags, lplpDDClipper, pUnkOuter );
}

HRESULT WINAPI IDirectDrawFake::CreatePalette( DWORD dwFlags, LPPALETTEENTRY lpDDColorArray, IDirectDrawPaletteFake** lplpDDPalette, IUnknownFake* pUnkOuter )
{
	return IDirectDrawFake_CreatePalette( this, dwFlags, lpDDColorArray, lplpDDPalette, pUnkOuter );
}

HRESULT WINAPI IDirectDrawFake_CreatePalette( IDirectDrawFake* This, DWORD dwFlags, LPPALETTEENTRY lpDDColorArray, IDirectDrawPaletteFake** lplpDDPalette, IUnknownFake* pUnkOuter )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawFake::CreateSurface(LPDDSURFACEDESC2 lpDDSurfaceDesc2, IDirectDrawSurfaceFake** lplpDDSurface, IUnknownFake* pUnkOuter)
{
	return IDirectDrawFake_CreateSurface( this, lpDDSurfaceDesc2, lplpDDSurface, pUnkOuter );
}

HRESULT WINAPI IDirectDrawFake_CreateSurface( IDirectDrawFake* This, LPDDSURFACEDESC2 lpDDSurfaceDesc2, IDirectDrawSurfaceFake** lplpDDSurface, IUnknownFake* pUnkOuter ) 
{
	GUARD( This, E_FAIL );

	/* Sanity check */
	if( !lplpDDSurface )
		return E_INVALIDARG;

	/* Allocate fake DirectDraw interface and initialize Vtable for C */
	(*lplpDDSurface ) = new IDirectDrawSurfaceFake;
	if( !(*lplpDDSurface ) )
		return E_OUTOFMEMORY;

	if( !DDrawSurfaceVtableCreate( lplpDDSurface ) )
		return E_OUTOFMEMORY;

	(*lplpDDSurface)->reserved = new DDrawSurfacePrivate;
	if( !(*lplpDDSurface)->reserved )
		return E_OUTOFMEMORY;

	memset( (*lplpDDSurface)->reserved, 0, sizeof( DDrawSurfacePrivate ) );

	HRESULT hr = D3D11Func_CreateSurface( ACCESS(DDrawPrivate)->pD3DContext, &_ACCESS(DDrawSurfacePrivate, (*lplpDDSurface))->pSurface, lpDDSurfaceDesc2 );
	if( FAILED( hr ) )
		return hr;

	_ACCESS(DDrawSurfacePrivate, (*lplpDDSurface))->pParentD3DContext = ACCESS(DDrawPrivate)->pD3DContext;
	_ACCESS(DDrawSurfacePrivate, (*lplpDDSurface))->RefCount = 1;

	return DD_OK;
}

HRESULT WINAPI IDirectDrawFake::DuplicateSurface( IDirectDrawSurfaceFake* lpDDSurface, IDirectDrawSurfaceFake** lplpDupDDSurface)
{
	return IDirectDrawFake_DuplicateSurface( this, lpDDSurface, lplpDupDDSurface );
}
HRESULT WINAPI IDirectDrawFake_DuplicateSurface( IDirectDrawFake* This, IDirectDrawSurfaceFake* lpDDSurface, IDirectDrawSurfaceFake** lplpDupDDSurface ) 
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawFake::EnumDisplayModes( DWORD dwFlags, LPDDSURFACEDESC2 lpDDSurfaceDesc2, LPVOID lpContext, LPDDENUMMODESCALLBACK2 lpEnumModesCallback )
{
	return IDirectDrawFake_EnumDisplayModes( this, dwFlags, lpDDSurfaceDesc2, lpContext, lpEnumModesCallback );
}

HRESULT WINAPI IDirectDrawFake_EnumDisplayModes( IDirectDrawFake* This, DWORD dwFlags, LPDDSURFACEDESC2 lpDDSurfaceDesc2, LPVOID lpContext, LPDDENUMMODESCALLBACK2 lpEnumModesCallback )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawFake::EnumSurfaces( DWORD dwFlags, LPDDSURFACEDESC2 lpDDSD2, LPVOID lpContext, LPDDENUMSURFACESCALLBACK7 lpEnumSurfacesCallback )
{
	return IDirectDrawFake_EnumSurfaces( this, dwFlags, lpDDSD2, lpContext, lpEnumSurfacesCallback );
}

HRESULT WINAPI IDirectDrawFake_EnumSurfaces( IDirectDrawFake* This, DWORD dwFlags, LPDDSURFACEDESC2 lpDDSD2, LPVOID lpContext, LPDDENUMSURFACESCALLBACK7 lpEnumSurfacesCallback )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawFake::EvaluateMode(DWORD dwFlags, DWORD* pSecondsUntilTimeout)
{
	return IDirectDrawFake_EvaluateMode( this, dwFlags, pSecondsUntilTimeout );
}

HRESULT WINAPI IDirectDrawFake_EvaluateMode( IDirectDrawFake* This, DWORD dwFlags, DWORD *pSecondsUntilTimeout )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawFake::FlipToGDISurface()
{
	return IDirectDrawFake_FlipToGDISurface( this );
}

HRESULT WINAPI IDirectDrawFake_FlipToGDISurface( IDirectDrawFake* This )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawFake::GetAvailableVidMem( LPDDSCAPS2 lpDDSCaps2, LPDWORD lpdwTotal, LPDWORD lpdwFree )
{
	return IDirectDrawFake_GetAvailableVidMem( this, lpDDSCaps2, lpdwTotal, lpdwFree );
}

HRESULT WINAPI IDirectDrawFake_GetAvailableVidMem( IDirectDrawFake* This, LPDDSCAPS2 lpDDSCaps2, LPDWORD lpdwTotal, LPDWORD lpdwFree )
{
	return D3D11Func_GetAvailableVidMem( ACCESS(DDrawPrivate)->pD3DContext, lpDDSCaps2, lpdwTotal, lpdwFree );
}

HRESULT WINAPI IDirectDrawFake::GetCaps( LPDDCAPS lpDDDriverCaps, LPDDCAPS lpDDHELCaps )
{
	return IDirectDrawFake_GetCaps( this, lpDDDriverCaps, lpDDHELCaps );
}

HRESULT WINAPI IDirectDrawFake_GetCaps( IDirectDrawFake* This, LPDDCAPS lpDDDriverCaps, LPDDCAPS lpDDHELCaps )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawFake::GetDeviceIdentifier( LPDDDEVICEIDENTIFIER2 lpdddi, DWORD dwFlags )
{
	return IDirectDrawFake_GetDeviceIdentifier( this, lpdddi, dwFlags );
}

HRESULT WINAPI IDirectDrawFake_GetDeviceIdentifier( IDirectDrawFake* This, LPDDDEVICEIDENTIFIER2 lpdddi, DWORD dwFlags )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawFake::GetDisplayMode( LPDDSURFACEDESC2 lpDDSurfaceDesc2  )
{
	return IDirectDrawFake_GetDisplayMode( this, lpDDSurfaceDesc2 );
}

HRESULT WINAPI IDirectDrawFake_GetDisplayMode( IDirectDrawFake* This, LPDDSURFACEDESC2 lpDDSurfaceDesc2  )
{
	lpDDSurfaceDesc2->ddpfPixelFormat.dwRGBBitCount = 32;

	// TODO
	LOGUNIMPL(DD_OK);
}

HRESULT WINAPI IDirectDrawFake::GetFourCCCodes( LPDWORD lpNumCodes, LPDWORD lpCodes )
{
	return IDirectDrawFake_GetFourCCCodes( this, lpNumCodes, lpCodes );
}

HRESULT WINAPI IDirectDrawFake_GetFourCCCodes( IDirectDrawFake* This, LPDWORD lpNumCodes, LPDWORD lpCodes )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawFake::GetGDISurface( IDirectDrawSurfaceFake** lplpGDIDDSurface )
{
	return IDirectDrawFake_GetGDISurface( this, lplpGDIDDSurface );
}

HRESULT WINAPI IDirectDrawFake_GetGDISurface( IDirectDrawFake* This, IDirectDrawSurfaceFake** lplpGDIDDSurface )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawFake::GetMonitorFrequency( LPDWORD lpdwFrequency )
{
	return IDirectDrawFake_GetMonitorFrequency( this, lpdwFrequency );
}

HRESULT WINAPI IDirectDrawFake_GetMonitorFrequency( IDirectDrawFake* This, LPDWORD lpdwFrequency )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawFake::GetScanLine( LPDWORD lpdwScanLine )
{
	return IDirectDrawFake_GetScanLine( this, lpdwScanLine );
}

HRESULT WINAPI IDirectDrawFake_GetScanLine( IDirectDrawFake* This, LPDWORD lpdwScanLine )
{
	GUARD( This, E_FAIL );
	return D3D11Func_GetScanLine( ACCESS(DDrawPrivate)->pD3DContext, lpdwScanLine );
}

HRESULT WINAPI IDirectDrawFake::GetSurfaceFromDC( HDC hdc, IDirectDrawSurfaceFake** lpDDS )
{
	return IDirectDrawFake_GetSurfaceFromDC( this, hdc, lpDDS );
}

HRESULT WINAPI IDirectDrawFake_GetSurfaceFromDC( IDirectDrawFake* This, HDC hdc, IDirectDrawSurfaceFake** lpDDS )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawFake::GetVerticalBlankStatus( LPBOOL lpbIsInVB )
{
	return IDirectDrawFake_GetVerticalBlankStatus( this, lpbIsInVB );
}

HRESULT WINAPI IDirectDrawFake_GetVerticalBlankStatus( IDirectDrawFake* This, LPBOOL lpbIsInVB )
{
	GUARD( This, E_FAIL );
	return D3D11Func_GetVerticalBlankStatus( ACCESS(DDrawPrivate)->pD3DContext, lpbIsInVB );
}

HRESULT WINAPI IDirectDrawFake::Initialize( GUID FAR *lpGUID )
{
	return IDirectDrawFake_Initialize( this, lpGUID );
}

HRESULT WINAPI IDirectDrawFake_Initialize( IDirectDrawFake* This, GUID FAR *lpGUID )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawFake::RestoreAllSurfaces()
{
	return IDirectDrawFake_RestoreAllSurfaces(this);
}

HRESULT WINAPI IDirectDrawFake_RestoreAllSurfaces( IDirectDrawFake* This )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawFake::RestoreDisplayMode()
{
	return IDirectDrawFake_RestoreDisplayMode( this );
}

HRESULT WINAPI IDirectDrawFake_RestoreDisplayMode( IDirectDrawFake* This )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawFake::SetCooperativeLevel( HWND hWnd, DWORD dwFlags )
{
	return IDirectDrawFake_SetCooperativeLevel( this, hWnd, dwFlags );
}

HRESULT WINAPI IDirectDrawFake_SetCooperativeLevel( IDirectDrawFake* This, HWND hWnd, DWORD dwFlags )
{
	GUARD( This, E_FAIL );

	/* Save cooperative level flags for later, we're gonna need those. */
	ACCESS( DDrawPrivate )->dwCooperativeLevel = dwFlags;

	return D3D11Func_CreateDevice( &ACCESS( DDrawPrivate )->pD3DContext, hWnd );;
}

HRESULT WINAPI IDirectDrawFake::SetDisplayMode( DWORD dwWidth, DWORD dwHeight, DWORD dwBPP, DWORD dwRefreshRate, DWORD dwFlags )
{
	return IDirectDrawFake_SetDisplayMode( this, dwWidth, dwHeight, dwBPP, dwRefreshRate, dwFlags );
}

HRESULT WINAPI IDirectDrawFake_SetDisplayMode( IDirectDrawFake* This, DWORD dwWidth, DWORD dwHeight, DWORD dwBPP, DWORD dwRefreshRate, DWORD dwFlags )
{
	GUARD( This, E_FAIL );


	HRESULT hr = D3D11Func_SetDisplayMode( ACCESS( DDrawPrivate )->pD3DContext, dwWidth, dwHeight, dwBPP, dwRefreshRate, TRUE );
	if( FAILED( hr ) )
		return E_INVALIDARG;

	return DD_OK;
}

HRESULT WINAPI IDirectDrawFake::StartModeTest( LPSIZE lpModesToTest, DWORD dwNumEntries, DWORD dwFlags )
{
	return IDirectDrawFake_StartModeTest( this, lpModesToTest, dwNumEntries, dwFlags );
}

HRESULT WINAPI IDirectDrawFake_StartModeTest( IDirectDrawFake* This, LPSIZE lpModesToTest, DWORD dwNumEntries, DWORD dwFlags )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawFake::TestCooperativeLevel( void )
{
	return IDirectDrawFake_TestCooperativeLevel( this );
}

HRESULT WINAPI IDirectDrawFake_TestCooperativeLevel( IDirectDrawFake* This )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawFake::WaitForVerticalBlank( DWORD dwFlags, HANDLE hEvent )
{
	return IDirectDrawFake_WaitForVerticalBlank( this, dwFlags, hEvent );
}

HRESULT WINAPI IDirectDrawFake_WaitForVerticalBlank( IDirectDrawFake* This, DWORD dwFlags, HANDLE hEvent )
{
	return D3D11Func_WaitForVerticalBlank( ACCESS(DDrawPrivate)->pD3DContext, dwFlags );
}

/*
 * IDirectDrawSurfaceFake 
 */
ULONG WINAPI IDirectDrawSurfaceFake::AddRef()
{
	return IDirectDrawSurfaceFake_AddRef( this );
}

ULONG WINAPI IDirectDrawSurfaceFake_AddRef( IDirectDrawSurfaceFake* This )
{
	GUARD( This, 0 );
	_INCREF( DDrawSurfacePrivate );
	_RETREF( DDrawSurfacePrivate );
}

ULONG WINAPI IDirectDrawSurfaceFake::Release()
{
	ULONG refcount = IDirectDrawSurfaceFake_Release( this );
	if( refcount < 1 )
		delete this;

	return refcount;
}

ULONG WINAPI IDirectDrawSurfaceFake_Release( IDirectDrawSurfaceFake* This )
{
	GUARD( This, 0 );
	ULONG ref = _GETREF( DDrawSurfacePrivate );
	_DECREF( DDrawSurfacePrivate, PVT_IDirectDrawSurfaceFake_Uninitialize );
	return ref-1;
}

HRESULT WINAPI IDirectDrawSurfaceFake::QueryInterface( REFIID riid, LPVOID FAR * ppvObj )
{
	return IDirectDrawSurfaceFake_QueryInterface( this, riid, ppvObj );
}

HRESULT WINAPI IDirectDrawSurfaceFake_QueryInterface( IDirectDrawSurfaceFake* This, REFIID riid, LPVOID FAR * ppvObj )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::AddAttachedSurface( IDirectDrawSurfaceFake* lpDDSAttachedSurface )
{
	return IDirectDrawSurfaceFake_AddAttachedSurface( this, lpDDSAttachedSurface );
}

HRESULT WINAPI IDirectDrawSurfaceFake_AddAttachedSurface( IDirectDrawSurfaceFake* This, IDirectDrawSurfaceFake* lpDDSAttachedSurface )
{
	LOGUNIMPL(DD_OK);
}

HRESULT WINAPI IDirectDrawSurfaceFake::AddOverlayDirtyRect( LPRECT lpRect )
{
	return IDirectDrawSurfaceFake_AddOverlayDirtyRect( this, lpRect );
}

HRESULT WINAPI IDirectDrawSurfaceFake_AddOverlayDirtyRect( IDirectDrawSurfaceFake* This, LPRECT lpRect )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::Blt( LPRECT lpDestRect, IDirectDrawSurfaceFake* lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags, LPDDBLTFX lpDDBltFx )
{
	return IDirectDrawSurfaceFake_Blt( this, lpDestRect, lpDDSrcSurface, lpSrcRect, dwFlags, lpDDBltFx );
}

HRESULT WINAPI IDirectDrawSurfaceFake_Blt( IDirectDrawSurfaceFake* This, LPRECT lpDestRect, IDirectDrawSurfaceFake* lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags, LPDDBLTFX lpDDBltFx )
{
	GUARD( This, E_FAIL );

	D3D11* d3d = ACCESS(DDrawSurfacePrivate)->pParentD3DContext;

	/* If this is the back buffer being blitted to the front buffer, just call Present and get it over with */
	if( lpDDSrcSurface )
		if( SUCCEEDED( D3D11Func_LazyPresent( d3d, _ACCESS(DDrawSurfacePrivate,lpDDSrcSurface)->pSurface, ACCESS(DDrawSurfacePrivate)->pSurface ) ) )
			return DD_OK;

	return D3D11SurfaceFunc_Blt( ACCESS(DDrawSurfacePrivate)->pParentD3DContext, ACCESS(DDrawSurfacePrivate)->pSurface, lpDestRect, lpSrcRect, dwFlags, lpDDBltFx );
}

HRESULT WINAPI IDirectDrawSurfaceFake::BltBatch( LPDDBLTBATCH lpDDBltBatch, DWORD dwCount, DWORD dwFlags )
{
	return IDirectDrawSurfaceFake_BltBatch( this, lpDDBltBatch, dwCount, dwFlags );
}

HRESULT WINAPI IDirectDrawSurfaceFake_BltBatch( IDirectDrawSurfaceFake* This, LPDDBLTBATCH lpDDBltBatch, DWORD dwCount, DWORD dwFlags )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::BltFast( DWORD dwX,DWORD dwY, IDirectDrawSurfaceFake* lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwTrans )
{
	return IDirectDrawSurfaceFake_BltFast( this, dwX, dwY, lpDDSrcSurface, lpSrcRect, dwTrans );
}

HRESULT WINAPI IDirectDrawSurfaceFake_BltFast( IDirectDrawSurfaceFake* This, DWORD dwX,DWORD dwY, IDirectDrawSurfaceFake* lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwTrans )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::ChangeUniquenessValue()
{
	return IDirectDrawSurfaceFake_ChangeUniquenessValue( this );
}

HRESULT WINAPI IDirectDrawSurfaceFake_ChangeUniquenessValue( IDirectDrawSurfaceFake* This )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::DeleteAttachedSurface( DWORD dwFlags, IDirectDrawSurfaceFake* lpDDSAttachedSurface )
{
	return IDirectDrawSurfaceFake_DeleteAttachedSurface( this, dwFlags, lpDDSAttachedSurface );
}

HRESULT WINAPI IDirectDrawSurfaceFake_DeleteAttachedSurface( IDirectDrawSurfaceFake* This, DWORD dwFlags, IDirectDrawSurfaceFake* lpDDSAttachedSurface )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::EnumAttachedSurfaces( LPVOID lpContext, LPDDENUMSURFACESCALLBACK7 lpEnumSurfacesCallback )
{
	return IDirectDrawSurfaceFake_EnumAttachedSurfaces( this, lpContext, lpEnumSurfacesCallback );
}

HRESULT WINAPI IDirectDrawSurfaceFake_EnumAttachedSurfaces( IDirectDrawSurfaceFake* This, LPVOID lpContext, LPDDENUMSURFACESCALLBACK7 lpEnumSurfacesCallback )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::EnumOverlayZOrders( DWORD dwFlags, LPVOID lpContext, LPDDENUMSURFACESCALLBACK7 lpfnCallback )
{
	return IDirectDrawSurfaceFake_EnumOverlayZOrders( this, dwFlags, lpContext, lpfnCallback );
}

HRESULT WINAPI IDirectDrawSurfaceFake_EnumOverlayZOrders( IDirectDrawSurfaceFake* This, DWORD dwFlags, LPVOID lpContext, LPDDENUMSURFACESCALLBACK7 lpfnCallback )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::Flip( IDirectDrawSurfaceFake* lpDDSurfaceTargetOverride, DWORD dwFlags )
{
	return IDirectDrawSurfaceFake_Flip( this, lpDDSurfaceTargetOverride, dwFlags );
}

HRESULT WINAPI IDirectDrawSurfaceFake_Flip( IDirectDrawSurfaceFake* This, IDirectDrawSurfaceFake* lpDDSurfaceTargetOverride, DWORD dwFlags )
{
	GUARD( This, E_FAIL );
	return D3D11Func_Present( ACCESS(DDrawSurfacePrivate)->pParentD3DContext );
}

HRESULT WINAPI IDirectDrawSurfaceFake::FreePrivateData( REFGUID guidTag )
{
	return IDirectDrawSurfaceFake_FreePrivateData( this, guidTag );
}

HRESULT WINAPI IDirectDrawSurfaceFake_FreePrivateData( IDirectDrawSurfaceFake* This, REFGUID guidTag )
{
	LOGUNIMPL_F;
}
 
HRESULT WINAPI IDirectDrawSurfaceFake::GetAttachedSurface( LPDDSCAPS2 lpDDSCaps, IDirectDrawSurfaceFake* FAR *lplpDDAttachedSurface )
{
	return IDirectDrawSurfaceFake_GetAttachedSurface( this, lpDDSCaps, lplpDDAttachedSurface );
}

HRESULT WINAPI IDirectDrawSurfaceFake_GetAttachedSurface( IDirectDrawSurfaceFake* This, LPDDSCAPS2 lpDDSCaps, IDirectDrawSurfaceFake* FAR *lplpDDAttachedSurface )
{
	GUARD( This, E_FAIL );

	/* Sanity check */
	if( !lplpDDAttachedSurface )
		return E_INVALIDARG;

	/* TODO: Actually use GUIDs */

	/* Allocate fake DirectDraw interface and initialize Vtable for C */
	(*lplpDDAttachedSurface ) = new IDirectDrawSurfaceFake;
	if( !(*lplpDDAttachedSurface ) )
		return E_OUTOFMEMORY;

	if( !DDrawSurfaceVtableCreate( lplpDDAttachedSurface ) )
		return E_OUTOFMEMORY;

	(*lplpDDAttachedSurface)->reserved = new DDrawSurfacePrivate;
	if( !(*lplpDDAttachedSurface)->reserved )
		return E_OUTOFMEMORY;

	memset( (*lplpDDAttachedSurface)->reserved, 0, sizeof( DDrawSurfacePrivate ) );

	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof( DDSURFACEDESC2 ) );
	ddsd.dwFlags = DDSD_CAPS;
	memmove( &ddsd.ddsCaps, lpDDSCaps, sizeof( DDSCAPS2 ) );

	HRESULT hr = D3D11Func_CreateSurface( ACCESS(DDrawSurfacePrivate)->pParentD3DContext, &_ACCESS(DDrawSurfacePrivate, (*lplpDDAttachedSurface))->pSurface, &ddsd );
	if( FAILED( hr ) )
		return hr;

	_ACCESS(DDrawSurfacePrivate, (*lplpDDAttachedSurface))->pParentD3DContext = ACCESS(DDrawSurfacePrivate)->pParentD3DContext;
	_ACCESS(DDrawSurfacePrivate, (*lplpDDAttachedSurface))->RefCount = 1;

	return DD_OK;
}

HRESULT WINAPI IDirectDrawSurfaceFake::GetBltStatus( DWORD dwFlags )
{
	return IDirectDrawSurfaceFake_GetBltStatus( this, dwFlags );
}

HRESULT WINAPI IDirectDrawSurfaceFake_GetBltStatus( IDirectDrawSurfaceFake* This, DWORD dwFlags )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::GetCaps( LPDDSCAPS2 lpDDSCaps )
{
	return IDirectDrawSurfaceFake_GetCaps( this, lpDDSCaps );
}

HRESULT WINAPI IDirectDrawSurfaceFake_GetCaps( IDirectDrawSurfaceFake* This, LPDDSCAPS2 lpDDSCaps )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::GetClipper( IDirectDrawClipperFake* FAR *lplpDDClipper )
{
	return IDirectDrawSurfaceFake_GetClipper( this, lplpDDClipper );
}

HRESULT WINAPI IDirectDrawSurfaceFake_GetClipper( IDirectDrawSurfaceFake* This, IDirectDrawClipperFake* FAR *lplpDDClipper )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::GetColorKey( DWORD dwFlags, LPDDCOLORKEY lpDDColorKey )
{
	return IDirectDrawSurfaceFake_GetColorKey( this, dwFlags, lpDDColorKey );
}

HRESULT WINAPI IDirectDrawSurfaceFake_GetColorKey( IDirectDrawSurfaceFake* This, DWORD dwFlags, LPDDCOLORKEY lpDDColorKey )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::GetDC( HDC FAR *lphDC  )
{
	return IDirectDrawSurfaceFake_GetDC( this, lphDC );
}

HRESULT WINAPI IDirectDrawSurfaceFake_GetDC( IDirectDrawSurfaceFake* This, HDC FAR *lphDC  )
{
	GUARD( This, E_FAIL );
	return D3D11SurfaceFunc_GetDC( ACCESS(DDrawSurfacePrivate)->pSurface, lphDC );
}

HRESULT WINAPI IDirectDrawSurfaceFake::GetDDInterface( LPVOID FAR *lplpDD )
{
	return IDirectDrawSurfaceFake_GetDDInterface( this, lplpDD );
}

HRESULT WINAPI IDirectDrawSurfaceFake_GetDDInterface( IDirectDrawSurfaceFake* This, LPVOID FAR *lplpDD )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::GetFlipStatus( DWORD dwFlags )
{
	return IDirectDrawSurfaceFake_GetFlipStatus( this, dwFlags );
}

HRESULT WINAPI IDirectDrawSurfaceFake_GetFlipStatus( IDirectDrawSurfaceFake* This, DWORD dwFlags )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::GetLOD( LPDWORD lpdwLOD )
{
	return IDirectDrawSurfaceFake_GetLOD( this, lpdwLOD );
}

HRESULT WINAPI IDirectDrawSurfaceFake_GetLOD( IDirectDrawSurfaceFake* This, LPDWORD lpdwLOD )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::GetOverlayPosition( LPLONG lplX, LPLONG lplY )
{
	return IDirectDrawSurfaceFake_GetOverlayPosition( this, lplX, lplY );
}

HRESULT WINAPI IDirectDrawSurfaceFake_GetOverlayPosition( IDirectDrawSurfaceFake* This, LPLONG lplX, LPLONG lplY )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::GetPalette( IDirectDrawPaletteFake* FAR *lplpDDPalette )
{
	return IDirectDrawSurfaceFake_GetPalette( this, lplpDDPalette );
}

HRESULT WINAPI IDirectDrawSurfaceFake_GetPalette( IDirectDrawSurfaceFake* This, IDirectDrawPaletteFake* FAR *lplpDDPalette )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::GetPixelFormat( LPDDPIXELFORMAT lpDDPixelFormat  )
{
	return IDirectDrawSurfaceFake_GetPixelFormat( this, lpDDPixelFormat );
}

HRESULT WINAPI IDirectDrawSurfaceFake_GetPixelFormat( IDirectDrawSurfaceFake* This, LPDDPIXELFORMAT lpDDPixelFormat  )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::GetPriority( LPDWORD lpdwPriority)
{
	return IDirectDrawSurfaceFake_GetPriority( this, lpdwPriority );
}

HRESULT WINAPI IDirectDrawSurfaceFake_GetPriority( IDirectDrawSurfaceFake* This, LPDWORD lpdwPriority)
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::GetPrivateData( REFGUID guidTag,  LPVOID  lpBuffer, LPDWORD lpcbBufferSize )
{
	return IDirectDrawSurfaceFake_GetPrivateData( this, guidTag, lpBuffer, lpcbBufferSize );
}

HRESULT WINAPI IDirectDrawSurfaceFake_GetPrivateData( IDirectDrawSurfaceFake* This, REFGUID guidTag,  LPVOID  lpBuffer, LPDWORD lpcbBufferSize )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::GetSurfaceDesc( LPDDSURFACEDESC2 lpDDSurfaceDesc  )
{
	return IDirectDrawSurfaceFake_GetSurfaceDesc( this, lpDDSurfaceDesc );
}

HRESULT WINAPI IDirectDrawSurfaceFake_GetSurfaceDesc( IDirectDrawSurfaceFake* This, LPDDSURFACEDESC2 lpDDSurfaceDesc  )
{
	GUARD( This, E_FAIL );

	if( !lpDDSurfaceDesc )
		return E_INVALIDARG;

	memmove( lpDDSurfaceDesc, &ACCESS(DDrawSurfacePrivate)->ddsd, sizeof( DDSURFACEDESC2 ) );

	return DD_OK;
}

HRESULT WINAPI IDirectDrawSurfaceFake::GetUniquenessValue( LPDWORD lpValue )
{
	return IDirectDrawSurfaceFake_GetUniquenessValue( this, lpValue );
}

HRESULT WINAPI IDirectDrawSurfaceFake_GetUniquenessValue( IDirectDrawSurfaceFake* This, LPDWORD lpValue )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::Initialize( IDirectDrawFake* lpDD, LPDDSURFACEDESC2 lpDDSurfaceDesc )
{
	return IDirectDrawSurfaceFake_Initialize( this, lpDD, lpDDSurfaceDesc );
}

HRESULT WINAPI IDirectDrawSurfaceFake_Initialize( IDirectDrawSurfaceFake* This, IDirectDrawFake* lpDD, LPDDSURFACEDESC2 lpDDSurfaceDesc )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::IsLost()
{
	return IDirectDrawSurfaceFake_IsLost( this );
}

HRESULT WINAPI IDirectDrawSurfaceFake_IsLost( IDirectDrawSurfaceFake* This )
{
	GUARD( This, E_FAIL );
	return DD_OK;
}

HRESULT WINAPI IDirectDrawSurfaceFake::Lock( LPRECT lpDestRect, LPDDSURFACEDESC2 lpDDSurfaceDesc, DWORD dwFlags, HANDLE hEvent )
{
	return IDirectDrawSurfaceFake_Lock( this, lpDestRect, lpDDSurfaceDesc, dwFlags, hEvent );
}

HRESULT WINAPI IDirectDrawSurfaceFake_Lock( IDirectDrawSurfaceFake* This, LPRECT lpDestRect, LPDDSURFACEDESC2 lpDDSurfaceDesc, DWORD dwFlags, HANDLE hEvent )
{
	LOGUNIMPL(S_OK);
}

HRESULT WINAPI IDirectDrawSurfaceFake::PageLock( DWORD dwFlags )
{
	return IDirectDrawSurfaceFake_PageLock( this, dwFlags );
}

HRESULT WINAPI IDirectDrawSurfaceFake_PageLock( IDirectDrawSurfaceFake* This, DWORD dwFlags )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::PageUnlock( DWORD dwFlags )
{
	return IDirectDrawSurfaceFake_PageUnlock( this, dwFlags );
}

HRESULT WINAPI IDirectDrawSurfaceFake_PageUnlock( IDirectDrawSurfaceFake* This, DWORD dwFlags )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::ReleaseDC( HDC hDC )
{
	return IDirectDrawSurfaceFake_ReleaseDC( this, hDC );
}

HRESULT WINAPI IDirectDrawSurfaceFake_ReleaseDC( IDirectDrawSurfaceFake* This, HDC hDC )
{
	GUARD( This, E_FAIL );
	return D3D11SurfaceFunc_ReleaseDC( ACCESS(DDrawSurfacePrivate)->pSurface, &hDC );
}

HRESULT WINAPI IDirectDrawSurfaceFake::Restore()
{
	return IDirectDrawSurfaceFake_Restore(this);
}

HRESULT WINAPI IDirectDrawSurfaceFake_Restore( IDirectDrawSurfaceFake* This )
{
	return DD_OK;
}

HRESULT WINAPI IDirectDrawSurfaceFake::SetClipper( IDirectDrawClipperFake* lpDDClipper )
{
	return IDirectDrawSurfaceFake_SetClipper( this, lpDDClipper );
}

HRESULT WINAPI IDirectDrawSurfaceFake_SetClipper( IDirectDrawSurfaceFake* This, IDirectDrawClipperFake* lpDDClipper )
{
	GUARD( This, E_FAIL );
	return DD_OK;
}

HRESULT WINAPI IDirectDrawSurfaceFake::SetColorKey( DWORD dwFlags, LPDDCOLORKEY lpDDColorKey )
{
	return IDirectDrawSurfaceFake_SetColorKey( this, dwFlags, lpDDColorKey );
}

HRESULT WINAPI IDirectDrawSurfaceFake_SetColorKey( IDirectDrawSurfaceFake* This, DWORD dwFlags, LPDDCOLORKEY lpDDColorKey )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::SetLOD( DWORD dwMaxLOD )
{
	return IDirectDrawSurfaceFake_SetLOD( this, dwMaxLOD );
}

HRESULT WINAPI IDirectDrawSurfaceFake_SetLOD( IDirectDrawSurfaceFake* This, DWORD dwMaxLOD )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::SetOverlayPosition( LONG lX, LONG lY )
{
	return IDirectDrawSurfaceFake_SetOverlayPosition( this, lX, lY );
}

HRESULT WINAPI IDirectDrawSurfaceFake_SetOverlayPosition( IDirectDrawSurfaceFake* This, LONG lX, LONG lY )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::SetPalette( IDirectDrawPaletteFake* lpDDPalette )
{
	return IDirectDrawSurfaceFake_SetPalette( this, lpDDPalette );
}

HRESULT WINAPI IDirectDrawSurfaceFake_SetPalette( IDirectDrawSurfaceFake* This, IDirectDrawPaletteFake* lpDDPalette )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::SetPriority( DWORD dwPriority )
{
	return IDirectDrawSurfaceFake_SetPriority( this, dwPriority );
}

HRESULT WINAPI IDirectDrawSurfaceFake_SetPriority( IDirectDrawSurfaceFake* This, DWORD dwPriority )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::SetPrivateData( REFGUID guidTag, LPVOID  lpData, DWORD   cbSize, DWORD   dwFlags )
{
	return IDirectDrawSurfaceFake_SetPrivateData( this, guidTag, lpData, cbSize, dwFlags );
}

HRESULT WINAPI IDirectDrawSurfaceFake_SetPrivateData( IDirectDrawSurfaceFake* This, REFGUID guidTag, LPVOID  lpData, DWORD   cbSize, DWORD   dwFlags )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::SetSurfaceDesc( LPDDSURFACEDESC2 lpddsd2, DWORD dwFlags )
{
	return IDirectDrawSurfaceFake_SetSurfaceDesc( this, lpddsd2, dwFlags );
}

HRESULT WINAPI IDirectDrawSurfaceFake_SetSurfaceDesc( IDirectDrawSurfaceFake* This, LPDDSURFACEDESC2 lpddsd2, DWORD dwFlags )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::Unlock( LPRECT lpRect )
{
	return IDirectDrawSurfaceFake_Unlock( this, lpRect );
}

HRESULT WINAPI IDirectDrawSurfaceFake_Unlock( IDirectDrawSurfaceFake* This, LPRECT lpRect )
{
	LOGUNIMPL(DD_OK);
}

HRESULT WINAPI IDirectDrawSurfaceFake::UpdateOverlay( LPRECT lpSrcRect, IDirectDrawSurfaceFake* lpDDDestSurface, LPRECT lpDestRect, DWORD dwFlags, LPDDOVERLAYFX lpDDOverlayFx )
{
	return IDirectDrawSurfaceFake_UpdateOverlay( this, lpSrcRect, lpDDDestSurface, lpDestRect, dwFlags, lpDDOverlayFx );
}

HRESULT WINAPI IDirectDrawSurfaceFake_UpdateOverlay( IDirectDrawSurfaceFake* This, LPRECT lpSrcRect, IDirectDrawSurfaceFake* lpDDDestSurface, LPRECT lpDestRect, DWORD dwFlags, LPDDOVERLAYFX lpDDOverlayFx )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::UpdateOverlayDisplay( DWORD dwFlags )
{
	return IDirectDrawSurfaceFake_UpdateOverlayDisplay( this, dwFlags );
}

HRESULT WINAPI IDirectDrawSurfaceFake_UpdateOverlayDisplay( IDirectDrawSurfaceFake* This, DWORD dwFlags )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawSurfaceFake::UpdateOverlayZOrder( DWORD dwFlags, IDirectDrawSurfaceFake* lpDDSReference )
{
	return IDirectDrawSurfaceFake_UpdateOverlayZOrder( this, dwFlags, lpDDSReference );
}

HRESULT WINAPI IDirectDrawSurfaceFake_UpdateOverlayZOrder( IDirectDrawSurfaceFake* This, DWORD dwFlags, IDirectDrawSurfaceFake* lpDDSReference )
{
	LOGUNIMPL_F;
}


/*
 * IDirectDrawClipperFake 
 */
ULONG WINAPI IDirectDrawClipperFake::AddRef()
{
	return IDirectDrawClipperFake_AddRef(this);
}

ULONG WINAPI IDirectDrawClipperFake_AddRef( IDirectDrawClipperFake* This )
{
	GUARD( This, 0 );
	_INCREF( DDrawClipperPrivate );
	_RETREF( DDrawClipperPrivate );;
}

ULONG WINAPI IDirectDrawClipperFake::Release() 
{
	ULONG refcount = IDirectDrawClipperFake_Release( this );
	if( refcount < 1 )
		delete this;

	return refcount;
}

ULONG WINAPI IDirectDrawClipperFake_Release( IDirectDrawClipperFake* This )
{
	GUARD( This, 0 );
	ULONG ref = _GETREF( DDrawClipperPrivate );
	_DECREF( DDrawClipperPrivate, PVT_IDirectDrawClipperFake_Uninitialize );
	return ref-1;
}

HRESULT WINAPI IDirectDrawClipperFake::QueryInterface( REFIID riid, LPVOID* ppvObj )
{
	return IDirectDrawClipperFake_QueryInterface( this, riid, ppvObj );
}

HRESULT WINAPI IDirectDrawClipperFake_QueryInterface( IDirectDrawClipperFake* This, REFIID riid, LPVOID FAR * ppvObj )
{
	return E_INVALIDARG;
}

HRESULT WINAPI IDirectDrawClipperFake::GetClipList( LPRECT lpRect, LPRGNDATA lpClipList, LPDWORD lpdwSize )
{
	return IDirectDrawClipperFake_GetClipList( this, lpRect, lpClipList, lpdwSize );
}

HRESULT WINAPI IDirectDrawClipperFake_GetClipList( IDirectDrawClipperFake* This, LPRECT lpRect, LPRGNDATA lpClipList, LPDWORD lpdwSize )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawClipperFake::GetHWnd( HWND FAR* lphWnd )
{
	return IDirectDrawClipperFake_GetHWnd( this, lphWnd );
}

HRESULT WINAPI IDirectDrawClipperFake_GetHWnd( IDirectDrawClipperFake* This, HWND FAR *lphWnd )
{
	GUARD( This, E_FAIL );

	*lphWnd = ACCESS( DDrawClipperPrivate )->hWnd;
	
	return DD_OK;
}

HRESULT WINAPI IDirectDrawClipperFake::Initialize( IDirectDrawFake* lpDD, DWORD dwFlags )
{
	return IDirectDrawClipperFake_Initialize( this, lpDD, dwFlags );
}

HRESULT WINAPI IDirectDrawClipperFake_Initialize( IDirectDrawClipperFake* This, IDirectDrawFake* lpDD, DWORD dwFlags )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawClipperFake::IsClipListChanged( BOOL FAR *lpbChanged )
{
	return IDirectDrawClipperFake_IsClipListChanged( this, lpbChanged );
}

HRESULT WINAPI IDirectDrawClipperFake_IsClipListChanged( IDirectDrawClipperFake* This, BOOL FAR *lpbChanged )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawClipperFake::SetClipList( LPRGNDATA lpClipList, DWORD dwFlags )
{
	return  IDirectDrawClipperFake_SetClipList( this, lpClipList, dwFlags );
}

HRESULT WINAPI IDirectDrawClipperFake_SetClipList( IDirectDrawClipperFake* This, LPRGNDATA lpClipList, DWORD dwFlags )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawClipperFake::SetHWnd( DWORD dwFlags, HWND hWnd )
{ 
	return IDirectDrawClipperFake_SetHWnd( this, dwFlags, hWnd );
}

HRESULT WINAPI IDirectDrawClipperFake_SetHWnd( IDirectDrawClipperFake* This, DWORD dwFlags, HWND hWnd )
{
	GUARD( This, E_FAIL );

	ACCESS( DDrawClipperPrivate )->hWnd = hWnd;

	return DD_OK;
}


/*
 * IDirectDrawPaletteFake
 */
ULONG WINAPI IDirectDrawPaletteFake_AddRef( IDirectDrawPaletteFake* This )
{
	GUARD( This, 0 );
	_INCREF( DDrawClipperPrivate );
	_RETREF( DDrawClipperPrivate );;
}

ULONG WINAPI IDirectDrawPaletteFake::Release()
{
	ULONG refcount = IDirectDrawPaletteFake_Release( this );
	if( refcount < 1 )
		delete this;

	return refcount;
}

ULONG WINAPI IDirectDrawPaletteFake_Release( IDirectDrawPaletteFake* This )
{
	GUARD( This, 0 );
	ULONG ref = _GETREF( DDrawPalettePrivate );
	_DECREF( DDrawPalettePrivate, PVT_IDirectDrawPaletteFake_Uninitialize );
	return ref-1;
}

HRESULT WINAPI IDirectDrawPaletteFake::QueryInterface( REFIID riid, LPVOID FAR * ppvObj )
{
	return IDirectDrawPaletteFake_QueryInterface( this, riid, ppvObj );
}

HRESULT WINAPI IDirectDrawPaletteFake_QueryInterface( IDirectDrawPaletteFake* This, REFIID riid, LPVOID FAR * ppvObj )
{
	return E_INVALIDARG;
}

HRESULT WINAPI IDirectDrawPaletteFake::GetCaps( LPDWORD lpdwCaps )
{
	return IDirectDrawPaletteFake_GetCaps( this, lpdwCaps );
}

HRESULT WINAPI IDirectDrawPaletteFake_GetCaps( IDirectDrawPaletteFake* This, LPDWORD lpdwCaps )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawPaletteFake::GetEntries( DWORD dwFlags, DWORD dwBase, DWORD dwNumEntries, LPPALETTEENTRY lpEntries )
{
	return IDirectDrawPaletteFake_GetEntries(this, dwFlags, dwBase, dwNumEntries, lpEntries );
}

HRESULT WINAPI IDirectDrawPaletteFake_GetEntries( IDirectDrawPaletteFake* This, DWORD dwFlags, DWORD dwBase, DWORD dwNumEntries, LPPALETTEENTRY lpEntries )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawPaletteFake::Initialize( IDirectDrawFake* lpDD, DWORD dwFlags, LPPALETTEENTRY lpDDColorTable )
{
	return IDirectDrawPaletteFake_Initialize( this, lpDD, dwFlags, lpDDColorTable );
}

HRESULT WINAPI IDirectDrawPaletteFake_Initialize(  IDirectDrawPaletteFake* This, IDirectDrawFake* lpDD, DWORD dwFlags, LPPALETTEENTRY lpDDColorTable )
{
	LOGUNIMPL_F;
}

HRESULT WINAPI IDirectDrawPaletteFake::SetEntries( DWORD dwFlags, DWORD dwStartingEntry, DWORD dwCount, LPPALETTEENTRY lpEntries )
{
	return IDirectDrawPaletteFake_SetEntries( this, dwFlags, dwStartingEntry, dwCount, lpEntries );
}

HRESULT WINAPI IDirectDrawPaletteFake_SetEntries( IDirectDrawPaletteFake* This, DWORD dwFlags, DWORD dwStartingEntry, DWORD dwCount, LPPALETTEENTRY lpEntries )
{
	LOGUNIMPL_F;
}






HRESULT WINAPI DirectDrawFakeCreate( GUID FAR *lpGUID, IDirectDrawFake FAR **lplpDD, IUnknownFake FAR *pUnkOuter )
{
	/* Sanity check */
	if( !lplpDD )
		return E_INVALIDARG;

	/* TODO: Actually use GUIDs */

	/* Allocate fake DirectDraw interface and initialize Vtable for C */
	(*lplpDD ) = new IDirectDrawFake;
	if( !(*lplpDD ) )
		return E_OUTOFMEMORY;

	if( !DDrawVtableCreate( lplpDD ) )
		return E_OUTOFMEMORY;

	(*lplpDD)->reserved = new DDrawPrivate;
	if( !(*lplpDD)->reserved )
		return E_OUTOFMEMORY;

	memset( (*lplpDD)->reserved, 0, sizeof( DDrawPrivate ) );

	//&_ACCESS( DDrawPrivate, (*lplpDD ) )->pD3DContext->ddrawguid = lpGUID;

	if( FAILED( D3D11Func_Initialize( &_ACCESS( DDrawPrivate, (*lplpDD) )->pD3DContext ) ) )
		E_FAIL;

	_ACCESS( DDrawPrivate, (*lplpDD) )->RefCount = 1;

	return S_OK;
}

HRESULT WINAPI DirectDrawFakeCreateEx( GUID FAR* lpGuid, LPVOID* lplpDD, REFIID  iid, IUnknownFake FAR* pUnkOuter )
{
	return DirectDrawFakeCreate( lpGuid, (IDirectDrawFake**) lplpDD, pUnkOuter );	// Laziness...
}

HRESULT WINAPI DirectDrawFakeCreateClipper( DWORD dwFlags, IDirectDrawClipperFake FAR **lplpDDClipper, IUnknownFake FAR *pUnkOuter )
{
	/* Sanity check */
	if( !lplpDDClipper )
		return E_INVALIDARG;

	/* TODO: Actually use GUIDs */

	/* Allocate fake DirectDraw interface and initialize Vtable for C */
	(*lplpDDClipper ) = new IDirectDrawClipperFake;
	if( !(*lplpDDClipper ) )
		return E_OUTOFMEMORY;

	if( !DDrawClipperVtableCreate( lplpDDClipper ) )
		return E_OUTOFMEMORY;

	(*lplpDDClipper)->reserved = new DDrawPrivate;
	if( !(*lplpDDClipper)->reserved )
		return E_OUTOFMEMORY;

	memset( (*lplpDDClipper)->reserved, 0, sizeof( DDrawPrivate ) );

	_ACCESS( DDrawClipperPrivate, (*lplpDDClipper ) )->hWnd = NULL;
	_ACCESS( DDrawClipperPrivate, (*lplpDDClipper) )->RefCount = 1;

	return DD_OK;
}