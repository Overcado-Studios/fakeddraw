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
HRESULT WINAPI IDirectDrawFake_EnumSurfaces( IDirectDrawFake* This, DWORD dwFlags, LPDDSURFACEDESC2 lpDDSD2, LPVOID lpContext, LPDDENUMSURFACESCALLBACKFAKE lpEnumSurfacesCallback );
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
HRESULT WINAPI IDirectDrawSurfaceFake_EnumAttachedSurfaces( IDirectDrawSurfaceFake* This, LPVOID lpContext, LPDDENUMSURFACESCALLBACKFAKE lpEnumSurfacesCallback );
HRESULT WINAPI IDirectDrawSurfaceFake_EnumOverlayZOrders( IDirectDrawSurfaceFake* This, DWORD dwFlags, LPVOID lpContext, LPDDENUMSURFACESCALLBACKFAKE lpfnCallback );
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


/*
 * IDirect3DFake 
 */
HRESULT WINAPI IDirect3DFake_QueryInterface( IDirect3DFake* This, REFIID riid, LPVOID FAR* ppvObj );
ULONG WINAPI IDirect3DFake_AddRef( IDirect3DFake* This );
ULONG WINAPI IDirect3DFake_Release( IDirect3DFake* This );
HRESULT WINAPI IDirect3DFake_CreateDevice( IDirect3DFake* This, REFCLSID rclsid, IDirectDrawSurfaceFake* lpDDS, IDirect3DDeviceFake** lplpD3DDevice );
HRESULT WINAPI IDirect3DFake_CreateVertexBuffer( IDirect3DFake* This, LPD3DVERTEXBUFFERDESC lpVBDesc, IDirect3DVertexBufferFake** lplpD3DVertexBuffer, DWORD dwFlags );
HRESULT WINAPI IDirect3DFake_EnumDevices( IDirect3DFake* This, LPD3DENUMDEVICESCALLBACK7 lpEnumDevicesCallback, LPVOID lpUserArg );
HRESULT WINAPI IDirect3DFake_EnumZBufferFormats( IDirect3DFake* This, REFCLSID riidDevice, LPD3DENUMPIXELFORMATSCALLBACK lpEnumCallback, LPVOID lpContext ); 
HRESULT WINAPI IDirect3DFake_EvictManagedTextures( IDirect3DFake* This ); 


/*
 * IDirect3DDeviceFake functions
 */

HRESULT WINAPI IDirect3DDeviceFake_QueryInterface( IDirect3DDeviceFake* This, REFIID riid, LPVOID FAR* ppvObj );
ULONG WINAPI IDirect3DDeviceFake_AddRef( IDirect3DDeviceFake* This );
ULONG WINAPI IDirect3DDeviceFake_Release( IDirect3DDeviceFake* This );
HRESULT WINAPI IDirect3DDeviceFake_ApplyStateBlock( IDirect3DDeviceFake* This,   DWORD dwBlockHandle ); 
HRESULT WINAPI IDirect3DDeviceFake_BeginScene( IDirect3DDeviceFake* This );
HRESULT WINAPI IDirect3DDeviceFake_BeginStateBlock( IDirect3DDeviceFake* This );
HRESULT WINAPI IDirect3DDeviceFake_CaptureStateBlock( IDirect3DDeviceFake* This,  DWORD dwBlockHandle );
HRESULT WINAPI IDirect3DDeviceFake_CreateStateBlock( IDirect3DDeviceFake* This, D3DSTATEBLOCKTYPE d3dsbType, LPDWORD lpdwBlockHandle );
HRESULT WINAPI IDirect3DDeviceFake_Clear( IDirect3DDeviceFake* This,  DWORD     dwCount,     LPD3DRECT lpRects,   
  DWORD     dwFlags,     DWORD     dwColor,     D3DVALUE  dvZ,       
  DWORD     dwStencil  );
HRESULT WINAPI IDirect3DDeviceFake_ComputeSphereVisibility( IDirect3DDeviceFake* This,   LPD3DVECTOR lpCenters,         
  LPD3DVALUE  lpRadii,             DWORD       dwNumSpheres,      
  DWORD       dwFlags,             LPDWORD     lpdwReturnValues   ); 
