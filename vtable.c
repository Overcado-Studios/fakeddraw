#include "framework.h"
#include <ddraw.h>
#include <d3d.h>
#include "fakeddraw.h"
#include "faked3d.h"

#include "vtable.h"

/*
 * Fake DirectDraw interface vtables 
 */
/*struct IDirectDrawFakeVtbl
{
	HRESULT (WINAPI* QueryInterface)( IDirectDrawFake*, REFIID, LPVOID );
	ULONG (WINAPI* AddRef)( IDirectDrawFake* );
	ULONG (WINAPI* Release)( IDirectDrawFake* );

	HRESULT (WINAPI* Compact)( IDirectDrawFake* );
	HRESULT (WINAPI* CreateClipper)( IDirectDrawFake*, DWORD, IDirectDrawClipperFake**, IUnknown* );
	HRESULT (WINAPI* CreatePalette)( IDirectDrawFake*, DWORD, LPPALETTEENTRY, IDirectDrawPaletteFake**, IUnknown* );
	HRESULT (WINAPI* CreateSurface)( IDirectDrawFake*, LPDDSURFACEDESC2, IDirectDrawSurfaceFake**, IUnknown* ) ;
	HRESULT (WINAPI* DuplicateSurface)( IDirectDrawFake*, IDirectDrawSurfaceFake*, IDirectDrawSurfaceFake** ) ;
	HRESULT (WINAPI* EnumDisplayModes)( IDirectDrawFake*, DWORD, LPDDSURFACEDESC2, LPVOID, LPDDENUMMODESCALLBACK2 );
	HRESULT (WINAPI* EnumSurfaces)( IDirectDrawFake*, DWORD, LPDDSURFACEDESC2, LPVOID, LPDDENUMSURFACESCALLBACK2 );
	HRESULT (WINAPI* EvaluateMode)( IDirectDrawFake*, DWORD, DWORD );
	HRESULT (WINAPI* FlipToGDISurface)( IDirectDrawFake* );
	HRESULT (WINAPI* GetAvailableVidMem)( IDirectDrawFake*, LPDDSCAPS2, LPDWORD, LPDWORD );
	HRESULT (WINAPI* GetCaps)( IDirectDrawFake*, LPDDCAPS, LPDDCAPS );
	HRESULT (WINAPI* GetDeviceIdentifier)( IDirectDrawFake*, LPDDDEVICEIDENTIFIER2, DWORD );
	HRESULT (WINAPI* GetDisplayMode)( IDirectDrawFake*, LPDDSURFACEDESC2 );
	HRESULT (WINAPI* GetFourCCCodes)( IDirectDrawFake*, LPDWORD, LPDWORD );
	HRESULT (WINAPI* GetGDISurface)( IDirectDrawFake*, IDirectDrawSurfaceFake** );
	HRESULT (WINAPI* GetMonitorFrequency)( IDirectDrawFake*, LPDWORD );
	HRESULT (WINAPI* GetScanLine)( IDirectDrawFake*, LPDWORD );
	HRESULT (WINAPI* GetSurfaceFromDC)( IDirectDrawFake*, HDC, IDirectDrawSurfaceFake** );
	HRESULT (WINAPI* GetVerticalBlankStatus)( IDirectDrawFake*, LPBOOL );
	HRESULT (WINAPI* Initialize)( IDirectDrawFake*, GUID FAR );
	HRESULT (WINAPI* RestoreAllSurfaces)( IDirectDrawFake* );
	HRESULT (WINAPI* RestoreDisplayMode)( IDirectDrawFake* );
	HRESULT (WINAPI* SetCooperativeLevel)( IDirectDrawFake*, HWND, DWORD dwFlags );
	HRESULT (WINAPI* SetDisplayMode)( IDirectDrawFake*, DWORD, DWORD, DWORD, DWORD, DWORD );
	HRESULT (WINAPI* StartModeTest)( IDirectDrawFake*, LPSIZE, DWORD, DWORD );
	HRESULT (WINAPI* TestCooperativeLevel)( IDirectDrawFake* );
	HRESULT (WINAPI* WaitForFerticalBlank)( IDirectDrawFake*, DWORD, HANDLE );
};*/


/*
 * Fake DirectDraw Surface interface vtables 
 */
