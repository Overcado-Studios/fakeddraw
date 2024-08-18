/*
 * Fake DirectDraw
 * 
 * (C) Shogunate 2024
 * 
 * File: faked3d.h
 * Desc: A fake version of Direct3D immediate mode using Direct3D11 instead.
 */

#ifndef __FAKE_D3DIM_INCLUDED__
#define __FAKE_D3DIM_INCLUDED__

#include <windows.h>

#include "fakecom.h"
#include "fakeddraw.h"

#undef PURE
#define PURE
/*
 * Fake DirectDraw interfaces
 */
struct IDirect3DFake;
struct IDirect3DDeviceFake;
struct IDirect3DVertexBufferFake;

typedef struct IDirect3DFake IDirect3DFake;
typedef struct IDirect3DDeviceFake IDirect3DDeviceFake;
typedef struct IDirect3DVertexBufferFake IDirect3DVertexBufferFake;


/*
 * IDirect3DFake 
 */
#undef INTERFACE
#define INTERFACE IDirect3DFake

FAKEINTERFACE(IDirect3DFake, IUnknownFake)
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    /*** IDirect3D7 methods ***/
    STDMETHOD(EnumDevices)(THIS_ LPD3DENUMDEVICESCALLBACK7,LPVOID) PURE;
    STDMETHOD(CreateDevice)(THIS_ REFCLSID,IDirectDrawSurfaceFake*,IDirect3DDeviceFake**) PURE;
    STDMETHOD(CreateVertexBuffer)(THIS_ LPD3DVERTEXBUFFERDESC,IDirect3DVertexBufferFake**,DWORD) PURE;
    STDMETHOD(EnumZBufferFormats)(THIS_ REFCLSID,LPD3DENUMPIXELFORMATSCALLBACK,LPVOID) PURE;
    STDMETHOD(EvictManagedTextures)(THIS) PURE;
};


/*
 * IDirect3DDeviceFake  
 */
#undef INTERFACE
#define INTERFACE IDirect3DDeviceFake