HRESULT WINAPI IDirect3DDeviceFake_DeleteStateBlock ( IDirect3DDeviceFake* This,  DWORD dwBlockHandle  ); 
HRESULT WINAPI IDirect3DDeviceFake_DrawIndexedPrimitive( IDirect3DDeviceFake* This,  D3DPRIMITIVETYPE d3dptPrimitiveType,  
  DWORD  dwVertexTypeDesc,                LPVOID lpvVertices,                   
  DWORD  dwVertexCount,                   LPWORD lpwIndices,                    
  DWORD  dwIndexCount,                    DWORD  dwFlags                        
);
HRESULT WINAPI IDirect3DDeviceFake_DrawIndexedPrimitiveStrided( IDirect3DDeviceFake* This,  D3DPRIMITIVETYPE d3dptPrimitiveType,  
  DWORD  dwVertexTypeDesc,              
  LPD3DDRAWPRIMITIVESTRIDEDDATA lpVertexArray,  
  DWORD  dwVertexCount,                   LPWORD lpwIndices,                    
  DWORD  dwIndexCount,                    DWORD  dwFlags                        
);
HRESULT WINAPI IDirect3DDeviceFake_DrawIndexedPrimitiveVB( IDirect3DDeviceFake* This,  D3DPRIMITIVETYPE        d3dptPrimitiveType,
  IDirect3DVertexBufferFake* lpd3dVertexBuffer,  DWORD  dwStartVertex,
  DWORD  dwNumVertices,  LPWORD lpwIndices,  DWORD  dwIndexCount,
  DWORD  dwFlags);
HRESULT WINAPI IDirect3DDeviceFake_DrawPrimitive( IDirect3DDeviceFake* This,  D3DPRIMITIVETYPE dptPrimitiveType,  
  DWORD  dwVertexTypeDesc,              LPVOID lpvVertices,                 
  DWORD  dwVertexCount,                 DWORD  dwFlags                      );
HRESULT WINAPI IDirect3DDeviceFake_DrawPrimitiveStrided( IDirect3DDeviceFake* This,  D3DPRIMITIVETYPE dptPrimitiveType,  
  DWORD  dwVertexTypeDesc,            
  LPD3DDRAWPRIMITIVESTRIDEDDATA lpVertexArray,  
  DWORD  dwVertexCount,                 DWORD  dwFlags                      );
HRESULT WINAPI IDirect3DDeviceFake_DrawPrimitiveVB( IDirect3DDeviceFake* This,   D3DPRIMITIVETYPE        d3dptPrimitiveType,
  IDirect3DVertexBufferFake* lpd3dVertexBuffer,  DWORD dwStartVertex,
  DWORD dwNumVertices,  DWORD dwFlags); 
