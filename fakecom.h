#pragma once


#ifdef __cplusplus

#define __STRUCT__ struct
#define interface __STRUCT__
#define STDMETHOD(method)        virtual HRESULT STDMETHODCALLTYPE method
#define STDMETHOD_(type,method)  virtual type STDMETHODCALLTYPE method
#define STDMETHODV(method)       virtual HRESULT STDMETHODVCALLTYPE method
#define STDMETHODV_(type,method) virtual type STDMETHODVCALLTYPE method
#define PURE                    = 0
#define THIS_
#define THIS                    void

#define FAKEINTERFACE( intf, inh ) struct intf : inh 
#else

#define interface               struct

#define STDMETHOD(method)       HRESULT (STDMETHODCALLTYPE * method)
#define STDMETHOD_(type,method) type (STDMETHODCALLTYPE * method)
#define STDMETHODV(method)       HRESULT (STDMETHODVCALLTYPE * method)
#define STDMETHODV_(type,method) type (STDMETHODVCALLTYPE * method)

#define IFACEMETHOD(method)         __override STDMETHOD(method)
#define IFACEMETHOD_(type,method)   __override STDMETHOD_(type,method)
#define IFACEMETHODV(method)        __override STDMETHODV(method)
#define IFACEMETHODV_(type,method)  __override STDMETHODV_(type,method)

#define PURE
#define THIS_                   INTERFACE FAR* This,
#define THIS                    INTERFACE FAR* This

#define FAKEINTERFACE( intf, inh ) typedef struct intf {\
			struct intf##Vtbl* lpVtbl; \
} intf;  typedef struct intf##Vtbl intf##Vtbl;  struct intf##Vtbl  
#endif



/*
 * IUnknownFake 
 */
typedef struct IUnknownFake
{
#ifdef __cplusplus
	virtual HRESULT WINAPI QueryInterface( REFIID riid, LPVOID FAR * ppvObj ) PURE;
	virtual ULONG WINAPI AddRef() PURE;
	virtual ULONG WINAPI Release() PURE;
#else
	struct IUnknownFakeVtbl* lpVtbl;
#endif
	VOID* reserved;
} IUnknownFake;