/*struct IDirectDrawSurfaceFakeVtbl
{
	HRESULT (WINAPI* QueryInterface)( IDirectDrawSurfaceFake*, REFIID, LPVOID );
	ULONG (WINAPI* AddRef)( IDirectDrawSurfaceFake* This );
	ULONG (WINAPI* Release)( IDirectDrawSurfaceFake* This );

	HRESULT (WINAPI* AddAttachedSurface)( IDirectDrawSurfaceFake* This, IDirectDrawSurfaceFake* lpDDSAttachedSurface );
	HRESULT (WINAPI* AddOverlayDirtyRect)( IDirectDrawSurfaceFake* This, LPRECT lpRect );
	HRESULT (WINAPI* Blt)( IDirectDrawSurfaceFake* This, LPRECT lpDestRect, IDirectDrawSurfaceFake* lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags, LPDDBLTFX lpDDBltFx );
	HRESULT (WINAPI* BltBatch)( IDirectDrawSurfaceFake* This, LPDDBLTBATCH lpDDBltBatch, DWORD dwCount, DWORD dwFlags );
	HRESULT (WINAPI* BltFast)( IDirectDrawSurfaceFake* This, DWORD dwX,DWORD dwY, IDirectDrawSurfaceFake* lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwTrans );
	HRESULT (WINAPI* ChangeUniquenessValue)( IDirectDrawSurfaceFake* This );
	HRESULT (WINAPI* DeleteAttachedSurface)( IDirectDrawSurfaceFake* This, DWORD dwFlags, IDirectDrawSurfaceFake* lpDDSAttachedSurface );
	HRESULT (WINAPI* EnumAttachedSurfaces)( IDirectDrawSurfaceFake* This, LPVOID lpContext, LPDDENUMSURFACESCALLBACK2 lpEnumSurfacesCallback );
	HRESULT (WINAPI* EnumOverlayZOrders)( IDirectDrawSurfaceFake* This, DWORD dwFlags, LPVOID lpContext, LPDDENUMSURFACESCALLBACK2 lpfnCallback );
	HRESULT (WINAPI* Flip)( IDirectDrawSurfaceFake* This, IDirectDrawSurfaceFake* lpDDSurfaceTargetOverride, DWORD dwFlags );
	HRESULT (WINAPI* FreePrivateData)( IDirectDrawSurfaceFake* This, REFGUID guidTag ); 
	HRESULT (WINAPI* GetAttachedSurface)( IDirectDrawSurfaceFake* This, LPDDSCAPS2 lpDDSCaps, IDirectDrawSurfaceFake* FAR *lplpDDAttachedSurface );
	HRESULT (WINAPI* GetBltStatus)( IDirectDrawSurfaceFake* This, DWORD dwFlags );
	HRESULT (WINAPI* GetCaps)( IDirectDrawSurfaceFake* This, LPDDSCAPS2 lpDDSCaps );
	HRESULT (WINAPI* GetClipper)( IDirectDrawSurfaceFake* This, IDirectDrawClipperFake* FAR *lplpDDClipper );
	HRESULT (WINAPI* GetColorKey)( IDirectDrawSurfaceFake* This, DWORD dwFlags, LPDDCOLORKEY lpDDColorKey );
	HRESULT (WINAPI* GetDC)( IDirectDrawSurfaceFake* This, HDC FAR *lphDC  );
	HRESULT (WINAPI* GetDDInterface)( IDirectDrawSurfaceFake* This, LPVOID FAR *lplpDD );
	HRESULT (WINAPI* GetFlipStatus)( IDirectDrawSurfaceFake* This, DWORD dwFlags );
	HRESULT (WINAPI* GetOverlayPosition)( IDirectDrawSurfaceFake* This, LPLONG lplX, LPLONG lplY );
	HRESULT (WINAPI* GetPalette)( IDirectDrawSurfaceFake* This, LPDIRECTDRAWPALETTE FAR *lplpDDPalette );
	HRESULT (WINAPI* GetPixelFormat)( IDirectDrawSurfaceFake* This, LPDDPIXELFORMAT lpDDPixelFormat  );
	HRESULT (WINAPI* GetPriority)( IDirectDrawSurfaceFake* This, LPDWORD lpdwPriority); 
	HRESULT (WINAPI* GetPrivateData)( IDirectDrawSurfaceFake* This, REFGUID guidTag,  LPVOID  lpBuffer, LPDWORD lpcbBufferSize );
	HRESULT (WINAPI* GetSurfaceDesc)( IDirectDrawSurfaceFake* This, LPDDSURFACEDESC2 lpDDSurfaceDesc  );
	HRESULT (WINAPI* GetUniquenessValue)( IDirectDrawSurfaceFake* This, LPDWORD lpValue ); 
	HRESULT (WINAPI* Initialize)( IDirectDrawSurfaceFake* This, LPDIRECTDRAW lpDD, LPDDSURFACEDESC2 lpDDSurfaceDesc );
	HRESULT (WINAPI* IsLost)( IDirectDrawSurfaceFake* This );
	HRESULT (WINAPI* Lock)( IDirectDrawSurfaceFake* This, LPRECT lpDestRect, LPDDSURFACEDESC2 lpDDSurfaceDesc, DWORD dwFlags, HANDLE hEvent );
	HRESULT (WINAPI* PageLock)( IDirectDrawSurfaceFake* This, DWORD dwFlags );
	HRESULT (WINAPI* PageUnlock)( IDirectDrawSurfaceFake* This, DWORD dwFlags );
	HRESULT (WINAPI* ReleaseDC)( IDirectDrawSurfaceFake* This, HDC hDC );
	HRESULT (WINAPI* Restore)( IDirectDrawSurfaceFake* This );
	HRESULT (WINAPI* SetClipper)( IDirectDrawSurfaceFake* This, IDirectDrawClipperFake* lpDDClipper );
	HRESULT (WINAPI* SetColorKey)( IDirectDrawSurfaceFake* This, DWORD dwFlags, LPDDCOLORKEY lpDDColorKey  );
	HRESULT (WINAPI* SetLOD)( IDirectDrawSurfaceFake* This, DWORD dwMaxLOD );
	HRESULT (WINAPI* SetOverlayPosition)( IDirectDrawSurfaceFake* This, LONG lX, LONG lY );
	HRESULT (WINAPI* SetPalette)( IDirectDrawSurfaceFake* This, LPDIRECTDRAWPALETTE lpDDPalette );
	HRESULT (WINAPI* SetPriority)( IDirectDrawSurfaceFake* This, DWORD dwPriority );
	HRESULT (WINAPI* SetPrivateData)( IDirectDrawSurfaceFake* This, REFGUID guidTag, LPVOID  lpData, DWORD   cbSize, DWORD   dwFlags ); 
	HRESULT (WINAPI* SetSurfaceDesc)( IDirectDrawSurfaceFake* This, LPDDSURFACEDESC2 lpddsd2, DWORD dwFlags );
	HRESULT (WINAPI* Unlock)( IDirectDrawSurfaceFake* This, LPRECT lpRect );
	HRESULT (WINAPI* UpdateOverlay)( IDirectDrawSurfaceFake* This, LPRECT lpSrcRect, IDirectDrawSurfaceFake* lpDDDestSurface, LPRECT lpDestRect, DWORD dwFlags, LPDDOVERLAYFX lpDDOverlayFx );
	HRESULT (WINAPI* UpdateOverlayDisplay)( IDirectDrawSurfaceFake* This, DWORD dwFlags );
	HRESULT (WINAPI* UpdateOverlayZOrder)( IDirectDrawSurfaceFake* This, DWORD dwFlags, IDirectDrawSurfaceFake* lpDDSReference );
};*/