HRESULT WINAPI IDirect3DDeviceFake_EndScene( IDirect3DDeviceFake* This );
HRESULT WINAPI IDirect3DDeviceFake_EndStateBlock( IDirect3DDeviceFake* This,   LPDWORD lpdwBlockHandle ); 
HRESULT WINAPI IDirect3DDeviceFake_EnumTextureFormats( IDirect3DDeviceFake* This,  LPD3DENUMPIXELFORMATSCALLBACK lpd3dEnumPixelProc, LPVOID lpArg );
HRESULT WINAPI IDirect3DDeviceFake_GetCaps( IDirect3DDeviceFake* This,  LPD3DDEVICEDESC7 lpD3DDevDesc );
HRESULT WINAPI IDirect3DDeviceFake_GetClipPlane( IDirect3DDeviceFake* This,   DWORD dwIndex,   D3DVALUE* pPlaneEquation ); 
HRESULT WINAPI IDirect3DDeviceFake_GetClipStatus( IDirect3DDeviceFake* This,  LPD3DCLIPSTATUS lpD3DClipStatus );
HRESULT WINAPI IDirect3DDeviceFake_GetDirect3D( IDirect3DDeviceFake* This,  IDirect3DFake* *lplpD3D );
HRESULT WINAPI IDirect3DDeviceFake_GetInfo( IDirect3DDeviceFake* This,   DWORD  dwDevInfoID,   LPVOID pDevInfoStruct,   DWORD  dwSize ); 
HRESULT WINAPI IDirect3DDeviceFake_GetLight( IDirect3DDeviceFake* This,   DWORD dwLightIndex,   LPD3DLIGHT7 lpLight ); 
HRESULT WINAPI IDirect3DDeviceFake_GetLightEnable( IDirect3DDeviceFake* This,   DWORD dwLightIndex,  BOOL* pbEnable ); 
HRESULT WINAPI IDirect3DDeviceFake_GetMaterial( IDirect3DDeviceFake* This,   LPD3DMATERIAL7 lpMaterial ); 
HRESULT WINAPI IDirect3DDeviceFake_GetRenderState( IDirect3DDeviceFake* This,  D3DRENDERSTATETYPE dwRenderStateType,  LPDWORD lpdwRenderState );
HRESULT WINAPI IDirect3DDeviceFake_GetRenderTarget( IDirect3DDeviceFake* This,  IDirectDrawSurfaceFake** lplpRenderTarget  );
HRESULT WINAPI IDirect3DDeviceFake_GetStateData( IDirect3DDeviceFake* This,   DWORD dwState,   LPVOID* lplpStateData );
HRESULT WINAPI IDirect3DDeviceFake_GetTexture( IDirect3DDeviceFake* This,   DWORD dwStage,  IDirectDrawSurfaceFake** lplpTexture ); 
HRESULT WINAPI IDirect3DDeviceFake_GetTextureStageState( IDirect3DDeviceFake* This,  DWORD   dwStage, D3DTEXTURESTAGESTATETYPE dwState,  LPDWORD lpdwValue ); 
HRESULT WINAPI IDirect3DDeviceFake_GetTransform( IDirect3DDeviceFake* This,  D3DTRANSFORMSTATETYPE dtstTransformStateType,  LPD3DMATRIX lpD3DMatrix );
HRESULT WINAPI IDirect3DDeviceFake_GetViewport( IDirect3DDeviceFake* This,  LPD3DVIEWPORT7 lpViewport ); 
HRESULT WINAPI IDirect3DDeviceFake_LightEnable( IDirect3DDeviceFake* This,   DWORD dwLightIndex,  BOOL bEnable ); 
HRESULT WINAPI IDirect3DDeviceFake_Load( IDirect3DDeviceFake* This,   IDirectDrawSurfaceFake* lpDestTex,   LPPOINT lpDestPoint, IDirectDrawSurfaceFake* lpSrcTex, LPRECT lprcSrcRect, DWORD dwFlags );
HRESULT WINAPI IDirect3DDeviceFake_MultiplyTransform( IDirect3DDeviceFake* This,  D3DTRANSFORMSTATETYPE dtstTransformStateType, LPD3DMATRIX lpD3DMatrix );
HRESULT WINAPI IDirect3DDeviceFake_PreLoad( IDirect3DDeviceFake* This,  IDirectDrawSurfaceFake* lpddsTexture );
HRESULT WINAPI IDirect3DDeviceFake_SetClipPlane( IDirect3DDeviceFake* This,   DWORD dwIndex,   D3DVALUE* pPlaneEquation ); 
HRESULT WINAPI IDirect3DDeviceFake_SetClipStatus( IDirect3DDeviceFake* This,  LPD3DCLIPSTATUS lpD3DClipStatus  );
HRESULT WINAPI IDirect3DDeviceFake_SetLight( IDirect3DDeviceFake* This,   DWORD dwLightIndex,   LPD3DLIGHT7 lpLight); 
HRESULT WINAPI IDirect3DDeviceFake_SetMaterial( IDirect3DDeviceFake* This,   LPD3DMATERIAL7 lpMaterial ); 
HRESULT WINAPI IDirect3DDeviceFake_SetRenderState( IDirect3DDeviceFake* This,  D3DRENDERSTATETYPE dwRenderStateType, DWORD dwRenderState );
HRESULT WINAPI IDirect3DDeviceFake_SetRenderTarget( IDirect3DDeviceFake* This,  IDirectDrawSurfaceFake* lpNewRenderTarget, DWORD dwFlags );
HRESULT WINAPI IDirect3DDeviceFake_SetStateData( IDirect3DDeviceFake* This,   DWORD dwState,   LPVOID lpStateData );
HRESULT WINAPI IDirect3DDeviceFake_SetTexture( IDirect3DDeviceFake* This, DWORD dwStage, IDirectDrawSurfaceFake* lpTexture ); 
HRESULT WINAPI IDirect3DDeviceFake_SetTextureStageState( IDirect3DDeviceFake* This, DWORD dwStage, D3DTEXTURESTAGESTATETYPE dwState, DWORD dwValue ); 
HRESULT WINAPI IDirect3DDeviceFake_SetTransform( IDirect3DDeviceFake* This,  D3DTRANSFORMSTATETYPE dtstTransformStateType, LPD3DMATRIX lpD3DMatrix );
HRESULT WINAPI IDirect3DDeviceFake_SetViewport( IDirect3DDeviceFake* This,  LPD3DVIEWPORT7 lpViewport ); 
HRESULT WINAPI IDirect3DDeviceFake_ValidateDevice( IDirect3DDeviceFake* This,  LPDWORD lpdwPasses );

