#pragma once

#ifdef __cplusplus
extern "C" {
#endif

ULONG WINAPI IDirectDrawFake_AddRef( IDirectDrawFake* This );
ULONG WINAPI IDirectDrawFake_Release( IDirectDrawFake* This );
HRESULT WINAPI IDirectDrawFake_QueryInterface( IDirectDrawFake* This, REFIID riid, LPVOID FAR * ppvObj );
HRESULT WINAPI IDirectDrawFake_Compact( IDirectDrawFake* This );
HRESULT WINAPI IDirectDrawFake_CreateClipper( IDirectDrawFake* This, DWORD dwFlags, IDirectDrawClipperFake** lplpDDClipper, IUnknownFake* pUnkOuter );
HRESULT WINAPI IDirectDrawFake_CreatePalette( IDirectDrawFake* This, DWORD dwFlags, LPPALETTEENTRY lpDDColorArray, IDirectDrawPaletteFake** lplpDDPalette, IUnknownFake* pUnkOuter );
HRESULT WINAPI IDirectDrawFake_CreateSurface( IDirectDrawFake* This, LPDDSURFACEDESC2 lpDDSurfaceDesc2, IDirectDrawSurfaceFake** lplpDDSurface, IUnknownFake* pUnkOuter );
HRESULT WINAPI IDirectDrawFake_DuplicateSurface( IDirectDrawFake* This, IDirectDrawSurfaceFake* lpDDSurface, IDirectDrawSurfaceFake** lplpDupDDSurface ) ;
HRESULT WINAPI IDirectDrawFake_EnumDisplayModes( IDirectDrawFake* This, DWORD dwFlags, LPDDSURFACEDESC2 lpDDSurfaceDesc2, LPVOID lpContext, LPDDENUMMODESCALLBACK2 lpEnumModesCallback );
HRESULT WINAPI IDirectDrawFake_EnumSurfaces( IDirectDrawFake* This, DWORD dwFlags, LPDDSURFACEDESC2 lpDDSD2, LPVOID lpContext, LPDDENUMSURFACESCALLBACK7 lpEnumSurfacesCallback );
HRESULT WINAPI IDirectDrawFake_EvaluateMode( IDirectDrawFake* This, DWORD dwFlags, DWORD *pSecondsUntilTimeout );
HRESULT WINAPI IDirectDrawFake_FlipToGDISurface( IDirectDrawFake* This );
HRESULT WINAPI IDirectDrawFake_GetAvailableVidMem( IDirectDrawFake* This, LPDDSCAPS2 lpDDSCaps2, LPDWORD lpdwTotal, LPDWORD lpdwFree );
HRESULT WINAPI IDirectDrawFake_GetCaps( IDirectDrawFake* This, LPDDCAPS lpDDDriverCaps, LPDDCAPS lpDDHELCaps   );
HRESULT WINAPI IDirectDrawFake_GetDeviceIdentifier( IDirectDrawFake* This, LPDDDEVICEIDENTIFIER2 lpdddi, DWORD dwFlags );
HRESULT WINAPI IDirectDrawFake_GetDisplayMode( IDirectDrawFake* This, LPDDSURFACEDESC2 lpDDSurfaceDesc2  );
HRESULT WINAPI IDirectDrawFake_GetFourCCCodes( IDirectDrawFake* This, LPDWORD lpNumCodes, LPDWORD lpCodes );
HRESULT WINAPI IDirectDrawFake_GetGDISurface( IDirectDrawFake* This, IDirectDrawSurfaceFake** lplpGDIDDSurface );
HRESULT WINAPI IDirectDrawFake_GetMonitorFrequency( IDirectDrawFake* This, LPDWORD lpdwFrequency );
HRESULT WINAPI IDirectDrawFake_GetScanLine( IDirectDrawFake* This, LPDWORD lpdwScanLine );
HRESULT WINAPI IDirectDrawFake_GetSurfaceFromDC( IDirectDrawFake* This, HDC hdc, IDirectDrawSurfaceFake** lpDDS );
HRESULT WINAPI IDirectDrawFake_GetVerticalBlankStatus( IDirectDrawFake* This, LPBOOL lpbIsInVB );
HRESULT WINAPI IDirectDrawFake_Initialize( IDirectDrawFake* This, GUID FAR *lpGUID );
HRESULT WINAPI IDirectDrawFake_RestoreAllSurfaces( IDirectDrawFake* This );
HRESULT WINAPI IDirectDrawFake_RestoreDisplayMode( IDirectDrawFake* This );
HRESULT WINAPI IDirectDrawFake_SetCooperativeLevel( IDirectDrawFake* This, HWND hWnd, DWORD dwFlags );
HRESULT WINAPI IDirectDrawFake_SetDisplayMode( IDirectDrawFake* This, DWORD dwWidth, DWORD dwHeight, DWORD dwBPP, DWORD dwRefreshRate, DWORD dwFlags );
HRESULT WINAPI IDirectDrawFake_StartModeTest( IDirectDrawFake* This, LPSIZE lpModesToTest, DWORD dwNumEntries, DWORD dwFlags );
HRESULT WINAPI IDirectDrawFake_TestCooperativeLevel( IDirectDrawFake* This );
HRESULT WINAPI IDirectDrawFake_WaitForVerticalBlank( IDirectDrawFake* This, DWORD dwFlags, HANDLE hEvent );


/*
 * IDirectDrawSurfaceFake
 */
ULONG WINAPI IDirectDrawSurfaceFake_AddRef( IDirectDrawSurfaceFake* This );
ULONG WINAPI IDirectDrawSurfaceFake_Release( IDirectDrawSurfaceFake* This );
HRESULT WINAPI IDirectDrawSurfaceFake_QueryInterface( IDirectDrawSurfaceFake* This, REFIID riid, LPVOID FAR * ppvObj );
HRESULT WINAPI IDirectDrawSurfaceFake_AddAttachedSurface( IDirectDrawSurfaceFake* This, IDirectDrawSurfaceFake* lpDDSAttachedSurface );
HRESULT WINAPI IDirectDrawSurfaceFake_AddOverlayDirtyRect( IDirectDrawSurfaceFake* This, LPRECT lpRect );
HRESULT WINAPI IDirectDrawSurfaceFake_Blt( IDirectDrawSurfaceFake* This, LPRECT lpDestRect, IDirectDrawSurfaceFake* lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags, LPDDBLTFX lpDDBltFx );
HRESULT WINAPI IDirectDrawSurfaceFake_BltBatch( IDirectDrawSurfaceFake* This, LPDDBLTBATCH lpDDBltBatch, DWORD dwCount, DWORD dwFlags );
HRESULT WINAPI IDirectDrawSurfaceFake_BltFast( IDirectDrawSurfaceFake* This, DWORD dwX,DWORD dwY, IDirectDrawSurfaceFake* lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwTrans );
HRESULT WINAPI IDirectDrawSurfaceFake_ChangeUniquenessValue( IDirectDrawSurfaceFake* This );
HRESULT WINAPI IDirectDrawSurfaceFake_DeleteAttachedSurface( IDirectDrawSurfaceFake* This, DWORD dwFlags, IDirectDrawSurfaceFake* lpDDSAttachedSurface );
HRESULT WINAPI IDirectDrawSurfaceFake_EnumAttachedSurfaces( IDirectDrawSurfaceFake* This, LPVOID lpContext, LPDDENUMSURFACESCALLBACK7 lpEnumSurfacesCallback );
HRESULT WINAPI IDirectDrawSurfaceFake_EnumOverlayZOrders( IDirectDrawSurfaceFake* This, DWORD dwFlags, LPVOID lpContext, LPDDENUMSURFACESCALLBACK7 lpfnCallback );
HRESULT WINAPI IDirectDrawSurfaceFake_Flip( IDirectDrawSurfaceFake* This, IDirectDrawSurfaceFake* lpDDSurfaceTargetOverride, DWORD dwFlags );
HRESULT WINAPI IDirectDrawSurfaceFake_FreePrivateData( IDirectDrawSurfaceFake* This, REFGUID guidTag ); 
HRESULT WINAPI IDirectDrawSurfaceFake_GetAttachedSurface( IDirectDrawSurfaceFake* This, LPDDSCAPS2 lpDDSCaps, IDirectDrawSurfaceFake* FAR *lplpDDAttachedSurface );
HRESULT WINAPI IDirectDrawSurfaceFake_GetBltStatus( IDirectDrawSurfaceFake* This, DWORD dwFlags );
HRESULT WINAPI IDirectDrawSurfaceFake_GetCaps( IDirectDrawSurfaceFake* This, LPDDSCAPS2 lpDDSCaps );
HRESULT WINAPI IDirectDrawSurfaceFake_GetClipper( IDirectDrawSurfaceFake* This, IDirectDrawClipperFake* FAR *lplpDDClipper );
HRESULT WINAPI IDirectDrawSurfaceFake_GetColorKey( IDirectDrawSurfaceFake* This, DWORD dwFlags, LPDDCOLORKEY lpDDColorKey );
HRESULT WINAPI IDirectDrawSurfaceFake_GetDC( IDirectDrawSurfaceFake* This, HDC FAR *lphDC  );
HRESULT WINAPI IDirectDrawSurfaceFake_GetDDInterface( IDirectDrawSurfaceFake* This, LPVOID FAR *lplpDD );
HRESULT WINAPI IDirectDrawSurfaceFake_GetFlipStatus( IDirectDrawSurfaceFake* This, DWORD dwFlags );
HRESULT WINAPI IDirectDrawSurfaceFake_GetLOD( IDirectDrawSurfaceFake* This, LPDWORD lpdwLOD );
HRESULT WINAPI IDirectDrawSurfaceFake_GetOverlayPosition( IDirectDrawSurfaceFake* This, LPLONG lplX, LPLONG lplY );
HRESULT WINAPI IDirectDrawSurfaceFake_GetPalette( IDirectDrawSurfaceFake* This, IDirectDrawPaletteFake* FAR *lplpDDPalette );
HRESULT WINAPI IDirectDrawSurfaceFake_GetPixelFormat( IDirectDrawSurfaceFake* This, LPDDPIXELFORMAT lpDDPixelFormat  );
HRESULT WINAPI IDirectDrawSurfaceFake_GetPriority( IDirectDrawSurfaceFake* This, LPDWORD lpdwPriority); 
HRESULT WINAPI IDirectDrawSurfaceFake_GetPrivateData( IDirectDrawSurfaceFake* This, REFGUID guidTag,  LPVOID  lpBuffer, LPDWORD lpcbBufferSize );
HRESULT WINAPI IDirectDrawSurfaceFake_GetSurfaceDesc( IDirectDrawSurfaceFake* This, LPDDSURFACEDESC2 lpDDSurfaceDesc  );
HRESULT WINAPI IDirectDrawSurfaceFake_GetUniquenessValue( IDirectDrawSurfaceFake* This, LPDWORD lpValue ); 
HRESULT WINAPI IDirectDrawSurfaceFake_Initialize( IDirectDrawSurfaceFake* This, IDirectDrawFake* lpDD, LPDDSURFACEDESC2 lpDDSurfaceDesc );
HRESULT WINAPI IDirectDrawSurfaceFake_IsLost( IDirectDrawSurfaceFake* This );
HRESULT WINAPI IDirectDrawSurfaceFake_Lock( IDirectDrawSurfaceFake* This, LPRECT lpDestRect, LPDDSURFACEDESC2 lpDDSurfaceDesc, DWORD dwFlags, HANDLE hEvent );
HRESULT WINAPI IDirectDrawSurfaceFake_PageLock( IDirectDrawSurfaceFake* This, DWORD dwFlags );
HRESULT WINAPI IDirectDrawSurfaceFake_PageUnlock( IDirectDrawSurfaceFake* This, DWORD dwFlags );
HRESULT WINAPI IDirectDrawSurfaceFake_ReleaseDC( IDirectDrawSurfaceFake* This, HDC hDC );
HRESULT WINAPI IDirectDrawSurfaceFake_Restore( IDirectDrawSurfaceFake* This );
HRESULT WINAPI IDirectDrawSurfaceFake_SetClipper( IDirectDrawSurfaceFake* This, IDirectDrawClipperFake* lpDDClipper );
HRESULT WINAPI IDirectDrawSurfaceFake_SetColorKey( IDirectDrawSurfaceFake* This, DWORD dwFlags, LPDDCOLORKEY lpDDColorKey  );
HRESULT WINAPI IDirectDrawSurfaceFake_SetLOD( IDirectDrawSurfaceFake* This, DWORD dwMaxLOD );
HRESULT WINAPI IDirectDrawSurfaceFake_SetOverlayPosition( IDirectDrawSurfaceFake* This, LONG lX, LONG lY );
HRESULT WINAPI IDirectDrawSurfaceFake_SetPalette( IDirectDrawSurfaceFake* This, IDirectDrawPaletteFake* lpDDPalette );
HRESULT WINAPI IDirectDrawSurfaceFake_SetPriority( IDirectDrawSurfaceFake* This, DWORD dwPriority );
HRESULT WINAPI IDirectDrawSurfaceFake_SetPrivateData( IDirectDrawSurfaceFake* This, REFGUID guidTag, LPVOID  lpData, DWORD   cbSize, DWORD   dwFlags ); 
HRESULT WINAPI IDirectDrawSurfaceFake_SetSurfaceDesc( IDirectDrawSurfaceFake* This, LPDDSURFACEDESC2 lpddsd2, DWORD dwFlags );
HRESULT WINAPI IDirectDrawSurfaceFake_Unlock( IDirectDrawSurfaceFake* This, LPRECT lpRect );
HRESULT WINAPI IDirectDrawSurfaceFake_UpdateOverlay( IDirectDrawSurfaceFake* This, LPRECT lpSrcRect, IDirectDrawSurfaceFake* lpDDDestSurface, LPRECT lpDestRect, DWORD dwFlags, LPDDOVERLAYFX lpDDOverlayFx );
HRESULT WINAPI IDirectDrawSurfaceFake_UpdateOverlayDisplay( IDirectDrawSurfaceFake* This, DWORD dwFlags );
HRESULT WINAPI IDirectDrawSurfaceFake_UpdateOverlayZOrder( IDirectDrawSurfaceFake* This, DWORD dwFlags, IDirectDrawSurfaceFake* lpDDSReference );


/*
 * IDirectDrawClipperFake 
 */
ULONG WINAPI IDirectDrawClipperFake_AddRef( IDirectDrawClipperFake* This );
ULONG WINAPI IDirectDrawClipperFake_Release( IDirectDrawClipperFake* This );
HRESULT WINAPI IDirectDrawClipperFake_QueryInterface( IDirectDrawClipperFake* This, REFIID riid, LPVOID FAR * ppvObj );
HRESULT WINAPI IDirectDrawClipperFake_GetClipList( IDirectDrawClipperFake* This, LPRECT lpRect, LPRGNDATA lpClipList, LPDWORD lpdwSize );
HRESULT WINAPI IDirectDrawClipperFake_GetHWnd( IDirectDrawClipperFake* This, HWND FAR *lphWnd );
HRESULT WINAPI IDirectDrawClipperFake_Initialize( IDirectDrawClipperFake* This, IDirectDrawFake* lpDD, DWORD dwFlags );
HRESULT WINAPI IDirectDrawClipperFake_IsClipListChanged( IDirectDrawClipperFake* This, BOOL FAR *lpbChanged );
HRESULT WINAPI IDirectDrawClipperFake_SetClipList( IDirectDrawClipperFake* This, LPRGNDATA lpClipList, DWORD dwFlags );
HRESULT WINAPI IDirectDrawClipperFake_SetHWnd( IDirectDrawClipperFake* This, DWORD dwFlags, HWND hWnd );


/*
 * IDirectDrawPaletteFake
 */
ULONG WINAPI IDirectDrawPaletteFake_AddRef( IDirectDrawPaletteFake* This );
ULONG WINAPI IDirectDrawPaletteFake_Release( IDirectDrawPaletteFake* This );
HRESULT WINAPI IDirectDrawPaletteFake_QueryInterface( IDirectDrawPaletteFake* This, REFIID riid, LPVOID FAR * ppvObj );
HRESULT WINAPI IDirectDrawPaletteFake_GetCaps( IDirectDrawPaletteFake* This, LPDWORD lpdwCaps );
HRESULT WINAPI IDirectDrawPaletteFake_GetEntries( IDirectDrawPaletteFake* This, DWORD dwFlags, DWORD dwBase, DWORD dwNumEntries, LPPALETTEENTRY lpEntries );
HRESULT WINAPI IDirectDrawPaletteFake_Initialize(  IDirectDrawPaletteFake* This, IDirectDrawFake* lpDD, DWORD dwFlags, LPPALETTEENTRY lpDDColorTable );
HRESULT WINAPI IDirectDrawPaletteFake_SetEntries( IDirectDrawPaletteFake* This, DWORD dwFlags, DWORD dwStartingEntry, DWORD dwCount, LPPALETTEENTRY lpEntries );



int DDrawVtableCreate( IDirectDrawFake** lplpDDraw );
void DDrawVtableDelete( IDirectDrawFake** lplpDDraw );

int DDrawSurfaceVtableCreate( IDirectDrawSurfaceFake** lplpDDrawSurface );
void DDrawSurfaceVtableDelete( IDirectDrawSurfaceFake** lplpDDrawSurface );

#ifdef __cplusplus
}
#endif