/*
 * Fake DirectDraw Clipper interface vtables 
 */
//struct IDirectDrawClipperFakeVtbl
//{
//	HRESULT (WINAPI* QueryInterface)( IDirectDrawClipperFake*, REFIID, LPVOID );
//	ULONG (WINAPI* AddRef)( IDirectDrawClipperFake* );
//	ULONG (WINAPI* Release)( IDirectDrawClipperFake* );
//
//	HRESULT (WINAPI* GetClipList)( IDirectDrawClipperFake*, LPRECT, LPRGNDATA, LPDWORD );
//	HRESULT (WINAPI* GetHWnd)( IDirectDrawClipperFake* , HWND* );
//	HRESULT (WINAPI* Initialize)( IDirectDrawClipperFake* , IDirectDrawFake*, DWORD );
//	HRESULT (WINAPI* IsClipListChanged)( IDirectDrawClipperFake* , BOOL* );
//	HRESULT (WINAPI* SetClipList)( IDirectDrawClipperFake*, LPRGNDATA, DWORD );
//	HRESULT (WINAPI* SetHWnd)( IDirectDrawClipperFake*, DWORD, HWND );
//};


/*
 * Fake DirectDraw Palette interface vtables 
 */
struct IDirectDrawPaletteFakeVtbl
{
	HRESULT (WINAPI* QueryInterface)( IDirectDrawPaletteFake*, REFIID, LPVOID );
	ULONG (WINAPI* AddRef)( IDirectDrawPaletteFake* );
	ULONG (WINAPI* Release)( IDirectDrawPaletteFake* );

	HRESULT (WINAPI* GetCaps)( IDirectDrawPaletteFake* This, LPDWORD lpdwCaps );
	HRESULT (WINAPI* GetEntries)( IDirectDrawPaletteFake* This, DWORD dwFlags, DWORD dwBase, DWORD dwNumEntries, LPPALETTEENTRY lpEntries );
	HRESULT (WINAPI* Initialize)(  IDirectDrawPaletteFake* This, IDirectDrawFake* lpDD, DWORD dwFlags, LPPALETTEENTRY lpDDColorTable );
	HRESULT (WINAPI* SetEntries)( IDirectDrawPaletteFake* This, DWORD dwFlags, DWORD dwStartingEntry, DWORD dwCount, LPPALETTEENTRY lpEntries );
};




