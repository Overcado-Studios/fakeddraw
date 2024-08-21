#pragma once

typedef struct D3D11 D3D11;
typedef struct D3D11Surface D3D11Surface;
typedef struct D3D11VertexBuffer D3D11VertexBuffer;


HRESULT D3D11Func_Initialize( D3D11** ppd3d );
void D3D11Func_Uninitialize( D3D11** ppd3d );
HRESULT D3D11Func_CreateDevice( D3D11** ppd3d, HWND hwnd );
void D3D11Func_ReleaseDevice( D3D11** ppd3d );

HRESULT D3D11Func_SetDisplayMode( D3D11* d3d, int width, int height, int bpp, int refresh_rate, int fullscreen );
HRESULT D3D11Func_WaitForVerticalBlank( D3D11* d3d, DWORD dwFlags );
HRESULT D3D11Func_GetAvailableVidMem( D3D11* d3d, LPDDSCAPS2 lpDDSCaps2, DWORD* dwTotal, DWORD* dwFree );
HRESULT D3D11Func_GetScanLine( D3D11* d3d, LPDWORD scanline );
HRESULT D3D11Func_GetVerticalBlankStatus( D3D11* d3d, LPBOOL lpbIsInVB );
HRESULT D3D11Func_LazyPresent( D3D11* d3d, D3D11Surface* src, D3D11Surface* dst );
HRESULT D3D11Func_Present( D3D11* d3d );
HRESULT D3D11Func_CreateSurface( D3D11* d3d, D3D11Surface** ppsurface, DDSURFACEDESC2* pddsd );
HRESULT D3D11Func_DeleteSurface( D3D11Surface** ppsurface );
HRESULT D3D11Func_SetViewport( D3D11* d3d, D3DVIEWPORT7* vp );
HRESULT D3D11Func_ClearRT( D3D11* d3d, DWORD dwColour );
HRESULT D3D11Func_ClearDS( D3D11* d3d, DWORD flag, float z, DWORD stencil );

HRESULT D3D11SurfaceFunc_GetDC( D3D11Surface* surface, HDC* pDC );
HRESULT D3D11SurfaceFunc_ReleaseDC( D3D11Surface* surface, HDC* pDC );
HRESULT D3D11SurfaceFunc_GetParentContext( D3D11Surface*, D3D11** ppd3d );
HRESULT D3D11SurfaceFunc_Blt( D3D11* d3d, D3D11Surface* surface,  LPRECT lpDestRect, LPRECT lpSrcRect, DWORD dwFlags, LPDDBLTFX lpDDBltFx );