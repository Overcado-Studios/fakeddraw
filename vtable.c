#include "framework.h"
#include <ddraw.h>
#include "fakeddraw.h"

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
struct IDirectDrawClipperFakeVtbl
{
	HRESULT (WINAPI* QueryInterface)( IDirectDrawClipperFake*, REFIID, LPVOID );
	ULONG (WINAPI* AddRef)( IDirectDrawClipperFake* );
	ULONG (WINAPI* Release)( IDirectDrawClipperFake* );

	HRESULT (WINAPI* GetClipList)( IDirectDrawClipperFake*, LPRECT, LPRGNDATA, LPDWORD );
	HRESULT (WINAPI* GetHWnd)( IDirectDrawClipperFake* , HWND* );
	HRESULT (WINAPI* Initialize)( IDirectDrawClipperFake* , IDirectDrawFake*, DWORD );
	HRESULT (WINAPI* IsClipListChanged)( IDirectDrawClipperFake* , BOOL* );
	HRESULT (WINAPI* SetClipList)( IDirectDrawClipperFake*, LPRGNDATA, DWORD );
	HRESULT (WINAPI* SetHWnd)( IDirectDrawClipperFake*, DWORD, HWND );
};


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