int DDrawVtableCreate( IDirectDrawFake** lplpDDraw )
{
	if( !lplpDDraw || (*lplpDDraw) == NULL )
		return FALSE;

	(*lplpDDraw)->lpVtbl = malloc( sizeof( struct IDirectDrawFakeVtbl ) );
	if( (*lplpDDraw)->lpVtbl == NULL )
		return FALSE;

	(*lplpDDraw)->lpVtbl->AddRef = IDirectDrawFake_AddRef;
	(*lplpDDraw)->lpVtbl->Release = IDirectDrawFake_Release;
	(*lplpDDraw)->lpVtbl->QueryInterface = IDirectDrawFake_QueryInterface;

	(*lplpDDraw)->lpVtbl->Compact = IDirectDrawFake_Compact;
	(*lplpDDraw)->lpVtbl->CreateClipper = IDirectDrawFake_CreateClipper;
	(*lplpDDraw)->lpVtbl->CreatePalette = IDirectDrawFake_CreatePalette;
	(*lplpDDraw)->lpVtbl->CreateSurface = IDirectDrawFake_CreateSurface;
	(*lplpDDraw)->lpVtbl->DuplicateSurface = IDirectDrawFake_DuplicateSurface;
	(*lplpDDraw)->lpVtbl->EnumDisplayModes = IDirectDrawFake_EnumDisplayModes;
	(*lplpDDraw)->lpVtbl->EnumSurfaces = IDirectDrawFake_EnumSurfaces;
	(*lplpDDraw)->lpVtbl->EvaluateMode = IDirectDrawFake_EvaluateMode;
	(*lplpDDraw)->lpVtbl->FlipToGDISurface = IDirectDrawFake_FlipToGDISurface;
	(*lplpDDraw)->lpVtbl->GetAvailableVidMem = IDirectDrawFake_GetAvailableVidMem;
	(*lplpDDraw)->lpVtbl->GetCaps = IDirectDrawFake_GetCaps;
	(*lplpDDraw)->lpVtbl->GetDeviceIdentifier = IDirectDrawFake_GetDeviceIdentifier;
	(*lplpDDraw)->lpVtbl->GetDisplayMode = IDirectDrawFake_GetDisplayMode;
	(*lplpDDraw)->lpVtbl->GetFourCCCodes = IDirectDrawFake_GetFourCCCodes;
	(*lplpDDraw)->lpVtbl->GetGDISurface = IDirectDrawFake_GetGDISurface;
	(*lplpDDraw)->lpVtbl->GetMonitorFrequency = IDirectDrawFake_GetMonitorFrequency;
	(*lplpDDraw)->lpVtbl->GetScanLine = IDirectDrawFake_GetScanLine;
	(*lplpDDraw)->lpVtbl->GetSurfaceFromDC = IDirectDrawFake_GetSurfaceFromDC;
	(*lplpDDraw)->lpVtbl->GetVerticalBlankStatus = IDirectDrawFake_GetVerticalBlankStatus;
	(*lplpDDraw)->lpVtbl->Initialize = IDirectDrawFake_Initialize;
	(*lplpDDraw)->lpVtbl->RestoreAllSurfaces = IDirectDrawFake_RestoreAllSurfaces;
	(*lplpDDraw)->lpVtbl->RestoreDisplayMode = IDirectDrawFake_RestoreDisplayMode;
	(*lplpDDraw)->lpVtbl->SetCooperativeLevel = IDirectDrawFake_SetCooperativeLevel;
	(*lplpDDraw)->lpVtbl->SetDisplayMode = IDirectDrawFake_SetDisplayMode;
	(*lplpDDraw)->lpVtbl->StartModeTest = IDirectDrawFake_StartModeTest;
	(*lplpDDraw)->lpVtbl->TestCooperativeLevel = IDirectDrawFake_TestCooperativeLevel;
	(*lplpDDraw)->lpVtbl->WaitForVerticalBlank = IDirectDrawFake_WaitForVerticalBlank;

	return TRUE;
}

void DDrawVtableDelete( IDirectDrawFake** lplpDDraw )
{
	if( !lplpDDraw || (*lplpDDraw) == NULL )
		return;

	free( (*lplpDDraw)->lpVtbl );
	(*lplpDDraw)->lpVtbl = NULL;
}

int DDrawSurfaceVtableCreate( IDirectDrawSurfaceFake** lplpDDrawSurface )
{
	if( !lplpDDrawSurface || (*lplpDDrawSurface) == NULL )
		return FALSE;

	(*lplpDDrawSurface)->lpVtbl = malloc( sizeof( struct IDirectDrawSurfaceFakeVtbl ) );
	if( (*lplpDDrawSurface)->lpVtbl == NULL )
		return FALSE;

	(*lplpDDrawSurface)->lpVtbl->QueryInterface = IDirectDrawSurfaceFake_QueryInterface;
	(*lplpDDrawSurface)->lpVtbl->AddRef = IDirectDrawSurfaceFake_AddRef;
	(*lplpDDrawSurface)->lpVtbl->Release = IDirectDrawSurfaceFake_Release;

	(*lplpDDrawSurface)->lpVtbl->AddAttachedSurface = IDirectDrawSurfaceFake_AddAttachedSurface;
	(*lplpDDrawSurface)->lpVtbl->AddOverlayDirtyRect = IDirectDrawSurfaceFake_AddOverlayDirtyRect;
	(*lplpDDrawSurface)->lpVtbl->Blt = IDirectDrawSurfaceFake_Blt;
	(*lplpDDrawSurface)->lpVtbl->BltBatch = IDirectDrawSurfaceFake_BltBatch;
	(*lplpDDrawSurface)->lpVtbl->BltFast = IDirectDrawSurfaceFake_BltFast;
	(*lplpDDrawSurface)->lpVtbl->ChangeUniquenessValue = IDirectDrawSurfaceFake_ChangeUniquenessValue;
	(*lplpDDrawSurface)->lpVtbl->DeleteAttachedSurface = IDirectDrawSurfaceFake_DeleteAttachedSurface;
	(*lplpDDrawSurface)->lpVtbl->EnumAttachedSurfaces = IDirectDrawSurfaceFake_EnumAttachedSurfaces;
	(*lplpDDrawSurface)->lpVtbl->EnumOverlayZOrders = IDirectDrawSurfaceFake_EnumOverlayZOrders;
	(*lplpDDrawSurface)->lpVtbl->Flip = IDirectDrawSurfaceFake_Flip;
	(*lplpDDrawSurface)->lpVtbl->FreePrivateData = IDirectDrawSurfaceFake_FreePrivateData;
	(*lplpDDrawSurface)->lpVtbl->GetAttachedSurface = IDirectDrawSurfaceFake_GetAttachedSurface;
	(*lplpDDrawSurface)->lpVtbl->GetBltStatus = IDirectDrawSurfaceFake_GetBltStatus;
	(*lplpDDrawSurface)->lpVtbl->GetCaps = IDirectDrawSurfaceFake_GetCaps;
	(*lplpDDrawSurface)->lpVtbl->GetClipper = IDirectDrawSurfaceFake_GetClipper;
	(*lplpDDrawSurface)->lpVtbl->GetColorKey = IDirectDrawSurfaceFake_GetColorKey;
	(*lplpDDrawSurface)->lpVtbl->GetDC = IDirectDrawSurfaceFake_GetDC;
	(*lplpDDrawSurface)->lpVtbl->GetDDInterface = IDirectDrawSurfaceFake_GetDDInterface;
	(*lplpDDrawSurface)->lpVtbl->GetFlipStatus = IDirectDrawSurfaceFake_GetFlipStatus;
	(*lplpDDrawSurface)->lpVtbl->GetLOD = IDirectDrawSurfaceFake_GetLOD;
	(*lplpDDrawSurface)->lpVtbl->GetOverlayPosition = IDirectDrawSurfaceFake_GetOverlayPosition;
	(*lplpDDrawSurface)->lpVtbl->GetPalette = IDirectDrawSurfaceFake_GetPalette;
	(*lplpDDrawSurface)->lpVtbl->GetPixelFormat = IDirectDrawSurfaceFake_GetPixelFormat;
	(*lplpDDrawSurface)->lpVtbl->GetPriority = IDirectDrawSurfaceFake_GetPriority;
	(*lplpDDrawSurface)->lpVtbl->GetPrivateData = IDirectDrawSurfaceFake_GetPrivateData;
	(*lplpDDrawSurface)->lpVtbl->GetSurfaceDesc = IDirectDrawSurfaceFake_GetSurfaceDesc;
	(*lplpDDrawSurface)->lpVtbl->GetUniquenessValue = IDirectDrawSurfaceFake_GetUniquenessValue;
	(*lplpDDrawSurface)->lpVtbl->Initialize = IDirectDrawSurfaceFake_Initialize;
	(*lplpDDrawSurface)->lpVtbl->IsLost = IDirectDrawSurfaceFake_IsLost;
	(*lplpDDrawSurface)->lpVtbl->Lock = IDirectDrawSurfaceFake_Lock;
	(*lplpDDrawSurface)->lpVtbl->PageLock = IDirectDrawSurfaceFake_PageLock;
	(*lplpDDrawSurface)->lpVtbl->PageUnlock = IDirectDrawSurfaceFake_PageUnlock;
	(*lplpDDrawSurface)->lpVtbl->ReleaseDC = IDirectDrawSurfaceFake_ReleaseDC;
	(*lplpDDrawSurface)->lpVtbl->Restore = IDirectDrawSurfaceFake_Restore;
	(*lplpDDrawSurface)->lpVtbl->SetClipper = IDirectDrawSurfaceFake_SetClipper;
	(*lplpDDrawSurface)->lpVtbl->SetColorKey = IDirectDrawSurfaceFake_SetColorKey;
	(*lplpDDrawSurface)->lpVtbl->SetLOD = IDirectDrawSurfaceFake_SetLOD;
	(*lplpDDrawSurface)->lpVtbl->SetOverlayPosition = IDirectDrawSurfaceFake_SetOverlayPosition;
	(*lplpDDrawSurface)->lpVtbl->SetPalette = IDirectDrawSurfaceFake_SetPalette;
	(*lplpDDrawSurface)->lpVtbl->SetPriority = IDirectDrawSurfaceFake_SetPriority;
	(*lplpDDrawSurface)->lpVtbl->SetPrivateData = IDirectDrawSurfaceFake_SetPrivateData;
	(*lplpDDrawSurface)->lpVtbl->SetSurfaceDesc = IDirectDrawSurfaceFake_SetSurfaceDesc;
	(*lplpDDrawSurface)->lpVtbl->Unlock = IDirectDrawSurfaceFake_Unlock;
	(*lplpDDrawSurface)->lpVtbl->UpdateOverlay = IDirectDrawSurfaceFake_UpdateOverlay;
	(*lplpDDrawSurface)->lpVtbl->UpdateOverlayDisplay = IDirectDrawSurfaceFake_UpdateOverlayDisplay;
	(*lplpDDrawSurface)->lpVtbl->UpdateOverlayZOrder = IDirectDrawSurfaceFake_UpdateOverlayZOrder;

	return TRUE;
}

void DDrawSurfaceVtableDelete( IDirectDrawSurfaceFake** lplpDDrawSurface )
{
	if( !lplpDDrawSurface || (*lplpDDrawSurface) == NULL )
		return;

	free( (*lplpDDrawSurface)->lpVtbl );
	(*lplpDDrawSurface)->lpVtbl = NULL;
}

int DDrawClipperVtableCreate( IDirectDrawClipperFake** lplpDDrawClipper )
{
	if( !lplpDDrawClipper || (*lplpDDrawClipper) == NULL )
		return FALSE;

	(*lplpDDrawClipper)->lpVtbl = malloc( sizeof( struct IDirectDrawClipperFakeVtbl ) );
	if( (*lplpDDrawClipper)->lpVtbl == NULL )
		return FALSE;

	(*lplpDDrawClipper)->lpVtbl->QueryInterface = IDirectDrawClipperFake_QueryInterface;
	(*lplpDDrawClipper)->lpVtbl->AddRef = IDirectDrawClipperFake_AddRef;
	(*lplpDDrawClipper)->lpVtbl->Release = IDirectDrawClipperFake_Release;
	(*lplpDDrawClipper)->lpVtbl->GetClipList = IDirectDrawClipperFake_GetClipList;
	(*lplpDDrawClipper)->lpVtbl->GetHWnd = IDirectDrawClipperFake_GetHWnd;
	(*lplpDDrawClipper)->lpVtbl->Initialize = IDirectDrawClipperFake_Initialize;
	(*lplpDDrawClipper)->lpVtbl->IsClipListChanged = IDirectDrawClipperFake_IsClipListChanged;
	(*lplpDDrawClipper)->lpVtbl->SetClipList = IDirectDrawClipperFake_SetClipList;
	(*lplpDDrawClipper)->lpVtbl->SetHWnd = IDirectDrawClipperFake_SetHWnd;

	return TRUE;
}

void DDrawClipperVtableDelete( IDirectDrawClipperFake** lplpDDrawClipper )
{
	if( !lplpDDrawClipper || (*lplpDDrawClipper) == NULL )
		return;

	free( (*lplpDDrawClipper)->lpVtbl );
	(*lplpDDrawClipper)->lpVtbl = NULL;
}

int DDrawPaletteVtableCreate( IDirectDrawPaletteFake** lplpDDrawPalette )
{
	if( !lplpDDrawPalette || (*lplpDDrawPalette) == NULL )
		return FALSE;

	(*lplpDDrawPalette)->lpVtbl = malloc( sizeof( struct IDirectDrawPaletteFakeVtbl ) );
	if( (*lplpDDrawPalette)->lpVtbl == NULL )
		return FALSE;

	(*lplpDDrawPalette)->lpVtbl->QueryInterface = IDirectDrawPaletteFake_QueryInterface;
	(*lplpDDrawPalette)->lpVtbl->AddRef = IDirectDrawPaletteFake_AddRef;
	(*lplpDDrawPalette)->lpVtbl->Release = IDirectDrawPaletteFake_Release;
	(*lplpDDrawPalette)->lpVtbl->GetCaps = IDirectDrawPaletteFake_GetCaps;
	(*lplpDDrawPalette)->lpVtbl->GetEntries = IDirectDrawPaletteFake_GetEntries;
	(*lplpDDrawPalette)->lpVtbl->SetEntries = IDirectDrawPaletteFake_SetEntries;

	return TRUE;
}

