// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		標準Ｃライブラリ使用コード		DirectShow BaseClasses
// 
// 				Ver 3.25a
//
//       Copyright (c) 1992-2002 Microsoft Corporation.  All rights reserved.
// 
// -------------------------------------------------------------------------------

#define DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_NAMESPACE
#ifdef DX_NON_USING_NAMESPACE_DXLIB

using namespace DxLib ;

#endif // DX_NON_USING_NAMESPACE_DXLIB
#endif // DX_NON_NAMESPACE

// インクルード ---------------------------------
#include "DxUseCStrmBaseFilter.h"
#include "DxDirectX.h"
#include "DxWindow.h"
#include "DxWinAPI.h"
#include "DxGuid.h"
#include "../DxUseCLib.h"
#include "../DxLib.h"
#include "../DxStatic.h"
#include "../DxBaseFunc.h"
#include "../DxBaseImage.h"

#if !defined( DX_NON_DSHOW_MOVIE ) && !defined( DX_NON_MOVIE )

#include <mmsystem.h>		// Needed for definition of timeGetTime
#include <limits.h>			// Standard data type limit definitions
#include <malloc.h>
#include <math.h>

#define ASSERT( x )
#define NOTE( x )
#define NOTE1( x, y )
#define NAME( x )	( ( TCHAR * )NULL )
#define RLOG( x )
#define EXECUTE_ASSERT

#define VFW_S_NO_MORE_ITEMS				((HRESULT)0x00040103L)
#define VFW_S_STATE_INTERMEDIATE		((HRESULT)0x00040237L)
#define VFW_S_NO_STOP_TIME				((HRESULT)0x00040270L)
#define VFW_E_INVALIDMEDIATYPE			((HRESULT)0x80040200L)
#define VFW_E_NO_ALLOCATOR				((HRESULT)0x8004020AL)
#define VFW_E_ALREADY_CONNECTED			((HRESULT)0x80040204L)
#define VFW_E_NO_ACCEPTABLE_TYPES		((HRESULT)0x80040207L)
#define VFW_E_INVALID_DIRECTION			((HRESULT)0x80040208L)
#define VFW_E_NOT_CONNECTED				((HRESULT)0x80040209L)
#define VFW_E_ENUM_OUT_OF_SYNC			((HRESULT)0x80040203L)
#define VFW_E_RUNTIME_ERROR				((HRESULT)0x8004020BL)
#define VFW_E_BUFFER_OVERFLOW			((HRESULT)0x8004020DL)
#define VFW_E_BADALIGN					((HRESULT)0x8004020EL)
#define VFW_E_ALREADY_COMMITTED			((HRESULT)0x8004020FL)
#define VFW_E_BUFFERS_OUTSTANDING		((HRESULT)0x80040210L)
#define VFW_E_NOT_COMMITTED				((HRESULT)0x80040211L)
#define VFW_E_SIZENOTSET				((HRESULT)0x80040212L)
#define VFW_E_NO_CLOCK					((HRESULT)0x80040213L)
#define VFW_E_NOT_FOUND					((HRESULT)0x80040216L)
#define VFW_E_STATE_CHANGED				((HRESULT)0x80040223L)
#define VFW_E_NOT_STOPPED				((HRESULT)0x80040224L)
#define VFW_E_WRONG_STATE				((HRESULT)0x80040227L)
#define VFW_E_START_TIME_AFTER_END		((HRESULT)0x80040228L)
#define VFW_E_TYPE_NOT_ACCEPTED			((HRESULT)0x8004022AL)
#define VFW_E_SAMPLE_REJECTED			((HRESULT)0x8004022BL)
#define VFW_E_TIMEOUT					((HRESULT)0x8004022EL)
#define VFW_E_INVALID_MEDIA_TYPE		((HRESULT)0x80040248L)
#define VFW_E_SAMPLE_TIME_NOT_SET		((HRESULT)0x80040249L)
#define VFW_E_MEDIA_TIME_NOT_SET		((HRESULT)0x80040251L)
#define VFW_E_NOT_IN_GRAPH				((HRESULT)0x8004025FL)

#define AM_GBF_PREVFRAMESKIPPED			1
#define AM_GBF_NOTASYNCPOINT			2
#define AM_GBF_NOWAIT					4
#define AM_GBF_NODDSURFACELOCK			8

#define EC_COMPLETE						0x01
#define EC_QUALITY_CHANGE				0x0B
#define EC_NOTIFY_WINDOW				0x19
#define EC_REPAINT						0x05
#define EC_DISPLAY_CHANGED				0x16
#define EC_ERRORABORT					0x03
#define EC_WINDOW_DESTROYED				0x15

#define D_iPALETTE_COLORS	256
#define D_iMASK_COLORS		3

#define DbgLog( x )
#define DbgBreak( x )
#define AmHresultFromWin32(x) (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, x))

#define ValidateReadPtr(p,cb)		0
#define ValidateWritePtr(p,cb)		0
#define ValidateReadWritePtr(p,cb)	0
#define ValidateStringPtr(p)		0
#define ValidateStringPtrA(p)		0
#define ValidateStringPtrW(p)		0

#define AVGPERIOD 4
#define CheckPointer(p,ret) {if((p)==NULL) return (ret);}

#define FCC(ch4) ((( *((DWORD *)(ch4)) & 0xFF) << 24) |     \
                  (( *((DWORD *)(ch4)) & 0xFF00) << 8) |    \
                  (( *((DWORD *)(ch4)) & 0xFF0000) >> 8) |  \
                  (( *((DWORD *)(ch4)) & 0xFF000000) >> 24))
#define RIFFROUND(cb) ((cb) + ((cb)&1))
#define RIFFNEXT(pChunk) ( D_RIFFCHUNK * )((LPBYTE)(pChunk) \
                          + sizeof( D_RIFFCHUNK) \
                          + RIFFROUND((( D_RIFFCHUNK * )pChunk)->cb))

#define DX_CSTRMBASE_MIN( a, b )		((a) > (b) ? (b) : (a))

#ifdef PERF
#define MSR_INIT() Msr_Init()
#define MSR_TERMINATE() Msr_Terminate()
#define MSR_REGISTER(a) Msr_Register(a)
#define MSR_RESET(a) Msr_Reset(a)
#define MSR_CONTROL(a) Msr_Control(a)
#define MSR_START(a) Msr_Start(a)
#define MSR_STOP(a) Msr_Stop(a)
#define MSR_NOTE(a) Msr_Note(a)
#define MSR_INTEGER(a,b) Msr_Integer(a,b)
#define MSR_DUMP(a) Msr_Dump(a)
#define MSR_DUMPSTATS(a) Msr_DumpStats(a)
#else
#define MSR_INIT() ((void)0)
#define MSR_TERMINATE() ((void)0)
#define MSR_REGISTER(a) 0
#define MSR_RESET(a) ((void)0)
#define MSR_CONTROL(a) ((void)0)
#define MSR_START(a) ((void)0)
#define MSR_STOP(a) ((void)0)
#define MSR_NOTE(a) ((void)0)
#define MSR_INTEGER(a,b) ((void)0)
#define MSR_DUMP(a) ((void)0)
#define MSR_DUMPSTATS(a) ((void)0)
#endif

//namespace DxLib
//{

enum __MIDL_IFilterMapper_0001
{
	MERIT_PREFERRED					= 0x800000,
	MERIT_NORMAL					= 0x600000,
	MERIT_UNLIKELY					= 0x400000,
	MERIT_DO_NOT_USE				= 0x200000,
	MERIT_SW_COMPRESSOR				= 0x100000,
	MERIT_HW_COMPRESSOR				= 0x100050
} ;

typedef struct tagD_TRUECOLORINFO
{
	DWORD				dwBitMasks[ D_iMASK_COLORS ] ;
	RGBQUAD				bmiColors[ D_iPALETTE_COLORS ] ;
} D_TRUECOLORINFO ;

typedef struct tagD_VIDEOINFO
{

	RECT				rcSource ;
	RECT				rcTarget ;
	DWORD				dwBitRate ;
	DWORD				dwBitErrorRate ;
	D_REFERENCE_TIME	AvgTimePerFrame ;
	BITMAPINFOHEADER	bmiHeader ;

	union
	{
		RGBQUAD			bmiColors[ D_iPALETTE_COLORS ] ;
		DWORD			dwBitMasks[ D_iMASK_COLORS ] ;
		D_TRUECOLORINFO	TrueColorInfo ;
	} ;

} D_VIDEOINFO ;

typedef struct tagD_RIFFCHUNK
{
	FOURCC				fcc;
	DWORD				cb;
} D_RIFFCHUNK ;

typedef struct tagD_RIFFLIST
{
	FOURCC fcc;
	DWORD  cb;
	 FOURCC fccListType;
} D_RIFFLIST ;

typedef D_CUnknown *(CALLBACK *LPFNNewCOMObject)(D_IUnknown * pUnkOuter, HRESULT *phr);
typedef void (CALLBACK *LPFNInitRoutine)(BOOL bLoading, const CLSID *rclsid);

class D_CFactoryTemplate {

public:

    const WCHAR *              m_Name;
    const CLSID *              m_ClsID;
    LPFNNewCOMObject           m_lpfnNew;
    LPFNInitRoutine            m_lpfnInit;
    const D_AMOVIESETUP_FILTER * m_pAMovieSetup_Filter;

    BOOL IsClassID(REFCLSID rclsid) const {
        return (IsEqualCLSID(*m_ClsID,rclsid));
    };

    D_CUnknown *CreateInstance(D_IUnknown * pUnk, HRESULT *phr) const {
        CheckPointer(phr,NULL);
        return m_lpfnNew(pUnk, phr);
    };
};


// {3C78B8E2-6C4D-11d1-ADE2-0000F8754B99}
static const GUID			CLSID_WavDest = { 0x3c78b8e2, 0x6c4d, 0x11d1, { 0xad, 0xe2, 0x0, 0x0, 0xf8, 0x75, 0x4b, 0x99 } };
       const D_AMOVIESETUP_FILTER sudWavDest =
{
    &CLSID_WavDest,           // clsID
    L"WAV Dest",              // strName
    MERIT_DO_NOT_USE,         // dwMerit
    0,                        // nPins
    0                         // lpPin
};

#ifndef DX_NON_DSHOW_MP3

// Global data
D_CFactoryTemplate g_Templates[]= {
    {L"WAV Dest", &CLSID_WavDest, D_CWavDestFilter::CreateInstance, NULL, &sudWavDest},
};

int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);

#endif

static			GUID		CLSID_AsyncSample   = { 0xc553f2c0, 0x1529, 0x11d0,0xb4,0xd1,0x00,0x80,0x5f,0x6c,0xbb,0xea } ;
static			GUID		CLSID_MovieRenderer = { 0x71771540, 0x2017, 0x11cf,0xae,0x26,0x00,0x20,0xaf,0xd7,0x97,0x67 } ;
static const	LONGLONG	MAX_TIME			= LL_PARAM( 0x7FFFFFFFFFFFFFFF ) ;
static const	TCHAR		szOle32Aut[]		= TEXT("OleAut32.dll") ;
static			HINSTANCE	hlibOLEAut32 ;

static DWORD		WINAPI	WaitDispatchingMessages		( HANDLE hObject, DWORD dwWait, HWND hwnd = NULL, UINT uMsg = 0, HANDLE hEvent = NULL) ;
static HINSTANCE			LoadOLEAut32				() ;
static LONGLONG		WINAPI	llMulDiv					( LONGLONG a, LONGLONG b, LONGLONG c, LONGLONG d ) ;
static MMRESULT				CompatibleTimeSetEvent		( UINT uDelay, UINT uResolution, LPD_TIMECALLBACK lpTimeProc, DWORD_PTR dwUser, UINT fuEvent ) ;
extern bool					TimeKillSynchronousFlagAvailable( void ) ;
static int					isqrt						( int x ) ;

STDAPI						D_AMGetWideString			( LPCWSTR psz, LPWSTR *ppszReturn) ;
STDAPI						D_AMovieSetupRegisterFilter	( const D_AMOVIESETUP_FILTER * const psetupdata, D_IFilterMapper *pIFM, BOOL bRegister ) ;
STDAPI						D_GetInterface				( D_IUnknown * pUnk, void **ppv) ;
HRESULT           WINAPI	D_CopyMediaType				( D_AM_MEDIA_TYPE *pmtTarget, const D_AM_MEDIA_TYPE *pmtSource );
D_AM_MEDIA_TYPE * WINAPI	D_CreateMediaType			( D_AM_MEDIA_TYPE const *pSrc );

HINSTANCE LoadOLEAut32()
{
	if( hlibOLEAut32 == 0 )
	{
		hlibOLEAut32 = LoadLibrary(szOle32Aut);
	}

	return hlibOLEAut32;
}

STDAPI D_AMGetWideString(LPCWSTR psz, LPWSTR *ppszReturn)
{
	SETUP_WIN_API

	CheckPointer(ppszReturn, E_POINTER);
	ValidateReadWritePtr(ppszReturn, sizeof(LPWSTR));
	DWORD nameLen = sizeof(WCHAR) * (lstrlenWFunc((WORD const *)psz)+1);
	*ppszReturn = (LPWSTR)WinAPIData.Win32Func.CoTaskMemAllocFunc(nameLen);
	if (*ppszReturn == NULL) {
		return E_OUTOFMEMORY;
	}
	_MEMCPY(*ppszReturn, psz, nameLen);
	return NOERROR;
}

HRESULT WINAPI D_CopyMediaType(D_AM_MEDIA_TYPE *pmtTarget, const D_AM_MEDIA_TYPE *pmtSource)
{
	SETUP_WIN_API

    //  We'll leak if we copy onto one that already exists - there's one
    //  case we can check like that - copying to itself.
    ASSERT(pmtSource != pmtTarget);
    *pmtTarget = *pmtSource;
    if (pmtSource->cbFormat != 0) {
        ASSERT(pmtSource->pbFormat != NULL);
        pmtTarget->pbFormat = (PBYTE)WinAPIData.Win32Func.CoTaskMemAllocFunc(pmtSource->cbFormat);
        if (pmtTarget->pbFormat == NULL) {
            pmtTarget->cbFormat = 0;
            return E_OUTOFMEMORY;
        } else {
            _MEMCPY((PVOID)pmtTarget->pbFormat, (PVOID)pmtSource->pbFormat,
                       pmtTarget->cbFormat);
        }
    }
    if (pmtTarget->pUnk != NULL) {
        pmtTarget->pUnk->AddRef();
    }

    return S_OK;
}

D_AM_MEDIA_TYPE * WINAPI D_CreateMediaType( D_AM_MEDIA_TYPE const *pSrc)
{
	SETUP_WIN_API

    ASSERT(pSrc);

    // Allocate a block of memory for the media type

    D_AM_MEDIA_TYPE *pMediaType =
        (D_AM_MEDIA_TYPE *)WinAPIData.Win32Func.CoTaskMemAllocFunc(sizeof(D_AM_MEDIA_TYPE));

    if (pMediaType == NULL) {
        return NULL;
    }
    // Copy the variable length format block

    HRESULT hr = D_CopyMediaType(pMediaType,pSrc);
    if (FAILED(hr)) {
        WinAPIData.Win32Func.CoTaskMemFreeFunc((PVOID)pMediaType);
        return NULL;
    }

    return pMediaType;
}

STDAPI D_AMovieSetupRegisterFilter( const D_AMOVIESETUP_FILTER * const psetupdata, D_IFilterMapper *pIFM, BOOL bRegister )
{
  DbgLog((LOG_TRACE, 3, TEXT("= D_AMovieSetupRegisterFilter")));

  // check we've got data
  //
  if( NULL == psetupdata ) return S_FALSE;

  // unregister filter
  // (as pins are subkeys of filter's CLSID key
  // they do not need to be removed separately).
  //
  DbgLog((LOG_TRACE, 3, TEXT("= = unregister filter")));
  HRESULT hr = pIFM->UnregisterFilter( *(psetupdata->clsID) );


  if( bRegister )
  {
	// register filter
	//
	DbgLog((LOG_TRACE, 3, TEXT("= = register filter")));
	hr = pIFM->RegisterFilter( *(psetupdata->clsID)
							 , psetupdata->strName
							 , psetupdata->dwMerit	);
	if( SUCCEEDED(hr) )
	{
	  // all its pins
	  //
	  DbgLog((LOG_TRACE, 3, TEXT("= = register filter pins")));
	  for( UINT m1=0; m1 < psetupdata->nPins; m1++ )
	  {
		hr = pIFM->RegisterPin( *(psetupdata->clsID)
							  , psetupdata->lpPin[m1].strName
							  , psetupdata->lpPin[m1].bRendered
							  , psetupdata->lpPin[m1].bOutput
							  , psetupdata->lpPin[m1].bZero
							  , psetupdata->lpPin[m1].bMany
							  , *(psetupdata->lpPin[m1].clsConnectsToFilter)
							  , psetupdata->lpPin[m1].strConnectsToPin );

		if( SUCCEEDED(hr) )
		{
		  // and each pin's media types
		  //
		  DbgLog((LOG_TRACE, 3, TEXT("= = register filter pin types")));
		  for( UINT m2=0; m2 < psetupdata->lpPin[m1].nMediaTypes; m2++ )
		  {
			hr = pIFM->RegisterPinType( *(psetupdata->clsID)
									  , psetupdata->lpPin[m1].strName
									  , *(psetupdata->lpPin[m1].lpMediaType[m2].clsMajorType)
									  , *(psetupdata->lpPin[m1].lpMediaType[m2].clsMinorType) );
			if( FAILED(hr) ) break;
		  }
		  if( FAILED(hr) ) break;
		}
		if( FAILED(hr) ) break;
	  }
	}
  }

  // handle one acceptable "error" - that
  // of filter not being registered!
  // (couldn't find a suitable #define'd
  // name for the error!)
  //
  if( 0x80070002 == hr)
	return NOERROR;
  else
	return hr;
}

// Waits for the HANDLE hObject.  While waiting messages sent
// to windows on our thread by SendMessage will be processed.
// Using this function to do waits and mutual exclusion
// avoids some deadlocks in objects with windows.
// Return codes are the same as for WaitForSingleObject
DWORD WINAPI WaitDispatchingMessages( HANDLE hObject, DWORD dwWait, HWND hwnd, UINT uMsg, HANDLE hEvent)
{
	BOOL bPeeked = FALSE;
	DWORD dwResult;
	DWORD dwStart = 0;
	DWORD dwThreadPriority = 0;

	static UINT uMsgId = 0;

	SETUP_WIN_API

	HANDLE hObjects[2] = { hObject, hEvent };
	if (dwWait != INFINITE && dwWait != 0) {
		dwStart = WinAPIData.Win32Func.GetTickCountFunc();
	}
	for (; ; ) {
		DWORD nCount = ( DWORD )( NULL != hEvent ? 2 : 1 );

		//  Minimize the chance of actually dispatching any messages
		//  by seeing if we can lock immediately.
		dwResult = WinAPIData.Win32Func.WaitForMultipleObjectsFunc(nCount, hObjects, FALSE, 0);
		if (dwResult < WAIT_OBJECT_0 + nCount) {
			break;
		}

		DWORD dwTimeOut = dwWait;
		if (dwTimeOut > 10) {
			dwTimeOut = 10;
		}
		dwResult = WinAPIData.Win32Func.MsgWaitForMultipleObjectsFunc(
							 ( DWORD )nCount,
							 hObjects,
							 FALSE,
							 ( DWORD )dwTimeOut,
							 ( DWORD )( hwnd == NULL ? QS_SENDMESSAGE :
											           QS_SENDMESSAGE + QS_POSTMESSAGE )) ;
		if (dwResult == WAIT_OBJECT_0 + nCount ||
			dwResult == WAIT_TIMEOUT && dwTimeOut != dwWait) {
			MSG msg;
			if (hwnd != NULL) {
				while (PeekMessageWFunc(&msg, hwnd, uMsg, uMsg, PM_REMOVE)) {
					DispatchMessageWFunc(&msg);
				}
			}
			// Do this anyway - the previous peek doesn't flush out the
			// messages
			PeekMessageWFunc(&msg, NULL, 0, 0, PM_NOREMOVE);

			if (dwWait != INFINITE && dwWait != 0) {
				DWORD dwNow = WinAPIData.Win32Func.GetTickCountFunc();

				// Working with differences handles wrap-around
				DWORD dwDiff = dwNow - dwStart;
				if (dwDiff > dwWait) {
					dwWait = 0;
				} else {
					dwWait -= dwDiff;
				}
				dwStart = dwNow;
			}
			if (!bPeeked) {
				//  Raise our priority to prevent our message queue
				//  building up
				dwThreadPriority = ( DWORD )WinAPIData.Win32Func.GetThreadPriorityFunc(WinAPIData.Win32Func.GetCurrentThreadFunc());
				if (dwThreadPriority < THREAD_PRIORITY_HIGHEST) {
					WinAPIData.Win32Func.SetThreadPriorityFunc(WinAPIData.Win32Func.GetCurrentThreadFunc(), THREAD_PRIORITY_HIGHEST);
				}
				bPeeked = TRUE;
			}
		} else {
			break;
		}
	}
	if (bPeeked) {
		WinAPIData.Win32Func.SetThreadPriorityFunc(WinAPIData.Win32Func.GetCurrentThreadFunc(), ( int )dwThreadPriority);
		if (HIWORD(WinAPIData.Win32Func.GetQueueStatusFunc(QS_POSTMESSAGE)) & QS_POSTMESSAGE) {
			if (uMsgId == 0) {
				uMsgId = WinAPIData.Win32Func.RegisterWindowMessageAFunc("AMUnblock");
			}
			if (uMsgId != 0) {
				MSG msg;
				//  Remove old ones
				while (PeekMessageWFunc(&msg, (HWND)-1, uMsgId, uMsgId, PM_REMOVE)) {
				}
			}
			WinAPIData.Win32Func.PostThreadMessageAFunc(WinAPIData.Win32Func.GetCurrentThreadIdFunc(), uMsgId, 0, 0);
		}
	}
	return dwResult;
}

STDAPI D_GetInterface(D_IUnknown * pUnk, void **ppv)
{
	CheckPointer(ppv, E_POINTER);
	*ppv = pUnk;
	pUnk->AddRef();
	return NOERROR;
}

/*	Compute (a * b + d) / c */
LONGLONG WINAPI llMulDiv(LONGLONG a, LONGLONG b, LONGLONG c, LONGLONG d)
{
	/*  Compute the absolute values to avoid signed arithmetic problems */
	ULARGE_INTEGER ua, ub;
	DWORDLONG uc;

	ua.QuadPart = (DWORDLONG)(a >= 0 ? a : -a);
	ub.QuadPart = (DWORDLONG)(b >= 0 ? b : -b);
	uc		  = (DWORDLONG)(c >= 0 ? c : -c);
	BOOL bSign = (a < 0) ^ (b < 0);

	/*  Do long multiplication */
	ULARGE_INTEGER p[2];
	p[0].QuadPart  = UInt32x32To64(ua.LowPart, ub.LowPart);

	/*  This next computation cannot overflow into p[1].HighPart because
		the max number we can compute here is:

				 (2 ** 32 - 1) * (2 ** 32 - 1) +  // ua.LowPart * ub.LowPart
	(2 ** 32) *  (2 ** 31) * (2 ** 32 - 1) * 2	// x.LowPart * y.HighPart * 2

	== 2 ** 96 - 2 ** 64 + (2 ** 64 - 2 ** 33 + 1)
	== 2 ** 96 - 2 ** 33 + 1
	< 2 ** 96
	*/

	ULARGE_INTEGER x;
	x.QuadPart	 = UInt32x32To64(ua.LowPart, ub.HighPart) +
					 UInt32x32To64(ua.HighPart, ub.LowPart) +
					 p[0].HighPart;
	p[0].HighPart  = x.LowPart;
	p[1].QuadPart  = UInt32x32To64(ua.HighPart, ub.HighPart) + x.HighPart;

	if (d != 0) {
		ULARGE_INTEGER ud[2];
		if (bSign) {
			ud[0].QuadPart = (DWORDLONG)(-d);
			if (d > 0) {
				/*  -d < 0 */
				ud[1].QuadPart = (DWORDLONG)(LONGLONG)-1;
			} else {
				ud[1].QuadPart = (DWORDLONG)0;
			}
		} else {
			ud[0].QuadPart = (DWORDLONG)d;
			if (d < 0) {
				ud[1].QuadPart = (DWORDLONG)(LONGLONG)-1;
			} else {
				ud[1].QuadPart = (DWORDLONG)0;
			}
		}
		/*  Now do extended addition */
		ULARGE_INTEGER uliTotal;

		/*  Add ls DWORDs */
		uliTotal.QuadPart  = (DWORDLONG)ud[0].LowPart + p[0].LowPart;
		p[0].LowPart		= uliTotal.LowPart;

		/*  Propagate carry */
		uliTotal.LowPart	= uliTotal.HighPart;
		uliTotal.HighPart  = 0;

		/*  Add 2nd most ls DWORDs */
		uliTotal.QuadPart += (DWORDLONG)ud[0].HighPart + p[0].HighPart;
		p[0].HighPart	  = uliTotal.LowPart;

		/*  Propagate carry */
		uliTotal.LowPart	= uliTotal.HighPart;
		uliTotal.HighPart  = 0;

		/*  Add MS DWORDLONGs - no carry expected */
		p[1].QuadPart	 += ud[1].QuadPart + uliTotal.QuadPart;

		/*  Now see if we got a sign change from the addition */
		if ((LONG)p[1].HighPart < 0) {
			bSign = !bSign;

			/*  Negate the current value (ugh!) */
			p[0].QuadPart  = ~p[0].QuadPart;
			p[1].QuadPart  = ~p[1].QuadPart;
			p[0].QuadPart += 1;
			p[1].QuadPart += (p[0].QuadPart == 0);
		}
	}

	/*  Now for the division */
	if (c < 0) {
		bSign = !bSign;
	}


	/*  This will catch c == 0 and overflow */
	if (uc <= p[1].QuadPart) {
		return bSign ? ( LONGLONG )ULL_NUM( 0x8000000000000000 ) :
					   ( LONGLONG )ULL_NUM( 0x7FFFFFFFFFFFFFFF ) ;
	}

	DWORDLONG ullResult;

	/*  Do the division */
	/*  If the dividend is a DWORD_LONG use the compiler */
	if (p[1].QuadPart == 0) {
		ullResult = p[0].QuadPart / uc;
		return bSign ? -(LONGLONG)ullResult : (LONGLONG)ullResult;
	}

	/*  If the divisor is a DWORD then its simpler */
	ULARGE_INTEGER ulic;
	ulic.QuadPart = uc;
	if (ulic.HighPart == 0) {
		ULARGE_INTEGER uliDividend;
		ULARGE_INTEGER uliResult;
		DWORD dwDivisor = (DWORD)uc;
		// ASSERT(p[1].HighPart == 0 && p[1].LowPart < dwDivisor);
		uliDividend.HighPart = p[1].LowPart;
		uliDividend.LowPart = p[0].HighPart;
#ifndef USE_LARGEINT
		uliResult.HighPart = (DWORD)(uliDividend.QuadPart / dwDivisor);
		p[0].HighPart = (DWORD)(uliDividend.QuadPart % dwDivisor);
		uliResult.LowPart = 0;
		uliResult.QuadPart = p[0].QuadPart / dwDivisor + uliResult.QuadPart;
#else
		/*  NOTE - this routine will take exceptions if
			the result does not fit in a DWORD
		*/
		if (uliDividend.QuadPart >= (DWORDLONG)dwDivisor) {
			uliResult.HighPart = EnlargedUnsignedDivide(
									 uliDividend,
									 dwDivisor,
									 &p[0].HighPart);
		} else {
			uliResult.HighPart = 0;
		}
		uliResult.LowPart = EnlargedUnsignedDivide(
								 p[0],
								 dwDivisor,
								 NULL);
#endif
		return bSign ? -(LONGLONG)uliResult.QuadPart :
						(LONGLONG)uliResult.QuadPart;
	}


	ullResult = 0;

	/*  OK - do long division */
	for (int i = 0; i < 64; i++) {
		ullResult <<= 1;

		/*  Shift 128 bit p left 1 */
		p[1].QuadPart <<= 1;
		if ((p[0].HighPart & 0x80000000) != 0) {
			p[1].LowPart++;
		}
		p[0].QuadPart <<= 1;

		/*  Compare */
		if (uc <= p[1].QuadPart) {
			p[1].QuadPart -= uc;
			ullResult += 1;
		}
	}

	return bSign ? - (LONGLONG)ullResult : (LONGLONG)ullResult;
}

MMRESULT CompatibleTimeSetEvent( UINT uDelay, UINT uResolution, LPD_TIMECALLBACK lpTimeProc, DWORD_PTR dwUser, UINT fuEvent )
{
	SETUP_WIN_API

	#if WINVER >= 0x0501
	{
		static bool fCheckedVersion = false;
		static bool fTimeKillSynchronousFlagAvailable = false; 

		if( !fCheckedVersion ) {
			fTimeKillSynchronousFlagAvailable = TimeKillSynchronousFlagAvailable();
			fCheckedVersion = true;
		}

		if( fTimeKillSynchronousFlagAvailable ) {
			fuEvent = fuEvent | TIME_KILL_SYNCHRONOUS;
		}
	}
	#endif // WINVER >= 0x0501

	return WinAPIData.Win32Func.timeSetEventFunc( uDelay, uResolution, lpTimeProc, dwUser, fuEvent );
}

bool TimeKillSynchronousFlagAvailable( void )
{
    OSVERSIONINFOW osverinfo;

	SETUP_WIN_API

    osverinfo.dwOSVersionInfoSize = sizeof(osverinfo);

    if( GetVersionExWFunc( &osverinfo ) ) {
        
        // Windows XP's major version is 5 and its' minor version is 1.
        // timeSetEvent() started supporting the TIME_KILL_SYNCHRONOUS flag
        // in Windows XP.
        if( (osverinfo.dwMajorVersion > 5) || 
            ( (osverinfo.dwMajorVersion == 5) && (osverinfo.dwMinorVersion >= 1) ) ) {
            return true;
        }
    }

    return false;
}

#pragma warning(disable:4355)

//  Helper function for clamping time differences
int inline TimeDiff( D_REFERENCE_TIME rt)
{
	if (rt < - (50 * UNITS)) {
		return -(50 * UNITS);
	} else
	if (rt > 50 * UNITS) {
		return 50 * UNITS;
	} else return (int)rt;
}

// Implements the D_CBaseRenderer class
D_CBaseRenderer::D_CBaseRenderer(REFCLSID RenderClass, // CLSID for this renderer
							 TCHAR *pName,		 // Debug ONLY description
							 D_IUnknown * pUnk,		// Aggregated owner object
							 HRESULT * /*phr*/) :		// General OLE return code

	D_CBaseFilter(pName,pUnk,&m_InterfaceLock,RenderClass),
	m_evComplete(TRUE),
	m_bAbort(FALSE),
	m_pPosition(NULL),
	m_ThreadSignal(TRUE),
	m_bStreaming(FALSE),
	m_bEOS(FALSE),
	m_bEOSDelivered(FALSE),
	m_pMediaSample(NULL),
	m_dwAdvise(0),
	m_pQSink(NULL),
	m_pInputPin(NULL),
	m_bRepaintStatus(TRUE),
	m_SignalTime(0),
	m_bInReceive(FALSE),
	m_EndOfStreamTimer(0)
{
	Ready();
#ifdef PERF
	m_idBaseStamp = MSR_REGISTER(TEXT("BaseRenderer: sample time stamp"));
	m_idBaseRenderTime = MSR_REGISTER(TEXT("BaseRenderer: draw time (msec)"));
	m_idBaseAccuracy = MSR_REGISTER(TEXT("BaseRenderer: Accuracy (msec)"));
#endif
}


// Delete the dynamically allocated D_IMediaPosition and D_IMediaSeeking helper
// object. The object is created when somebody queries us. These are standard
// control interfaces for seeking and setting start/stop positions and rates.
// We will probably also have made an input pin based on D_CRendererInputPin
// that has to be deleted, it's created when an enumerator calls our GetPin

D_CBaseRenderer::~D_CBaseRenderer()
{
	ASSERT(m_bStreaming == FALSE);
	ASSERT(m_EndOfStreamTimer == 0);
	StopStreaming();
	ClearPendingSample();

	// Delete any D_IMediaPosition implementation

	if (m_pPosition) {
		delete m_pPosition;
		m_pPosition = NULL;
	}

	// Delete any input pin created

	if (m_pInputPin) {
		delete m_pInputPin;
		m_pInputPin = NULL;
	}

	// Release any D_Quality sink

	ASSERT(m_pQSink == NULL);
}

// This returns the D_IMediaPosition and D_IMediaSeeking interfaces

HRESULT D_CBaseRenderer::GetMediaPositionInterface(REFIID riid,void **ppv)
{
	D_CAutoLock cObjectCreationLock(&m_ObjectCreationLock);
	if (m_pPosition) {
		return m_pPosition->NonDelegatingQueryInterface(riid,ppv);
	}

	HRESULT hr = NOERROR;

	// Create implementation of this dynamically since sometimes we may
	// never try and do a seek. The helper object implements a position
	// control interface (D_IMediaPosition) which in fact simply takes the
	// calls normally from the filter graph and passes them upstream

	m_pPosition = new D_CRendererPosPassThru(TEXT("Renderer D_CPosPassThru"),
											D_CBaseFilter::GetOwner(),
											(HRESULT *) &hr,
											GetPin(0));
	if (m_pPosition == NULL) {
		return E_OUTOFMEMORY;
	}

	if (FAILED(hr)) {
		delete m_pPosition;
		m_pPosition = NULL;
		return E_NOINTERFACE;
	}
	return GetMediaPositionInterface(riid,ppv);
}


// Overriden to say what interfaces we support and where

STDMETHODIMP D_CBaseRenderer::NonDelegatingQueryInterface(REFIID riid,void **ppv)
{
	// Do we have this interface

	if (riid == IID_IMEDIAPOSITION || riid == IID_IMEDIASEEKING ) {
		return GetMediaPositionInterface(riid,ppv);
	} else {
		return D_CBaseFilter::NonDelegatingQueryInterface(riid,ppv);
	}
}


// This is called whenever we change states, we have a manual reset event that
// is signalled whenever we don't won't the source filter thread to wait in us
// (such as in a stopped state) and likewise is not signalled whenever it can
// wait (during paused and running) this function sets or resets the thread
// event. The event is used to stop source filter threads waiting in Receive

HRESULT D_CBaseRenderer::SourceThreadCanWait(BOOL bCanWait)
{
	if (bCanWait == TRUE) {
		m_ThreadSignal.Reset();
	} else {
		m_ThreadSignal.Set();
	}
	return NOERROR;
}


#ifdef DEBUG
// Dump the current renderer state to the debug terminal. The hardest part of
// the renderer is the window where we unlock everything to wait for a clock
// to signal it is time to draw or for the application to cancel everything
// by stopping the filter. If we get things wrong we can leave the thread in
// WaitForRenderTime with no way for it to ever get out and we will deadlock

void D_CBaseRenderer::DisplayRendererState()
{
	DbgLog((LOG_TIMING, 1, TEXT("\nTimed out in WaitForRenderTime")));

	// No way should this be signalled at this point

	BOOL bSignalled = m_ThreadSignal.Check();
	DbgLog((LOG_TIMING, 1, TEXT("Signal sanity check %d"),bSignalled));

	// Now output the current renderer state variables

	DbgLog((LOG_TIMING, 1, TEXT("Filter state %d"),m_State));

	DbgLog((LOG_TIMING, 1, TEXT("Abort flag %d"),m_bAbort));

	DbgLog((LOG_TIMING, 1, TEXT("Streaming flag %d"),m_bStreaming));

	DbgLog((LOG_TIMING, 1, TEXT("Clock advise link %d"),m_dwAdvise));

	DbgLog((LOG_TIMING, 1, TEXT("Current media sample %x"),m_pMediaSample));

	DbgLog((LOG_TIMING, 1, TEXT("EOS signalled %d"),m_bEOS));

	DbgLog((LOG_TIMING, 1, TEXT("EOS delivered %d"),m_bEOSDelivered));

	DbgLog((LOG_TIMING, 1, TEXT("Repaint status %d"),m_bRepaintStatus));


	// Output the delayed end of stream timer information

	DbgLog((LOG_TIMING, 1, TEXT("End of stream timer %x"),m_EndOfStreamTimer));

	DbgLog((LOG_TIMING, 1, TEXT("Deliver time %s"),CDisp((LONGLONG)m_SignalTime)));


	// Should never timeout during a flushing state

	BOOL bFlushing = m_pInputPin->IsFlushing();
	DbgLog((LOG_TIMING, 1, TEXT("Flushing sanity check %d"),bFlushing));

	// Display the time we were told to start at
	DbgLog((LOG_TIMING, 1, TEXT("Last run time %s"),CDisp((LONGLONG)m_tStart.m_time)));

	// Have we got a reference clock
	if (m_pClock == NULL) return;

	// Get the current time from the wall clock

	D_CRefTime CurrentTime,StartTime,EndTime;
	m_pClock->GetTime((D_REFERENCE_TIME*) &CurrentTime);
	D_CRefTime Offset = CurrentTime - m_tStart;

	// Display the current time from the clock

	DbgLog((LOG_TIMING, 1, TEXT("Clock time %s"),CDisp((LONGLONG)CurrentTime.m_time)));

	DbgLog((LOG_TIMING, 1, TEXT("Time difference %dms"),Offset.Millisecs()));


	// Do we have a sample ready to render
	if (m_pMediaSample == NULL) return;

	m_pMediaSample->GetTime((D_REFERENCE_TIME*)&StartTime, (D_REFERENCE_TIME*)&EndTime);
	DbgLog((LOG_TIMING, 1, TEXT("Next sample stream times (Start %d End %d ms)"),
			StartTime.Millisecs(),EndTime.Millisecs()));

	// Calculate how long it is until it is due for rendering
	D_CRefTime Wait = (m_tStart + StartTime) - CurrentTime;
	DbgLog((LOG_TIMING, 1, TEXT("Wait required %d ms"),Wait.Millisecs()));
}
#endif


// Wait until the clock sets the timer event or we're otherwise signalled. We
// set an arbitrary timeout for this wait and if it fires then we display the
// current renderer state on the debugger. It will often fire if the filter's
// left paused in an application however it may also fire during stress tests
// if the synchronisation with application seeks and state changes is faulty

#define RENDER_TIMEOUT 10000

HRESULT D_CBaseRenderer::WaitForRenderTime()
{
	HANDLE WaitObjects[] = { m_ThreadSignal, m_RenderEvent };
	DWORD Result = WAIT_TIMEOUT;

	SETUP_WIN_API

	// Wait for either the time to arrive or for us to be stopped

	OnWaitStart();
	while (Result == WAIT_TIMEOUT) {
		Result = WinAPIData.Win32Func.WaitForMultipleObjectsFunc(2,WaitObjects,FALSE,RENDER_TIMEOUT);

#ifdef DEBUG
		if (Result == WAIT_TIMEOUT) DisplayRendererState();
#endif

	}
	OnWaitEnd();

	// We may have been awoken without the timer firing

	if (Result == WAIT_OBJECT_0) {
		return VFW_E_STATE_CHANGED;
	}

	SignalTimerFired();
	return NOERROR;
}


// Poll waiting for Receive to complete.  This really matters when
// Receive may set the palette and cause window messages
// The problem is that if we don't really wait for a renderer to
// stop processing we can deadlock waiting for a transform which
// is calling the renderer's Receive() method because the transform's
// Stop method doesn't know to process window messages to unblock
// the renderer's Receive processing
void D_CBaseRenderer::WaitForReceiveToComplete()
{
	SETUP_WIN_API

	for (;;) {
		if (!m_bInReceive) {
			break;
		}

		MSG msg;
		//  Receive all interthread sendmessages
		PeekMessageWFunc(&msg, NULL, WM_NULL, WM_NULL, PM_NOREMOVE);

		WinAPIData.Win32Func.SleepFunc(1);
	}

	// If the wakebit for QS_POSTMESSAGE is set, the PeekMessage call
	// above just cleared the changebit which will cause some messaging
	// calls to block (waitMessage, MsgWaitFor...) now.
	// Post a dummy message to set the QS_POSTMESSAGE bit again
	if (HIWORD(WinAPIData.Win32Func.GetQueueStatusFunc(QS_POSTMESSAGE)) & QS_POSTMESSAGE) {
		//  Send dummy message
		WinAPIData.Win32Func.PostThreadMessageAFunc(WinAPIData.Win32Func.GetCurrentThreadIdFunc(), WM_NULL, 0, 0);
	}
}

// A filter can have four discrete states, namely Stopped, Running, Paused,
// Intermediate. We are in an intermediate state if we are currently trying
// to pause but haven't yet got the first sample (or if we have been flushed
// in paused state and therefore still have to wait for a sample to arrive)

// This class contains an event called m_evComplete which is signalled when
// the current state is completed and is not signalled when we are waiting to
// complete the last state transition. As mentioned above the only time we
// use this at the moment is when we wait for a media sample in paused state
// If while we are waiting we receive an end of stream notification from the
// source filter then we know no data is imminent so we can reset the event
// This means that when we transition to paused the source filter must call
// end of stream on us or send us an image otherwise we'll hang indefinately


// Simple internal way of getting the real state

D_FILTER_STATE D_CBaseRenderer::GetRealState() {
	return m_State;
}


// The renderer doesn't complete the full transition to paused states until
// it has got one media sample to render. If you ask it for its state while
// it's waiting it will return the state along with VFW_S_STATE_INTERMEDIATE

STDMETHODIMP D_CBaseRenderer::GetState(DWORD dwMSecs,D_FILTER_STATE *State)
{
	CheckPointer(State,E_POINTER);

	if (WaitDispatchingMessages(m_evComplete, dwMSecs) == WAIT_TIMEOUT) {
		*State = m_State;
		return VFW_S_STATE_INTERMEDIATE;
	}
	*State = m_State;
	return NOERROR;
}


// If we're pausing and we have no samples we don't complete the transition
// to D_State_Paused and we return S_FALSE. However if the m_bAbort flag has
// been set then all samples are rejected so there is no point waiting for
// one. If we do have a sample then return NOERROR. We will only ever return
// VFW_S_STATE_INTERMEDIATE from GetState after being paused with no sample
// (calling GetState after either being stopped or Run will NOT return this)

HRESULT D_CBaseRenderer::CompleteStateChange(D_FILTER_STATE OldState)
{
	// Allow us to be paused when disconnected

	if (m_pInputPin->IsConnected() == FALSE) {
		Ready();
		return S_OK;
	}

	// Have we run off the end of stream

	if (IsEndOfStream() == TRUE) {
		Ready();
		return S_OK;
	}

	// Make sure we get fresh data after being stopped

	if (HaveCurrentSample() == TRUE) {
		if (OldState != D_State_Stopped) {
			Ready();
			return S_OK;
		}
	}
	NotReady();
	return S_FALSE;
}


// When we stop the filter the things we do are:-

//	  Decommit the allocator being used in the connection
//	  Release the source filter if it's waiting in Receive
//	  Cancel any advise link we set up with the clock
//	  Any end of stream signalled is now obsolete so reset
//	  Allow us to be stopped when we are not connected

STDMETHODIMP D_CBaseRenderer::Stop()
{
	D_CAutoLock cRendererLock(&m_InterfaceLock);

	SETUP_WIN_API

	// Make sure there really is a state change

	if (m_State == D_State_Stopped) {
		return NOERROR;
	}

	// Is our input pin connected

	if (m_pInputPin->IsConnected() == FALSE) {
		NOTE("Input pin is not connected");
		m_State = D_State_Stopped;
		return NOERROR;
	}

	D_CBaseFilter::Stop();

	// If we are going into a stopped state then we must decommit whatever
	// allocator we are using it so that any source filter waiting in the
	// GetBuffer can be released and unlock themselves for a state change

	if (m_pInputPin->Allocator()) {
		m_pInputPin->Allocator()->Decommit();
	}

	// Cancel any scheduled rendering

	SetRepaintStatus(TRUE);
	StopStreaming();
	SourceThreadCanWait(FALSE);
	ResetEndOfStream();
	CancelNotification();

	// There should be no outstanding clock advise
	ASSERT(CancelNotification() == S_FALSE);
	ASSERT(WAIT_TIMEOUT == WinAPIData.Win32Func.WaitForSingleObjectFunc((HANDLE)m_RenderEvent,0));
	ASSERT(m_EndOfStreamTimer == 0);

	Ready();
	WaitForReceiveToComplete();
	m_bAbort = FALSE;

	return NOERROR;
}


// When we pause the filter the things we do are:-

//	  Commit the allocator being used in the connection
//	  Allow a source filter thread to wait in Receive
//	  Cancel any clock advise link (we may be running)
//	  Possibly complete the state change if we have data
//	  Allow us to be paused when we are not connected

STDMETHODIMP D_CBaseRenderer::Pause()
{
	D_CAutoLock cRendererLock(&m_InterfaceLock);
	D_FILTER_STATE OldState = m_State;
	ASSERT(m_pInputPin->IsFlushing() == FALSE);

	SETUP_WIN_API

	// Make sure there really is a state change

	if (m_State == D_State_Paused) {
		return CompleteStateChange(D_State_Paused);
	}

	// Has our input pin been connected

	if (m_pInputPin->IsConnected() == FALSE) {
		NOTE("Input pin is not connected");
		m_State = D_State_Paused;
		return CompleteStateChange(D_State_Paused);
	}

	// Pause the base filter class

	HRESULT hr = D_CBaseFilter::Pause();
	if (FAILED(hr)) {
		NOTE("Pause failed");
		return hr;
	}

	// Enable EC_REPAINT events again

	SetRepaintStatus(TRUE);
	StopStreaming();
	SourceThreadCanWait(TRUE);
	CancelNotification();
	ResetEndOfStreamTimer();

	// If we are going into a paused state then we must commit whatever
	// allocator we are using it so that any source filter can call the
	// GetBuffer and expect to get a buffer without returning an error

	if (m_pInputPin->Allocator()) {
		m_pInputPin->Allocator()->Commit();
	}

	// There should be no outstanding advise
	ASSERT(CancelNotification() == S_FALSE);
	ASSERT(WAIT_TIMEOUT == WinAPIData.Win32Func.WaitForSingleObjectFunc((HANDLE)m_RenderEvent,0));
	ASSERT(m_EndOfStreamTimer == 0);
	ASSERT(m_pInputPin->IsFlushing() == FALSE);

	// When we come out of a stopped state we must clear any image we were
	// holding onto for frame refreshing. Since renderers see state changes
	// first we can reset ourselves ready to accept the source thread data
	// Paused or running after being stopped causes the current position to
	// be reset so we're not interested in passing end of stream signals

	if (OldState == D_State_Stopped) {
		m_bAbort = FALSE;
		ClearPendingSample();
	}
	return CompleteStateChange(OldState);
}


// When we run the filter the things we do are:-

//	  Commit the allocator being used in the connection
//	  Allow a source filter thread to wait in Receive
//	  Signal the render event just to get us going
//	  Start the base class by calling StartStreaming
//	  Allow us to be run when we are not connected
//	  Signal EC_COMPLETE if we are not connected

STDMETHODIMP D_CBaseRenderer::Run(D_REFERENCE_TIME StartTime)
{
	D_CAutoLock cRendererLock(&m_InterfaceLock);
	D_FILTER_STATE OldState = m_State;

	SETUP_WIN_API

	// Make sure there really is a state change

	if (m_State == D_State_Running) {
		return NOERROR;
	}

	// Send EC_COMPLETE if we're not connected

	if (m_pInputPin->IsConnected() == FALSE) {
		NotifyEvent(EC_COMPLETE,S_OK,(LONG_PTR)(D_IBaseFilter *)this);
		m_State = D_State_Running;
		return NOERROR;
	}

	Ready();

	// Pause the base filter class

	HRESULT hr = D_CBaseFilter::Run(StartTime);
	if (FAILED(hr)) {
		NOTE("Run failed");
		return hr;
	}

	// Allow the source thread to wait
	ASSERT(m_pInputPin->IsFlushing() == FALSE);
	SourceThreadCanWait(TRUE);
	SetRepaintStatus(FALSE);

	// There should be no outstanding advise
	ASSERT(CancelNotification() == S_FALSE);
	ASSERT(WAIT_TIMEOUT == WinAPIData.Win32Func.WaitForSingleObjectFunc((HANDLE)m_RenderEvent,0));
	ASSERT(m_EndOfStreamTimer == 0);
	ASSERT(m_pInputPin->IsFlushing() == FALSE);

	// If we are going into a running state then we must commit whatever
	// allocator we are using it so that any source filter can call the
	// GetBuffer and expect to get a buffer without returning an error

	if (m_pInputPin->Allocator()) {
		m_pInputPin->Allocator()->Commit();
	}

	// When we come out of a stopped state we must clear any image we were
	// holding onto for frame refreshing. Since renderers see state changes
	// first we can reset ourselves ready to accept the source thread data
	// Paused or running after being stopped causes the current position to
	// be reset so we're not interested in passing end of stream signals

	if (OldState == D_State_Stopped) {
		m_bAbort = FALSE;
		ClearPendingSample();
	}
	return StartStreaming();
}


// Return the number of input pins we support

int D_CBaseRenderer::GetPinCount()
{
	return 1;
}


// We only support one input pin and it is numbered zero

D_CBasePin *D_CBaseRenderer::GetPin(int n)
{
	D_CAutoLock cObjectCreationLock(&m_ObjectCreationLock);

	// Should only ever be called with zero
	ASSERT(n == 0);

	if (n != 0) {
		return NULL;
	}

	// Create the input pin if not already done so

	if (m_pInputPin == NULL) {

		// hr must be initialized to NOERROR because
		// D_CRendererInputPin's constructor only changes
		// hr's value if an error occurs.
		HRESULT hr = NOERROR;

		m_pInputPin = new D_CRendererInputPin(this,&hr,L"In");
		if (NULL == m_pInputPin) {
			return NULL;
		}

		if (FAILED(hr)) {
			delete m_pInputPin;
			m_pInputPin = NULL;
			return NULL;
		}
	}
	return m_pInputPin;
}


// If "In" then return the D_IPin for our input pin, otherwise NULL and error

STDMETHODIMP D_CBaseRenderer::FindPin(LPCWSTR Id, D_IPin **ppPin)
{
	CheckPointer(ppPin,E_POINTER);

	SETUP_WIN_API

	if (0==lstrcmpWFunc((WORD const *)Id,(WORD const *)L"In")) {
		*ppPin = GetPin(0);
		ASSERT(*ppPin);
		(*ppPin)->AddRef();
	} else {
		*ppPin = NULL;
		return VFW_E_NOT_FOUND;
	}
	return NOERROR;
}


// Called when the input pin receives an EndOfStream notification. If we have
// not got a sample, then notify EC_COMPLETE now. If we have samples, then set
// m_bEOS and check for this on completing samples. If we're waiting to pause
// then complete the transition to paused state by setting the state event

HRESULT D_CBaseRenderer::EndOfStream()
{
	// Ignore these calls if we are stopped

	if (m_State == D_State_Stopped) {
		return NOERROR;
	}

	// If we have a sample then wait for it to be rendered

	m_bEOS = TRUE;
	if (m_pMediaSample) {
		return NOERROR;
	}

	// If we are waiting for pause then we are now ready since we cannot now
	// carry on waiting for a sample to arrive since we are being told there
	// won't be any. This sets an event that the GetState function picks up

	Ready();

	// Only signal completion now if we are running otherwise queue it until
	// we do run in StartStreaming. This is used when we seek because a seek
	// causes a pause where early notification of completion is misleading

	if (m_bStreaming) {
		SendEndOfStream();
	}
	return NOERROR;
}


// When we are told to flush we should release the source thread

HRESULT D_CBaseRenderer::BeginFlush()
{
	// If paused then report state intermediate until we get some data

	if (m_State == D_State_Paused) {
		NotReady();
	}

	SourceThreadCanWait(FALSE);
	CancelNotification();
	ClearPendingSample();
	//  Wait for Receive to complete
	WaitForReceiveToComplete();

	return NOERROR;
}


// After flushing the source thread can wait in Receive again

HRESULT D_CBaseRenderer::EndFlush()
{
	// Reset the current sample media time
	if (m_pPosition) m_pPosition->ResetMediaTime();

	// There should be no outstanding advise

	ASSERT(CancelNotification() == S_FALSE);
	SourceThreadCanWait(TRUE);
	return NOERROR;
}


// We can now send EC_REPAINTs if so required

HRESULT D_CBaseRenderer::CompleteConnect(D_IPin * /*pReceivePin*/ )
{
	// The caller should always hold the interface lock because
	// the function uses D_CBaseFilter::m_State.
	ASSERT(CritCheckIn(&m_InterfaceLock));

	m_bAbort = FALSE;

	if (D_State_Running == GetRealState()) {
		HRESULT hr = StartStreaming();
		if (FAILED(hr)) {
			return hr;
		}

		SetRepaintStatus(FALSE);
	} else {
		SetRepaintStatus(TRUE);
	}

	return NOERROR;
}


// Called when we go paused or running

HRESULT D_CBaseRenderer::Active()
{
	return NOERROR;
}


// Called when we go into a stopped state

HRESULT D_CBaseRenderer::Inactive()
{
	if (m_pPosition) {
		m_pPosition->ResetMediaTime();
	}
	//  People who derive from this may want to override this behaviour
	//  to keep hold of the sample in some circumstances
	ClearPendingSample();

	return NOERROR;
}


// Tell derived classes about the media type agreed

HRESULT D_CBaseRenderer::SetMediaType(const D_CMediaType * /*pmt*/ )
{
	return NOERROR;
}


// When we break the input pin connection we should reset the EOS flags. When
// we are asked for either D_IMediaPosition or D_IMediaSeeking we will create a
// D_CPosPassThru object to handles media time pass through. When we're handed
// samples we store (by calling D_CPosPassThru::RegisterMediaTime) their media
// times so we can then return a real current position of data being rendered

HRESULT D_CBaseRenderer::BreakConnect()
{
	// Do we have a quality management sink

	if (m_pQSink) {
		m_pQSink->Release();
		m_pQSink = NULL;
	}

	// Check we have a valid connection

	if (m_pInputPin->IsConnected() == FALSE) {
		return S_FALSE;
	}

	// Check we are stopped before disconnecting
	if (m_State != D_State_Stopped && !m_pInputPin->CanReconnectWhenActive()) {
		return VFW_E_NOT_STOPPED;
	}

	SetRepaintStatus(FALSE);
	ResetEndOfStream();
	ClearPendingSample();
	m_bAbort = FALSE;

	if (D_State_Running == m_State) {
		StopStreaming();
	}

	return NOERROR;
}


// Retrieves the sample times for this samples (note the sample times are
// passed in by reference not value). We return S_FALSE to say schedule this
// sample according to the times on the sample. We also return S_OK in
// which case the object should simply render the sample data immediately

HRESULT D_CBaseRenderer::GetSampleTimes(D_IMediaSample *pMediaSample,
									  D_REFERENCE_TIME *pStartTime,
									  D_REFERENCE_TIME *pEndTime)
{
	ASSERT(m_dwAdvise == 0);
	ASSERT(pMediaSample);

	// If the stop time for this sample is before or the same as start time,
	// then just ignore it (release it) and schedule the next one in line
	// Source filters should always fill in the start and end times properly!

	if (SUCCEEDED(pMediaSample->GetTime(pStartTime, pEndTime))) {
		if (*pEndTime < *pStartTime) {
			return VFW_E_START_TIME_AFTER_END;
		}
	} else {
		// no time set in the sample... draw it now?
		return S_OK;
	}

	// Can't synchronise without a clock so we return S_OK which tells the
	// caller that the sample should be rendered immediately without going
	// through the overhead of setting a timer advise link with the clock

	if (m_pClock == NULL) {
		return S_OK;
	}
	return ShouldDrawSampleNow(pMediaSample,pStartTime,pEndTime);
}


// By default all samples are drawn according to their time stamps so we
// return S_FALSE. Returning S_OK means draw immediately, this is used
// by the derived video renderer class in its quality management.

HRESULT D_CBaseRenderer::ShouldDrawSampleNow(D_IMediaSample * /*pMediaSample*/,
											D_REFERENCE_TIME * /*ptrStart*/,
											D_REFERENCE_TIME * /*ptrEnd*/ )
{
	return S_FALSE;
}


// We must always reset the current advise time to zero after a timer fires
// because there are several possible ways which lead us not to do any more
// scheduling such as the pending image being cleared after state changes

void D_CBaseRenderer::SignalTimerFired()
{
	m_dwAdvise = 0;
}


// Cancel any notification currently scheduled. This is called by the owning
// window object when it is told to stop streaming. If there is no timer link
// outstanding then calling this is benign otherwise we go ahead and cancel
// We must always reset the render event as the quality management code can
// signal immediate rendering by setting the event without setting an advise
// link. If we're subsequently stopped and run the first attempt to setup an
// advise link with the reference clock will find the event still signalled

HRESULT D_CBaseRenderer::CancelNotification()
{
	ASSERT(m_dwAdvise == 0 || m_pClock);
	DWORD_PTR dwAdvise = m_dwAdvise;

	// Have we a live advise link

	if (m_dwAdvise) {
		m_pClock->Unadvise(m_dwAdvise);
		SignalTimerFired();
		ASSERT(m_dwAdvise == 0);
	}

	// Clear the event and return our status

	m_RenderEvent.Reset();
	return (dwAdvise ? S_OK : S_FALSE);
}


// Responsible for setting up one shot advise links with the clock
// Return FALSE if the sample is to be dropped (not drawn at all)
// Return TRUE if the sample is to be drawn and in this case also
// arrange for m_RenderEvent to be set at the appropriate time

BOOL D_CBaseRenderer::ScheduleSample(D_IMediaSample *pMediaSample)
{
	D_REFERENCE_TIME StartSample, EndSample;

	SETUP_WIN_API

	// Is someone pulling our leg

	if (pMediaSample == NULL) {
		return FALSE;
	}

	// Get the next sample due up for rendering.  If there aren't any ready
	// then GetNextSampleTimes returns an error.  If there is one to be done
	// then it succeeds and yields the sample times. If it is due now then
	// it returns S_OK other if it's to be done when due it returns S_FALSE

	HRESULT hr = GetSampleTimes(pMediaSample, &StartSample, &EndSample);
	if (FAILED(hr)) {
		return FALSE;
	}

	// If we don't have a reference clock then we cannot set up the advise
	// time so we simply set the event indicating an image to render. This
	// will cause us to run flat out without any timing or synchronisation

	if (hr == S_OK) {
		EXECUTE_ASSERT(WinAPIData.Win32Func.SetEventFunc((HANDLE) m_RenderEvent));
		return TRUE;
	}

	ASSERT(m_dwAdvise == 0);
	ASSERT(m_pClock);
	ASSERT(WAIT_TIMEOUT == WinAPIData.Win32Func.WaitForSingleObjectFunc((HANDLE)m_RenderEvent,0));

	// We do have a valid reference clock interface so we can ask it to
	// set an event when the image comes due for rendering. We pass in
	// the reference time we were told to start at and also the current
	// stream time which is the offset from the start reference time

	hr = m_pClock->AdviseTime(
			(D_REFERENCE_TIME) m_tStart,		  // Start run time
			StartSample,						// Stream time
			(D_HEVENT)(HANDLE) m_RenderEvent,	 // Render notification
			&m_dwAdvise);						// Advise cookie

	if (SUCCEEDED(hr)) {
		return TRUE;
	}

	// We could not schedule the next sample for rendering despite the fact
	// we have a valid sample here. This is a fair indication that either
	// the system clock is wrong or the time stamp for the sample is duff

	ASSERT(m_dwAdvise == 0);
	return FALSE;
}


// This is called when a sample comes due for rendering. We pass the sample
// on to the derived class. After rendering we will initialise the timer for
// the next sample, NOTE signal that the last one fired first, if we don't
// do this it thinks there is still one outstanding that hasn't completed

HRESULT D_CBaseRenderer::Render(D_IMediaSample *pMediaSample)
{
	// If the media sample is NULL then we will have been notified by the
	// clock that another sample is ready but in the mean time someone has
	// stopped us streaming which causes the next sample to be released

	if (pMediaSample == NULL) {
		return S_FALSE;
	}

	// If we have stopped streaming then don't render any more samples, the
	// thread that got in and locked us and then reset this flag does not
	// clear the pending sample as we can use it to refresh any output device

	if (m_bStreaming == FALSE) {
		return S_FALSE;
	}

	// Time how long the rendering takes

	OnRenderStart(pMediaSample);
	DoRenderSample(pMediaSample);
	OnRenderEnd(pMediaSample);

	return NOERROR;
}


// Checks if there is a sample waiting at the renderer

BOOL D_CBaseRenderer::HaveCurrentSample()
{
	D_CAutoLock cRendererLock(&m_RendererLock);
	return (m_pMediaSample == NULL ? FALSE : TRUE);
}


// Returns the current sample waiting at the video renderer. We AddRef the
// sample before returning so that should it come due for rendering the
// person who called this method will hold the remaining reference count
// that will stop the sample being added back onto the allocator free list

D_IMediaSample *D_CBaseRenderer::GetCurrentSample()
{
	D_CAutoLock cRendererLock(&m_RendererLock);
	if (m_pMediaSample) {
		m_pMediaSample->AddRef();
	}
	return m_pMediaSample;
}


// Called when the source delivers us a sample. We go through a few checks to
// make sure the sample can be rendered. If we are running (streaming) then we
// have the sample scheduled with the reference clock, if we are not streaming
// then we have received an sample in paused mode so we can complete any state
// transition. On leaving this function everything will be unlocked so an app
// thread may get in and change our state to stopped (for example) in which
// case it will also signal the thread event so that our wait call is stopped

HRESULT D_CBaseRenderer::PrepareReceive(D_IMediaSample *pMediaSample)
{
	D_CAutoLock cInterfaceLock(&m_InterfaceLock);
	m_bInReceive = TRUE;

	SETUP_WIN_API

	// Check our flushing and filter state

	// This function must hold the interface lock because it calls 
	// D_CBaseInputPin::Receive() and D_CBaseInputPin::Receive() uses
	// D_CBasePin::m_bRunTimeError.
	HRESULT hr = m_pInputPin->D_CBaseInputPin::Receive(pMediaSample);

	if (hr != NOERROR) {
		m_bInReceive = FALSE;
		return E_FAIL;
	}

	// Has the type changed on a media sample. We do all rendering
	// synchronously on the source thread, which has a side effect
	// that only one buffer is ever outstanding. Therefore when we
	// have Receive called we can go ahead and change the format
	// Since the format change can cause a SendMessage we just don't
	// lock
	if (m_pInputPin->SampleProps()->pMediaType) {
		hr = m_pInputPin->SetMediaType(
				(D_CMediaType *)m_pInputPin->SampleProps()->pMediaType);
		if (FAILED(hr)) {
			m_bInReceive = FALSE;
			return hr;
		}
	}


	D_CAutoLock cSampleLock(&m_RendererLock);

	ASSERT(IsActive() == TRUE);
	ASSERT(m_pInputPin->IsFlushing() == FALSE);
	ASSERT(m_pInputPin->IsConnected() == TRUE);
	ASSERT(m_pMediaSample == NULL);

	// Return an error if we already have a sample waiting for rendering
	// source pins must serialise the Receive calls - we also check that
	// no data is being sent after the source signalled an end of stream

	if (m_pMediaSample || m_bEOS || m_bAbort) {
		Ready();
		m_bInReceive = FALSE;
		return E_UNEXPECTED;
	}

	// Store the media times from this sample
	if (m_pPosition) m_pPosition->RegisterMediaTime(pMediaSample);

	// Schedule the next sample if we are streaming

	if ((m_bStreaming == TRUE) && (ScheduleSample(pMediaSample) == FALSE)) {
		ASSERT(WAIT_TIMEOUT == WinAPIData.Win32Func.WaitForSingleObjectFunc((HANDLE)m_RenderEvent,0));
		ASSERT(CancelNotification() == S_FALSE);
		m_bInReceive = FALSE;
		return VFW_E_SAMPLE_REJECTED;
	}

	// Store the sample end time for EC_COMPLETE handling
	m_SignalTime = m_pInputPin->SampleProps()->tStop;

	// BEWARE we sometimes keep the sample even after returning the thread to
	// the source filter such as when we go into a stopped state (we keep it
	// to refresh the device with) so we must AddRef it to keep it safely. If
	// we start flushing the source thread is released and any sample waiting
	// will be released otherwise GetBuffer may never return (see BeginFlush)

	m_pMediaSample = pMediaSample;
	m_pMediaSample->AddRef();

	if (m_bStreaming == FALSE) {
		SetRepaintStatus(TRUE);
	}
	return NOERROR;
}


// Called by the source filter when we have a sample to render. Under normal
// circumstances we set an advise link with the clock, wait for the time to
// arrive and then render the data using the PURE virtual DoRenderSample that
// the derived class will have overriden. After rendering the sample we may
// also signal EOS if it was the last one sent before EndOfStream was called

HRESULT D_CBaseRenderer::Receive(D_IMediaSample *pSample)
{
	ASSERT(pSample);

	// It may return VFW_E_SAMPLE_REJECTED code to say don't bother

	HRESULT hr = PrepareReceive(pSample);
	ASSERT(m_bInReceive == SUCCEEDED(hr));
	if (FAILED(hr)) {
		if (hr == VFW_E_SAMPLE_REJECTED) {
			return NOERROR;
		}
		return hr;
	}

	// We realize the palette in "PrepareRender()" so we have to give away the
	// filter lock here.
	if (m_State == D_State_Paused) {
		PrepareRender();
		// no need to use InterlockedExchange
		m_bInReceive = FALSE;
		{
			// We must hold both these locks
			D_CAutoLock cRendererLock(&m_InterfaceLock);
			if (m_State == D_State_Stopped)
				return NOERROR;

			m_bInReceive = TRUE;
			D_CAutoLock cSampleLock(&m_RendererLock);
			OnReceiveFirstSample(pSample);
		}
		Ready();
	}
	// Having set an advise link with the clock we sit and wait. We may be
	// awoken by the clock firing or by a state change. The rendering call
	// will lock the critical section and check we can still render the data

	hr = WaitForRenderTime();
	if (FAILED(hr)) {
		m_bInReceive = FALSE;
		return NOERROR;
	}

	PrepareRender();

	//  Set this here and poll it until we work out the locking correctly
	//  It can't be right that the streaming stuff grabs the interface
	//  lock - after all we want to be able to wait for this stuff
	//  to complete
	m_bInReceive = FALSE;

	// We must hold both these locks
	D_CAutoLock cRendererLock(&m_InterfaceLock);

	// since we gave away the filter wide lock, the sate of the filter could
	// have chnaged to Stopped
	if (m_State == D_State_Stopped)
		return NOERROR;

	D_CAutoLock cSampleLock(&m_RendererLock);

	// Deal with this sample

	Render(m_pMediaSample);
	ClearPendingSample();
	SendEndOfStream();
	CancelNotification();
	return NOERROR;
}


// This is called when we stop or are inactivated to clear the pending sample
// We release the media sample interface so that they can be allocated to the
// source filter again, unless of course we are changing state to inactive in
// which case GetBuffer will return an error. We must also reset the current
// media sample to NULL so that we know we do not currently have an image

HRESULT D_CBaseRenderer::ClearPendingSample()
{
	D_CAutoLock cRendererLock(&m_RendererLock);
	if (m_pMediaSample) {
		m_pMediaSample->Release();
		m_pMediaSample = NULL;
	}
	return NOERROR;
}


// Used to signal end of stream according to the sample end time

#if _MSC_VER > 1200 || defined( DX_GCC_COMPILE_4_9_2 )
void CALLBACK EndOfStreamTimer(UINT /*uID*/,		// Timer identifier
								UINT /*uMsg*/,		// Not currently used
								DWORD_PTR dwUser,// User information
								DWORD_PTR /*dw1*/,	// Windows reserved
								DWORD_PTR /*dw2*/)	// is also reserved
#else
void CALLBACK EndOfStreamTimer(UINT /*uID*/,		// Timer identifier
								UINT /*uMsg*/,		// Not currently used
								DWORD dwUser,// User information
								DWORD /*dw1*/,	// Windows reserved
								DWORD /*dw2*/)	// is also reserved
#endif
{
	D_CBaseRenderer *pRenderer = (D_CBaseRenderer *) dwUser;
	NOTE1("EndOfStreamTimer called (%d)",uID);
	pRenderer->TimerCallback();
}

//  Do the timer callback work
void D_CBaseRenderer::TimerCallback()
{
	//  Lock for synchronization (but don't hold this lock when calling
	//  timeKillEvent)
	D_CAutoLock cRendererLock(&m_RendererLock);

	// See if we should signal end of stream now

	if (m_EndOfStreamTimer) {
		m_EndOfStreamTimer = 0;
		SendEndOfStream();
	}
}


// If we are at the end of the stream signal the filter graph but do not set
// the state flag back to FALSE. Once we drop off the end of the stream we
// leave the flag set (until a subsequent ResetEndOfStream). Each sample we
// get delivered will update m_SignalTime to be the last sample's end time.
// We must wait this long before signalling end of stream to the filtergraph

#define TIMEOUT_DELIVERYWAIT 50
#define TIMEOUT_RESOLUTION 10

HRESULT D_CBaseRenderer::SendEndOfStream()
{
	ASSERT(CritCheckIn(&m_RendererLock));
	if (m_bEOS == FALSE || m_bEOSDelivered || m_EndOfStreamTimer) {
		return NOERROR;
	}

	// If there is no clock then signal immediately
	if (m_pClock == NULL) {
		return NotifyEndOfStream();
	}

	// How long into the future is the delivery time

	D_REFERENCE_TIME Signal = m_tStart + m_SignalTime;
	D_REFERENCE_TIME CurrentTime;
	m_pClock->GetTime(&CurrentTime);
	LONG Delay = LONG((Signal - CurrentTime) / 10000);

	// Dump the timing information to the debugger

	NOTE1("Delay until end of stream delivery %d",Delay);
	NOTE1("Current %s",(LPCTSTR)CDisp((LONGLONG)CurrentTime));
	NOTE1("Signal %s",(LPCTSTR)CDisp((LONGLONG)Signal));

	// Wait for the delivery time to arrive

	if (Delay < TIMEOUT_DELIVERYWAIT) {
		return NotifyEndOfStream();
	}

	// Signal a timer callback on another worker thread

	m_EndOfStreamTimer = CompatibleTimeSetEvent((UINT) Delay, // Period of timer
									  TIMEOUT_RESOLUTION,	 // Timer resolution
									  EndOfStreamTimer,		// Callback function
									  (DWORD_PTR)this,		// Used information
									  TIME_ONESHOT);		  // Type of callback
	if (m_EndOfStreamTimer == 0) {
		return NotifyEndOfStream();
	}
	return NOERROR;
}


// Signals EC_COMPLETE to the filtergraph manager

HRESULT D_CBaseRenderer::NotifyEndOfStream()
{
	D_CAutoLock cRendererLock(&m_RendererLock);
	ASSERT(m_bEOSDelivered == FALSE);
	ASSERT(m_EndOfStreamTimer == 0);

	// Has the filter changed state

	if (m_bStreaming == FALSE) {
		ASSERT(m_EndOfStreamTimer == 0);
		return NOERROR;
	}

	// Reset the end of stream timer
	m_EndOfStreamTimer = 0;

	// If we've been using the D_IMediaPosition interface, set it's start
	// and end media "times" to the stop position by hand.  This ensures
	// that we actually get to the end, even if the MPEG guestimate has
	// been bad or if the quality management dropped the last few frames

	if (m_pPosition) m_pPosition->EOS();
	m_bEOSDelivered = TRUE;
	NOTE("Sending EC_COMPLETE...");
	return NotifyEvent(EC_COMPLETE,S_OK,(LONG_PTR)(D_IBaseFilter *)this);
}


// Reset the end of stream flag, this is typically called when we transfer to
// stopped states since that resets the current position back to the start so
// we will receive more samples or another EndOfStream if there aren't any. We
// keep two separate flags one to say we have run off the end of the stream
// (this is the m_bEOS flag) and another to say we have delivered EC_COMPLETE
// to the filter graph. We need the latter otherwise we can end up sending an
// EC_COMPLETE every time the source changes state and calls our EndOfStream

HRESULT D_CBaseRenderer::ResetEndOfStream()
{
	ResetEndOfStreamTimer();
	D_CAutoLock cRendererLock(&m_RendererLock);

	m_bEOS = FALSE;
	m_bEOSDelivered = FALSE;
	m_SignalTime = 0;

	return NOERROR;
}


// Kills any outstanding end of stream timer

void D_CBaseRenderer::ResetEndOfStreamTimer()
{
	ASSERT(CritCheckOut(&m_RendererLock));

	SETUP_WIN_API

	if (m_EndOfStreamTimer) {
		WinAPIData.Win32Func.timeKillEventFunc(m_EndOfStreamTimer);
		m_EndOfStreamTimer = 0;
	}
}


// This is called when we start running so that we can schedule any pending
// image we have with the clock and display any timing information. If we
// don't have any sample but we have queued an EOS flag then we send it. If
// we do have a sample then we wait until that has been rendered before we
// signal the filter graph otherwise we may change state before it's done

HRESULT D_CBaseRenderer::StartStreaming()
{
	D_CAutoLock cRendererLock(&m_RendererLock);
	if (m_bStreaming == TRUE) {
		return NOERROR;
	}

	SETUP_WIN_API

	// Reset the streaming times ready for running

	m_bStreaming = TRUE;

	WinAPIData.Win32Func.timeBeginPeriodFunc(1);
	OnStartStreaming();

	// There should be no outstanding advise
	ASSERT(WAIT_TIMEOUT == WinAPIData.Win32Func.WaitForSingleObjectFunc((HANDLE)m_RenderEvent,0));
	ASSERT(CancelNotification() == S_FALSE);

	// If we have an EOS and no data then deliver it now

	if (m_pMediaSample == NULL) {
		return SendEndOfStream();
	}

	// Have the data rendered

	ASSERT(m_pMediaSample);
	if (!ScheduleSample(m_pMediaSample))
		m_RenderEvent.Set();

	return NOERROR;
}


// This is called when we stop streaming so that we can set our internal flag
// indicating we are not now to schedule any more samples arriving. The state
// change methods in the filter implementation take care of cancelling any
// clock advise link we have set up and clearing any pending sample we have

HRESULT D_CBaseRenderer::StopStreaming()
{
	D_CAutoLock cRendererLock(&m_RendererLock);
	m_bEOSDelivered = FALSE;

	SETUP_WIN_API

	if (m_bStreaming == TRUE) {
		m_bStreaming = FALSE;
		OnStopStreaming();
		WinAPIData.Win32Func.timeEndPeriodFunc(1);
	}
	return NOERROR;
}


// We have a boolean flag that is reset when we have signalled EC_REPAINT to
// the filter graph. We set this when we receive an image so that should any
// conditions arise again we can send another one. By having a flag we ensure
// we don't flood the filter graph with redundant calls. We do not set the
// event when we receive an EndOfStream call since there is no point in us
// sending further EC_REPAINTs. In particular the AutoShowWindow method and
// the DirectDraw object use this method to control the window repainting

void D_CBaseRenderer::SetRepaintStatus(BOOL bRepaint)
{
	D_CAutoLock cSampleLock(&m_RendererLock);
	m_bRepaintStatus = bRepaint;
}


// Pass the window handle to the upstream filter

void D_CBaseRenderer::SendNotifyWindow(D_IPin *pPin,HWND hwnd)
{
	D_IMediaEventSink *pSink;

	// Does the pin support D_IMediaEventSink
	HRESULT hr = pPin->QueryInterface(IID_IMEDIAEVENTSINK,(void **)&pSink);
	if (SUCCEEDED(hr)) {
		pSink->Notify(EC_NOTIFY_WINDOW,(LONG_PTR)hwnd,0);
		pSink->Release();
	}
	NotifyEvent(EC_NOTIFY_WINDOW,(LONG_PTR)hwnd,0);
}


// Signal an EC_REPAINT to the filter graph. This can be used to have data
// sent to us. For example when a video window is first displayed it may
// not have an image to display, at which point it signals EC_REPAINT. The
// filtergraph will either pause the graph if stopped or if already paused
// it will call put_CurrentPosition of the current position. Setting the
// current position to itself has the stream flushed and the image resent

//#define RLOG(_x_) DbgLog((LOG_TRACE,1,TEXT(_x_)));

void D_CBaseRenderer::SendRepaint()
{
	D_CAutoLock cSampleLock(&m_RendererLock);
	ASSERT(m_pInputPin);

	// We should not send repaint notifications when...
	//	- An end of stream has been notified
	//	- Our input pin is being flushed
	//	- The input pin is not connected
	//	- We have aborted a video playback
	//	- There is a repaint already sent

	if (m_bAbort == FALSE) {
		if (m_pInputPin->IsConnected() == TRUE) {
			if (m_pInputPin->IsFlushing() == FALSE) {
				if (IsEndOfStream() == FALSE) {
					if (m_bRepaintStatus == TRUE) {
						D_IPin *pPin = (D_IPin *) m_pInputPin;
						NotifyEvent(EC_REPAINT,(LONG_PTR) pPin,0);
						SetRepaintStatus(FALSE);
						RLOG("Sending repaint");
					}
				}
			}
		}
	}
}


// When a video window detects a display change (WM_DISPLAYCHANGE message) it
// can send an EC_DISPLAY_CHANGED event code along with the renderer pin. The
// filtergraph will stop everyone and reconnect our input pin. As we're then
// reconnected we can accept the media type that matches the new display mode
// since we may no longer be able to draw the current image type efficiently

BOOL D_CBaseRenderer::OnDisplayChange()
{
	// Ignore if we are not connected yet

	D_CAutoLock cSampleLock(&m_RendererLock);
	if (m_pInputPin->IsConnected() == FALSE) {
		return FALSE;
	}

	RLOG("Notification of EC_DISPLAY_CHANGE");

	// Pass our input pin as parameter on the event

	D_IPin *pPin = (D_IPin *) m_pInputPin;
	m_pInputPin->AddRef();
	NotifyEvent(EC_DISPLAY_CHANGED,(LONG_PTR) pPin,0);
	SetAbortSignal(TRUE);
	ClearPendingSample();
	m_pInputPin->Release();

	return TRUE;
}


// Called just before we start drawing.
// Store the current time in m_trRenderStart to allow the rendering time to be
// logged.  Log the time stamp of the sample and how late it is (neg is early)

#ifdef PERF
void D_CBaseRenderer::OnRenderStart(D_IMediaSample * pMediaSample)
{
	D_REFERENCE_TIME trStart, trEnd;
	pMediaSample->GetTime(&trStart, &trEnd);

	MSR_INTEGER(m_idBaseStamp, (int)trStart);	 // dump low order 32 bits

	m_pClock->GetTime(&m_trRenderStart);
	MSR_INTEGER(0, (int)m_trRenderStart);
	D_REFERENCE_TIME trStream;
	trStream = m_trRenderStart-m_tStart;	 // convert reftime to stream time
	MSR_INTEGER(0,(int)trStream);

	const int trLate = (int)(trStream - trStart);
	MSR_INTEGER(m_idBaseAccuracy, trLate/10000);  // dump in mSec
} // OnRenderStart
#else
void D_CBaseRenderer::OnRenderStart(D_IMediaSample * )
{
}
#endif


// Called directly after drawing an image.
// calculate the time spent drawing and log it.

#ifdef PERF
void D_CBaseRenderer::OnRenderEnd(D_IMediaSample *pMediaSample)
{
	D_REFERENCE_TIME trNow;
	m_pClock->GetTime(&trNow);
	MSR_INTEGER(0,(int)trNow);
	int t = (int)((trNow - m_trRenderStart)/10000);	// convert UNITS->msec
	MSR_INTEGER(m_idBaseRenderTime, t);
} // OnRenderEnd
#else
void D_CBaseRenderer::OnRenderEnd(D_IMediaSample *)
{
} // OnRenderEnd
#endif




// Constructor must be passed the base renderer object

D_CRendererInputPin::D_CRendererInputPin(D_CBaseRenderer *pRenderer,
									 HRESULT *phr,
									 LPCWSTR pPinName) :
//	D_CBaseInputPin(TEXT("Renderer pin"),
	D_CBaseInputPin((CHAR *)"Renderer pin",
				  pRenderer,
				  &pRenderer->m_InterfaceLock,
				  (HRESULT *) phr,
				  pPinName)
{
	m_pRenderer = pRenderer;
	ASSERT(m_pRenderer);
}


// Signals end of data stream on the input pin

STDMETHODIMP D_CRendererInputPin::EndOfStream()
{
	D_CAutoLock cRendererLock(&m_pRenderer->m_InterfaceLock);
	D_CAutoLock cSampleLock(&m_pRenderer->m_RendererLock);

	// Make sure we're streaming ok

	HRESULT hr = CheckStreaming();
	if (hr != NOERROR) {
		return hr;
	}

	// Pass it onto the renderer

	hr = m_pRenderer->EndOfStream();
	if (SUCCEEDED(hr)) {
		hr = D_CBaseInputPin::EndOfStream();
	}
	return hr;
}


// Signals start of flushing on the input pin - we do the final reset end of
// stream with the renderer lock unlocked but with the interface lock locked
// We must do this because we call timeKillEvent, our timer callback method
// has to take the renderer lock to serialise our state. Therefore holding a
// renderer lock when calling timeKillEvent could cause a deadlock condition

STDMETHODIMP D_CRendererInputPin::BeginFlush()
{
	D_CAutoLock cRendererLock(&m_pRenderer->m_InterfaceLock);
	{
		D_CAutoLock cSampleLock(&m_pRenderer->m_RendererLock);
		D_CBaseInputPin::BeginFlush();
		m_pRenderer->BeginFlush();
	}
	return m_pRenderer->ResetEndOfStream();
}


// Signals end of flushing on the input pin

STDMETHODIMP D_CRendererInputPin::EndFlush()
{
	D_CAutoLock cRendererLock(&m_pRenderer->m_InterfaceLock);
	D_CAutoLock cSampleLock(&m_pRenderer->m_RendererLock);

	HRESULT hr = m_pRenderer->EndFlush();
	if (SUCCEEDED(hr)) {
		hr = D_CBaseInputPin::EndFlush();
	}
	return hr;
}


// Pass the sample straight through to the renderer object

STDMETHODIMP D_CRendererInputPin::Receive(D_IMediaSample *pSample)
{
	HRESULT hr = m_pRenderer->Receive(pSample);
	if (FAILED(hr)) {

		// A deadlock could occur if the caller holds the renderer lock and
		// attempts to acquire the interface lock.
		ASSERT(CritCheckOut(&m_pRenderer->m_RendererLock));

		{
			// The interface lock must be held when the filter is calling
			// IsStopped() or IsFlushing().  The interface lock must also
			// be held because the function uses m_bRunTimeError.
			D_CAutoLock cRendererLock(&m_pRenderer->m_InterfaceLock);

			// We do not report errors which occur while the filter is stopping,
			// flushing or if the m_bAbort flag is set .  Errors are expected to 
			// occur during these operations and the streaming thread correctly 
			// handles the errors.  
			if (!IsStopped() && !IsFlushing() && !m_pRenderer->m_bAbort && !m_bRunTimeError) {

				// EC_ERRORABORT's first parameter is the error which caused
				// the event and its' last parameter is 0.  See the Direct
				// Show SDK documentation for more information.
				m_pRenderer->NotifyEvent(EC_ERRORABORT,hr,0);

				{
					D_CAutoLock alRendererLock(&m_pRenderer->m_RendererLock);
					if (m_pRenderer->IsStreaming() && !m_pRenderer->IsEndOfStreamDelivered()) {
						m_pRenderer->NotifyEndOfStream();
					}
				}
	
				m_bRunTimeError = TRUE;
			}
		}
	}

	return hr;
}


// Called when the input pin is disconnected

HRESULT D_CRendererInputPin::BreakConnect()
{
	HRESULT hr = m_pRenderer->BreakConnect();
	if (FAILED(hr)) {
		return hr;
	}
	return D_CBaseInputPin::BreakConnect();
}


// Called when the input pin is connected

HRESULT D_CRendererInputPin::CompleteConnect(D_IPin *pReceivePin)
{
	HRESULT hr = m_pRenderer->CompleteConnect(pReceivePin);
	if (FAILED(hr)) {
		return hr;
	}
	return D_CBaseInputPin::CompleteConnect(pReceivePin);
}


// Give the pin id of our one and only pin

STDMETHODIMP D_CRendererInputPin::QueryId(LPWSTR *Id)
{
	CheckPointer(Id,E_POINTER);

	SETUP_WIN_API

	*Id = (LPWSTR)WinAPIData.Win32Func.CoTaskMemAllocFunc(8);
	if (*Id == NULL) {
		return E_OUTOFMEMORY;
	}
	lstrcpyWFunc((WORD *)*Id, (WORD const *)L"In");
	return NOERROR;
}


// Will the filter accept this media type

HRESULT D_CRendererInputPin::CheckMediaType(const D_CMediaType *pmt)
{
	return m_pRenderer->CheckMediaType(pmt);
}


// Called when we go paused or running

HRESULT D_CRendererInputPin::Active()
{
	return m_pRenderer->Active();
}


// Called when we go into a stopped state

HRESULT D_CRendererInputPin::Inactive()
{
	// The caller must hold the interface lock because 
	// this function uses m_bRunTimeError.
	ASSERT(CritCheckIn(&m_pRenderer->m_InterfaceLock));

	m_bRunTimeError = FALSE;

	return m_pRenderer->Inactive();
}


// Tell derived classes about the media type agreed

HRESULT D_CRendererInputPin::SetMediaType(const D_CMediaType *pmt)
{
	HRESULT hr = D_CBaseInputPin::SetMediaType(pmt);
	if (FAILED(hr)) {
		return hr;
	}
	return m_pRenderer->SetMediaType(pmt);
}


// We do not keep an event object to use when setting up a timer link with
// the clock but are given a pointer to one by the owning object through the
// SetNotificationObject method - this must be initialised before starting
// We can override the default quality management process to have it always
// draw late frames, this is currently done by having the following registry
// key (actually an INI key) called DrawLateFrames set to 1 (default is 0)

const TCHAR AMQUALITY[] = TEXT("ActiveMovie");
const TCHAR DRAWLATEFRAMES[] = TEXT("DrawLateFrames");

D_CBaseVideoRenderer::D_CBaseVideoRenderer(
	  REFCLSID RenderClass, // CLSID for this renderer
	  TCHAR *pName,		 // Debug ONLY description
	  D_IUnknown * pUnk,		// Aggregated owner object
	  HRESULT *phr) :		// General OLE return code

	D_CBaseRenderer(RenderClass,pName,pUnk,phr),
	m_cFramesDropped(0),
	m_cFramesDrawn(0),
	m_bSupplierHandlingQuality(FALSE)
{
	ResetStreamingTimes();

#ifdef PERF
	m_idTimeStamp		= MSR_REGISTER(TEXT("Frame time stamp"));
	m_idEarliness		= MSR_REGISTER(TEXT("Earliness fudge"));
	m_idTarget			= MSR_REGISTER(TEXT("Target (mSec)"));
	m_idSchLateTime		= MSR_REGISTER(TEXT("mSec late when scheduled"));
	m_idDecision		= MSR_REGISTER(TEXT("Scheduler decision code"));
	m_idQualityRate		= MSR_REGISTER(TEXT("D_Quality rate sent"));
	m_idQualityTime		= MSR_REGISTER(TEXT("D_Quality time sent"));
	m_idWaitReal		= MSR_REGISTER(TEXT("Render wait"));
	// m_idWait			= MSR_REGISTER(TEXT("wait time recorded (msec)"));
	m_idFrameAccuracy	= MSR_REGISTER(TEXT("Frame accuracy (msecs)"));
	m_bDrawLateFrames	= GetProfileInt(AMQUALITY, DRAWLATEFRAMES, FALSE);
	//m_idSendQuality	= MSR_REGISTER(TEXT("Processing D_Quality message"));

	m_idRenderAvg		= MSR_REGISTER(TEXT("Render draw time Avg"));
	m_idFrameAvg		= MSR_REGISTER(TEXT("FrameAvg"));
	m_idWaitAvg			= MSR_REGISTER(TEXT("WaitAvg"));
	m_idDuration		= MSR_REGISTER(TEXT("Duration"));
	m_idThrottle		= MSR_REGISTER(TEXT("Audio-video throttle wait"));
	// m_idDebug		= MSR_REGISTER(TEXT("Debug stuff"));
#endif // PERF
} // Constructor


// Destructor is just a placeholder

D_CBaseVideoRenderer::~D_CBaseVideoRenderer()
{
	ASSERT(m_dwAdvise == 0);
}


// The timing functions in this class are called by the window object and by
// the renderer's allocator.
// The windows object calls timing functions as it receives media sample
// images for drawing using GDI.
// The allocator calls timing functions when it starts passing DCI/DirectDraw
// surfaces which are not rendered in the same way; The decompressor writes
// directly to the surface with no separate rendering, so those code paths
// call direct into us.  Since we only ever hand out DCI/DirectDraw surfaces
// when we have allocated one and only one image we know there cannot be any
// conflict between the two.
//
// We use timeGetTime to return the timing counts we use (since it's relative
// performance we are interested in rather than absolute compared to a clock)
// The window object sets the accuracy of the system clock (normally 1ms) by
// calling timeBeginPeriod/timeEndPeriod when it changes streaming states


// Reset all times controlling streaming.
// Set them so that
// 1. Frames will not initially be dropped
// 2. The first frame will definitely be drawn (achieved by saying that there
//	has not ben a frame drawn for a long time).

HRESULT D_CBaseVideoRenderer::ResetStreamingTimes()
{
	SETUP_WIN_API

	m_trLastDraw = -1000;	 // set up as first frame since ages (1 sec) ago
	m_tStreamingStart = ( int )WinAPIData.Win32Func.timeGetTimeFunc();
	m_trRenderAvg = 0;
	m_trFrameAvg = -1;		// -1000 fps == "unset"
	m_trDuration = 0;		 // 0 - strange value
	m_trRenderLast = 0;
	m_trWaitAvg = 0;
	m_tRenderStart = 0;
	m_cFramesDrawn = 0;
	m_cFramesDropped = 0;
	m_iTotAcc = 0;
	m_iSumSqAcc = 0;
	m_iSumSqFrameTime = 0;
	m_trFrame = 0;		  // hygiene - not really needed
	m_trLate = 0;			// hygiene - not really needed
	m_iSumFrameTime = 0;
	m_nNormal = 0;
	m_trEarliness = 0;
	m_trTarget = -300000;  // 30mSec early
	m_trThrottle = 0;
	m_trRememberStampForPerf = 0;

#ifdef PERF
	m_trRememberFrameForPerf = 0;
#endif

	return NOERROR;
} // ResetStreamingTimes


// Reset all times controlling streaming. Note that we're now streaming. We
// don't need to set the rendering event to have the source filter released
// as it is done during the Run processing. When we are run we immediately
// release the source filter thread and draw any image waiting (that image
// may already have been drawn once as a poster frame while we were paused)

HRESULT D_CBaseVideoRenderer::OnStartStreaming()
{
	ResetStreamingTimes();
	return NOERROR;
} // OnStartStreaming


// Called at end of streaming.  Fixes times for property page report

HRESULT D_CBaseVideoRenderer::OnStopStreaming()
{
	SETUP_WIN_API

	m_tStreamingStart = ( int )( WinAPIData.Win32Func.timeGetTimeFunc()-m_tStreamingStart );
	return NOERROR;
} // OnStopStreaming


// Called when we start waiting for a rendering event.
// Used to update times spent waiting and not waiting.

void D_CBaseVideoRenderer::OnWaitStart()
{
	MSR_START(m_idWaitReal);
} // OnWaitStart


// Called when we are awoken from the wait in the window OR by our allocator
// when it is hanging around until the next sample is due for rendering on a
// DCI/DirectDraw surface. We add the wait time into our rolling average.
// We grab the interface lock so that we're serialised with the application
// thread going through the run code - which in due course ends up calling
// ResetStreaming times - possibly as we run through this section of code

void D_CBaseVideoRenderer::OnWaitEnd()
{
#ifdef PERF
	SETUP_WIN_API

	MSR_STOP(m_idWaitReal);
	// for a perf build we want to know just exactly how late we REALLY are.
	// even if this means that we have to look at the clock again.

	D_REFERENCE_TIME trRealStream;	 // the real time now expressed as stream time.
#if 0
	m_pClock->GetTime(&trRealStream); // Calling clock here causes W95 deadlock!
#else
	// We will be discarding overflows like mad here!
	// This is wrong really because WinAPIData.Win32Func.timeGetTimeFunc() can wrap but it's
	// only for PERF
	D_REFERENCE_TIME tr = WinAPIData.Win32Func.timeGetTimeFunc()*10000;
	trRealStream = tr + m_llTimeOffset;
#endif
	trRealStream -= m_tStart;	 // convert to stream time (this is a reftime)

	if (m_trRememberStampForPerf==0) {
		// This is probably the poster frame at the start, and it is not scheduled
		// in the usual way at all.  Just count it.  The rememberstamp gets set
		// in ShouldDrawSampleNow, so this does invalid frame recording until we
		// actually start playing.
		PreparePerformanceData(0, 0);
	} else {
		int trLate = (int)(trRealStream - m_trRememberStampForPerf);
		int trFrame = (int)(tr - m_trRememberFrameForPerf);
		PreparePerformanceData(trLate, trFrame);
	}
	m_trRememberFrameForPerf = tr;
#endif //PERF
} // OnWaitEnd


// Put data on one side that describes the lateness of the current frame.
// We don't yet know whether it will actually be drawn.  In direct draw mode,
// this decision is up to the filter upstream, and it could change its mind.
// The rules say that if it did draw it must call Receive().  One way or
// another we eventually get into either OnRenderStart or OnDirectRender and
// these both call RecordFrameLateness to update the statistics.

void D_CBaseVideoRenderer::PreparePerformanceData(int trLate, int trFrame)
{
	m_trLate = trLate;
	m_trFrame = trFrame;
} // PreparePerformanceData


// update the statistics:
// m_iTotAcc, m_iSumSqAcc, m_iSumSqFrameTime, m_iSumFrameTime, m_cFramesDrawn
// Note that because the properties page reports using these variables,
// 1. We need to be inside a critical section
// 2. They must all be updated together.  Updating the sums here and the count
// elsewhere can result in imaginary jitter (i.e. attempts to find square roots
// of negative numbers) in the property page code.

void D_CBaseVideoRenderer::RecordFrameLateness(int trLate, int trFrame)
{
	// Record how timely we are.
	int tLate = trLate/10000;

	// Best estimate of moment of appearing on the screen is average of
	// start and end draw times.  Here we have only the end time.  This may
	// tend to show us as spuriously late by up to 1/2 frame rate achieved.
	// Decoder probably monitors draw time.  We don't bother.
	MSR_INTEGER( m_idFrameAccuracy, tLate );

	// This is a kludge - we can get frames that are very late
	// especially (at start-up) and they invalidate the statistics.
	// So ignore things that are more than 1 sec off.
	if (tLate>1000 || tLate<-1000) {
		if (m_cFramesDrawn<=1) {
			tLate = 0;
		} else if (tLate>0) {
			tLate = 1000;
		} else {
			tLate = -1000;
		}
	}
	// The very first frame often has a invalid time, so don't
	// count it into the statistics.	(???)
	if (m_cFramesDrawn>1) {
		m_iTotAcc += tLate;
		m_iSumSqAcc += (tLate*tLate);
	}

	// calculate inter-frame time.  Doesn't make sense for first frame
	// second frame suffers from invalid first frame stamp.
	if (m_cFramesDrawn>2) {
		int tFrame = trFrame/10000;	// convert to mSec else it overflows

		// This is a kludge.  It can overflow anyway (a pause can cause
		// a very long inter-frame time) and it overflows at 2**31/10**7
		// or about 215 seconds i.e. 3min 35sec
		if (tFrame>1000||tFrame<0) tFrame = 1000;
		m_iSumSqFrameTime += tFrame*tFrame;
		ASSERT(m_iSumSqFrameTime>=0);
		m_iSumFrameTime += tFrame;
	}
	++m_cFramesDrawn;

} // RecordFrameLateness


void D_CBaseVideoRenderer::ThrottleWait()
{
	SETUP_WIN_API

	if (m_trThrottle>0) {
		int iThrottle = m_trThrottle/10000;	// convert to mSec
		MSR_INTEGER( m_idThrottle, iThrottle);
		DbgLog((LOG_TRACE, 0, TEXT("Throttle %d ms"), iThrottle));
		WinAPIData.Win32Func.SleepFunc(( DWORD )iThrottle);
	} else {
		WinAPIData.Win32Func.SleepFunc(0);
	}
} // ThrottleWait


// Whenever a frame is rendered it goes though either OnRenderStart
// or OnDirectRender.  Data that are generated during ShouldDrawSample
// are added to the statistics by calling RecordFrameLateness from both
// these two places.

// Called in place of OnRenderStart..OnRenderEnd
// When a DirectDraw image is drawn
void D_CBaseVideoRenderer::OnDirectRender(D_IMediaSample * /*pMediaSample*/)
{
	m_trRenderAvg = 0;
	m_trRenderLast = 5000000;  // If we mode switch, we do NOT want this
								// to inhibit the new average getting going!
								// so we set it to half a second
	// MSR_INTEGER(m_idRenderAvg, m_trRenderAvg/10000);
	RecordFrameLateness(m_trLate, m_trFrame);
	ThrottleWait();
} // OnDirectRender


// Called just before we start drawing.  All we do is to get the current clock
// time (from the system) and return.  We have to store the start render time
// in a member variable because it isn't used until we complete the drawing
// The rest is just performance logging.

void D_CBaseVideoRenderer::OnRenderStart(D_IMediaSample * /*pMediaSample*/ )
{
	SETUP_WIN_API

	RecordFrameLateness(m_trLate, m_trFrame);
	m_tRenderStart = ( int )WinAPIData.Win32Func.timeGetTimeFunc();
} // OnRenderStart


// Called directly after drawing an image.  We calculate the time spent in the
// drawing code and if this doesn't appear to have any odd looking spikes in
// it then we add it to the current average draw time.  Measurement spikes may
// occur if the drawing thread is interrupted and switched to somewhere else.

void D_CBaseVideoRenderer::OnRenderEnd(D_IMediaSample * /*pMediaSample*/)
{
	// The renderer time can vary erratically if we are interrupted so we do
	// some smoothing to help get more sensible figures out but even that is
	// not enough as figures can go 9,10,9,9,83,9 and we must disregard 83

	SETUP_WIN_API

	int tr = ( int )( (WinAPIData.Win32Func.timeGetTimeFunc() - m_tRenderStart)*10000 );	// convert mSec->UNITS
	if (tr < m_trRenderAvg*2 || tr < 2 * m_trRenderLast) {
		// DO_MOVING_AVG(m_trRenderAvg, tr);
		m_trRenderAvg = (tr + (AVGPERIOD-1)*m_trRenderAvg)/AVGPERIOD;
	}
	m_trRenderLast = tr;
	ThrottleWait();
} // OnRenderEnd


STDMETHODIMP D_CBaseVideoRenderer::SetSink( D_IQualityControl * piqc)
{

	m_pQSink = piqc;

	return NOERROR;
} // SetSink


STDMETHODIMP D_CBaseVideoRenderer::Notify( D_IBaseFilter * /*pSelf*/, D_Quality q)
{
	// NOTE:  We are NOT getting any locks here.  We could be called
	// asynchronously and possibly even on a time critical thread of
	// someone else's - so we do the minumum.  We only set one state
	// variable (an integer) and if that happens to be in the middle
	// of another thread reading it they will just get either the new
	// or the old value.  Locking would achieve no more than this.

	// It might be nice to check that we are being called from m_pGraph, but
	// it turns out to be a millisecond or so per throw!

	// This is heuristics, these numbers are aimed at being "what works"
	// rather than anything based on some theory.
	// We use a hyperbola because it's easy to calculate and it includes
	// a panic button asymptote (which we push off just to the left)
	// The throttling fits the following table (roughly)
	// Proportion	Throttle (msec)
	//	 >=1000		 0
	//		900		 3
	//		800		 7
	//		700		11
	//		600		17
	//		500		25
	//		400		35
	//		300		50
	//		200		72
	//		125		100
	//		100		112
	//		 50		146
	//		  0		200

	// (some evidence that we could go for a sharper kink - e.g. no throttling
	// until below the 750 mark - might give fractionally more frames on a
	// P60-ish machine).  The easy way to get these coefficients is to use
	// Renbase.xls follow the instructions therein using excel solver.

	if (q.Proportion>=1000) { m_trThrottle = 0; }
	else {
		// The DWORD is to make quite sure I get unsigned arithmetic
		// as the constant is between 2**31 and 2**32
		m_trThrottle = -330000 + (388880000/(q.Proportion+167));
	}
	return NOERROR;
} // Notify


// Send a message to indicate what our supplier should do about quality.
// Theory:
// What a supplier wants to know is "is the frame I'm working on NOW
// going to be late?".
// F1 is the frame at the supplier (as above)
// Tf1 is the due time for F1
// T1 is the time at that point (NOW!)
// Tr1 is the time that f1 WILL actually be rendered
// L1 is the latency of the graph for frame F1 = Tr1-T1
// D1 (for delay) is how late F1 will be beyond its due time i.e.
// D1 = (Tr1-Tf1) which is what the supplier really wants to know.
// Unfortunately Tr1 is in the future and is unknown, so is L1
//
// We could estimate L1 by its value for a previous frame,
// L0 = Tr0-T0 and work off
// D1' = ((T1+L0)-Tf1) = (T1 + (Tr0-T0) -Tf1)
// Rearranging terms:
// D1' = (T1-T0) + (Tr0-Tf1)
//		adding (Tf0-Tf0) and rearranging again:
//	 = (T1-T0) + (Tr0-Tf0) + (Tf0-Tf1)
//	 = (T1-T0) - (Tf1-Tf0) + (Tr0-Tf0)
// But (Tr0-Tf0) is just D0 - how late frame zero was, and this is the
// Late field in the quality message that we send.
// The other two terms just state what correction should be applied before
// using the lateness of F0 to predict the lateness of F1.
// (T1-T0) says how much time has actually passed (we have lost this much)
// (Tf1-Tf0) says how much time should have passed if we were keeping pace
// (we have gained this much).
//
// Suppliers should therefore work off:
//	D_Quality.Late + (T1-T0)  - (Tf1-Tf0)
// and see if this is "acceptably late" or even early (i.e. negative).
// They get T1 and T0 by polling the clock, they get Tf1 and Tf0 from
// the time stamps in the frames.  They get D_Quality.Late from us.
//

HRESULT D_CBaseVideoRenderer::SendQuality(D_REFERENCE_TIME trLate,
										D_REFERENCE_TIME trRealStream)
{
	D_Quality q;
	HRESULT hr;

	// If we are the main user of time, then report this as Flood/Dry.
	// If our suppliers are, then report it as D_Famine/Glut.
	//
	// We need to take action, but avoid hunting.  Hunting is caused by
	// 1. Taking too much action too soon and overshooting
	// 2. Taking too long to react (so averaging can CAUSE hunting).
	//
	// The reason why we use trLate as well as Wait is to reduce hunting;
	// if the wait time is coming down and about to go into the red, we do
	// NOT want to rely on some average which is only telling is that it used
	// to be OK once.

	q.TimeStamp = (D_REFERENCE_TIME)trRealStream;

	if (m_trFrameAvg<0) {
		q.Type = D_Famine;	  // guess
	}
	// Is the greater part of the time taken bltting or something else
	else if (m_trFrameAvg > 2*m_trRenderAvg) {
		q.Type = D_Famine;						// mainly other
	} else {
		q.Type = D_Flood;						 // mainly bltting
	}

	q.Proportion = 1000;				// default

	if (m_trFrameAvg<0) {
		// leave it alone - we don't know enough
	}
	else if ( trLate> 0 ) {
		// try to catch up over the next second
		// We could be Really, REALLY late, but rendering all the frames
		// anyway, just because it's so cheap.

		q.Proportion = 1000 - (int)((trLate)/(UNITS/1000));
		if (q.Proportion<500) {
			q.Proportion = 500;	  // don't go daft. (could've been negative!)
		} else {
		}

	} else if (  m_trWaitAvg>20000
			  && trLate<-20000
			  ){
		// Go cautiously faster - aim at 2mSec wait.
		if (m_trWaitAvg>=m_trFrameAvg) {
			// This can happen because of some fudges.
			// The waitAvg is how long we originally planned to wait
			// The frameAvg is more honest.
			// It means that we are spending a LOT of time waiting
			q.Proportion = 2000;	// double.
		} else {
			if (m_trFrameAvg+20000 > m_trWaitAvg) {
				q.Proportion
					= 1000 * (m_trFrameAvg / (m_trFrameAvg + 20000 - m_trWaitAvg));
			} else {
				// We're apparently spending more than the whole frame time waiting.
				// Assume that the averages are slightly out of kilter, but that we
				// are indeed doing a lot of waiting.  (This leg probably never
				// happens, but the code avoids any potential divide by zero).
				q.Proportion = 2000;
			}
		}

		if (q.Proportion>2000) {
			q.Proportion = 2000;	// don't go crazy.
		}
	}

	// Tell the supplier how late frames are when they get rendered
	// That's how late we are now.
	// If we are in directdraw mode then the guy upstream can see the drawing
	// times and we'll just report on the start time.  He can figure out any
	// offset to apply.  If we are in DIB Section mode then we will apply an
	// extra offset which is half of our drawing time.  This is usually small
	// but can sometimes be the dominant effect.  For this we will use the
	// average drawing time rather than the last frame.  If the last frame took
	// a long time to draw and made us late, that's already in the lateness
	// figure.  We should not add it in again unless we expect the next frame
	// to be the same.  We don't, we expect the average to be a better shot.
	// In direct draw mode the RenderAvg will be zero.

	q.Late = trLate + m_trRenderAvg/2;

	// log what we're doing
	MSR_INTEGER(m_idQualityRate, q.Proportion);
	MSR_INTEGER( m_idQualityTime, (int)q.Late / 10000 );

	// A specific sink interface may be set through D_IPin

	if (m_pQSink==NULL) {
		// Get our input pin's peer.  We send quality management messages
		// to any nominated receiver of these things (set in the D_IPin
		// interface), or else to our source filter.

		D_IQualityControl *pQC = NULL;
		D_IPin *pOutputPin = m_pInputPin->GetConnected();
		ASSERT(pOutputPin != NULL);

		// And get an AddRef'd quality control interface

		hr = pOutputPin->QueryInterface(IID_IQUALITYCONTROL,(void**) &pQC);
		if (SUCCEEDED(hr)) {
			m_pQSink = pQC;
		}
	}
	if (m_pQSink) {
		return m_pQSink->Notify(this,q);
	}

	return S_FALSE;

} // SendQuality


// We are called with a valid D_IMediaSample image to decide whether this is to
// be drawn or not.  There must be a reference clock in operation.
// Return S_OK if it is to be drawn Now (as soon as possible)
// Return S_FALSE if it is to be drawn when it's due
// Return an error if we want to drop it
// m_nNormal=-1 indicates that we dropped the previous frame and so this
// one should be drawn early.  Respect it and update it.
// Use current stream time plus a number of heuristics (detailed below)
// to make the decision

HRESULT D_CBaseVideoRenderer::ShouldDrawSampleNow(D_IMediaSample *pMediaSample,
												D_REFERENCE_TIME *ptrStart,
												D_REFERENCE_TIME *ptrEnd)
{
	SETUP_WIN_API

	// Don't call us unless there's a clock interface to synchronise with
	ASSERT(m_pClock);

	MSR_INTEGER(m_idTimeStamp, (int)((*ptrStart)>>32));	// high order 32 bits
	MSR_INTEGER(m_idTimeStamp, (int)(*ptrStart));		 // low order 32 bits

	// We lose a bit of time depending on the monitor type waiting for the next
	// screen refresh.  On average this might be about 8mSec - so it will be
	// later than we think when the picture appears.  To compensate a bit
	// we bias the media samples by -8mSec i.e. 80000 UNITs.
	// We don't ever make a stream time negative (call it paranoia)
	if (*ptrStart>=80000) {
		*ptrStart -= 80000;
		*ptrEnd -= 80000;		// bias stop to to retain valid frame duration
	}

	// Cache the time stamp now.  We will want to compare what we did with what
	// we started with (after making the monitor allowance).
	m_trRememberStampForPerf = *ptrStart;

	// Get reference times (current and late)
	D_REFERENCE_TIME trRealStream;	 // the real time now expressed as stream time.
	m_pClock->GetTime(&trRealStream);
#ifdef PERF
	// While the reference clock is expensive:
	// Remember the offset from timeGetTime and use that.
	// This overflows all over the place, but when we subtract to get
	// differences the overflows all cancel out.
	m_llTimeOffset = trRealStream-WinAPIData.Win32Func.timeGetTimeFunc()*10000;
#endif
	trRealStream -= m_tStart;	 // convert to stream time (this is a reftime)

	// We have to wory about two versions of "lateness".  The truth, which we
	// try to work out here and the one measured against m_trTarget which
	// includes long term feedback.  We report statistics against the truth
	// but for operational decisions we work to the target.
	// We use TimeDiff to make sure we get an integer because we
	// may actually be late (or more likely early if there is a big time
	// gap) by a very long time.
	const int trTrueLate = TimeDiff(trRealStream - *ptrStart);
	const int trLate = trTrueLate;

	MSR_INTEGER(m_idSchLateTime, trTrueLate/10000);

	// Send quality control messages upstream, measured against target
	HRESULT hr = SendQuality(trLate, trRealStream);
	// Note: the filter upstream is allowed to this FAIL meaning "you do it".
	m_bSupplierHandlingQuality = (hr==S_OK);

	// Decision time!  Do we drop, draw when ready or draw immediately?

	const int trDuration = (int)(*ptrEnd - *ptrStart);
	{
		// We need to see if the frame rate of the file has just changed.
		// This would make comparing our previous frame rate with the current
		// frame rate inefficent.  Hang on a moment though.  I've seen files
		// where the frames vary between 33 and 34 mSec so as to average
		// 30fps.  A minor variation like that won't hurt us.
		int t = m_trDuration/32;
		if (  trDuration > m_trDuration+t
			|| trDuration < m_trDuration-t
			) {
			// There's a major variation.  Reset the average frame rate to
			// exactly the current rate to disable decision 9002 for this frame,
			// and remember the new rate.
			m_trFrameAvg = trDuration;
			m_trDuration = trDuration;
		}
	}

	MSR_INTEGER(m_idEarliness, m_trEarliness/10000);
	MSR_INTEGER(m_idRenderAvg, m_trRenderAvg/10000);
	MSR_INTEGER(m_idFrameAvg, m_trFrameAvg/10000);
	MSR_INTEGER(m_idWaitAvg, m_trWaitAvg/10000);
	MSR_INTEGER(m_idDuration, trDuration/10000);

#ifdef PERF
	if (S_OK==pMediaSample->IsDiscontinuity()) {
		MSR_INTEGER(m_idDecision, 9000);
	}
#endif

	// Control the graceful slide back from slow to fast machine mode.
	// After a frame drop accept an early frame and set the earliness to here
	// If this frame is already later than the earliness then slide it to here
	// otherwise do the standard slide (reduce by about 12% per frame).
	// Note: earliness is normally NEGATIVE
	BOOL bJustDroppedFrame
		= (  m_bSupplierHandlingQuality
		  //  Can't use the pin sample properties because we might
		  //  not be in Receive when we call this
		  && (S_OK == pMediaSample->IsDiscontinuity())		  // he just dropped one
		  )
		|| (m_nNormal==-1);						  // we just dropped one


	// Set m_trEarliness (slide back from slow to fast machine mode)
	if (trLate>0) {
		m_trEarliness = 0;	// we are no longer in fast machine mode at all!
	} else if (  (trLate>=m_trEarliness) || bJustDroppedFrame) {
		m_trEarliness = trLate;  // Things have slipped of their own accord
	} else {
		m_trEarliness = m_trEarliness - m_trEarliness/8;  // graceful slide
	}

	// prepare the new wait average - but don't pollute the old one until
	// we have finished with it.
	int trWaitAvg;
	{
		// We never mix in a negative wait.  This causes us to believe in fast machines
		// slightly more.
		int trL = trLate<0 ? -trLate : 0;
		trWaitAvg = (trL + m_trWaitAvg*(AVGPERIOD-1))/AVGPERIOD;
	}


	int trFrame;
	{
		D_REFERENCE_TIME tr = trRealStream - m_trLastDraw; // Cd be large - 4 min pause!
		if (tr>10000000) {
			tr = 10000000;	// 1 second - arbitrarily.
		}
		trFrame = int(tr);
	}

	// We will DRAW this frame IF...
	if (
		  // ...the time we are spending drawing is a small fraction of the total
		  // observed inter-frame time so that dropping it won't help much.
		  (3*m_trRenderAvg <= m_trFrameAvg)

		 // ...or our supplier is NOT handling things and the next frame would
		 // be less timely than this one or our supplier CLAIMS to be handling
		 // things, and is now less than a full FOUR frames late.
		|| ( m_bSupplierHandlingQuality
		  ? (trLate <= trDuration*4)
		  : (trLate+trLate < trDuration)
		  )

		  // ...or we are on average waiting for over eight milliseconds then
		  // this may be just a glitch.  Draw it and we'll hope to catch up.
		|| (m_trWaitAvg > 80000)

		  // ...or we haven't drawn an image for over a second.  We will update
		  // the display, which stops the video looking hung.
		  // Do this regardless of how late this media sample is.
		|| ((trRealStream - m_trLastDraw) > UNITS)

	) {
		HRESULT Result;

		// We are going to play this frame.  We may want to play it early.
		// We will play it early if we think we are in slow machine mode.
		// If we think we are NOT in slow machine mode, we will still play
		// it early by m_trEarliness as this controls the graceful slide back.
		// and in addition we aim at being m_trTarget late rather than "on time".

		BOOL bPlayASAP = FALSE;

		// we will play it AT ONCE (slow machine mode) if...

			// ...we are playing catch-up
		if ( bJustDroppedFrame) {
			bPlayASAP = TRUE;
			MSR_INTEGER(m_idDecision, 9001);
		}

			// ...or if we are running below the true frame rate
			// exact comparisons are glitchy, for these measurements,
			// so add an extra 5% or so
		else if (  (m_trFrameAvg > trDuration + trDuration/16)

					// It's possible to get into a state where we are losing ground, but
					// are a very long way ahead.  To avoid this or recover from it
					// we refuse to play early by more than 10 frames.
				&& (trLate > - trDuration*10)
				){
			bPlayASAP = TRUE;
			MSR_INTEGER(m_idDecision, 9002);
		}
#if 0
			// ...or if we have been late and are less than one frame early
		else if (  (trLate + trDuration > 0)
				&& (m_trWaitAvg<=20000)
				) {
			bPlayASAP = TRUE;
			MSR_INTEGER(m_idDecision, 9003);
		}
#endif
		// We will NOT play it at once if we are grossly early.  On very slow frame
		// rate movies - e.g. clock.avi - it is not a good idea to leap ahead just
		// because we got starved (for instance by the net) and dropped one frame
		// some time or other.  If we are more than 900mSec early, then wait.
		if (trLate<-9000000) {
			bPlayASAP = FALSE;
		}

		if (bPlayASAP) {

			m_nNormal = 0;
			MSR_INTEGER(m_idDecision, 0);
			// When we are here, we are in slow-machine mode.  trLate may well
			// oscillate between negative and positive when the supplier is
			// dropping frames to keep sync.  We should not let that mislead
			// us into thinking that we have as much as zero spare time!
			// We just update with a zero wait.
			m_trWaitAvg = (m_trWaitAvg*(AVGPERIOD-1))/AVGPERIOD;

			// Assume that we draw it immediately.  Update inter-frame stats
			m_trFrameAvg = (trFrame + m_trFrameAvg*(AVGPERIOD-1))/AVGPERIOD;
#ifndef PERF
			// If this is NOT a perf build, then report what we know so far
			// without looking at the clock any more.  This assumes that we
			// actually wait for exactly the time we hope to.  It also reports
			// how close we get to the manipulated time stamps that we now have
			// rather than the ones we originally started with.  It will
			// therefore be a little optimistic.  However it's fast.
			PreparePerformanceData(trTrueLate, trFrame);
#endif
			m_trLastDraw = trRealStream;
			if (m_trEarliness > trLate) {
				m_trEarliness = trLate;  // if we are actually early, this is neg
			}
			Result = S_OK;					// Draw it now

		} else {
			++m_nNormal;
			// Set the average frame rate to EXACTLY the ideal rate.
			// If we are exiting slow-machine mode then we will have caught up
			// and be running ahead, so as we slide back to exact timing we will
			// have a longer than usual gap at this point.  If we record this
			// real gap then we'll think that we're running slow and go back
			// into slow-machine mode and vever get it straight.
			m_trFrameAvg = trDuration;
			MSR_INTEGER(m_idDecision, 1);

			// Play it early by m_trEarliness and by m_trTarget

			{
				int trE = m_trEarliness;
				if (trE < -m_trFrameAvg) {
					trE = -m_trFrameAvg;
				}
				*ptrStart += trE;			// N.B. earliness is negative
			}

			int Delay = -trTrueLate;
			Result = Delay<=0 ? S_OK : S_FALSE;	 // OK = draw now, FALSE = wait

			m_trWaitAvg = trWaitAvg;

			// Predict when it will actually be drawn and update frame stats

			if (Result==S_FALSE) {	// We are going to wait
				trFrame = TimeDiff(*ptrStart-m_trLastDraw);
				m_trLastDraw = *ptrStart;
			} else {
				// trFrame is already = trRealStream-m_trLastDraw;
				m_trLastDraw = trRealStream;
			}
#ifndef PERF
			int iAccuracy;
			if (Delay>0) {
				// Report lateness based on when we intend to play it
				iAccuracy = TimeDiff(*ptrStart-m_trRememberStampForPerf);
			} else {
				// Report lateness based on playing it *now*.
				iAccuracy = trTrueLate;	 // trRealStream-RememberStampForPerf;
			}
			PreparePerformanceData(iAccuracy, trFrame);
#endif
		}
		return Result;
	}

	// We are going to drop this frame!
	// Of course in DirectDraw mode the guy upstream may draw it anyway.

	// This will probably give a large negative wack to the wait avg.
	m_trWaitAvg = trWaitAvg;

#ifdef PERF
	// Respect registry setting - debug only!
	if (m_bDrawLateFrames) {
		return S_OK;						// draw it when it's ready
	}									  // even though it's late.
#endif

	// We are going to drop this frame so draw the next one early
	// n.b. if the supplier is doing direct draw then he may draw it anyway
	// but he's doing something funny to arrive here in that case.

	MSR_INTEGER(m_idDecision, 2);
	m_nNormal = -1;
	return E_FAIL;						 // drop it

} // ShouldDrawSampleNow


// NOTE we're called by both the window thread and the source filter thread
// so we have to be protected by a critical section (locked before called)
// Also, when the window thread gets signalled to render an image, it always
// does so regardless of how late it is. All the degradation is done when we
// are scheduling the next sample to be drawn. Hence when we start an advise
// link to draw a sample, that sample's time will always become the last one
// drawn - unless of course we stop streaming in which case we cancel links

BOOL D_CBaseVideoRenderer::ScheduleSample(D_IMediaSample *pMediaSample)
{
	// We override ShouldDrawSampleNow to add quality management

	BOOL bDrawImage = D_CBaseRenderer::ScheduleSample(pMediaSample);
	if (bDrawImage == FALSE) {
	++m_cFramesDropped;
	return FALSE;
	}

	// m_cFramesDrawn must NOT be updated here.  It has to be updated
	// in RecordFrameLateness at the same time as the other statistics.
	return TRUE;
}


// Implementation of D_IQualProp interface needed to support the property page
// This is how the property page gets the data out of the scheduler. We are
// passed into the constructor the owning object in the COM sense, this will
// either be the video renderer or an external IUnknown if we're aggregated.
// We initialise our D_CUnknown base class with this interface pointer. Then
// all we have to do is to override NonDelegatingQueryInterface to expose
// our D_IQualProp interface. The AddRef and Release are handled automatically
// by the base class and will be passed on to the appropriate outer object

STDMETHODIMP D_CBaseVideoRenderer::get_FramesDroppedInRenderer(int *pcFramesDropped)
{
	CheckPointer(pcFramesDropped,E_POINTER);
	D_CAutoLock cVideoLock(&m_InterfaceLock);
	*pcFramesDropped = m_cFramesDropped;
	return NOERROR;
} // get_FramesDroppedInRenderer


// Set *pcFramesDrawn to the number of frames drawn since
// streaming started.

STDMETHODIMP D_CBaseVideoRenderer::get_FramesDrawn( int *pcFramesDrawn)
{
	CheckPointer(pcFramesDrawn,E_POINTER);
	D_CAutoLock cVideoLock(&m_InterfaceLock);
	*pcFramesDrawn = m_cFramesDrawn;
	return NOERROR;
} // get_FramesDrawn


// Set iAvgFrameRate to the frames per hundred secs since
// streaming started.  0 otherwise.

STDMETHODIMP D_CBaseVideoRenderer::get_AvgFrameRate( int *piAvgFrameRate)
{
	SETUP_WIN_API

	CheckPointer(piAvgFrameRate,E_POINTER);
	D_CAutoLock cVideoLock(&m_InterfaceLock);

	int t;
	if (m_bStreaming) {
		t = ( int )( WinAPIData.Win32Func.timeGetTimeFunc()-m_tStreamingStart );
	} else {
		t = m_tStreamingStart;
	}

	if (t<=0) {
		*piAvgFrameRate = 0;
		ASSERT(m_cFramesDrawn == 0);
	} else {
		// i is frames per hundred seconds
		*piAvgFrameRate = WinAPIData.Win32Func.MulDivFunc(100000, m_cFramesDrawn, t);
	}
	return NOERROR;
} // get_AvgFrameRate


// Set *piAvg to the average sync offset since streaming started
// in mSec.  The sync offset is the time in mSec between when the frame
// should have been drawn and when the frame was actually drawn.

STDMETHODIMP D_CBaseVideoRenderer::get_AvgSyncOffset( int *piAvg)
{
	CheckPointer(piAvg,E_POINTER);
	D_CAutoLock cVideoLock(&m_InterfaceLock);

	if (NULL==m_pClock) {
		*piAvg = 0;
		return NOERROR;
	}

	// Note that we didn't gather the stats on the first frame
	// so we use m_cFramesDrawn-1 here
	if (m_cFramesDrawn<=1) {
		*piAvg = 0;
	} else {
		*piAvg = (int)(m_iTotAcc / (m_cFramesDrawn-1));
	}
	return NOERROR;
} // get_AvgSyncOffset


// To avoid dragging in the maths library - a cheap
// approximate integer square root.
// We do this by getting a starting guess which is between 1
// and 2 times too large, followed by THREE iterations of
// Newton Raphson.  (That will give accuracy to the nearest mSec
// for the range in question - roughly 0..1000)
//
// It would be faster to use a linear interpolation and ONE NR, but
// who cares.  If anyone does - the best linear interpolation is
// to approximates sqrt(x) by
// y = x * (sqrt(2)-1) + 1 - 1/sqrt(2) + 1/(8*(sqrt(2)-1))
// 0r y = x*0.41421 + 0.59467
// This minimises the maximal error in the range in question.
// (error is about +0.008883 and then one NR will give error .0000something
// (Of course these are integers, so you can't just multiply by 0.41421
// you'd have to do some sort of MulDiv).
// Anyone wanna check my maths?  (This is only for a property display!)

int isqrt(int x)
{
	int s = 1;
	// Make s an initial guess for sqrt(x)
	if (x > 0x40000000) {
		s = 0x8000;	 // prevent any conceivable closed loop
	} else {
		while (s*s<x) {	// loop cannot possible go more than 31 times
			s = 2*s;		// normally it goes about 6 times
		}
		// Three NR iterations.
		if (x==0) {
			s= 0; // Wouldn't it be tragic to divide by zero whenever our
				 // accuracy was perfect!
		} else {
			s = (s*s+x)/(2*s);
			if (s>=0) s = (s*s+x)/(2*s);
			if (s>=0) s = (s*s+x)/(2*s);
		}
	}
	return s;
}

//
//  Do estimates for standard deviations for per-frame
//  statistics
//
HRESULT D_CBaseVideoRenderer::GetStdDev(
	int nSamples,
	int *piResult,
	LONGLONG llSumSq,
	LONGLONG iTot
)
{
	CheckPointer(piResult,E_POINTER);
	D_CAutoLock cVideoLock(&m_InterfaceLock);

	if (NULL==m_pClock) {
		*piResult = 0;
		return NOERROR;
	}

	// If S is the Sum of the Squares of observations and
	//	T the Total (i.e. sum) of the observations and there were
	//	N observations, then an estimate of the standard deviation is
	//	  sqrt( (S - T**2/N) / (N-1) )

	if (nSamples<=1) {
		*piResult = 0;
	} else {
		LONGLONG x;
		// First frames have invalid stamps, so we get no stats for them
		// So we need 2 frames to get 1 datum, so N is cFramesDrawn-1

		// so we use m_cFramesDrawn-1 here
		x = llSumSq - llMulDiv(iTot, iTot, nSamples, 0);
		x = x / (nSamples-1);
		ASSERT(x>=0);
		*piResult = isqrt((LONG)x);
	}
	return NOERROR;
}

// Set *piDev to the standard deviation in mSec of the sync offset
// of each frame since streaming started.

STDMETHODIMP D_CBaseVideoRenderer::get_DevSyncOffset( int *piDev)
{
	// First frames have invalid stamps, so we get no stats for them
	// So we need 2 frames to get 1 datum, so N is cFramesDrawn-1
	return GetStdDev(m_cFramesDrawn - 1,
					 piDev,
					 m_iSumSqAcc,
					 m_iTotAcc);
} // get_DevSyncOffset


// Set *piJitter to the standard deviation in mSec of the inter-frame time
// of frames since streaming started.

STDMETHODIMP D_CBaseVideoRenderer::get_Jitter( int *piJitter)
{
	// First frames have invalid stamps, so we get no stats for them
	// So second frame gives invalid inter-frame time
	// So we need 3 frames to get 1 datum, so N is cFramesDrawn-2
	return GetStdDev(m_cFramesDrawn - 2,
					 piJitter,
					 m_iSumSqFrameTime,
					 m_iSumFrameTime);
} // get_Jitter


// Overidden to return our D_IQualProp interface

STDMETHODIMP
D_CBaseVideoRenderer::NonDelegatingQueryInterface(REFIID riid,VOID **ppv)
{
	// We return D_IQualProp and delegate everything else

	if (riid == IID_IQUALPROP) {
		return D_GetInterface( (D_IQualProp *)this, ppv);
	} else if (riid == IID_IQUALITYCONTROL) {
		return D_GetInterface( (D_IQualityControl *)this, ppv);
	}
	return D_CBaseRenderer::NonDelegatingQueryInterface(riid,ppv);
}


// Override JoinFilterGraph so that, just before leaving
// the graph we can send an EC_WINDOW_DESTROYED event

STDMETHODIMP
D_CBaseVideoRenderer::JoinFilterGraph(D_IFilterGraph *pGraph,LPCWSTR pName)
{
	// Since we send EC_ACTIVATE, we also need to ensure
	// we send EC_WINDOW_DESTROYED or the resource manager may be
	// holding us as a focus object
	if (!pGraph && m_pGraph) {

		// We were in a graph and now we're not
		// Do this properly in case we are aggregated
		D_IBaseFilter* pFilter;
		QueryInterface(IID_IBASEFILTER,(void **) &pFilter);
		NotifyEvent(EC_WINDOW_DESTROYED, (LPARAM) pFilter, 0);
		pFilter->Release();
	}
	return D_CBaseFilter::JoinFilterGraph(pGraph, pName);
}


// This removes a large number of level 4 warnings from the
// Microsoft compiler which in this case are not very useful
#pragma warning(disable: 4514)
























STDMETHODIMP D_CBaseFilter::NonDelegatingQueryInterface(REFIID riid,
													  void **ppv)
{
	/* Do we have this interface */

	if (riid == IID_IBASEFILTER) {
		return D_GetInterface((D_IBaseFilter *) this, ppv);
	} else if (riid == IID_IMEDIAFILTER) {
		return D_GetInterface((D_IMediaFilter *) this, ppv);
	} else if (riid == IID_IPERSIST) {
		return D_GetInterface((D_IPersist *) this, ppv);
	} else if (riid == IID_IAMOVIESETUP) {
		return D_GetInterface((D_IAMovieSetup *) this, ppv);
	} else {
		return D_CUnknown::NonDelegatingQueryInterface(riid, ppv);
	}
}

#ifdef DEBUG
STDMETHODIMP_(ULONG) D_CBaseFilter::NonDelegatingRelease()
{
	if (m_cRef == 1) {
		KASSERT(m_pGraph == NULL);
	}
	return D_CUnknown::NonDelegatingRelease();
}
#endif


/* Constructor */

D_CBaseFilter::D_CBaseFilter(const TCHAR	*pName,
			 D_IUnknown *  pUnk,
			 D_CCritSec	*pLock,
			 REFCLSID	clsid) :
	D_CUnknown( pName, pUnk ),
	m_pLock(pLock),
	m_clsid(clsid),
	m_State(D_State_Stopped),
	m_pClock(NULL),
	m_pGraph(NULL),
	m_pSink(NULL),
	m_pName(NULL),
	m_PinVersion(1)
{

	ASSERT(pLock != NULL);
}

/* Passes in a redundant HRESULT argument */

D_CBaseFilter::D_CBaseFilter(const TCHAR *pName,
						 D_IUnknown *  pUnk,
						 D_CCritSec  *pLock,
						 REFCLSID	clsid,
						 HRESULT	*phr) :
	D_CUnknown( pName, pUnk ),
	m_pLock(pLock),
	m_clsid(clsid),
	m_State(D_State_Stopped),
	m_pClock(NULL),
	m_pGraph(NULL),
	m_pSink(NULL),
	m_pName(NULL),
	m_PinVersion(1)
{

	ASSERT(pLock != NULL);
	UNREFERENCED_PARAMETER(phr);
}

#ifdef UNICODE
D_CBaseFilter::D_CBaseFilter(const CHAR *pName,
			 D_IUnknown *  pUnk,
			 D_CCritSec	*pLock,
			 REFCLSID	clsid) :
	D_CUnknown( pName, pUnk ),
	m_pLock(pLock),
	m_clsid(clsid),
	m_State(D_State_Stopped),
	m_pClock(NULL),
	m_pGraph(NULL),
	m_pSink(NULL),
	m_pName(NULL),
	m_PinVersion(1)
{

	ASSERT(pLock != NULL);
}
D_CBaseFilter::D_CBaseFilter(CHAR	 *pName,
						 D_IUnknown *  pUnk,
						 D_CCritSec  *pLock,
						 REFCLSID	clsid,
						 HRESULT	*phr) :
	D_CUnknown( pName, pUnk ),
	m_pLock(pLock),
	m_clsid(clsid),
	m_State(D_State_Stopped),
	m_pClock(NULL),
	m_pGraph(NULL),
	m_pSink(NULL),
	m_pName(NULL),
	m_PinVersion(1)
{

	ASSERT(pLock != NULL);
	UNREFERENCED_PARAMETER(phr);
}
#endif

/* Destructor */

D_CBaseFilter::~D_CBaseFilter()
{

	// NOTE we do NOT hold references on the filtergraph for m_pGraph or m_pSink
	// When we did we had the circular reference problem.  Nothing would go away.

	delete[] m_pName;

	// must be stopped, but can't call Stop here since
	// our critsec has been destroyed.

	/* Release any clock we were using */
	if (m_pClock) {
		m_pClock->Release();
		m_pClock = NULL;
	}
}

/* Return the filter's clsid */
STDMETHODIMP
D_CBaseFilter::GetClassID(CLSID *pClsID)
{
	CheckPointer(pClsID,E_POINTER);
	ValidateReadWritePtr(pClsID,sizeof(CLSID));
	*pClsID = m_clsid;
	return NOERROR;
}

/* Override this if your state changes are not done synchronously */
STDMETHODIMP
D_CBaseFilter::GetState(DWORD dwMSecs, D_FILTER_STATE *State)
{
	UNREFERENCED_PARAMETER(dwMSecs);
	CheckPointer(State,E_POINTER);
	ValidateReadWritePtr(State,sizeof(D_FILTER_STATE));

	*State = m_State;
	return S_OK;
}


/* Set the clock we will use for synchronisation */

STDMETHODIMP
D_CBaseFilter::SetSyncSource(D_IReferenceClock *pClock)
{
	D_CAutoLock cObjectLock(m_pLock);

	// Ensure the new one does not go away - even if the same as the old
	if (pClock) {
		pClock->AddRef();
	}

	// if we have a clock, release it
	if (m_pClock) {
		m_pClock->Release();
	}

	// Set the new reference clock (might be NULL)
	// Should we query it to ensure it is a clock?  Consider for a debug build.
	m_pClock = pClock;

	return NOERROR;
}

/* Return the clock we are using for synchronisation */
STDMETHODIMP
D_CBaseFilter::GetSyncSource(D_IReferenceClock **pClock)
{
	CheckPointer(pClock,E_POINTER);
	ValidateReadWritePtr(pClock,sizeof(D_IReferenceClock *));
	D_CAutoLock cObjectLock(m_pLock);

	if (m_pClock) {
		// returning an interface... addref it...
		m_pClock->AddRef();
	}
	*pClock = (D_IReferenceClock*)m_pClock;
	return NOERROR;
}



// override CBaseMediaFilter Stop method, to deactivate any pins this
// filter has.
STDMETHODIMP
D_CBaseFilter::Stop()
{
	D_CAutoLock cObjectLock(m_pLock);
	HRESULT hr = NOERROR;

	// notify all pins of the state change
	if (m_State != D_State_Stopped) {
		int cPins = GetPinCount();
		for (int c = 0; c < cPins; c++) {

			D_CBasePin *pPin = GetPin(c);

			// Disconnected pins are not activated - this saves pins worrying
			// about this state themselves. We ignore the return code to make
			// sure everyone is inactivated regardless. The base input pin
			// class can return an error if it has no allocator but Stop can
			// be used to resync the graph state after something has gone bad

			if (pPin->IsConnected()) {
				HRESULT hrTmp = pPin->Inactive();
				if (FAILED(hrTmp) && SUCCEEDED(hr)) {
					hr = hrTmp;
				}
			}
		}
	}


	m_State = D_State_Stopped;
	return hr;
}


// override CBaseMediaFilter Pause method to activate any pins
// this filter has (also called from Run)

STDMETHODIMP
D_CBaseFilter::Pause()
{
	D_CAutoLock cObjectLock(m_pLock);

	// notify all pins of the change to active state
	if (m_State == D_State_Stopped) {
		int cPins = GetPinCount();
		for (int c = 0; c < cPins; c++) {

			D_CBasePin *pPin = GetPin(c);

			// Disconnected pins are not activated - this saves pins
			// worrying about this state themselves

			if (pPin->IsConnected()) {
				HRESULT hr = pPin->Active();
				if (FAILED(hr)) {
					return hr;
				}
			}
		}
	}



	m_State = D_State_Paused;
	return S_OK;
}

// Put the filter into a running state.

// The time parameter is the offset to be added to the samples'
// stream time to get the reference time at which they should be presented.
//
// you can either add these two and compare it against the reference clock,
// or you can call D_CBaseFilter::StreamTime and compare that against
// the sample timestamp.

STDMETHODIMP
D_CBaseFilter::Run(D_REFERENCE_TIME tStart)
{
	D_CAutoLock cObjectLock(m_pLock);

	// remember the stream time offset
	m_tStart = tStart;

	if (m_State == D_State_Stopped){
	HRESULT hr = Pause();

	if (FAILED(hr)) {
		return hr;
	}
	}
	// notify all pins of the change to active state
	if (m_State != D_State_Running) {
		int cPins = GetPinCount();
		for (int c = 0; c < cPins; c++) {

			D_CBasePin *pPin = GetPin(c);

			// Disconnected pins are not activated - this saves pins
			// worrying about this state themselves

			if (pPin->IsConnected()) {
				HRESULT hr = pPin->Run(tStart);
				if (FAILED(hr)) {
					return hr;
				}
			}
		}
	}


	m_State = D_State_Running;
	return S_OK;
}

//
// return the current stream time - samples with start timestamps of this
// time or before should be rendered by now
HRESULT
D_CBaseFilter::StreamTime(D_CRefTime& rtStream)
{
	// Caller must lock for synchronization
	// We can't grab the filter lock because we want to be able to call
	// this from worker threads without deadlocking

	if (m_pClock == NULL) {
		return VFW_E_NO_CLOCK;
	}

	// get the current reference time
	HRESULT hr = m_pClock->GetTime((D_REFERENCE_TIME*)&rtStream);
	if (FAILED(hr)) {
		return hr;
	}

	// subtract the stream offset to get stream time
	rtStream -= m_tStart;

	return S_OK;
}


/* Create an enumerator for the pins attached to this filter */

STDMETHODIMP
D_CBaseFilter::EnumPins(D_IEnumPins **ppEnum)
{
	CheckPointer(ppEnum,E_POINTER);
	ValidateReadWritePtr(ppEnum,sizeof(D_IEnumPins *));

	/* Create a new ref counted enumerator */

	*ppEnum = new D_CEnumPins(this,
						NULL);

	return *ppEnum == NULL ? E_OUTOFMEMORY : NOERROR;
}


// default behaviour of FindPin is to assume pins are named
// by their pin names
STDMETHODIMP
D_CBaseFilter::FindPin(
	LPCWSTR Id,
	D_IPin ** ppPin
)
{
	SETUP_WIN_API

	CheckPointer(ppPin,E_POINTER);
	ValidateReadWritePtr(ppPin,sizeof(D_IPin *));

	//  We're going to search the pin list so maintain integrity
	D_CAutoLock lck(m_pLock);
	int iCount = GetPinCount();
	for (int i = 0; i < iCount; i++) {
		D_CBasePin *pPin = GetPin(i);
		ASSERT(pPin != NULL);

		if (0 == lstrcmpWFunc((WORD const *)pPin->Name(), (WORD const *)Id)) {
			//  Found one that matches
			//
			//  AddRef() and return it
			*ppPin = pPin;
			pPin->AddRef();
			return S_OK;
		}
	}
	*ppPin = NULL;
	return VFW_E_NOT_FOUND;
}

/* Return information about this filter */

STDMETHODIMP
D_CBaseFilter::QueryFilterInfo(D_FILTER_INFO * pInfo)
{
	SETUP_WIN_API

	CheckPointer(pInfo,E_POINTER);
	ValidateReadWritePtr(pInfo,sizeof(D_FILTER_INFO));

	if (m_pName) {
		lstrcpynWFunc((WORD *)pInfo->achName, (WORD const *)m_pName, sizeof(pInfo->achName)/sizeof(WCHAR));
	} else {
		pInfo->achName[0] = L'\0';
	}
	pInfo->pGraph = m_pGraph;
	if (m_pGraph)
		m_pGraph->AddRef();
	return NOERROR;
}


/* Provide the filter with a filter graph */

STDMETHODIMP
D_CBaseFilter::JoinFilterGraph(
	D_IFilterGraph * pGraph,
	LPCWSTR pName)
{
	D_CAutoLock cObjectLock(m_pLock);

	SETUP_WIN_API

	// NOTE: we no longer hold references on the graph (m_pGraph, m_pSink)

	m_pGraph = pGraph;
	if (m_pGraph) {
		HRESULT hr = m_pGraph->QueryInterface(IID_IMEDIAEVENTSINK,
						(void**) &m_pSink);
		if (FAILED(hr)) {
			ASSERT(m_pSink == NULL);
		}
		else m_pSink->Release();		// we do NOT keep a reference on it.
	} else {
		// if graph pointer is null, then we should
		// also release the D_IMediaEventSink on the same object - we don't
		// refcount it, so just set it to null
		m_pSink = NULL;
	}


	if (m_pName) {
		delete[] m_pName;
		m_pName = NULL;
	}

	if (pName) {
		DWORD nameLen = ( DWORD )( lstrlenWFunc((WORD const *)pName)+1 );
		m_pName = new WCHAR[nameLen];
		if (m_pName) {
			_MEMCPY(m_pName, pName, nameLen*sizeof(WCHAR));
		} else {
			// !!! error here?
			ASSERT(FALSE);
		}
	}


	return NOERROR;
}


// return a Vendor information string. Optional - may return E_NOTIMPL.
// memory returned should be freed using CoTaskMemFree
// default implementation returns E_NOTIMPL
STDMETHODIMP
D_CBaseFilter::QueryVendorInfo(
	LPWSTR* pVendorInfo)
{
	UNREFERENCED_PARAMETER(pVendorInfo);
	return E_NOTIMPL;
}


// send an event notification to the filter graph if we know about it.
// returns S_OK if delivered, S_FALSE if the filter graph does not sink
// events, or an error otherwise.
HRESULT
D_CBaseFilter::NotifyEvent(
	long EventCode,
	LONG_PTR EventParam1,
	LONG_PTR EventParam2)
{
	// Snapshot so we don't have to lock up
	D_IMediaEventSink *pSink = m_pSink;
	if (pSink) {
		if (EC_COMPLETE == EventCode) {
			EventParam2 = (LONG_PTR)(D_IBaseFilter*)this;
		}

		return pSink->Notify(EventCode, EventParam1, EventParam2);
	} else {
		return E_NOTIMPL;
	}
}

// Request reconnect
// pPin is the pin to reconnect
// pmt is the type to reconnect with - can be NULL
// Calls ReconnectEx on the filter graph
HRESULT
D_CBaseFilter::ReconnectPin(
	D_IPin *pPin,
	D_AM_MEDIA_TYPE const *pmt
)
{
	D_IFilterGraph2 *pGraph2;
	if (m_pGraph != NULL) {
		HRESULT hr = m_pGraph->QueryInterface(IID_IFILTERGRAPH2, (void **)&pGraph2);
		if (SUCCEEDED(hr)) {
			hr = pGraph2->ReconnectEx(pPin, pmt);
			pGraph2->Release();
			return hr;
		} else {
			return m_pGraph->Reconnect(pPin);
		}
	} else {
		return E_NOINTERFACE;
	}
}



/* This is the same idea as the media type version does for type enumeration
	on pins but for the list of pins available. So if the list of pins you
	provide changes dynamically then either override this virtual function
	to provide the version number, or more simply call IncrementPinVersion */

LONG D_CBaseFilter::GetPinVersion()
{
	return m_PinVersion;
}


/* Increment the current pin version cookie */

void D_CBaseFilter::IncrementPinVersion()
{
	InterlockedIncrement(&m_PinVersion);
}

/* register filter */

STDMETHODIMP D_CBaseFilter::Register()
{
	SETUP_WIN_API

	// get setup data, if it exists
	//
	D_LPAMOVIESETUP_FILTER psetupdata = GetSetupData();

	// check we've got data
	//
	if( NULL == psetupdata ) return S_FALSE;

	// init is ref counted so call just in case
	// we're being called cold.
	//
	HRESULT hr = WinAPIData.Win32Func.CoInitializeExFunc( (LPVOID)NULL, 0 );
	ASSERT( SUCCEEDED(hr) );

	// get hold of D_IFilterMapper
	//
	D_IFilterMapper *pIFM;
	hr = WinAPIData.Win32Func.CoCreateInstanceFunc( CLSID_FILTERMAPPER
							 , NULL
							 , CLSCTX_INPROC_SERVER
							 , IID_IFILTERMAPPER
							 , (void **)&pIFM		);
	if( SUCCEEDED(hr) )
	{
		hr = D_AMovieSetupRegisterFilter( psetupdata, pIFM, TRUE );
		pIFM->Release();
	}

	// and clear up
	//
	WinAPIData.Win32Func.CoFreeUnusedLibrariesFunc();
	WinAPIData.Win32Func.CoUninitializeFunc();

	return NOERROR;
}


/* unregister filter */

STDMETHODIMP D_CBaseFilter::Unregister()
{
	SETUP_WIN_API

	// get setup data, if it exists
	//
	D_LPAMOVIESETUP_FILTER psetupdata = GetSetupData();

	// check we've got data
	//
	if( NULL == psetupdata ) return S_FALSE;

	// OLE init is ref counted so call
	// just in case we're being called cold.
	//
	HRESULT hr = WinAPIData.Win32Func.CoInitializeExFunc( (LPVOID)NULL, 0 );
	ASSERT( SUCCEEDED(hr) );

	// get hold of D_IFilterMapper
	//
	D_IFilterMapper *pIFM;
	hr = WinAPIData.Win32Func.CoCreateInstanceFunc( CLSID_FILTERMAPPER
							 , NULL
							 , CLSCTX_INPROC_SERVER
							 , IID_IFILTERMAPPER
							 , (void **)&pIFM		);
	if( SUCCEEDED(hr) )
	{
		hr = D_AMovieSetupRegisterFilter( psetupdata, pIFM, FALSE );

		// release interface
		//
		pIFM->Release();
	}

	// clear up
	//
	WinAPIData.Win32Func.CoFreeUnusedLibrariesFunc();
	WinAPIData.Win32Func.CoUninitializeFunc();

	// handle one acceptable "error" - that
	// of filter not being registered!
	// (couldn't find a suitable #define'd
	// name for the error!)
	//
	if( 0x80070002 == hr)
	  return NOERROR;
	else
	  return hr;
}







D_CRendererPosPassThru::D_CRendererPosPassThru(const TCHAR *pName,
						D_IUnknown * pUnk,
						HRESULT *phr,
						D_IPin *pPin) :
	D_CPosPassThru(pName,pUnk,phr,pPin),
	m_StartMedia(0),
	m_EndMedia(0),
	m_bReset(TRUE)
{
}


// Sets the media times the object should report

HRESULT
D_CRendererPosPassThru::RegisterMediaTime(D_IMediaSample *pMediaSample)
{
	ASSERT(pMediaSample);
	LONGLONG StartMedia;
	LONGLONG EndMedia;

	D_CAutoLock cAutoLock(&m_PositionLock);

	// Get the media times from the sample

	HRESULT hr = pMediaSample->GetTime(&StartMedia,&EndMedia);
	if (FAILED(hr))
	{
	ASSERT(hr == VFW_E_SAMPLE_TIME_NOT_SET);
	return hr;
	}

	m_StartMedia = StartMedia;
	m_EndMedia = EndMedia;
	m_bReset = FALSE;
	return NOERROR;
}


// Sets the media times the object should report

HRESULT
D_CRendererPosPassThru::RegisterMediaTime(LONGLONG StartTime,LONGLONG EndTime)
{
	D_CAutoLock cAutoLock(&m_PositionLock);
	m_StartMedia = StartTime;
	m_EndMedia = EndTime;
	m_bReset = FALSE;
	return NOERROR;
}


// Return the current media times registered in the object

HRESULT
D_CRendererPosPassThru::GetMediaTime(LONGLONG *pStartTime,LONGLONG *pEndTime)
{
	ASSERT(pStartTime);

	D_CAutoLock cAutoLock(&m_PositionLock);
	if (m_bReset == TRUE) {
	return E_FAIL;
	}

	// We don't have to return the end time

	HRESULT hr = ConvertTimeFormat( pStartTime, 0, m_StartMedia, &D_TIME_FORMAT_MEDIA_TIME );
	if (pEndTime && SUCCEEDED(hr)) {
	hr = ConvertTimeFormat( pEndTime, 0, m_EndMedia, &D_TIME_FORMAT_MEDIA_TIME );
	}
	return hr;
}


// Resets the media times we hold

HRESULT
D_CRendererPosPassThru::ResetMediaTime()
{
	D_CAutoLock cAutoLock(&m_PositionLock);
	m_StartMedia = 0;
	m_EndMedia = 0;
	m_bReset = TRUE;
	return NOERROR;
}

// Intended to be called by the owing filter during EOS processing so
// that the media times can be adjusted to the stop time.  This ensures
// that the GetCurrentPosition will actully get to the stop position.
HRESULT
D_CRendererPosPassThru::EOS()
{
	HRESULT hr;

	if ( m_bReset == TRUE ) hr = E_FAIL;
	else
	{
	LONGLONG llStop;
	if SUCCEEDED(hr=GetStopPosition(&llStop))
	{
		D_CAutoLock cAutoLock(&m_PositionLock);
		m_StartMedia =
		m_EndMedia	 = llStop;
	}
	}
	return hr;
}






D_CPosPassThru::D_CPosPassThru(const TCHAR *pName,
				D_IUnknown * pUnk,
				HRESULT *phr,
				D_IPin *pPin) :
	D_CMediaPosition(pName,pUnk),
	m_pPin(pPin)
{
	if (pPin == NULL) {
	*phr = E_POINTER;
	return;
	}
}


// Expose our D_IMediaSeeking and D_IMediaPosition interfaces

STDMETHODIMP
D_CPosPassThru::NonDelegatingQueryInterface(REFIID riid,void **ppv)
{
	CheckPointer(ppv,E_POINTER);
	*ppv = NULL;

	if (riid == IID_IMEDIASEEKING) {
	return D_GetInterface( static_cast<D_IMediaSeeking *>(this), ppv);
	}
	return D_CMediaPosition::NonDelegatingQueryInterface(riid,ppv);
}


// Return the D_IMediaPosition interface from our peer

HRESULT
D_CPosPassThru::GetPeer(D_IMediaPosition ** ppMP)
{
	*ppMP = NULL;

	D_IPin *pConnected;
	HRESULT hr = m_pPin->ConnectedTo(&pConnected);
	if (FAILED(hr)) {
	return E_NOTIMPL;
	}
	D_IMediaPosition * pMP;
	hr = pConnected->QueryInterface(IID_IMEDIAPOSITION, (void **) &pMP);
	pConnected->Release();
	if (FAILED(hr)) {
	return E_NOTIMPL;
	}

	*ppMP = pMP;
	return S_OK;
}


// Return the D_IMediaSeeking interface from our peer

HRESULT
D_CPosPassThru::GetPeerSeeking(D_IMediaSeeking ** ppMS)
{
	*ppMS = NULL;

	D_IPin *pConnected;
	HRESULT hr = m_pPin->ConnectedTo(&pConnected);
	if (FAILED(hr)) {
	return E_NOTIMPL;
	}
	D_IMediaSeeking * pMS;
	hr = pConnected->QueryInterface(IID_IMEDIASEEKING, (void **) &pMS);
	pConnected->Release();
	if (FAILED(hr)) {
	return E_NOTIMPL;
	}

	*ppMS = pMS;
	return S_OK;
}


// --- D_IMediaSeeking methods ----------


STDMETHODIMP
D_CPosPassThru::GetCapabilities(DWORD * pCaps)
{
	D_IMediaSeeking* pMS;
	HRESULT hr = GetPeerSeeking(&pMS);
	if (FAILED(hr)) {
	return hr;
	}

	hr = pMS->GetCapabilities(pCaps);
	pMS->Release();
	return hr;
}

STDMETHODIMP
D_CPosPassThru::CheckCapabilities(DWORD * pCaps)
{
	D_IMediaSeeking* pMS;
	HRESULT hr = GetPeerSeeking(&pMS);
	if (FAILED(hr)) {
	return hr;
	}

	hr = pMS->CheckCapabilities(pCaps);
	pMS->Release();
	return hr;
}

STDMETHODIMP
D_CPosPassThru::IsFormatSupported(const GUID * pFormat)
{
	D_IMediaSeeking* pMS;
	HRESULT hr = GetPeerSeeking(&pMS);
	if (FAILED(hr)) {
	return hr;
	}

	hr = pMS->IsFormatSupported(pFormat);
	pMS->Release();
	return hr;
}


STDMETHODIMP
D_CPosPassThru::QueryPreferredFormat(GUID *pFormat)
{
	D_IMediaSeeking* pMS;
	HRESULT hr = GetPeerSeeking(&pMS);
	if (FAILED(hr)) {
	return hr;
	}

	hr = pMS->QueryPreferredFormat(pFormat);
	pMS->Release();
	return hr;
}


STDMETHODIMP
D_CPosPassThru::SetTimeFormat(const GUID * pFormat)
{
	D_IMediaSeeking* pMS;
	HRESULT hr = GetPeerSeeking(&pMS);
	if (FAILED(hr)) {
	return hr;
	}

	hr = pMS->SetTimeFormat(pFormat);
	pMS->Release();
	return hr;
}


STDMETHODIMP
D_CPosPassThru::GetTimeFormat(GUID *pFormat)
{
	D_IMediaSeeking* pMS;
	HRESULT hr = GetPeerSeeking(&pMS);
	if (FAILED(hr)) {
	return hr;
	}

	hr = pMS->GetTimeFormat(pFormat);
	pMS->Release();
	return hr;
}


STDMETHODIMP
D_CPosPassThru::IsUsingTimeFormat(const GUID * pFormat)
{
	D_IMediaSeeking* pMS;
	HRESULT hr = GetPeerSeeking(&pMS);
	if (FAILED(hr)) {
	return hr;
	}

	hr = pMS->IsUsingTimeFormat(pFormat);
	pMS->Release();
	return hr;
}


STDMETHODIMP
D_CPosPassThru::ConvertTimeFormat(LONGLONG * pTarget, const GUID * pTargetFormat,
				LONGLONG	Source, const GUID * pSourceFormat )
{
	D_IMediaSeeking* pMS;
	HRESULT hr = GetPeerSeeking(&pMS);
	if (FAILED(hr)) {
	return hr;
	}

	hr = pMS->ConvertTimeFormat(pTarget, pTargetFormat, Source, pSourceFormat );
	pMS->Release();
	return hr;
}


STDMETHODIMP
D_CPosPassThru::SetPositions( LONGLONG * pCurrent, DWORD CurrentFlags
			  , LONGLONG * pStop, DWORD StopFlags )
{
	D_IMediaSeeking* pMS;
	HRESULT hr = GetPeerSeeking(&pMS);
	if (FAILED(hr)) {
	return hr;
	}

	hr = pMS->SetPositions(pCurrent, CurrentFlags, pStop, StopFlags );
	pMS->Release();
	return hr;
}

STDMETHODIMP
D_CPosPassThru::GetPositions(LONGLONG *pCurrent, LONGLONG * pStop)
{
	D_IMediaSeeking* pMS;
	HRESULT hr = GetPeerSeeking(&pMS);
	if (FAILED(hr)) {
	return hr;
	}

	hr = pMS->GetPositions(pCurrent,pStop);
	pMS->Release();
	return hr;
}

HRESULT
D_CPosPassThru::GetSeekingLongLong
( HRESULT (__stdcall D_IMediaSeeking::*pMethod)( LONGLONG * )
, LONGLONG * pll
)
{
	D_IMediaSeeking* pMS;
	HRESULT hr = GetPeerSeeking(&pMS);
	if (SUCCEEDED(hr))
	{
	hr = (pMS->*pMethod)(pll);
	pMS->Release();
	}
	return hr;
}

// If we don't have a current position then ask upstream

STDMETHODIMP
D_CPosPassThru::GetCurrentPosition(LONGLONG *pCurrent)
{
	// Can we report the current position
	HRESULT hr = GetMediaTime(pCurrent,NULL);
	if (SUCCEEDED(hr)) hr = NOERROR;
	else hr = GetSeekingLongLong( &D_IMediaSeeking::GetCurrentPosition, pCurrent );
	return hr;
}


STDMETHODIMP
D_CPosPassThru::GetStopPosition(LONGLONG *pStop)
{
	return GetSeekingLongLong( &D_IMediaSeeking::GetStopPosition, pStop );;
}

STDMETHODIMP
D_CPosPassThru::GetDuration(LONGLONG *pDuration)
{
	return GetSeekingLongLong( &D_IMediaSeeking::GetDuration, pDuration );;
}


STDMETHODIMP
D_CPosPassThru::GetPreroll(LONGLONG *pllPreroll)
{
	return GetSeekingLongLong( &D_IMediaSeeking::GetPreroll, pllPreroll );;
}


STDMETHODIMP
D_CPosPassThru::GetAvailable( LONGLONG *pEarliest, LONGLONG *pLatest )
{
	D_IMediaSeeking* pMS;
	HRESULT hr = GetPeerSeeking(&pMS);
	if (FAILED(hr)) {
	return hr;
	}

	hr = pMS->GetAvailable( pEarliest, pLatest );
	pMS->Release();
	return hr;
}


STDMETHODIMP
D_CPosPassThru::GetRate(double * pdRate)
{
	D_IMediaSeeking* pMS;
	HRESULT hr = GetPeerSeeking(&pMS);
	if (FAILED(hr)) {
	return hr;
	}
	hr = pMS->GetRate(pdRate);
	pMS->Release();
	return hr;
}


STDMETHODIMP
D_CPosPassThru::SetRate(double dRate)
{
	if (0.0 == dRate) {
		return E_INVALIDARG;
	}

	D_IMediaSeeking* pMS;
	HRESULT hr = GetPeerSeeking(&pMS);
	if (FAILED(hr)) {
	return hr;
	}
	hr = pMS->SetRate(dRate);
	pMS->Release();
	return hr;
}




// --- D_IMediaPosition methods ----------


STDMETHODIMP
D_CPosPassThru::get_Duration(D_REFTIME * plength)
{
	D_IMediaPosition* pMP;
	HRESULT hr = GetPeer(&pMP);
	if (FAILED(hr)) {
	return hr;
	}

	hr = pMP->get_Duration(plength);
	pMP->Release();
	return hr;
}


STDMETHODIMP
D_CPosPassThru::get_CurrentPosition(D_REFTIME * pllTime)
{
	D_IMediaPosition* pMP;
	HRESULT hr = GetPeer(&pMP);
	if (FAILED(hr)) {
	return hr;
	}
	hr = pMP->get_CurrentPosition(pllTime);
	pMP->Release();
	return hr;
}


STDMETHODIMP
D_CPosPassThru::put_CurrentPosition(D_REFTIME llTime)
{
	D_IMediaPosition* pMP;
	HRESULT hr = GetPeer(&pMP);
	if (FAILED(hr)) {
	return hr;
	}
	hr = pMP->put_CurrentPosition(llTime);
	pMP->Release();
	return hr;
}


STDMETHODIMP
D_CPosPassThru::get_StopTime(D_REFTIME * pllTime)
{
	D_IMediaPosition* pMP;
	HRESULT hr = GetPeer(&pMP);
	if (FAILED(hr)) {
	return hr;
	}
	hr = pMP->get_StopTime(pllTime);
	pMP->Release();
	return hr;
}


STDMETHODIMP
D_CPosPassThru::put_StopTime(D_REFTIME llTime)
{
	D_IMediaPosition* pMP;
	HRESULT hr = GetPeer(&pMP);
	if (FAILED(hr)) {
	return hr;
	}
	hr = pMP->put_StopTime(llTime);
	pMP->Release();
	return hr;
}


STDMETHODIMP
D_CPosPassThru::get_PrerollTime(D_REFTIME * pllTime)
{
	D_IMediaPosition* pMP;
	HRESULT hr = GetPeer(&pMP);
	if (FAILED(hr)) {
	return hr;
	}
	hr = pMP->get_PrerollTime(pllTime);
	pMP->Release();
	return hr;
}


STDMETHODIMP
D_CPosPassThru::put_PrerollTime(D_REFTIME llTime)
{
	D_IMediaPosition* pMP;
	HRESULT hr = GetPeer(&pMP);
	if (FAILED(hr)) {
	return hr;
	}
	hr = pMP->put_PrerollTime(llTime);
	pMP->Release();
	return hr;
}


STDMETHODIMP
D_CPosPassThru::get_Rate(double * pdRate)
{
	D_IMediaPosition* pMP;
	HRESULT hr = GetPeer(&pMP);
	if (FAILED(hr)) {
	return hr;
	}
	hr = pMP->get_Rate(pdRate);
	pMP->Release();
	return hr;
}


STDMETHODIMP
D_CPosPassThru::put_Rate(double dRate)
{
	if (0.0 == dRate) {
		return E_INVALIDARG;
	}

	D_IMediaPosition* pMP;
	HRESULT hr = GetPeer(&pMP);
	if (FAILED(hr)) {
	return hr;
	}
	hr = pMP->put_Rate(dRate);
	pMP->Release();
	return hr;
}


STDMETHODIMP
D_CPosPassThru::CanSeekForward(LONG *pCanSeekForward)
{
	D_IMediaPosition* pMP;
	HRESULT hr = GetPeer(&pMP);
	if (FAILED(hr)) {
	return hr;
	}
	hr = pMP->CanSeekForward(pCanSeekForward);
	pMP->Release();
	return hr;
}


STDMETHODIMP
D_CPosPassThru::CanSeekBackward(LONG *pCanSeekBackward)
{
	D_IMediaPosition* pMP;
	HRESULT hr = GetPeer(&pMP);
	if (FAILED(hr)) {
	return hr;
	}
	hr = pMP->CanSeekBackward(pCanSeekBackward);
	pMP->Release();
	return hr;
}




D_CMediaPosition::D_CMediaPosition(const TCHAR * name,D_IUnknown * pUnk) :
	D_CUnknown(name, pUnk)
{
}

D_CMediaPosition::D_CMediaPosition(const TCHAR * name,
								D_IUnknown * pUnk,
								HRESULT * phr) :
	D_CUnknown(name, pUnk)
{
	UNREFERENCED_PARAMETER(phr);
}


// expose our interfaces D_IMediaPosition and IUnknown

STDMETHODIMP
D_CMediaPosition::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
	ValidateReadWritePtr(ppv,sizeof(PVOID));
	if (riid == IID_IMEDIAPOSITION) {
	return D_GetInterface( (D_IMediaPosition *) this, ppv);
	} else {
	return D_CUnknown::NonDelegatingQueryInterface(riid, ppv);
	}
}


// return 1 if we support GetTypeInfo

STDMETHODIMP
D_CMediaPosition::GetTypeInfoCount(UINT * pctinfo)
{
	return m_basedisp.GetTypeInfoCount(pctinfo);
}


// attempt to find our type library

STDMETHODIMP
D_CMediaPosition::GetTypeInfo(
  UINT itinfo,
  LCID lcid,
  ITypeInfo ** pptinfo)
{
	return m_basedisp.GetTypeInfo(
		IID_IMEDIAPOSITION,
		itinfo,
		lcid,
		pptinfo);
}


STDMETHODIMP
D_CMediaPosition::GetIDsOfNames(
  REFIID /*riid*/,
  OLECHAR  ** rgszNames,
  UINT cNames,
  LCID lcid,
  DISPID * rgdispid)
{
	return m_basedisp.GetIDsOfNames(
			IID_IMEDIAPOSITION,
			rgszNames,
			cNames,
			lcid,
			rgdispid);
}


STDMETHODIMP
D_CMediaPosition::Invoke(
  DISPID dispidMember,
  REFIID riid,
  LCID lcid,
  WORD wFlags,
  DISPPARAMS * pdispparams,
  VARIANT * pvarResult,
  EXCEPINFO * pexcepinfo,
  UINT * puArgErr)
{
	// this parameter is a dead leftover from an earlier interface
	if (_GUID_NULL/*IID_NULL*/ != riid) {
	return DISP_E_UNKNOWNINTERFACE;
	}

	ITypeInfo * pti;
	HRESULT hr = GetTypeInfo(0, lcid, &pti);

	if (FAILED(hr)) {
	return hr;
	}

	hr = pti->Invoke(
		(D_IMediaPosition *)this,
		dispidMember,
		wFlags,
		pdispparams,
		pvarResult,
		pexcepinfo,
		puArgErr);

	pti->Release();
	return hr;
}



D_CBaseDispatch::~D_CBaseDispatch()
{
	if (m_pti) {
	m_pti->Release();
	}
}


// return 1 if we support GetTypeInfo

STDMETHODIMP
D_CBaseDispatch::GetTypeInfoCount(UINT * pctinfo)
{
	CheckPointer(pctinfo,E_POINTER);
	ValidateReadWritePtr(pctinfo,sizeof(UINT *));
	*pctinfo = 1;
	return S_OK;
}


typedef HRESULT (STDAPICALLTYPE *LPLOADTYPELIB)(
				const OLECHAR FAR *szFile,
				ITypeLib FAR* FAR* pptlib);

typedef HRESULT (STDAPICALLTYPE *LPLOADREGTYPELIB)(REFGUID rguid,
				WORD wVerMajor,
				WORD wVerMinor,
				LCID lcid,
				ITypeLib FAR* FAR* pptlib);

// attempt to find our type library

STDMETHODIMP
D_CBaseDispatch::GetTypeInfo(
  REFIID riid,
  UINT itinfo,
  LCID lcid,
  ITypeInfo ** pptinfo)
{
	SETUP_WIN_API

	CheckPointer(pptinfo,E_POINTER);
	ValidateReadWritePtr(pptinfo,sizeof(ITypeInfo *));
	HRESULT hr;

	*pptinfo = NULL;

	// we only support one type element
	if (0 != itinfo) {
	return TYPE_E_ELEMENTNOTFOUND;
	}

	if (NULL == pptinfo) {
	return E_POINTER;
	}

	// always look for neutral
	if (NULL == m_pti) {

	LPLOADTYPELIB		lpfnLoadTypeLib;
	LPLOADREGTYPELIB	lpfnLoadRegTypeLib;
	ITypeLib		*ptlib;
	HINSTANCE		hInst;

	static const char  szTypeLib[]	  = "LoadTypeLib";
	static const char  szRegTypeLib[] = "LoadRegTypeLib";
	static const WCHAR szControl[]	  = L"control.tlb";

	//
	// Try to get the Ole32Aut.dll module handle.
	//

	hInst = LoadOLEAut32();
	if (hInst == NULL) {
		DWORD dwError = WinAPIData.Win32Func.GetLastErrorFunc();
		return AmHresultFromWin32(dwError);
	}
	lpfnLoadRegTypeLib = (LPLOADREGTYPELIB)GetProcAddress(hInst,
								  szRegTypeLib);
	if (lpfnLoadRegTypeLib == NULL) {
		DWORD dwError = WinAPIData.Win32Func.GetLastErrorFunc();
		return AmHresultFromWin32(dwError);
	}

	hr = (*lpfnLoadRegTypeLib)(LIBID_QUARTZTYPELIB, 1, 0, // version 1.0
					lcid, &ptlib);

	if (FAILED(hr)) {

		// attempt to load directly - this will fill the
		// registry in if it finds it

		lpfnLoadTypeLib = (LPLOADTYPELIB)GetProcAddress(hInst, szTypeLib);
		if (lpfnLoadTypeLib == NULL) {
		DWORD dwError = WinAPIData.Win32Func.GetLastErrorFunc();
		return AmHresultFromWin32(dwError);
		}

		hr = (*lpfnLoadTypeLib)(szControl, &ptlib);
		if (FAILED(hr)) {
		return hr;
		}
	}

	hr = ptlib->GetTypeInfoOfGuid(
			riid,
			&m_pti);

	ptlib->Release();

	if (FAILED(hr)) {
		return hr;
	}
	}

	*pptinfo = m_pti;
	m_pti->AddRef();
	return S_OK;
}


STDMETHODIMP
D_CBaseDispatch::GetIDsOfNames(
  REFIID riid,
  OLECHAR  ** rgszNames,
  UINT cNames,
  LCID lcid,
  DISPID * rgdispid)
{
	// although the IDispatch riid is dead, we use this to pass from
	// the interface implementation class to us the iid we are talking about.

	ITypeInfo * pti;
	HRESULT hr = GetTypeInfo(riid, 0, lcid, &pti);

	if (SUCCEEDED(hr)) {
	hr = pti->GetIDsOfNames(rgszNames, cNames, rgdispid);

	pti->Release();
	}
	return hr;
}







D_CBaseInputPin::D_CBaseInputPin(TCHAR *pObjectName,
				 D_CBaseFilter *pFilter,
				 D_CCritSec *pLock,
				 HRESULT *phr,
				 LPCWSTR pPinName) :
	D_CBasePin(pObjectName, pFilter, pLock, phr, pPinName, D_PINDIR_INPUT),
	m_pAllocator(NULL),
	m_bReadOnly(FALSE),
	m_bFlushing(FALSE)
{
	_MEMSET(&m_SampleProps, 0, sizeof(m_SampleProps));
}
#ifdef UNICODE

D_CBaseInputPin::D_CBaseInputPin(CHAR *pObjectName,
				 D_CBaseFilter *pFilter,
				 D_CCritSec *pLock,
				 HRESULT *phr,
				 LPCWSTR pPinName) :
	D_CBasePin(pObjectName, pFilter, pLock, phr, pPinName, D_PINDIR_INPUT),
	m_pAllocator(NULL),
	m_bReadOnly(FALSE),
	m_bFlushing(FALSE)
{
	_MEMSET(&m_SampleProps, 0, sizeof(m_SampleProps));
}
#endif

/* Destructor releases it's reference count on the default allocator */

D_CBaseInputPin::~D_CBaseInputPin()
{
	if (m_pAllocator != NULL) {
	m_pAllocator->Release();
	m_pAllocator = NULL;
	}
}


// override this to publicise our interfaces
STDMETHODIMP
D_CBaseInputPin::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
	/* Do we know about this interface */

	if (riid == IID_IMEMINPUTPIN) {
		return D_GetInterface((D_IMemInputPin *) this, ppv);
	} else {
		return D_CBasePin::NonDelegatingQueryInterface(riid, ppv);
	}
}


/* Return the allocator interface that this input pin would like the output
	pin to use. NOTE subsequent calls to GetAllocator should all return an
	interface onto the SAME object so we create one object at the start

	Note:
		The allocator is Release()'d on disconnect and replaced on
		NotifyAllocator().

	Override this to provide your own allocator.
*/

STDAPI CreateMemoryAllocator(D_IMemAllocator **ppAllocator)
{
	SETUP_WIN_API

	return WinAPIData.Win32Func.CoCreateInstanceFunc(CLSID_MEMORYALLOCATOR,
							0,
							CLSCTX_INPROC_SERVER,
							IID_IMEMALLOCATOR,
							(void **)ppAllocator);
}

STDMETHODIMP
D_CBaseInputPin::GetAllocator(
	D_IMemAllocator **ppAllocator)
{
	CheckPointer(ppAllocator,E_POINTER);
	ValidateReadWritePtr(ppAllocator,sizeof(D_IMemAllocator *));
	D_CAutoLock cObjectLock(m_pLock);

	if (m_pAllocator == NULL) {
		HRESULT hr = CreateMemoryAllocator(&m_pAllocator);
		if (FAILED(hr)) {
			return hr;
		}
	}
	ASSERT(m_pAllocator != NULL);
	*ppAllocator = m_pAllocator;
	m_pAllocator->AddRef();
	return NOERROR;
}


/* Tell the input pin which allocator the output pin is actually going to use
	Override this if you care - NOTE the locking we do both here and also in
	GetAllocator is unnecessary but derived classes that do something useful
	will undoubtedly have to lock the object so this might help remind people */

STDMETHODIMP
D_CBaseInputPin::NotifyAllocator(
	D_IMemAllocator * pAllocator,
	BOOL bReadOnly)
{
	CheckPointer(pAllocator,E_POINTER);
	ValidateReadPtr(pAllocator,sizeof(D_IMemAllocator));
	D_CAutoLock cObjectLock(m_pLock);

	D_IMemAllocator *pOldAllocator = m_pAllocator;
	pAllocator->AddRef();
	m_pAllocator = pAllocator;

	if (pOldAllocator != NULL) {
		pOldAllocator->Release();
	}

	// the readonly flag indicates whether samples from this allocator should
	// be regarded as readonly - if true, then inplace transforms will not be
	// allowed.
	m_bReadOnly = (BYTE)bReadOnly;
	return NOERROR;
}


HRESULT
D_CBaseInputPin::BreakConnect()
{
	/* We don't need our allocator any more */
	if (m_pAllocator) {
		// Always decommit the allocator because a downstream filter may or
		// may not decommit the connection's allocator.  A memory leak could
		// occur if the allocator is not decommited when a pin is disconnected.
		HRESULT hr = m_pAllocator->Decommit();
		if( FAILED( hr ) ) {
			return hr;
		}

		m_pAllocator->Release();
		m_pAllocator = NULL;
	}

	return S_OK;
}


/* Do something with this media sample - this base class checks to see if the
	format has changed with this media sample and if so checks that the filter
	will accept it, generating a run time error if not. Once we have raised a
	run time error we set a flag so that no more samples will be accepted

	It is important that any filter should override this method and implement
	synchronization so that samples are not processed when the pin is
	disconnected etc
*/

STDMETHODIMP
D_CBaseInputPin::Receive(D_IMediaSample *pSample)
{
	CheckPointer(pSample,E_POINTER);
	ValidateReadPtr(pSample,sizeof(D_IMediaSample));
	ASSERT(pSample);

	HRESULT hr = CheckStreaming();
	if (S_OK != hr) {
		return hr;
	}



	/* Check for D_IMediaSample2 */
	D_IMediaSample2 *pSample2;
	if (SUCCEEDED(pSample->QueryInterface(IID_IMEDIASAMPLE2, (void **)&pSample2))) {
		hr = pSample2->GetProperties(sizeof(m_SampleProps), (PBYTE)&m_SampleProps);
		pSample2->Release();
		if (FAILED(hr)) {
			return hr;
		}
	} else {
		/*  Get the properties the hard way */
		m_SampleProps.cbData = sizeof(m_SampleProps);
		m_SampleProps.dwTypeSpecificFlags = 0;
		m_SampleProps.dwStreamId = AM_STREAM_MEDIA;
		m_SampleProps.dwSampleFlags = 0;
		if (S_OK == pSample->IsDiscontinuity()) {
			m_SampleProps.dwSampleFlags |= AM_SAMPLE_DATADISCONTINUITY;
		}
		if (S_OK == pSample->IsPreroll()) {
			m_SampleProps.dwSampleFlags |= AM_SAMPLE_PREROLL;
		}
		if (S_OK == pSample->IsSyncPoint()) {
			m_SampleProps.dwSampleFlags |= AM_SAMPLE_SPLICEPOINT;
		}
		if (SUCCEEDED(pSample->GetTime(&m_SampleProps.tStart,
										&m_SampleProps.tStop))) {
			m_SampleProps.dwSampleFlags |= AM_SAMPLE_TIMEVALID |
											AM_SAMPLE_STOPVALID;
		}
		if (S_OK == pSample->GetMediaType(&m_SampleProps.pMediaType)) {
			m_SampleProps.dwSampleFlags |= AM_SAMPLE_TYPECHANGED;
		}
		pSample->GetPointer(&m_SampleProps.pbBuffer);
		m_SampleProps.lActual = pSample->GetActualDataLength();
		m_SampleProps.cbBuffer = pSample->GetSize();
	}

	/* Has the format changed in this sample */

	if (!(m_SampleProps.dwSampleFlags & AM_SAMPLE_TYPECHANGED)) {
		return NOERROR;
	}

	/* Check the derived class accepts this format */
	/* This shouldn't fail as the source must call QueryAccept first */

	hr = CheckMediaType((D_CMediaType *)m_SampleProps.pMediaType);

	if (hr == NOERROR) {
		return NOERROR;
	}

	/* Raise a runtime error if we fail the media type */

	m_bRunTimeError = TRUE;
	EndOfStream();
	m_pFilter->NotifyEvent(EC_ERRORABORT,(LONG_PTR)VFW_E_TYPE_NOT_ACCEPTED,0);
	return VFW_E_INVALIDMEDIATYPE;
}


/*  Receive multiple samples */
STDMETHODIMP
D_CBaseInputPin::ReceiveMultiple (
	D_IMediaSample **pSamples,
	long nSamples,
	long *nSamplesProcessed)
{
	CheckPointer(pSamples,E_POINTER);
	ValidateReadPtr(pSamples,nSamples * sizeof(D_IMediaSample *));

	HRESULT hr = S_OK;
	*nSamplesProcessed = 0;
	while (nSamples-- > 0) {
		 hr = Receive(pSamples[*nSamplesProcessed]);

		 /*  S_FALSE means don't send any more */
		 if (hr != S_OK) {
			 break;
		 }
		 (*nSamplesProcessed)++;
	}
	return hr;
}

/*  See if Receive() might block */
STDMETHODIMP
D_CBaseInputPin::ReceiveCanBlock()
{
	/*  Ask all the output pins if they block
		If there are no output pin assume we do block
	*/
	int cPins = m_pFilter->GetPinCount();
	int cOutputPins = 0;
	for (int c = 0; c < cPins; c++) {
		D_CBasePin *pPin = m_pFilter->GetPin(c);
		D_PIN_DIRECTION pd;
		HRESULT hr = pPin->QueryDirection(&pd);
		if (FAILED(hr)) {
			return hr;
		}

		if (pd == D_PINDIR_OUTPUT) {

			D_IPin *pConnected;
			hr = pPin->ConnectedTo(&pConnected);
			if (SUCCEEDED(hr)) {
				ASSERT(pConnected != NULL);
				cOutputPins++;
				D_IMemInputPin *pInputPin;
				hr = pConnected->QueryInterface(
											  IID_IMEMINPUTPIN,
											  (void **)&pInputPin);
				pConnected->Release();
				if (SUCCEEDED(hr)) {
					hr = pInputPin->ReceiveCanBlock();
					pInputPin->Release();
					if (hr != S_FALSE) {
						return S_OK;
					}
				} else {
					/*  There's a transport we don't understand here */
					return S_OK;
				}
			}
		}
	}
	return cOutputPins == 0 ? S_OK : S_FALSE;
}

// Default handling for BeginFlush - call at the beginning
// of your implementation (makes sure that all Receive calls
// fail). After calling this, you need to free any queued data
// and then call downstream.
STDMETHODIMP
D_CBaseInputPin::BeginFlush(void)
{
	//  BeginFlush is NOT synchronized with streaming but is part of
	//  a control action - hence we synchronize with the filter
	D_CAutoLock lck(m_pLock);

	// if we are already in mid-flush, this is probably a mistake
	// though not harmful - try to pick it up for now so I can think about it
	ASSERT(!m_bFlushing);

	// first thing to do is ensure that no further Receive calls succeed
	m_bFlushing = TRUE;

	// now discard any data and call downstream - must do that
	// in derived classes
	return S_OK;
}

// default handling for EndFlush - call at end of your implementation
// - before calling this, ensure that there is no queued data and no thread
// pushing any more without a further receive, then call downstream,
// then call this method to clear the m_bFlushing flag and re-enable
// receives
STDMETHODIMP
D_CBaseInputPin::EndFlush(void)
{
	//  Endlush is NOT synchronized with streaming but is part of
	//  a control action - hence we synchronize with the filter
	D_CAutoLock lck(m_pLock);

	// almost certainly a mistake if we are not in mid-flush
	ASSERT(m_bFlushing);

	// before calling, sync with pushing thread and ensure
	// no more data is going downstream, then call EndFlush on
	// downstream pins.

	// now re-enable Receives
	m_bFlushing = FALSE;

	// No more errors
	m_bRunTimeError = FALSE;

	return S_OK;
}


STDMETHODIMP
D_CBaseInputPin::Notify(D_IBaseFilter * pSender, D_Quality q)
{
	UNREFERENCED_PARAMETER(q);
	CheckPointer(pSender,E_POINTER);
	ValidateReadPtr(pSender,sizeof(D_IBaseFilter));
	DbgBreak("IQuality::Notify called on an input pin");
	return NOERROR;
} // Notify

/* Free up or unprepare allocator's memory, this is called through
	D_IMediaFilter which is responsible for locking the object first */

HRESULT
D_CBaseInputPin::Inactive(void)
{
	m_bRunTimeError = FALSE;
	if (m_pAllocator == NULL) {
		return VFW_E_NO_ALLOCATOR;
	}

	m_bFlushing = FALSE;

	return m_pAllocator->Decommit();
}

// what requirements do we have of the allocator - override if you want
// to support other people's allocators but need a specific alignment
// or prefix.
STDMETHODIMP
D_CBaseInputPin::GetAllocatorRequirements(D_ALLOCATOR_PROPERTIES*pProps)
{
	UNREFERENCED_PARAMETER(pProps);
	return E_NOTIMPL;
}

//  Check if it's OK to process data
//
HRESULT
D_CBaseInputPin::CheckStreaming()
{
	//  Shouldn't be able to get any data if we're not connected!
	ASSERT(IsConnected());

	//  Don't process stuff in Stopped state
	if (IsStopped()) {
		return VFW_E_WRONG_STATE;
	}
	if (m_bFlushing) {
		return S_FALSE;
	}
	if (m_bRunTimeError) {
		return VFW_E_RUNTIME_ERROR;
	}
	return S_OK;
}

// Pass on the D_Quality notification q to
// a. Our QualityControl sink (if we have one) or else
// b. to our upstream filter
// and if that doesn't work, throw it away with a bad return code
HRESULT
D_CBaseInputPin::PassNotify(D_Quality& q)
{
	// We pass the message on, which means that we find the quality sink
	// for our input pin and send it there

	DbgLog((LOG_TRACE,3,TEXT("Passing D_Quality notification through transform")));
	if (m_pQSink!=NULL) {
		return m_pQSink->Notify(m_pFilter, q);
	} else {
		// no sink set, so pass it upstream
		HRESULT hr;
		D_IQualityControl * pIQC;

		hr = VFW_E_NOT_FOUND;					// default
		if (m_Connected) {
			m_Connected->QueryInterface(IID_IQUALITYCONTROL, (void**)&pIQC);

			if (pIQC!=NULL) {
				hr = pIQC->Notify(m_pFilter, q);
				pIQC->Release();
			}
		}
		return hr;
	}

} // PassNotify








D_CBasePin::D_CBasePin(TCHAR *pObjectName,
			D_CBaseFilter *pFilter,
			D_CCritSec *pLock,
			HRESULT * /* phr */,
			LPCWSTR pName,
			D_PIN_DIRECTION dir) :
	D_CUnknown( pObjectName, NULL ),
	m_pFilter(pFilter),
	m_pLock(pLock),
	m_pName(NULL),
	m_Connected(NULL),
	m_dir(dir),
	m_bRunTimeError(FALSE),
	m_pQSink(NULL),
	m_TypeVersion(1),
	m_tStart(),
	m_tStop(MAX_TIME),
	m_bCanReconnectWhenActive(false),
	m_bTryMyTypesFirst(false),
	m_dRate(1.0)
{
	//  WARNING - pFilter is often not a properly constituted object at
	//	this state (in particular QueryInterface may not work) - this
	//	is because its owner is often its containing object and we
	//	have been called from the containing object's constructor so
	//	the filter's owner has not yet had its D_CUnknown constructor
	//	called
	
	SETUP_WIN_API

	ASSERT(pFilter != NULL);
	ASSERT(pLock != NULL);

	if (pName) {
		DWORD nameLen = lstrlenWFunc((WORD const *)pName)+1;
		m_pName = new WCHAR[nameLen];
		if (m_pName) {
			_MEMCPY(m_pName, pName, nameLen*sizeof(WCHAR));
		}
	}

#ifdef DEBUG
	m_cRef = 0;
#endif
}

#ifdef UNICODE

D_CBasePin::D_CBasePin(CHAR *pObjectName,
			D_CBaseFilter *pFilter,
			D_CCritSec *pLock,
			HRESULT * /*phr*/,
			LPCWSTR pName,
			D_PIN_DIRECTION dir) :
	D_CUnknown( pObjectName, NULL ),
	m_pFilter(pFilter),
	m_pLock(pLock),
	m_pName(NULL),
	m_Connected(NULL),
	m_dir(dir),
	m_bRunTimeError(FALSE),
	m_pQSink(NULL),
	m_TypeVersion(1),
	m_tStart(),
	m_tStop(MAX_TIME),
	m_bCanReconnectWhenActive(false),
	m_bTryMyTypesFirst(false),
	m_dRate(1.0)
{
	SETUP_WIN_API

	/*  WARNING - pFilter is often not a properly constituted object at
		this state (in particular QueryInterface may not work) - this
		is because its owner is often its containing object and we
		have been called from the containing object's constructor so
		the filter's owner has not yet had its D_CUnknown constructor
		called
	*/

	ASSERT(pFilter != NULL);
	ASSERT(pLock != NULL);

	if (pName) {
		DWORD nameLen = ( DWORD )( lstrlenWFunc((WORD const *)pName)+1 );
		m_pName = new WCHAR[nameLen];
		if (m_pName) {
			_MEMCPY(m_pName, pName, nameLen*sizeof(WCHAR));
		}
	}

#ifdef DEBUG
	m_cRef = 0;
#endif
}
#endif

/* Destructor since a connected pin holds a reference count on us there is
	no way that we can be deleted unless we are not currently connected */

D_CBasePin::~D_CBasePin()
{

	//  We don't call disconnect because if the filter is going away
	//  all the pins must have a reference count of zero so they must
	//  have been disconnected anyway - (but check the assumption)
	ASSERT(m_Connected == FALSE);

	delete[] m_pName;

	// check the internal reference count is consistent
	ASSERT(m_cRef == 0);
}


/* Override this to say what interfaces we support and where */

STDMETHODIMP
D_CBasePin::NonDelegatingQueryInterface(REFIID riid, void ** ppv)
{
	/* Do we have this interface */

	if (riid == IID_IPIN) {
		return D_GetInterface((D_IPin *) this, ppv);
	} else if (riid == IID_IQUALITYCONTROL) {
		return D_GetInterface((D_IQualityControl *) this, ppv);
	} else {
		return D_CUnknown::NonDelegatingQueryInterface(riid, ppv);
	}
}


/* Override to increment the owning filter's reference count */

STDMETHODIMP_(ULONG)
D_CBasePin::NonDelegatingAddRef()
{
	ASSERT(InterlockedIncrement(&m_cRef) > 0);
	return m_pFilter->AddRef();
}


/* Override to decrement the owning filter's reference count */

STDMETHODIMP_(ULONG)
D_CBasePin::NonDelegatingRelease()
{
	ASSERT(InterlockedDecrement(&m_cRef) >= 0);
	return m_pFilter->Release();
}


/* Displays pin connection information */

#ifdef DEBUG
void
D_CBasePin::DisplayPinInfo(D_IPin *pReceivePin)
{
	SETUP_WIN_API

	if (DbgCheckModuleLevel(LOG_TRACE, CONNECT_TRACE_LEVEL)) {
		D_PIN_INFO ConnectPinInfo;
		D_PIN_INFO ReceivePinInfo;

		if (FAILED(QueryPinInfo(&ConnectPinInfo))) {
			lstrcpyWFunc((WORD *)ConnectPinInfo.achName, (WORD const *)L"Bad Pin");
		} else {
			QueryPinInfoReleaseFilter(ConnectPinInfo);
		}

		if (FAILED(pReceivePin->QueryPinInfo(&ReceivePinInfo))) {
			lstrcpyWFunc((WORD *)ReceivePinInfo.achName, (WORD const *)L"Bad Pin");
		} else {
			QueryPinInfoReleaseFilter(ReceivePinInfo);
		}

		DbgLog((LOG_TRACE, CONNECT_TRACE_LEVEL, TEXT("Trying to connect Pins :")));
		DbgLog((LOG_TRACE, CONNECT_TRACE_LEVEL, TEXT("	<%ls>"), ConnectPinInfo.achName));
		DbgLog((LOG_TRACE, CONNECT_TRACE_LEVEL, TEXT("	<%ls>"), ReceivePinInfo.achName));
	}
}
#endif


/* Displays general information on the pin media type */

#ifdef DEBUG
void D_CBasePin::DisplayTypeInfo(D_IPin *pPin, const D_CMediaType *pmt)
{
	UNREFERENCED_PARAMETER(pPin);
	if (DbgCheckModuleLevel(LOG_TRACE, CONNECT_TRACE_LEVEL)) {
		DbgLog((LOG_TRACE, CONNECT_TRACE_LEVEL, TEXT("Trying media type:")));
		DbgLog((LOG_TRACE, CONNECT_TRACE_LEVEL, TEXT("	major type:  %hs"),
				GuidNames[*pmt->Type()]));
		DbgLog((LOG_TRACE, CONNECT_TRACE_LEVEL, TEXT("	sub type  :  %hs"),
				GuidNames[*pmt->Subtype()]));
	}
}
#endif

/* Asked to connect to a pin. A pin is always attached to an owning filter
	object so we always delegate our locking to that object. We first of all
	retrieve a media type enumerator for the input pin and see if we accept
	any of the formats that it would ideally like, failing that we retrieve
	our enumerator and see if it will accept any of our preferred types */

STDMETHODIMP
D_CBasePin::Connect(
	D_IPin * pReceivePin,
	const D_AM_MEDIA_TYPE *pmt	// optional media type
)
{
	CheckPointer(pReceivePin,E_POINTER);
	ValidateReadPtr(pReceivePin,sizeof(D_IPin));
	D_CAutoLock cObjectLock(m_pLock);
	DisplayPinInfo(pReceivePin);

	/* See if we are already connected */

	if (m_Connected) {
		DbgLog((LOG_TRACE, CONNECT_TRACE_LEVEL, TEXT("Already connected")));
		return VFW_E_ALREADY_CONNECTED;
	}

	/* See if the filter is active */
	if (!IsStopped() && !m_bCanReconnectWhenActive) {
		return VFW_E_NOT_STOPPED;
	}


	// Find a mutually agreeable media type -
	// Pass in the template media type. If this is partially specified,
	// each of the enumerated media types will need to be checked against
	// it. If it is non-null and fully specified, we will just try to connect
	// with this.

	const D_CMediaType * ptype = (D_CMediaType*)pmt;
	HRESULT hr = AgreeMediaType(pReceivePin, ptype);
	if (FAILED(hr)) {
		DbgLog((LOG_TRACE, CONNECT_TRACE_LEVEL, TEXT("Failed to agree type")));

		// Since the procedure is already returning an error code, there
		// is nothing else this function can do to report the error.
		EXECUTE_ASSERT( SUCCEEDED( BreakConnect() ) );


		return hr;
	}

	DbgLog((LOG_TRACE, CONNECT_TRACE_LEVEL, TEXT("Connection succeeded")));


	return NOERROR;
}

// given a specific media type, attempt a connection (includes
// checking that the type is acceptable to this pin)
HRESULT
D_CBasePin::AttemptConnection(
	D_IPin* pReceivePin,	  // connect to this pin
	const D_CMediaType* pmt	// using this type
)
{
	// The caller should hold the filter lock becasue this function
	// uses m_Connected.  The caller should also hold the filter lock
	// because this function calls SetMediaType(), IsStopped() and
	// CompleteConnect().
	ASSERT(CritCheckIn(m_pLock));

	// Check that the connection is valid  -- need to do this for every
	// connect attempt since BreakConnect will undo it.
	HRESULT hr = CheckConnect(pReceivePin);
	if (FAILED(hr)) {
		DbgLog((LOG_TRACE, CONNECT_TRACE_LEVEL, TEXT("CheckConnect failed")));

		// Since the procedure is already returning an error code, there
		// is nothing else this function can do to report the error.
		EXECUTE_ASSERT( SUCCEEDED( BreakConnect() ) );

		return hr;
	}

	DisplayTypeInfo(pReceivePin, pmt);

	/* Check we will accept this media type */

	hr = CheckMediaType(pmt);
	if (hr == NOERROR) {

		/*  Make ourselves look connected otherwise ReceiveConnection
			may not be able to complete the connection
		*/
		m_Connected = pReceivePin;
		m_Connected->AddRef();
		hr = SetMediaType(pmt);
		if (SUCCEEDED(hr)) {
			/* See if the other pin will accept this type */

			hr = pReceivePin->ReceiveConnection((D_IPin *)this, pmt);
			if (SUCCEEDED(hr)) {
				/* Complete the connection */

				hr = CompleteConnect(pReceivePin);
				if (SUCCEEDED(hr)) {
					return hr;
				} else {
					DbgLog((LOG_TRACE,
							CONNECT_TRACE_LEVEL,
							TEXT("Failed to complete connection")));
					pReceivePin->Disconnect();
				}
			}
		}
	} else {
		// we cannot use this media type

		// return a specific media type error if there is one
		// or map a general failure code to something more helpful
		// (in particular S_FALSE gets changed to an error code)
		if (SUCCEEDED(hr) ||
			(hr == E_FAIL) ||
			(hr == E_INVALIDARG)) {
			hr = VFW_E_TYPE_NOT_ACCEPTED;
		}
	}

	// BreakConnect and release any connection here in case CheckMediaType
	// failed, or if we set anything up during a call back during
	// ReceiveConnection.

	// Since the procedure is already returning an error code, there
	// is nothing else this function can do to report the error.
	EXECUTE_ASSERT( SUCCEEDED( BreakConnect() ) );

	/*  If failed then undo our state */
	if (m_Connected) {
		m_Connected->Release();
		m_Connected = NULL;
	}

	return hr;
}

HRESULT WINAPI CopyMediaType(D_AM_MEDIA_TYPE *pmtTarget, const D_AM_MEDIA_TYPE *pmtSource)
{
	SETUP_WIN_API

	//  We'll leak if we copy onto one that already exists - there's one
	//  case we can check like that - copying to itself.
	ASSERT(pmtSource != pmtTarget);
	*pmtTarget = *pmtSource;
	if (pmtSource->cbFormat != 0) {
		ASSERT(pmtSource->pbFormat != NULL);
		pmtTarget->pbFormat = (PBYTE)WinAPIData.Win32Func.CoTaskMemAllocFunc(pmtSource->cbFormat);
		if (pmtTarget->pbFormat == NULL) {
			pmtTarget->cbFormat = 0;
			return E_OUTOFMEMORY;
		} else {
			_MEMCPY((PVOID)pmtTarget->pbFormat, (PVOID)pmtSource->pbFormat,
						pmtTarget->cbFormat);
		}
	}
	if (pmtTarget->pUnk != NULL) {
		pmtTarget->pUnk->AddRef();
	}

	return S_OK;
}

void WINAPI FreeMediaType(D_AM_MEDIA_TYPE& mt)
{
	SETUP_WIN_API

	if (mt.cbFormat != 0) {
		WinAPIData.Win32Func.CoTaskMemFreeFunc((PVOID)mt.pbFormat);

		// Strictly unnecessary but tidier
		mt.cbFormat = 0;
		mt.pbFormat = NULL;
	}
	if (mt.pUnk != NULL) {
		mt.pUnk->Release();
		mt.pUnk = NULL;
	}
}

void WINAPI DeleteMediaType(D_AM_MEDIA_TYPE *pmt)
{
	SETUP_WIN_API

	// allow NULL pointers for coding simplicity

	if (pmt == NULL) {
		return;
	}

	FreeMediaType(*pmt);
	WinAPIData.Win32Func.CoTaskMemFreeFunc((PVOID)pmt);
}

/* Given an enumerator we cycle through all the media types it proposes and
	firstly suggest them to our derived pin class and if that succeeds try
	them with the pin in a ReceiveConnection call. This means that if our pin
	proposes a media type we still check in here that we can support it. This
	is deliberate so that in simple cases the enumerator can hold all of the
	media types even if some of them are not really currently available */

HRESULT D_CBasePin::TryMediaTypes(
	D_IPin *pReceivePin,
	const D_CMediaType *pmt,
	D_IEnumMediaTypes *pEnum)
{
	/* Reset the current enumerator position */

	HRESULT hr = pEnum->Reset();
	if (FAILED(hr)) {
		return hr;
	}

	D_CMediaType *pMediaType = NULL;
	ULONG ulMediaCount = 0;

	// attempt to remember a specific error code if there is one
	HRESULT hrFailure = S_OK;

	for (;;) {

		/* Retrieve the next media type NOTE each time round the loop the
			enumerator interface will allocate another D_AM_MEDIA_TYPE structure
			If we are successful then we copy it into our output object, if
			not then we must delete the memory allocated before returning */

		hr = pEnum->Next(1, (D_AM_MEDIA_TYPE**)&pMediaType,&ulMediaCount);
		if (hr != S_OK) {
			if (S_OK == hrFailure) {
				hrFailure = VFW_E_NO_ACCEPTABLE_TYPES;
			}
			return hrFailure;
		}


		ASSERT(ulMediaCount == 1);
		ASSERT(pMediaType);

		// check that this matches the partial type (if any)

		if ((pmt == NULL) ||
			pMediaType->MatchesPartial(pmt)) {

			hr = AttemptConnection(pReceivePin, pMediaType);

			// attempt to remember a specific error code
			if (FAILED(hr) &&
			SUCCEEDED(hrFailure) &&
			(hr != E_FAIL) &&
			(hr != E_INVALIDARG) &&
			(hr != VFW_E_TYPE_NOT_ACCEPTED)) {
				hrFailure = hr;
			}
		} else {
			hr = VFW_E_NO_ACCEPTABLE_TYPES;
		}

		DeleteMediaType(pMediaType);

		if (S_OK == hr) {
			return hr;
		}
	}
}


/* This is called to make the connection, including the taask of finding
	a media type for the pin connection. pmt is the proposed media type
	from the Connect call: if this is fully specified, we will try that.
	Otherwise we enumerate and try all the input pin's types first and
	if that fails we then enumerate and try all our preferred media types.
	For each media type we check it against pmt (if non-null and partially
	specified) as well as checking that both pins will accept it.
 */

HRESULT D_CBasePin::AgreeMediaType(
	D_IPin *pReceivePin,
	const D_CMediaType *pmt)
{
	ASSERT(pReceivePin);
	D_IEnumMediaTypes *pEnumMediaTypes = NULL;

	// if the media type is fully specified then use that
	if ( (pmt != NULL) && (!pmt->IsPartiallySpecified())) {

		// if this media type fails, then we must fail the connection
		// since if pmt is nonnull we are only allowed to connect
		// using a type that matches it.

		return AttemptConnection(pReceivePin, pmt);
	}


	/* Try the other pin's enumerator */

	HRESULT hrFailure = VFW_E_NO_ACCEPTABLE_TYPES;

	for (int i = 0; i < 2; i++) {
		HRESULT hr;
		if (i == (int)m_bTryMyTypesFirst) {
			hr = pReceivePin->EnumMediaTypes(&pEnumMediaTypes);
		} else {
			hr = EnumMediaTypes(&pEnumMediaTypes);
		}
		if (SUCCEEDED(hr)) {
			ASSERT(pEnumMediaTypes);
			hr = TryMediaTypes(pReceivePin,pmt,pEnumMediaTypes);
			pEnumMediaTypes->Release();
			if (SUCCEEDED(hr)) {
				return NOERROR;
			} else {
				// try to remember specific error codes if there are any
				if ((hr != E_FAIL) &&
					(hr != E_INVALIDARG) &&
					(hr != VFW_E_TYPE_NOT_ACCEPTED)) {
					hrFailure = hr;
				}
			}
		}
	}

	return hrFailure;
}


/* Called when we want to complete a connection to another filter. Failing
	this will also fail the connection and disconnect the other pin as well */

HRESULT
D_CBasePin::CompleteConnect(D_IPin *pReceivePin)
{
	UNREFERENCED_PARAMETER(pReceivePin);
	return NOERROR;
}


/* This is called to set the format for a pin connection - CheckMediaType
	will have been called to check the connection format and if it didn't
	return an error code then this (virtual) function will be invoked */

HRESULT
D_CBasePin::SetMediaType(const D_CMediaType *pmt)
{
	HRESULT hr = m_mt.Set(*pmt);
	if (FAILED(hr)) {
		return hr;
	}

	return NOERROR;
}


/* This is called during Connect() to provide a virtual method that can do
	any specific check needed for connection such as QueryInterface. This
	base class method just checks that the pin directions don't match */

HRESULT
D_CBasePin::CheckConnect(D_IPin * pPin)
{
	/* Check that pin directions DONT match */

	D_PIN_DIRECTION pd;
	pPin->QueryDirection(&pd);

	ASSERT((pd == D_PINDIR_OUTPUT) || (pd == D_PINDIR_INPUT));
	ASSERT((m_dir == D_PINDIR_OUTPUT) || (m_dir == D_PINDIR_INPUT));

	// we should allow for non-input and non-output connections?
	if (pd == m_dir) {
		return VFW_E_INVALID_DIRECTION;
	}
	return NOERROR;
}


/* This is called when we realise we can't make a connection to the pin and
	must undo anything we did in CheckConnect - override to release QIs done */

HRESULT
D_CBasePin::BreakConnect()
{
	return NOERROR;
}


/* Called normally by an output pin on an input pin to try and establish a
	connection.
*/

STDMETHODIMP
D_CBasePin::ReceiveConnection(
	D_IPin * pConnector,	  // this is the pin who we will connect to
	const D_AM_MEDIA_TYPE *pmt	// this is the media type we will exchange
)
{
	CheckPointer(pConnector,E_POINTER);
	CheckPointer(pmt,E_POINTER);
	ValidateReadPtr(pConnector,sizeof(D_IPin));
	ValidateReadPtr(pmt,sizeof(D_AM_MEDIA_TYPE));
	D_CAutoLock cObjectLock(m_pLock);

	/* Are we already connected */
	if (m_Connected) {
		return VFW_E_ALREADY_CONNECTED;
	}

	/* See if the filter is active */
	if (!IsStopped() && !m_bCanReconnectWhenActive) {
		return VFW_E_NOT_STOPPED;
	}

	HRESULT hr = CheckConnect(pConnector);
	if (FAILED(hr)) {
		// Since the procedure is already returning an error code, there
		// is nothing else this function can do to report the error.
		EXECUTE_ASSERT( SUCCEEDED( BreakConnect() ) );


		return hr;
	}

	/* Ask derived class if this media type is ok */

	D_CMediaType * pcmt = (D_CMediaType*) pmt;
	hr = CheckMediaType(pcmt);
	if (hr != NOERROR) {
		// no -we don't support this media type

		// Since the procedure is already returning an error code, there
		// is nothing else this function can do to report the error.
		EXECUTE_ASSERT( SUCCEEDED( BreakConnect() ) );

		// return a specific media type error if there is one
		// or map a general failure code to something more helpful
		// (in particular S_FALSE gets changed to an error code)
		if (SUCCEEDED(hr) ||
			(hr == E_FAIL) ||
			(hr == E_INVALIDARG)) {
			hr = VFW_E_TYPE_NOT_ACCEPTED;
		}


		return hr;
	}

	/* Complete the connection */

	m_Connected = pConnector;
	m_Connected->AddRef();
	hr = SetMediaType(pcmt);
	if (SUCCEEDED(hr)) {
		hr = CompleteConnect(pConnector);
		if (SUCCEEDED(hr)) {


			return NOERROR;
		}
	}

	DbgLog((LOG_TRACE, CONNECT_TRACE_LEVEL, TEXT("Failed to set the media type or failed to complete the connection.")));
	m_Connected->Release();
	m_Connected = NULL;

	// Since the procedure is already returning an error code, there
	// is nothing else this function can do to report the error.
	EXECUTE_ASSERT( SUCCEEDED( BreakConnect() ) );


	return hr;
}


/* Called when we want to terminate a pin connection */

STDMETHODIMP
D_CBasePin::Disconnect()
{
	D_CAutoLock cObjectLock(m_pLock);

	/* See if the filter is active */
	if (!IsStopped()) {
		return VFW_E_NOT_STOPPED;
	}

	return DisconnectInternal();
}

STDMETHODIMP
D_CBasePin::DisconnectInternal()
{
	ASSERT(CritCheckIn(m_pLock));

	if (m_Connected) {
		HRESULT hr = BreakConnect();
		if( FAILED( hr ) ) {


			// There is usually a bug in the program if BreakConnect() fails.
			DbgBreak( "WARNING: BreakConnect() failed in D_CBasePin::Disconnect()." );
			return hr;
		}

		m_Connected->Release();
		m_Connected = NULL;


		return S_OK;
	} else {
		// no connection - not an error


		return S_FALSE;
	}
}


/* Return an AddRef()'d pointer to the connected pin if there is one */
STDMETHODIMP
D_CBasePin::ConnectedTo(
	D_IPin **ppPin
)
{
	CheckPointer(ppPin,E_POINTER);
	ValidateReadWritePtr(ppPin,sizeof(D_IPin *));
	//
	//  It's pointless to lock here.
	//  The caller should ensure integrity.
	//

	D_IPin *pPin = m_Connected;
	*ppPin = pPin;
	if (pPin != NULL) {
		pPin->AddRef();
		return S_OK;
	} else {
		ASSERT(*ppPin == NULL);
		return VFW_E_NOT_CONNECTED;
	}
}

/* Return the media type of the connection */
STDMETHODIMP
D_CBasePin::ConnectionMediaType(
	D_AM_MEDIA_TYPE *pmt
)
{
	CheckPointer(pmt,E_POINTER);
	ValidateReadWritePtr(pmt,sizeof(D_AM_MEDIA_TYPE));
	D_CAutoLock cObjectLock(m_pLock);

	/*  Copy constructor of m_mt allocates the memory */
	if (IsConnected()) {
		CopyMediaType( pmt, &m_mt );
		return S_OK;
	} else {
		((D_CMediaType *)pmt)->InitMediaType();
		return VFW_E_NOT_CONNECTED;
	}
}

/* Return information about the filter we are connect to */

STDMETHODIMP
D_CBasePin::QueryPinInfo(
	D_PIN_INFO * pInfo
)
{
	SETUP_WIN_API

	CheckPointer(pInfo,E_POINTER);
	ValidateReadWritePtr(pInfo,sizeof(D_PIN_INFO));

	pInfo->pFilter = m_pFilter;
	if (m_pFilter) {
		m_pFilter->AddRef();
	}

	if (m_pName) {
		lstrcpynWFunc((WORD *)pInfo->achName, (WORD const *)m_pName, sizeof(pInfo->achName)/sizeof(WCHAR));
	} else {
		pInfo->achName[0] = L'\0';
	}

	pInfo->dir = m_dir;

	return NOERROR;
}

STDMETHODIMP
D_CBasePin::QueryDirection(
	D_PIN_DIRECTION * pPinDir
)
{
	CheckPointer(pPinDir,E_POINTER);
	ValidateReadWritePtr(pPinDir,sizeof(D_PIN_DIRECTION));

	*pPinDir = m_dir;
	return NOERROR;
}

// Default QueryId to return the pin's name
STDMETHODIMP
D_CBasePin::QueryId(
	LPWSTR * Id
)
{
	//  We're not going away because someone's got a pointer to us
	//  so there's no need to lock

	return D_AMGetWideString(Name(), Id);
}

/* Does this pin support this media type WARNING this interface function does
	not lock the main object as it is meant to be asynchronous by nature - if
	the media types you support depend on some internal state that is updated
	dynamically then you will need to implement locking in a derived class */

STDMETHODIMP
D_CBasePin::QueryAccept(
	const D_AM_MEDIA_TYPE *pmt
)
{
	CheckPointer(pmt,E_POINTER);
	ValidateReadPtr(pmt,sizeof(D_AM_MEDIA_TYPE));

	/* The CheckMediaType method is valid to return error codes if the media
		type is horrible, an example might be E_INVALIDARG. What we do here
		is map all the error codes into either S_OK or S_FALSE regardless */

	HRESULT hr = CheckMediaType((D_CMediaType*)pmt);
	if (FAILED(hr)) {
		return S_FALSE;
	}
	// note that the only defined success codes should be S_OK and S_FALSE...
	return hr;
}


/* This can be called to return an enumerator for the pin's list of preferred
	media types. An input pin is not obliged to have any preferred formats
	although it can do. For example, the window renderer has a preferred type
	which describes a video image that matches the current window size. All
	output pins should expose at least one preferred format otherwise it is
	possible that neither pin has any types and so no connection is possible */

STDMETHODIMP
D_CBasePin::EnumMediaTypes(
	D_IEnumMediaTypes **ppEnum
)
{
	CheckPointer(ppEnum,E_POINTER);
	ValidateReadWritePtr(ppEnum,sizeof(D_IEnumMediaTypes *));

	/* Create a new ref counted enumerator */

	*ppEnum = new D_CEnumMediaTypes(this,
							  NULL);

	if (*ppEnum == NULL) {
		return E_OUTOFMEMORY;
	}

	return NOERROR;
}



/* This is a virtual function that returns a media type corresponding with
	place iPosition in the list. This base class simply returns an error as
	we support no media types by default but derived classes should override */

HRESULT D_CBasePin::GetMediaType(int iPosition, D_CMediaType *pMediaType)
{
	UNREFERENCED_PARAMETER(iPosition);
	UNREFERENCED_PARAMETER(pMediaType);
	return E_UNEXPECTED;
}


/* This is a virtual function that returns the current media type version.
	The base class initialises the media type enumerators with the value 1
	By default we always returns that same value. A Derived class may change
	the list of media types available and after doing so it should increment
	the version either in a method derived from this, or more simply by just
	incrementing the m_TypeVersion base pin variable. The type enumerators
	call this when they want to see if their enumerations are out of date */

LONG D_CBasePin::GetMediaTypeVersion()
{
	return m_TypeVersion;
}


/* Increment the cookie representing the current media type version */

void D_CBasePin::IncrementTypeVersion()
{
	InterlockedIncrement(&m_TypeVersion);
}


/* Called by D_IMediaFilter implementation when the state changes from Stopped
	to either paused or running and in derived classes could do things like
	commit memory and grab hardware resource (the default is to do nothing) */

HRESULT
D_CBasePin::Active(void)
{
	return NOERROR;
}

/* Called by D_IMediaFilter implementation when the state changes from
	to either paused to running and in derived classes could do things like
	commit memory and grab hardware resource (the default is to do nothing) */

HRESULT
D_CBasePin::Run(D_REFERENCE_TIME tStart)
{
	UNREFERENCED_PARAMETER(tStart);
	return NOERROR;
}


/* Also called by the D_IMediaFilter implementation when the state changes to
	Stopped at which point you should decommit allocators and free hardware
	resources you grabbed in the Active call (default is also to do nothing) */

HRESULT
D_CBasePin::Inactive(void)
{
	m_bRunTimeError = FALSE;
	return NOERROR;
}


// Called when no more data will arrive
STDMETHODIMP
D_CBasePin::EndOfStream(void)
{
	return S_OK;
}


STDMETHODIMP
D_CBasePin::SetSink(D_IQualityControl * piqc)
{
	D_CAutoLock cObjectLock(m_pLock);
	if (piqc) ValidateReadPtr(piqc,sizeof(D_IQualityControl));
	m_pQSink = piqc;
	return NOERROR;
} // SetSink


STDMETHODIMP
D_CBasePin::Notify(D_IBaseFilter * pSender, D_Quality q)
{
	UNREFERENCED_PARAMETER(q);
	UNREFERENCED_PARAMETER(pSender);
	DbgBreak("D_IQualityControl::Notify not over-ridden from D_CBasePin.  (IGNORE is OK)");
	return E_NOTIMPL;
} //Notify


// NewSegment notifies of the start/stop/rate applying to the data
// about to be received. Default implementation records data and
// returns S_OK.
// Override this to pass downstream.
STDMETHODIMP
D_CBasePin::NewSegment(
				D_REFERENCE_TIME tStart,
				D_REFERENCE_TIME tStop,
				double dRate)
{
	m_tStart = tStart;
	m_tStop = tStop;
	m_dRate = dRate;

	return S_OK;
}


D_CEnumMediaTypes::D_CEnumMediaTypes(D_CBasePin *pPin,
				 D_CEnumMediaTypes *pEnumMediaTypes) :
	m_Position(0),
	m_pPin(pPin),
	m_cRef(1)
{

#ifdef DEBUG
	m_dwCookie = DbgRegisterObjectCreation("D_CEnumMediaTypes", 0);
#endif

	/* We must be owned by a pin derived from D_CBasePin */

	ASSERT(pPin != NULL);

	/* Hold a reference count on our pin */
	m_pPin->AddRef();

	/* Are we creating a new enumerator */

	if (pEnumMediaTypes == NULL) {
		m_Version = m_pPin->GetMediaTypeVersion();
		return;
	}

	m_Position = pEnumMediaTypes->m_Position;
	m_Version = pEnumMediaTypes->m_Version;
}


/* Destructor releases the reference count on our base pin. NOTE since we hold
	a reference count on the pin who created us we know it is safe to release
	it, no access can be made to it afterwards though as we might have just
	caused the last reference count to go and the object to be deleted */

D_CEnumMediaTypes::~D_CEnumMediaTypes()
{
#ifdef DEBUG
	DbgRegisterObjectDestruction(m_dwCookie);
#endif
	m_pPin->Release();
}


/* Override this to say what interfaces we support where */

STDMETHODIMP
D_CEnumMediaTypes::QueryInterface(REFIID riid,void **ppv)
{
	CheckPointer(ppv, E_POINTER);

	/* Do we have this interface */

	if (riid == IID_IENUMMEDIATYPES || riid == IID_IUNKNOWN) {
		return D_GetInterface((D_IEnumMediaTypes *) this, ppv);
	} else {
		*ppv = NULL;
		return E_NOINTERFACE;
	}
}

STDMETHODIMP_(ULONG)
D_CEnumMediaTypes::AddRef()
{
	return ( ULONG )InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG)
D_CEnumMediaTypes::Release()
{
	ULONG cRef = ( ULONG )InterlockedDecrement(&m_cRef);
	if (cRef == 0) {
		delete this;
	}
	return cRef;
}

/* One of an enumerator's basic member functions allows us to create a cloned
	interface that initially has the same state. Since we are taking a snapshot
	of an object (current position and all) we must lock access at the start */

STDMETHODIMP
D_CEnumMediaTypes::Clone(D_IEnumMediaTypes **ppEnum)
{
	CheckPointer(ppEnum,E_POINTER);
	ValidateReadWritePtr(ppEnum,sizeof(D_IEnumMediaTypes *));
	HRESULT hr = NOERROR;

	/* Check we are still in sync with the pin */
	if (AreWeOutOfSync() == TRUE) {
		*ppEnum = NULL;
		hr = VFW_E_ENUM_OUT_OF_SYNC;
	} else {

		*ppEnum = new D_CEnumMediaTypes(m_pPin,
									  this);

		if (*ppEnum == NULL) {
			hr =  E_OUTOFMEMORY;
		}
	}
	return hr;
}


/* Enumerate the next pin(s) after the current position. The client using this
	interface passes in a pointer to an array of pointers each of which will
	be filled in with a pointer to a fully initialised media type format
	Return NOERROR if it all works,
		  S_FALSE if fewer than cMediaTypes were enumerated.
		  VFW_E_ENUM_OUT_OF_SYNC if the enumerator has been broken by
								 state changes in the filter
	The actual count always correctly reflects the number of types in the array.
*/

STDMETHODIMP
D_CEnumMediaTypes::Next(ULONG cMediaTypes,		  // place this many types...
			  D_AM_MEDIA_TYPE **ppMediaTypes,	// ...in this array
			  ULONG *pcFetched)			// actual count passed
{
	SETUP_WIN_API

	CheckPointer(ppMediaTypes,E_POINTER);
	ValidateReadWritePtr(ppMediaTypes,cMediaTypes * sizeof(D_AM_MEDIA_TYPE *));
	/* Check we are still in sync with the pin */
	if (AreWeOutOfSync() == TRUE) {
		return VFW_E_ENUM_OUT_OF_SYNC;
	}

	if (pcFetched!=NULL) {
		ValidateWritePtr(pcFetched, sizeof(ULONG));
		*pcFetched = 0;			// default unless we succeed
	}
	// now check that the parameter is valid
	else if (cMediaTypes>1) {	 // pcFetched == NULL
		return E_INVALIDARG;
	}
	ULONG cFetched = 0;			// increment as we get each one.

	/* Return each media type by asking the filter for them in turn - If we
		have an error code retured to us while we are retrieving a media type
		we assume that our internal state is stale with respect to the filter
		(for example the window size changing) so we return
		VFW_E_ENUM_OUT_OF_SYNC */

	while (cMediaTypes) {

		D_CMediaType cmt;

		HRESULT hr = m_pPin->GetMediaType(m_Position++, &cmt);
		if (S_OK != hr) {
			break;
		}

		/* We now have a D_CMediaType object that contains the next media type
			but when we assign it to the array position we CANNOT just assign
			the D_AM_MEDIA_TYPE structure because as soon as the object goes out of
			scope it will delete the memory we have just copied. The function
			we use is D_CreateMediaType which allocates a task memory block */

		/*  Transfer across the format block manually to save an allocate
			and free on the format block and generally go faster */

		*ppMediaTypes = (D_AM_MEDIA_TYPE *)WinAPIData.Win32Func.CoTaskMemAllocFunc(sizeof(D_AM_MEDIA_TYPE));
		if (*ppMediaTypes == NULL) {
			break;
		}

		/*  Do a regular copy */
		**ppMediaTypes = (D_AM_MEDIA_TYPE)cmt;

		/*  Make sure the destructor doesn't free these */
		cmt.pbFormat = NULL;
		cmt.cbFormat = (ULONG)NULL;
		cmt.pUnk	 = NULL;


		ppMediaTypes++;
		cFetched++;
		cMediaTypes--;
	}

	if (pcFetched!=NULL) {
		*pcFetched = cFetched;
	}

	return ( cMediaTypes==0 ? NOERROR : S_FALSE );
}


/* Skip over one or more entries in the enumerator */

STDMETHODIMP
D_CEnumMediaTypes::Skip(ULONG cMediaTypes)
{
	//  If we're skipping 0 elements we're guaranteed to skip the
	//  correct number of elements
	if (cMediaTypes == 0) {
		return S_OK;
	}

	/* Check we are still in sync with the pin */
	if (AreWeOutOfSync() == TRUE) {
		return VFW_E_ENUM_OUT_OF_SYNC;
	}

	m_Position += cMediaTypes;

	/*  See if we're over the end */
	D_CMediaType cmt;
	return S_OK == m_pPin->GetMediaType(m_Position - 1, &cmt) ? S_OK : S_FALSE;
}


/* Set the current position back to the start */
/* Reset has 3 simple steps:
 *
 * set position to head of list
 * sync enumerator with object being enumerated
 * return S_OK
 */

STDMETHODIMP
D_CEnumMediaTypes::Reset()

{
	m_Position = 0;

	// Bring the enumerator back into step with the current state.  This
	// may be a noop but ensures that the enumerator will be valid on the
	// next call.
	m_Version = m_pPin->GetMediaTypeVersion();
	return NOERROR;
}




LONG D_CBaseObject::m_cObjects = 0;


/* Constructor */

#ifdef DEBUG
D_CBaseObject::D_CBaseObject(const TCHAR *pName)
#else
D_CBaseObject::D_CBaseObject(const TCHAR *)
#endif
{
	/* Increment the number of active objects */
	InterlockedIncrement(&m_cObjects);

#ifdef DEBUG

#ifdef UNICODE
	m_dwCookie = DbgRegisterObjectCreation(0, pName);
#else
	m_dwCookie = DbgRegisterObjectCreation(pName, 0);
#endif

#endif
}

#ifdef UNICODE
#ifdef DEBUG
D_CBaseObject::D_CBaseObject(const char *pName)
#else
D_CBaseObject::D_CBaseObject(const char *)
#endif
{
	/* Increment the number of active objects */
	InterlockedIncrement(&m_cObjects);

#ifdef DEBUG
	m_dwCookie = DbgRegisterObjectCreation(pName, 0);
#endif
}
#endif


/* Destructor */

D_CBaseObject::~D_CBaseObject()
{
	/* Decrement the number of objects active */
	if (InterlockedDecrement(&m_cObjects) == 0) {
	if (hlibOLEAut32) {
		FreeLibrary(hlibOLEAut32);

		hlibOLEAut32 = 0;
	}
	};


#ifdef DEBUG
	DbgRegisterObjectDestruction(m_dwCookie);
#endif
}




typedef D_INonDelegatingUnknown *D_PNDUNKNOWN;

D_CUnknown::D_CUnknown(const TCHAR *pName, D_IUnknown * pUnk) : 
	D_CBaseObject(pName),
	m_cRef(0),
	m_pUnknown( pUnk != 0 ? pUnk : reinterpret_cast<D_IUnknown *>( static_cast<D_PNDUNKNOWN>(this) ) )
{
	// Everything we need to do has been done in the initializer list
}

// This does the same as above except it has a useless HRESULT argument
// use the previous constructor, this is just left for compatibility...
D_CUnknown::D_CUnknown(TCHAR *pName, D_IUnknown * pUnk,HRESULT * /*phr*/) :
	D_CBaseObject(pName),
	m_cRef(0),
	m_pUnknown( pUnk != 0 ? pUnk : reinterpret_cast<D_IUnknown *>( static_cast<D_PNDUNKNOWN>(this) ) )
{
}

#ifdef UNICODE
D_CUnknown::D_CUnknown(const CHAR *pName, D_IUnknown * pUnk)
: D_CBaseObject(pName), m_cRef(0),
	m_pUnknown( pUnk != 0 ? pUnk : reinterpret_cast<D_IUnknown *>( static_cast<D_PNDUNKNOWN>(this) ) )
{ }

D_CUnknown::D_CUnknown(CHAR *pName, D_IUnknown * pUnk,HRESULT * /*phr*/ ) :
	D_CBaseObject(pName), m_cRef(0),
	m_pUnknown( pUnk != 0 ? pUnk : reinterpret_cast<D_IUnknown *>( static_cast<D_PNDUNKNOWN>(this) ) )
{ }

#endif

#pragma warning( default : 4355 4100 )


/* QueryInterface */

STDMETHODIMP D_CUnknown::NonDelegatingQueryInterface(REFIID riid, void ** ppv)
{
	CheckPointer(ppv,E_POINTER);
	ValidateReadWritePtr(ppv,sizeof(PVOID));

	/* We know only about IUnknown */

	if (riid == IID_IUNKNOWN) {
		D_GetInterface((D_IUnknown *) (D_PNDUNKNOWN) this, ppv);
		return NOERROR;
	} else {
		*ppv = NULL;
		return E_NOINTERFACE;
	}
}

template<class T> inline static T ourmax( const T & a, const T & b )
{
	return a > b ? a : b;
}

/* AddRef */

STDMETHODIMP_(ULONG) D_CUnknown::NonDelegatingAddRef()
{
//	LONG lRef = InterlockedIncrement( ( LONG * )&m_cRef );
//	ASSERT(lRef > 0);
	DbgLog((LOG_MEMORY,3,TEXT("	Obj %d ref++ = %d"),
			m_dwCookie, m_cRef));
	return ourmax(ULONG(m_cRef), 1ul);
}


/* Release */

STDMETHODIMP_(ULONG) D_CUnknown::NonDelegatingRelease()
{
	/* If the reference count drops to zero delete ourselves */

	LONG lRef = InterlockedDecrement( ( LONG * )&m_cRef );
	ASSERT(lRef >= 0);

	DbgLog((LOG_MEMORY,3,TEXT("	Object %d ref-- = %d"),
		m_dwCookie, m_cRef));
	if (lRef == 0) {

		m_cRef++;

		delete this;
		return ULONG(0);
	} else {
		return ourmax(ULONG(m_cRef), 1ul);
	}
}

// --- CAMEvent -----------------------
D_CAMEvent::D_CAMEvent(BOOL fManualReset)
{
	SETUP_WIN_API

	m_hEvent = WinAPIData.Win32Func.CreateEventAFunc(NULL, fManualReset, FALSE, NULL);
}

D_CAMEvent::~D_CAMEvent()
{
	SETUP_WIN_API

	if (m_hEvent) {
	EXECUTE_ASSERT(WinAPIData.Win32Func.CloseHandleFunc(m_hEvent));
	}
}




D_CEnumPins::D_CEnumPins(D_CBaseFilter *pFilter,
			 D_CEnumPins *pEnumPins) :
	m_Position(0),
	m_PinCount(0),
	m_pFilter(pFilter),
	m_cRef(1),				// Already ref counted
	m_PinCache(NAME("Pin Cache"))
{

#ifdef DEBUG
	m_dwCookie = DbgRegisterObjectCreation("D_CEnumPins", 0);
#endif

	/* We must be owned by a filter derived from D_CBaseFilter */

	ASSERT(pFilter != NULL);

	/* Hold a reference count on our filter */
	m_pFilter->AddRef();

	/* Are we creating a new enumerator */

	if (pEnumPins == NULL) {
		m_Version = m_pFilter->GetPinVersion();
		m_PinCount = m_pFilter->GetPinCount();
	} else {
		ASSERT(m_Position <= m_PinCount);
		m_Position = pEnumPins->m_Position;
		m_PinCount = pEnumPins->m_PinCount;
		m_Version = pEnumPins->m_Version;
		m_PinCache.AddTail(&(pEnumPins->m_PinCache));
	}
}


/* Destructor releases the reference count on our filter NOTE since we hold
	a reference count on the filter who created us we know it is safe to
	release it, no access can be made to it afterwards though as we have just
	caused the last reference count to go and the object to be deleted */

D_CEnumPins::~D_CEnumPins()
{
	m_pFilter->Release();

#ifdef DEBUG
	DbgRegisterObjectDestruction(m_dwCookie);
#endif
}


/* Override this to say what interfaces we support where */

STDMETHODIMP
D_CEnumPins::QueryInterface(REFIID riid,void **ppv)
{
	CheckPointer(ppv, E_POINTER);

	/* Do we have this interface */

	if (riid == IID_IENUMPINS || riid == IID_IUNKNOWN) {
		return D_GetInterface((D_IEnumPins *) this, ppv);
	} else {
		*ppv = NULL;
		return E_NOINTERFACE;
	}
}

STDMETHODIMP_(ULONG)
D_CEnumPins::AddRef()
{
	return ( ULONG )InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG)
D_CEnumPins::Release()
{
	ULONG cRef = ( ULONG )InterlockedDecrement(&m_cRef);
	if (cRef == 0) {
		delete this;
	}
	return cRef;
}

/* One of an enumerator's basic member functions allows us to create a cloned
	interface that initially has the same state. Since we are taking a snapshot
	of an object (current position and all) we must lock access at the start */

STDMETHODIMP
D_CEnumPins::Clone(D_IEnumPins **ppEnum)
{
	CheckPointer(ppEnum,E_POINTER);
	ValidateReadWritePtr(ppEnum,sizeof(D_IEnumPins *));
	HRESULT hr = NOERROR;

	/* Check we are still in sync with the filter */
	if (AreWeOutOfSync() == TRUE) {
		*ppEnum = NULL;
		hr =  VFW_E_ENUM_OUT_OF_SYNC;
	} else {

		*ppEnum = new D_CEnumPins(m_pFilter,
								this);
		if (*ppEnum == NULL) {
			hr = E_OUTOFMEMORY;
		}
	}
	return hr;
}


/* Return the next pin after the current position */

STDMETHODIMP
D_CEnumPins::Next(ULONG cPins,		// place this many pins...
		D_IPin **ppPins,	  // ...in this array
		ULONG *pcFetched)	// actual count passed returned here
{
	CheckPointer(ppPins,E_POINTER);
	ValidateReadWritePtr(ppPins,cPins * sizeof(D_IPin *));

	ASSERT(ppPins);

	if (pcFetched!=NULL) {
		ValidateWritePtr(pcFetched, sizeof(ULONG));
		*pcFetched = 0;			// default unless we succeed
	}
	// now check that the parameter is valid
	else if (cPins>1) {	// pcFetched == NULL
		return E_INVALIDARG;
	}
	ULONG cFetched = 0;			// increment as we get each one.

	/* Check we are still in sync with the filter */
	if (AreWeOutOfSync() == TRUE) {
	// If we are out of sync, we should refresh the enumerator.
	// This will reset the position and update the other members, but
	// will not clear cache of pins we have already returned.
	Refresh();
	}

	/* Calculate the number of available pins */

	int cRealPins = DX_CSTRMBASE_MIN(m_PinCount - m_Position, (int) cPins);
	if (cRealPins == 0) {
		return S_FALSE;
	}

	/* Return each pin interface NOTE GetPin returns D_CBasePin * not addrefed
		so we must QI for the D_IPin (which increments its reference count)
		If while we are retrieving a pin from the filter an error occurs we
		assume that our internal state is stale with respect to the filter
		(for example someone has deleted a pin) so we
		return VFW_E_ENUM_OUT_OF_SYNC							*/

	while (cRealPins && (m_PinCount - m_Position)) {

		/* Get the next pin object from the filter */

		D_CBasePin *pPin = m_pFilter->GetPin(m_Position++);
		if (pPin == NULL) {
			// If this happend, and it's not the first time through, then we've got a problem,
			// since we should really go back and release the iPins, which we have previously
			// AddRef'ed.
			ASSERT( cFetched==0 );
			return VFW_E_ENUM_OUT_OF_SYNC;
		}

		/* We only want to return this pin, if it is not in our cache */
		if (0 == m_PinCache.Find(pPin))
		{
			/* From the object get an D_IPin interface */

			*ppPins = pPin;
			pPin->AddRef();

			cFetched++;
			ppPins++;

			m_PinCache.AddTail(pPin);

			cRealPins--;

		}
	}

	if (pcFetched!=NULL) {
		*pcFetched = cFetched;
	}

	return (cPins==cFetched ? NOERROR : S_FALSE);
}


/* Skip over one or more entries in the enumerator */

STDMETHODIMP
D_CEnumPins::Skip(ULONG cPins)
{
	/* Check we are still in sync with the filter */
	if (AreWeOutOfSync() == TRUE) {
		return VFW_E_ENUM_OUT_OF_SYNC;
	}

	/* Work out how many pins are left to skip over */
	/* We could position at the end if we are asked to skip too many... */
	/* ..which would match the base implementation for D_CEnumMediaTypes::Skip */

	ULONG PinsLeft = ( ULONG )( m_PinCount - m_Position );
	if (cPins > PinsLeft) {
		return S_FALSE;
	}
	m_Position += cPins;
	return NOERROR;
}


/* Set the current position back to the start */
/* Reset has 4 simple steps:
 *
 * Set position to head of list
 * Sync enumerator with object being enumerated
 * Clear the cache of pins already returned
 * return S_OK
 */

STDMETHODIMP
D_CEnumPins::Reset()
{
	m_Version = m_pFilter->GetPinVersion();
	m_PinCount = m_pFilter->GetPinCount();

	m_Position = 0;

	// Clear the cache
	m_PinCache.RemoveAll();

	return S_OK;
}


/* Set the current position back to the start */
/* Refresh has 3 simple steps:
 *
 * Set position to head of list
 * Sync enumerator with object being enumerated
 * return S_OK
 */

STDMETHODIMP
D_CEnumPins::Refresh()
{
	m_Version = m_pFilter->GetPinVersion();
	m_PinCount = m_pFilter->GetPinCount();

	m_Position = 0;
	return S_OK;
}









D_CMediaType::~D_CMediaType(){
	FreeMediaType(*this);
}


D_CMediaType::D_CMediaType()
{
	InitMediaType();
}


D_CMediaType::D_CMediaType(const GUID * type)
{
	InitMediaType();
	majortype = *type;
}


// copy constructor does a deep copy of the format block

D_CMediaType::D_CMediaType(const D_AM_MEDIA_TYPE& rt, HRESULT* phr)
{
	HRESULT hr = CopyMediaType(this, &rt);
	if (FAILED(hr) && (NULL != phr)) {
		*phr = hr;
	}
}


D_CMediaType::D_CMediaType(const D_CMediaType& rt, HRESULT* phr)
{
	HRESULT hr = CopyMediaType(this, &rt);
	if (FAILED(hr) && (NULL != phr)) {
		*phr = hr;
	}
}


// this class inherits publicly from D_AM_MEDIA_TYPE so the compiler could generate
// the following assignment operator itself, however it could introduce some
// memory conflicts and leaks in the process because the structure contains
// a dynamically allocated block (pbFormat) which it will not copy correctly

D_CMediaType&
D_CMediaType::operator=(const D_AM_MEDIA_TYPE& rt)
{
	Set(rt);
	return *this;
}

D_CMediaType&
D_CMediaType::operator=(const D_CMediaType& rt)
{
	*this = (D_AM_MEDIA_TYPE &) rt;
	return *this;
}

BOOL
D_CMediaType::operator == (const D_CMediaType& rt) const
{
	// I don't believe we need to check sample size or
	// temporal compression flags, since I think these must
	// be represented in the type, subtype and format somehow. They
	// are pulled out as separate flags so that people who don't understand
	// the particular format representation can still see them, but
	// they should duplicate information in the format block.

	return ((IsEqualGUID(majortype,rt.majortype) == TRUE) &&
		(IsEqualGUID(subtype,rt.subtype) == TRUE) &&
		(IsEqualGUID(formattype,rt.formattype) == TRUE) &&
		(cbFormat == rt.cbFormat) &&
		( (cbFormat == 0) ||
		  (memcmp(pbFormat, rt.pbFormat, cbFormat) == 0)));
}


BOOL
D_CMediaType::operator != (const D_CMediaType& rt) const
{
	/* Check to see if they are equal */

	if (*this == rt) {
		return FALSE;
	}
	return TRUE;
}


HRESULT
D_CMediaType::Set(const D_CMediaType& rt)
{
	return Set((D_AM_MEDIA_TYPE &) rt);
}


HRESULT
D_CMediaType::Set(const D_AM_MEDIA_TYPE& rt)
{
	if (&rt != this) {
		FreeMediaType(*this);
		HRESULT hr = CopyMediaType(this, &rt);
		if (FAILED(hr)) {
			return E_OUTOFMEMORY;
		}
	}

	return S_OK;	
}


BOOL
D_CMediaType::IsValid() const
{
	return (!IsEqualGUID(majortype,_GUID_NULL));
}


void
D_CMediaType::SetType(const GUID* ptype)
{
	majortype = *ptype;
}


void
D_CMediaType::SetSubtype(const GUID* ptype)
{
	subtype = *ptype;
}


ULONG
D_CMediaType::GetSampleSize() const {
	if (IsFixedSize()) {
		return lSampleSize;
	} else {
		return 0;
	}
}


void
D_CMediaType::SetSampleSize(ULONG sz) {
	if (sz == 0) {
		SetVariableSize();
	} else {
		bFixedSizeSamples = TRUE;
		lSampleSize = sz;
	}
}


void
D_CMediaType::SetVariableSize() {
	bFixedSizeSamples = FALSE;
}


void
D_CMediaType::SetTemporalCompression(BOOL bCompressed) {
	bTemporalCompression = bCompressed;
}

BOOL
D_CMediaType::SetFormat(BYTE * pformat, ULONG cb)
{
	if (NULL == AllocFormatBuffer(cb))
	return(FALSE);

	ASSERT(pbFormat);
	_MEMCPY(pbFormat, pformat, cb);
	return(TRUE);
}


// set the type of the media type format block, this type defines what you
// will actually find in the format pointer. For example FORMAT_VideoInfo or
// FORMAT_WaveFormatEx. In the future this may be an interface pointer to a
// property set. Before sending out media types this should be filled in.

void
D_CMediaType::SetFormatType(const GUID *pformattype)
{
	formattype = *pformattype;
}


// reset the format buffer

void D_CMediaType::ResetFormatBuffer()
{
	SETUP_WIN_API

	if (cbFormat) {
		WinAPIData.Win32Func.CoTaskMemFreeFunc((PVOID)pbFormat);
	}
	cbFormat = 0;
	pbFormat = NULL;
}


// allocate length bytes for the format and return a read/write pointer
// If we cannot allocate the new block of memory we return NULL leaving
// the original block of memory untouched (as does ReallocFormatBuffer)

BYTE*
D_CMediaType::AllocFormatBuffer(ULONG length)
{
	SETUP_WIN_API

	ASSERT(length);

	// do the types have the same buffer size

	if (cbFormat == length) {
		return pbFormat;
	}

	// allocate the new format buffer

	BYTE *pNewFormat = (PBYTE)WinAPIData.Win32Func.CoTaskMemAllocFunc(length);
	if (pNewFormat == NULL) {
		if (length <= cbFormat) return pbFormat; //reuse the old block anyway.
		return NULL;
	}

	// delete the old format

	if (cbFormat != 0) {
		ASSERT(pbFormat);
		WinAPIData.Win32Func.CoTaskMemFreeFunc((PVOID)pbFormat);
	}

	cbFormat = length;
	pbFormat = pNewFormat;
	return pbFormat;
}


// reallocate length bytes for the format and return a read/write pointer
// to it. We keep as much information as we can given the new buffer size
// if this fails the original format buffer is left untouched. The caller
// is responsible for ensuring the size of memory required is non zero

BYTE*
D_CMediaType::ReallocFormatBuffer(ULONG length)
{
	SETUP_WIN_API

	ASSERT(length);

	// do the types have the same buffer size

	if (cbFormat == length) {
		return pbFormat;
	}

	// allocate the new format buffer

	BYTE *pNewFormat = (PBYTE)WinAPIData.Win32Func.CoTaskMemAllocFunc(length);
	if (pNewFormat == NULL) {
		if (length <= cbFormat) return pbFormat; //reuse the old block anyway.
		return NULL;
	}

	// copy any previous format (or part of if new is smaller)
	// delete the old format and replace with the new one

	if (cbFormat != 0) {
		ASSERT(pbFormat);
		_MEMCPY(pNewFormat,pbFormat,DX_CSTRMBASE_MIN(length,cbFormat));
		WinAPIData.Win32Func.CoTaskMemFreeFunc((PVOID)pbFormat);
	}

	cbFormat = length;
	pbFormat = pNewFormat;
	return pNewFormat;
}

// initialise a media type structure

void D_CMediaType::InitMediaType()
{
	_MEMSET((PVOID)this, 0, sizeof(*this));
	lSampleSize = 1;
	bFixedSizeSamples = TRUE;
}


// a partially specified media type can be passed to D_IPin::Connect
// as a constraint on the media type used in the connection.
// the type, subtype or format type can be null.
BOOL
D_CMediaType::IsPartiallySpecified(void) const
{
	if ((majortype == _GUID_NULL) ||
		(formattype == _GUID_NULL)) {
			return TRUE;
	} else {
		return FALSE;
	}
}

BOOL
D_CMediaType::MatchesPartial(const D_CMediaType* ppartial) const
{
	if ((ppartial->majortype != _GUID_NULL) &&
		(majortype != ppartial->majortype)) {
			return FALSE;
	}
	if ((ppartial->subtype != _GUID_NULL) &&
		(subtype != ppartial->subtype)) {
			return FALSE;
	}

	if (ppartial->formattype != _GUID_NULL) {
		// if the format block is specified then it must match exactly
		if (formattype != ppartial->formattype) {
			return FALSE;
		}
		if (cbFormat != ppartial->cbFormat) {
			return FALSE;
		}
		if ((cbFormat != 0) &&
			(memcmp(pbFormat, ppartial->pbFormat, cbFormat) != 0)) {
				return FALSE;
		}
	}

	return TRUE;

}


D_CBaseList::CNode::CNode()
{
}

D_CBaseList::CNode *D_CBaseList::CNode::Prev() const 
{
	return m_pPrev;
}

D_CBaseList::CNode *D_CBaseList::CNode::Next() const 
{
	return m_pNext;
}

void D_CBaseList::CNode::SetPrev(CNode *p)
{
	m_pPrev = p;
}

void D_CBaseList::CNode::SetNext(CNode *p)
{
	m_pNext = p;
}

void *D_CBaseList::CNode::GetData() const 
{
	return m_pObject;
}

void D_CBaseList::CNode::SetData(void *p)
{
	m_pObject = p;
}


D_CBaseList::CNodeCache::CNodeCache( INT iCacheSize ) :
	m_iCacheSize(iCacheSize),
	m_pHead(NULL),
	m_iUsed(0)
{
}

D_CBaseList::CNodeCache::~CNodeCache()
{
	D_CBaseList::CNode *pNode = m_pHead;
	while(pNode)
	{
		D_CBaseList::CNode *pCurrent = pNode;
		pNode = pNode->Next();
		delete pCurrent;
	}
}

void D_CBaseList::CNodeCache::AddToCache(CNode *pNode)
{
	if (m_iUsed < m_iCacheSize)
	{
		pNode->SetNext(m_pHead);
		m_pHead = pNode;
		m_iUsed++;
	}
	else
	{
		delete pNode;
	}
}

D_POSITION D_CBaseList::Next(D_POSITION pos) const
{
	if (pos == NULL)
	{
		return (D_POSITION) m_pFirst;
	}
	D_CBaseList::CNode *pn = (D_CBaseList::CNode *) pos;
	return (D_POSITION) pn->Next();
}

D_POSITION D_CBaseList::Prev(D_POSITION pos) const
{
	if (pos == NULL)
	{
		return (D_POSITION) m_pLast;
	}
	D_CBaseList::CNode *pn = (D_CBaseList::CNode *) pos;
	return (D_POSITION) pn->Prev();
}


D_CBaseList::CNode *D_CBaseList::CNodeCache::RemoveFromCache()
{
	D_CBaseList::CNode *pNode = m_pHead;
	if (pNode != NULL)
	{
		m_pHead = pNode->Next();
		m_iUsed--;
		ASSERT(m_iUsed >= 0);
	}
	else
	{
		ASSERT(m_iUsed == 0);
	}
	return pNode;
}


/* set cursor to the position of each element of list in turn  */
#define INTERNALTRAVERSELIST(list, cursor)				\
for ( cursor = (list).GetHeadPositionI()			\
	; cursor!=NULL								\
	; cursor = (list).Next(cursor)				\
	)


/* set cursor to the position of each element of list in turn
	in reverse order
*/
#define INTERNALREVERSETRAVERSELIST(list, cursor)		\
for ( cursor = (list).GetTailPositionI()			\
	; cursor!=NULL								\
	; cursor = (list).Prev(cursor)				\
	)

const int DEFAULTCACHE = 10;	/* Default node object cache size */


#ifdef DEBUG
D_CBaseList::D_CBaseList(TCHAR *pName,	// Descriptive list name
					 INT iItems) :	// Node cache size
	CBaseObject(pName),
#else
D_CBaseList::D_CBaseList(TCHAR * /*pName*/,	// Descriptive list name
					 INT iItems) :	// Node cache size
#endif
	m_pFirst(NULL),
	m_pLast(NULL),
	m_Count(0),
	m_Cache(iItems)
{
} // constructor

#ifdef DEBUG
D_CBaseList::D_CBaseList(TCHAR *pName) :  // Descriptive list name
	CBaseObject(pName),
#else
D_CBaseList::D_CBaseList(TCHAR *) :  // Descriptive list name
#endif
	m_pFirst(NULL),
	m_pLast(NULL),
	m_Count(0),
	m_Cache(DEFAULTCACHE)
{
} // constructor

#ifdef UNICODE
#ifdef DEBUG
D_CBaseList::D_CBaseList(CHAR *pName,	// Descriptive list name
					 INT iItems) :	// Node cache size
	CBaseObject(pName),
#else
D_CBaseList::D_CBaseList(CHAR *,	// Descriptive list name
					 INT iItems) :	// Node cache size
#endif
	m_pFirst(NULL),
	m_pLast(NULL),
	m_Count(0),
	m_Cache(iItems)
{
} // constructor

#ifdef DEBUG
D_CBaseList::D_CBaseList(CHAR *pName) :  // Descriptive list name
	CBaseObject(pName),
#else
D_CBaseList::D_CBaseList(CHAR *) :  // Descriptive list name
#endif
	m_pFirst(NULL),
	m_pLast(NULL),
	m_Count(0),
	m_Cache(DEFAULTCACHE)
{
} // constructor

#endif

/* The destructor enumerates all the node objects in the list and
	in the cache deleting each in turn. We do not do any processing
	on the objects that the list holds (i.e. points to) so if they
	represent interfaces for example the creator of the list should
	ensure that each of them is released before deleting us
*/
D_CBaseList::~D_CBaseList()
{
	/* Delete all our list nodes */

	RemoveAll();

} // destructor

/* Remove all the nodes from the list but don't do anything
	with the objects that each node looks after (this is the
	responsibility of the creator).
	Aa a last act we reset the signalling event
	(if available) to indicate to clients that the list
	does not have any entries in it.
*/
void D_CBaseList::RemoveAll()
{
	/* Free up all the CNode objects NOTE we don't bother putting the
		deleted nodes into the cache as this method is only really called
		in serious times of change such as when we are being deleted at
		which point the cache will be deleted anway */

	CNode *pn = m_pFirst;
	while (pn) {
		CNode *op = pn;
		pn = pn->Next();
		delete op;
	}

	/* Reset the object count and the list pointers */

	m_Count = 0;
	m_pFirst = m_pLast = NULL;

} // RemoveAll



/* Return a position enumerator for the entire list.
	A position enumerator is a pointer to a node object cast to a
	transparent type so all we do is return the head/tail node
	pointer in the list.
	WARNING because the position is a pointer to a node there is
	an implicit assumption for users a the list class that after
	deleting an object from the list that any other position
	enumerators that you have may be invalid (since the node
	may be gone).
*/
D_POSITION D_CBaseList::GetHeadPositionI() const
{
	return (D_POSITION) m_pFirst;
} // GetHeadPosition



D_POSITION D_CBaseList::GetTailPositionI() const
{
	return (D_POSITION) m_pLast;
} // GetTailPosition



/* Get the number of objects in the list,
	Get the lock before accessing the count.
	Locking may not be entirely necessary but it has the side effect
	of making sure that all operations are complete before we get it.
	So for example if a list is being added to this list then that
	will have completed in full before we continue rather than seeing
	an intermediate albeit valid state
*/
int D_CBaseList::GetCountI() const
{
	return m_Count;
} // GetCount



/* Return the object at rp, update rp to the next object from
	the list or NULL if you have moved over the last object.
	You may still call this function once we return NULL but
	we will continue to return a NULL position value
*/
void *D_CBaseList::GetNextI(D_POSITION& rp) const
{
	/* have we reached the end of the list */

	if (rp == NULL) {
		return NULL;
	}

	/* Lock the object before continuing */

	void *pObject;

	/* Copy the original position then step on */

	CNode *pn = (CNode *) rp;
	ASSERT(pn != NULL);
	rp = (D_POSITION) pn->Next();

	/* Get the object at the original position from the list */

	pObject = pn->GetData();
	// ASSERT(pObject != NULL);	// NULL pointers in the list are allowed.
	return pObject;
} //GetNext



/* Return the object at p.
	Asking for the object at NULL ASSERTs then returns NULL
	The object is NOT locked.  The list is not being changed
	in any way.  If another thread is busy deleting the object
	then locking would only result in a change from one bad
	behaviour to another.
*/
void *D_CBaseList::GetI(D_POSITION p) const
{
	if (p == NULL) {
		return NULL;
	}

	CNode * pn = (CNode *) p;
	void *pObject = pn->GetData();
	// ASSERT(pObject != NULL);	// NULL pointers in the list are allowed.
	return pObject;
} //Get



/* Return the first position in the list which holds the given pointer.
	Return NULL if it's not found.
*/
D_POSITION D_CBaseList::FindI( void * pObj) const
{
	D_POSITION pn;
	INTERNALTRAVERSELIST(*this, pn){
		if (GetI(pn)==pObj) {
			return pn;
		}
	}
	return NULL;
} // Find



/* Remove the first node in the list (deletes the pointer to its object
	from the list, does not free the object itself).
	Return the pointer to its object or NULL if empty
*/
void *D_CBaseList::RemoveHeadI()
{
	/* All we do is get the head position and ask for that to be deleted.
		We could special case this since some of the code path checking
		in Remove() is redundant as we know there is no previous
		node for example but it seems to gain little over the
		added complexity
	*/

	return RemoveI((D_POSITION)m_pFirst);
} // RemoveHead



/* Remove the last node in the list (deletes the pointer to its object
	from the list, does not free the object itself).
	Return the pointer to its object or NULL if empty
*/
void *D_CBaseList::RemoveTailI()
{
	/* All we do is get the tail position and ask for that to be deleted.
		We could special case this since some of the code path checking
		in Remove() is redundant as we know there is no previous
		node for example but it seems to gain little over the
		added complexity
	*/

	return RemoveI((D_POSITION)m_pLast);
} // RemoveTail



/* Remove the pointer to the object in this position from the list.
	Deal with all the chain pointers
	Return a pointer to the object removed from the list.
	The node object that is freed as a result
	of this operation is added to the node cache where
	it can be used again.
	Remove(NULL) is a harmless no-op - but probably is a wart.
*/
void *D_CBaseList::RemoveI(D_POSITION pos)
{
	/* Lock the critical section before continuing */

	// ASSERT (pos!=NULL);	 // Removing NULL is to be harmless!
	if (pos==NULL) return NULL;


	CNode *pCurrent = (CNode *) pos;
	ASSERT(pCurrent != NULL);

	/* Update the previous node */

	CNode *pNode = pCurrent->Prev();
	if (pNode == NULL) {
		m_pFirst = pCurrent->Next();
	} else {
		pNode->SetNext(pCurrent->Next());
	}

	/* Update the following node */

	pNode = pCurrent->Next();
	if (pNode == NULL) {
		m_pLast = pCurrent->Prev();
	} else {
		pNode->SetPrev(pCurrent->Prev());
	}

	/* Get the object this node was looking after */

	void *pObject = pCurrent->GetData();

	// ASSERT(pObject != NULL);	// NULL pointers in the list are allowed.

	/* Try and add the node object to the cache -
		a NULL return code from the cache means we ran out of room.
		The cache size is fixed by a constructor argument when the
		list is created and defaults to DEFAULTCACHE.
		This means that the cache will have room for this many
		node objects. So if you have a list of media samples
		and you know there will never be more than five active at
		any given time of them for example then override the default
		constructor
	*/

	m_Cache.AddToCache(pCurrent);

	/* If the list is empty then reset the list event */

	--m_Count;
	ASSERT(m_Count >= 0);
	return pObject;
} // Remove



/* Add this object to the tail end of our list
	Return the new tail position.
*/

D_POSITION D_CBaseList::AddTailI(void *pObject)
{
	/* Lock the critical section before continuing */

	CNode *pNode;
	// ASSERT(pObject);	// NULL pointers in the list are allowed.

	/* If there is a node objects in the cache then use
		that otherwise we will have to create a new one */

	pNode = (CNode *) m_Cache.RemoveFromCache();
	if (pNode == NULL) {
		pNode = new CNode;
	}

	/* Check we have a valid object */

	if (pNode == NULL) {
		return NULL;
	}

	/* Initialise all the CNode object
		just in case it came from the cache
	*/

	pNode->SetData(pObject);
	pNode->SetNext(NULL);
	pNode->SetPrev(m_pLast);

	if (m_pLast == NULL) {
		m_pFirst = pNode;
	} else {
		m_pLast->SetNext(pNode);
	}

	/* Set the new last node pointer and also increment the number
		of list entries, the critical section is unlocked when we
		exit the function
	*/

	m_pLast = pNode;
	++m_Count;

	return (D_POSITION) pNode;
} // AddTail(object)



/* Add this object to the head end of our list
	Return the new head position.
*/
D_POSITION D_CBaseList::AddHeadI(void *pObject)
{
	CNode *pNode;
	// ASSERT(pObject);  // NULL pointers in the list are allowed.

	/* If there is a node objects in the cache then use
		that otherwise we will have to create a new one */

	pNode = (CNode *) m_Cache.RemoveFromCache();
	if (pNode == NULL) {
		pNode = new CNode;
	}

	/* Check we have a valid object */

	if (pNode == NULL) {
		return NULL;
	}

	/* Initialise all the CNode object
		just in case it came from the cache
	*/

	pNode->SetData(pObject);

	/* chain it in (set four pointers) */
	pNode->SetPrev(NULL);
	pNode->SetNext(m_pFirst);

	if (m_pFirst == NULL) {
		m_pLast = pNode;
	} else {
		m_pFirst->SetPrev(pNode);
	}
	m_pFirst = pNode;

	++m_Count;

	return (D_POSITION) pNode;
} // AddHead(object)



/* Add all the elements in *pList to the tail of this list.
	Return TRUE if it all worked, FALSE if it didn't.
	If it fails some elements may have been added.
*/
BOOL D_CBaseList::AddTail(D_CBaseList *pList)
{
	/* lock the object before starting then enumerate
		each entry in the source list and add them one by one to
		our list (while still holding the object lock)
		Lock the other list too.
	*/
	D_POSITION pos = pList->GetHeadPositionI();

	while (pos) {
		if (NULL == AddTailI(pList->GetNextI(pos))) {
			return FALSE;
		}
	}
	return TRUE;
} // AddTail(list)



/* Add all the elements in *pList to the head of this list.
	Return TRUE if it all worked, FALSE if it didn't.
	If it fails some elements may have been added.
*/
BOOL D_CBaseList::AddHead(D_CBaseList *pList)
{
	/* lock the object before starting then enumerate
		each entry in the source list and add them one by one to
		our list (while still holding the object lock)
		Lock the other list too.

		To avoid reversing the list, traverse it backwards.
	*/

	D_POSITION pos;

	INTERNALREVERSETRAVERSELIST(*pList, pos) {
		if (NULL== AddHeadI(pList->GetI(pos))){
			return FALSE;
		}
	}
	return TRUE;
} // AddHead(list)



/* Add the object after position p
	p is still valid after the operation.
	AddAfter(NULL,x) adds x to the start - same as AddHead
	Return the position of the new object, NULL if it failed
*/
D_POSITION  D_CBaseList::AddAfterI(D_POSITION pos, void * pObj)
{
	if (pos==NULL)
		return AddHeadI(pObj);

	/* As someone else might be furkling with the list -
		Lock the critical section before continuing
	*/
	CNode *pAfter = (CNode *) pos;
	ASSERT(pAfter != NULL);
	if (pAfter==m_pLast)
		return AddTailI(pObj);

	/* set pnode to point to a new node, preferably from the cache */

	CNode *pNode = (CNode *) m_Cache.RemoveFromCache();
	if (pNode == NULL) {
		pNode = new CNode;
	}

	/* Check we have a valid object */

	if (pNode == NULL) {
		return NULL;
	}

	/* Initialise all the CNode object
		just in case it came from the cache
	*/

	pNode->SetData(pObj);

	/* It is to be added to the middle of the list - there is a before
		and after node.  Chain it after pAfter, before pBefore.
	*/
	CNode * pBefore = pAfter->Next();
	ASSERT(pBefore != NULL);

	/* chain it in (set four pointers) */
	pNode->SetPrev(pAfter);
	pNode->SetNext(pBefore);
	pBefore->SetPrev(pNode);
	pAfter->SetNext(pNode);

	++m_Count;

	return (D_POSITION) pNode;

} // AddAfter(object)



BOOL D_CBaseList::AddAfter(D_POSITION p, D_CBaseList *pList)
{
	D_POSITION pos;
	INTERNALTRAVERSELIST(*pList, pos) {
		/* p follows along the elements being added */
		p = AddAfterI(p, pList->GetI(pos));
		if (p==NULL) return FALSE;
	}
	return TRUE;
} // AddAfter(list)



/* Mirror images:
	Add the element or list after position p.
	p is still valid after the operation.
	AddBefore(NULL,x) adds x to the end - same as AddTail
*/
D_POSITION D_CBaseList::AddBeforeI(D_POSITION pos, void * pObj)
{
	if (pos==NULL)
		return AddTailI(pObj);

	/* set pnode to point to a new node, preferably from the cache */

	CNode *pBefore = (CNode *) pos;
	ASSERT(pBefore != NULL);
	if (pBefore==m_pFirst)
		return AddHeadI(pObj);

	CNode * pNode = (CNode *) m_Cache.RemoveFromCache();
	if (pNode == NULL) {
		pNode = new CNode;
	}

	/* Check we have a valid object */

	if (pNode == NULL) {
		return NULL;
	}

	/* Initialise all the CNode object
		just in case it came from the cache
	*/

	pNode->SetData(pObj);

	/* It is to be added to the middle of the list - there is a before
		and after node.  Chain it after pAfter, before pBefore.
	*/

	CNode * pAfter = pBefore->Prev();
	ASSERT(pAfter != NULL);

	/* chain it in (set four pointers) */
	pNode->SetPrev(pAfter);
	pNode->SetNext(pBefore);
	pBefore->SetPrev(pNode);
	pAfter->SetNext(pNode);

	++m_Count;

	return (D_POSITION) pNode;

} // Addbefore(object)



BOOL D_CBaseList::AddBefore(D_POSITION p, D_CBaseList *pList)
{
	D_POSITION pos;
	INTERNALREVERSETRAVERSELIST(*pList, pos) {
		/* p follows along the elements being added */
		p = AddBeforeI(p, pList->GetI(pos));
		if (p==NULL) return FALSE;
	}
	return TRUE;
} // AddBefore(list)



/* Split *this after position p in *this
	Retain as *this the tail portion of the original *this
	Add the head portion to the tail end of *pList
	Return TRUE if it all worked, FALSE if it didn't.

	e.g.
	  foo->MoveToTail(foo->GetHeadPosition(), bar);
		  moves one element from the head of foo to the tail of bar
	  foo->MoveToTail(NULL, bar);
		  is a no-op
	  foo->MoveToTail(foo->GetTailPosition, bar);
		  concatenates foo onto the end of bar and empties foo.

	A better, except excessively long name might be
		MoveElementsFromHeadThroughPositionToOtherTail
*/
BOOL D_CBaseList::MoveToTail
		(D_POSITION pos, D_CBaseList *pList)
{
	/* Algorithm:
		Note that the elements (including their order) in the concatenation
		of *pList to the head of *this is invariant.
		1. Count elements to be moved
		2. Join *pList onto the head of this to make one long chain
		3. Set first/Last pointers in *this and *pList
		4. Break the chain at the new place
		5. Adjust counts
		6. Set/Reset any events
	*/

	if (pos==NULL) return TRUE;  // no-op.  Eliminates special cases later.


	/* Make cMove the number of nodes to move */
	CNode * p = (CNode *)pos;
	int cMove = 0;			// number of nodes to move
	while(p!=NULL) {
		p = p->Prev();
		++cMove;
	}


	/* Join the two chains together */
	if (pList->m_pLast!=NULL)
		pList->m_pLast->SetNext(m_pFirst);
	if (m_pFirst!=NULL)
		m_pFirst->SetPrev(pList->m_pLast);


	/* set first and last pointers */
	p = (CNode *)pos;

	if (pList->m_pFirst==NULL)
		pList->m_pFirst = m_pFirst;
	m_pFirst = p->Next();
	if (m_pFirst==NULL)
		m_pLast = NULL;
	pList->m_pLast = p;


	/* Break the chain after p to create the new pieces */
	if (m_pFirst!=NULL)
		m_pFirst->SetPrev(NULL);
	p->SetNext(NULL);


	/* Adjust the counts */
	m_Count -= cMove;
	pList->m_Count += cMove;

	return TRUE;

} // MoveToTail



/* Mirror image of MoveToTail:
	Split *this before position p in *this.
	Retain in *this the head portion of the original *this
	Add the tail portion to the start (i.e. head) of *pList
	Return TRUE if it all worked, FALSE if it didn't.

	e.g.
	  foo->MoveToHead(foo->GetTailPosition(), bar);
		  moves one element from the tail of foo to the head of bar
	  foo->MoveToHead(NULL, bar);
		  is a no-op
	  foo->MoveToHead(foo->GetHeadPosition, bar);
		  concatenates foo onto the start of bar and empties foo.
*/
BOOL D_CBaseList::MoveToHead
		(D_POSITION pos, D_CBaseList *pList)
{

	/* See the comments on the algorithm in MoveToTail */

	if (pos==NULL) return TRUE;  // no-op.  Eliminates special cases later.

	/* Make cMove the number of nodes to move */
	CNode * p = (CNode *)pos;
	int cMove = 0;			// number of nodes to move
	while(p!=NULL) {
		p = p->Next();
		++cMove;
	}


	/* Join the two chains together */
	if (pList->m_pFirst!=NULL)
		pList->m_pFirst->SetPrev(m_pLast);
	if (m_pLast!=NULL)
		m_pLast->SetNext(pList->m_pFirst);


	/* set first and last pointers */
	p = (CNode *)pos;


	if (pList->m_pLast==NULL)
		pList->m_pLast = m_pLast;

	m_pLast = p->Prev();
	if (m_pLast==NULL)
		m_pFirst = NULL;
	pList->m_pFirst = p;


	/* Break the chain after p to create the new pieces */
	if (m_pLast!=NULL)
		m_pLast->SetNext(NULL);
	p->SetPrev(NULL);


	/* Adjust the counts */
	m_Count -= cMove;
	pList->m_Count += cMove;

	return TRUE;

} // MoveToHead



/* Reverse the order of the [pointers to] objects in *this
*/
void D_CBaseList::Reverse()
{
	/* algorithm:
		The obvious booby trap is that you flip pointers around and lose
		addressability to the node that you are going to process next.
		The easy way to avoid this is do do one chain at a time.

		Run along the forward chain,
		For each node, set the reverse pointer to the one ahead of us.
		The reverse chain is now a copy of the old forward chain, including
		the NULL termination.

		Run along the reverse chain (i.e. old forward chain again)
		For each node set the forward pointer of the node ahead to point back
		to the one we're standing on.
		The first node needs special treatment,
		it's new forward pointer is NULL.
		Finally set the First/Last pointers

	*/
	CNode * p;

	// Yes we COULD use a traverse, but it would look funny!
	p = m_pFirst;
	while (p!=NULL) {
		CNode * q;
		q = p->Next();
		p->SetNext(p->Prev());
		p->SetPrev(q);
		p = q;
	}

	p = m_pFirst;
	m_pFirst = m_pLast;
	m_pLast = p;


#if 0	 // old version

	if (m_pFirst==NULL) return;		  // empty list
	if (m_pFirst->Next()==NULL) return;  // single node list


	/* run along forward chain */
	for ( p = m_pFirst
		; p!=NULL
		; p = p->Next()
		){
		p->SetPrev(p->Next());
	}


	/* special case first element */
	m_pFirst->SetNext(NULL);	 // fix the old first element


	/* run along new reverse chain i.e. old forward chain again */
	for ( p = m_pFirst			// start at the old first element
		; p->Prev()!=NULL		// while there's a node still to be set
		; p = p->Prev()		  // work in the same direction as before
		){
		p->Prev()->SetNext(p);
	}


	/* fix forward and reverse pointers
		- the triple XOR swap would work but all the casts look hideous */
	p = m_pFirst;
	m_pFirst = m_pLast;
	m_pLast = p;
#endif

} // Reverse




//コンストラクタ
D_CMovieRender::D_CMovieRender( D_IUnknown * pUnk, HRESULT *phr ) :
	D_CBaseVideoRenderer( CLSID_MovieRenderer, NAME("Movie Renderer"), pUnk, phr),
	SetMediaTypeFlag( 0 ),
	ImageBuffer( NULL ),
	YImageBuffer( NULL ),
	UVImageBuffer( NULL ),
	TempBaseImage( NULL ),
	UseTempBaseImage( 0 ),
	UseTempBaseImageFirstFrame( 0 ),
	AlwaysBaseImage( 0 ),
	AlwaysBaseImageUseTempBaseImage( 0 )
{
	ASSERT(phr) ;
	if( phr )
	{
		*phr = S_OK ;
	}
}

//デストラクタ
D_CMovieRender::~D_CMovieRender()
{
	if( ImageBuffer )
	{
		DXFREE( ImageBuffer ) ;
		ImageBuffer = NULL ;
	}

	if( YImageBuffer )
	{
		DXFREE( YImageBuffer ) ;
		YImageBuffer = NULL ;
		UVImageBuffer = NULL ;
	}

	if( TempBaseImage )
	{
		NS_ReleaseBaseImage( TempBaseImage ) ;
		delete TempBaseImage ;
		TempBaseImage = NULL ;
	}
}

ULONG __stdcall D_CMovieRender::Release()
{
	if( ImageBuffer )
	{
		DXFREE( ImageBuffer ) ;
		ImageBuffer = NULL ;
	}

	if( YImageBuffer )
	{
		DXFREE( YImageBuffer ) ;
		YImageBuffer = NULL ;
		UVImageBuffer = NULL ;
	}

	if( TempBaseImage )
	{
		NS_ReleaseBaseImage( TempBaseImage ) ;
		delete TempBaseImage ;
		TempBaseImage = NULL ;
	}

	return D_CBaseVideoRenderer::Release();
}


HRESULT D_CMovieRender::CheckMediaType( const D_CMediaType *pMediaType )
{
	GUID SubType;
	GUID FormatType;

	CheckPointer(pMediaType,E_POINTER);

	FormatType = *pMediaType->FormatType();
	if( FormatType != FORMAT_VIDEOINFO )
	{
		return E_INVALIDARG;
	}

	if( !IsEqualGUID( *pMediaType->Type(), MEDIATYPE_VIDEO ) )
	{
		return E_FAIL ;
	}

	SubType = *pMediaType->Subtype();
	if( IsEqualGUID( SubType, D_MEDIASUBTYPE_RGB8 ) )
	{
		ImageType = 10 ;	// RGB8
		return S_OK;
	}

	if( IsEqualGUID( SubType, D_MEDIASUBTYPE_RGB555 ) )
	{
		ImageType = 9 ;	// RGB555
		return S_OK;
	}

	if( IsEqualGUID( SubType, D_MEDIASUBTYPE_RGB565 ) )
	{
		ImageType = 8 ;	// RGB565
		return S_OK;
	}

	if( IsEqualGUID( SubType, D_MEDIASUBTYPE_NV11 ) )
	{
		ImageType = 7 ;	// NV11
		return S_OK;
	}

	if( IsEqualGUID( SubType, D_MEDIASUBTYPE_YVYU ) )
	{
		ImageType = 6 ;	// YVYU
		return S_OK;
	}

	if( IsEqualGUID( SubType, D_MEDIASUBTYPE_UYVY ) )
	{
		ImageType = 5 ;	// UYVY
		return S_OK;
	}

	if( IsEqualGUID( SubType, D_MEDIASUBTYPE_YUY2 ) )
	{
		ImageType = 4 ;	// YUY2
		return S_OK;
	}

	if( IsEqualGUID( SubType, D_MEDIASUBTYPE_NV12 ) )
	{
		ImageType = 3 ;	// NV12
		return S_OK ;
	}

	if( IsEqualGUID( SubType, D_MEDIASUBTYPE_YV12 ) )
	{
		ImageType = 2 ;	// YV12
		return S_OK ;
	}

	if( IsEqualGUID( SubType, D_MEDIASUBTYPE_RGB32 ) )
	{
		ImageType = 1 ;	// RGB32
		return S_OK ;
	}

	if( IsEqualGUID( SubType, D_MEDIASUBTYPE_RGB24 ) )
	{
		ImageType = 0 ;	// RGB24
		return S_OK;
	}

	return E_FAIL;
}

void D_CMovieRender::AllocImageBuffer()
{
	if( SetMediaTypeFlag == 0 )
		return ;

	if( AlwaysBaseImageUseTempBaseImage == 1 && TempBaseImage == NULL )
	{
		TempBaseImage = new BASEIMAGE ;
		NS_CreateXRGB8ColorBaseImage( ( int )Width, ( int )Height, TempBaseImage ) ;
	}

	if( ImageBuffer == NULL )
	{
		ImageBuffer = DXALLOC( Pitch * Height ) ;
	}

	if( YImageBuffer == NULL && 
		( UseTempBaseImage == 0 &&
			ImageType != 0 /*RGB24*/ && ImageType != 1 /*RGB32*/ && ImageType != 8 /*RGB565*/ && ImageType != 9 /*RGB555*/ && ImageType != 10 /*RGB8*/ ) )
	{
		YImageBuffer = DXALLOC( YPitch * YHeight + UVPitch * UVHeight ) ;
		UVImageBuffer = ( BYTE * )YImageBuffer + YPitch * YHeight ;
	}
}

HRESULT D_CMovieRender::SetMediaType( const D_CMediaType *pMediaType )
{
	D_VIDEOINFO *VideoInfo ;
	int i ;

	VideoInfo = ( D_VIDEOINFO * )pMediaType->Format() ;
	Width  = ( DWORD )VideoInfo->bmiHeader.biWidth ;
	if( VideoInfo->bmiHeader.biHeight >= 0 )
	{
		ImageReverse = TRUE ;
		Height =  ( DWORD )VideoInfo->bmiHeader.biHeight ;
	}
	else
	{
		ImageReverse = FALSE ;
		Height = ( DWORD )( -VideoInfo->bmiHeader.biHeight ) ;
	}

	if( TempBaseImage )
	{
		NS_ReleaseBaseImage( TempBaseImage ) ;
		delete TempBaseImage ;
		TempBaseImage = NULL ;
	}

	switch( ImageType )
	{
	case 0 :	// RGB24
		Pitch  = ( Width * 3 + 3 ) / 4 * 4 ;
		break ;

	case 1 :	// RGB32
		Pitch  = ( Width * 4 + 3 ) / 4 * 4 ;
		break ;

	case 2 :	// YV12
		ImageReverse = FALSE ;
		Pitch = ( Width * 4 + 3 ) / 4 * 4 ;
		AlwaysBaseImageUseTempBaseImage = 1 ;
		UseTempBaseImage = GetValidShaderVersion() >= 200 ? 0 : 1 ;
		YWidth   = Width ;
		YHeight  = Height ;
		YPitch   = Width ;
		UVWidth  = Width  / 2 ;
		UVHeight = Height / 2 ;
		UVPitch  = UVWidth * 2 ;
		break ;

	case 3 :	// NV12
		ImageReverse = FALSE ;
		Pitch = ( Width * 4 + 3 ) / 4 * 4 ;
		AlwaysBaseImageUseTempBaseImage = 1 ;
		UseTempBaseImage = GetValidShaderVersion() >= 200 ? 0 : 1 ;
		YWidth   = Width ;
		YHeight  = Height ;
		YPitch   = Width ;
		UVWidth  = Width / 2 ;
		UVHeight = Height / 2 ;
		UVPitch  = UVWidth * 2 ;
		break ;

	case 4 :	// YUY2
		ImageReverse = FALSE ;
		Pitch = ( Width * 4 + 3 ) / 4 * 4 ;
		AlwaysBaseImageUseTempBaseImage = 1 ;
		UseTempBaseImage = GetValidShaderVersion() >= 200 ? 0 : 1 ;
		YWidth   = Width ;
		YHeight  = Height ;
		YPitch   = Width ;
		UVWidth  = Width  / 2 ;
		UVHeight = Height ;
		UVPitch  = UVWidth * 2 ;
		break ;

	case 5 :	// UYVY
		ImageReverse = FALSE ;
		Pitch = ( Width * 4 + 3 ) / 4 * 4 ;
		AlwaysBaseImageUseTempBaseImage = 1 ;
		UseTempBaseImage = GetValidShaderVersion() >= 200 ? 0 : 1 ;
		YWidth   = Width ;
		YHeight  = Height ;
		YPitch   = Width ;
		UVWidth  = Width  / 2 ;
		UVHeight = Height ;
		UVPitch  = UVWidth * 2 ;
		break ;

	case 6 :	// YVYU
		ImageReverse = FALSE ;
		Pitch = ( Width * 4 + 3 ) / 4 * 4 ;
		AlwaysBaseImageUseTempBaseImage = 1 ;
		UseTempBaseImage = GetValidShaderVersion() >= 200 ? 0 : 1 ;
		YWidth   = Width ;
		YHeight  = Height ;
		YPitch   = Width ;
		UVWidth  = Width  / 2 ;
		UVHeight = Height ;
		UVPitch  = UVWidth * 2 ;
		break ;

	case 7 :	// NV11
		ImageReverse = FALSE ;
		Pitch = ( Width * 4 + 3 ) / 4 * 4 ;
		AlwaysBaseImageUseTempBaseImage = 1 ;
		UseTempBaseImage = GetValidShaderVersion() >= 200 ? 0 : 1 ;
		YWidth   = Width ;
		YHeight  = Height ;
		YPitch   = Width ;
		UVWidth  = Width  / 4 ;
		UVHeight = Height ;
		UVPitch  = UVWidth * 2 ;
		break ;

	case 8 :	// RGB565
		Pitch  = ( Width * 2 + 3 ) / 4 * 4 ;
		break ;

	case 9 :	// RGB555
		Pitch  = ( Width * 2 + 3 ) / 4 * 4 ;
		break ;

	case 10 :	// RGB8
		Pitch  = ( Width + 3 ) / 4 * 4 ;
		for( i = 0 ; i < 256 ; i ++ )
		{
			Palette[ i ] = VideoInfo->bmiColors[ i ] ;
		}
		break ;
	}

	if( ImageBuffer )
	{
		DXFREE( ImageBuffer ) ;
		ImageBuffer = NULL ;
	}

	if( YImageBuffer )
	{
		DXFREE( YImageBuffer ) ;
		YImageBuffer = NULL ;
		UVImageBuffer = NULL ;
	}

	AllocImageBuffer() ;

	NewImageSet = 0 ;
	SetMediaTypeFlag = 1 ;

	return S_OK;
}

HRESULT D_CMovieRender::DoRenderSample( D_IMediaSample * pSample )
{
	BYTE *Src, *Dest ;
	DWORD i ;

	CheckPointer( pSample, E_POINTER ) ;

	AllocImageBuffer() ;

	pSample->GetPointer( &Src );

	if( ImageBuffer &&
		( AlwaysBaseImage == 1 ||				// 外部から BASEIMAGE の構築を依頼されているか
		  YImageBuffer == NULL ||				// シェーダーが使用できないか
		  UseTempBaseImageFirstFrame == 0 ) )	// 最初のフレームだったら BASEIMAGE を構築する
	{
		BYTE *UseSrc = Src ;

		UseTempBaseImageFirstFrame = 1 ;
		if( AlwaysBaseImageUseTempBaseImage == 1 )
		{
			switch( ImageType )
			{
			case 2 :	// YV12
				ConvertYV12ToXRGB32( Src, ( int )Width, ( int )Height, TempBaseImage ) ;
				break ;

			case 3 :	// NV12
				ConvertNV12ToXRGB32( Src, ( int )Width, ( int )Height, TempBaseImage ) ;
				break ;

			case 4 :	// YUY2
				ConvertYUY2ToXRGB32( Src, ( int )Width, ( int )Height, TempBaseImage ) ;
				break ;

			case 5 :	// UYVY
				ConvertUYVYToXRGB32( Src, ( int )Width, ( int )Height, TempBaseImage ) ;
				break ;

			case 6 :	// UYVY
				ConvertYVYUToXRGB32( Src, ( int )Width, ( int )Height, TempBaseImage ) ;
				break ;

			case 7 :	// NV11
				ConvertNV11ToXRGB32( Src, ( int )Width, ( int )Height, TempBaseImage ) ;
				break ;
			}
			UseSrc = ( BYTE * )TempBaseImage->GraphData ;
		}

		if( ImageReverse )
		{
			Dest = ( BYTE * )ImageBuffer + ( Height - 1 ) * Pitch ;
			for( i = Height ; i ; i -- )
			{
				_MEMCPY( Dest, UseSrc, Pitch ) ;
				Dest -= Pitch ;
				UseSrc  += Pitch ;
			}
		}
		else
		{
			_MEMCPY( ImageBuffer, UseSrc, Pitch * Height ) ;
		}

		NewImageSet = 1 ;
	}

	if( YImageBuffer )
	{
		switch( ImageType )
		{
		case 2 :	// YV12
			ConvertYV12ToYPlane_UVPlane( Src, ( int )YWidth, ( int )YHeight, YImageBuffer, UVImageBuffer ) ;
			break ;

		case 3 :	// NV12
			ConvertNV12ToYPlane_UVPlane( Src, ( int )YWidth, ( int )YHeight, YImageBuffer, UVImageBuffer ) ;
			break ;

		case 4 :	// YUY2
			ConvertYUY2ToYPlane_UVPlane( Src, ( int )YWidth, ( int )YHeight, YImageBuffer, UVImageBuffer ) ;
			break ;

		case 5 :	// UYVY
			ConvertUYVYToYPlane_UVPlane( Src, ( int )YWidth, ( int )YHeight, YImageBuffer, UVImageBuffer ) ;
			break ;

		case 6 :	// UYVY
			ConvertYVYUToYPlane_UVPlane( Src, ( int )YWidth, ( int )YHeight, YImageBuffer, UVImageBuffer ) ;
			break ;

		case 7 :	// NV11
			ConvertNV11ToYPlane_UVPlane( Src, ( int )YWidth, ( int )YHeight, YImageBuffer, UVImageBuffer ) ;
			break ;
		}

		NewImageSet = 1 ;
	}

	return S_OK;
}

extern	D_CMovieRender *New_D_CMovieRender( D_IUnknown * pUnk, HRESULT *phr )
{
	return new D_CMovieRender( pUnk, phr ) ;
}


#ifndef DX_NON_DSHOW_MP3



D_CBaseOutputPin::D_CBaseOutputPin( TCHAR *pObjectName, D_CBaseFilter *pFilter, D_CCritSec *pLock, HRESULT *phr, LPCWSTR pName) :
    D_CBasePin(pObjectName, pFilter, pLock, phr, pName, D_PINDIR_OUTPUT),
    m_pAllocator(NULL),
    m_pInputPin(NULL)
{
    ASSERT(pFilter);
}

#ifdef UNICODE

D_CBaseOutputPin::D_CBaseOutputPin(CHAR *pObjectName,
                   D_CBaseFilter *pFilter,
                   D_CCritSec *pLock,
                   HRESULT *phr,
                   LPCWSTR pName) :
    D_CBasePin(pObjectName, pFilter, pLock, phr, pName, D_PINDIR_OUTPUT),
    m_pAllocator(NULL),
    m_pInputPin(NULL)
{
    ASSERT(pFilter);
}
#endif

/*   This is called after a media type has been proposed

     Try to complete the connection by agreeing the allocator
*/
HRESULT D_CBaseOutputPin::CompleteConnect(D_IPin *pReceivePin)
{
    UNREFERENCED_PARAMETER(pReceivePin);
    return DecideAllocator(m_pInputPin, &m_pAllocator);
}


/* This method is called when the output pin is about to try and connect to
   an input pin. It is at this point that you should try and grab any extra
   interfaces that you need, in this case D_IMemInputPin. Because this is
   only called if we are not currently connected we do NOT need to call
   BreakConnect. This also makes it easier to derive classes from us as
   BreakConnect is only called when we actually have to break a connection
   (or a partly made connection) and not when we are checking a connection */

/* Overriden from D_CBasePin */

HRESULT D_CBaseOutputPin::CheckConnect(D_IPin * pPin)
{
    HRESULT hr = D_CBasePin::CheckConnect(pPin);
    if (FAILED(hr)) {
    return hr;
    }

    // get an input pin and an allocator interface
    hr = pPin->QueryInterface(IID_IMEMINPUTPIN, (void **) &m_pInputPin);
    if (FAILED(hr)) {
        return hr;
    }
    return NOERROR;
}


/* Overriden from D_CBasePin */

HRESULT D_CBaseOutputPin::BreakConnect()
{
    /* Release any allocator we hold */

    if (m_pAllocator) {
        // Always decommit the allocator because a downstream filter may or
        // may not decommit the connection's allocator.  A memory leak could
        // occur if the allocator is not decommited when a connection is broken.
        HRESULT hr = m_pAllocator->Decommit();
        if( FAILED( hr ) ) {
            return hr;
        }

        m_pAllocator->Release();
        m_pAllocator = NULL;
    }

    /* Release any input pin interface we hold */

    if (m_pInputPin) {
        m_pInputPin->Release();
        m_pInputPin = NULL;
    }
    return NOERROR;
}


/* This is called when the input pin didn't give us a valid allocator */

HRESULT D_CBaseOutputPin::InitAllocator(D_IMemAllocator **ppAlloc)
{
    return CreateMemoryAllocator(ppAlloc);
}


/* Decide on an allocator, override this if you want to use your own allocator
   Override DecideBufferSize to call SetProperties. If the input pin fails
   the GetAllocator call then this will construct a D_CMemAllocator and call
   DecideBufferSize on that, and if that fails then we are completely hosed.
   If the you succeed the DecideBufferSize call, we will notify the input
   pin of the selected allocator. NOTE this is called during Connect() which
   therefore looks after grabbing and locking the object's critical section */

// We query the input pin for its requested properties and pass this to
// DecideBufferSize to allow it to fulfill requests that it is happy
// with (eg most people don't care about alignment and are thus happy to
// use the downstream pin's alignment request).

HRESULT D_CBaseOutputPin::DecideAllocator(D_IMemInputPin *pPin, D_IMemAllocator **ppAlloc)
{
    HRESULT hr = NOERROR;
    *ppAlloc = NULL;

    // get downstream prop request
    // the derived class may modify this in DecideBufferSize, but
    // we assume that he will consistently modify it the same way,
    // so we only get it once
    D_ALLOCATOR_PROPERTIES prop;
    _MEMSET(&prop, 0, sizeof(prop));

    // whatever he returns, we assume prop is either all zeros
    // or he has filled it out.
    pPin->GetAllocatorRequirements(&prop);

    // if he doesn't care about alignment, then set it to 1
    if (prop.cbAlign == 0) {
        prop.cbAlign = 1;
    }

    /* Try the allocator provided by the input pin */

    hr = pPin->GetAllocator(ppAlloc);
    if (SUCCEEDED(hr)) {

        hr = DecideBufferSize(*ppAlloc, &prop);
        if (SUCCEEDED(hr)) {
            hr = pPin->NotifyAllocator(*ppAlloc, FALSE);
            if (SUCCEEDED(hr)) {
                return NOERROR;
            }
        }
    }

    /* If the GetAllocator failed we may not have an interface */

    if (*ppAlloc) {
        (*ppAlloc)->Release();
        *ppAlloc = NULL;
    }

    /* Try the output pin's allocator by the same method */

    hr = InitAllocator(ppAlloc);
    if (SUCCEEDED(hr)) {

        // note - the properties passed here are in the same
        // structure as above and may have been modified by
        // the previous call to DecideBufferSize
        hr = DecideBufferSize(*ppAlloc, &prop);
        if (SUCCEEDED(hr)) {
            hr = pPin->NotifyAllocator(*ppAlloc, FALSE);
            if (SUCCEEDED(hr)) {
                return NOERROR;
            }
        }
    }

    /* Likewise we may not have an interface to release */

    if (*ppAlloc) {
        (*ppAlloc)->Release();
        *ppAlloc = NULL;
    }
    return hr;
}


/* This returns an empty sample buffer from the allocator WARNING the same
   dangers and restrictions apply here as described below for Deliver() */

HRESULT D_CBaseOutputPin::GetDeliveryBuffer(D_IMediaSample ** ppSample,
                                  D_REFERENCE_TIME * pStartTime,
                                  D_REFERENCE_TIME * pEndTime,
                                  DWORD dwFlags)
{
    if (m_pAllocator != NULL) {
        return m_pAllocator->GetBuffer(ppSample,pStartTime,pEndTime,dwFlags);
    } else {
        return E_NOINTERFACE;
    }
}


/* Deliver a filled-in sample to the connected input pin. NOTE the object must
   have locked itself before calling us otherwise we may get halfway through
   executing this method only to find the filter graph has got in and
   disconnected us from the input pin. If the filter has no worker threads
   then the lock is best applied on Receive(), otherwise it should be done
   when the worker thread is ready to deliver. There is a wee snag to worker
   threads that this shows up. The worker thread must lock the object when
   it is ready to deliver a sample, but it may have to wait until a state
   change has completed, but that may never complete because the state change
   is waiting for the worker thread to complete. The way to handle this is for
   the state change code to grab the critical section, then set an abort event
   for the worker thread, then release the critical section and wait for the
   worker thread to see the event we set and then signal that it has finished
   (with another event). At which point the state change code can complete */

// note (if you've still got any breath left after reading that) that you
// need to release the sample yourself after this call. if the connected
// input pin needs to hold onto the sample beyond the call, it will addref
// the sample itself.

// of course you must release this one and call GetDeliveryBuffer for the
// next. You cannot reuse it directly.

HRESULT D_CBaseOutputPin::Deliver(D_IMediaSample * pSample)
{
    if (m_pInputPin == NULL) {
        return VFW_E_NOT_CONNECTED;
    }


    return m_pInputPin->Receive(pSample);
}


// called from elsewhere in our filter to pass EOS downstream to
// our connected input pin
HRESULT D_CBaseOutputPin::DeliverEndOfStream(void)
{
    // remember this is on D_IPin not D_IMemInputPin
    if (m_Connected == NULL) {
        return VFW_E_NOT_CONNECTED;
    }
    return m_Connected->EndOfStream();
}


/* Commit the allocator's memory, this is called through IMediaFilter
   which is responsible for locking the object before calling us */
HRESULT D_CBaseOutputPin::Active(void)
{
    if (m_pAllocator == NULL) {
        return VFW_E_NO_ALLOCATOR;
    }
    return m_pAllocator->Commit();
}


/* Free up or unprepare allocator's memory, this is called through
   IMediaFilter which is responsible for locking the object first */

HRESULT D_CBaseOutputPin::Inactive(void)
{
    m_bRunTimeError = FALSE;
    if (m_pAllocator == NULL) {
        return VFW_E_NO_ALLOCATOR;
    }
    return m_pAllocator->Decommit();
}

// we have a default handling of EndOfStream which is to return
// an error, since this should be called on input pins only
STDMETHODIMP D_CBaseOutputPin::EndOfStream(void)
{
    return E_UNEXPECTED;
}


// BeginFlush should be called on input pins only
STDMETHODIMP D_CBaseOutputPin::BeginFlush(void)
{
    return E_UNEXPECTED;
}

// EndFlush should be called on input pins only
STDMETHODIMP D_CBaseOutputPin::EndFlush(void)
{
    return E_UNEXPECTED;
}

// call BeginFlush on the connected input pin
HRESULT D_CBaseOutputPin::DeliverBeginFlush(void)
{
    // remember this is on D_IPin not D_IMemInputPin
    if (m_Connected == NULL) {
        return VFW_E_NOT_CONNECTED;
    }
    return m_Connected->BeginFlush();
}

// call EndFlush on the connected input pin
HRESULT D_CBaseOutputPin::DeliverEndFlush(void)
{
    // remember this is on D_IPin not D_IMemInputPin
    if (m_Connected == NULL) {
        return VFW_E_NOT_CONNECTED;
    }
    return m_Connected->EndFlush();
}
// deliver NewSegment to connected pin
HRESULT D_CBaseOutputPin::DeliverNewSegment(
    D_REFERENCE_TIME tStart,
    D_REFERENCE_TIME tStop,
    double dRate)
{
    if (m_Connected == NULL) {
        return VFW_E_NOT_CONNECTED;
    }
    return m_Connected->NewSegment(tStart, tStop, dRate);
}



D_CTransformFilter::D_CTransformFilter(TCHAR     *pName,
                                   D_IUnknown * pUnk,
                                   REFCLSID  clsid) :
    D_CBaseFilter(pName,pUnk,&m_csFilter, clsid),
    m_pInput(NULL),
    m_pOutput(NULL),
    m_bEOSDelivered(FALSE),
    m_bQualityChanged(FALSE),
    m_bSampleSkipped(FALSE)
{
#ifdef PERF
    RegisterPerfId();
#endif //  PERF
}

#ifdef UNICODE

D_CTransformFilter::D_CTransformFilter(char     *pName,
                                   D_IUnknown * pUnk,
                                   REFCLSID  clsid) :
    D_CBaseFilter(pName,pUnk,&m_csFilter, clsid),
    m_pInput(NULL),
    m_pOutput(NULL),
    m_bEOSDelivered(FALSE),
    m_bQualityChanged(FALSE),
    m_bSampleSkipped(FALSE)
{
#ifdef PERF
    RegisterPerfId();
#endif //  PERF
}
#endif

// destructor

D_CTransformFilter::~D_CTransformFilter()
{
    // Delete the pins

    delete m_pInput;
    delete m_pOutput;
}


// Transform place holder - should never be called
HRESULT D_CTransformFilter::Transform(D_IMediaSample * pIn, D_IMediaSample *pOut)
{
    UNREFERENCED_PARAMETER(pIn);
    UNREFERENCED_PARAMETER(pOut);
    DbgBreak("D_CTransformFilter::Transform() should never be called");
    return E_UNEXPECTED;
}


// return the number of pins we provide

int D_CTransformFilter::GetPinCount()
{
    return 2;
}


// return a non-addrefed D_CBasePin * for the user to addref if he holds onto it
// for longer than his pointer to us. We create the pins dynamically when they
// are asked for rather than in the constructor. This is because we want to
// give the derived class an oppportunity to return different pin objects

// We return the objects as and when they are needed. If either of these fails
// then we return NULL, the assumption being that the caller will realise the
// whole deal is off and destroy us - which in turn will delete everything.

D_CBasePin *
D_CTransformFilter::GetPin(int n)
{
    HRESULT hr = S_OK;

    // Create an input pin if necessary

    if (m_pInput == NULL) {

        m_pInput = new D_CTransformInputPin(NAME("Transform input pin"),
                                          this,              // Owner filter
                                          &hr,               // Result code
                                          L"XForm In");      // Pin name


        //  Can't fail
        ASSERT(SUCCEEDED(hr));
        if (m_pInput == NULL) {
            return NULL;
        }
        m_pOutput = (D_CTransformOutputPin *)
		   new D_CTransformOutputPin(NAME("Transform output pin"),
                                            this,            // Owner filter
                                            &hr,             // Result code
                                            L"XForm Out");   // Pin name


        // Can't fail
        ASSERT(SUCCEEDED(hr));
        if (m_pOutput == NULL) {
            delete m_pInput;
            m_pInput = NULL;
        }
    }

    // Return the appropriate pin

    if (n == 0) {
        return m_pInput;
    } else
    if (n == 1) {
        return m_pOutput;
    } else {
        return NULL;
    }
}


//
// FindPin
//
// If Id is In or Out then return the D_IPin* for that pin
// creating the pin if need be.  Otherwise return NULL with an error.

STDMETHODIMP D_CTransformFilter::FindPin(LPCWSTR Id, D_IPin **ppPin)
{
	SETUP_WIN_API

    CheckPointer(ppPin,E_POINTER);
    ValidateReadWritePtr(ppPin,sizeof(D_IPin *));

    if (0==lstrcmpWFunc((WORD const *)Id,(WORD const *)L"In")) {
        *ppPin = GetPin(0);
    } else if (0==lstrcmpWFunc((WORD const *)Id,(WORD const *)L"Out")) {
        *ppPin = GetPin(1);
    } else {
        *ppPin = NULL;
        return VFW_E_NOT_FOUND;
    }

    HRESULT hr = NOERROR;
    //  AddRef() returned pointer - but GetPin could fail if memory is low.
    if (*ppPin) {
        (*ppPin)->AddRef();
    } else {
        hr = E_OUTOFMEMORY;  // probably.  There's no pin anyway.
    }
    return hr;
}


// override these two functions if you want to inform something
// about entry to or exit from streaming state.

HRESULT
D_CTransformFilter::StartStreaming()
{
    return NOERROR;
}


HRESULT
D_CTransformFilter::StopStreaming()
{
    return NOERROR;
}


// override this to grab extra interfaces on connection

HRESULT
D_CTransformFilter::CheckConnect(D_PIN_DIRECTION dir,D_IPin *pPin)
{
    UNREFERENCED_PARAMETER(dir);
    UNREFERENCED_PARAMETER(pPin);
    return NOERROR;
}


// place holder to allow derived classes to release any extra interfaces

HRESULT
D_CTransformFilter::BreakConnect(D_PIN_DIRECTION dir)
{
    UNREFERENCED_PARAMETER(dir);
    return NOERROR;
}


// Let derived classes know about connection completion

HRESULT
D_CTransformFilter::CompleteConnect(D_PIN_DIRECTION direction,D_IPin *pReceivePin)
{
    UNREFERENCED_PARAMETER(direction);
    UNREFERENCED_PARAMETER(pReceivePin);
    return NOERROR;
}


// override this to know when the media type is really set

HRESULT
D_CTransformFilter::SetMediaType(D_PIN_DIRECTION direction,const D_CMediaType *pmt)
{
    UNREFERENCED_PARAMETER(direction);
    UNREFERENCED_PARAMETER(pmt);
    return NOERROR;
}


// Set up our output sample
HRESULT
D_CTransformFilter::InitializeOutputSample(D_IMediaSample *pSample, D_IMediaSample **ppOutSample)
{
    D_IMediaSample *pOutSample;

    // default - times are the same

    D_AM_SAMPLE2_PROPERTIES * const pProps = m_pInput->SampleProps();
    DWORD dwFlags = ( DWORD )( m_bSampleSkipped ? AM_GBF_PREVFRAMESKIPPED : 0 );

    // This will prevent the image renderer from switching us to DirectDraw
    // when we can't do it without skipping frames because we're not on a
    // keyframe.  If it really has to switch us, it still will, but then we
    // will have to wait for the next keyframe
    if (!(pProps->dwSampleFlags & AM_SAMPLE_SPLICEPOINT)) {
	dwFlags |= AM_GBF_NOTASYNCPOINT;
    }

    ASSERT(m_pOutput->m_pAllocator != NULL);
    HRESULT hr = m_pOutput->m_pAllocator->GetBuffer(
             &pOutSample
             , pProps->dwSampleFlags & AM_SAMPLE_TIMEVALID ?
                   &pProps->tStart : NULL
             , pProps->dwSampleFlags & AM_SAMPLE_STOPVALID ?
                   &pProps->tStop : NULL
             , dwFlags
         );
    *ppOutSample = pOutSample;
    if (FAILED(hr)) {
        return hr;
    }

    ASSERT(pOutSample);
    D_IMediaSample2 *pOutSample2;
    if (SUCCEEDED(pOutSample->QueryInterface(IID_IMEDIASAMPLE2,
                                             (void **)&pOutSample2))) {
        /*  Modify it */
        D_AM_SAMPLE2_PROPERTIES OutProps;
        EXECUTE_ASSERT(SUCCEEDED(pOutSample2->GetProperties(
            FIELD_OFFSET(D_AM_SAMPLE2_PROPERTIES, tStart), (PBYTE)&OutProps)
        ));
        OutProps.dwTypeSpecificFlags = pProps->dwTypeSpecificFlags;
        OutProps.dwSampleFlags =
            (OutProps.dwSampleFlags & AM_SAMPLE_TYPECHANGED) |
            (pProps->dwSampleFlags & ~AM_SAMPLE_TYPECHANGED);
        OutProps.tStart = pProps->tStart;
        OutProps.tStop  = pProps->tStop;
        OutProps.cbData = FIELD_OFFSET(D_AM_SAMPLE2_PROPERTIES, dwStreamId);
        hr = pOutSample2->SetProperties(
            FIELD_OFFSET(D_AM_SAMPLE2_PROPERTIES, dwStreamId),
            (PBYTE)&OutProps
        );
        if (pProps->dwSampleFlags & AM_SAMPLE_DATADISCONTINUITY) {
            m_bSampleSkipped = FALSE;
        }
        pOutSample2->Release();
    } else {
        if (pProps->dwSampleFlags & AM_SAMPLE_TIMEVALID) {
            pOutSample->SetTime(&pProps->tStart,
                                &pProps->tStop);
        }
        if (pProps->dwSampleFlags & AM_SAMPLE_SPLICEPOINT) {
            pOutSample->SetSyncPoint(TRUE);
        }
        if (pProps->dwSampleFlags & AM_SAMPLE_DATADISCONTINUITY) {
            pOutSample->SetDiscontinuity(TRUE);
            m_bSampleSkipped = FALSE;
        }
        // Copy the media times

        LONGLONG MediaStart, MediaEnd;
        if (pSample->GetMediaTime(&MediaStart,&MediaEnd) == NOERROR) {
            pOutSample->SetMediaTime(&MediaStart,&MediaEnd);
        }
    }
    return S_OK;
}

// override this to customize the transform process

HRESULT
D_CTransformFilter::Receive(D_IMediaSample *pSample)
{
    /*  Check for other streams and pass them on */
    D_AM_SAMPLE2_PROPERTIES * const pProps = m_pInput->SampleProps();
    if (pProps->dwStreamId != AM_STREAM_MEDIA) {
        return m_pOutput->m_pInputPin->Receive(pSample);
    }
    HRESULT hr;
    ASSERT(pSample);
    D_IMediaSample * pOutSample;

    // If no output to deliver to then no point sending us data

    ASSERT (m_pOutput != NULL) ;

    // Set up the output sample
    hr = InitializeOutputSample(pSample, &pOutSample);

    if (FAILED(hr)) {
        return hr;
    }

    // Start timing the transform (if PERF is defined)
    MSR_START(m_idTransform);

    // have the derived class transform the data

    hr = Transform(pSample, pOutSample);

    // Stop the clock and log it (if PERF is defined)
    MSR_STOP(m_idTransform);

    if (FAILED(hr)) {
	DbgLog((LOG_TRACE,1,TEXT("Error from transform")));
    } else {
        // the Transform() function can return S_FALSE to indicate that the
        // sample should not be delivered; we only deliver the sample if it's
        // really S_OK (same as NOERROR, of course.)
        if (hr == NOERROR) {
    	    hr = m_pOutput->m_pInputPin->Receive(pOutSample);
            m_bSampleSkipped = FALSE;	// last thing no longer dropped
        } else {
            // S_FALSE returned from Transform is a PRIVATE agreement
            // We should return NOERROR from Receive() in this cause because returning S_FALSE
            // from Receive() means that this is the end of the stream and no more data should
            // be sent.
            if (S_FALSE == hr) {

                //  Release the sample before calling notify to avoid
                //  deadlocks if the sample holds a lock on the system
                //  such as DirectDraw buffers do
                pOutSample->Release();
                m_bSampleSkipped = TRUE;
                if (!m_bQualityChanged) {
                    NotifyEvent(EC_QUALITY_CHANGE,0,0);
                    m_bQualityChanged = TRUE;
                }
                return NOERROR;
            }
        }
    }

    // release the output buffer. If the connected pin still needs it,
    // it will have addrefed it itself.
    pOutSample->Release();

    return hr;
}


// Return S_FALSE to mean "pass the note on upstream"
// Return NOERROR (Same as S_OK)
// to mean "I've done something about it, don't pass it on"
HRESULT D_CTransformFilter::AlterQuality(D_Quality q)
{
    UNREFERENCED_PARAMETER(q);
    return S_FALSE;
}


// EndOfStream received. Default behaviour is to deliver straight
// downstream, since we have no queued data. If you overrode Receive
// and have queue data, then you need to handle this and deliver EOS after
// all queued data is sent
HRESULT
D_CTransformFilter::EndOfStream(void)
{
    HRESULT hr = NOERROR;
    if (m_pOutput != NULL) {
        hr = m_pOutput->DeliverEndOfStream();
    }

    return hr;
}


// enter flush state. Receives already blocked
// must override this if you have queued data or a worker thread
HRESULT
D_CTransformFilter::BeginFlush(void)
{
    HRESULT hr = NOERROR;
    if (m_pOutput != NULL) {
	// block receives -- done by caller (D_CBaseInputPin::BeginFlush)

	// discard queued data -- we have no queued data

	// free anyone blocked on receive - not possible in this filter

	// call downstream
	hr = m_pOutput->DeliverBeginFlush();
    }
    return hr;
}


// leave flush state. must override this if you have queued data
// or a worker thread
HRESULT
D_CTransformFilter::EndFlush(void)
{
    // sync with pushing thread -- we have no worker thread

    // ensure no more data to go downstream -- we have no queued data

    // call EndFlush on downstream pins
    ASSERT (m_pOutput != NULL);
    return m_pOutput->DeliverEndFlush();

    // caller (the input pin's method) will unblock Receives
}


// override these so that the derived filter can catch them

STDMETHODIMP
D_CTransformFilter::Stop()
{
    D_CAutoLock lck1(&m_csFilter);
    if (m_State == D_State_Stopped) {
        return NOERROR;
    }

    // Succeed the Stop if we are not completely connected

    ASSERT(m_pInput == NULL || m_pOutput != NULL);
    if (m_pInput == NULL || m_pInput->IsConnected() == FALSE ||
        m_pOutput->IsConnected() == FALSE) {
                m_State = D_State_Stopped;
                m_bEOSDelivered = FALSE;
                return NOERROR;
    }

    ASSERT(m_pInput);
    ASSERT(m_pOutput);

    // decommit the input pin before locking or we can deadlock
    m_pInput->Inactive();

    // synchronize with Receive calls

    D_CAutoLock lck2(&m_csReceive);
    m_pOutput->Inactive();

    // allow a class derived from D_CTransformFilter
    // to know about starting and stopping streaming

    HRESULT hr = StopStreaming();
    if (SUCCEEDED(hr)) {
	// complete the state transition
	m_State = D_State_Stopped;
	m_bEOSDelivered = FALSE;
    }
    return hr;
}


STDMETHODIMP
D_CTransformFilter::Pause()
{
    D_CAutoLock lck(&m_csFilter);
    HRESULT hr = NOERROR;

    if (m_State == D_State_Paused) {
        // (This space left deliberately blank)
    }

    // If we have no input pin or it isn't yet connected then when we are
    // asked to pause we deliver an end of stream to the downstream filter.
    // This makes sure that it doesn't sit there forever waiting for
    // samples which we cannot ever deliver without an input connection.

    else if (m_pInput == NULL || m_pInput->IsConnected() == FALSE) {
        if (m_pOutput && m_bEOSDelivered == FALSE) {
            m_pOutput->DeliverEndOfStream();
            m_bEOSDelivered = TRUE;
        }
        m_State = D_State_Paused;
    }

    // We may have an input connection but no output connection
    // However, if we have an input pin we do have an output pin

    else if (m_pOutput->IsConnected() == FALSE) {
        m_State = D_State_Paused;
    }

    else {
	if (m_State == D_State_Stopped) {
	    // allow a class derived from D_CTransformFilter
	    // to know about starting and stopping streaming
            D_CAutoLock lck2(&m_csReceive);
	    hr = StartStreaming();
	}
	if (SUCCEEDED(hr)) {
	    hr = D_CBaseFilter::Pause();
	}
    }

    m_bSampleSkipped = FALSE;
    m_bQualityChanged = FALSE;
    return hr;
}

HRESULT
D_CTransformFilter::NewSegment(
    D_REFERENCE_TIME tStart,
    D_REFERENCE_TIME tStop,
    double dRate)
{
    if (m_pOutput != NULL) {
        return m_pOutput->DeliverNewSegment(tStart, tStop, dRate);
    }
    return S_OK;
}


// Check streaming status
HRESULT
D_CTransformInputPin::CheckStreaming()
{
    ASSERT(m_pTransformFilter->m_pOutput != NULL);
    if (!m_pTransformFilter->m_pOutput->IsConnected()) {
        return VFW_E_NOT_CONNECTED;
    } else {
        //  Shouldn't be able to get any data if we're not connected!
        ASSERT(IsConnected());

        //  we're flushing
        if (m_bFlushing) {
            return S_FALSE;
        }
        //  Don't process stuff in Stopped state
        if (IsStopped()) {
            return VFW_E_WRONG_STATE;
        }
        if (m_bRunTimeError) {
    	    return VFW_E_RUNTIME_ERROR;
        }
        return S_OK;
    }
}


// =================================================================
// Implements the D_CTransformInputPin class
// =================================================================


// constructor


D_CTransformInputPin::D_CTransformInputPin(
    TCHAR *pObjectName,
    D_CTransformFilter *pTransformFilter,
    HRESULT * phr,
    LPCWSTR pName)
    : D_CBaseInputPin(pObjectName, pTransformFilter, &pTransformFilter->m_csFilter, phr, pName)
{
    DbgLog((LOG_TRACE,2,TEXT("D_CTransformInputPin::D_CTransformInputPin")));
    m_pTransformFilter = pTransformFilter;
}

#ifdef UNICODE
D_CTransformInputPin::D_CTransformInputPin(
    CHAR *pObjectName,
    D_CTransformFilter *pTransformFilter,
    HRESULT * phr,
    LPCWSTR pName)
    : D_CBaseInputPin(pObjectName, pTransformFilter, &pTransformFilter->m_csFilter, phr, pName)
{
    DbgLog((LOG_TRACE,2,TEXT("D_CTransformInputPin::D_CTransformInputPin")));
    m_pTransformFilter = pTransformFilter;
}
#endif

// provides derived filter a chance to grab extra interfaces

HRESULT
D_CTransformInputPin::CheckConnect(D_IPin *pPin)
{
    HRESULT hr = m_pTransformFilter->CheckConnect(D_PINDIR_INPUT,pPin);
    if (FAILED(hr)) {
    	return hr;
    }
    return D_CBaseInputPin::CheckConnect(pPin);
}


// provides derived filter a chance to release it's extra interfaces

HRESULT
D_CTransformInputPin::BreakConnect()
{
    //  Can't disconnect unless stopped
    ASSERT(IsStopped());
    m_pTransformFilter->BreakConnect(D_PINDIR_INPUT);
    return D_CBaseInputPin::BreakConnect();
}


// Let derived class know when the input pin is connected

HRESULT
D_CTransformInputPin::CompleteConnect(D_IPin *pReceivePin)
{
    HRESULT hr = m_pTransformFilter->CompleteConnect(D_PINDIR_INPUT,pReceivePin);
    if (FAILED(hr)) {
        return hr;
    }
    return D_CBaseInputPin::CompleteConnect(pReceivePin);
}


// check that we can support a given media type

HRESULT
D_CTransformInputPin::CheckMediaType(const D_CMediaType* pmt)
{
    // Check the input type

    HRESULT hr = m_pTransformFilter->CheckInputType(pmt);
    if (S_OK != hr) {
        return hr;
    }

    // if the output pin is still connected, then we have
    // to check the transform not just the input format

    if ((m_pTransformFilter->m_pOutput != NULL) &&
        (m_pTransformFilter->m_pOutput->IsConnected())) {
            return m_pTransformFilter->CheckTransform(
                      pmt,
		      &m_pTransformFilter->m_pOutput->CurrentMediaType());
    } else {
        return hr;
    }
}


// set the media type for this connection

HRESULT
D_CTransformInputPin::SetMediaType(const D_CMediaType* mtIn)
{
    // Set the base class media type (should always succeed)
    HRESULT hr = D_CBasePin::SetMediaType(mtIn);
    if (FAILED(hr)) {
        return hr;
    }

    // check the transform can be done (should always succeed)
    ASSERT(SUCCEEDED(m_pTransformFilter->CheckInputType(mtIn)));

    return m_pTransformFilter->SetMediaType(D_PINDIR_INPUT,mtIn);
}


// =================================================================
// Implements D_IMemInputPin interface
// =================================================================


// provide EndOfStream that passes straight downstream
// (there is no queued data)
STDMETHODIMP
D_CTransformInputPin::EndOfStream(void)
{
    D_CAutoLock lck(&m_pTransformFilter->m_csReceive);
    HRESULT hr = CheckStreaming();
    if (S_OK == hr) {
       hr = m_pTransformFilter->EndOfStream();
    }
    return hr;
}


// enter flushing state. Call default handler to block Receives, then
// pass to overridable method in filter
STDMETHODIMP
D_CTransformInputPin::BeginFlush(void)
{
    D_CAutoLock lck(&m_pTransformFilter->m_csFilter);
    //  Are we actually doing anything?
    ASSERT(m_pTransformFilter->m_pOutput != NULL);
    if (!IsConnected() ||
        !m_pTransformFilter->m_pOutput->IsConnected()) {
        return VFW_E_NOT_CONNECTED;
    }
    HRESULT hr = D_CBaseInputPin::BeginFlush();
    if (FAILED(hr)) {
    	return hr;
    }

    return m_pTransformFilter->BeginFlush();
}


// leave flushing state.
// Pass to overridable method in filter, then call base class
// to unblock receives (finally)
STDMETHODIMP
D_CTransformInputPin::EndFlush(void)
{
    D_CAutoLock lck(&m_pTransformFilter->m_csFilter);
    //  Are we actually doing anything?
    ASSERT(m_pTransformFilter->m_pOutput != NULL);
    if (!IsConnected() ||
        !m_pTransformFilter->m_pOutput->IsConnected()) {
        return VFW_E_NOT_CONNECTED;
    }

    HRESULT hr = m_pTransformFilter->EndFlush();
    if (FAILED(hr)) {
        return hr;
    }

    return D_CBaseInputPin::EndFlush();
}


// here's the next block of data from the stream.
// AddRef it yourself if you need to hold it beyond the end
// of this call.

HRESULT __stdcall
D_CTransformInputPin::Receive(D_IMediaSample * pSample)
{
    HRESULT hr;
    D_CAutoLock lck(&m_pTransformFilter->m_csReceive);
    ASSERT(pSample);

    // check all is well with the base class
    hr = D_CBaseInputPin::Receive(pSample);
    if (S_OK == hr) {
        hr = m_pTransformFilter->Receive(pSample);
    }
    return hr;
}




// override to pass downstream
STDMETHODIMP
D_CTransformInputPin::NewSegment(
    D_REFERENCE_TIME tStart,
    D_REFERENCE_TIME tStop,
    double dRate)
{
    //  Save the values in the pin
    D_CBasePin::NewSegment(tStart, tStop, dRate);
    return m_pTransformFilter->NewSegment(tStart, tStop, dRate);
}




// =================================================================
// Implements the D_CTransformOutputPin class
// =================================================================


// constructor


D_CTransformOutputPin::D_CTransformOutputPin(
    TCHAR *pObjectName,
    D_CTransformFilter *pTransformFilter,
    HRESULT * phr,
    LPCWSTR pPinName)
    : D_CBaseOutputPin(pObjectName, pTransformFilter, &pTransformFilter->m_csFilter, phr, pPinName),
      m_pPosition(NULL)
{
    DbgLog((LOG_TRACE,2,TEXT("D_CTransformOutputPin::D_CTransformOutputPin")));
    m_pTransformFilter = pTransformFilter;

}

#ifdef UNICODE
D_CTransformOutputPin::D_CTransformOutputPin(
    CHAR *pObjectName,
    D_CTransformFilter *pTransformFilter,
    HRESULT * phr,
    LPCWSTR pPinName)
    : D_CBaseOutputPin(pObjectName, pTransformFilter, &pTransformFilter->m_csFilter, phr, pPinName),
      m_pPosition(NULL)
{
    DbgLog((LOG_TRACE,2,TEXT("D_CTransformOutputPin::D_CTransformOutputPin")));
    m_pTransformFilter = pTransformFilter;

}
#endif

// destructor

D_CTransformOutputPin::~D_CTransformOutputPin()
{
    DbgLog((LOG_TRACE,2,TEXT("D_CTransformOutputPin::~D_CTransformOutputPin")));

    if (m_pPosition) m_pPosition->Release();
}


STDAPI CreatePosPassThru( D_IUnknown * pAgg, BOOL bRenderer, D_IPin *pPin, D_IUnknown **ppPassThru )
{
	SETUP_WIN_API

    *ppPassThru = NULL;
    D_IUnknown *pUnkSeek;
    HRESULT hr = WinAPIData.Win32Func.CoCreateInstanceFunc( CLSID_SEEKINGPASSTHRU, pAgg, CLSCTX_INPROC_SERVER, IID_IUNKNOWN, (void **)&pUnkSeek );
    if (FAILED(hr)) {
        return hr;
    }

    D_ISeekingPassThru *pPassThru;
    hr = pUnkSeek->QueryInterface( IID_ISEEKINGPASSTHRU, (void**)&pPassThru);
    if (FAILED(hr)) {
        pUnkSeek->Release();
        return hr;
    }
    hr = pPassThru->Init(bRenderer, pPin);
    pPassThru->Release();
    if (FAILED(hr)) {
        pUnkSeek->Release();
        return hr;
    }
    *ppPassThru = pUnkSeek;
    return S_OK;
}

// overriden to expose IMediaPosition and IMediaSeeking control interfaces

STDMETHODIMP
D_CTransformOutputPin::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    CheckPointer(ppv,E_POINTER);
    ValidateReadWritePtr(ppv,sizeof(PVOID));
    *ppv = NULL;

    if (riid == IID_IMEDIAPOSITION || riid == IID_IMEDIASEEKING) {

        // we should have an input pin by now

        ASSERT(m_pTransformFilter->m_pInput != NULL);

        if (m_pPosition == NULL) {

            HRESULT hr = CreatePosPassThru(
                             GetOwner(),
                             FALSE,
                             (D_IPin *)m_pTransformFilter->m_pInput,
                             &m_pPosition);
            if (FAILED(hr)) {
                return hr;
            }
        }
        return m_pPosition->QueryInterface(riid, ppv);
    } else {
        return D_CBaseOutputPin::NonDelegatingQueryInterface(riid, ppv);
    }
}


// provides derived filter a chance to grab extra interfaces

HRESULT
D_CTransformOutputPin::CheckConnect(D_IPin *pPin)
{
    // we should have an input connection first

    ASSERT(m_pTransformFilter->m_pInput != NULL);
    if ((m_pTransformFilter->m_pInput->IsConnected() == FALSE)) {
	    return E_UNEXPECTED;
    }

    HRESULT hr = m_pTransformFilter->CheckConnect(D_PINDIR_OUTPUT,pPin);
    if (FAILED(hr)) {
	    return hr;
    }
    return D_CBaseOutputPin::CheckConnect(pPin);
}


// provides derived filter a chance to release it's extra interfaces

HRESULT
D_CTransformOutputPin::BreakConnect()
{
    //  Can't disconnect unless stopped
    ASSERT(IsStopped());
    m_pTransformFilter->BreakConnect(D_PINDIR_OUTPUT);
    return D_CBaseOutputPin::BreakConnect();
}


// Let derived class know when the output pin is connected

HRESULT
D_CTransformOutputPin::CompleteConnect(D_IPin *pReceivePin)
{
    HRESULT hr = m_pTransformFilter->CompleteConnect(D_PINDIR_OUTPUT,pReceivePin);
    if (FAILED(hr)) {
        return hr;
    }
    return D_CBaseOutputPin::CompleteConnect(pReceivePin);
}


// check a given transform - must have selected input type first

HRESULT
D_CTransformOutputPin::CheckMediaType(const D_CMediaType* pmtOut)
{
    // must have selected input first
    ASSERT(m_pTransformFilter->m_pInput != NULL);
    if ((m_pTransformFilter->m_pInput->IsConnected() == FALSE)) {
	        return E_INVALIDARG;
    }

    return m_pTransformFilter->CheckTransform(
				    &m_pTransformFilter->m_pInput->CurrentMediaType(),
				    pmtOut);
}


// called after we have agreed a media type to actually set it in which case
// we run the CheckTransform function to get the output format type again

HRESULT
D_CTransformOutputPin::SetMediaType(const D_CMediaType* pmtOut)
{
    HRESULT hr = NOERROR;
    ASSERT(m_pTransformFilter->m_pInput != NULL);

    ASSERT(m_pTransformFilter->m_pInput->CurrentMediaType().IsValid());

    // Set the base class media type (should always succeed)
    hr = D_CBasePin::SetMediaType(pmtOut);
    if (FAILED(hr)) {
        return hr;
    }

#ifdef DEBUG
    if (FAILED(m_pTransformFilter->CheckTransform(&m_pTransformFilter->
					m_pInput->CurrentMediaType(),pmtOut))) {
	DbgLog((LOG_ERROR,0,TEXT("*** This filter is accepting an output media type")));
	DbgLog((LOG_ERROR,0,TEXT("    that it can't currently transform to.  I hope")));
	DbgLog((LOG_ERROR,0,TEXT("    it's smart enough to reconnect its input.")));
    }
#endif

    return m_pTransformFilter->SetMediaType(D_PINDIR_OUTPUT,pmtOut);
}


// pass the buffer size decision through to the main transform class

HRESULT
D_CTransformOutputPin::DecideBufferSize(
    D_IMemAllocator * pAllocator,
    D_ALLOCATOR_PROPERTIES* pProp)
{
    return m_pTransformFilter->DecideBufferSize(pAllocator, pProp);
}



// return a specific media type indexed by iPosition

HRESULT
D_CTransformOutputPin::GetMediaType(
    int iPosition,
    D_CMediaType *pMediaType)
{
    ASSERT(m_pTransformFilter->m_pInput != NULL);

    //  We don't have any media types if our input is not connected

    if (m_pTransformFilter->m_pInput->IsConnected()) {
        return m_pTransformFilter->GetMediaType(iPosition,pMediaType);
    } else {
        return VFW_S_NO_MORE_ITEMS;
    }
}


// Override this if you can do something constructive to act on the
// quality message.  Consider passing it upstream as well

// Pass the quality mesage on upstream.

STDMETHODIMP
D_CTransformOutputPin::Notify(D_IBaseFilter * pSender, D_Quality q)
{
    UNREFERENCED_PARAMETER(pSender);
    ValidateReadPtr(pSender,sizeof(D_IBaseFilter));

    // First see if we want to handle this ourselves
    HRESULT hr = m_pTransformFilter->AlterQuality(q);
    if (hr!=S_FALSE) {
        return hr;        // either S_OK or a failure
    }

    // S_FALSE means we pass the message on.
    // Find the quality sink for our input pin and send it there

    ASSERT(m_pTransformFilter->m_pInput != NULL);

    return m_pTransformFilter->m_pInput->PassNotify(q);

} // Notify









// ------------------------------------------------------------------------
// filter constructor

#pragma warning(disable:4355)


D_CWavDestFilter::D_CWavDestFilter(D_IUnknown * pUnk, HRESULT *phr) :
                D_CTransformFilter(NAME("WavDest filter"), pUnk, CLSID_WavDest)
{
    ASSERT(m_pOutput == 0);
    ASSERT(phr);

    if(SUCCEEDED(*phr))
    {
        // Create an output pin so we can have control over the connection
        // media type.
        D_CWavDestOutputPin *pOut = new D_CWavDestOutputPin(this, phr);

        if(pOut)
        {
            if(SUCCEEDED(*phr))
            {
                m_pOutput = pOut;
            }
            else
            {
                delete pOut;
            }
        }
        else
        {
            *phr = E_OUTOFMEMORY;
        }

        //
        // NOTE!: If we've created our own output pin we must also create
        // the input pin ourselves because the D_CTransformFilter base class 
        // will create an extra output pin if the input pin wasn't created.        
        //
        D_CTransformInputPin *pIn = new D_CTransformInputPin(NAME("Transform input pin"),
                                        this,              // Owner filter
                                        phr,               // Result code
                                        L"In");            // Pin name
        // a failed return code should delete the object
        if(pIn)
        {
            if(SUCCEEDED(*phr))
            {
                m_pInput = pIn;
            }
            else
            {
                delete pIn;
            }
        }
        else
        {
            *phr = E_OUTOFMEMORY;
        }
    }
}


// ------------------------------------------------------------------------
// destructor

D_CWavDestFilter::~D_CWavDestFilter()
{
}


D_CUnknown * WINAPI D_CWavDestFilter::CreateInstance(D_IUnknown * pUnk, HRESULT * phr)
{
    return new D_CWavDestFilter(pUnk, phr);
}


//
// D_CWavDestFilter::CheckTransform
//
// To be able to transform, the formats must be identical
//
HRESULT D_CWavDestFilter::CheckTransform(const D_CMediaType *mtIn, const D_CMediaType * /*mtOut*/)
{
    HRESULT hr;

    if(FAILED(hr = CheckInputType(mtIn)))
    {
        return hr;
    }

    return NOERROR;

} // CheckTransform


// overridden because we need to know if Deliver() failed.

HRESULT D_CWavDestFilter::Receive(D_IMediaSample *pSample)
{
    ULONG cbOld = m_cbWavData;
    HRESULT hr = D_CTransformFilter::Receive(pSample);

    // don't update the count if Deliver() downstream fails.
    if(hr != S_OK)
    {
        m_cbWavData = cbOld;
    }

    return hr;
}

//
// D_CWavDestFilter::Transform
//
//
HRESULT D_CWavDestFilter::Transform(D_IMediaSample *pIn, D_IMediaSample *pOut)
{
    D_REFERENCE_TIME rtStart, rtEnd;

    // First just copy the data to the output sample
    HRESULT hr = Copy(pIn, pOut);
    if(FAILED(hr))
    {
        return hr;
    }

    // Prepare it for writing    
    LONG lActual = pOut->GetActualDataLength();

    if(m_cbWavData + m_cbHeader + lActual < m_cbWavData + m_cbHeader)
    { 
        return E_FAIL;      // overflow
    }

    rtStart = m_cbWavData + m_cbHeader;
    rtEnd   = rtStart + lActual;
    m_cbWavData += lActual;

    EXECUTE_ASSERT(pOut->SetTime(&rtStart, &rtEnd) == S_OK);

    return S_OK;
}


//
// D_CWavDestFilter::Copy
//
// Make destination an identical copy of source
//
HRESULT D_CWavDestFilter::Copy(D_IMediaSample *pSource, D_IMediaSample *pDest) const
{
    CheckPointer(pSource,E_POINTER);
    CheckPointer(pDest,E_POINTER);

    // Copy the sample data

    BYTE *pSourceBuffer, *pDestBuffer;
    long lSourceSize = pSource->GetActualDataLength();

#ifdef DEBUG    
    long lDestSize = pDest->GetSize();
    ASSERT(lDestSize >= lSourceSize);
#endif

    pSource->GetPointer(&pSourceBuffer);
    pDest->GetPointer(&pDestBuffer);

    _MEMCPY((PVOID) pDestBuffer,(PVOID) pSourceBuffer,( size_t )lSourceSize);

    // Copy the sample times

    D_REFERENCE_TIME TimeStart, TimeEnd;
    if(NOERROR == pSource->GetTime(&TimeStart, &TimeEnd))
    {
        pDest->SetTime(&TimeStart, &TimeEnd);
    }

    LONGLONG MediaStart, MediaEnd;
    if(pSource->GetMediaTime(&MediaStart,&MediaEnd) == NOERROR)
    {
        pDest->SetMediaTime(&MediaStart,&MediaEnd);
    }

    // Copy the media type
    D_AM_MEDIA_TYPE *pMediaType;
    pSource->GetMediaType(&pMediaType);
    pDest->SetMediaType(pMediaType);
    DeleteMediaType(pMediaType);

    // Copy the actual data length
    long lDataLength = pSource->GetActualDataLength();
    pDest->SetActualDataLength(lDataLength);

    return NOERROR;

} // Copy


//
// CheckInputType
//
HRESULT D_CWavDestFilter::CheckInputType(const D_CMediaType* mtIn)
{
    if(mtIn->formattype == FORMAT_WAVEFORMATEX)
    {
        return S_OK;
    }
    return S_FALSE;
}

//
// GetMediaType
//
HRESULT D_CWavDestFilter::GetMediaType(int iPosition, D_CMediaType *pMediaType)
{
    ASSERT(iPosition == 0 || iPosition == 1);

    if(iPosition == 0)
    {
        CheckPointer(pMediaType,E_POINTER);

        pMediaType->SetType(&MEDIATYPE_STREAM);
        pMediaType->SetSubtype(&D_MEDIASUBTYPE_WAVE);
        return S_OK;
    }

    return VFW_S_NO_MORE_ITEMS;
}

//
// DecideBufferSize
//
// Tell the output pin's allocator what size buffers we
// require. Can only do this when the input is connected
//
HRESULT D_CWavDestFilter::DecideBufferSize(D_IMemAllocator *pAlloc,
                                         D_ALLOCATOR_PROPERTIES *pProperties)
{
    HRESULT hr = NOERROR;

    // Is the input pin connected
    if(m_pInput->IsConnected() == FALSE)
    {
        return E_UNEXPECTED;
    }

    CheckPointer(pAlloc,E_POINTER);
    CheckPointer(pProperties,E_POINTER);

    pProperties->cBuffers = 1;
    pProperties->cbAlign  = 1;

    // Get input pin's allocator size and use that
    D_ALLOCATOR_PROPERTIES InProps;
    D_IMemAllocator * pInAlloc = NULL;

    hr = m_pInput->GetAllocator(&pInAlloc);
    if(SUCCEEDED(hr))
    {
        hr = pInAlloc->GetProperties(&InProps);
        if(SUCCEEDED(hr))
        {
            pProperties->cbBuffer = InProps.cbBuffer;
        }
        pInAlloc->Release();
    }

    if(FAILED(hr))
        return hr;

    ASSERT(pProperties->cbBuffer);

    // Ask the allocator to reserve us some sample memory, NOTE the function
    // can succeed (that is return NOERROR) but still not have allocated the
    // memory that we requested, so we must check we got whatever we wanted

    D_ALLOCATOR_PROPERTIES Actual;
    hr = pAlloc->SetProperties(pProperties,&Actual);
    if(FAILED(hr))
    {
        return hr;
    }

    ASSERT(Actual.cBuffers == 1);

    if(pProperties->cBuffers > Actual.cBuffers ||
        pProperties->cbBuffer > Actual.cbBuffer)
    {
        return E_FAIL;
    }

    return NOERROR;

} // DecideBufferSize


//
// StartStreaming
//
// Compute the header size to allow space for us to write it at the end.
//
// 00000000    RIFF (00568BFE) 'WAVE'
// 0000000C        fmt  (00000010)
// 00000024        data (00568700)
// 0056872C
//
HRESULT D_CWavDestFilter::StartStreaming()
{
    // leave space for the header
    m_cbHeader = sizeof(D_RIFFLIST) + 
                 sizeof(D_RIFFCHUNK) + 
                 m_pInput->CurrentMediaType().FormatLength() + 
                 sizeof(D_RIFFCHUNK);

    m_cbWavData = 0;
    return S_OK;
}


//
// StopStreaming
//
// Write out the header
//
HRESULT D_CWavDestFilter::StopStreaming()
{
    IStream *pStream;
    if(m_pOutput->IsConnected() == FALSE)
        return E_FAIL;

    D_IPin * pDwnstrmInputPin = m_pOutput->GetConnected();

    if(!pDwnstrmInputPin)
        return E_FAIL;

    HRESULT hr = ((D_IMemInputPin *) pDwnstrmInputPin)->QueryInterface(IID_ISTREAM, 
                                                                    (void **)&pStream);
    if(SUCCEEDED(hr))
    {
        BYTE *pb = (BYTE *)_alloca(m_cbHeader);

        D_RIFFLIST  *pRiffWave = (D_RIFFLIST *)pb;
        D_RIFFCHUNK *pRiffFmt  = (D_RIFFCHUNK *)(pRiffWave + 1);
        D_RIFFCHUNK *pRiffData = (D_RIFFCHUNK *)(((BYTE *)(pRiffFmt + 1)) + 
                               m_pInput->CurrentMediaType().FormatLength());

        pRiffData->fcc = FCC("data");
        pRiffData->cb = m_cbWavData;

        pRiffFmt->fcc = FCC("fmt ");
        pRiffFmt->cb = m_pInput->CurrentMediaType().FormatLength();
        _MEMCPY(pRiffFmt + 1, m_pInput->CurrentMediaType().Format(), pRiffFmt->cb);

        pRiffWave->fcc = FCC("RIFF");
        pRiffWave->cb = m_cbWavData + m_cbHeader - sizeof(D_RIFFCHUNK);
        pRiffWave->fccListType = FCC("WAVE");

        LARGE_INTEGER li;
        _MEMSET(&li, 0, sizeof(li));

        hr = pStream->Seek(li, STREAM_SEEK_SET, 0);
        if(SUCCEEDED(hr))
        {
            hr = pStream->Write(pb, m_cbHeader, 0);
        }
        pStream->Release();
    }

    return hr;
}

//
// D_CWavDestOutputPin::D_CWavDestOutputPin 
//
D_CWavDestOutputPin::D_CWavDestOutputPin(D_CTransformFilter *pFilter, HRESULT * phr) :
        D_CTransformOutputPin(NAME("WavDest output pin"), pFilter, phr, L"Out")
{
    // Empty
}


//
// D_CWavDestOutputPin::EnumMediaTypes
//
STDMETHODIMP D_CWavDestOutputPin::EnumMediaTypes( D_IEnumMediaTypes **ppEnum )
{
    return D_CBaseOutputPin::EnumMediaTypes(ppEnum);
}

//
// D_CWavDestOutputPin::CheckMediaType
//
// Make sure it's our default type
//
HRESULT D_CWavDestOutputPin::CheckMediaType(const D_CMediaType* pmt)
{
    CheckPointer(pmt,E_POINTER);

    if(pmt->majortype == MEDIATYPE_STREAM && pmt->subtype == D_MEDIASUBTYPE_WAVE)
        return S_OK;
    else
        return S_FALSE;
}

















HRESULT D_CAsyncOutputPin::CompleteConnect( D_IPin *pReceivePin )
{
	if (m_bQueriedForAsyncReader) {
		return D_CBasePin::CompleteConnect(pReceivePin);
	} else {

#ifdef VFW_E_NO_TRANSPORT
		return VFW_E_NO_TRANSPORT;
#else
		return E_FAIL;
#endif
	}
}

HRESULT D_CAsyncOutputPin::BreakConnect()
{
	m_bQueriedForAsyncReader = FALSE;
	return D_CBasePin::BreakConnect();
}



D_CAsyncOutputPin::D_CAsyncOutputPin(
    HRESULT * phr,
    D_CAsyncReader *pReader,
    D_CAsyncIo *pIo,
    D_CCritSec * pLock)
  : D_CBasePin(
    NAME("Async output pin"),
    pReader,
    pLock,
    phr,
    L"Output",
    D_PINDIR_OUTPUT),
    m_pReader(pReader),
    m_pIo(pIo)
{
}

D_CAsyncOutputPin::~D_CAsyncOutputPin()
{
}


STDMETHODIMP D_CAsyncOutputPin::NonDelegatingQueryInterface(REFIID riid, void** ppv)
{
    CheckPointer(ppv,E_POINTER);

    if(riid == IID_IASYNCREADER)
    {
        m_bQueriedForAsyncReader = TRUE;
        return D_GetInterface((D_IAsyncReader*) this, ppv);
    }
    else
    {
        return D_CBasePin::NonDelegatingQueryInterface(riid, ppv);
    }
}


HRESULT D_CAsyncOutputPin::GetMediaType(int iPosition, D_CMediaType *pMediaType)
{
    if(iPosition < 0)
    {
        return E_INVALIDARG;
    }
    if(iPosition > 0)
    {
        return VFW_S_NO_MORE_ITEMS;
    }

    CheckPointer(pMediaType,E_POINTER); 
    CheckPointer(m_pReader,E_UNEXPECTED); 
    
    *pMediaType = *m_pReader->LoadType();

    return S_OK;
}


HRESULT D_CAsyncOutputPin::CheckMediaType(const D_CMediaType* pType)
{
    D_CAutoLock lck(m_pLock);

    /*  We treat MEDIASUBTYPE_NULL subtype as a wild card */
    if((m_pReader->LoadType()->majortype == pType->majortype) &&
       (m_pReader->LoadType()->subtype == D_MEDIASUBTYPE_NULL   ||
        m_pReader->LoadType()->subtype == pType->subtype))
    {
        return S_OK;
    }

    return S_FALSE;
}


HRESULT D_CAsyncOutputPin::InitAllocator( D_IMemAllocator **ppAlloc)
{
    CheckPointer(ppAlloc,E_POINTER);

    HRESULT hr = NOERROR;
    D_CMemAllocator *pMemObject = NULL;

    *ppAlloc = NULL;

    /* Create a default memory allocator */
    pMemObject = new D_CMemAllocator(NAME("Base memory allocator"), NULL, &hr);
    if(pMemObject == NULL)
    {
        return E_OUTOFMEMORY;
    }
    if(FAILED(hr))
    {
        delete pMemObject;
        return hr;
    }

    /* Get a reference counted IID_IMEMALLOCATOR interface */
    hr = pMemObject->QueryInterface(IID_IMEMALLOCATOR,(void **)ppAlloc);
    if(FAILED(hr))
    {
        delete pMemObject;
        return E_NOINTERFACE;
    }

    ASSERT(*ppAlloc != NULL);
    return NOERROR;
}


// we need to return an addrefed allocator, even if it is the preferred
// one, since he doesn't know whether it is the preferred one or not.
STDMETHODIMP 
D_CAsyncOutputPin::RequestAllocator(
    D_IMemAllocator* pPreferred,
    D_ALLOCATOR_PROPERTIES* pProps,
    D_IMemAllocator ** ppActual)
{
    CheckPointer(pPreferred,E_POINTER);
    CheckPointer(pProps,E_POINTER);
    CheckPointer(ppActual,E_POINTER);
    ASSERT(m_pIo);

    // we care about alignment but nothing else
    if(!pProps->cbAlign || !m_pIo->IsAligned(pProps->cbAlign))
    {
        m_pIo->Alignment(&pProps->cbAlign);
    }

    D_ALLOCATOR_PROPERTIES Actual;
    HRESULT hr;

    if(pPreferred)
    {
        hr = pPreferred->SetProperties(pProps, &Actual);

        if(SUCCEEDED(hr) && m_pIo->IsAligned(Actual.cbAlign))
        {
            pPreferred->AddRef();
            *ppActual = pPreferred;
            return S_OK;
        }
    }

    // create our own allocator
    D_IMemAllocator* pAlloc;
    hr = InitAllocator(&pAlloc);
    if(FAILED(hr))
    {
        return hr;
    }

    //...and see if we can make it suitable
    hr = pAlloc->SetProperties(pProps, &Actual);
    if(SUCCEEDED(hr) && m_pIo->IsAligned(Actual.cbAlign))
    {
        // we need to release our refcount on pAlloc, and addref
        // it to pass a refcount to the caller - this is a net nothing.
        *ppActual = pAlloc;
        return S_OK;
    }

    // failed to find a suitable allocator
    pAlloc->Release();

    // if we failed because of the IsAligned test, the error code will
    // not be failure
    if(SUCCEEDED(hr))
    {
        hr = VFW_E_BADALIGN;
    }
    return hr;
}


// queue an aligned read request. call WaitForNext to get
// completion.
STDMETHODIMP D_CAsyncOutputPin::Request(
    D_IMediaSample* pSample,
    DWORD dwUser)           // user context
{
    CheckPointer(pSample,E_POINTER);

    D_REFERENCE_TIME tStart, tStop;
    HRESULT hr = pSample->GetTime(&tStart, &tStop);
    if(FAILED(hr))
    {
        return hr;
    }

    LONGLONG llPos = tStart / UNITS;
    LONG lLength = (LONG) ((tStop - tStart) / UNITS);

    LONGLONG llTotal=0, llAvailable=0;

    hr = m_pIo->Length(&llTotal, &llAvailable);
    if(llPos + lLength > llTotal)
    {
        // the end needs to be aligned, but may have been aligned
        // on a coarser alignment.
        LONG lAlign;
        m_pIo->Alignment(&lAlign);

        llTotal = (llTotal + lAlign -1) & ~(lAlign-1);

        if(llPos + lLength > llTotal)
        {
            lLength = (LONG) (llTotal - llPos);

            // must be reducing this!
            ASSERT((llTotal * UNITS) <= tStop);
            tStop = llTotal * UNITS;
            pSample->SetTime(&tStart, &tStop);
        }
    }

    BYTE* pBuffer;
    hr = pSample->GetPointer(&pBuffer);
    if(FAILED(hr))
    {
        return hr;
    }

    return m_pIo->Request(llPos,
                          lLength,
                          TRUE,
                          pBuffer,
                          (LPVOID)pSample,
                          dwUser);
}


// sync-aligned request. just like a request/waitfornext pair.
STDMETHODIMP 
D_CAsyncOutputPin::SyncReadAligned(
                  D_IMediaSample* pSample)
{
    CheckPointer(pSample,E_POINTER);

    D_REFERENCE_TIME tStart, tStop;
    HRESULT hr = pSample->GetTime(&tStart, &tStop);
    if(FAILED(hr))
    {
        return hr;
    }

    LONGLONG llPos = tStart / UNITS;
    LONG lLength = (LONG) ((tStop - tStart) / UNITS);

    LONGLONG llTotal;
    LONGLONG llAvailable;

    hr = m_pIo->Length(&llTotal, &llAvailable);
    if(llPos + lLength > llTotal)
    {
        // the end needs to be aligned, but may have been aligned
        // on a coarser alignment.
        LONG lAlign;
        m_pIo->Alignment(&lAlign);

        llTotal = (llTotal + lAlign -1) & ~(lAlign-1);

        if(llPos + lLength > llTotal)
        {
            lLength = (LONG) (llTotal - llPos);

            // must be reducing this!
            ASSERT((llTotal * UNITS) <= tStop);
            tStop = llTotal * UNITS;
            pSample->SetTime(&tStart, &tStop);
        }
    }

    BYTE* pBuffer;
    hr = pSample->GetPointer(&pBuffer);
    if(FAILED(hr))
    {
        return hr;
    }

    LONG cbActual;
    hr = m_pIo->SyncReadAligned(llPos,
                                lLength,
                                pBuffer,
                                &cbActual,
                                pSample);

    pSample->SetActualDataLength(cbActual);
    return hr;
}


//
// collect the next ready sample
STDMETHODIMP
D_CAsyncOutputPin::WaitForNext(
    DWORD dwTimeout,
    D_IMediaSample** ppSample,  // completed sample
    DWORD * pdwUser)        // user context
{
    CheckPointer(ppSample,E_POINTER);

    LONG cbActual;
    D_IMediaSample* pSample=0;

    HRESULT hr = m_pIo->WaitForNext(dwTimeout,
                                    (LPVOID*) &pSample,
                                    pdwUser,
                                    &cbActual);

    if(SUCCEEDED(hr))
    {
        pSample->SetActualDataLength(cbActual);
    }

    *ppSample = pSample;
    return hr;
}


//
// synchronous read that need not be aligned.
STDMETHODIMP
D_CAsyncOutputPin::SyncRead(
    LONGLONG llPosition,    // absolute Io position
    LONG lLength,       // nr bytes required
    BYTE* pBuffer)      // write data here
{
    return m_pIo->SyncRead(llPosition, lLength, pBuffer);
	}


// return the length of the file, and the length currently
// available locally. We only support locally accessible files,
// so they are always the same
STDMETHODIMP
D_CAsyncOutputPin::Length(
    LONGLONG* pTotal,
    LONGLONG* pAvailable)
{
    return m_pIo->Length(pTotal, pAvailable);
}


STDMETHODIMP
D_CAsyncOutputPin::BeginFlush(void)
{
    return m_pIo->BeginFlush();
}


STDMETHODIMP
D_CAsyncOutputPin::EndFlush(void)
{
    return m_pIo->EndFlush();
}


STDMETHODIMP
D_CAsyncOutputPin::Connect(
    D_IPin * pReceivePin,
    const D_AM_MEDIA_TYPE *pmt   // optional media type
)
{
    return m_pReader->Connect(pReceivePin, pmt);
}































HRESULT
D_CAsyncRequest::Request(
    D_CAsyncIo *pIo,
    D_CAsyncStream *pStream,
    LONGLONG llPos,
    LONG lLength,
    BOOL bAligned,
    BYTE* pBuffer,
    LPVOID pContext,    // filter's context
    DWORD dwUser)   // downstream filter's context
{
    m_pIo = pIo;
    m_pStream = pStream;
    m_llPos = llPos;
    m_lLength = lLength;
    m_bAligned = bAligned;
    m_pBuffer = pBuffer;
    m_pContext = pContext;
    m_dwUser = dwUser;
    m_hr = VFW_E_TIMEOUT;   // not done yet

    return S_OK;
}


// issue the i/o if not overlapped, and block until i/o complete.
// returns error code of file i/o
//
//
HRESULT
D_CAsyncRequest::Complete()
{
    m_pStream->Lock();

    m_hr = m_pStream->SetPointer(m_llPos);
    if(S_OK == m_hr)
    {
        DWORD dwActual;

        m_hr = m_pStream->Read(m_pBuffer, ( DWORD )m_lLength, m_bAligned, &dwActual);
        if(m_hr == OLE_S_FIRST)
        {
            if(m_pContext)
            {
                D_IMediaSample *pSample = reinterpret_cast<D_IMediaSample *>(m_pContext);
                pSample->SetDiscontinuity(TRUE);
                m_hr = S_OK;
            }
        }

        if(FAILED(m_hr))
        {
        }
        else if(dwActual != (DWORD)m_lLength)
        {
            // tell caller size changed - probably because of EOF
            m_lLength = (LONG) dwActual;
            m_hr = S_FALSE;
        }
        else
        {
            m_hr = S_OK;
        }
    }

    m_pStream->Unlock();
    return m_hr;
}






D_CAsyncIo::D_CAsyncIo(D_CAsyncStream *pStream)
         : m_hThread(NULL),
           m_evWork(TRUE),
           m_evDone(TRUE),
           m_evStop(TRUE),
           m_listWork(NAME("Work list")),
           m_listDone(NAME("Done list")),
           m_bFlushing(FALSE),
           m_cItemsOut(0),
           m_bWaiting(FALSE),
           m_pStream(pStream)
{

}


D_CAsyncIo::~D_CAsyncIo()
{
    // move everything to the done list
    BeginFlush();

    // shutdown worker thread
    CloseThread();

    // empty the done list
    D_POSITION pos = m_listDone.GetHeadPosition();
    while(pos)
    {
        D_CAsyncRequest* pRequest = m_listDone.GetNext(pos);
        delete pRequest;
    }

    m_listDone.RemoveAll();
}


// ready for async activity - call this before calling Request.
//
// start the worker thread if we need to
//
// !!! use overlapped i/o if possible
HRESULT
D_CAsyncIo::AsyncActive(void)
{
    return StartThread();
}

// call this when no more async activity will happen before
// the next AsyncActive call
//
// stop the worker thread if active
HRESULT
D_CAsyncIo::AsyncInactive(void)
{
    return CloseThread();
}


// add a request to the queue.
HRESULT
D_CAsyncIo::Request(
                LONGLONG llPos,
                LONG lLength,
                BOOL bAligned,
                BYTE * pBuffer,
                LPVOID pContext,
                DWORD dwUser)
{
    if(bAligned)
    {
        if(!IsAligned(llPos) ||
            !IsAligned(lLength) ||
            !IsAligned((LONGLONG) pBuffer))
        {
            return VFW_E_BADALIGN;
        }
    }

    D_CAsyncRequest* pRequest = new D_CAsyncRequest;
    if (!pRequest)
        return E_OUTOFMEMORY;

    HRESULT hr = pRequest->Request(this,
                                   m_pStream,
                                   llPos,
                                   lLength,
                                   bAligned,
                                   pBuffer,
                                   pContext,
                                   dwUser);
    if(SUCCEEDED(hr))
    {
        // might fail if flushing
        hr = PutWorkItem(pRequest);
    }

    if(FAILED(hr))
    {
        delete pRequest;
    }

    return hr;
}


// wait for the next request to complete
HRESULT
D_CAsyncIo::WaitForNext(
    DWORD dwTimeout,
    LPVOID * ppContext,
    DWORD  * pdwUser,
    LONG   * pcbActual)
{
    CheckPointer(ppContext,E_POINTER);
    CheckPointer(pdwUser,E_POINTER);
    CheckPointer(pcbActual,E_POINTER);

    // some errors find a sample, others don't. Ensure that
    // *ppContext is NULL if no sample found
    *ppContext = NULL;

    // wait until the event is set, but since we are not
    // holding the critsec when waiting, we may need to re-wait
    for(;;)
    {
        if(!m_evDone.Wait(dwTimeout))
        {
            // timeout occurred
            return VFW_E_TIMEOUT;
        }

        // get next event from list
        D_CAsyncRequest* pRequest = GetDoneItem();
        if(pRequest)
        {
            // found a completed request

            // check if ok
            HRESULT hr = pRequest->GetHResult();
            if(hr == S_FALSE)
            {
                // this means the actual length was less than
                // requested - may be ok if he aligned the end of file
                if((pRequest->GetActualLength() +
                    pRequest->GetStart()) == Size())
                {
                    hr = S_OK;
                }
                else
                {
                    // it was an actual read error
                    hr = E_FAIL;
                }
            }

            // return actual bytes read
            *pcbActual = pRequest->GetActualLength();

            // return his context
            *ppContext = pRequest->GetContext();
            *pdwUser = pRequest->GetUser();

            delete pRequest;
            return hr;
        }
        else
        {
            //  Hold the critical section while checking the list state
            D_CAutoLock lck(&m_csLists);
            if(m_bFlushing && !m_bWaiting)
            {
                // can't block as we are between BeginFlush and EndFlush

                // but note that if m_bWaiting is set, then there are some
                // items not yet complete that we should block for.

                return VFW_E_WRONG_STATE;
            }
        }

        // done item was grabbed between completion and
        // us locking m_csLists.
    }
}


// perform a synchronous read request on this thread.
// Need to hold m_csFile while doing this (done in request object)
HRESULT
D_CAsyncIo::SyncReadAligned(
                        LONGLONG llPos,
                        LONG lLength,
                        BYTE * pBuffer,
                        LONG * pcbActual,
                        PVOID pvContext)
{
    CheckPointer(pcbActual,E_POINTER);

    if(!IsAligned(llPos) ||
        !IsAligned(lLength) ||
        !IsAligned((LONGLONG) pBuffer))
    {
        return VFW_E_BADALIGN;
    }

    D_CAsyncRequest request;

    HRESULT hr = request.Request(this,
                                m_pStream,
                                llPos,
                                lLength,
                                TRUE,
                                pBuffer,
                                pvContext,
                                0);
    if(FAILED(hr))
        return hr;

    hr = request.Complete();

    // return actual data length
    *pcbActual = request.GetActualLength();
    return hr;
}


HRESULT
D_CAsyncIo::Length(LONGLONG *pllTotal, LONGLONG *pllAvailable)
{
    CheckPointer(pllTotal,E_POINTER);

    *pllTotal = m_pStream->Size(pllAvailable);
    return S_OK;
}


// cancel all items on the worklist onto the done list
// and refuse further requests or further WaitForNext calls
// until the end flush
//
// WaitForNext must return with NULL only if there are no successful requests.
// So Flush does the following:
// 1. set m_bFlushing ensures no more requests succeed
// 2. move all items from work list to the done list.
// 3. If there are any outstanding requests, then we need to release the
//    critsec to allow them to complete. The m_bWaiting as well as ensuring
//    that we are signalled when they are all done is also used to indicate
//    to WaitForNext that it should continue to block.
// 4. Once all outstanding requests are complete, we force m_evDone set and
//    m_bFlushing set and m_bWaiting false. This ensures that WaitForNext will
//    not block when the done list is empty.
HRESULT
D_CAsyncIo::BeginFlush()
{
    // hold the lock while emptying the work list
    {
        D_CAutoLock lock(&m_csLists);

        // prevent further requests being queued.
        // Also WaitForNext will refuse to block if this is set
        // unless m_bWaiting is also set which it will be when we release
        // the critsec if there are any outstanding).
        m_bFlushing = TRUE;

        D_CAsyncRequest * preq;
        while((preq = GetWorkItem()) != 0)
        {
            preq->Cancel();
            PutDoneItem(preq);
        }

        // now wait for any outstanding requests to complete
        if(m_cItemsOut > 0)
        {
            // can be only one person waiting
            ASSERT(!m_bWaiting);

            // this tells the completion routine that we need to be
            // signalled via m_evAllDone when all outstanding items are
            // done. It also tells WaitForNext to continue blocking.
            m_bWaiting = TRUE;
        }
        else
        {
            // all done

            // force m_evDone set so that even if list is empty,
            // WaitForNext will not block
            // don't do this until we are sure that all
            // requests are on the done list.
            m_evDone.Set();
            return S_OK;
        }
    }

    ASSERT(m_bWaiting);

    // wait without holding critsec
    for(;;)
    {
        m_evAllDone.Wait();
        {
            // hold critsec to check
            D_CAutoLock lock(&m_csLists);

            if(m_cItemsOut == 0)
            {
                // now we are sure that all outstanding requests are on
                // the done list and no more will be accepted
                m_bWaiting = FALSE;

                // force m_evDone set so that even if list is empty,
                // WaitForNext will not block
                // don't do this until we are sure that all
                // requests are on the done list.
                m_evDone.Set();

                return S_OK;
            }
        }
    }
}


// end a flushing state
HRESULT
D_CAsyncIo::EndFlush()
{
    D_CAutoLock lock(&m_csLists);

    m_bFlushing = FALSE;

    ASSERT(!m_bWaiting);

    // m_evDone might have been set by BeginFlush - ensure it is
    // set IFF m_listDone is non-empty
    if(m_listDone.GetCount() > 0)
    {
        m_evDone.Set();
    }
    else
    {
        m_evDone.Reset();
    }

    return S_OK;
}


// start the thread
HRESULT
D_CAsyncIo::StartThread(void)
{
	SETUP_WIN_API

    if(m_hThread)
    {
        return S_OK;
    }

    // clear the stop event before starting
    m_evStop.Reset();

    DWORD dwThreadID;
    m_hThread = WinAPIData.Win32Func.CreateThreadFunc(NULL,
                            0,
                            InitialThreadProc,
                            this,
                            0,
                            &dwThreadID);
    if(!m_hThread)
    {
        DWORD dwErr = WinAPIData.Win32Func.GetLastErrorFunc();
        return HRESULT_FROM_WIN32(dwErr);
    }

    return S_OK;
}


// stop the thread and close the handle
HRESULT
D_CAsyncIo::CloseThread(void)
{
	SETUP_WIN_API

    // signal the thread-exit object
    m_evStop.Set();

    if(m_hThread)
    {
        WinAPIData.Win32Func.WaitForSingleObjectFunc(m_hThread, INFINITE);
        WinAPIData.Win32Func.CloseHandleFunc(m_hThread);
        m_hThread = NULL;
    }

    return S_OK;
}


// manage the list of requests. hold m_csLists and ensure
// that the (manual reset) event hevList is set when things on
// the list but reset when the list is empty.
// returns null if list empty
D_CAsyncRequest*
D_CAsyncIo::GetWorkItem()
{
    D_CAutoLock lck(&m_csLists);
    D_CAsyncRequest * preq  = m_listWork.RemoveHead();

    // force event set correctly
    if(m_listWork.GetCount() == 0)
    {
        m_evWork.Reset();
    }

    return preq;
}


// get an item from the done list
D_CAsyncRequest*
D_CAsyncIo::GetDoneItem()
{
    D_CAutoLock lock(&m_csLists);
    D_CAsyncRequest * preq  = m_listDone.RemoveHead();

    // force event set correctly if list now empty
    // or we're in the final stages of flushing
    // Note that during flushing the way it's supposed to work is that
    // everything is shoved on the Done list then the application is
    // supposed to pull until it gets nothing more
    //
    // Thus we should not set m_evDone unconditionally until everything
    // has moved to the done list which means we must wait until
    // cItemsOut is 0 (which is guaranteed by m_bWaiting being TRUE).

    if(m_listDone.GetCount() == 0 &&
        (!m_bFlushing || m_bWaiting))
    {
        m_evDone.Reset();
    }

    return preq;
}


// put an item on the work list - fail if bFlushing
HRESULT
D_CAsyncIo::PutWorkItem(D_CAsyncRequest* pRequest)
{
    D_CAutoLock lock(&m_csLists);
    HRESULT hr;

    if(m_bFlushing)
    {
        hr = VFW_E_WRONG_STATE;
    }
    else if(m_listWork.AddTail(pRequest))
    {
        // event should now be in a set state - force this
        m_evWork.Set();

        // start the thread now if not already started
        hr = StartThread();

    }
    else
    {
        hr = E_OUTOFMEMORY;
    }

    return(hr);
}


// put an item on the done list - ok to do this when
// flushing
HRESULT
D_CAsyncIo::PutDoneItem(D_CAsyncRequest* pRequest)
{
    ASSERT(CritCheckIn(&m_csLists));

    if(m_listDone.AddTail(pRequest))
    {
        // event should now be in a set state - force this
        m_evDone.Set();
        return S_OK;
    }
    else
    {
        return E_OUTOFMEMORY;
    }
}


// called on thread to process any active requests
void
D_CAsyncIo::ProcessRequests(void)
{
    // lock to get the item and increment the outstanding count
    D_CAsyncRequest * preq = NULL;

    for(;;)
    {
        {
            D_CAutoLock lock(&m_csLists);

            preq = GetWorkItem();
            if(preq == NULL)
            {
                // done
                return;
            }

            // one more item not on the done or work list
            m_cItemsOut++;

            // release critsec
        }

        preq->Complete();

        // regain critsec to replace on done list
        {
            D_CAutoLock l(&m_csLists);

            PutDoneItem(preq);

            if(--m_cItemsOut == 0)
            {
                if(m_bWaiting)
                    m_evAllDone.Set();
            }
        }
    }
}


// the thread proc - assumes that DWORD thread param is the
// this pointer
DWORD
D_CAsyncIo::ThreadProc(void)
{
	SETUP_WIN_API

    HANDLE ahev[] = {m_evStop, m_evWork};

    for(;;)
    {
        DWORD dw = WinAPIData.Win32Func.WaitForMultipleObjectsFunc(2,
                                          ahev,
                                          FALSE,
                                          INFINITE);
        if(dw == WAIT_OBJECT_0+1)
        {
            // requests need processing
            ProcessRequests();
        }
        else
        {
            // any error or stop event - we should exit
            return 0;
        }
    }
}


// perform a synchronous read request on this thread.
// may not be aligned - so we will have to buffer.
HRESULT
D_CAsyncIo::SyncRead(
                LONGLONG llPos,
                LONG lLength,
                BYTE * pBuffer)
{
    if(IsAligned(llPos) &&
        IsAligned(lLength) &&
        IsAligned((LONGLONG) pBuffer))
    {
        LONG cbUnused;
        return SyncReadAligned(llPos, lLength, pBuffer, &cbUnused, NULL);
    }

    // not aligned with requirements - use buffered file handle.
    //!!! might want to fix this to buffer the data ourselves?

    D_CAsyncRequest request;

    HRESULT hr = request.Request(this,
                                m_pStream,
                                llPos,
                                lLength,
                                FALSE,
                                pBuffer,
                                NULL,
                                0);

    if(FAILED(hr))
    {
        return hr;
    }

    return request.Complete();
}


//  Return the alignment
HRESULT
D_CAsyncIo::Alignment(LONG *pAlignment)
{
    CheckPointer(pAlignment,E_POINTER);

    *pAlignment = Alignment();
    return S_OK;
}










#pragma warning(disable:4355)

D_CAsyncReader::D_CAsyncReader(
    const TCHAR *pName,
    D_IUnknown * pUnk,
    D_CAsyncStream *pStream,
    HRESULT *phr)
  : D_CBaseFilter(
                pName,
                pUnk,
                &m_csFilter,
                CLSID_AsyncSample,
                NULL
                ),
    m_OutputPin(
                phr,
                this,
                &m_Io,
                &m_csFilter),
    m_Io(pStream)
{
}

D_CAsyncReader::~D_CAsyncReader()
{
}

int D_CAsyncReader::GetPinCount()
{
    return 1;
}

D_CBasePin * D_CAsyncReader::GetPin(int n)
{
    if((GetPinCount() > 0) && (n == 0))
    {
        return &m_OutputPin;
    }
    else
    {
        return NULL;
    }
}













void D_CBaseAllocator::D_CSampleList::Add( D_CMediaSample *pSample)
{
	//ASSERT(pSample != NULL);
	D_CBaseAllocator::NextSample(pSample) = m_List;
	m_List = pSample;
	m_nOnList++;
};
D_CMediaSample *D_CBaseAllocator::D_CSampleList::RemoveHead()
{
	D_CMediaSample *pSample = m_List;
	if (pSample != NULL) {
		m_List = D_CBaseAllocator::NextSample(m_List);
		m_nOnList--;
	}
	return pSample;
};


D_CBaseAllocator::D_CBaseAllocator(const TCHAR *pName,
                               D_IUnknown * pUnk,
                               HRESULT *phr,
                               BOOL bEvent,
                               BOOL fEnableReleaseCallback
                               ) :
    D_CUnknown(pName, pUnk),
    m_lAllocated(0),
    m_bChanged(FALSE),
    m_bCommitted(FALSE),
    m_bDecommitInProgress(FALSE),
    m_lSize(0),
    m_lCount(0),
    m_lAlignment(0),
    m_lPrefix(0),
    m_hSem(NULL),
    m_lWaiting(0),
    m_fEnableReleaseCallback(fEnableReleaseCallback),
    m_pNotify(NULL)
{
	SETUP_WIN_API

    if (bEvent) {
        m_hSem = WinAPIData.Win32Func.CreateSemaphoreAFunc(NULL, 0, 0x7FFFFFFF, NULL);
        if (m_hSem == NULL) {
            *phr = E_OUTOFMEMORY;
            return;
        }
    }
}

#ifdef UNICODE
D_CBaseAllocator::D_CBaseAllocator(const CHAR *pName,
                               D_IUnknown * pUnk,
                               HRESULT *phr,
                               BOOL bEvent,
                               BOOL fEnableReleaseCallback) :
    D_CUnknown(pName, pUnk),
    m_lAllocated(0),
    m_bChanged(FALSE),
    m_bCommitted(FALSE),
    m_bDecommitInProgress(FALSE),
    m_lSize(0),
    m_lCount(0),
    m_lAlignment(0),
    m_lPrefix(0),
    m_hSem(NULL),
    m_lWaiting(0),
    m_fEnableReleaseCallback(fEnableReleaseCallback),
    m_pNotify(NULL)
{
	SETUP_WIN_API

    if (bEvent) {
        m_hSem = WinAPIData.Win32Func.CreateSemaphoreAFunc(NULL, 0, 0x7FFFFFFF, NULL);
        if (m_hSem == NULL) {
            *phr = E_OUTOFMEMORY;
            return;
        }
    }
}
#endif

/* Destructor */

D_CBaseAllocator::~D_CBaseAllocator()
{
	SETUP_WIN_API

    // we can't call Decommit here since that would mean a call to a
    // pure virtual in destructor.
    // We must assume that the derived class has gone into decommit state in
    // its destructor.

    ASSERT(!m_bCommitted);
    if (m_hSem != NULL) {
        EXECUTE_ASSERT(WinAPIData.Win32Func.CloseHandleFunc(m_hSem));
    }
    if (m_pNotify) {
        m_pNotify->Release();
    }
}


/* Override this to publicise our interfaces */

STDMETHODIMP
D_CBaseAllocator::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    /* Do we know about this interface */

    if (riid == IID_IMEMALLOCATOR ||
        riid == IID_IMEMALLOCATORCALLBACKTEMP && m_fEnableReleaseCallback) {
        return D_GetInterface((D_IMemAllocatorCallbackTemp *) this, ppv);
    } else {
        return D_CUnknown::NonDelegatingQueryInterface(riid, ppv);
    }
}


/* This sets the size and count of the required samples. The memory isn't
   actually allocated until Commit() is called, if memory has already been
   allocated then assuming no samples are outstanding the user may call us
   to change the buffering, the memory will be released in Commit() */

STDMETHODIMP
D_CBaseAllocator::SetProperties(
                D_ALLOCATOR_PROPERTIES* pRequest,
                D_ALLOCATOR_PROPERTIES* pActual)
{
    CheckPointer(pRequest, E_POINTER);
    CheckPointer(pActual, E_POINTER);
    ValidateReadWritePtr(pActual, sizeof(D_ALLOCATOR_PROPERTIES));
    D_CAutoLock cObjectLock(this);

    _MEMSET(pActual, 0, sizeof(D_ALLOCATOR_PROPERTIES));

    ASSERT(pRequest->cbBuffer > 0);

    /*  Check the alignment requested */
    if (pRequest->cbAlign != 1) {
        DbgLog((LOG_ERROR, 2, TEXT("Alignment requested was 0x%x, not 1"),
               pRequest->cbAlign));
        return VFW_E_BADALIGN;
    }

    /* Can't do this if already committed, there is an argument that says we
       should not reject the SetProperties call if there are buffers still
       active. However this is called by the source filter, which is the same
       person who is holding the samples. Therefore it is not unreasonable
       for them to free all their samples before changing the requirements */

    if (m_bCommitted) {
        return VFW_E_ALREADY_COMMITTED;
    }

    /* Must be no outstanding buffers */

    if (m_lAllocated != m_lFree.GetCount()) {
        return VFW_E_BUFFERS_OUTSTANDING;
    }

    /* There isn't any real need to check the parameters as they
       will just be rejected when the user finally calls Commit */

    pActual->cbBuffer = m_lSize = pRequest->cbBuffer;
    pActual->cBuffers = m_lCount = pRequest->cBuffers;
    pActual->cbAlign = m_lAlignment = pRequest->cbAlign;
    pActual->cbPrefix = m_lPrefix = pRequest->cbPrefix;

    m_bChanged = TRUE;
    return NOERROR;
}

STDMETHODIMP
D_CBaseAllocator::GetProperties(
    D_ALLOCATOR_PROPERTIES * pActual)
{
    CheckPointer(pActual,E_POINTER);
    ValidateReadWritePtr(pActual,sizeof(D_ALLOCATOR_PROPERTIES));

    D_CAutoLock cObjectLock(this);
    pActual->cbBuffer = m_lSize;
    pActual->cBuffers = m_lCount;
    pActual->cbAlign = m_lAlignment;
    pActual->cbPrefix = m_lPrefix;
    return NOERROR;
}

// get container for a sample. Blocking, synchronous call to get the
// next free buffer (as represented by an D_IMediaSample interface).
// on return, the time etc properties will be invalid, but the buffer
// pointer and size will be correct.

HRESULT __stdcall D_CBaseAllocator::GetBuffer(D_IMediaSample **ppBuffer,
                                  D_REFERENCE_TIME *pStartTime,
                                  D_REFERENCE_TIME *pEndTime,
                                  DWORD dwFlags
                                  )
{
    UNREFERENCED_PARAMETER(pStartTime);
    UNREFERENCED_PARAMETER(pEndTime);
    UNREFERENCED_PARAMETER(dwFlags);
    D_CMediaSample *pSample;

	SETUP_WIN_API

    *ppBuffer = NULL;	
    for (;;)
    {
        {  // scope for lock
            D_CAutoLock cObjectLock(this);

            /* Check we are committed */
            if (!m_bCommitted) {
                return VFW_E_NOT_COMMITTED;
            }
            pSample = (D_CMediaSample *) m_lFree.RemoveHead();
            if (pSample == NULL) {
                SetWaiting();
            }
        }

        /* If we didn't get a sample then wait for the list to signal */

        if (pSample) {
            break;
        }
        if (dwFlags & AM_GBF_NOWAIT) {
            return VFW_E_TIMEOUT;
        }
        ASSERT(m_hSem != NULL);
        WinAPIData.Win32Func.WaitForSingleObjectFunc(m_hSem, INFINITE);
    }

    /* Addref the buffer up to one. On release
       back to zero instead of being deleted, it will requeue itself by
       calling the ReleaseBuffer member function. NOTE the owner of a
       media sample must always be derived from D_CBaseAllocator */


    ASSERT(pSample->m_cRef == 0);
    pSample->m_cRef = 1;
    *ppBuffer = pSample;


    return NOERROR;
}


/* Final release of a D_CMediaSample will call this */

STDMETHODIMP
D_CBaseAllocator::ReleaseBuffer(D_IMediaSample * pSample)
{
    CheckPointer(pSample,E_POINTER);
    ValidateReadPtr(pSample,sizeof(D_IMediaSample));



    BOOL bRelease = FALSE;
    {
        D_CAutoLock cal(this);

        /* Put back on the free list */

        m_lFree.Add((D_CMediaSample *)pSample);
        if (m_lWaiting != 0) {
            NotifySample();
        }

        // if there is a pending Decommit, then we need to complete it by
        // calling Free() when the last buffer is placed on the free list

        LONG l1 = m_lFree.GetCount();
        if (m_bDecommitInProgress && (l1 == m_lAllocated)) {
            Free();
            m_bDecommitInProgress = FALSE;
            bRelease = TRUE;
        }
    }

    if (m_pNotify) {

        ASSERT(m_fEnableReleaseCallback);

        //
        // Note that this is not synchronized with setting up a notification
        // method.
        //
        m_pNotify->NotifyRelease();
    }

    /* For each buffer there is one AddRef, made in GetBuffer and released
       here. This may cause the allocator and all samples to be deleted */

    if (bRelease) {
        Release();
    }
    return NOERROR;
}

STDMETHODIMP
D_CBaseAllocator::SetNotify(
    D_IMemAllocatorNotifyCallbackTemp* pNotify
    )
{
    ASSERT(m_fEnableReleaseCallback);
    D_CAutoLock lck(this);
    if (pNotify) {
        pNotify->AddRef();
    }
    if (m_pNotify) {
        m_pNotify->Release();
    }
    m_pNotify = pNotify;
    return S_OK;
}

STDMETHODIMP
D_CBaseAllocator::GetFreeCount(
    LONG* plBuffersFree
    )
{
    ASSERT(m_fEnableReleaseCallback);
    D_CAutoLock cObjectLock(this);
    *plBuffersFree = m_lCount - m_lAllocated + m_lFree.GetCount();
    return NOERROR;
}

void
D_CBaseAllocator::NotifySample()
{
	SETUP_WIN_API

    if (m_lWaiting != 0) {
        ASSERT(m_hSem != NULL);
        WinAPIData.Win32Func.ReleaseSemaphoreFunc(m_hSem, m_lWaiting, 0);
        m_lWaiting = 0;
    }
}

STDMETHODIMP
D_CBaseAllocator::Commit()
{
    /* Check we are not decommitted */
    D_CAutoLock cObjectLock(this);

    // cannot need to alloc or re-alloc if we are committed
    if (m_bCommitted) {
        return NOERROR;
    }

    /* Allow GetBuffer calls */

    m_bCommitted = TRUE;

    // is there a pending decommit ? if so, just cancel it
    if (m_bDecommitInProgress) {
        m_bDecommitInProgress = FALSE;

        // don't call Alloc at this point. He cannot allow SetProperties
        // between Decommit and the last free, so the buffer size cannot have
        // changed. And because some of the buffers are not free yet, he
        // cannot re-alloc anyway.
        return NOERROR;
    }

    DbgLog((LOG_MEMORY, 1, TEXT("Allocating: %ldx%ld"), m_lCount, m_lSize));

    // actually need to allocate the samples
    HRESULT hr = Alloc();
    if (FAILED(hr)) {
        m_bCommitted = FALSE;
        return hr;
    }
    AddRef();
    return NOERROR;
}


STDMETHODIMP
D_CBaseAllocator::Decommit()
{
    BOOL bRelease = FALSE;
    {
        /* Check we are not already decommitted */
        D_CAutoLock cObjectLock(this);
        if (m_bCommitted == FALSE) {
            if (m_bDecommitInProgress == FALSE) {
                return NOERROR;
            }
        }

        /* No more GetBuffer calls will succeed */
        m_bCommitted = FALSE;

        // are any buffers outstanding?
        if (m_lFree.GetCount() < m_lAllocated) {
            // please complete the decommit when last buffer is freed
            m_bDecommitInProgress = TRUE;
        } else {
            m_bDecommitInProgress = FALSE;

            // need to complete the decommit here as there are no
            // outstanding buffers

            Free();
            bRelease = TRUE;
        }

        // Tell anyone waiting that they can go now so we can
        // reject their call
        NotifySample();
    }

    if (bRelease) {
        Release();
    }
    return NOERROR;
}


/* Base definition of allocation which checks we are ok to go ahead and do
   the full allocation. We return S_FALSE if the requirements are the same */

HRESULT
D_CBaseAllocator::Alloc(void)
{
    /* Error if he hasn't set the size yet */
    if (m_lCount <= 0 || m_lSize <= 0 || m_lAlignment <= 0) {
        return VFW_E_SIZENOTSET;
    }

    /* should never get here while buffers outstanding */
    ASSERT(m_lFree.GetCount() == m_lAllocated);

    /* If the requirements haven't changed then don't reallocate */
    if (m_bChanged == FALSE) {
        return S_FALSE;
    }

    return NOERROR;
}

/*  Implement D_CBaseAllocator::D_CSampleList::Remove(pSample)
    Removes pSample from the list
*/
void
D_CBaseAllocator::D_CSampleList::Remove(D_CMediaSample * pSample)
{
    D_CMediaSample **pSearch;
    for (pSearch = &m_List;
         *pSearch != NULL;
         pSearch = &(D_CBaseAllocator::NextSample(*pSearch))) {
       if (*pSearch == pSample) {
           *pSearch = D_CBaseAllocator::NextSample(pSample);
           D_CBaseAllocator::NextSample(pSample) = NULL;
           m_nOnList--;
           return;
       }
    }
    DbgBreak("Couldn't find sample in list");
}



//=====================================================================
//=====================================================================
// Implements D_CMemAllocator
//=====================================================================
//=====================================================================

/* This goes in the factory template table to create new instances */
D_CUnknown *D_CMemAllocator::CreateInstance(D_IUnknown * pUnk, HRESULT *phr)
{
    D_CUnknown *pUnkRet = new D_CMemAllocator(NAME("D_CMemAllocator"), pUnk, phr);
    return pUnkRet;
}


D_CMemAllocator::D_CMemAllocator(
    const TCHAR *pName,
    D_IUnknown * pUnk,
    HRESULT *phr)
    : D_CBaseAllocator(pName, pUnk, phr, TRUE, TRUE),
    m_pBuffer(NULL)
{
}

#ifdef UNICODE

D_CMemAllocator::D_CMemAllocator(
    const CHAR *pName,
    D_IUnknown * pUnk,
    HRESULT *phr)
    : D_CBaseAllocator(pName, pUnk, phr, TRUE, TRUE),
    m_pBuffer(NULL)
{
}
#endif

/* This sets the size and count of the required samples. The memory isn't
   actually allocated until Commit() is called, if memory has already been
   allocated then assuming no samples are outstanding the user may call us
   to change the buffering, the memory will be released in Commit() */
STDMETHODIMP
D_CMemAllocator::SetProperties(
                D_ALLOCATOR_PROPERTIES* pRequest,
                D_ALLOCATOR_PROPERTIES* pActual)
{
	SETUP_WIN_API

    CheckPointer(pActual,E_POINTER);
    ValidateReadWritePtr(pActual,sizeof(D_ALLOCATOR_PROPERTIES));
    D_CAutoLock cObjectLock(this);

    _MEMSET(pActual, 0, sizeof(D_ALLOCATOR_PROPERTIES));

    ASSERT(pRequest->cbBuffer > 0);

    SYSTEM_INFO SysInfo;
    WinAPIData.Win32Func.GetSystemInfoFunc(&SysInfo);

    /*  Check the alignment request is a power of 2 */
    if ((-pRequest->cbAlign & pRequest->cbAlign) != pRequest->cbAlign) {
        DbgLog((LOG_ERROR, 1, TEXT("Alignment requested 0x%x not a power of 2!"),
               pRequest->cbAlign));
    }
    /*  Check the alignment requested */
    if (pRequest->cbAlign == 0 ||
    (SysInfo.dwAllocationGranularity & (pRequest->cbAlign - 1)) != 0) {
        DbgLog((LOG_ERROR, 1, TEXT("Invalid alignment 0x%x requested - granularity = 0x%x"),
               pRequest->cbAlign, SysInfo.dwAllocationGranularity));
        return VFW_E_BADALIGN;
    }

    /* Can't do this if already committed, there is an argument that says we
       should not reject the SetProperties call if there are buffers still
       active. However this is called by the source filter, which is the same
       person who is holding the samples. Therefore it is not unreasonable
       for them to free all their samples before changing the requirements */

    if (m_bCommitted == TRUE) {
        return VFW_E_ALREADY_COMMITTED;
    }

    /* Must be no outstanding buffers */

    if (m_lFree.GetCount() < m_lAllocated) {
        return VFW_E_BUFFERS_OUTSTANDING;
    }

    /* There isn't any real need to check the parameters as they
       will just be rejected when the user finally calls Commit */

    // round length up to alignment - remember that prefix is included in
    // the alignment
    LONG lSize = pRequest->cbBuffer + pRequest->cbPrefix;
    LONG lRemainder = lSize % pRequest->cbAlign;
    if (lRemainder != 0) {
        lSize = lSize - lRemainder + pRequest->cbAlign;
    }
    pActual->cbBuffer = m_lSize = (lSize - pRequest->cbPrefix);

    pActual->cBuffers = m_lCount = pRequest->cBuffers;
    pActual->cbAlign = m_lAlignment = pRequest->cbAlign;
    pActual->cbPrefix = m_lPrefix = pRequest->cbPrefix;

    m_bChanged = TRUE;
    return NOERROR;
}

// override this to allocate our resources when Commit is called.
//
// note that our resources may be already allocated when this is called,
// since we don't free them on Decommit. We will only be called when in
// decommit state with all buffers free.
//
// object locked by caller
HRESULT
D_CMemAllocator::Alloc(void)
{
    D_CAutoLock lck(this);

	SETUP_WIN_API

    /* Check he has called SetProperties */
    HRESULT hr = D_CBaseAllocator::Alloc();
    if (FAILED(hr)) {
        return hr;
    }

    /* If the requirements haven't changed then don't reallocate */
    if (hr == S_FALSE) {
        ASSERT(m_pBuffer);
        return NOERROR;
    }
    ASSERT(hr == S_OK); // we use this fact in the loop below

    /* Free the old resources */
    if (m_pBuffer) {
        ReallyFree();
    }

    /* Compute the aligned size */
    LONG lAlignedSize = m_lSize + m_lPrefix;
    if (m_lAlignment > 1) {
        LONG lRemainder = lAlignedSize % m_lAlignment;
        if (lRemainder != 0) {
            lAlignedSize += (m_lAlignment - lRemainder);
        }
    }

    /* Create the contiguous memory block for the samples
       making sure it's properly aligned (64K should be enough!)
    */
    ASSERT(lAlignedSize % m_lAlignment == 0);

    m_pBuffer = (PBYTE)WinAPIData.Win32Func.VirtualAllocFunc(NULL,
                    ( SIZE_T )( m_lCount * lAlignedSize ),
                    MEM_COMMIT,
                    PAGE_READWRITE);

    if (m_pBuffer == NULL) {
        return E_OUTOFMEMORY;
    }

    LPBYTE pNext = m_pBuffer;
    D_CMediaSample *pSample;

    ASSERT(m_lAllocated == 0);

    // Create the new samples - we have allocated m_lSize bytes for each sample
    // plus m_lPrefix bytes per sample as a prefix. We set the pointer to
    // the memory after the prefix - so that GetPointer() will return a pointer
    // to m_lSize bytes.
    for (; m_lAllocated < m_lCount; m_lAllocated++, pNext += lAlignedSize) {


        pSample = new D_CMediaSample(
                            NAME("Default memory media sample"),
                this,
                            &hr,
                            pNext + m_lPrefix,      // GetPointer() value
                            m_lSize);               // not including prefix

            ASSERT(SUCCEEDED(hr));
        if (pSample == NULL) {
            return E_OUTOFMEMORY;
        }

        // This CANNOT fail
        m_lFree.Add(pSample);
    }

    m_bChanged = FALSE;
    return NOERROR;
}


// override this to free up any resources we have allocated.
// called from the base class on Decommit when all buffers have been
// returned to the free list.
//
// caller has already locked the object.

// in our case, we keep the memory until we are deleted, so
// we do nothing here. The memory is deleted in the destructor by
// calling ReallyFree()
void
D_CMemAllocator::Free(void)
{
    return;
}


// called from the destructor (and from Alloc if changing size/count) to
// actually free up the memory
void
D_CMemAllocator::ReallyFree(void)
{
	SETUP_WIN_API

    /* Should never be deleting this unless all buffers are freed */

    ASSERT(m_lAllocated == m_lFree.GetCount());

    /* Free up all the CMediaSamples */

    D_CMediaSample *pSample;
    for (;;) {
        pSample = m_lFree.RemoveHead();
        if (pSample != NULL) {
            delete pSample;
        } else {
            break;
        }
    }

    m_lAllocated = 0;

    // free the block of buffer memory
    if (m_pBuffer) {
        EXECUTE_ASSERT(WinAPIData.Win32Func.VirtualFreeFunc(m_pBuffer, 0, MEM_RELEASE));
        m_pBuffer = NULL;
    }
}


/* Destructor frees our memory resources */

D_CMemAllocator::~D_CMemAllocator()
{
    Decommit();
    ReallyFree();
}














D_CMediaSample::D_CMediaSample( const TCHAR * /*pName*/,
               D_CBaseAllocator *pAllocator,
               HRESULT * /*phr*/,
               LPBYTE pBuffer,
               LONG length) :
    m_pBuffer(pBuffer),             // Initialise the buffer
    m_cbBuffer(length),             // And it's length
    m_lActual(length),              // By default, actual = length
    m_pMediaType(NULL),             // No media type change
    m_dwFlags(0),                   // Nothing set
    m_cRef(0),                      // 0 ref count
    m_dwTypeSpecificFlags(0),       // Type specific flags
    m_dwStreamId(AM_STREAM_MEDIA),  // Stream id
    m_pAllocator(pAllocator)        // Allocator
{

    // We must have an owner and it must also be derived from class
    // D_CBaseAllocator BUT we do not hold a reference count on it 

    ASSERT(pAllocator);
}
#ifdef UNICODE

D_CMediaSample::D_CMediaSample( const CHAR * /*pName*/,
               D_CBaseAllocator *pAllocator,
               HRESULT * /*phr*/,
               LPBYTE pBuffer,
               LONG length) :
    m_pBuffer(pBuffer),             // Initialise the buffer
    m_cbBuffer(length),             // And it's length
    m_lActual(length),              // By default, actual = length
    m_pMediaType(NULL),             // No media type change
    m_dwFlags(0),                   // Nothing set
    m_cRef(0),                      // 0 ref count
    m_dwTypeSpecificFlags(0),       // Type specific flags
    m_dwStreamId(AM_STREAM_MEDIA),  // Stream id
    m_pAllocator(pAllocator)        // Allocator
{

    /* We must have an owner and it must also be derived from class
       D_CBaseAllocator BUT we do not hold a reference count on it */

    ASSERT(pAllocator);
}
#endif

/* Destructor deletes the media type memory */

D_CMediaSample::~D_CMediaSample()
{

    if (m_pMediaType) {
    DeleteMediaType(m_pMediaType);
    }
}

/* Override this to publicise our interfaces */

STDMETHODIMP
D_CMediaSample::QueryInterface(REFIID riid, void **ppv)
{
    if (riid == IID_IMEDIASAMPLE ||
        riid == IID_IMEDIASAMPLE2 ||
        riid == IID_IUNKNOWN) {
        return D_GetInterface((D_IMediaSample *) this, ppv);
    } else {
        return E_NOINTERFACE;
    }
}

STDMETHODIMP_(ULONG)
D_CMediaSample::AddRef()
{
    return ( ULONG )InterlockedIncrement(&m_cRef);
}


// --  D_CMediaSample lifetimes --
//
// On final release of this sample buffer it is not deleted but
// returned to the freelist of the owning memory allocator
//
// The allocator may be waiting for the last buffer to be placed on the free
// list in order to decommit all the memory, so the ReleaseBuffer() call may
// result in this sample being deleted. We also need to hold a refcount on
// the allocator to stop that going away until we have finished with this.
// However, we cannot release the allocator before the ReleaseBuffer, as the
// release may cause us to be deleted. Similarly we can't do it afterwards.
//
// Thus we must leave it to the allocator to hold an addref on our behalf.
// When he issues us in GetBuffer, he addref's himself. When ReleaseBuffer
// is called, he releases himself, possibly causing us and him to be deleted.


STDMETHODIMP_(ULONG)
D_CMediaSample::Release()
{
    /* Decrement our own private reference count */
    LONG lRef;
    if (m_cRef == 1) {
        lRef = 0;
        m_cRef = 0;
    } else {
        lRef = InterlockedDecrement(&m_cRef);
    }
    ASSERT(lRef >= 0);

    DbgLog((LOG_MEMORY,3,TEXT("    Unknown %X ref-- = %d"),
        this, m_cRef));

    /* Did we release our final reference count */
    if (lRef == 0) {
        /* Free all resources */
        if (m_dwFlags & Sample_TypeChanged) {
            SetMediaType(NULL);
        }
        ASSERT(m_pMediaType == NULL);
        m_dwFlags = 0;
        m_dwTypeSpecificFlags = 0;
        m_dwStreamId = AM_STREAM_MEDIA;

        /* This may cause us to be deleted */
        // Our refcount is reliably 0 thus no-one will mess with us
        m_pAllocator->ReleaseBuffer(this);
    }
    return (ULONG)lRef;
}


// set the buffer pointer and length. Used by allocators that
// want variable sized pointers or pointers into already-read data.
// This is only available through a D_CMediaSample* not an D_IMediaSample*
// and so cannot be changed by clients.
HRESULT
D_CMediaSample::SetPointer(BYTE * ptr, LONG cBytes)
{
    m_pBuffer = ptr;            // new buffer area (could be null)
    m_cbBuffer = cBytes;        // length of buffer
    m_lActual = cBytes;         // length of data in buffer (assume full)

    return S_OK;
}


// get me a read/write pointer to this buffer's memory. I will actually
// want to use sizeUsed bytes.
STDMETHODIMP
D_CMediaSample::GetPointer(BYTE ** ppBuffer)
{
    ValidateReadWritePtr(ppBuffer,sizeof(BYTE *));

    // creator must have set pointer either during
    // constructor or by SetPointer
    ASSERT(m_pBuffer);

    *ppBuffer = m_pBuffer;
    return NOERROR;
}


// return the size in bytes of this buffer
STDMETHODIMP_(LONG)
D_CMediaSample::GetSize(void)
{
    return m_cbBuffer;
}


// get the stream time at which this sample should start and finish.
STDMETHODIMP
D_CMediaSample::GetTime(
    D_REFERENCE_TIME * pTimeStart,     // put time here
    D_REFERENCE_TIME * pTimeEnd
)
{
    ValidateReadWritePtr(pTimeStart,sizeof(D_REFERENCE_TIME));
    ValidateReadWritePtr(pTimeEnd,sizeof(D_REFERENCE_TIME));

    if (!(m_dwFlags & Sample_StopValid)) {
        if (!(m_dwFlags & Sample_TimeValid)) {
            return VFW_E_SAMPLE_TIME_NOT_SET;
        } else {
            *pTimeStart = m_Start;

            //  Make sure old stuff works
            *pTimeEnd = m_Start + 1;
            return VFW_S_NO_STOP_TIME;
        }
    }

    *pTimeStart = m_Start;
    *pTimeEnd = m_End;
    return NOERROR;
}


// Set the stream time at which this sample should start and finish.
// NULL pointers means the time is reset
STDMETHODIMP
D_CMediaSample::SetTime(
    D_REFERENCE_TIME * pTimeStart,
    D_REFERENCE_TIME * pTimeEnd
)
{
    if (pTimeStart == NULL) {
        ASSERT(pTimeEnd == NULL);
        m_dwFlags &= ~(Sample_TimeValid | Sample_StopValid);
    } else {
        if (pTimeEnd == NULL) {
            m_Start = *pTimeStart;
            m_dwFlags |= Sample_TimeValid;
            m_dwFlags &= ~Sample_StopValid;
        } else {
            ValidateReadPtr(pTimeStart,sizeof(D_REFERENCE_TIME));
            ValidateReadPtr(pTimeEnd,sizeof(D_REFERENCE_TIME));
            ASSERT(*pTimeEnd >= *pTimeStart);

            m_Start = *pTimeStart;
            m_End = *pTimeEnd;
            m_dwFlags |= Sample_TimeValid | Sample_StopValid;
        }
    }
    return NOERROR;
}


// get the media times (eg bytes) for this sample
STDMETHODIMP
D_CMediaSample::GetMediaTime(
    LONGLONG * pTimeStart,
    LONGLONG * pTimeEnd
)
{
    ValidateReadWritePtr(pTimeStart,sizeof(LONGLONG));
    ValidateReadWritePtr(pTimeEnd,sizeof(LONGLONG));

    if (!(m_dwFlags & Sample_MediaTimeValid)) {
        return VFW_E_MEDIA_TIME_NOT_SET;
    }

    *pTimeStart = m_MediaStart;
    *pTimeEnd = (m_MediaStart + m_MediaEnd);
    return NOERROR;
}


// Set the media times for this sample
STDMETHODIMP
D_CMediaSample::SetMediaTime(
    LONGLONG * pTimeStart,
    LONGLONG * pTimeEnd
)
{
    if (pTimeStart == NULL) {
        ASSERT(pTimeEnd == NULL);
        m_dwFlags &= ~Sample_MediaTimeValid;
    } else {
        ValidateReadPtr(pTimeStart,sizeof(LONGLONG));
        ValidateReadPtr(pTimeEnd,sizeof(LONGLONG));
        ASSERT(*pTimeEnd >= *pTimeStart);

        m_MediaStart = *pTimeStart;
        m_MediaEnd = (LONG)(*pTimeEnd - *pTimeStart);
        m_dwFlags |= Sample_MediaTimeValid;
    }
    return NOERROR;
}


STDMETHODIMP
D_CMediaSample::IsSyncPoint(void)
{
    if (m_dwFlags & Sample_SyncPoint) {
        return S_OK;
    } else {
        return S_FALSE;
    }
}


STDMETHODIMP
D_CMediaSample::SetSyncPoint(BOOL bIsSyncPoint)
{
    if (bIsSyncPoint) {
        m_dwFlags |= Sample_SyncPoint;
    } else {
        m_dwFlags &= ~Sample_SyncPoint;
    }
    return NOERROR;
}

// returns S_OK if there is a discontinuity in the data (this same is
// not a continuation of the previous stream of data
// - there has been a seek).
STDMETHODIMP
D_CMediaSample::IsDiscontinuity(void)
{
    if (m_dwFlags & Sample_Discontinuity) {
        return S_OK;
    } else {
        return S_FALSE;
    }
}

// set the discontinuity property - TRUE if this sample is not a
// continuation, but a new sample after a seek.
STDMETHODIMP
D_CMediaSample::SetDiscontinuity(BOOL bDiscont)
{
    // should be TRUE or FALSE
    if (bDiscont) {
        m_dwFlags |= Sample_Discontinuity;
    } else {
        m_dwFlags &= ~Sample_Discontinuity;
    }
    return S_OK;
}

STDMETHODIMP
D_CMediaSample::IsPreroll(void)
{
    if (m_dwFlags & Sample_Preroll) {
        return S_OK;
    } else {
        return S_FALSE;
    }
}


STDMETHODIMP
D_CMediaSample::SetPreroll(BOOL bIsPreroll)
{
    if (bIsPreroll) {
        m_dwFlags |= Sample_Preroll;
    } else {
        m_dwFlags &= ~Sample_Preroll;
    }
    return NOERROR;
}

STDMETHODIMP_(LONG)
D_CMediaSample::GetActualDataLength(void)
{
    return m_lActual;
}


STDMETHODIMP
D_CMediaSample::SetActualDataLength(LONG lActual)
{
    if (lActual > m_cbBuffer) {
        ASSERT(lActual <= GetSize());
        return VFW_E_BUFFER_OVERFLOW;
    }
    m_lActual = lActual;
    return NOERROR;
}


/* These allow for limited format changes in band */

STDMETHODIMP
D_CMediaSample::GetMediaType(D_AM_MEDIA_TYPE **ppMediaType)
{
    ValidateReadWritePtr(ppMediaType,sizeof(D_AM_MEDIA_TYPE *));
    ASSERT(ppMediaType);

    /* Do we have a new media type for them */

    if (!(m_dwFlags & Sample_TypeChanged)) {
        ASSERT(m_pMediaType == NULL);
        *ppMediaType = NULL;
        return S_FALSE;
    }

    ASSERT(m_pMediaType);

    /* Create a copy of our media type */

    *ppMediaType = D_CreateMediaType(m_pMediaType);
    if (*ppMediaType == NULL) {
        return E_OUTOFMEMORY;
    }
    return NOERROR;
}


/* Mark this sample as having a different format type */

STDMETHODIMP
D_CMediaSample::SetMediaType(D_AM_MEDIA_TYPE *pMediaType)
{
    /* Delete the current media type */

    if (m_pMediaType) {
        DeleteMediaType(m_pMediaType);
        m_pMediaType = NULL;
    }

    /* Mechanism for resetting the format type */

    if (pMediaType == NULL) {
        m_dwFlags &= ~Sample_TypeChanged;
        return NOERROR;
    }

    ASSERT(pMediaType);
    ValidateReadPtr(pMediaType,sizeof(D_AM_MEDIA_TYPE));

    /* Take a copy of the media type */

    m_pMediaType = D_CreateMediaType(pMediaType);
    if (m_pMediaType == NULL) {
        m_dwFlags &= ~Sample_TypeChanged;
        return E_OUTOFMEMORY;
    }

    m_dwFlags |= Sample_TypeChanged;
    return NOERROR;
}

// Set and get properties (D_IMediaSample2)
STDMETHODIMP D_CMediaSample::GetProperties(
    DWORD cbProperties,
    BYTE * pbProperties
)
{
    if (0 != cbProperties) {
        CheckPointer(pbProperties, E_POINTER);
        //  Return generic stuff up to the length
        D_AM_SAMPLE2_PROPERTIES Props;
        Props.cbData     = (DWORD) (DX_CSTRMBASE_MIN(cbProperties, ( DWORD )sizeof(Props)));
        Props.dwSampleFlags = m_dwFlags & ~Sample_MediaTimeValid;
        Props.dwTypeSpecificFlags = m_dwTypeSpecificFlags;
        Props.pbBuffer   = m_pBuffer;
        Props.cbBuffer   = m_cbBuffer;
        Props.lActual    = m_lActual;
        Props.tStart     = m_Start;
        Props.tStop      = m_End;
        Props.dwStreamId = m_dwStreamId;
        if (m_dwFlags & AM_SAMPLE_TYPECHANGED) {
            Props.pMediaType = m_pMediaType;
        } else {
            Props.pMediaType = NULL;
        }
        _MEMCPY(pbProperties, &Props, Props.cbData);
    }
    return S_OK;
}

#define CONTAINS_FIELD(type, field, offset) \
    ((FIELD_OFFSET(type, field) + sizeof(((type *)0)->field)) <= offset)

HRESULT __stdcall D_CMediaSample::SetProperties(
    DWORD cbProperties,
    const BYTE * pbProperties
)
{

    /*  Generic properties */
    D_AM_MEDIA_TYPE *pMediaType = NULL;

    if (CONTAINS_FIELD(D_AM_SAMPLE2_PROPERTIES, cbData, cbProperties)) {
        CheckPointer(pbProperties, E_POINTER);
        D_AM_SAMPLE2_PROPERTIES *pProps =
            (D_AM_SAMPLE2_PROPERTIES *)pbProperties;

        /*  Don't use more data than is actually there */
        if (pProps->cbData < cbProperties) {
            cbProperties = pProps->cbData;
        }
        /*  We only handle D_IMediaSample2 */
        if (cbProperties > sizeof(*pProps) ||
            pProps->cbData > sizeof(*pProps)) {
            return E_INVALIDARG;
        }
        /*  Do checks first, the assignments (for backout) */
        if (CONTAINS_FIELD(D_AM_SAMPLE2_PROPERTIES, dwSampleFlags, cbProperties)) {
            /*  Check the flags */
            if (pProps->dwSampleFlags &
                    (~Sample_ValidFlags | Sample_MediaTimeValid)) {
                return E_INVALIDARG;
            }
            /*  Check a flag isn't being set for a property
                not being provided
            */
            if ((pProps->dwSampleFlags & AM_SAMPLE_TIMEVALID) &&
                 !(m_dwFlags & AM_SAMPLE_TIMEVALID) &&
                 !CONTAINS_FIELD(D_AM_SAMPLE2_PROPERTIES, tStop, cbProperties)) {
                 return E_INVALIDARG;
            }
        }
        /*  NB - can't SET the pointer or size */
        if (CONTAINS_FIELD(D_AM_SAMPLE2_PROPERTIES, pbBuffer, cbProperties)) {

            /*  Check pbBuffer */
            if (pProps->pbBuffer != 0 && pProps->pbBuffer != m_pBuffer) {
                return E_INVALIDARG;
            }
        }
        if (CONTAINS_FIELD(D_AM_SAMPLE2_PROPERTIES, cbBuffer, cbProperties)) {

            /*  Check cbBuffer */
            if (pProps->cbBuffer != 0 && pProps->cbBuffer != m_cbBuffer) {
                return E_INVALIDARG;
            }
        }
        if (CONTAINS_FIELD(D_AM_SAMPLE2_PROPERTIES, cbBuffer, cbProperties) &&
            CONTAINS_FIELD(D_AM_SAMPLE2_PROPERTIES, lActual, cbProperties)) {

            /*  Check lActual */
            if (pProps->cbBuffer < pProps->lActual) {
                return E_INVALIDARG;
            }
        }

        if (CONTAINS_FIELD(D_AM_SAMPLE2_PROPERTIES, pMediaType, cbProperties)) {

            /*  Check pMediaType */
            if (pProps->dwSampleFlags & AM_SAMPLE_TYPECHANGED) {
                CheckPointer(pProps->pMediaType, E_POINTER);
                pMediaType = D_CreateMediaType(pProps->pMediaType);
                if (pMediaType == NULL) {
                    return E_OUTOFMEMORY;
                }
            }
        }

        /*  Now do the assignments */
        if (CONTAINS_FIELD(D_AM_SAMPLE2_PROPERTIES, dwStreamId, cbProperties)) {
            m_dwStreamId = pProps->dwStreamId;
        }
        if (CONTAINS_FIELD(D_AM_SAMPLE2_PROPERTIES, dwSampleFlags, cbProperties)) {
            /*  Set the flags */
            m_dwFlags = pProps->dwSampleFlags |
                                (m_dwFlags & Sample_MediaTimeValid);
            m_dwTypeSpecificFlags = pProps->dwTypeSpecificFlags;
        } else {
            if (CONTAINS_FIELD(D_AM_SAMPLE2_PROPERTIES, dwTypeSpecificFlags, cbProperties)) {
                m_dwTypeSpecificFlags = pProps->dwTypeSpecificFlags;
            }
        }

        if (CONTAINS_FIELD(D_AM_SAMPLE2_PROPERTIES, lActual, cbProperties)) {
            /*  Set lActual */
            m_lActual = pProps->lActual;
        }

        if (CONTAINS_FIELD(D_AM_SAMPLE2_PROPERTIES, tStop, cbProperties)) {

            /*  Set the times */
            m_End   = pProps->tStop;
        }
        if (CONTAINS_FIELD(D_AM_SAMPLE2_PROPERTIES, tStart, cbProperties)) {

            /*  Set the times */
            m_Start = pProps->tStart;
        }

        if (CONTAINS_FIELD(D_AM_SAMPLE2_PROPERTIES, pMediaType, cbProperties)) {
            /*  Set pMediaType */
            if (pProps->dwSampleFlags & AM_SAMPLE_TYPECHANGED) {
                if (m_pMediaType != NULL) {
                    DeleteMediaType(m_pMediaType);
                }
                m_pMediaType = pMediaType;
            }
        }

        /*  Fix up the type changed flag to correctly reflect the current state
            If, for instance the input contained no type change but the
            output does then if we don't do this we'd lose the
            output media type.
        */
        if (m_pMediaType) {
            m_dwFlags |= Sample_TypeChanged;
        } else {
            m_dwFlags &= ~Sample_TypeChanged;
        }
    }

    return S_OK;
}

#endif





















































#ifndef DX_NON_DSHOW_MP3

D_CTransInPlaceInputPin::D_CTransInPlaceInputPin
    ( TCHAR               *pObjectName
    , D_CTransInPlaceFilter *pFilter
    , HRESULT             *phr
    , LPCWSTR              pName
    )
    : D_CTransformInputPin(pObjectName,
                         pFilter,
                         phr,
                         pName)
    , m_bReadOnly(FALSE)
    , m_pTIPFilter(pFilter)
{
    DbgLog((LOG_TRACE, 2
           , TEXT("D_CTransInPlaceInputPin::D_CTransInPlaceInputPin")));

} // constructor

#ifdef UNICODE
D_CTransInPlaceInputPin::D_CTransInPlaceInputPin
    ( char               *pObjectName
    , D_CTransInPlaceFilter *pFilter
    , HRESULT             *phr
    , LPCWSTR              pName
    )
    : D_CTransformInputPin(pObjectName,
                         pFilter,
                         phr,
                         pName)
    , m_bReadOnly(FALSE)
    , m_pTIPFilter(pFilter)
{
    DbgLog((LOG_TRACE, 2
           , TEXT("D_CTransInPlaceInputPin::D_CTransInPlaceInputPin")));

} // constructor
#endif

// =================================================================
// Implements IMemInputPin interface
// =================================================================


// If the downstream filter has one then offer that (even if our own output
// pin is not using it yet.  If the upstream filter chooses it then we will
// tell our output pin to ReceiveAllocator).
// Else if our output pin is using an allocator then offer that.
//     ( This could mean offering the upstream filter his own allocator,
//       it could mean offerring our own
//     ) or it could mean offering the one from downstream
// Else fail to offer any allocator at all.

STDMETHODIMP D_CTransInPlaceInputPin::GetAllocator(D_IMemAllocator ** ppAllocator)
{
    CheckPointer(ppAllocator,E_POINTER);
    ValidateReadWritePtr(ppAllocator,sizeof(D_IMemAllocator *));
    D_CAutoLock cObjectLock(m_pLock);

    HRESULT hr;

    if ( m_pTIPFilter->m_pOutput->IsConnected() ) {
        //  Store the allocator we got
        hr = m_pTIPFilter->OutputPin()->ConnectedIMemInputPin()
                                        ->GetAllocator( ppAllocator );
        if (SUCCEEDED(hr)) {
            m_pTIPFilter->OutputPin()->SetAllocator( *ppAllocator );
        }
    }
    else {
        //  Help upstream filter (eg TIP filter which is having to do a copy)
        //  by providing a temp allocator here - we'll never use
        //  this allocator because when our output is connected we'll
        //  reconnect this pin
        hr = D_CTransformInputPin::GetAllocator( ppAllocator );
    }
    return hr;

} // GetAllocator



/* Get told which allocator the upstream output pin is actually going to use */


STDMETHODIMP
D_CTransInPlaceInputPin::NotifyAllocator(
    D_IMemAllocator * pAllocator,
    BOOL bReadOnly)
{
    HRESULT hr = S_OK;
    CheckPointer(pAllocator,E_POINTER);
    ValidateReadPtr(pAllocator,sizeof(D_IMemAllocator));

    D_CAutoLock cObjectLock(m_pLock);

    m_bReadOnly = bReadOnly;
    //  If we modify data then don't accept the allocator if it's
    //  the same as the output pin's allocator

    //  If our output is not connected just accept the allocator
    //  We're never going to use this allocator because when our
    //  output pin is connected we'll reconnect this pin
    if (!m_pTIPFilter->OutputPin()->IsConnected()) {
        return D_CTransformInputPin::NotifyAllocator(pAllocator, bReadOnly);
    }

    //  If the allocator is read-only and we're modifying data
    //  and the allocator is the same as the output pin's
    //  then reject
    if (bReadOnly && m_pTIPFilter->m_bModifiesData) {
        D_IMemAllocator *pOutputAllocator =
            m_pTIPFilter->OutputPin()->PeekAllocator();

        //  Make sure we have an output allocator
        if (pOutputAllocator == NULL) {
            hr = m_pTIPFilter->OutputPin()->ConnectedIMemInputPin()->
                                      GetAllocator(&pOutputAllocator);
            if(FAILED(hr)) {
                hr = CreateMemoryAllocator(&pOutputAllocator);
            }
            if (SUCCEEDED(hr)) {
                m_pTIPFilter->OutputPin()->SetAllocator(pOutputAllocator);
                pOutputAllocator->Release();
            }
        }
        if (pAllocator == pOutputAllocator) {
            hr = E_FAIL;
        } else if(SUCCEEDED(hr)) {
            //  Must copy so set the allocator properties on the output
			D_ALLOCATOR_PROPERTIES Props, Actual = { 0 } ;
            hr = pAllocator->GetProperties(&Props);
            if (SUCCEEDED(hr)) {
                hr = pOutputAllocator->SetProperties(&Props, &Actual);
            }
            if (SUCCEEDED(hr)) {
                if (  (Props.cBuffers > Actual.cBuffers)
                   || (Props.cbBuffer > Actual.cbBuffer)
                   || (Props.cbAlign  > Actual.cbAlign)
                   ) {
                    hr =  E_FAIL;
                }
            }

            //  Set the allocator on the output pin
            if (SUCCEEDED(hr)) {
                hr = m_pTIPFilter->OutputPin()->ConnectedIMemInputPin()
                                       ->NotifyAllocator( pOutputAllocator, FALSE );
            }
        }
    } else {
        hr = m_pTIPFilter->OutputPin()->ConnectedIMemInputPin()
                                   ->NotifyAllocator( pAllocator, bReadOnly );
        if (SUCCEEDED(hr)) {
            m_pTIPFilter->OutputPin()->SetAllocator( pAllocator );
        }
    }

    if (SUCCEEDED(hr)) {

        // It's possible that the old and the new are the same thing.
        // AddRef before release ensures that we don't unload it.
        pAllocator->AddRef();

        if( m_pAllocator != NULL )
            m_pAllocator->Release();

        m_pAllocator = pAllocator;    // We have an allocator for the input pin
    }

    return hr;

} // NotifyAllocator


// EnumMediaTypes
// - pass through to our downstream filter
STDMETHODIMP D_CTransInPlaceInputPin::EnumMediaTypes( D_IEnumMediaTypes **ppEnum )
{
    // Can only pass through if connected
    if( !m_pTIPFilter->m_pOutput->IsConnected() )
        return VFW_E_NOT_CONNECTED;

    return m_pTIPFilter->m_pOutput->GetConnected()->EnumMediaTypes( ppEnum );

} // EnumMediaTypes


// CheckMediaType
// - agree to anything if not connected,
// otherwise pass through to the downstream filter.
// This assumes that the filter does not change the media type.

HRESULT D_CTransInPlaceInputPin::CheckMediaType(const D_CMediaType *pmt )
{
    HRESULT hr = m_pTIPFilter->CheckInputType(pmt);
    if (hr!=S_OK) return hr;

    if( m_pTIPFilter->m_pOutput->IsConnected() )
        return m_pTIPFilter->m_pOutput->GetConnected()->QueryAccept( pmt );
    else
        return S_OK;

} // CheckMediaType


// If upstream asks us what our requirements are, we will try to ask downstream
// if that doesn't work, we'll just take the defaults.
STDMETHODIMP
D_CTransInPlaceInputPin::GetAllocatorRequirements(D_ALLOCATOR_PROPERTIES *pProps)
{

    if( m_pTIPFilter->m_pOutput->IsConnected() )
        return m_pTIPFilter->OutputPin()
               ->ConnectedIMemInputPin()->GetAllocatorRequirements( pProps );
    else
        return E_NOTIMPL;

} // GetAllocatorRequirements


// D_CTransInPlaceInputPin::CompleteConnect() calls D_CBaseInputPin::CompleteConnect()
// and then calls D_CTransInPlaceFilter::CompleteConnect().  It does this because 
// D_CTransInPlaceFilter::CompleteConnect() can reconnect a pin and we do not
// want to reconnect a pin if D_CBaseInputPin::CompleteConnect() fails.
HRESULT
D_CTransInPlaceInputPin::CompleteConnect(D_IPin *pReceivePin)
{
    HRESULT hr = D_CBaseInputPin::CompleteConnect(pReceivePin);
    if (FAILED(hr)) {
        return hr;
    }

    return m_pTransformFilter->CompleteConnect(D_PINDIR_INPUT,pReceivePin);
} // CompleteConnect


D_CTransInPlaceOutputPin::D_CTransInPlaceOutputPin(
    TCHAR *pObjectName,
    D_CTransInPlaceFilter *pFilter,
    HRESULT * phr,
    LPCWSTR pPinName)
    : D_CTransformOutputPin( pObjectName
                         , pFilter
                         , phr
                         , pPinName),
      m_pTIPFilter(pFilter)
{
    DbgLog(( LOG_TRACE, 2
           , TEXT("D_CTransInPlaceOutputPin::D_CTransInPlaceOutputPin")));

} // constructor

#ifdef UNICODE
D_CTransInPlaceOutputPin::D_CTransInPlaceOutputPin(
    char *pObjectName,
    D_CTransInPlaceFilter *pFilter,
    HRESULT * phr,
    LPCWSTR pPinName)
    : D_CTransformOutputPin( pObjectName
                         , pFilter
                         , phr
                         , pPinName),
      m_pTIPFilter(pFilter)
{
    DbgLog(( LOG_TRACE, 2
           , TEXT("D_CTransInPlaceOutputPin::D_CTransInPlaceOutputPin")));

} // constructor
#endif

// EnumMediaTypes
// - pass through to our upstream filter
STDMETHODIMP D_CTransInPlaceOutputPin::EnumMediaTypes( D_IEnumMediaTypes **ppEnum )
{
    // Can only pass through if connected.
    if( ! m_pTIPFilter->m_pInput->IsConnected() )
        return VFW_E_NOT_CONNECTED;

    return m_pTIPFilter->m_pInput->GetConnected()->EnumMediaTypes( ppEnum );

} // EnumMediaTypes



// CheckMediaType
// - agree to anything if not connected,
// otherwise pass through to the upstream filter.

HRESULT D_CTransInPlaceOutputPin::CheckMediaType(const D_CMediaType *pmt )
{
    // Don't accept any output pin type changes if we're copying
    // between allocators - it's too late to change the input
    // allocator size.
    if (m_pTIPFilter->UsingDifferentAllocators() && !m_pFilter->IsStopped()) {
        if (*pmt == m_mt) {
            return S_OK;
        } else {
            return VFW_E_TYPE_NOT_ACCEPTED;
        }
    }

    // Assumes the type does not change.  That's why we're calling
    // CheckINPUTType here on the OUTPUT pin.
    HRESULT hr = m_pTIPFilter->CheckInputType(pmt);
    if (hr!=S_OK) return hr;

    if( m_pTIPFilter->m_pInput->IsConnected() )
        return m_pTIPFilter->m_pInput->GetConnected()->QueryAccept( pmt );
    else
        return S_OK;

} // CheckMediaType


/* Save the allocator pointer in the output pin
*/
void
D_CTransInPlaceOutputPin::SetAllocator(D_IMemAllocator * pAllocator)
{
    pAllocator->AddRef();
    if (m_pAllocator) {
        m_pAllocator->Release();
    }
    m_pAllocator = pAllocator;
} // SetAllocator


// D_CTransInPlaceOutputPin::CompleteConnect() calls D_CBaseOutputPin::CompleteConnect()
// and then calls D_CTransInPlaceFilter::CompleteConnect().  It does this because 
// D_CTransInPlaceFilter::CompleteConnect() can reconnect a pin and we do not want to 
// reconnect a pin if D_CBaseOutputPin::CompleteConnect() fails.  
// D_CBaseOutputPin::CompleteConnect() often fails when our output pin is being connected 
// to the Video Mixing Renderer.
HRESULT
D_CTransInPlaceOutputPin::CompleteConnect(D_IPin *pReceivePin)
{
    HRESULT hr = D_CBaseOutputPin::CompleteConnect(pReceivePin);
    if (FAILED(hr)) {
        return hr;
    }

    return m_pTransformFilter->CompleteConnect(D_PINDIR_OUTPUT,pReceivePin);
} // CompleteConnect



D_CTransInPlaceFilter::D_CTransInPlaceFilter
   ( TCHAR     *pName,
     D_IUnknown *  pUnk,
     REFCLSID   clsid,
     HRESULT   * /* phr */,
     bool       bModifiesData
   )
   : D_CTransformFilter(pName, pUnk, clsid),
     m_bModifiesData(bModifiesData)
{
#ifdef PERF
    RegisterPerfId();
#endif //  PERF

} // constructor

#ifdef UNICODE

D_CTransInPlaceFilter::D_CTransInPlaceFilter
   ( CHAR     *pName,
     D_IUnknown *  pUnk,
     REFCLSID   clsid,
     HRESULT   * /*phr*/,
     bool       bModifiesData
   )
   : D_CTransformFilter(pName, pUnk, clsid),
     m_bModifiesData(bModifiesData)
{
#ifdef PERF
    RegisterPerfId();
#endif //  PERF

} // constructor
#endif


D_CTransInPlaceFilter::~D_CTransInPlaceFilter()
{
}

// return a non-addrefed D_CBasePin * for the user to addref if he holds onto it
// for longer than his pointer to us. We create the pins dynamically when they
// are asked for rather than in the constructor. This is because we want to
// give the derived class an oppportunity to return different pin objects

// As soon as any pin is needed we create both (this is different from the
// usual transform filter) because enumerators, allocators etc are passed
// through from one pin to another and it becomes very painful if the other
// pin isn't there.  If we fail to create either pin we ensure we fail both.

D_CBasePin *
D_CTransInPlaceFilter::GetPin(int n)
{
    HRESULT hr = S_OK;

    // Create an input pin if not already done

    if (m_pInput == NULL) {

        m_pInput = new D_CTransInPlaceInputPin( NAME("TransInPlace input pin")
                                            , this        // Owner filter
                                            , &hr         // Result code
                                            , L"Input"    // Pin name
                                            );

        // Constructor for D_CTransInPlaceInputPin can't fail
        ASSERT(SUCCEEDED(hr));
    }

    // Create an output pin if not already done

    if (m_pInput!=NULL && m_pOutput == NULL) {

        m_pOutput = new D_CTransInPlaceOutputPin( NAME("TransInPlace output pin")
                                              , this       // Owner filter
                                              , &hr        // Result code
                                              , L"Output"  // Pin name
                                              );

        // a failed return code should delete the object

        ASSERT(SUCCEEDED(hr));
        if (m_pOutput == NULL) {
            delete m_pInput;
            m_pInput = NULL;
        }
    }

    // Return the appropriate pin

    ASSERT (n>=0 && n<=1);
    if (n == 0) {
        return m_pInput;
    } else if (n==1) {
        return m_pOutput;
    } else {
        return NULL;
    }

} // GetPin



// dir is the direction of our pin.
// pReceivePin is the pin we are connecting to.
HRESULT D_CTransInPlaceFilter::CompleteConnect(D_PIN_DIRECTION dir,D_IPin *pReceivePin)
{
    UNREFERENCED_PARAMETER(pReceivePin);
    ASSERT(m_pInput);
    ASSERT(m_pOutput);

    // if we are not part of a graph, then don't indirect the pointer
    // this probably prevents use of the filter without a filtergraph
    if (!m_pGraph) {
        return VFW_E_NOT_IN_GRAPH;
    }

    // Always reconnect the input to account for buffering changes
    //
    // Because we don't get to suggest a type on ReceiveConnection
    // we need another way of making sure the right type gets used.
    //
    // One way would be to have our EnumMediaTypes return our output
    // connection type first but more deterministic and simple is to
    // call ReconnectEx passing the type we want to reconnect with
    // via the base class ReconeectPin method.

    if (dir == D_PINDIR_OUTPUT) {
        if( m_pInput->IsConnected() ) {
            return ReconnectPin( m_pInput, &m_pOutput->CurrentMediaType() );
        }
        return NOERROR;
    }

    ASSERT(dir == D_PINDIR_INPUT);

    // Reconnect output if necessary

    if( m_pOutput->IsConnected() ) {

        if (  m_pInput->CurrentMediaType()
           != m_pOutput->CurrentMediaType()
           ) {
            return ReconnectPin( m_pOutput, &m_pInput->CurrentMediaType() );
        }
    }
    return NOERROR;

} // ComnpleteConnect


//
// DecideBufferSize
//
// Tell the output pin's allocator what size buffers we require.
// *pAlloc will be the allocator our output pin is using.
//

HRESULT D_CTransInPlaceFilter::DecideBufferSize
            ( D_IMemAllocator *pAlloc
            , D_ALLOCATOR_PROPERTIES *pProperties
            )
{
    D_ALLOCATOR_PROPERTIES Request, Actual;
    HRESULT hr;

    // If we are connected upstream, get his views
    if (m_pInput->IsConnected()) {
        // Get the input pin allocator, and get its size and count.
        // we don't care about his alignment and prefix.

        hr = InputPin()->PeekAllocator()->GetProperties(&Request);
        if (FAILED(hr)) {
            // Input connected but with a secretive allocator - enough!
            return hr;
        }
    } else {
        // We're reduced to blind guessing.  Let's guess one byte and if
        // this isn't enough then when the other pin does get connected
        // we can revise it.
        _MEMSET(&Request, 0,sizeof(Request));
        Request.cBuffers = 1;
        Request.cbBuffer = 1;
    }


    DbgLog((LOG_MEMORY,1,TEXT("Setting Allocator Requirements")));
    DbgLog((LOG_MEMORY,1,TEXT("Count %d, Size %d"),
           Request.cBuffers, Request.cbBuffer));

    // Pass the allocator requirements to our output side
    // but do a little sanity checking first or we'll just hit
    // asserts in the allocator.

    pProperties->cBuffers = Request.cBuffers;
    pProperties->cbBuffer = Request.cbBuffer;
    pProperties->cbAlign = Request.cbAlign;
    if (pProperties->cBuffers<=0) {pProperties->cBuffers = 1; }
    if (pProperties->cbBuffer<=0) {pProperties->cbBuffer = 1; }
    hr = pAlloc->SetProperties(pProperties, &Actual);

    if (FAILED(hr)) {
        return hr;
    }

    DbgLog((LOG_MEMORY,1,TEXT("Obtained Allocator Requirements")));
    DbgLog((LOG_MEMORY,1,TEXT("Count %d, Size %d, Alignment %d"),
           Actual.cBuffers, Actual.cbBuffer, Actual.cbAlign));

    // Make sure we got the right alignment and at least the minimum required

    if (  (Request.cBuffers > Actual.cBuffers)
       || (Request.cbBuffer > Actual.cbBuffer)
       || (Request.cbAlign  > Actual.cbAlign)
       ) {
        return E_FAIL;
    }
    return NOERROR;

} // DecideBufferSize

//
// Copy
//
// return a pointer to an identical copy of pSample
D_IMediaSample * D_CTransInPlaceFilter::Copy(D_IMediaSample *pSource)
{
    D_IMediaSample * pDest;

    HRESULT hr;
    D_REFERENCE_TIME tStart, tStop;
    const BOOL bTime = S_OK == pSource->GetTime( &tStart, &tStop);

    // this may block for an indeterminate amount of time
    hr = OutputPin()->PeekAllocator()->GetBuffer(
              &pDest
              , bTime ? &tStart : NULL
              , bTime ? &tStop : NULL
              , ( DWORD )( m_bSampleSkipped ? AM_GBF_PREVFRAMESKIPPED : 0 )
              );

    if (FAILED(hr)) {
        return NULL;
    }

    ASSERT(pDest);
    D_IMediaSample2 *pSample2;
    if (SUCCEEDED(pDest->QueryInterface(IID_IMEDIASAMPLE2, (void **)&pSample2))) {
        hr = pSample2->SetProperties(
            FIELD_OFFSET(D_AM_SAMPLE2_PROPERTIES, pbBuffer),
            (PBYTE)m_pInput->SampleProps());
        pSample2->Release();
        if (FAILED(hr)) {
            pDest->Release();
            return NULL;
        }
    } else {
        if (bTime) {
            pDest->SetTime(&tStart, &tStop);
        }

        if (S_OK == pSource->IsSyncPoint()) {
            pDest->SetSyncPoint(TRUE);
        }
        if (S_OK == pSource->IsDiscontinuity() || m_bSampleSkipped) {
            pDest->SetDiscontinuity(TRUE);
        }
        if (S_OK == pSource->IsPreroll()) {
            pDest->SetPreroll(TRUE);
        }

        // Copy the media type
        D_AM_MEDIA_TYPE *pMediaType;
        if (S_OK == pSource->GetMediaType(&pMediaType)) {
            pDest->SetMediaType(pMediaType);
            DeleteMediaType( pMediaType );
        }

    }

    m_bSampleSkipped = FALSE;

    // Copy the sample media times
    D_REFERENCE_TIME TimeStart, TimeEnd;
    if (pSource->GetMediaTime(&TimeStart,&TimeEnd) == NOERROR) {
        pDest->SetMediaTime(&TimeStart,&TimeEnd);
    }

    // Copy the actual data length and the actual data.
    {
        const long lDataLength = pSource->GetActualDataLength();
        pDest->SetActualDataLength(lDataLength);

        // Copy the sample data
        {
            BYTE *pSourceBuffer, *pDestBuffer;
//          long lSourceSize  = pSource->GetSize();
//          long lDestSize = pDest->GetSize();

//          ASSERT(lDestSize >= lSourceSize && lDestSize >= lDataLength);

            pSource->GetPointer(&pSourceBuffer);
            pDest->GetPointer(&pDestBuffer);
            ASSERT(lDestSize == 0 || pSourceBuffer != NULL && pDestBuffer != NULL);

            _MEMCPY( (PVOID) pDestBuffer, (PVOID) pSourceBuffer, ( size_t )lDataLength );
        }
    }

    return pDest;

} // Copy


// override this to customize the transform process

HRESULT
D_CTransInPlaceFilter::Receive(D_IMediaSample *pSample)
{
    /*  Check for other streams and pass them on */
    D_AM_SAMPLE2_PROPERTIES * const pProps = m_pInput->SampleProps();
    if (pProps->dwStreamId != AM_STREAM_MEDIA) {
        return m_pOutput->Deliver(pSample);
    }
    HRESULT hr;

    // Start timing the TransInPlace (if PERF is defined)
    MSR_START(m_idTransInPlace);

    if (UsingDifferentAllocators()) {

        // We have to copy the data.

        pSample = Copy(pSample);

        if (pSample==NULL) {
            MSR_STOP(m_idTransInPlace);
            return E_UNEXPECTED;
        }
    }

    // have the derived class transform the data
    hr = Transform(pSample);

    // Stop the clock and log it (if PERF is defined)
    MSR_STOP(m_idTransInPlace);

    if (FAILED(hr)) {
        DbgLog((LOG_TRACE, 1, TEXT("Error from TransInPlace")));
        if (UsingDifferentAllocators()) {
            pSample->Release();
        }
        return hr;
    }

    // the Transform() function can return S_FALSE to indicate that the
    // sample should not be delivered; we only deliver the sample if it's
    // really S_OK (same as NOERROR, of course.)
    if (hr == NOERROR) {
        hr = m_pOutput->Deliver(pSample);
    } else {
        //  But it would be an error to return this private workaround
        //  to the caller ...
        if (S_FALSE == hr) {
            // S_FALSE returned from Transform is a PRIVATE agreement
            // We should return NOERROR from Receive() in this cause because
            // returning S_FALSE from Receive() means that this is the end
            // of the stream and no more data should be sent.
            m_bSampleSkipped = TRUE;
            if (!m_bQualityChanged) {
                NotifyEvent(EC_QUALITY_CHANGE,0,0);
                m_bQualityChanged = TRUE;
            }
            hr = NOERROR;
        }
    }

    // release the output buffer. If the connected pin still needs it,
    // it will have addrefed it itself.
    if (UsingDifferentAllocators()) {
        pSample->Release();
    }

    return hr;

} // Receive


HRESULT D_CSampleGrabberAllocator::Alloc( )
{
    // look at the base class code to see where this came from!

    D_CAutoLock lck(this);

    // Check he has called SetProperties
    HRESULT hr = D_CBaseAllocator::Alloc();
    if (FAILED(hr)) {
        return hr;
    }

    // If the requirements haven't changed then don't reallocate
    if (hr == S_FALSE) {
        ASSERT(m_pBuffer);
        return NOERROR;
    }
    ASSERT(hr == S_OK); // we use this fact in the loop below

    // Free the old resources
    if (m_pBuffer) {
        ReallyFree();
    }

    // Compute the aligned size
    LONG lAlignedSize = m_lSize + m_lPrefix;
    if (m_lAlignment > 1) 
    {
        LONG lRemainder = lAlignedSize % m_lAlignment;
        if (lRemainder != 0) 
        {
            lAlignedSize += (m_lAlignment - lRemainder);
        }
    }

    // Create the contiguous memory block for the samples
    // making sure it's properly aligned (64K should be enough!)
    ASSERT(lAlignedSize % m_lAlignment == 0);

    // don't create the buffer - use what was passed to us
    //
    m_pBuffer = m_pPin->m_pBuffer;

    if (m_pBuffer == NULL) {
        return E_OUTOFMEMORY;
    }

    LPBYTE pNext = m_pBuffer;
    D_CMediaSample *pSample;

    ASSERT(m_lAllocated == 0);

    // Create the new samples - we have allocated m_lSize bytes for each sample
    // plus m_lPrefix bytes per sample as a prefix. We set the pointer to
    // the memory after the prefix - so that GetPointer() will return a pointer
    // to m_lSize bytes.
    for (; m_lAllocated < m_lCount; m_lAllocated++, pNext += lAlignedSize) 
    {
        pSample = new D_CMediaSample(
                                NAME("Sample Grabber memory media sample"),
                                this,
                                &hr,
                                pNext + m_lPrefix,      // GetPointer() value
                                m_lSize);               // not including prefix

        ASSERT(SUCCEEDED(hr));
        if (pSample == NULL)
            return E_OUTOFMEMORY;

        // This CANNOT fail
        m_lFree.Add(pSample);
    }

    m_bChanged = FALSE;
    return NOERROR;
}


//----------------------------------------------------------------------------
// don't really free the memory
//----------------------------------------------------------------------------

void D_CSampleGrabberAllocator::ReallyFree()
{
    // look at the base class code to see where this came from!

    // Should never be deleting this unless all buffers are freed

    ASSERT(m_lAllocated == m_lFree.GetCount());

    // Free up all the CMediaSamples

    D_CMediaSample *pSample;
    for (;;) 
    {
        pSample = m_lFree.RemoveHead();
        if (pSample != NULL) 
        {
            delete pSample;
        } 
        else 
        {
            break;
        }
    }

    m_lAllocated = 0;

    // don't free the buffer - let the app do it
}


//----------------------------------------------------------------------------
// SetProperties: Called by the upstream filter to set the allocator
// properties. The application has already allocated the buffer, so we reject 
// anything that is not compatible with that, and return the actual props.
//----------------------------------------------------------------------------

STDMETHODIMP D_CSampleGrabberAllocator::SetProperties(
    D_ALLOCATOR_PROPERTIES *pRequest, 
    D_ALLOCATOR_PROPERTIES *pActual
)
{
    HRESULT hr = D_CMemAllocator::SetProperties(pRequest, pActual);

    if (FAILED(hr))
    {
        return hr;
    }
    
    D_ALLOCATOR_PROPERTIES *pRequired = &(m_pPin->m_allocprops);
    if (pRequest->cbAlign != pRequired->cbAlign)
    {
        return VFW_E_BADALIGN;
    }
    if (pRequest->cbPrefix != pRequired->cbPrefix)
    {
        return E_FAIL;
    }
    if (pRequest->cbBuffer > pRequired->cbBuffer)
    {
        return E_FAIL;
    }
    if (pRequest->cBuffers > pRequired->cBuffers)
    {
        return E_FAIL;
    }

    *pActual = *pRequired;

    m_lCount = pRequired->cBuffers;
    m_lSize = pRequired->cbBuffer;
    m_lAlignment = pRequired->cbAlign;
    m_lPrefix = pRequired->cbPrefix;

    return S_OK;
}


D_CSampleGrabberInPin::D_CSampleGrabberInPin( D_CTransInPlaceFilter * pFilter, HRESULT * pHr ) 
//	: D_CTransInPlaceInputPin( TEXT("SampleGrabberInputPin\0"), pFilter, pHr, L"Input\0" )
	: D_CTransInPlaceInputPin( (char *)"SampleGrabberInputPin\0", pFilter, pHr, L"Input\0" )
	, m_pPrivateAllocator( NULL )
	, m_pBuffer( NULL )
	, m_bMediaTypeChanged( FALSE )
{
	_MEMSET( &m_allocprops, 0, sizeof( m_allocprops ) );
}

D_CSampleGrabberInPin::~D_CSampleGrabberInPin( )
{
	if( m_pPrivateAllocator ) delete m_pPrivateAllocator;
}


HRESULT D_CSampleGrabberInPin::GetMediaType( int iPosition, D_CMediaType * pMediaType )
{
    CheckPointer(pMediaType,E_POINTER);

    if (iPosition < 0) {
        return E_INVALIDARG;
    }
    if (iPosition > 0) {
        return VFW_S_NO_MORE_ITEMS;
    }

    *pMediaType = D_CMediaType( );
    pMediaType->SetType( ((D_CSampleGrabber*)m_pFilter)->m_mtAccept.Type( ) );

    return S_OK;
}


//----------------------------------------------------------------------------
// override the D_CTransInPlaceInputPin's method, and return a new enumerator
// if the input pin is disconnected. This will allow GetMediaType to be
// called. If we didn't do this, EnumMediaTypes returns a failure code
// and GetMediaType is never called. 
//----------------------------------------------------------------------------

STDMETHODIMP D_CSampleGrabberInPin::EnumMediaTypes( D_IEnumMediaTypes **ppEnum )
{
    CheckPointer(ppEnum,E_POINTER);
    ValidateReadWritePtr(ppEnum,sizeof(D_IEnumMediaTypes *));

    // if the output pin isn't connected yet, offer the possibly 
    // partially specified media type that has been set by the user

    if( !((D_CSampleGrabber*)m_pTIPFilter)->OutputPin( )->IsConnected() )
    {
        // Create a new reference counted enumerator

        *ppEnum = new D_CEnumMediaTypes( this, NULL );

        return (*ppEnum) ? NOERROR : E_OUTOFMEMORY;
    }

    // if the output pin is connected, offer it's fully qualified media type

    return ((D_CSampleGrabber*)m_pTIPFilter)->OutputPin( )->GetConnected()->EnumMediaTypes( ppEnum );
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

STDMETHODIMP D_CSampleGrabberInPin::NotifyAllocator( D_IMemAllocator *pAllocator, BOOL bReadOnly )
{
    if( m_pPrivateAllocator )
    {
        if( pAllocator != m_pPrivateAllocator )
        {
            return E_FAIL;
        }
        else
        {
            // if the upstream guy wants to be read only and we don't, then that's bad
            // if the upstream guy doesn't request read only, but we do, that's okay
            if( bReadOnly && !SampleGrabber( )->IsReadOnly( ) )
            {
                return E_FAIL;
            }
        }
    }

    return D_CTransInPlaceInputPin::NotifyAllocator( pAllocator, bReadOnly );
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

STDMETHODIMP D_CSampleGrabberInPin::GetAllocator( D_IMemAllocator **ppAllocator )
{
    if( m_pPrivateAllocator )
    {
        CheckPointer(ppAllocator,E_POINTER);

        *ppAllocator = m_pPrivateAllocator;
        m_pPrivateAllocator->AddRef( );
        return NOERROR;
    }
    else
    {
        return D_CTransInPlaceInputPin::GetAllocator( ppAllocator );
    }
}

//----------------------------------------------------------------------------
// GetAllocatorRequirements: The upstream filter calls this to get our
// filter's allocator requirements. If the app has set the buffer, then
// we return those props. Otherwise, we use the default TransInPlace behavior.
//----------------------------------------------------------------------------

STDMETHODIMP D_CSampleGrabberInPin::GetAllocatorRequirements( D_ALLOCATOR_PROPERTIES *pProps )
{
    CheckPointer(pProps,E_POINTER);

    if (m_pPrivateAllocator)
    {
        *pProps = m_allocprops;
        return S_OK;
    }
    else
    {
        return D_CTransInPlaceInputPin::GetAllocatorRequirements(pProps);
    }
}




//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

HRESULT D_CSampleGrabberInPin::SetDeliveryBuffer( D_ALLOCATOR_PROPERTIES props, BYTE * pBuffer )
{
    // don't allow more than one buffer

    if( props.cBuffers != 1 )
    {
        return E_INVALIDARG;
    }
    if( !pBuffer )
    {
        return E_POINTER;
    }

    m_allocprops = props;
    m_pBuffer = pBuffer;

    // If there is an existing allocator, make sure that it is released
    // to prevent a memory leak
    if (m_pPrivateAllocator)
    {
        m_pPrivateAllocator->Release();
        m_pPrivateAllocator = NULL;
    }

    HRESULT hr = S_OK;

    m_pPrivateAllocator = new D_CSampleGrabberAllocator( this, &hr );
    if( !m_pPrivateAllocator )
    {
        return E_OUTOFMEMORY;
    }

    m_pPrivateAllocator->AddRef( );
    return hr;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

HRESULT D_CSampleGrabberInPin::SetMediaType( const D_CMediaType *pmt )
{
    m_bMediaTypeChanged = TRUE;

    return D_CTransInPlaceInputPin::SetMediaType( pmt );
}




D_CUnknown * WINAPI D_CSampleGrabber::CreateInstance(D_IUnknown * punk, HRESULT *phr) 
{
    ASSERT(phr);
    
    // assuming we don't want to modify the data
    D_CSampleGrabber *pNewObject = new D_CSampleGrabber(punk, phr, FALSE);

    if(pNewObject == NULL) {
        if (phr)
            *phr = E_OUTOFMEMORY;
    }

    return pNewObject;   

} // CreateInstance

void D_CSampleGrabber::DeleteInstance()
{
	delete this ;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

D_CSampleGrabber::D_CSampleGrabber( D_IUnknown * pOuter, HRESULT * phr, BOOL ModifiesData )
//                : D_CTransInPlaceFilter( TEXT("SampleGrabber"), (D_IUnknown*) pOuter, 
                : D_CTransInPlaceFilter( (TCHAR  *)"SampleGrabber", (D_IUnknown*) pOuter, 
				CLSID_GRABBERSAMPLE, phr, ModifiesData ? true : false )
                , m_callback( NULL )
{
    // this is used to override the input pin with our own   
    m_pInput = (D_CTransInPlaceInputPin*) new D_CSampleGrabberInPin( this, phr );
    if( !m_pInput )
    {
        if (phr)
            *phr = E_OUTOFMEMORY;
    }
    
    // Ensure that the output pin gets created.  This is necessary because our
    // SetDeliveryBuffer() method assumes that the input/output pins are created, but
    // the output pin isn't created until GetPin() is called.  The 
    // D_CTransInPlaceFilter::GetPin() method will create the output pin, since we
    // have not already created one.
/*  D_IPin *pOutput = GetPin(1); */
    // The pointer is not AddRef'ed by GetPin(), so don't release it
}

D_CSampleGrabber::~D_CSampleGrabber()
{
}

STDMETHODIMP D_CSampleGrabber::NonDelegatingQueryInterface( REFIID riid, void ** ppv) 
{
    CheckPointer(ppv,E_POINTER);

    if(riid == IID_IGRABBERSAMPLE) {                
        return D_GetInterface((D_IGrabberSample *) this, ppv);
    }
    else {
        return D_CTransInPlaceFilter::NonDelegatingQueryInterface(riid, ppv);
    }
}


//----------------------------------------------------------------------------
// This is where you force the sample grabber to connect with one type
// or the other. What you do here is crucial to what type of data your
// app will be dealing with in the sample grabber's callback. For instance,
// if you don't enforce right-side-up video in this call, you may not get
// right-side-up video in your callback. It all depends on what you do here.
//----------------------------------------------------------------------------

HRESULT D_CSampleGrabber::CheckInputType( const D_CMediaType * pmt )
{
    CheckPointer(pmt,E_POINTER);
    D_CAutoLock lock( &m_Lock );

    // if the major type is not set, then accept anything

    GUID g = *m_mtAccept.Type( );
    if( g == _GUID_NULL )
    {
        return NOERROR;
    }

    // if the major type is set, don't accept anything else

    if( g != *pmt->Type( ) )
    {
        return VFW_E_INVALID_MEDIA_TYPE;
    }

    // subtypes must match, if set. if not set, accept anything

    g = *m_mtAccept.Subtype( );
    if( g == _GUID_NULL )
    {
        return NOERROR;
    }
    if( g != *pmt->Subtype( ) )
    {
        return VFW_E_INVALID_MEDIA_TYPE;
    }

    // format types must match, if one is set

    g = *m_mtAccept.FormatType( );
    if( g == _GUID_NULL )
    {
        return NOERROR;
    }
    if( g != *pmt->FormatType( ) )
    {
        return VFW_E_INVALID_MEDIA_TYPE;
    }

    // at this point, for this sample code, this is good enough,
    // but you may want to make it more strict

    return NOERROR;
}


//----------------------------------------------------------------------------
// This bit is almost straight out of the base classes.
// We override this so we can handle Transform( )'s error
// result differently.
//----------------------------------------------------------------------------

HRESULT D_CSampleGrabber::Receive( D_IMediaSample * pms )
{
    CheckPointer(pms,E_POINTER);

    HRESULT hr;
    D_AM_SAMPLE2_PROPERTIES * const pProps = m_pInput->SampleProps();

    if (pProps->dwStreamId != AM_STREAM_MEDIA) 
    {
        if( m_pOutput->IsConnected() )
            return m_pOutput->Deliver(pms);
        else
            return NOERROR;
    }

    if (UsingDifferentAllocators()) 
    {
        // We have to copy the data.

        pms = Copy(pms);

        if (pms == NULL) 
        {
            return E_UNEXPECTED;
        }
    }

    // have the derived class transform the data
    hr = Transform(pms);

    if (FAILED(hr)) 
    {
        DbgLog((LOG_TRACE, 1, TEXT("Error from TransInPlace")));
        if (UsingDifferentAllocators()) 
        {
            pms->Release();
        }
        return hr;
    }

    if (hr == NOERROR) 
    {
        hr = m_pOutput->Deliver(pms);
    }
    
    // release the output buffer. If the connected pin still needs it,
    // it will have addrefed it itself.
    if (UsingDifferentAllocators()) 
    {
        pms->Release();
    }

    return hr;
}


//----------------------------------------------------------------------------
// Transform
//----------------------------------------------------------------------------

HRESULT D_CSampleGrabber::Transform ( D_IMediaSample * pms )
{
    CheckPointer(pms,E_POINTER);
    D_CAutoLock lock( &m_Lock );

    if( m_callback )
    {
        D_REFERENCE_TIME StartTime, StopTime;
        pms->GetTime( &StartTime, &StopTime);

        StartTime += m_pInput->CurrentStartTime( );
        StopTime  += m_pInput->CurrentStartTime( );

        BOOL * pTypeChanged = &((D_CSampleGrabberInPin*) m_pInput)->m_bMediaTypeChanged;

        HRESULT hr = m_callback( pms, &StartTime, &StopTime, *pTypeChanged, m_CallbackData );

        *pTypeChanged = FALSE; // now that we notified user, we can clear it

        return hr;
    }

    return NOERROR;
}


//----------------------------------------------------------------------------
// SetAcceptedMediaType
//----------------------------------------------------------------------------

STDMETHODIMP D_CSampleGrabber::SetAcceptedMediaType( const D_CMediaType * pmt )
{
    D_CAutoLock lock( &m_Lock );

    if( !pmt )
    {
        m_mtAccept = D_CMediaType( );
        return NOERROR;        
    }

    HRESULT hr;
    hr = CopyMediaType( &m_mtAccept, pmt );

    return hr;
}

//----------------------------------------------------------------------------
// GetAcceptedMediaType
//----------------------------------------------------------------------------

STDMETHODIMP D_CSampleGrabber::GetConnectedMediaType( D_CMediaType * pmt )
{
    if( !m_pInput || !m_pInput->IsConnected( ) )
    {
        return VFW_E_NOT_CONNECTED;
    }

    return m_pInput->ConnectionMediaType( pmt );
}


//----------------------------------------------------------------------------
// SetCallback
//----------------------------------------------------------------------------

STDMETHODIMP D_CSampleGrabber::SetCallback( D_SAMPLECALLBACK Callback, void *CallbackData )
{
    D_CAutoLock lock( &m_Lock );

    m_callback = Callback;
	m_CallbackData = CallbackData ;

    return NOERROR;
}


//----------------------------------------------------------------------------
// inform the input pin of the allocator buffer we wish to use. See the
// input pin's SetDeliverBuffer method for comments. 
//----------------------------------------------------------------------------

STDMETHODIMP D_CSampleGrabber::SetDeliveryBuffer( D_ALLOCATOR_PROPERTIES props, BYTE * m_pBuffer )
{
    // have the input/output pins been created?
    if( !InputPin( ) || !OutputPin( ) )
    {
        return E_POINTER;
    }

    // they can't be connected if we're going to be changing delivery buffers
    //
    if( InputPin( )->IsConnected( ) || OutputPin( )->IsConnected( ) )
    {
        return E_INVALIDARG;
    }

    return ((D_CSampleGrabberInPin*)m_pInput)->SetDeliveryBuffer( props, m_pBuffer );
}


D_CUnknown * WINAPI D_CMemReader::CreateInstance( D_CMemStream *pStream, D_CMediaType *pmt, HRESULT *phr )
{
    ASSERT(phr);
    
    // assuming we don't want to modify the data
    D_CMemReader *pNewObject = new D_CMemReader( pStream, pmt, phr );

    if(pNewObject == NULL) {
        if (phr)
            *phr = E_OUTOFMEMORY;
    }

    return pNewObject;   

}

void D_CMemReader::DeleteInstance()
{
	delete this ;
}


D_CUnknown *WINAPI D_CMemStream::CreateInstance( LPBYTE pbData, LONGLONG llLength, DWORD dwKBPerSec )
{
    ASSERT(phr);
    
    // assuming we don't want to modify the data
    D_CMemStream *pNewObject = new D_CMemStream( pbData, llLength, dwKBPerSec );

    return ( D_CUnknown * )pNewObject;   
}

void D_CMemStream::DeleteInstance()
{
	delete this ;
}

D_CMemStream::D_CMemStream( LPBYTE pbData, LONGLONG llLength, DWORD dwKBPerSec ) :
	m_pbData(pbData),
	m_llLength(llLength),
	m_llPosition(0),
	m_dwKBPerSec(dwKBPerSec)
{
	SETUP_WIN_API

	m_dwTimeStart = WinAPIData.Win32Func.timeGetTimeFunc();
}

HRESULT D_CMemStream::SetPointer(LONGLONG llPos)
{
	if (llPos < 0 || llPos > m_llLength) {
		return S_FALSE;
	} else {
		m_llPosition = llPos;
		return S_OK;
	}
}

HRESULT D_CMemStream::Read(PBYTE pbBuffer, DWORD dwBytesToRead, BOOL /*bAlign*/, LPDWORD pdwBytesRead )
{
	D_CAutoLock lck(&m_csLock);
	DWORD dwReadLength;

	SETUP_WIN_API

	/*  Wait until the bytes are here! */
	DWORD dwTime = WinAPIData.Win32Func.timeGetTimeFunc();
	if( m_llPosition + dwBytesToRead > m_llLength )
	{
		dwReadLength = (DWORD)(m_llLength - m_llPosition );
	}
	else
	{
		dwReadLength = dwBytesToRead;
	}

	DWORD dwTimeToArrive = ((DWORD)m_llPosition + dwReadLength) / m_dwKBPerSec;
	if( dwTime - m_dwTimeStart < dwTimeToArrive )
	{
		WinAPIData.Win32Func.SleepFunc(dwTimeToArrive - dwTime + m_dwTimeStart);
	}

	_MEMCPY( ( PVOID )pbBuffer, (PVOID)(m_pbData + m_llPosition), dwReadLength);
	m_llPosition += dwReadLength;
	*pdwBytesRead = dwReadLength;
	return S_OK;
}

LONGLONG D_CMemStream::Size(LONGLONG *pSizeAvailable)
{
	SETUP_WIN_API

	LONGLONG llCurrentAvailable =
		Int32x32To64((WinAPIData.Win32Func.timeGetTimeFunc() - m_dwTimeStart),m_dwKBPerSec);

	*pSizeAvailable = DX_CSTRMBASE_MIN(m_llLength, llCurrentAvailable);
	return m_llLength;
}

DWORD D_CMemStream::Alignment()
{
	return 1;
}
void D_CMemStream::Lock()
{
	m_csLock.Lock();
}
void D_CMemStream::Unlock()
{
	m_csLock.Unlock();
}

#endif // DX_NON_DSHOW_MP3

//}

#endif // !defined( DX_NON_DSHOW_MOVIE ) && !defined( DX_NON_MOVIE )







































#ifndef DX_NON_MEDIA_FOUNDATION

// コンストラクタ
D_SoundConvertMFByteStream::D_SoundConvertMFByteStream( STREAMDATA *_Stream, ULONGLONG _StreamSize )
    :
	ReferenceCount( 0 ),
	ASyncReadRequest( 0 ),
	pMFASyncResult( NULL ),
	Stream( _Stream ),
	StreamSize( _StreamSize ),
	ASyncThread( NULL ),
	ASyncThreadEvent( NULL ),
	SeekRequest( 0 ),
	ASyncThreadEndFlag( 0 ),
	ASyncThreadEndRequestFlag( 0 )
{
	SETUP_WIN_API

	CriticalSection_Initialize( &ASyncReadCriticalSection ) ;

	AddRef() ;
	WinAPIData.Win32Func.MFCreateAttributesFunc( &pMFAttributes, 0 ) ;
}

// デストラクタ
D_SoundConvertMFByteStream::~D_SoundConvertMFByteStream()
{
    Close() ;

    if( pMFAttributes != NULL )
	{
        pMFAttributes->Release() ;
        pMFAttributes = NULL ;
    }

	if( pMFASyncResult != NULL )
	{
		pMFASyncResult->Release() ;
		pMFASyncResult = NULL ;
	}

	CriticalSection_Delete( &ASyncReadCriticalSection ) ;
	_MEMSET( &ASyncReadCriticalSection, 0, sizeof( ASyncReadCriticalSection ) ) ;
}

// IUnknown
HRESULT __stdcall D_SoundConvertMFByteStream::QueryInterface( REFIID riid, void **ppv )
{
	if( ppv == NULL )
	{
		return E_INVALIDARG ;
	}

	*ppv = NULL ;

    if( _MEMCMP( &riid, &IID_IMFBYTESTREAM, sizeof( GUID ) ) == 0 )
	{
		AddRef() ;
		*ppv = this ;
		return S_OK ;
	}
    else
	if( _MEMCMP( &riid, &IID_IMFATTRIBUTES, sizeof( GUID ) ) == 0 )
	{
		pMFAttributes->AddRef() ;
		*ppv = pMFAttributes ;
		return S_OK ;
	}

	return E_NOINTERFACE ;
}

ULONG __stdcall D_SoundConvertMFByteStream::AddRef()
{
	InterlockedIncrement( ( LONG * )&ReferenceCount );
	return ReferenceCount ;
}

ULONG __stdcall D_SoundConvertMFByteStream::Release()
{
	LONG lRef = InterlockedDecrement( ( LONG * )&ReferenceCount );
	if( lRef == 0 )
	{
		ReferenceCount++;

		delete this ;
		return ULONG( 0 ) ;
	}
	else
	{
		return ReferenceCount ;
	}
}

// IMFByteStream
HRESULT __stdcall D_SoundConvertMFByteStream::GetCapabilities(DWORD *pdwCapabilities)
{
    *pdwCapabilities = D_MFBYTESTREAM_IS_READABLE | D_MFBYTESTREAM_IS_SEEKABLE ;
    return S_OK ;
}

HRESULT __stdcall D_SoundConvertMFByteStream::GetLength(ULONGLONG *pqwLength)
{
    *pqwLength = StreamSize ;
    return S_OK ;
}

HRESULT __stdcall D_SoundConvertMFByteStream::SetLength(ULONGLONG /*qwLength*/)
{
    return E_NOTIMPL ;
}

HRESULT __stdcall D_SoundConvertMFByteStream::GetCurrentPosition(ULONGLONG *pqwPosition)
{
	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &ASyncReadCriticalSection ) ;

	// 非同期読み込み中だったら分岐
//	if( ASyncReadRequest )
//	{
//		LONGLONG ResultPos = 0 ;
//
//		if( SeekRequest )
//		{
//			ResultPos = SeekRequestPoint ;
//		}
//		else
//		{
//			ResultPos = ASyncReadPoint + ( LONGLONG )ASyncReadSize ;
//			if( ResultPos > ( LONGLONG )StreamSize )
//			{
//				ResultPos = ( LONGLONG )StreamSize ;
//			}
//		}
//
//		*pqwPosition = ResultPos ;
//	}
//	else
	{
	    *pqwPosition = Stream->ReadShred.Tell( Stream->DataPoint ) ;
	}

	// クリティカルセクションの解放
	CriticalSection_Unlock( &ASyncReadCriticalSection ) ;

    return S_OK ;
}

HRESULT __stdcall D_SoundConvertMFByteStream::SetCurrentPosition( ULONGLONG qwPosition )
{
    return Seek( D_msoBegin, qwPosition, 0, NULL ) ;
}

HRESULT __stdcall D_SoundConvertMFByteStream::IsEndOfStream(BOOL *pfEndOfStream)
{
	ULONGLONG Pos ;

	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &ASyncReadCriticalSection ) ;

	// 非同期読み込み中だったら分岐
//	if( ASyncReadRequest )
//	{
//		if( SeekRequest )
//		{
//			Pos = ( ULONGLONG )SeekRequestPoint ;
//		}
//		else
//		{
//			Pos = ASyncReadPoint + ( LONGLONG )ASyncReadSize ;
//			if( Pos > StreamSize )
//			{
//				Pos = StreamSize ;
//			}
//		}
//	}
//	else
	{
		Pos = ( ULONGLONG )Stream->ReadShred.Tell( Stream->DataPoint ) ;
	}

	*pfEndOfStream = Pos >= StreamSize ? TRUE : FALSE ;

	// クリティカルセクションの解放
	CriticalSection_Unlock( &ASyncReadCriticalSection ) ;

    return S_OK ;
}

HRESULT __stdcall D_SoundConvertMFByteStream::Read(BYTE *pb,ULONG cb,ULONG *pcbRead )
{
	size_t ReadSize ;

	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &ASyncReadCriticalSection ) ;

	// 非同期読み込み中だったらエラー
	if( ASyncReadRequest )
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &ASyncReadCriticalSection ) ;
		return E_FAIL ;
	}

	// シークリクエストがあったらシーク
//	if( SeekRequest )
//	{
//		Stream->ReadShred.Seek( Stream->DataPoint, SeekRequestPoint, SEEK_SET ) ;
//		SeekRequest = FALSE ;
//	}

	ReadSize = Stream->ReadShred.Read( pb, 1, cb, Stream->DataPoint ) ;

	// クリティカルセクションの解放
	CriticalSection_Unlock( &ASyncReadCriticalSection ) ;

	if( pcbRead != NULL )
	{
		*pcbRead = ( ULONG )ReadSize ;
	}

	return S_OK ;
}

HRESULT __stdcall D_SoundConvertMFByteStream::BeginRead( BYTE *pb,ULONG cb,D_IMFAsyncCallback *pCallback,D_IUnknown *punkState)
{
	HRESULT hr ;

	SETUP_WIN_API

	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &ASyncReadCriticalSection ) ;

	// 非同期読み込み中だったらエラー
	if( ASyncReadRequest )
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &ASyncReadCriticalSection ) ;
		return E_FAIL ;
	}

	// シークリクエストがあったらシーク
//	if( SeekRequest )
//	{
//		Stream->ReadShred.Seek( Stream->DataPoint, SeekRequestPoint, SEEK_SET ) ;
//		SeekRequest = FALSE ;
//	}

	hr = WinAPIData.Win32Func.MFCreateAsyncResultFunc(
		NULL,
		pCallback,
		punkState,
		&pMFASyncResult
	) ;
    if( FAILED( hr ) )
	{
		return hr ;
	}

	// 非同期読み込み中フラグを立てる
	ASyncReadRequest = TRUE ;

	pASyncReadBuffer = pb ;
	ASyncReadSize = cb ;
	ASyncReadPoint = Stream->ReadShred.Tell( Stream->DataPoint ) ;
	ASyncReadResultSize = 0 ;

	WinAPIData.Win32Func.SetEventFunc( ASyncThreadEvent ) ;

	// クリティカルセクションの解放
	CriticalSection_Unlock( &ASyncReadCriticalSection ) ;

    return S_OK ;
}

HRESULT __stdcall D_SoundConvertMFByteStream::EndRead( D_IMFAsyncResult *pResult, LONG *pcbRead )
{
	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &ASyncReadCriticalSection ) ;

	// 非同期読み込み中ではなかったらエラー
	if( ASyncReadRequest == FALSE )
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &ASyncReadCriticalSection ) ;
		return E_FAIL ;
	}

	if( pMFASyncResult != pResult )
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &ASyncReadCriticalSection ) ;
		return E_FAIL ;
	}

	// 結果を代入
	*pcbRead = ASyncReadResultSize ;

	pMFASyncResult->Release() ;
	pMFASyncResult = NULL ;

	// 非同期読み込み中フラグを倒す
	ASyncReadRequest = FALSE ;

	// クリティカルセクションの解放
	CriticalSection_Unlock( &ASyncReadCriticalSection ) ;

	return S_OK ;
}

HRESULT __stdcall D_SoundConvertMFByteStream::Write(const BYTE * /*pb*/,ULONG /*cb*/,ULONG * /*pcbWritten*/)
{
    return E_NOTIMPL;
}

HRESULT __stdcall D_SoundConvertMFByteStream::BeginWrite(const BYTE * /*pb*/,ULONG /*cb*/,D_IMFAsyncCallback * /*pCallback*/,D_IUnknown * /*punkState*/)
{
    return E_NOTIMPL;
}

HRESULT __stdcall D_SoundConvertMFByteStream::EndWrite(D_IMFAsyncResult * /*pResult*/,ULONG * /*pcbWritten*/)
{
    return E_NOTIMPL;
}

HRESULT __stdcall D_SoundConvertMFByteStream::Seek( D_MFBYTESTREAM_SEEK_ORIGIN SeekOrigin,LONGLONG llSeekOffset,DWORD /*dwSeekFlags*/,ULONGLONG *pqwCurrentPosition)
{
	int SeekMode ;

	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &ASyncReadCriticalSection ) ;

	switch( SeekOrigin )
	{
	case D_msoBegin   : SeekMode = SEEK_SET ; break ;
	case D_msoCurrent : SeekMode = SEEK_CUR ; break ;

	default :
		// クリティカルセクションの解放
		CriticalSection_Unlock( &ASyncReadCriticalSection ) ;
		return E_INVALIDARG ;
	}

	// 非同期読み込み中だったら次の読み込み先として保存
	if( ASyncReadRequest )
	{
//		SeekRequest = TRUE ;
//		SeekRequestPoint = 0 ;
//		switch( SeekOrigin )
//		{
//		case D_msoBegin   :
//			SeekRequestPoint = llSeekOffset ;
//			break ;
//
//		case D_msoCurrent :
//			SeekRequestPoint = ASyncReadPoint + ( LONGLONG )ASyncReadSize + llSeekOffset ;
//			break ;
//		}
//
//		if( SeekRequestPoint > ( LONGLONG )StreamSize )
//		{
//			SeekRequestPoint = ( LONGLONG )StreamSize ;
//		}
//		else
//		if( SeekRequestPoint < 0 )
//		{
//			SeekRequestPoint = 0 ;
//		}
//
//		if( pqwCurrentPosition != NULL )
//		{
//			*pqwCurrentPosition = SeekRequestPoint ;
//		}

		// クリティカルセクションの解放
		CriticalSection_Unlock( &ASyncReadCriticalSection ) ;
		return S_OK ;
	}

	Stream->ReadShred.Seek( Stream->DataPoint, llSeekOffset, SeekMode ) ;

	if( pqwCurrentPosition != NULL )
	{
		*pqwCurrentPosition = Stream->ReadShred.Tell( Stream->DataPoint ) ;
	}

	// クリティカルセクションの解放
	CriticalSection_Unlock( &ASyncReadCriticalSection ) ;

	return S_OK ;
}

HRESULT __stdcall D_SoundConvertMFByteStream::Flush(void)
{
    return E_NOTIMPL;
}

HRESULT __stdcall D_SoundConvertMFByteStream::Close( void )
{
	SETUP_WIN_API

//	// クリティカルセクションの取得
//	CRITICALSECTION_LOCK( &ASyncReadCriticalSection ) ;
//
//	// 非同期読み込み中だったら読み込み完了まで待つ
//	if( ASyncReadRequest )
//	{
//		int Flag = ASyncReadRequest ;
//
//		while( Flag )
//		{
//			// クリティカルセクションの解放
//			CriticalSection_Unlock( &ASyncReadCriticalSection ) ;
//
//			WinAPIData.Win32Func.SleepFunc( 1 ) ;
//
//			// クリティカルセクションの取得
//			CRITICALSECTION_LOCK( &ASyncReadCriticalSection ) ;
//			
//			Flag = ASyncReadRequest ;
//		}
//	}
//
//	// クリティカルセクションの解放
//	CriticalSection_Unlock( &ASyncReadCriticalSection ) ;

	if( ASyncThread != NULL )
	{
		// クリティカルセクションの取得
		CRITICALSECTION_LOCK( &ASyncReadCriticalSection ) ;

		// 終了リクエストをする
		ASyncThreadEndRequestFlag = 1 ;

		// クリティカルセクションの解放
		CriticalSection_Unlock( &ASyncReadCriticalSection ) ;

//		WinAPIData.Win32Func.SetEventFunc( ASyncThreadEvent ) ;
//		WinAPIData.Win32Func.WaitForSingleObjectFunc( ASyncThread, INFINITE ) ;

		while( ASyncThreadEndFlag == 0 )
		{
			WinAPIData.Win32Func.SleepFunc( 2 ) ;
		}

		WinAPIData.Win32Func.CloseHandleFunc( ASyncThreadEvent ) ;
//		WinAPIData.Win32Func.CloseHandleFunc( ASyncThread ) ;
		ASyncThreadEvent = NULL ;
		ASyncThread = NULL ;
	}

    return S_OK ;
}

HRESULT D_SoundConvertMFByteStream::Initialize( void )
{
	DWORD ThreadID ;

	SETUP_WIN_API

	ASyncThreadEvent = WinAPIData.Win32Func.CreateEventAFunc( NULL, FALSE, FALSE, NULL ) ;
	if( ASyncThreadEvent == NULL )
	{
		return E_FAIL;
	}

	ASyncThread = WinAPIData.Win32Func.CreateThreadFunc(
		NULL,
		0,
		&D_SoundConvertMFByteStream::AsyncThread,
		this,
		0,
		&ThreadID
	) ;
	if( ASyncThread == NULL )
	{
		WinAPIData.Win32Func.CloseHandleFunc( ASyncThreadEvent ) ;
		ASyncThreadEvent = NULL ;
		return E_FAIL ;
	}

	return S_OK ;
}

DWORD WINAPI D_SoundConvertMFByteStream::AsyncThread( void *pContext )
{
	D_SoundConvertMFByteStream *pThis ;
	DWORD Result ;

	SETUP_WIN_API
     
	pThis = ( D_SoundConvertMFByteStream * )pContext ;
	for(;;)
	{
		for(;;)
		{
			Result = WinAPIData.Win32Func.WaitForSingleObjectFunc( pThis->ASyncThreadEvent, 16 ) ;
			if( Result != WAIT_TIMEOUT || pThis->ASyncThreadEndRequestFlag )
			{
				break ;
			}
		}

		// クリティカルセクションの取得
		CRITICALSECTION_LOCK( &pThis->ASyncReadCriticalSection ) ;
		if( Result != WAIT_OBJECT_0 || pThis->ASyncThreadEndRequestFlag || pThis->ASyncReadRequest == FALSE )
		{
			// クリティカルセクションの解放
			CriticalSection_Unlock( &pThis->ASyncReadCriticalSection ) ;
			break ;
		}

		// クリティカルセクションの解放
		CriticalSection_Unlock( &pThis->ASyncReadCriticalSection ) ;

		// 読み込み
		pThis->ASyncReadResultSize = ( LONG )pThis->Stream->ReadShred.Read(
			( void * )pThis->pASyncReadBuffer, 1, pThis->ASyncReadSize, pThis->Stream->DataPoint ) ;

		WinAPIData.Win32Func.MFInvokeCallbackFunc( pThis->pMFASyncResult ) ;
	}

	pThis->ASyncThreadEndFlag = TRUE ;
	WinAPIData.Win32Func.ExitThreadFunc( 0 ) ;

	return 0 ;
}

extern	D_SoundConvertMFByteStream *New_D_SoundConvertMFByteStream( STREAMDATA *_Stream, ULONGLONG _StreamSize )
{
	return new D_SoundConvertMFByteStream( _Stream, _StreamSize ) ;
}

#endif // DX_NON_MEDIA_FOUNDATION