FAKEINTERFACE(IDirect3DDeviceFake, IUnknownFake)
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    /*** IDirect3DDevice7 methods ***/
    STDMETHOD(GetCaps)(THIS_ LPD3DDEVICEDESC7) PURE;
    STDMETHOD(EnumTextureFormats)(THIS_ LPD3DENUMPIXELFORMATSCALLBACK,LPVOID) PURE;
    STDMETHOD(BeginScene)(THIS) PURE;
    STDMETHOD(EndScene)(THIS) PURE;
    STDMETHOD(GetDirect3D)(THIS_ IDirect3DFake**) PURE;
    STDMETHOD(SetRenderTarget)(THIS_ IDirectDrawSurfaceFake*,DWORD) PURE;
    STDMETHOD(GetRenderTarget)(THIS_ IDirectDrawSurfaceFake**) PURE;
    STDMETHOD(Clear)(THIS_ DWORD,LPD3DRECT,DWORD,D3DCOLOR,D3DVALUE,DWORD) PURE;
    STDMETHOD(SetTransform)(THIS_ D3DTRANSFORMSTATETYPE,LPD3DMATRIX) PURE;
    STDMETHOD(GetTransform)(THIS_ D3DTRANSFORMSTATETYPE,LPD3DMATRIX) PURE;
    STDMETHOD(SetViewport)(THIS_ LPD3DVIEWPORT7) PURE;
    STDMETHOD(MultiplyTransform)(THIS_ D3DTRANSFORMSTATETYPE,LPD3DMATRIX) PURE;
    STDMETHOD(GetViewport)(THIS_ LPD3DVIEWPORT7) PURE;
    STDMETHOD(SetMaterial)(THIS_ LPD3DMATERIAL7) PURE;
    STDMETHOD(GetMaterial)(THIS_ LPD3DMATERIAL7) PURE;
    STDMETHOD(SetLight)(THIS_ DWORD,LPD3DLIGHT7) PURE;
    STDMETHOD(GetLight)(THIS_ DWORD,LPD3DLIGHT7) PURE;
    STDMETHOD(SetRenderState)(THIS_ D3DRENDERSTATETYPE,DWORD) PURE;
    STDMETHOD(GetRenderState)(THIS_ D3DRENDERSTATETYPE,LPDWORD) PURE;
    STDMETHOD(BeginStateBlock)(THIS) PURE;
    STDMETHOD(EndStateBlock)(THIS_ LPDWORD) PURE;
    STDMETHOD(PreLoad)(THIS_ IDirectDrawSurfaceFake*) PURE;
    STDMETHOD(DrawPrimitive)(THIS_ D3DPRIMITIVETYPE,DWORD,LPVOID,DWORD,DWORD) PURE;
    STDMETHOD(DrawIndexedPrimitive)(THIS_ D3DPRIMITIVETYPE,DWORD,LPVOID,DWORD,LPWORD,DWORD,DWORD) PURE;
    STDMETHOD(SetClipStatus)(THIS_ LPD3DCLIPSTATUS) PURE;
    STDMETHOD(GetClipStatus)(THIS_ LPD3DCLIPSTATUS) PURE;
    STDMETHOD(DrawPrimitiveStrided)(THIS_ D3DPRIMITIVETYPE,DWORD,LPD3DDRAWPRIMITIVESTRIDEDDATA,DWORD,DWORD) PURE;
    STDMETHOD(DrawIndexedPrimitiveStrided)(THIS_ D3DPRIMITIVETYPE,DWORD,LPD3DDRAWPRIMITIVESTRIDEDDATA,DWORD,LPWORD,DWORD,DWORD) PURE;
    STDMETHOD(DrawPrimitiveVB)(THIS_ D3DPRIMITIVETYPE,IDirect3DVertexBufferFake*,DWORD,DWORD,DWORD) PURE;
    STDMETHOD(DrawIndexedPrimitiveVB)(THIS_ D3DPRIMITIVETYPE,IDirect3DVertexBufferFake*,DWORD,DWORD,LPWORD,DWORD,DWORD) PURE;
    STDMETHOD(ComputeSphereVisibility)(THIS_ LPD3DVECTOR,LPD3DVALUE,DWORD,DWORD,LPDWORD) PURE;
    STDMETHOD(GetTexture)(THIS_ DWORD,IDirectDrawSurfaceFake**) PURE;
    STDMETHOD(SetTexture)(THIS_ DWORD,IDirectDrawSurfaceFake*) PURE;
    STDMETHOD(GetTextureStageState)(THIS_ DWORD,D3DTEXTURESTAGESTATETYPE,LPDWORD) PURE;
    STDMETHOD(SetTextureStageState)(THIS_ DWORD,D3DTEXTURESTAGESTATETYPE,DWORD) PURE;
    STDMETHOD(ValidateDevice)(THIS_ LPDWORD) PURE;
    STDMETHOD(ApplyStateBlock)(THIS_ DWORD) PURE;
    STDMETHOD(CaptureStateBlock)(THIS_ DWORD) PURE;
    STDMETHOD(DeleteStateBlock)(THIS_ DWORD) PURE;
    STDMETHOD(CreateStateBlock)(THIS_ D3DSTATEBLOCKTYPE,LPDWORD) PURE;
    STDMETHOD(Load)(THIS_ IDirectDrawSurfaceFake*,LPPOINT,IDirectDrawSurfaceFake*,LPRECT,DWORD) PURE;
    STDMETHOD(LightEnable)(THIS_ DWORD,BOOL) PURE;
    STDMETHOD(GetLightEnable)(THIS_ DWORD,BOOL*) PURE;
    STDMETHOD(SetClipPlane)(THIS_ DWORD,D3DVALUE*) PURE;
    STDMETHOD(GetClipPlane)(THIS_ DWORD,D3DVALUE*) PURE;
    STDMETHOD(GetInfo)(THIS_ DWORD,LPVOID,DWORD) PURE;
};


/*
 * IDirect3DVertexBufferFake
 */
#undef INTERFACE
#define INTERFACE IDirect3DVertexBufferFake

FAKEINTERFACE(IDirect3DVertexBufferFake, IUnknownFake)
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    /*** IDirect3DVertexBuffer7 methods ***/
    STDMETHOD(Lock)(THIS_ DWORD,LPVOID*,LPDWORD) PURE;
    STDMETHOD(Unlock)(THIS) PURE;
    STDMETHOD(ProcessVertices)(THIS_ DWORD,DWORD,DWORD,IDirect3DVertexBufferFake*,DWORD,IDirect3DDeviceFake*,DWORD) PURE;
    STDMETHOD(GetVertexBufferDesc)(THIS_ LPD3DVERTEXBUFFERDESC) PURE;
    STDMETHOD(Optimize)(THIS_ IDirect3DDeviceFake*,DWORD) PURE;
    STDMETHOD(ProcessVerticesStrided)(THIS_ DWORD,DWORD,DWORD,LPD3DDRAWPRIMITIVESTRIDEDDATA,DWORD,IDirect3DDeviceFake*,DWORD) PURE;
};

#undef PURE
#define PURE =0 // #undef PURE

#endif // __FAKE_D3DIM_INCLUDED__