void DDrawPaletteVtableDelete( IDirectDrawPaletteFake** lplpDDrawPalette )
{
	if( !lplpDDrawPalette || (*lplpDDrawPalette) == NULL )
		return;

	free( (*lplpDDrawPalette)->lpVtbl );
	(*lplpDDrawPalette)->lpVtbl = NULL;
}

int D3DVtableCreate( IDirect3DFake** lplpD3D )
{
	if( !lplpD3D || (*lplpD3D) == NULL )
		return FALSE;

	(*lplpD3D)->lpVtbl = malloc( sizeof( struct IDirect3DFakeVtbl ) );
	if( (*lplpD3D)->lpVtbl == NULL )
		return FALSE;

	(*lplpD3D)->lpVtbl->AddRef = IDirect3DFake_AddRef;
	(*lplpD3D)->lpVtbl->Release = IDirect3DFake_Release;
	(*lplpD3D)->lpVtbl->QueryInterface = IDirect3DFake_QueryInterface;

	(*lplpD3D)->lpVtbl->CreateDevice = IDirect3DFake_CreateDevice;
	(*lplpD3D)->lpVtbl->CreateVertexBuffer = IDirect3DFake_CreateVertexBuffer;
	(*lplpD3D)->lpVtbl->EnumDevices = IDirect3DFake_EnumDevices;
	(*lplpD3D)->lpVtbl->EnumZBufferFormats = IDirect3DFake_EnumZBufferFormats;
	(*lplpD3D)->lpVtbl->EvictManagedTextures = IDirect3DFake_EvictManagedTextures;

	return TRUE;
}

void D3DVtableDelete( IDirect3DFake** lplpD3D )
{
	if( !lplpD3D || (*lplpD3D) == NULL )
		return;

	free( (*lplpD3D)->lpVtbl );
	(*lplpD3D)->lpVtbl = NULL;
}

