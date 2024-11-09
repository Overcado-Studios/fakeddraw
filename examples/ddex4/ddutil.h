/*==========================================================================
 *
 *  Copyright (C) 1998 Microsoft Corporation. All Rights Reserved.
 *
 *  File:       ddutil.cpp
 *  Content:    Routines for loading bitmap and palettes from resources
 *
 ***************************************************************************/
#ifndef __DDUTIL_INCLUDED__
#define __DDUTIL_INCLUDED__

#include <ddraw.h>
#include <fakeddraw.h>

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif	/* __cplusplus */

extern IDirectDrawPaletteFake  *DDLoadPalette(IDirectDrawFake* pdd, LPCSTR szBitmap);
extern IDirectDrawSurfaceFake  *DDLoadBitmap(IDirectDrawFake *pdd, LPCSTR szBitmap, int dx, int dy);
extern HRESULT					DDReLoadBitmap(IDirectDrawSurfaceFake *pdds, LPCSTR szBitmap);
extern HRESULT					DDCopyBitmap(IDirectDrawSurfaceFake *pdds,HBITMAP hbm/*LPCSTR szBitmap*/, int x, int y, int dx, int dy);
extern DWORD					DDColorMatch(IDirectDrawSurfaceFake *pdds, COLORREF rgb);
extern HRESULT					DDSetColorKey(IDirectDrawSurfaceFake *pdds, COLORREF rgb);
extern void DDFillSurface(LPDIRECTDRAWSURFACE g_pDDs, WORD color);

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif