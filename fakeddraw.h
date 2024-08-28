/*
 * Fake DirectDraw
 * 
 * (C) Shogunate 2024
 * 
 * File: fakeddraw.h
 * Desc: A fake version of DirectDraw using Direct3D11 instead.
 */

#ifndef __FAKE_DDRAW_INCLUDED__
#define __FAKE_DDRAW_INCLUDED__

#include <windows.h>

#include "fakecom.h"

/*
 * Fake DirectDraw interfaces
 */
struct IDirectDrawFake;
struct IDirectDrawSurfaceFake;
struct IDirectDrawClipperFake;
struct IDirectDrawPaletteFake;

typedef struct IDirectDrawFake IDirectDrawFake;
typedef struct IDirectDrawSurfaceFake IDirectDrawSurfaceFake;
typedef struct IDirectDrawClipperFake IDirectDrawClipperFake;
typedef struct IDirectDrawPaletteFake IDirectDrawPaletteFake;


HRESULT WINAPI DirectDrawFakeCreate( GUID FAR *lpGUID, IDirectDrawFake FAR **lplpDD, IUnknownFake FAR *pUnkOuter );
HRESULT WINAPI DirectDrawFakeCreateEx( GUID FAR * lpGuid, LPVOID  *lplpDD, REFIID  iid,IUnknownFake FAR *pUnkOuter );
HRESULT WINAPI DirectDrawFakeCreateClipper( DWORD dwFlags, IDirectDrawClipperFake FAR **lplpDDClipper, IUnknownFake FAR *pUnkOuter );

/*
 * IDirectDrawFake
 */
#if 0
typedef struct IDirectDrawFake
#ifdef __cplusplus
	: public IUnknownFake
#endif
{
#ifdef __cplusplus
#else
	struct IDirectDrawFakeVtbl* lpVtbl;
#endif

	VOID* reserved;
}IDirectDrawFake;
typedef struct IDirectDrawFakeVtbl IDirectDrawFakeVtbl;
struct IDirectDrawFakeVtbl;
#else
#undef INTERFACE
#define INTERFACE IDirectDrawFake
FAKEINTERFACE( IDirectDrawFake, IUnknownFake )
{
	/*** IUnknown methods ***/
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR * ppvObj);
    STDMETHOD_(ULONG,AddRef) (THIS);
    STDMETHOD_(ULONG,Release) (THIS);

    /*** IDirectDraw methods ***/
    STDMETHOD(Compact)(THIS);
    STDMETHOD(CreateClipper)(THIS_ DWORD, IDirectDrawClipperFake**, IUnknownFake FAR * );
    STDMETHOD(CreatePalette)(THIS_ DWORD, LPPALETTEENTRY, IDirectDrawPaletteFake* FAR*, IUnknownFake FAR * );
    STDMETHOD(CreateSurface)(THIS_  LPDDSURFACEDESC2, IDirectDrawSurfaceFake* FAR *, IUnknownFake FAR *);
    STDMETHOD(DuplicateSurface)( THIS_ IDirectDrawSurfaceFake*, IDirectDrawSurfaceFake* FAR * );
    STDMETHOD(EnumDisplayModes)( THIS_ DWORD, LPDDSURFACEDESC2, LPVOID, LPDDENUMMODESCALLBACK2 );
    STDMETHOD(EnumSurfaces)(THIS_ DWORD, LPDDSURFACEDESC2, LPVOID,LPDDENUMSURFACESCALLBACK7 );
    STDMETHOD(FlipToGDISurface)(THIS);
    STDMETHOD(GetCaps)( THIS_ LPDDCAPS, LPDDCAPS);
    STDMETHOD(GetDisplayMode)( THIS_ LPDDSURFACEDESC2);
    STDMETHOD(GetFourCCCodes)(THIS_  LPDWORD, LPDWORD );
    STDMETHOD(GetGDISurface)(THIS_ IDirectDrawSurfaceFake* FAR *);
    STDMETHOD(GetMonitorFrequency)(THIS_ LPDWORD);
    STDMETHOD(GetScanLine)(THIS_ LPDWORD);
    STDMETHOD(GetVerticalBlankStatus)(THIS_ LPBOOL );
    STDMETHOD(Initialize)(THIS_ GUID FAR *);
    STDMETHOD(RestoreDisplayMode)(THIS);
    STDMETHOD(SetCooperativeLevel)(THIS_ HWND, DWORD);
    STDMETHOD(SetDisplayMode)(THIS_ DWORD, DWORD,DWORD, DWORD, DWORD);
    STDMETHOD(WaitForVerticalBlank)(THIS_ DWORD, HANDLE );
    /*** Added in the v2 interface ***/
    STDMETHOD(GetAvailableVidMem)(THIS_ LPDDSCAPS2, LPDWORD, LPDWORD);
    /*** Added in the V4 Interface ***/
    STDMETHOD(GetSurfaceFromDC) (THIS_ HDC, IDirectDrawSurfaceFake**);
    STDMETHOD(RestoreAllSurfaces)(THIS);
    STDMETHOD(TestCooperativeLevel)(THIS);
    STDMETHOD(GetDeviceIdentifier)(THIS_ LPDDDEVICEIDENTIFIER2, DWORD );
    STDMETHOD(StartModeTest)(THIS_ LPSIZE, DWORD, DWORD );
    STDMETHOD(EvaluateMode)(THIS_ DWORD, DWORD * );
};
#endif


/*
 * IDirectDrawSurfaceFake
 */
#if 0
typedef struct IDirectDrawSurfaceFake
#ifdef __cplusplus
	: public IUnknownFake
#endif
{
#ifdef __cplusplus
#else
	struct IDirectDrawSurfaceFakeVtbl* lpVtbl;
#endif

	VOID* reserved;
}IDirectDrawSurfaceFake;
typedef struct IDirectDrawSurfaceFakeVtbl IDirectDrawSurfaceFakeVtbl;
struct IDirectDrawSurfaceFakeVtbl;
#else
#undef INTERFACE
#define INTERFACE IDirectDrawSurfaceFake
FAKEINTERFACE( IDirectDrawSurfaceFake, IUnknownFake )
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR * ppvObj);
    STDMETHOD_(ULONG,AddRef) (THIS);
    STDMETHOD_(ULONG,Release) (THIS);
    /*** IDirectDrawSurface methods ***/
    STDMETHOD(AddAttachedSurface)(THIS_ IDirectDrawSurfaceFake*);
    STDMETHOD(AddOverlayDirtyRect)(THIS_ LPRECT);
    STDMETHOD(Blt)(THIS_ LPRECT,IDirectDrawSurfaceFake*, LPRECT,DWORD, LPDDBLTFX);
    STDMETHOD(BltBatch)(THIS_ LPDDBLTBATCH, DWORD, DWORD );
    STDMETHOD(BltFast)(THIS_ DWORD,DWORD,IDirectDrawSurfaceFake*, LPRECT,DWORD);
    STDMETHOD(DeleteAttachedSurface)(THIS_ DWORD,IDirectDrawSurfaceFake*);
    STDMETHOD(EnumAttachedSurfaces)(THIS_ LPVOID,LPDDENUMSURFACESCALLBACK7);
    STDMETHOD(EnumOverlayZOrders)(THIS_ DWORD,LPVOID,LPDDENUMSURFACESCALLBACK7);
    STDMETHOD(Flip)(THIS_ IDirectDrawSurfaceFake*, DWORD);
    STDMETHOD(GetAttachedSurface)(THIS_ LPDDSCAPS2, IDirectDrawSurfaceFake* FAR *);
    STDMETHOD(GetBltStatus)(THIS_ DWORD);
    STDMETHOD(GetCaps)(THIS_ LPDDSCAPS2);
    STDMETHOD(GetClipper)(THIS_ IDirectDrawClipperFake* FAR*);
    STDMETHOD(GetColorKey)(THIS_ DWORD, LPDDCOLORKEY);
    STDMETHOD(GetDC)(THIS_ HDC FAR *);
    STDMETHOD(GetFlipStatus)(THIS_ DWORD);
    STDMETHOD(GetOverlayPosition)(THIS_ LPLONG, LPLONG );
    STDMETHOD(GetPalette)(THIS_ IDirectDrawPaletteFake* FAR*);
    STDMETHOD(GetPixelFormat)(THIS_ LPDDPIXELFORMAT);
    STDMETHOD(GetSurfaceDesc)(THIS_ LPDDSURFACEDESC2);
    STDMETHOD(Initialize)(THIS_ IDirectDrawFake*, LPDDSURFACEDESC2);
    STDMETHOD(IsLost)(THIS);
    STDMETHOD(Lock)(THIS_ LPRECT,LPDDSURFACEDESC2,DWORD,HANDLE);
    STDMETHOD(ReleaseDC)(THIS_ HDC);
    STDMETHOD(Restore)(THIS);
    STDMETHOD(SetClipper)(THIS_ IDirectDrawClipperFake*);
    STDMETHOD(SetColorKey)(THIS_ DWORD, LPDDCOLORKEY);
    STDMETHOD(SetOverlayPosition)(THIS_ LONG, LONG );
    STDMETHOD(SetPalette)(THIS_ IDirectDrawPaletteFake*);
    STDMETHOD(Unlock)(THIS_ LPRECT);
    STDMETHOD(UpdateOverlay)(THIS_ LPRECT, IDirectDrawSurfaceFake*,LPRECT,DWORD, LPDDOVERLAYFX);
    STDMETHOD(UpdateOverlayDisplay)(THIS_ DWORD);
    STDMETHOD(UpdateOverlayZOrder)(THIS_ DWORD, IDirectDrawSurfaceFake*);
    /*** Added in the v2 interface ***/
    STDMETHOD(GetDDInterface)(THIS_ LPVOID FAR *);
    STDMETHOD(PageLock)(THIS_ DWORD);
    STDMETHOD(PageUnlock)(THIS_ DWORD);
    /*** Added in the v3 interface ***/
    STDMETHOD(SetSurfaceDesc)(THIS_ LPDDSURFACEDESC2, DWORD);
    /*** Added in the v4 interface ***/
    STDMETHOD(SetPrivateData)(THIS_ REFGUID, LPVOID, DWORD, DWORD);
    STDMETHOD(GetPrivateData)(THIS_ REFGUID, LPVOID, LPDWORD);
    STDMETHOD(FreePrivateData)(THIS_ REFGUID);
    STDMETHOD(GetUniquenessValue)(THIS_ LPDWORD);
    STDMETHOD(ChangeUniquenessValue)(THIS);
    /*** Moved Texture7 methods here ***/
    STDMETHOD(SetPriority)(THIS_ DWORD);
    STDMETHOD(GetPriority)(THIS_ LPDWORD);
    STDMETHOD(SetLOD)(THIS_ DWORD);
    STDMETHOD(GetLOD)(THIS_ LPDWORD);
};
#endif


/*
 * IDirectDrawClipperFake
 */
#if 0
typedef struct IDirectDrawClipperFake
#ifdef __cplusplus
	: public IUnknownFake
#endif
{
#ifdef __cplusplus
#else
	struct IDirectDrawClipperFakeVtbl* lpVtbl;
#endif

	VOID* reserved;
}IDirectDrawClipperFake;
typedef struct IDirectDrawClipperFakeVtbl IDirectDrawClipperFakeVtbl;
struct IDirectDrawClipperFakeVtbl;
#else
#undef INTERFACE
#define INTERFACE IDirectDrawClipperFake
DECLARE_INTERFACE_( IDirectDrawClipperFake, IUnknownFake )
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR * ppvObj);
    STDMETHOD_(ULONG,AddRef) (THIS);
    STDMETHOD_(ULONG,Release) (THIS);
    /*** IDirectDrawClipper methods ***/
    STDMETHOD(GetClipList)(THIS_ LPRECT, LPRGNDATA, LPDWORD);
    STDMETHOD(GetHWnd)(THIS_ HWND FAR *);
    STDMETHOD(Initialize)(THIS_ IDirectDrawFake*, DWORD) ;
    STDMETHOD(IsClipListChanged)(THIS_ BOOL FAR *);
    STDMETHOD(SetClipList)(THIS_ LPRGNDATA,DWORD) ;
    STDMETHOD(SetHWnd)(THIS_ DWORD, HWND ) ;
};

#endif



/*
 * IDirectDrawPaletteFake
 */
#if 0
typedef struct IDirectDrawPaletteFake
#ifdef __cplusplus
	: public IUnknownFake
#endif
{
#ifdef __cplusplus
#else
	struct IDirectDrawPaletteFakeVtbl* lpVtbl;
#endif

	VOID* reserved;
}IDirectDrawPaletteFake;
typedef struct IDirectDrawPaletteFakeVtbl IDirectDrawPaletteFakeVtbl;
struct IDirectDrawPaletteFakeVtbl;
#else
#undef INTERFACE
#define INTERFACE IDirectDrawPaletteFake
DECLARE_INTERFACE_( IDirectDrawPaletteFake, IUnknownFake )
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
    STDMETHOD_(ULONG,Release) (THIS) PURE;
    /*** IDirectDrawPalette methods ***/
    STDMETHOD(GetCaps)(THIS_ LPDWORD) PURE;
    STDMETHOD(GetEntries)(THIS_ DWORD,DWORD,DWORD,LPPALETTEENTRY) PURE;
    STDMETHOD(Initialize)(THIS_ IDirectDrawFake*, DWORD, LPPALETTEENTRY) PURE;
    STDMETHOD(SetEntries)(THIS_ DWORD,DWORD,DWORD,LPPALETTEENTRY) PURE;
};
#endif

#endif