/*
 * IDirect3DVertexBufferFake functions
 */
HRESULT WINAPI IDirect3DVertexBufferFake_QueryInterface( IDirect3DVertexBufferFake* This, REFIID riid, LPVOID FAR* ppvObj );
ULONG WINAPI IDirect3DVertexBufferFake_AddRef( IDirect3DVertexBufferFake* This );
ULONG WINAPI IDirect3DVertexBufferFake_Release( IDirect3DVertexBufferFake* This );
HRESULT WINAPI IDirect3DVertexBufferFake_GetVertexBufferDesc( IDirect3DVertexBufferFake* This,  LPD3DVERTEXBUFFERDESC lpVBDesc );
HRESULT WINAPI IDirect3DVertexBufferFake_Lock( IDirect3DVertexBufferFake* This, DWORD   dwFlags,    LPVOID* lplpData,   LPDWORD lpdwSize  ) ;
HRESULT WINAPI IDirect3DVertexBufferFake_Optimize( IDirect3DVertexBufferFake* This,  IDirect3DDeviceFake* lpD3DDevice,  DWORD dwFlags );
HRESULT WINAPI IDirect3DVertexBufferFake_ProcessVertices( IDirect3DVertexBufferFake* This,  DWORD dwVertexOp,   DWORD dwDestIndex, 
  DWORD dwCount,   IDirect3DVertexBufferFake* lpSrcBuffer,   DWORD dwSrcIndex, 
  IDirect3DDeviceFake*       lpD3DDevice,   DWORD dwFlags);
HRESULT WINAPI IDirect3DVertexBufferFake_ProcessVerticesStrided( IDirect3DVertexBufferFake* This,  DWORD dwVertexOp,   DWORD dwDestIndex, 
  DWORD dwCount,   LPD3DDRAWPRIMITIVESTRIDEDDATA lpVertexArray, 
  DWORD dwSrcIndex,  IDirect3DDeviceFake* lpD3DDevice,   DWORD dwFlags);
HRESULT WINAPI IDirect3DVertexBufferFake_Unlock( IDirect3DVertexBufferFake* This );


int DDrawVtableCreate( IDirectDrawFake** lplpDDraw );
void DDrawVtableDelete( IDirectDrawFake** lplpDDraw );

int DDrawSurfaceVtableCreate( IDirectDrawSurfaceFake** lplpDDrawSurface );
void DDrawSurfaceVtableDelete( IDirectDrawSurfaceFake** lplpDDrawSurface );

int DDrawClipperVtableCreate( IDirectDrawClipperFake** lplpDDrawClipper );
void DDrawClipperVtableDelete( IDirectDrawClipperFake** lplpDDrawClipper );

int DDrawPaletteVtableCreate( IDirectDrawPaletteFake** lplpDDrawPalette );
void DDrawPaletteVtableDelete( IDirectDrawPaletteFake** lplpDDrawPalette );

int D3DVtableCreate( IDirect3DFake** lplpD3D );
void D3DVtableDelete( IDirect3DFake** lplpD3D );

int D3DDeviceVtableCreate( IDirect3DDeviceFake** lplpD3DDevice );
void D3DDeviceVtableDelete( IDirect3DDeviceFake** lplpD3DDevice );

int D3DVertexBufferVtableCreate( IDirect3DVertexBufferFake** lplpD3DVB );
void D3DVertexBufferVtableDelete( IDirect3DVertexBufferFake** lplpD3DVB );

#ifdef __cplusplus
}
#endif