int D3DDeviceVtableCreate( IDirect3DDeviceFake** lplpD3DDevice )
{
	if( !lplpD3DDevice || (*lplpD3DDevice) == NULL )
		return FALSE;

	(*lplpD3DDevice)->lpVtbl = malloc( sizeof( struct IDirect3DDeviceFakeVtbl ) );
	if( (*lplpD3DDevice)->lpVtbl == NULL )
		return FALSE;

	(*lplpD3DDevice)->lpVtbl->AddRef = IDirect3DDeviceFake_AddRef;
	(*lplpD3DDevice)->lpVtbl->Release = IDirect3DDeviceFake_Release;
	(*lplpD3DDevice)->lpVtbl->QueryInterface = IDirect3DDeviceFake_QueryInterface;

	(*lplpD3DDevice)->lpVtbl->ApplyStateBlock = IDirect3DDeviceFake_ApplyStateBlock;
	(*lplpD3DDevice)->lpVtbl->BeginScene = IDirect3DDeviceFake_BeginScene;
	(*lplpD3DDevice)->lpVtbl->BeginStateBlock = IDirect3DDeviceFake_BeginStateBlock;
	(*lplpD3DDevice)->lpVtbl->CaptureStateBlock = IDirect3DDeviceFake_CaptureStateBlock;
	(*lplpD3DDevice)->lpVtbl->CreateStateBlock = IDirect3DDeviceFake_CreateStateBlock;
	(*lplpD3DDevice)->lpVtbl->Clear = IDirect3DDeviceFake_Clear;
	(*lplpD3DDevice)->lpVtbl->ComputeSphereVisibility = IDirect3DDeviceFake_ComputeSphereVisibility;
	(*lplpD3DDevice)->lpVtbl->DeleteStateBlock = IDirect3DDeviceFake_DeleteStateBlock;
	(*lplpD3DDevice)->lpVtbl->DrawIndexedPrimitive = IDirect3DDeviceFake_DrawIndexedPrimitive;
	(*lplpD3DDevice)->lpVtbl->DrawIndexedPrimitiveStrided = IDirect3DDeviceFake_DrawIndexedPrimitiveStrided;
	(*lplpD3DDevice)->lpVtbl->DrawIndexedPrimitiveVB = IDirect3DDeviceFake_DrawIndexedPrimitiveVB;
	(*lplpD3DDevice)->lpVtbl->DrawPrimitive = IDirect3DDeviceFake_DrawPrimitive;
	(*lplpD3DDevice)->lpVtbl->DrawPrimitiveStrided = IDirect3DDeviceFake_DrawPrimitiveStrided;
	(*lplpD3DDevice)->lpVtbl->DrawPrimitiveVB = IDirect3DDeviceFake_DrawPrimitiveVB;
	(*lplpD3DDevice)->lpVtbl->EndScene = IDirect3DDeviceFake_EndScene;
	(*lplpD3DDevice)->lpVtbl->EndStateBlock = IDirect3DDeviceFake_EndStateBlock;
	(*lplpD3DDevice)->lpVtbl->GetInfo = IDirect3DDeviceFake_GetInfo;
	(*lplpD3DDevice)->lpVtbl->GetCaps = IDirect3DDeviceFake_GetCaps;
	(*lplpD3DDevice)->lpVtbl->GetClipPlane = IDirect3DDeviceFake_GetClipPlane;
	(*lplpD3DDevice)->lpVtbl->GetClipStatus = IDirect3DDeviceFake_GetClipStatus;
	(*lplpD3DDevice)->lpVtbl->GetDirect3D = IDirect3DDeviceFake_GetDirect3D;
	(*lplpD3DDevice)->lpVtbl->GetLight = IDirect3DDeviceFake_GetLight;
	(*lplpD3DDevice)->lpVtbl->GetLightEnable = IDirect3DDeviceFake_GetLightEnable;
	(*lplpD3DDevice)->lpVtbl->GetMaterial = IDirect3DDeviceFake_GetMaterial;
	(*lplpD3DDevice)->lpVtbl->GetRenderState = IDirect3DDeviceFake_GetRenderState;
	(*lplpD3DDevice)->lpVtbl->GetRenderTarget = IDirect3DDeviceFake_GetRenderTarget;
	(*lplpD3DDevice)->lpVtbl->GetTexture = IDirect3DDeviceFake_GetTexture;
	(*lplpD3DDevice)->lpVtbl->GetTextureStageState = IDirect3DDeviceFake_GetTextureStageState;
	(*lplpD3DDevice)->lpVtbl->GetTransform = IDirect3DDeviceFake_GetTransform;
	(*lplpD3DDevice)->lpVtbl->GetViewport = IDirect3DDeviceFake_GetViewport;
	(*lplpD3DDevice)->lpVtbl->LightEnable = IDirect3DDeviceFake_LightEnable;
	(*lplpD3DDevice)->lpVtbl->Load = IDirect3DDeviceFake_Load;
	(*lplpD3DDevice)->lpVtbl->MultiplyTransform = IDirect3DDeviceFake_MultiplyTransform;
	(*lplpD3DDevice)->lpVtbl->PreLoad = IDirect3DDeviceFake_PreLoad;
	(*lplpD3DDevice)->lpVtbl->SetClipPlane = IDirect3DDeviceFake_SetClipPlane;
	(*lplpD3DDevice)->lpVtbl->SetClipStatus = IDirect3DDeviceFake_SetClipStatus;
	(*lplpD3DDevice)->lpVtbl->SetLight = IDirect3DDeviceFake_SetLight;
	(*lplpD3DDevice)->lpVtbl->SetMaterial = IDirect3DDeviceFake_SetMaterial;
	(*lplpD3DDevice)->lpVtbl->SetRenderState = IDirect3DDeviceFake_SetRenderState;
	(*lplpD3DDevice)->lpVtbl->SetTexture = IDirect3DDeviceFake_SetTexture;
	(*lplpD3DDevice)->lpVtbl->SetTextureStageState = IDirect3DDeviceFake_SetTextureStageState;
	(*lplpD3DDevice)->lpVtbl->SetTransform = IDirect3DDeviceFake_SetTransform;
	(*lplpD3DDevice)->lpVtbl->SetViewport = IDirect3DDeviceFake_SetViewport;
	(*lplpD3DDevice)->lpVtbl->ValidateDevice = IDirect3DDeviceFake_ValidateDevice;

	return TRUE;
}

void D3DDeviceVtableDelete( IDirect3DDeviceFake** lplpD3DDevice )
{
	if( !lplpD3DDevice || (*lplpD3DDevice) == NULL )
		return;

	free( (*lplpD3DDevice)->lpVtbl );
	(*lplpD3DDevice)->lpVtbl = NULL;
}

int D3DVertexBufferVtableCreate( IDirect3DVertexBufferFake** lplpD3DVB )
{
	if( !lplpD3DVB || (*lplpD3DVB) == NULL )
		return FALSE;

	(*lplpD3DVB)->lpVtbl = malloc( sizeof( struct IDirect3DVertexBufferFakeVtbl ) );
	if( (*lplpD3DVB)->lpVtbl == NULL )
		return FALSE;

	(*lplpD3DVB)->lpVtbl->AddRef = IDirect3DVertexBufferFake_AddRef;
	(*lplpD3DVB)->lpVtbl->Release = IDirect3DVertexBufferFake_Release;
	(*lplpD3DVB)->lpVtbl->QueryInterface = IDirect3DVertexBufferFake_QueryInterface;

	return TRUE;
}

void D3DVertexBufferVtableDelete( IDirect3DVertexBufferFake** lplpD3DVB )
{
	if( !lplpD3DVB || (*lplpD3DVB) == NULL )
		return;

	free( (*lplpD3DVB)->lpVtbl );
	(*lplpD3DVB)->lpVtbl = NULL;
}