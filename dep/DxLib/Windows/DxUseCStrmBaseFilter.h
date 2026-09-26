// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		標準Ｃライブラリ使用コード		DirectShow BaseClasses
// 
// 				Ver 3.25a
//
//       Copyright (c) 1992-2002 Microsoft Corporation.  All rights reserved.
// 
// -------------------------------------------------------------------------------

// 多重インクルード防止用マクロ
#ifndef DX_USECSTRMBASEFILTER_H
#define DX_USECSTRMBASEFILTER_H

#include "../DxCompileConfig.h"

#include "../DxLib.h"
#include "../DxThread.h"
#include "DxDirectX.h"
#include "DxWinAPI.h"

// ＤｉｒｅｃｔＳｈｏｗ -------------------------------------------------------

#ifndef DX_NON_MOVIE
#ifndef DX_NON_DSHOW_MOVIE

#ifndef DX_NON_NAMESPACE

//namespace DxLib
//{

#endif // DX_NON_NAMESPACE

#define D_AMMSF_NOGRAPHTHREAD					(0x1)
#define D_AMMSF_ADDDEFAULTRENDERER				(0x1)
#define D_DDSFF_PROGRESSIVERENDER				(0x1)

#define D_MS_SUCCESS_CODE(x)					MAKE_HRESULT(0, FACILITY_ITF, x)
#define D_MS_S_PENDING							D_MS_SUCCESS_CODE(1)

#define D_EC_COMPLETE							(0x01)
#define D_EC_USERABORT							(0x02)
#define D_EC_ERRORABORT							(0x03)
#define D_EC_STREAM_ERROR_STOPPED				(0x06)
#define D_EC_STREAM_ERROR_STILLPLAYING			(0x07)
#define D_EC_ERROR_STILLPLAYING					(0x08)
#define D_EC_END_OF_SEGMENT						(0x1C)

typedef enum tagD_AM_SEEKING_SEEKINGFLAGS
{
	D_AM_SEEKING_NoPositioning					= 0,
	D_AM_SEEKING_AbsolutePositioning			= 0x1,
	D_AM_SEEKING_RelativePositioning			= 0x2,
	D_AM_SEEKING_IncrementalPositioning			= 0x3,
	D_AM_SEEKING_PositioningBitsMask			= 0x3,
	D_AM_SEEKING_SeekToKeyFrame					= 0x4,
	D_AM_SEEKING_ReturnTime						= 0x8,
	D_AM_SEEKING_Segment						= 0x10,
	D_AM_SEEKING_NoFlush						= 0x20
} D_AM_SEEKING_SEEKING_FLAGS ;

typedef enum tagD_STREAM_STATE
{
	D_STREAMSTATE_STOP 							= 0,
	D_STREAMSTATE_RUN							= 1
} D_STREAM_STATE ;

typedef enum tagD_STREAM_TYPE
{
	D_STREAMTYPE_READ							= 0,
	D_STREAMTYPE_WRITE 							= 1,
	D_STREAMTYPE_TRANSFORM 						= 2
} D_STREAM_TYPE ;

typedef enum tagD_FILTERSTATE
{
	D_State_Stopped 							= 0,
	D_State_Paused 								= D_State_Stopped + 1,
	D_State_Running 							= D_State_Paused + 1
} D_FILTER_STATE ;

typedef enum tagD_VMR9Mode
{
	D_VMR9Mode_Windowed							= 0x1,
	D_VMR9Mode_Windowless						= 0x2,
	D_VMR9Mode_Renderless						= 0x4,
	D_VMR9Mode_Mask								= 0x7
} D_VMR9Mode ;

typedef enum tagD_VMR9SurfaceAllocationFlags
{
	D_VMR9AllocFlag_3DRenderTarget				= 0x1,
	D_VMR9AllocFlag_DXVATarget					= 0x2,
	D_VMR9AllocFlag_TextureSurface				= 0x4,
	D_VMR9AllocFlag_OffscreenSurface			= 0x8,
	D_VMR9AllocFlag_UsageReserved				= 0xf0,
	D_VMR9AllocFlag_UsageMask					= 0xff
} D_VMR9SurfaceAllocationFlags ;

typedef enum tagD_PIN_DIRECTION
{
	D_PINDIR_INPUT								= 0,
	D_PINDIR_OUTPUT								= D_PINDIR_INPUT + 1
} D_PIN_DIRECTION ;

typedef DWORD 									D_OAEVENT ;
typedef DWORD 									D_OAHWND ;
typedef UINT 									D_MMRESULT ;
typedef REFGUID 								D_REFMSPID ;
typedef double									D_REFTIME ;
typedef GUID 									D_MSPID ;
typedef LONGLONG 								D_STREAM_TIME ;
typedef	long									D_OAFilterState ;
typedef DWORD_PTR								D_HEVENT;
typedef DWORD_PTR								D_HSEMAPHORE;

typedef void ( __stdcall *D_PAPCFUNC )( DWORD_PTR dwParam );

typedef struct tagD_VMR9ALLOCATIONINFO
{
	DWORD										dwFlags;
	DWORD										dwWidth;
	DWORD										dwHeight;
	D_D3DFORMAT									Format;
	D_D3DPOOL									Pool;
	DWORD										MinBuffers;
	SIZE										szAspectRatio;
	SIZE										szNativeSize;
} D_VMR9ALLOCATIONINFO ;

typedef struct tagD_VMR9PRESENTATIONINFO
{
	DWORD										dwFlags;
	D_IDirect3DSurface9							*lpSurf;
	D_REFERENCE_TIME							rtStart;
	D_REFERENCE_TIME							rtEnd;
	SIZE										szAspectRatio;
	RECT										rcSrc;
	RECT										rcDst;
	DWORD										dwReserved1;
	DWORD										dwReserved2;
} D_VMR9PRESENTATIONINFO ;

class D_AM_MEDIA_TYPE
{
public:
	GUID										majortype;
	GUID										subtype;
	BOOL										bFixedSizeSamples;
	BOOL										bTemporalCompression;
	ULONG										lSampleSize;
	GUID										formattype;
	D_IUnknown									*pUnk;
	ULONG										cbFormat;
	BYTE										*pbFormat;
};

typedef struct tagD_PIN_INFO
{
	class D_IBaseFilter							*pFilter ;
	D_PIN_DIRECTION								dir;
	WCHAR										achName[ 128 ];
} D_PIN_INFO;

typedef struct tagD_FILTER_INFO
{
	WCHAR										achName[ 128 ];
	class D_IFilterGraph						*pGraph;
} D_FILTER_INFO;

typedef struct tagD_ALLOCATOR_PROPERTIES
{
	long										cBuffers;
	long										cbBuffer;
	long										cbAlign;
	long										cbPrefix;
} D_ALLOCATOR_PROPERTIES;

typedef enum tagD_QualityMessageType
{
	D_Famine									= 0,
	D_Flood										= D_Famine + 1
} D_QualityMessageType ;

typedef struct tagD_Quality
{
	D_QualityMessageType						Type;
	long										Proportion;
	D_REFERENCE_TIME							Late;
	D_REFERENCE_TIME							TimeStamp;
} D_Quality;

typedef enum tagD_AM_SAMPLE_PROPERTY_FLAGS
{
	AM_SAMPLE_SPLICEPOINT						= 0x1,
	AM_SAMPLE_PREROLL							= 0x2,
	AM_SAMPLE_DATADISCONTINUITY					= 0x4,
	AM_SAMPLE_TYPECHANGED						= 0x8,
	AM_SAMPLE_TIMEVALID							= 0x10,
	AM_SAMPLE_TIMEDISCONTINUITY					= 0x40,
	AM_SAMPLE_FLUSH_ON_PAUSE					= 0x80,
	AM_SAMPLE_STOPVALID							= 0x100,
	AM_SAMPLE_ENDOFSTREAM						= 0x200,
	AM_STREAM_MEDIA								= 0,
	AM_STREAM_CONTROL							= 1
} D_AM_SAMPLE_PROPERTY_FLAGS ;

typedef struct tagD_AM_SAMPLE2_PROPERTIES
{
	DWORD										cbData;
	DWORD										dwTypeSpecificFlags;
	DWORD										dwSampleFlags;
	LONG										lActual;
	D_REFERENCE_TIME							tStart;
	D_REFERENCE_TIME							tStop;
	DWORD										dwStreamId;
	D_AM_MEDIA_TYPE								*pMediaType;
	BYTE										*pbBuffer;
	LONG										cbBuffer;
} D_AM_SAMPLE2_PROPERTIES ;

typedef struct tagD_REGFILTER
{
	CLSID										Clsid;
	LPWSTR										Name;
} D_REGFILTER;

typedef struct tagD_REGPINTYPES
{
	const CLSID									*clsMajorType;
	const CLSID									*clsMinorType;
} D_REGPINTYPES;

typedef struct tagD_REGFILTERPINS
{
	LPWSTR										strName;
	BOOL										bRendered;
	BOOL										bOutput;
	BOOL										bZero;
	BOOL										bMany;
	const CLSID									*clsConnectsToFilter;
	const WCHAR									*strConnectsToPin;
	UINT										nMediaTypes;
	const D_REGPINTYPES							*lpMediaType;
} D_REGFILTERPINS;

typedef D_REGFILTERPINS D_AMOVIESETUP_PIN, * D_PAMOVIESETUP_PIN, * FAR D_LPAMOVIESETUP_PIN ;

typedef struct tagD_AMOVIESETUP_FILTER
{
	const CLSID									*clsID;
	const WCHAR									*strName;
	DWORD										dwMerit;
	UINT										nPins;
	const D_AMOVIESETUP_PIN						*lpPin;
} D_AMOVIESETUP_FILTER, *D_PAMOVIESETUP_FILTER, * FAR D_LPAMOVIESETUP_FILTER;

STDAPI D_AMGetWideString( LPCWSTR pszString, LPWSTR *ppszReturn ) ;

class D_IEnumMediaTypes : public D_IUnknown
{
public:
	virtual HRESULT __stdcall Next						( ULONG cMediaTypes, D_AM_MEDIA_TYPE **ppMediaTypes, ULONG *pcFetched ) = 0;
	virtual HRESULT __stdcall Skip						( ULONG cMediaTypes ) = 0;
	virtual HRESULT __stdcall Reset						( void ) = 0;
	virtual HRESULT __stdcall Clone						( D_IEnumMediaTypes **ppEnum ) = 0;
};

class D_IPin : public D_IUnknown
{
public:
	virtual HRESULT __stdcall Connect					( D_IPin *pReceivePin, const D_AM_MEDIA_TYPE *pmt ) = 0 ;
	virtual HRESULT __stdcall ReceiveConnection			( D_IPin *pConnector, const D_AM_MEDIA_TYPE *pmt ) = 0 ;
	virtual HRESULT __stdcall Disconnect				( void ) = 0 ;
	virtual HRESULT __stdcall ConnectedTo				( D_IPin **pPin ) = 0 ;
	virtual HRESULT __stdcall ConnectionMediaType		( D_AM_MEDIA_TYPE *pmt ) = 0 ;
	virtual HRESULT __stdcall QueryPinInfo				( D_PIN_INFO *pInfo ) = 0 ;
	virtual HRESULT __stdcall QueryDirection			( D_PIN_DIRECTION *pPinDir ) = 0 ;
	virtual HRESULT __stdcall QueryId					( LPWSTR *Id ) = 0 ;
	virtual HRESULT __stdcall QueryAccept				( const D_AM_MEDIA_TYPE *pmt ) = 0 ;
	virtual HRESULT __stdcall EnumMediaTypes			( D_IEnumMediaTypes **ppEnum ) = 0 ;
	virtual HRESULT __stdcall QueryInternalConnections	( D_IPin **apPin, ULONG *nPin ) = 0 ;
	virtual HRESULT __stdcall EndOfStream				( void ) = 0 ;
	virtual HRESULT __stdcall BeginFlush				( void ) = 0 ;
	virtual HRESULT __stdcall EndFlush					( void ) = 0 ;
	virtual HRESULT __stdcall NewSegment				( D_REFERENCE_TIME tStart, D_REFERENCE_TIME tStop, double dRate) = 0 ;
};

class D_IEnumPins : public D_IUnknown
{
public:
	virtual HRESULT __stdcall Next						( ULONG cPins, D_IPin **ppPins, ULONG *pcFetched ) = 0 ;
	virtual HRESULT __stdcall Skip						( ULONG cPins ) = 0 ;
	virtual HRESULT __stdcall Reset						( void ) = 0 ;
	virtual HRESULT __stdcall Clone						( D_IEnumPins **ppEnum ) = 0 ;
};

class D_IReferenceClock : public D_IUnknown
{
public:
	virtual HRESULT __stdcall GetTime					( D_REFERENCE_TIME *pTime ) = 0 ;
	virtual HRESULT __stdcall AdviseTime				( D_REFERENCE_TIME baseTime, D_REFERENCE_TIME streamTime, D_HEVENT hEvent, DWORD_PTR *pdwAdviseCookie ) = 0;
	virtual HRESULT __stdcall AdvisePeriodic			( D_REFERENCE_TIME startTime, D_REFERENCE_TIME periodTime, D_HSEMAPHORE hSemaphore, DWORD_PTR *pdwAdviseCookie) = 0;
	virtual HRESULT __stdcall Unadvise					( DWORD_PTR dwAdviseCookie) = 0;
} ;

class D_IMediaSample : public D_IUnknown
{
public:
	virtual HRESULT __stdcall GetPointer				( BYTE **ppBuffer) = 0;
	virtual long    __stdcall GetSize					( void) = 0;
	virtual HRESULT __stdcall GetTime					( D_REFERENCE_TIME *pTimeStart, D_REFERENCE_TIME *pTimeEnd) = 0;
	virtual HRESULT __stdcall SetTime					( D_REFERENCE_TIME *pTimeStart, D_REFERENCE_TIME *pTimeEnd) = 0;
	virtual HRESULT __stdcall IsSyncPoint				( void) = 0;
	virtual HRESULT __stdcall SetSyncPoint				( BOOL bIsSyncPoint) = 0;
	virtual HRESULT __stdcall IsPreroll					( void) = 0;
	virtual HRESULT __stdcall SetPreroll				( BOOL bIsPreroll) = 0;
	virtual long    __stdcall GetActualDataLength		( void) = 0;
	virtual HRESULT __stdcall SetActualDataLength		( long __MIDL_0010) = 0;
	virtual HRESULT __stdcall GetMediaType				( D_AM_MEDIA_TYPE **ppMediaType) = 0;
	virtual HRESULT __stdcall SetMediaType				( D_AM_MEDIA_TYPE *pMediaType) = 0;
	virtual HRESULT __stdcall IsDiscontinuity			( void) = 0;
	virtual HRESULT __stdcall SetDiscontinuity			( BOOL bDiscontinuity) = 0;
	virtual HRESULT __stdcall GetMediaTime				( LONGLONG *pTimeStart, LONGLONG *pTimeEnd) = 0;
	virtual HRESULT __stdcall SetMediaTime				( LONGLONG *pTimeStart, LONGLONG *pTimeEnd) = 0;
};

class D_IMediaSample2 : public D_IMediaSample
{
public:
    virtual HRESULT __stdcall GetProperties				( DWORD cbProperties, BYTE *pbProperties) = 0;
    virtual HRESULT __stdcall SetProperties				( DWORD cbProperties, const BYTE *pbProperties) = 0;
};

class D_IMemAllocator : public D_IUnknown
{
public:
	virtual HRESULT __stdcall SetProperties				( D_ALLOCATOR_PROPERTIES *pRequest, D_ALLOCATOR_PROPERTIES *pActual) = 0;
	virtual HRESULT __stdcall GetProperties				( D_ALLOCATOR_PROPERTIES *pProps) = 0;
	virtual HRESULT __stdcall Commit					( void) = 0;
	virtual HRESULT __stdcall Decommit					( void) = 0;
	virtual HRESULT __stdcall GetBuffer					( D_IMediaSample **ppBuffer, D_REFERENCE_TIME *pStartTime, D_REFERENCE_TIME *pEndTime, DWORD dwFlags) = 0;
	virtual HRESULT __stdcall ReleaseBuffer				( D_IMediaSample *pBuffer) = 0;
};

class D_IMemAllocatorNotifyCallbackTemp : public D_IUnknown
{
public:
    virtual HRESULT __stdcall NotifyRelease				() = 0;
};

class D_IMemAllocatorCallbackTemp : public D_IMemAllocator
{
public:
	virtual HRESULT __stdcall SetNotify					( D_IMemAllocatorNotifyCallbackTemp *pNotify) = 0;		
	virtual HRESULT __stdcall GetFreeCount				( LONG *plBuffersFree) = 0;
};

class D_IMemInputPin : public D_IUnknown
{
public:
	virtual HRESULT __stdcall GetAllocator				( D_IMemAllocator **ppAllocator) = 0;
	virtual HRESULT __stdcall NotifyAllocator			( D_IMemAllocator *pAllocator, BOOL bReadOnly) = 0;
	virtual HRESULT __stdcall GetAllocatorRequirements	( D_ALLOCATOR_PROPERTIES *pProps) = 0;
	virtual HRESULT __stdcall Receive					( D_IMediaSample *pSample) = 0;
	virtual HRESULT __stdcall ReceiveMultiple			( D_IMediaSample **pSamples, long nSamples, long *nSamplesProcessed) = 0;
	virtual HRESULT __stdcall ReceiveCanBlock			( void) = 0;
};

class D_IMediaFilter : public D_IPersist
{
public:
	virtual HRESULT __stdcall Stop						( void ) = 0 ;
	virtual HRESULT __stdcall Pause						( void ) = 0 ;
	virtual HRESULT __stdcall Run						( D_REFERENCE_TIME tStart ) = 0 ;
	virtual HRESULT __stdcall GetState					( DWORD dwMilliSecsTimeout, D_FILTER_STATE *State ) = 0 ;
	virtual HRESULT __stdcall SetSyncSource				( D_IReferenceClock *pClock ) = 0 ;
	virtual HRESULT __stdcall GetSyncSource				( D_IReferenceClock **pClock ) = 0 ;

};

class D_IBaseFilter : public D_IMediaFilter
{
public:
	virtual HRESULT __stdcall EnumPins					( D_IEnumPins **ppEnum ) = 0 ;
	virtual HRESULT __stdcall FindPin					( LPCWSTR Id, D_IPin **ppPin) = 0;
	virtual HRESULT __stdcall QueryFilterInfo			( D_FILTER_INFO *pInfo) = 0;
	virtual HRESULT __stdcall JoinFilterGraph			( class D_IFilterGraph *pGraph, LPCWSTR pName) = 0;
	virtual HRESULT __stdcall QueryVendorInfo			( LPWSTR *pVendorInfo) = 0;

};

class D_IQualityControl : public D_IUnknown
{
public:
	virtual HRESULT __stdcall Notify					( D_IBaseFilter *pSelf, D_Quality q ) = 0 ;
	virtual HRESULT __stdcall SetSink					( D_IQualityControl *piqc ) = 0 ;
};

class D_IQualProp : public D_IUnknown
{
public :
	virtual HRESULT __stdcall get_FramesDroppedInRenderer( int *pcFrames) = 0 ;  // Out
	virtual HRESULT __stdcall get_FramesDrawn			( int *pcFramesDrawn) = 0 ;         // Out
	virtual HRESULT __stdcall get_AvgFrameRate			( int *piAvgFrameRate) = 0 ;       // Out
	virtual HRESULT __stdcall get_Jitter				( int *iJitter) = 0 ;                    // Out
	virtual HRESULT __stdcall get_AvgSyncOffset			( int *piAvg) = 0 ;               // Out
	virtual HRESULT __stdcall get_DevSyncOffset			( int *piDev) = 0 ;               // Out
} ;

class D_IEnumFilters : public D_IUnknown
{
public:
	virtual HRESULT __stdcall Next						( ULONG cFilters, D_IBaseFilter **ppFilter, ULONG *pcFetched) = 0;
	virtual HRESULT __stdcall Skip						( ULONG cFilters) = 0;
	virtual HRESULT __stdcall Reset						( void) = 0;
	virtual HRESULT __stdcall Clone						( D_IEnumFilters **ppEnum) = 0;
};

class D_IFilterGraph : public D_IUnknown
{
public:
	virtual HRESULT __stdcall AddFilter					( D_IBaseFilter *pFilter, LPCWSTR pName ) = 0 ;
	virtual HRESULT __stdcall RemoveFilter				( D_IBaseFilter *pFilter) = 0;
	virtual HRESULT __stdcall EnumFilters				( D_IEnumFilters **ppEnum) = 0;	
	virtual HRESULT __stdcall FindFilterByName			( LPCWSTR pName, D_IBaseFilter **ppFilter) = 0;
	virtual HRESULT __stdcall ConnectDirect				( D_IPin *ppinOut, D_IPin *ppinIn, const D_AM_MEDIA_TYPE *pmt) = 0;
	virtual HRESULT __stdcall Reconnect					( D_IPin *ppin) = 0;
	virtual HRESULT __stdcall Disconnect				( D_IPin *ppin) = 0;
	virtual HRESULT __stdcall SetDefaultSyncSource		( void) = 0;
	
};

class D_IGraphBuilder : public D_IFilterGraph
{
public:
	virtual HRESULT __stdcall Connect					( D_IPin *ppinOut, D_IPin *ppinIn) = 0;
	virtual HRESULT __stdcall Render					( D_IPin *ppinOut) = 0;
	virtual HRESULT __stdcall RenderFile				( LPCWSTR lpcwstrFile, LPCWSTR lpcwstrPlayList ) = 0;
	virtual HRESULT __stdcall AddSourceFilter			( LPCWSTR lpcwstrFileName, LPCWSTR lpcwstrFilterName, D_IBaseFilter **ppFilter) = 0;
	virtual HRESULT __stdcall SetLogFile				( DWORD_PTR hFile) = 0;
	virtual HRESULT __stdcall Abort						( void ) = 0;
	virtual HRESULT __stdcall ShouldOperationContinue	( void ) = 0;
};

class D_IFilterGraph2 : public D_IGraphBuilder
{
public:
	virtual HRESULT __stdcall AddSourceFilterForMoniker	( IMoniker *pMoniker, IBindCtx *pCtx, LPCWSTR lpcwstrFilterName, D_IBaseFilter **ppFilter) = 0;
	virtual HRESULT __stdcall ReconnectEx				( D_IPin *ppin, const D_AM_MEDIA_TYPE *pmt) = 0;
	virtual HRESULT __stdcall RenderEx					( D_IPin *pPinOut, DWORD dwFlags, DWORD *pvContext) = 0;
};

class D_IEnumRegFilters : public D_IUnknown
{
public:
	virtual HRESULT __stdcall Next						( ULONG cFilters, D_REGFILTER **apRegFilter, ULONG *pcFetched) = 0;
	virtual HRESULT __stdcall Skip						( ULONG cFilters) = 0;
	virtual HRESULT __stdcall Reset						( void) = 0;
	virtual HRESULT __stdcall Clone						( D_IEnumRegFilters **ppEnum) = 0;
};

class D_IFilterMapper : public D_IUnknown
{
public:
	virtual HRESULT __stdcall RegisterFilter			( CLSID clsid, LPCWSTR Name, DWORD dwMerit) = 0;
	virtual HRESULT __stdcall RegisterFilterInstance	( CLSID clsid, LPCWSTR Name, CLSID *MRId) = 0;
	virtual HRESULT __stdcall RegisterPin				( CLSID Filter, LPCWSTR Name, BOOL bRendered, BOOL bOutput, BOOL bZero, BOOL bMany, CLSID ConnectsToFilter, LPCWSTR ConnectsToPin) = 0;
	virtual HRESULT __stdcall RegisterPinType			( CLSID clsFilter, LPCWSTR strName, CLSID clsMajorType, CLSID clsSubType) = 0;
	virtual HRESULT __stdcall UnregisterFilter			( CLSID Filter) = 0;
	virtual HRESULT __stdcall UnregisterFilterInstance	( CLSID MRId) = 0;
	virtual HRESULT __stdcall UnregisterPin				( CLSID Filter, LPCWSTR Name) = 0;
	virtual HRESULT __stdcall EnumMatchingFilters		( D_IEnumRegFilters **ppEnum, DWORD dwMerit, BOOL bInputNeeded, CLSID clsInMaj, CLSID clsInSub, BOOL bRender, BOOL bOututNeeded, CLSID clsOutMaj, CLSID clsOutSub) = 0;
};

class D_IStreamSample : public D_IUnknown
{
public:
	virtual HRESULT __stdcall GetMediaStream			( class D_IMediaStream **ppMediaStream ) = 0 ;
	virtual HRESULT __stdcall GetSampleTimes			( D_STREAM_TIME *pStartTime, D_STREAM_TIME *pEndTime, D_STREAM_TIME *pCurrentTime) = 0;
	virtual HRESULT __stdcall SetSampleTimes			( const D_STREAM_TIME *pStartTime, const D_STREAM_TIME *pEndTime ) = 0;
	virtual HRESULT __stdcall Update					( DWORD dwFlags, HANDLE hEvent, D_PAPCFUNC pfnAPC, DWORD_PTR dwAPCData) = 0;
	virtual HRESULT __stdcall CompletionStatus			( DWORD dwFlags, DWORD dwMilliseconds) = 0;
};

class D_IDirectDrawStreamSample : public D_IStreamSample
{
public:
	virtual HRESULT __stdcall GetSurface				( D_IDirectDrawSurface **ppDirectDrawSurface, RECT *pRect) = 0;
	virtual HRESULT __stdcall NonUse03					( void ) = 0 ;				// SetRect( const RECT *pRect) = 0;
};

class D_IMultiMediaStream : public D_IUnknown
{
public:
	virtual HRESULT __stdcall NonUse00					( void ) = 0 ;				// GetInformation( DWORD *pdwFlags, STREAM_TYPE *pStreamType ) = 0 ;
	virtual HRESULT __stdcall GetMediaStream			( D_REFMSPID idPurpose, class D_IMediaStream **ppMediaStream ) = 0 ;
	virtual HRESULT __stdcall NonUse01					( void ) = 0 ;				// EnumMediaStreams( long Index, class IMediaStream **ppMediaStream ) = 0 ;
	virtual HRESULT __stdcall GetState					( D_STREAM_STATE *pCurrentState ) = 0 ;
	virtual HRESULT __stdcall SetState					( D_STREAM_STATE NewState ) = 0 ;
	virtual HRESULT __stdcall GetTime					( D_STREAM_TIME *pCurrentTime ) = 0 ;
	virtual HRESULT __stdcall NonUse02					( void ) = 0 ;				// GetDuration( D_STREAM_TIME *pDuration ) = 0 ;
	virtual HRESULT __stdcall Seek						( D_STREAM_TIME SeekTime ) = 0 ;
	virtual HRESULT __stdcall NonUse03					( void ) = 0 ;				// GetEndOfStreamEventHandle( HANDLE *phEOS ) = 0 ;
} ;

class D_IAMMultiMediaStream : public D_IMultiMediaStream
{
public:
	virtual HRESULT __stdcall Initialize				( D_STREAM_TYPE StreamType, DWORD dwFlags, D_IGraphBuilder *pFilterGraph) = 0;	
	virtual HRESULT __stdcall NonUse04					( void ) = 0 ;				// GetFilterGraph( IGraphBuilder **ppGraphBuilder) = 0;	
	virtual HRESULT __stdcall NonUse05					( void ) = 0 ;				// GetFilter( IMediaStreamFilter **ppFilter) = 0;	
	virtual HRESULT __stdcall AddMediaStream			( D_IUnknown *pStreamObject, const D_MSPID *PurposeId, DWORD dwFlags, D_IMediaStream **ppNewStream) = 0;
	virtual HRESULT __stdcall OpenFile					( LPCWSTR pszFileName, DWORD dwFlags) = 0;
	virtual HRESULT __stdcall NonUse06					( void ) = 0 ;				// OpenMoniker( IBindCtx *pCtx, IMoniker *pMoniker, DWORD dwFlags) = 0;
	virtual HRESULT __stdcall NonUse07					( void ) = 0 ;				// Render( DWORD dwFlags) = 0;
} ;

class D_IMediaStream : public D_IUnknown
{
public:
	virtual HRESULT __stdcall NonUse00					( void ) = 0 ;				// GetMultiMediaStream( IMultiMediaStream **ppMultiMediaStream) = 0;
	virtual HRESULT __stdcall NonUse01					( void ) = 0 ;				// GetInformation( MSPID *pPurposeId, STREAM_TYPE *pType) = 0;
	virtual HRESULT __stdcall NonUse02					( void ) = 0 ;				// SetSameFormat( IMediaStream *pStreamThatHasDesiredFormat, DWORD dwFlags) = 0;
	virtual HRESULT __stdcall NonUse03					( void ) = 0 ;				// AllocateSample( DWORD dwFlags, IStreamSample **ppSample) = 0;
	virtual HRESULT __stdcall NonUse04					( void ) = 0 ;				// CreateSharedSample( IStreamSample *pExistingSample, DWORD dwFlags, IStreamSample **ppNewSample) = 0;
	virtual HRESULT __stdcall NonUse05					( void ) = 0 ;				// SendEndOfStream( DWORD dwFlags) = 0;
};

class D_IAMMediaStream : public D_IMediaStream
{
public:
	virtual HRESULT __stdcall NonUse06					( void ) = 0 ;				// Initialize( D_IUnknown *pSourceObject, DWORD dwFlags, REFMSPID PurposeId, const STREAM_TYPE StreamType) = 0;
	virtual HRESULT __stdcall SetState( D_FILTER_STATE State) = 0;
	virtual HRESULT __stdcall NonUse08					( void ) = 0 ;				// JoinAMMultiMediaStream( IAMMultiMediaStream *pAMMultiMediaStream) = 0;
	virtual HRESULT __stdcall NonUse09					( void ) = 0 ;				// JoinFilter( IMediaStreamFilter *pMediaStreamFilter) = 0;
	virtual HRESULT __stdcall NonUse10					( void ) = 0 ;				// JoinFilterGraph( IFilterGraph *pFilterGraph) = 0;
};

class D_IDirectDrawMediaStream : public D_IMediaStream
{
public:
	virtual HRESULT __stdcall GetFormat					( D_DDSURFACEDESC *pDDSDCurrent, D_IDirectDrawPalette **ppDirectDrawPalette, D_DDSURFACEDESC *pDDSDDesired, DWORD *pdwFlags) = 0;
	virtual HRESULT __stdcall NonUse06					( void ) = 0 ;				// SetFormat( const DDSURFACEDESC *pDDSurfaceDesc, IDirectDrawPalette *pDirectDrawPalette) = 0;
	virtual HRESULT __stdcall NonUse07					( void ) = 0 ;				// GetDirectDraw( IDirectDraw **ppDirectDraw) = 0;
	virtual HRESULT __stdcall NonUse08					( void ) = 0 ;				// SetDirectDraw( IDirectDraw *pDirectDraw) = 0;
	virtual HRESULT __stdcall CreateSample				( D_IDirectDrawSurface *pSurface, const RECT *pRect, DWORD dwFlags, class D_IDirectDrawStreamSample **ppSample) = 0;
	virtual HRESULT __stdcall GetTimePerFrame			( D_STREAM_TIME *pFrameTime) = 0;
};

class D_IMediaControl : public D_IDispatch
{
public:
	virtual HRESULT __stdcall Run						( void ) = 0 ;
	virtual HRESULT __stdcall Pause						( void ) = 0 ;
	virtual HRESULT __stdcall Stop						( void ) = 0 ;
	virtual HRESULT __stdcall GetState					( LONG msTimeout, D_OAFilterState *pfs ) = 0;
	virtual HRESULT __stdcall RenderFile				( BSTR strFilename) = 0;
	virtual HRESULT __stdcall AddSourceFilter			( BSTR strFilename, D_IDispatch **ppUnk) = 0;
	virtual HRESULT __stdcall get_FilterCollection		( D_IDispatch **ppUnk) = 0;
	virtual HRESULT __stdcall get_RegFilterCollection	( D_IDispatch **ppUnk) = 0;
	virtual HRESULT __stdcall StopWhenReady				( void ) = 0 ;
};

class D_IMediaSeeking : public D_IUnknown
{
public:
	virtual HRESULT __stdcall GetCapabilities			( DWORD *pCapabilities) = 0;
	virtual HRESULT __stdcall CheckCapabilities			( DWORD *pCapabilities) = 0;
	virtual HRESULT __stdcall IsFormatSupported			( const GUID *pFormat) = 0;
	virtual HRESULT __stdcall QueryPreferredFormat		( GUID *pFormat) = 0;
	virtual HRESULT __stdcall GetTimeFormat				( GUID *pFormat ) = 0 ;
	virtual HRESULT __stdcall IsUsingTimeFormat			( const GUID *pFormat) = 0;
	virtual HRESULT __stdcall SetTimeFormat				( const GUID *pFormat ) = 0 ;
	virtual HRESULT __stdcall GetDuration				( LONGLONG *pDuration) = 0;
	virtual HRESULT __stdcall GetStopPosition			( LONGLONG *pStop) = 0 ;
	virtual HRESULT __stdcall GetCurrentPosition		( LONGLONG *pCurrent) = 0 ;
	virtual HRESULT __stdcall ConvertTimeFormat			( LONGLONG *pTarget, const GUID *pTargetFormat, LONGLONG Source, const GUID *pSourceFormat) = 0;
	virtual HRESULT __stdcall SetPositions				( LONGLONG *pCurrent, DWORD dwCurrentFlags, LONGLONG *pStop, DWORD dwStopFlags) = 0;
	virtual HRESULT __stdcall GetPositions				( LONGLONG *pCurrent, LONGLONG *pStop) = 0;
	virtual HRESULT __stdcall GetAvailable				( LONGLONG *pEarliest, LONGLONG *pLatest) = 0;
	virtual HRESULT __stdcall SetRate					( double dRate) = 0;
	virtual HRESULT __stdcall GetRate					( double *pdRate) = 0;
	virtual HRESULT __stdcall GetPreroll				( LONGLONG *pllPreroll) = 0;
};

class D_IMediaPosition : public D_IDispatch
{
public:
	virtual HRESULT __stdcall get_Duration				( D_REFTIME *plength) = 0;
	virtual HRESULT __stdcall put_CurrentPosition		( D_REFTIME llTime) = 0;
	virtual HRESULT __stdcall get_CurrentPosition		( D_REFTIME *pllTime) = 0;
	virtual HRESULT __stdcall get_StopTime				( D_REFTIME *pllTime) = 0;
	virtual HRESULT __stdcall put_StopTime				( D_REFTIME llTime) = 0;
	virtual HRESULT __stdcall get_PrerollTime			( D_REFTIME *pllTime) = 0;
	virtual HRESULT __stdcall put_PrerollTime			( D_REFTIME llTime) = 0;
	virtual HRESULT __stdcall put_Rate					( double dRate) = 0;
	virtual HRESULT __stdcall get_Rate					( double *pdRate) = 0;
	virtual HRESULT __stdcall CanSeekForward			( LONG *pCanSeekForward) = 0;
	virtual HRESULT __stdcall CanSeekBackward			( LONG *pCanSeekBackward) = 0;
};

class D_ISampleGrabberCB : public D_IUnknown
{
public:
	virtual HRESULT  __stdcall SampleCB					( double SampleTime, D_IMediaSample *pSample ) = 0;
	virtual HRESULT  __stdcall BufferCB					( double SampleTime, BYTE *pBuffer, long BufferLen ) = 0;
};

class D_ISampleGrabber : public D_IUnknown
{
public:
	virtual HRESULT __stdcall SetOneShot				( BOOL OneShot) = 0;
	virtual HRESULT __stdcall SetMediaType				( const D_AM_MEDIA_TYPE *pType) = 0;
	virtual HRESULT __stdcall GetConnectedMediaType		( D_AM_MEDIA_TYPE *pType) = 0;
	virtual HRESULT __stdcall SetBufferSamples			( BOOL BufferThem) = 0;
	virtual HRESULT __stdcall GetCurrentBuffer			( long *pBufferSize, long *pBuffer) = 0;
	virtual HRESULT __stdcall GetCurrentSample			( D_IMediaSample **ppSample) = 0;
	virtual HRESULT __stdcall SetCallback				( D_ISampleGrabberCB *pCallback, long WhichMethodToCallback) = 0;
};

class D_IFileSinkFilter : public D_IUnknown
{
public:
	virtual HRESULT __stdcall SetFileName				( LPCOLESTR pszFileName, const D_AM_MEDIA_TYPE *pmt) = 0;
	virtual HRESULT __stdcall GetCurFile				( LPOLESTR *ppszFileName, D_AM_MEDIA_TYPE *pmt) = 0;
	
};

class D_IFileSinkFilter2 : public D_IFileSinkFilter
{
public:
	virtual HRESULT __stdcall SetMode					( DWORD dwFlags) = 0;
	virtual HRESULT __stdcall GetMode					( DWORD *pdwFlags) = 0;
	
};

class D_IBasicAudio : public D_IDispatch
{
public:
	virtual	HRESULT __stdcall put_Volume				( long lVolume) = 0;
	virtual	HRESULT __stdcall get_Volume				( long *plVolume) = 0;
	virtual	HRESULT __stdcall put_Balance				( long lBalance) = 0;
	virtual	HRESULT __stdcall get_Balance				( long *plBalance) = 0;
};

class D_IMediaEvent : public D_IDispatch
{
public:
	virtual HRESULT __stdcall NonUse00					( void ) = 0 ;				// GetEventHandle( OAEVENT *hEvent ) = 0 ;
	virtual HRESULT __stdcall GetEvent					( long *lEventCode, LONG_PTR *lParam1, LONG_PTR *lParam2, long msTimeout) = 0;
	virtual HRESULT __stdcall WaitForCompletion			( long msTimeout, long *pEvCode) = 0;
	virtual HRESULT __stdcall NonUse02					( void ) = 0 ;				// CancelDefaultHandling( long lEvCode) = 0;
	virtual HRESULT __stdcall NonUse03					( void ) = 0 ;				// RestoreDefaultHandling( long lEvCode) = 0;
	virtual HRESULT __stdcall FreeEventParams			( long lEvCode, LONG_PTR lParam1, LONG_PTR lParam2) = 0;
};

class D_IMediaEventEx : public D_IMediaEvent
{
public:
	virtual HRESULT __stdcall SetNotifyWindow			( D_OAHWND hwnd, long lMsg, LONG_PTR lInstanceData) = 0;
	virtual HRESULT __stdcall NonUse04					( void ) = 0 ;				// SetNotifyFlags( long lNoNotifyFlags) = 0;
	virtual HRESULT __stdcall NonUse05					( void ) = 0 ;				// GetNotifyFlags( long *lplNoNotifyFlags) = 0;
};

class D_IMediaEventSink : public D_IUnknown
{
public:
	virtual HRESULT __stdcall Notify					( long EventCode, LONG_PTR EventParam1, LONG_PTR EventParam2 ) = 0 ;
};

class D_IVMRSurfaceAllocatorNotify9 : public D_IUnknown
{
public:
	virtual HRESULT __stdcall AdviseSurfaceAllocator	( DWORD_PTR dwUserID, class D_IVMRSurfaceAllocator9 *lpIVRMSurfaceAllocator) = 0;
	virtual HRESULT __stdcall SetD3DDevice				( D_IDirect3DDevice9 *lpD3DDevice, D_HMONITOR hMonitor) = 0;
	virtual HRESULT __stdcall ChangeD3DDevice			( D_IDirect3DDevice9 *lpD3DDevice, D_HMONITOR hMonitor) = 0;
	virtual HRESULT __stdcall AllocateSurfaceHelper		( D_VMR9ALLOCATIONINFO *lpAllocInfo, DWORD *lpNumBuffers, D_IDirect3DSurface9 **lplpSurface) = 0;
	virtual HRESULT __stdcall NotifyEvent				( LONG EventCode, LONG_PTR Param1, LONG_PTR Param2) = 0;
};

class D_IVMRSurfaceAllocator9 : public D_IUnknown
{
public:
	virtual HRESULT __stdcall InitializeDevice			( DWORD_PTR dwUserID, D_VMR9ALLOCATIONINFO *lpAllocInfo, DWORD *lpNumBuffers) = 0;
	virtual HRESULT __stdcall TerminateDevice			( DWORD_PTR dwID) = 0;
	virtual HRESULT __stdcall GetSurface				( DWORD_PTR dwUserID, DWORD SurfaceIndex, DWORD SurfaceFlags, D_IDirect3DSurface9 **lplpSurface) = 0;
	virtual HRESULT __stdcall AdviseNotify				( D_IVMRSurfaceAllocatorNotify9 *lpIVMRSurfAllocNotify) = 0;
} ;

class D_IVMRImagePresenter9 : public D_IUnknown
{
public:
	virtual HRESULT __stdcall StartPresenting			( DWORD_PTR dwUserID) = 0;
	virtual HRESULT __stdcall StopPresenting			( DWORD_PTR dwUserID) = 0;
	virtual HRESULT __stdcall PresentImage				( DWORD_PTR dwUserID, D_VMR9PRESENTATIONINFO *lpPresInfo) = 0;
} ;

class D_IVMRFilterConfig9 : public D_IUnknown
{
public:
	virtual HRESULT __stdcall NonUse00					( void ) = 0 ;				// SetImageCompositor( D_IVMRImageCompositor9 *lpVMRImgCompositor) = 0;
	virtual HRESULT __stdcall SetNumberOfStreams		( DWORD dwMaxStreams) = 0;
	virtual HRESULT __stdcall GetNumberOfStreams		( DWORD *pdwMaxStreams) = 0;
	virtual HRESULT __stdcall SetRenderingPrefs			( DWORD dwRenderFlags) = 0;
	virtual HRESULT __stdcall GetRenderingPrefs			( DWORD *pdwRenderFlags) = 0;
	virtual HRESULT __stdcall SetRenderingMode			( DWORD Mode) = 0;
	virtual HRESULT __stdcall GetRenderingMode			( DWORD *pMode) = 0;
} ;

class D_IAMovieSetup : public D_IUnknown
{
public:
	virtual HRESULT __stdcall Register					( void ) = 0 ;
	virtual HRESULT __stdcall Unregister				( void ) = 0 ;
};


struct __D_POSITION { int unused; };
typedef __D_POSITION* D_POSITION;

interface D_INonDelegatingUnknown
{
	virtual HRESULT	__stdcall NonDelegatingQueryInterface( REFIID, LPVOID * ) = 0 ;
	virtual ULONG	__stdcall NonDelegatingAddRef		( void ) = 0 ;
	virtual ULONG	__stdcall NonDelegatingRelease		( void ) = 0 ;
};

class D_CBaseList 
{
public:
	class CNode
	{
		CNode									*m_pPrev;
		CNode									*m_pNext;
		void									*m_pObject;
	public:
		CNode();
			CNode			  *Prev						() const;
			CNode			  *Next						() const;
			void			  SetPrev					(CNode *p);
			void			  SetNext					(CNode *p);
			void			  *GetData					() const;
			void			  SetData					(void *p);
	};

	class CNodeCache
	{
	public:
		CNodeCache(INT iCacheSize);
		~CNodeCache();

			void			  AddToCache				(CNode *pNode);
			CNode			  *RemoveFromCache			();
	private:
		INT										m_iCacheSize;
		INT										m_iUsed;
		CNode									*m_pHead;
	};

protected:
	CNode										*m_pFirst;
	CNode										*m_pLast;
	LONG										m_Count;

private:
	CNodeCache									m_Cache;

	D_CBaseList	(const D_CBaseList &refList);
	D_CBaseList &operator=(const D_CBaseList &refList);

public:
	D_CBaseList(TCHAR *pName, INT iItems);
	D_CBaseList(TCHAR *pName);
#ifdef UNICODE
	D_CBaseList(CHAR *pName, INT iItems);
	D_CBaseList(CHAR *pName);
#endif
	~D_CBaseList();

			void			  RemoveAll					();
			D_POSITION		  GetHeadPositionI			() const;
			D_POSITION		  GetTailPositionI			() const;
			int				  GetCountI					() const;

protected:
			void			  *GetNextI					(D_POSITION& rp) const;
			void			  *GetI						(D_POSITION p) const;
			D_POSITION		  AddBeforeI				(D_POSITION p, void * pObj);
			D_POSITION		  AddHeadI					(void * pObj);
			D_POSITION		  AddAfterI					(D_POSITION p, void * pObj);
			D_POSITION		  FindI						( void * pObj) const;
			void			  *RemoveHeadI				();
			void			  *RemoveTailI				();
			void			  *RemoveI					(D_POSITION p);
			D_POSITION		  AddTailI					(void * pObj);

public:
			D_POSITION		  Next						(D_POSITION pos) const ;
			D_POSITION		  Prev						(D_POSITION pos) const ;
			BOOL			  AddTail					(D_CBaseList *pList);
			BOOL			  AddHead					(D_CBaseList *pList);
			BOOL			  AddAfter					(D_POSITION p, D_CBaseList *pList);
			BOOL			  AddBefore					(D_POSITION p, D_CBaseList *pList);
			BOOL			  MoveToTail				(D_POSITION pos, D_CBaseList *pList);
			BOOL			  MoveToHead				(D_POSITION pos, D_CBaseList *pList);
			void			  Reverse					();
};


template < class OBJECT > class D_CGenericList : public D_CBaseList
{
public:
	D_CGenericList(TCHAR *pName, INT iItems, BOOL bLock = TRUE, BOOL bAlert = FALSE) : D_CBaseList(pName, iItems) {
		UNREFERENCED_PARAMETER(bAlert);
		UNREFERENCED_PARAMETER(bLock);
	};
	D_CGenericList(TCHAR *pName) : D_CBaseList(pName) {};

			D_POSITION		  GetHeadPosition			() const { return (D_POSITION)m_pFirst; }
			D_POSITION		  GetTailPosition			() const { return (D_POSITION)m_pLast; }
			int				  GetCount					() const { return m_Count; }
			OBJECT			  *GetNext					( D_POSITION& rp) const { return (OBJECT *) GetNextI(rp); }
			OBJECT			  *Get						( D_POSITION p) const { return (OBJECT *) GetI(p); }
			OBJECT			  *GetHead					() const  { return Get(GetHeadPosition()); }
			OBJECT			  *RemoveHead				() { return (OBJECT *) RemoveHeadI(); }
			OBJECT			  *RemoveTail				() { return (OBJECT *) RemoveTailI(); }
			OBJECT			  *Remove					( D_POSITION p) { return (OBJECT *) RemoveI(p); }
			D_POSITION		  AddBefore					( D_POSITION p, OBJECT * pObj) { return AddBeforeI(p, pObj); }
			D_POSITION		  AddAfter					( D_POSITION p, OBJECT * pObj)  { return AddAfterI(p, pObj); }
			D_POSITION		  AddHead					( OBJECT * pObj) { return AddHeadI(pObj); }
			D_POSITION		  AddTail					( OBJECT * pObj)  { return AddTailI(pObj); }
			BOOL			  AddTail					( D_CGenericList<OBJECT> *pList) { return D_CBaseList::AddTail((D_CBaseList *) pList); }
			BOOL			  AddHead					( D_CGenericList<OBJECT> *pList) { return D_CBaseList::AddHead((D_CBaseList *) pList); }
			BOOL			  AddAfter					( D_POSITION p, D_CGenericList<OBJECT> *pList) { return D_CBaseList::AddAfter(p, (D_CBaseList *) pList); };
			BOOL			  AddBefore					( D_POSITION p, D_CGenericList<OBJECT> *pList) { return D_CBaseList::AddBefore(p, (D_CBaseList *) pList); };
			D_POSITION		  Find						( OBJECT * pObj) const { return FindI(pObj); }
} ;


class D_CBaseObject
{
private:
	D_CBaseObject( const D_CBaseObject& objectSrc ) ;
	void operator = ( const D_CBaseObject& objectSrc ) ;

private:
	static LONG m_cObjects;

protected:
#ifdef DEBUG
	DWORD m_dwCookie;
#endif

public:
	D_CBaseObject(const TCHAR *pName);
#ifdef UNICODE
	D_CBaseObject(const char *pName);
#endif
	~D_CBaseObject();

	static LONG ObjectsActive(){ return m_cObjects; };
};



class 
#ifndef DX_GCC_COMPILE
__declspec( novtable )
#endif
 D_CUnknown : public D_INonDelegatingUnknown, public D_CBaseObject
{
private:
	const D_IUnknown * m_pUnknown ;

protected:
	volatile LONG m_cRef;

public:

	D_CUnknown(const TCHAR *pName, D_IUnknown * pUnk);
	virtual ~D_CUnknown() {};

	D_CUnknown(TCHAR *pName, D_IUnknown * pUnk,HRESULT *phr);
#ifdef UNICODE
	D_CUnknown(const char *pName, D_IUnknown * pUnk);
	D_CUnknown(char *pName, D_IUnknown * pUnk,HRESULT *phr);
#endif

	D_IUnknown * GetOwner() const { return ( D_IUnknown * )m_pUnknown; };

	HRESULT	__stdcall NonDelegatingQueryInterface( REFIID, void ** ) ;
	ULONG	__stdcall NonDelegatingAddRef() ;
	ULONG	__stdcall NonDelegatingRelease() ;
} ;




class D_CCritSec
{
	D_CCritSec(const D_CCritSec &refCritSec);
	D_CCritSec &operator=(const D_CCritSec &refCritSec);

	CRITICAL_SECTION m_CritSec;

#ifdef DEBUG
public:
	DWORD   m_currentOwner;
	DWORD   m_lockCount;
	BOOL    m_fTrace;        // Trace this one
public:
	D_CCritSec();
	~D_CCritSec();
	void Lock();
	void Unlock();
#else
public:
#ifndef DX_NON_NAMESPACE
	D_CCritSec() { SETUP_WIN_API DxLib::WinAPIData.Win32Func.InitializeCriticalSectionFunc( &m_CritSec ) ; } ;
	~D_CCritSec(){ SETUP_WIN_API DxLib::WinAPIData.Win32Func.DeleteCriticalSectionFunc( &m_CritSec ) ; } ;
	void Lock()  { SETUP_WIN_API DxLib::WinAPIData.Win32Func.EnterCriticalSectionFunc( &m_CritSec ) ; } ;
	void Unlock(){ SETUP_WIN_API DxLib::WinAPIData.Win32Func.LeaveCriticalSectionFunc( &m_CritSec ) ; } ;
#else // DX_NON_NAMESPACE
	D_CCritSec() { SETUP_WIN_API WinAPIData.Win32Func.InitializeCriticalSectionFunc( &m_CritSec ) ; } ;
	~D_CCritSec(){ SETUP_WIN_API WinAPIData.Win32Func.DeleteCriticalSectionFunc( &m_CritSec ) ; } ;
	void Lock()  { SETUP_WIN_API WinAPIData.Win32Func.EnterCriticalSectionFunc( &m_CritSec ) ; } ;
	void Unlock(){ SETUP_WIN_API WinAPIData.Win32Func.LeaveCriticalSectionFunc( &m_CritSec ) ; } ;
#endif // DX_NON_NAMESPACE

#endif
};

class D_CAutoLock
{
	D_CAutoLock(const D_CAutoLock &refAutoLock);
	D_CAutoLock &operator=(const D_CAutoLock &refAutoLock);

protected:
	D_CCritSec * m_pLock;

public:
	D_CAutoLock(D_CCritSec * plock)
	{
		m_pLock = plock;
		m_pLock->Lock();
	};

	~D_CAutoLock() {
		m_pLock->Unlock();
	};
};



const LONGLONG MILLISECONDS = (1000);            // 10 ^ 3
const LONGLONG NANOSECONDS = (1000000000);       // 10 ^ 9
const LONGLONG UNITS = (NANOSECONDS / 100);      // 10 ^ 7

#define MILLISECONDS_TO_100NS_UNITS(lMs) Int32x32To64((lMs), (UNITS / MILLISECONDS))

class D_CRefTime
{
public:
	D_REFERENCE_TIME							m_time;

	inline D_CRefTime(){ m_time = 0; };
	inline D_CRefTime(LONG msecs){ m_time = MILLISECONDS_TO_100NS_UNITS(msecs); };
	inline D_CRefTime(D_REFERENCE_TIME rt){ m_time = rt; };
	inline operator D_REFERENCE_TIME() const { return m_time; };
	inline D_CRefTime& operator=(const D_CRefTime& rt){ m_time = rt.m_time; return *this; };
	inline D_CRefTime& operator=(const LONGLONG ll){ m_time = ll; return *this; };
	inline D_CRefTime& operator+=(const D_CRefTime& rt){ return (*this = *this + rt); };
	inline D_CRefTime& operator-=(const D_CRefTime& rt){ return (*this = *this - rt); };
	inline LONG Millisecs(void){ return (LONG)(m_time / (UNITS / MILLISECONDS)); };
	inline LONGLONG GetUnits(void){ return m_time; };
};

class D_CMediaType : public D_AM_MEDIA_TYPE
{
public:
	~D_CMediaType();
	D_CMediaType();
	D_CMediaType(const GUID * majortype);
	D_CMediaType(const D_AM_MEDIA_TYPE&, HRESULT* phr = NULL);
	D_CMediaType(const D_CMediaType&, HRESULT* phr = NULL);

	D_CMediaType& operator=(const D_CMediaType&);
	D_CMediaType& operator=(const D_AM_MEDIA_TYPE&);
	BOOL operator == (const D_CMediaType&) const;
	BOOL operator != (const D_CMediaType&) const;
			HRESULT			  Set						(const D_CMediaType& rt);
			HRESULT			  Set						(const D_AM_MEDIA_TYPE& rt);
			BOOL			  IsValid					() const;
			const GUID		  *Type						() const { return &majortype;} ;
			void			  SetType					(const GUID *);
			const GUID		  *Subtype					() const { return &subtype;} ;
			void			  SetSubtype				(const GUID *);
			BOOL			  IsFixedSize				() const {return bFixedSizeSamples; };
			BOOL			  IsTemporalCompressed		() const {return bTemporalCompression; };
			ULONG			  GetSampleSize				() const;
			void			  SetSampleSize				(ULONG sz);
			void			  SetVariableSize			();
			void			  SetTemporalCompression	(BOOL bCompressed);
			BYTE			  *Format					() const {return pbFormat; };
			ULONG			  FormatLength				() const { return cbFormat; };
			void			  SetFormatType				(const GUID *);
			const GUID		  *FormatType				() const {return &formattype; };
			BOOL			  SetFormat					(BYTE *pFormat, ULONG length);
			void			  ResetFormatBuffer			();
			BYTE			  *AllocFormatBuffer		(ULONG length);
			BYTE			  *ReallocFormatBuffer		(ULONG length);
			void			  InitMediaType				();
			BOOL			  MatchesPartial			(const D_CMediaType* ppartial) const;
			BOOL			  IsPartiallySpecified		(void) const;
};

class 
#ifndef DX_GCC_COMPILE
__declspec( novtable )
#endif
 D_CBaseFilter : public D_CUnknown, public D_IBaseFilter, public D_IAMovieSetup
{
friend class D_CBasePin;

protected:
	D_FILTER_STATE								m_State;
	D_IReferenceClock							*m_pClock;
	D_CRefTime									m_tStart;
	CLSID										m_clsid;
	D_CCritSec									*m_pLock;

	WCHAR										*m_pName;
	D_IFilterGraph								*m_pGraph;
	D_IMediaEventSink							*m_pSink;
	LONG										m_PinVersion;

public:

	D_CBaseFilter( const TCHAR *pName, D_IUnknown * pUnk, D_CCritSec  *pLock, REFCLSID   clsid);
	D_CBaseFilter( const TCHAR *pName, D_IUnknown * pUnk, D_CCritSec  *pLock, REFCLSID   clsid, HRESULT   *phr);
#ifdef UNICODE
	D_CBaseFilter( const CHAR *pName, D_IUnknown * pUnk, D_CCritSec  *pLock, REFCLSID   clsid);
	D_CBaseFilter( CHAR       *pName, D_IUnknown * pUnk, D_CCritSec  *pLock, REFCLSID   clsid, HRESULT   *phr);
#endif
	virtual ~D_CBaseFilter();
			HRESULT	__stdcall QueryInterface			( REFIID riid, void **ppv ){ return GetOwner()->QueryInterface( riid,ppv ) ; } ;
			ULONG	__stdcall AddRef					() { return GetOwner()->AddRef();  };
			ULONG	__stdcall Release					(){ return GetOwner()->Release(); };

			HRESULT	__stdcall NonDelegatingQueryInterface( REFIID riid, void ** ppv ) ;
#ifdef DEBUG
			ULONG	__stdcall NonDelegatingRelease		();
#endif

	// --- IPersist method ---
			HRESULT	__stdcall GetClassID				(CLSID *pClsID);

	// --- IMediaFilter methods ---
			HRESULT	__stdcall GetState					( DWORD dwMSecs, D_FILTER_STATE *State ) ;
			HRESULT	__stdcall SetSyncSource				( D_IReferenceClock *pClock ) ;
			HRESULT	__stdcall GetSyncSource				( D_IReferenceClock **pClock ) ;
			HRESULT	__stdcall Stop						();
			HRESULT	__stdcall Pause						();
			HRESULT	__stdcall Run						( D_REFERENCE_TIME tStart ) ;

	// --- helper methods ---
	virtual HRESULT			  StreamTime				(D_CRefTime& rtStream);
			BOOL			  IsActive					(){ D_CAutoLock cObjectLock(m_pLock); return ((m_State == D_State_Paused) || (m_State == D_State_Running)); };
			BOOL			  IsStopped					(){ return (m_State == D_State_Stopped); };

	// --- IBaseFilter methods ---
			HRESULT	__stdcall EnumPins					( D_IEnumPins ** ppEnum);
			HRESULT	__stdcall FindPin					( LPCWSTR Id, D_IPin ** ppPin );
			HRESULT	__stdcall QueryFilterInfo			( D_FILTER_INFO * pInfo);
			HRESULT	__stdcall JoinFilterGraph			( D_IFilterGraph * pGraph, LPCWSTR pName);
			HRESULT	__stdcall QueryVendorInfo			( LPWSTR* pVendorInfo );

	// --- helper methods ---
			HRESULT			  NotifyEvent				( long EventCode, LONG_PTR EventParam1, LONG_PTR EventParam2);
			D_IFilterGraph	  *GetFilterGraph			() { return m_pGraph; }
			HRESULT			  ReconnectPin				( D_IPin *pPin, D_AM_MEDIA_TYPE const *pmt ) ;
	virtual LONG			  GetPinVersion				();
			void			  IncrementPinVersion		();
	virtual int				  GetPinCount				() = 0 ;
	virtual class D_CBasePin  *GetPin					(int n) = 0 ;

	// --- IAMovieSetup methods ---
			HRESULT	__stdcall Register					();
			HRESULT	__stdcall Unregister				();

	// --- setup helper methods ---
	virtual D_LPAMOVIESETUP_FILTER GetSetupData			(){ return NULL; }
};

class
#ifndef DX_GCC_COMPILE
 __declspec( novtable )
#endif
  D_CBasePin : public D_CUnknown, public D_IPin, public D_IQualityControl
{
protected:
	WCHAR										*m_pName;
	D_IPin										*m_Connected;
	D_PIN_DIRECTION								m_dir;
	D_CCritSec									*m_pLock;
	bool										m_bRunTimeError;
	bool										m_bCanReconnectWhenActive;
	bool										m_bTryMyTypesFirst;
	class D_CBaseFilter							*m_pFilter;
	D_IQualityControl							*m_pQSink;
	LONG										m_TypeVersion;
	D_CMediaType								m_mt;
	D_CRefTime									m_tStart;
	D_CRefTime									m_tStop;
	double										m_dRate;
#ifdef DEBUG
	LONG										m_cRef;
#endif

	// displays pin connection information
#ifdef DEBUG
			void DisplayPinInfo							( D_IPin *pReceivePin);
			void DisplayTypeInfo						( D_IPin *pPin, const D_CMediaType *pmt);
#else
			void DisplayPinInfo							( D_IPin * /*pReceivePin*/) {};
			void DisplayTypeInfo						( D_IPin * /*pPin*/, const D_CMediaType * /*pmt*/) {};
#endif

			HRESULT AttemptConnection					( D_IPin* pReceivePin, const D_CMediaType* pmt );
			HRESULT TryMediaTypes						( D_IPin *pReceivePin, const D_CMediaType *pmt, D_IEnumMediaTypes *pEnum) ;
			HRESULT AgreeMediaType						( D_IPin *pReceivePin, const D_CMediaType *pmt);

public:
	D_CBasePin( TCHAR *pObjectName, D_CBaseFilter *pFilter, D_CCritSec *pLock, HRESULT *phr, LPCWSTR pName, D_PIN_DIRECTION dir ) ;
#ifdef UNICODE
	D_CBasePin( CHAR *pObjectName, D_CBaseFilter *pFilter, D_CCritSec *pLock, HRESULT *phr, LPCWSTR pName, D_PIN_DIRECTION dir ) ;
#endif
	virtual ~D_CBasePin();
			HRESULT __stdcall QueryInterface			( REFIID riid, void **ppv ){ return GetOwner()->QueryInterface(riid,ppv); };
			ULONG   __stdcall AddRef					(){ return GetOwner()->AddRef(); }; 
			ULONG   __stdcall Release					(){ return GetOwner()->Release(); };

			HRESULT __stdcall NonDelegatingQueryInterface( REFIID riid, void ** ppv);
			ULONG   __stdcall NonDelegatingRelease		();
			ULONG   __stdcall NonDelegatingAddRef		();

	// --- IPin methods ---
			HRESULT __stdcall Connect					( D_IPin * pReceivePin, const D_AM_MEDIA_TYPE *pmt );
			HRESULT __stdcall ReceiveConnection			( D_IPin * pConnector, const D_AM_MEDIA_TYPE *pmt );
			HRESULT __stdcall Disconnect				();
			HRESULT __stdcall ConnectedTo				( D_IPin **pPin);
			HRESULT __stdcall ConnectionMediaType		( D_AM_MEDIA_TYPE *pmt);
			HRESULT __stdcall QueryPinInfo				( D_PIN_INFO * pInfo );
			HRESULT __stdcall QueryDirection			( D_PIN_DIRECTION * pPinDir );
			HRESULT __stdcall QueryId					( LPWSTR * Id );
			HRESULT __stdcall QueryAccept				( const D_AM_MEDIA_TYPE *pmt );
			HRESULT __stdcall EnumMediaTypes			( D_IEnumMediaTypes **ppEnum );
			HRESULT __stdcall QueryInternalConnections	( D_IPin* * /*apPin*/, ULONG * /*nPin*/ ){ return E_NOTIMPL; }
			HRESULT __stdcall EndOfStream				( void);
			HRESULT __stdcall NewSegment				( D_REFERENCE_TIME tStart, D_REFERENCE_TIME tStop, double dRate ) ;

	// IQualityControl methods
			HRESULT __stdcall Notify					( D_IBaseFilter * pSender, D_Quality q  ) ;
			HRESULT __stdcall SetSink					( D_IQualityControl * piqc ) ;

	// --- helper methods ---
			BOOL			  IsConnected				( void) {return (m_Connected != NULL); };
			D_IPin			  *GetConnected				() { return m_Connected; };
			BOOL			  IsStopped					(){ return (m_pFilter->m_State == D_State_Stopped ) ; };
	virtual LONG			  GetMediaTypeVersion		();
			void			  IncrementTypeVersion		();
	virtual HRESULT			  Active					( void);
	virtual HRESULT			  Inactive					( void);
	virtual HRESULT			  Run						( D_REFERENCE_TIME tStart);
	virtual HRESULT			  CheckMediaType			( const D_CMediaType *) = 0 ;
	virtual HRESULT			  SetMediaType				( const D_CMediaType *);
	virtual HRESULT			  CheckConnect				( D_IPin *);
	virtual HRESULT			  BreakConnect				();
	virtual HRESULT			  CompleteConnect			( D_IPin *pReceivePin);
	virtual HRESULT			  GetMediaType				( int iPosition,D_CMediaType *pMediaType);
			D_REFERENCE_TIME  CurrentStopTime			() { return m_tStop; }
			D_REFERENCE_TIME  CurrentStartTime			() { return m_tStart; }
			double			  CurrentRate				() { return m_dRate; }
			LPWSTR			  Name						() { return m_pName; };
			void			  SetReconnectWhenActive	( bool bCanReconnect){ m_bCanReconnectWhenActive = bCanReconnect; }
			bool			  CanReconnectWhenActive	(){ return m_bCanReconnectWhenActive; }

protected:
	virtual HRESULT __stdcall DisconnectInternal		();
};

class D_CEnumPins : public D_IEnumPins
{
	typedef D_CGenericList<D_CBasePin> CPinList;
	int											m_Position;
	int											m_PinCount;
	class D_CBaseFilter							*m_pFilter;
	LONG										m_Version;
	LONG										m_cRef;
	CPinList									m_PinCache;

#ifdef DEBUG
	DWORD m_dwCookie;
#endif
			BOOL			  AreWeOutOfSync			() { return (m_pFilter->GetPinVersion() == m_Version ? FALSE : TRUE); };
			HRESULT __stdcall Refresh					();

public:
	D_CEnumPins( class D_CBaseFilter *pFilter, D_CEnumPins *pEnumPins);
	virtual ~D_CEnumPins();

	// D_IUnknown
			HRESULT __stdcall QueryInterface			(REFIID riid, void **ppv);
			ULONG   __stdcall AddRef					();
			ULONG   __stdcall Release					();

	// D_IEnumPins
			HRESULT __stdcall Next						( ULONG cPins, D_IPin ** ppPins, ULONG * pcFetched ) ;
			HRESULT __stdcall Skip						( ULONG cPins ) ;
			HRESULT __stdcall Reset						() ;
			HRESULT __stdcall Clone						( D_IEnumPins **ppEnum ) ;
};

class
#ifndef DX_GCC_COMPILE
__declspec( novtable )
#endif
D_CBaseInputPin : public D_CBasePin,  public D_IMemInputPin
{
protected:
	D_IMemAllocator								*m_pAllocator;
	BYTE										m_bReadOnly;
	BYTE										m_bFlushing;
	D_AM_SAMPLE2_PROPERTIES						m_SampleProps;

public:
	D_CBaseInputPin( TCHAR *pObjectName, D_CBaseFilter *pFilter, D_CCritSec *pLock,	HRESULT *phr, LPCWSTR pName);
#ifdef UNICODE
	D_CBaseInputPin( CHAR *pObjectName, D_CBaseFilter *pFilter, D_CCritSec *pLock, HRESULT *phr, LPCWSTR pName);
#endif
	virtual ~D_CBaseInputPin();
			HRESULT	__stdcall QueryInterface			( REFIID riid, void **ppv ){ return GetOwner()->QueryInterface( riid,ppv ) ; } ;
			ULONG	__stdcall AddRef					() { return GetOwner()->AddRef();  };
			ULONG	__stdcall Release					(){ return GetOwner()->Release(); };

			HRESULT	__stdcall NonDelegatingQueryInterface(REFIID riid, void **ppv);
			HRESULT	__stdcall GetAllocator				( D_IMemAllocator ** ppAllocator);
			HRESULT	__stdcall NotifyAllocator			( D_IMemAllocator * pAllocator, BOOL bReadOnly);
			HRESULT	__stdcall Receive					( D_IMediaSample *pSample);
			HRESULT	__stdcall ReceiveMultiple			( D_IMediaSample **pSamples, long nSamples, long *nSamplesProcessed);
			HRESULT	__stdcall ReceiveCanBlock			();
			HRESULT	__stdcall BeginFlush				( void);
			HRESULT	__stdcall EndFlush					( void);
			HRESULT	__stdcall GetAllocatorRequirements	( D_ALLOCATOR_PROPERTIES *pProps);
			HRESULT			  BreakConnect				();
			BOOL			  IsReadOnly				(){ return m_bReadOnly; };
			BOOL			  IsFlushing				(){ return m_bFlushing; };
	virtual HRESULT			  CheckStreaming			();
			HRESULT			  PassNotify				( D_Quality& q);
			HRESULT	__stdcall Notify					( D_IBaseFilter * pSender, D_Quality q);
	virtual HRESULT			  Inactive					( void);
	D_AM_SAMPLE2_PROPERTIES * SampleProps				() { /*ASSERT(m_SampleProps.cbData != 0);*/ return &m_SampleProps; }
};

class D_CRendererInputPin : public D_CBaseInputPin
{
protected:
	class D_CBaseRenderer						*m_pRenderer;

public:
	D_CRendererInputPin( D_CBaseRenderer *pRenderer, HRESULT *phr, LPCWSTR Name );

	// Overriden from the base pin classes
			HRESULT			  BreakConnect				() ;
			HRESULT			  CompleteConnect			( D_IPin *pReceivePin ) ;
			HRESULT			  SetMediaType				( const D_CMediaType *pmt ) ;
			HRESULT			  CheckMediaType			( const D_CMediaType *pmt ) ;
			HRESULT			  Active					() ;
			HRESULT			  Inactive					() ;

	// Add rendering behaviour to interface functions
			HRESULT	__stdcall QueryId(LPWSTR *Id);
			HRESULT	__stdcall EndOfStream();
			HRESULT	__stdcall BeginFlush();
			HRESULT	__stdcall EndFlush();
			HRESULT	__stdcall Receive(D_IMediaSample *pMediaSample);

	// Helper
	D_IMemAllocator inline		*Allocator				() const { return m_pAllocator; }
};

class D_CBaseDispatch
{
	ITypeInfo									*m_pti;

public:

	D_CBaseDispatch() : m_pti(NULL) {}
	~D_CBaseDispatch();

	/* IDispatch methods */
			HRESULT	__stdcall GetTypeInfoCount			( UINT * pctinfo ) ;
			HRESULT	__stdcall GetTypeInfo				( REFIID riid, UINT itinfo, LCID lcid, ITypeInfo ** pptinfo ) ;
			HRESULT	__stdcall GetIDsOfNames				( REFIID riid, OLECHAR  ** rgszNames, UINT cNames, LCID lcid, DISPID * rgdispid ) ;
};

class
#ifndef DX_GCC_COMPILE
__declspec( novtable )
#endif
D_CMediaPosition : public D_IMediaPosition, public D_CUnknown
{
	D_CBaseDispatch								m_basedisp;

public:
	D_CMediaPosition( const TCHAR *, D_IUnknown *);
	D_CMediaPosition( const TCHAR *, D_IUnknown *, HRESULT *phr);

			HRESULT	__stdcall QueryInterface			( REFIID riid, void **ppv ){ return GetOwner()->QueryInterface( riid,ppv ) ; } ;
			ULONG	__stdcall AddRef					() { return GetOwner()->AddRef();  };
			ULONG	__stdcall Release					(){ return GetOwner()->Release(); };

	// override this to publicise our interfaces
			HRESULT	__stdcall NonDelegatingQueryInterface(REFIID riid, void **ppv);

	/* IDispatch methods */
			HRESULT	__stdcall GetTypeInfoCount			( UINT * pctinfo);
			HRESULT	__stdcall GetTypeInfo				( UINT itinfo, LCID lcid, ITypeInfo ** pptinfo);
			HRESULT	__stdcall GetIDsOfNames				( REFIID riid, OLECHAR  ** rgszNames, UINT cNames, LCID lcid, DISPID * rgdispid);
			HRESULT	__stdcall Invoke					( DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS * pdispparams, VARIANT * pvarResult, EXCEPINFO * pexcepinfo, UINT * puArgErr );
};

class D_CPosPassThru : public D_IMediaSeeking, public D_CMediaPosition
{
	D_IPin										*m_pPin;

			HRESULT			  GetPeer					( D_IMediaPosition **ppMP ) ;
			HRESULT			  GetPeerSeeking			( D_IMediaSeeking **ppMS ) ;
public:
	D_CPosPassThru(const TCHAR *, D_IUnknown *, HRESULT*, D_IPin *);

			HRESULT	__stdcall QueryInterface			( REFIID riid, void **ppv ){ return GetOwner()->QueryInterface( riid,ppv ) ; } ;
			ULONG	__stdcall AddRef					() { return GetOwner()->AddRef();  };
			ULONG	__stdcall Release					(){ return GetOwner()->Release(); };
			HRESULT			  ForceRefresh				(){ return S_OK; };

	// override to return an accurate current position
	virtual HRESULT			  GetMediaTime				( LONGLONG * /*pStartTime*/,LONGLONG * /*pEndTime*/) { return E_FAIL; }
			HRESULT	__stdcall NonDelegatingQueryInterface( REFIID riid,void **ppv);

	// IMediaSeeking methods
			HRESULT	__stdcall GetCapabilities			( DWORD * pCapabilities );
			HRESULT	__stdcall CheckCapabilities			( DWORD * pCapabilities );
			HRESULT	__stdcall SetTimeFormat				( const GUID * pFormat);
			HRESULT	__stdcall GetTimeFormat				( GUID *pFormat);
			HRESULT	__stdcall IsUsingTimeFormat			( const GUID * pFormat);
			HRESULT	__stdcall IsFormatSupported			( const GUID * pFormat);
			HRESULT	__stdcall QueryPreferredFormat		( GUID *pFormat);
			HRESULT	__stdcall ConvertTimeFormat			( LONGLONG * pTarget, const GUID * pTargetFormat, LONGLONG    Source, const GUID * pSourceFormat ); 
			HRESULT	__stdcall SetPositions				( LONGLONG * pCurrent, DWORD CurrentFlags, LONGLONG * pStop, DWORD StopFlags );
			HRESULT	__stdcall GetPositions				( LONGLONG * pCurrent, LONGLONG * pStop );
			HRESULT	__stdcall GetCurrentPosition		( LONGLONG * pCurrent );
			HRESULT	__stdcall GetStopPosition			( LONGLONG * pStop );
			HRESULT	__stdcall SetRate					( double dRate);
			HRESULT	__stdcall GetRate					( double * pdRate);
			HRESULT	__stdcall GetDuration				( LONGLONG *pDuration);
			HRESULT	__stdcall GetAvailable				( LONGLONG *pEarliest, LONGLONG *pLatest );
			HRESULT	__stdcall GetPreroll				( LONGLONG *pllPreroll );

	// D_IMediaPosition properties
			HRESULT	__stdcall get_Duration				( D_REFTIME * plength);
			HRESULT	__stdcall put_CurrentPosition		( D_REFTIME llTime);
			HRESULT	__stdcall get_StopTime				( D_REFTIME * pllTime);
			HRESULT	__stdcall put_StopTime				( D_REFTIME llTime);
			HRESULT	__stdcall get_PrerollTime			( D_REFTIME * pllTime);
			HRESULT	__stdcall put_PrerollTime			( D_REFTIME llTime);
			HRESULT	__stdcall get_Rate					( double * pdRate);
			HRESULT	__stdcall put_Rate					( double dRate);
			HRESULT	__stdcall get_CurrentPosition		( D_REFTIME * pllTime);
			HRESULT	__stdcall CanSeekForward			( LONG *pCanSeekForward);
			HRESULT	__stdcall CanSeekBackward			( LONG *pCanSeekBackward ) ;

private:
			HRESULT			  GetSeekingLongLong		( HRESULT (__stdcall D_IMediaSeeking::*pMethod)( LONGLONG * ), LONGLONG * pll );
};

class D_CRendererPosPassThru : public D_CPosPassThru
{
	D_CCritSec									m_PositionLock;
	LONGLONG									m_StartMedia;
	LONGLONG									m_EndMedia;
	BOOL										m_bReset;

public:
	D_CRendererPosPassThru( const TCHAR *, D_IUnknown *, HRESULT*, D_IPin * ) ;
			HRESULT			  RegisterMediaTime			( D_IMediaSample *pMediaSample ) ;
			HRESULT			  RegisterMediaTime			( LONGLONG StartTime,LONGLONG EndTime ) ;
			HRESULT			  GetMediaTime				( LONGLONG *pStartTime,LONGLONG *pEndTime ) ;
			HRESULT			  ResetMediaTime			() ;
			HRESULT			  EOS						() ;
};

class D_CAMEvent
{
	D_CAMEvent(const D_CAMEvent &refEvent);
	D_CAMEvent &operator=(const D_CAMEvent &refEvent);
protected:
	HANDLE										m_hEvent;
public:
	D_CAMEvent(BOOL fManualReset = FALSE);
	~D_CAMEvent();

	// Cast to HANDLE - we don't support this as an lvalue
	operator HANDLE										() const { return m_hEvent; };
#ifndef DX_NON_NAMESPACE
			void			  Set						(){ SETUP_WIN_API  /*EXECUTE_ASSERT*/(DxLib::WinAPIData.Win32Func.SetEventFunc(m_hEvent));};
			BOOL			  Wait						( DWORD dwTimeout = INFINITE) { SETUP_WIN_API return (DxLib::WinAPIData.Win32Func.WaitForSingleObjectFunc(m_hEvent, dwTimeout) == WAIT_OBJECT_0); };
			void			  Reset						(){ SETUP_WIN_API DxLib::WinAPIData.Win32Func.ResetEventFunc(m_hEvent); };
#else DX_NON_NAMESPACE
			void			  Set						(){ SETUP_WIN_API  /*EXECUTE_ASSERT*/(WinAPIData.Win32Func.SetEventFunc(m_hEvent));};
			BOOL			  Wait						( DWORD dwTimeout = INFINITE) { SETUP_WIN_API return (WinAPIData.Win32Func.WaitForSingleObjectFunc(m_hEvent, dwTimeout) == WAIT_OBJECT_0); };
			void			  Reset						(){ SETUP_WIN_API WinAPIData.Win32Func.ResetEventFunc(m_hEvent); };
#endif // DX_NON_NAMESPACE
			BOOL			  Check						(){ return Wait(0); };
};

class D_CBaseRenderer : public D_CBaseFilter
{
protected:
	friend class D_CRendererInputPin;

	friend void CALLBACK EndOfStreamTimer(UINT uID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);

	D_CRendererPosPassThru						*m_pPosition;
	D_CAMEvent									m_RenderEvent;
	D_CAMEvent									m_ThreadSignal;
	D_CAMEvent									m_evComplete;
	BOOL										m_bAbort;
	BOOL										m_bStreaming;
	DWORD_PTR									m_dwAdvise;
	D_IMediaSample								*m_pMediaSample;
	BOOL										m_bEOS;
	BOOL										m_bEOSDelivered;
	D_CRendererInputPin							*m_pInputPin;
	D_CCritSec									m_InterfaceLock;
	D_CCritSec									m_RendererLock;
	D_IQualityControl							*m_pQSink;
	BOOL										m_bRepaintStatus;
	volatile BOOL								m_bInReceive;
	D_REFERENCE_TIME							m_SignalTime;
	UINT										m_EndOfStreamTimer;
	D_CCritSec									m_ObjectCreationLock;

public:
	D_CBaseRenderer( REFCLSID RenderClass, TCHAR *pName, D_IUnknown * pUnk, HRESULT *phr ) ;
	virtual ~D_CBaseRenderer();

	virtual HRESULT			  GetMediaPositionInterface	( REFIID riid, void **ppv ) ;
			HRESULT	__stdcall NonDelegatingQueryInterface( REFIID, void ** ) ;
	virtual HRESULT			  SourceThreadCanWait		( BOOL bCanWait ) ;

#ifdef DEBUG
	// Debug only dump of the renderer state
			void			  DisplayRendererState		() ;
#endif
	virtual HRESULT			  WaitForRenderTime			() ;
	virtual HRESULT			  CompleteStateChange		( D_FILTER_STATE OldState ) ;

	// Return internal information about this filter
			BOOL			  IsEndOfStream				(){ return m_bEOS; };
			BOOL			  IsEndOfStreamDelivered	(){ return m_bEOSDelivered; };
			BOOL			  IsStreaming				(){ return m_bStreaming; };
			void			  SetAbortSignal			( BOOL bAbort) { m_bAbort = bAbort; };
	virtual void			  OnReceiveFirstSample		( D_IMediaSample * /*pMediaSample*/){ };
			D_CAMEvent		  *GetRenderEvent			(){ return &m_RenderEvent ; } ;

	// Permit access to the transition state
			void			  Ready						(){ m_evComplete.Set() ; } ;
			void			  NotReady					(){ m_evComplete.Reset() ; } ;
			BOOL			  CheckReady				(){ return m_evComplete.Check() ; } ;
	virtual int				  GetPinCount				() ;
	virtual D_CBasePin		  *GetPin					( int n ) ;
			D_FILTER_STATE	  GetRealState				() ;
			void			  SendRepaint				() ;
			void			  SendNotifyWindow			( D_IPin *pPin, HWND hwnd ) ;
			BOOL			  OnDisplayChange			() ;
			void			  SetRepaintStatus			( BOOL bRepaint ) ;

	// Override the filter and pin interface functions
			HRESULT	__stdcall Stop						() ;
			HRESULT	__stdcall Pause						() ;
			HRESULT	__stdcall Run						( D_REFERENCE_TIME StartTime ) ;
			HRESULT	__stdcall GetState					( DWORD dwMSecs, D_FILTER_STATE *State ) ;
			HRESULT	__stdcall FindPin					( LPCWSTR Id, D_IPin **ppPin ) ;

	// These are available for a quality management implementation
	virtual void			  OnRenderStart				( D_IMediaSample *pMediaSample ) ;
	virtual void			  OnRenderEnd				( D_IMediaSample *pMediaSample ) ;
	virtual HRESULT			  OnStartStreaming			(){ return NOERROR; } ;
	virtual HRESULT			  OnStopStreaming			(){ return NOERROR; } ;
	virtual void			  OnWaitStart				(){} ;
	virtual void			  OnWaitEnd					(){} ;
	virtual void			  PrepareRender				(){} ;

#ifdef PERF
	D_REFERENCE_TIME							m_trRenderStart ;
	int											m_idBaseStamp;
	int											m_idBaseRenderTime;
	int											m_idBaseAccuracy;
#endif

	// Quality management implementation for scheduling rendering
	virtual BOOL			  ScheduleSample			( D_IMediaSample *pMediaSample ) ;
	virtual HRESULT			  GetSampleTimes			( D_IMediaSample *pMediaSample, D_REFERENCE_TIME *pStartTime, D_REFERENCE_TIME *pEndTime ) ;
	virtual HRESULT			  ShouldDrawSampleNow		( D_IMediaSample *pMediaSample, D_REFERENCE_TIME *ptrStart, D_REFERENCE_TIME *ptrEnd ) ;

	// Lots of end of stream complexities
			void			  TimerCallback				() ;
			void			  ResetEndOfStreamTimer		() ;
			HRESULT			  NotifyEndOfStream			() ;
	virtual HRESULT			  SendEndOfStream			() ;
	virtual HRESULT			  ResetEndOfStream			() ;
	virtual HRESULT			  EndOfStream				() ;

	// Rendering is based around the clock
			void			  SignalTimerFired			() ;
	virtual HRESULT			  CancelNotification		() ;
	virtual HRESULT			  ClearPendingSample		() ;

	// Called when the filter changes state
	virtual HRESULT			  Active					() ;
	virtual HRESULT			  Inactive					() ;
	virtual HRESULT			  StartStreaming			() ;
	virtual HRESULT			  StopStreaming				() ;
	virtual HRESULT			  BeginFlush				() ;
	virtual HRESULT			  EndFlush					() ;

	// Deal with connections and type changes
	virtual HRESULT			  BreakConnect				() ;
	virtual HRESULT			  SetMediaType				( const D_CMediaType *pmt ) ;
	virtual HRESULT			  CompleteConnect			( D_IPin *pReceivePin ) ;

	// These look after the handling of data samples
	virtual HRESULT			  PrepareReceive			( D_IMediaSample *pMediaSample ) ;
	virtual HRESULT			  Receive					( D_IMediaSample *pMediaSample ) ;
	virtual BOOL			  HaveCurrentSample			() ;
	virtual D_IMediaSample	  *GetCurrentSample			() ;
	virtual HRESULT			  Render					( D_IMediaSample *pMediaSample ) ;

	// Derived classes MUST override these
	virtual HRESULT			  DoRenderSample			( D_IMediaSample *pMediaSample ) = 0 ;
	virtual HRESULT			  CheckMediaType			( const D_CMediaType * ) = 0 ;

	// Helper
			void			  WaitForReceiveToComplete();
};

class D_CBaseVideoRenderer : public D_CBaseRenderer, public D_IQualProp, public D_IQualityControl
{
protected:
	int											m_nNormal;
#ifdef PERF
	BOOL										m_bDrawLateFrames;
#endif
	BOOL										m_bSupplierHandlingQuality;
	int											m_trThrottle;
	int											m_trRenderAvg;
	int											m_trRenderLast;
	int											m_tRenderStart;
	int											m_trEarliness;
	int											m_trTarget;
	int											m_trWaitAvg;
	int											m_trFrameAvg;
	int											m_trDuration;

#ifdef PERF
	int											m_idTimeStamp;
	int											m_idEarliness;
	int											m_idTarget;
	int											m_idWaitReal;
	int											m_idWait;
	int											m_idFrameAccuracy;
	int											m_idRenderAvg;
	int											m_idSchLateTime;
	int											m_idQualityRate;
	int											m_idQualityTime;
	int											m_idDecision;
	int											m_idDuration;
	int											m_idThrottle;
#endif // PERF
	D_REFERENCE_TIME							m_trRememberStampForPerf;
#ifdef PERF
	D_REFERENCE_TIME							m_trRememberFrameForPerf;

	// debug...
	int											m_idFrameAvg;
	int											m_idWaitAvg;
#endif
	int											m_cFramesDropped;
	int											m_cFramesDrawn;
	LONGLONG									m_iTotAcc;
	LONGLONG									m_iSumSqAcc;
	D_REFERENCE_TIME							m_trLastDraw;
	LONGLONG									m_iSumSqFrameTime;
	LONGLONG									m_iSumFrameTime;
	int											m_trLate;
	int											m_trFrame;
	int											m_tStreamingStart;
#ifdef PERF
	LONGLONG									m_llTimeOffset;
#endif

public:
	D_CBaseVideoRenderer( REFCLSID RenderClass, TCHAR *pName, D_IUnknown * pUnk, HRESULT *phr ) ;
	virtual ~D_CBaseVideoRenderer();

	// IQualityControl methods - Notify allows audio-video throttling
	virtual HRESULT __stdcall SetSink					( D_IQualityControl * piqc ) ;
	virtual HRESULT __stdcall Notify					( D_IBaseFilter * pSelf, D_Quality q ) ;

	// These provide a full video quality management implementation
			void			  OnRenderStart				( D_IMediaSample *pMediaSample ) ;
			void			  OnRenderEnd				( D_IMediaSample *pMediaSample ) ;
			void			  OnWaitStart				();
			void			  OnWaitEnd					();
			HRESULT			  OnStartStreaming			();
			HRESULT			  OnStopStreaming			();
			void			  ThrottleWait				();

	// Handle the statistics gathering for our quality management
			void			  PreparePerformanceData	( int trLate, int trFrame ) ;
	virtual void			  RecordFrameLateness		( int trLate, int trFrame ) ;
	virtual void			  OnDirectRender			( D_IMediaSample *pMediaSample ) ;
	virtual HRESULT			  ResetStreamingTimes		();
	virtual BOOL			  ScheduleSample			( D_IMediaSample *pMediaSample ) ;
	virtual HRESULT			  ShouldDrawSampleNow		( D_IMediaSample *pMediaSample, D_REFERENCE_TIME *ptrStart, D_REFERENCE_TIME *ptrEnd ) ;
	virtual HRESULT			  SendQuality				( D_REFERENCE_TIME trLate, D_REFERENCE_TIME trRealStream ) ;
	virtual HRESULT __stdcall JoinFilterGraph			( D_IFilterGraph * pGraph, LPCWSTR pName ) ;
			HRESULT			  GetStdDev					( int nSamples, int *piResult, LONGLONG llSumSq, LONGLONG iTot ) ;

public:
	// IQualProp property page support
	virtual HRESULT __stdcall get_FramesDroppedInRenderer( int *cFramesDropped ) ;
	virtual HRESULT __stdcall get_FramesDrawn			( int *pcFramesDrawn ) ;
	virtual HRESULT __stdcall get_AvgFrameRate			( int *piAvgFrameRate ) ;
	virtual HRESULT __stdcall get_Jitter				( int *piJitter ) ;
	virtual HRESULT __stdcall get_AvgSyncOffset			( int *piAvg ) ;
	virtual HRESULT __stdcall get_DevSyncOffset			( int *piDev ) ;

	// Implement an D_IUnknown interface and expose IQualProp
	virtual HRESULT	__stdcall QueryInterface			( REFIID riid, void **ppv )	{ return GetOwner()->QueryInterface( riid,ppv ) ; } ;
	virtual ULONG	__stdcall AddRef					()							{ return GetOwner()->AddRef();  } ;
	virtual ULONG	__stdcall Release					()							{ return GetOwner()->Release(); } ;
	virtual HRESULT __stdcall NonDelegatingQueryInterface( REFIID riid,VOID **ppv ) ;
};

class D_CEnumMediaTypes : public D_IEnumMediaTypes
{
	int											m_Position;
	D_CBasePin									*m_pPin;
	LONG										m_Version;
	LONG										m_cRef;
#ifdef DEBUG
	DWORD										m_dwCookie;
#endif

	BOOL AreWeOutOfSync(){ return (m_pPin->GetMediaTypeVersion() == m_Version ? FALSE : TRUE); };

public:
	D_CEnumMediaTypes( D_CBasePin *pPin, D_CEnumMediaTypes *pEnumMediaTypes ) ;
	virtual ~D_CEnumMediaTypes() ;

	// D_IUnknown
	virtual HRESULT __stdcall QueryInterface			( REFIID riid, void **ppv ) ;
	virtual ULONG   __stdcall AddRef					() ;
	virtual ULONG   __stdcall Release					() ;

	// IEnumMediaTypes
	virtual HRESULT __stdcall Next						( ULONG cMediaTypes, D_AM_MEDIA_TYPE ** ppMediaTypes, ULONG * pcFetched ) ;
	virtual HRESULT __stdcall Skip						( ULONG cMediaTypes ) ;
	virtual HRESULT __stdcall Reset						() ;
	virtual HRESULT __stdcall Clone						( D_IEnumMediaTypes **ppEnum) ;
};

class D_CMovieRender : public D_CBaseVideoRenderer
{
public:
	D_CMovieRender( D_IUnknown * pUnk, HRESULT *phr ) ;
	~D_CMovieRender();

	virtual ULONG  __stdcall Release					();

	virtual HRESULT CheckMediaType						( const D_CMediaType *pMediaType ) ;
	virtual HRESULT SetMediaType						( const D_CMediaType *pMediaType ) ;
	virtual HRESULT DoRenderSample						( D_IMediaSample *pMediaSample ) ;
	void AllocImageBuffer() ;

	int											SetMediaTypeFlag ;
	int											UseTempBaseImage ;
	int											UseTempBaseImageFirstFrame ;
	int											AlwaysBaseImage ;
	int											AlwaysBaseImageUseTempBaseImage ;
#ifndef DX_NON_NAMESPACE
	DxLib::BASEIMAGE							*TempBaseImage ;
#else // DX_NON_NAMESPACE
	BASEIMAGE									*TempBaseImage ;
#endif // DX_NON_NAMESPACE
	int											ImageType ;			// 0:RGB24   1:RGB32   2:YV12   3:NV12   4:YUY2   5:UYVY   6:YVYU   7:NV11
	RGBQUAD										Palette[ 256 ] ;
	void										*ImageBuffer ;
	void										*YImageBuffer ;
	void										*UVImageBuffer ;
	BOOL										NewImageSet ;
	BOOL										ImageReverse ;
	DWORD										Width ;
	DWORD										Height ;
	DWORD										Pitch ;
	DWORD										YWidth ;
	DWORD										YHeight ;
	DWORD										YPitch ;
	DWORD										UVWidth ;
	DWORD										UVHeight ;
	DWORD										UVPitch ;
} ;

#ifndef DX_NON_DSHOW_MP3

class D_ISeekingPassThru : public D_IUnknown
{
public:
	virtual HRESULT __stdcall Init						( BOOL bSupportRendering, D_IPin *pPin) = 0 ;
};

class D_CBaseOutputPin : public D_CBasePin
{

protected:

	D_IMemAllocator								*m_pAllocator ;
	D_IMemInputPin								*m_pInputPin ;

public:

	D_CBaseOutputPin( TCHAR *pObjectName, D_CBaseFilter *pFilter, D_CCritSec *pLock, HRESULT *phr, LPCWSTR pName);
#ifdef UNICODE
	D_CBaseOutputPin( CHAR  *pObjectName, D_CBaseFilter *pFilter, D_CCritSec *pLock, HRESULT *phr, LPCWSTR pName);
#endif
	virtual HRESULT			  CompleteConnect			( D_IPin *pReceivePin );
	virtual HRESULT			  DecideAllocator			( D_IMemInputPin * pPin, D_IMemAllocator ** pAlloc );
	virtual HRESULT			  DecideBufferSize			( D_IMemAllocator * pAlloc, D_ALLOCATOR_PROPERTIES * ppropInputRequest ) = 0 ;
	virtual HRESULT			  GetDeliveryBuffer			( D_IMediaSample ** ppSample, D_REFERENCE_TIME * pStartTime, D_REFERENCE_TIME * pEndTime, DWORD dwFlags);
	virtual HRESULT			  Deliver					( D_IMediaSample *);

	// override this to control the connection
	virtual HRESULT			  InitAllocator				( D_IMemAllocator **ppAlloc ) ;
			HRESULT			  CheckConnect				( D_IPin *pPin ) ;
			HRESULT			  BreakConnect				() ;

	// override to call Commit and Decommit
			HRESULT			  Active					() ;
			HRESULT			  Inactive					() ;

	STDMETHODIMP			  EndOfStream				() ;
	virtual HRESULT			  DeliverEndOfStream		() ;

	STDMETHODIMP			  BeginFlush				() ;
	STDMETHODIMP			  EndFlush					() ;
	virtual HRESULT			  DeliverBeginFlush			() ;
	virtual HRESULT			  DeliverEndFlush			() ;
	virtual HRESULT			  DeliverNewSegment			( D_REFERENCE_TIME tStart, D_REFERENCE_TIME tStop, double dRate ) ;
};

class D_CTransformInputPin : public D_CBaseInputPin
{
	friend class D_CTransformFilter;

protected:
	class D_CTransformFilter					*m_pTransformFilter ;

public:
	D_CTransformInputPin( TCHAR *pObjectName, class D_CTransformFilter *pTransformFilter, HRESULT * phr, LPCWSTR pName);
#ifdef UNICODE
	D_CTransformInputPin( CHAR  *pObjectName, class D_CTransformFilter *pTransformFilter, HRESULT * phr, LPCWSTR pName);
#endif

	STDMETHODIMP			  QueryId					( LPWSTR * Id ){ return D_AMGetWideString(L"In", Id); }

			HRESULT			  CheckConnect				( D_IPin *pPin );
			HRESULT			  BreakConnect				();
			HRESULT			  CompleteConnect			( D_IPin *pReceivePin );

			HRESULT			  CheckMediaType			( const D_CMediaType* mtIn ) ;
			HRESULT			  SetMediaType				( const D_CMediaType* mt ) ;

	// --- IMemInputPin -----
	STDMETHODIMP			  Receive					( D_IMediaSample * pSample ) ;
	STDMETHODIMP			  EndOfStream				() ;
	STDMETHODIMP			  BeginFlush				() ;
	STDMETHODIMP			  EndFlush					() ;
	STDMETHODIMP			  NewSegment				( D_REFERENCE_TIME tStart, D_REFERENCE_TIME tStop, double dRate ) ;

	virtual HRESULT			  CheckStreaming			() ;

	// Media type
public:
			D_CMediaType	 &CurrentMediaType			(){ return m_mt; };
};

class D_CTransformOutputPin : public D_CBaseOutputPin
{
	friend class D_CTransformFilter;

protected:
	class D_CTransformFilter					*m_pTransformFilter ;

public:
	D_IUnknown									*m_pPosition ;

	D_CTransformOutputPin( TCHAR *pObjectName, class D_CTransformFilter *pTransformFilter, HRESULT *phr, LPCWSTR pName ) ;
#ifdef UNICODE
	D_CTransformOutputPin( CHAR  *pObjectName, class D_CTransformFilter *pTransformFilter, HRESULT * phr, LPCWSTR pName ) ;
#endif
	~D_CTransformOutputPin();

	STDMETHODIMP			  NonDelegatingQueryInterface( REFIID riid, void **ppv ) ;
	STDMETHODIMP			  QueryId					( LPWSTR * Id ){ return D_AMGetWideString( L"Out", Id ) ; }

			HRESULT			  CheckConnect				( D_IPin *pPin ) ;
			HRESULT			  BreakConnect				();
			HRESULT			  CompleteConnect			( D_IPin *pReceivePin ) ;

			HRESULT			  CheckMediaType			( const D_CMediaType* mtOut ) ;
			HRESULT			  SetMediaType				( const D_CMediaType *pmt ) ;
			HRESULT			  DecideBufferSize			( D_IMemAllocator * pAlloc, D_ALLOCATOR_PROPERTIES *pProp ) ;

			HRESULT			  GetMediaType				( int iPosition, D_CMediaType *pMediaType ) ;

	STDMETHODIMP			  Notify					( D_IBaseFilter * pSender, D_Quality q ) ;

public:
			D_CMediaType	 &CurrentMediaType			(){ return m_mt; } ;
};

class
#ifndef DX_GCC_COMPILE
__declspec(novtable)
#endif
D_CTransformFilter : public D_CBaseFilter
{
public:
	virtual int				  GetPinCount				() ;
	virtual D_CBasePin		 *GetPin					( int n ) ;
	STDMETHODIMP			  FindPin					( LPCWSTR Id, D_IPin **ppPin ) ;

	STDMETHODIMP			  Stop						() ;
	STDMETHODIMP			  Pause						() ;

	D_CTransformFilter( TCHAR *, D_IUnknown *, REFCLSID clsid);
#ifdef UNICODE
	D_CTransformFilter( CHAR  *, D_IUnknown *, REFCLSID clsid);
#endif
	virtual ~D_CTransformFilter();

	virtual HRESULT			  Transform					( D_IMediaSample * pIn, D_IMediaSample *pOut ) ;

	virtual HRESULT			  CheckInputType			( const D_CMediaType* mtIn ) = 0 ;
	virtual HRESULT			  CheckTransform			( const D_CMediaType* mtIn, const D_CMediaType* mtOut ) = 0 ;
	virtual HRESULT			  DecideBufferSize			( D_IMemAllocator * pAllocator, D_ALLOCATOR_PROPERTIES *pprop ) = 0 ;

	virtual HRESULT			  GetMediaType				( int iPosition, D_CMediaType *pMediaType ) = 0 ;

	virtual HRESULT			  StartStreaming			() ;
	virtual HRESULT			  StopStreaming				() ;
	virtual HRESULT			  AlterQuality				( D_Quality q);
	virtual HRESULT			  SetMediaType				( D_PIN_DIRECTION direction, const D_CMediaType *pmt ) ;
	virtual HRESULT			  CheckConnect				( D_PIN_DIRECTION dir, D_IPin *pPin);
	virtual HRESULT			  BreakConnect				( D_PIN_DIRECTION dir ) ;
	virtual HRESULT			  CompleteConnect			( D_PIN_DIRECTION direction, D_IPin *pReceivePin ) ;
	virtual HRESULT			  Receive					( D_IMediaSample *pSample ) ;
			HRESULT			  InitializeOutputSample	( D_IMediaSample *pSample, D_IMediaSample **ppOutSample ) ;

	virtual HRESULT			  EndOfStream				() ;
	virtual HRESULT			  BeginFlush				() ;
	virtual HRESULT			  EndFlush					() ;
	virtual HRESULT			  NewSegment				( D_REFERENCE_TIME tStart, D_REFERENCE_TIME tStop, double dRate ) ;

#ifdef PERF
	virtual void			  RegisterPerfId			(){ m_idTransform = MSR_REGISTER( TEXT( "Transform" ) ) ; }
#endif // PERF

protected:

#ifdef PERF
	int											m_idTransform ;
#endif
	BOOL										m_bEOSDelivered ;
	BOOL										m_bSampleSkipped ;
	BOOL										m_bQualityChanged ;
	D_CCritSec									m_csFilter ;
	D_CCritSec									m_csReceive ;

	friend class D_CTransformInputPin ;
	friend class D_CTransformOutputPin ;
	D_CTransformInputPin						*m_pInput ;
	D_CTransformOutputPin						*m_pOutput ;
};

class D_CWavDestOutputPin : public D_CTransformOutputPin
{
public:
	D_CWavDestOutputPin( D_CTransformFilter *pFilter, HRESULT * phr ) ;

	STDMETHODIMP			  EnumMediaTypes			( D_IEnumMediaTypes **ppEnum ) ;
			HRESULT			  CheckMediaType			( const D_CMediaType* pmt ) ;
} ;

class D_CWavDestFilter : public D_CTransformFilter
{
public:
	virtual HRESULT	__stdcall QueryInterface			( REFIID riid, void **ppv )	{ return GetOwner()->QueryInterface( riid,ppv ) ; } ;
	virtual ULONG	__stdcall AddRef					()							{ return GetOwner()->AddRef();  } ;
	virtual ULONG	__stdcall Release					()							{ return GetOwner()->Release(); } ;

	D_CWavDestFilter( D_IUnknown * pUnk, HRESULT *pHr) ;
	~D_CWavDestFilter();

	static	D_CUnknown * WINAPI CreateInstance			( D_IUnknown * punk, HRESULT *pHr ) ;
			HRESULT			  Transform					( D_IMediaSample *pIn, D_IMediaSample *pOut ) ;
			HRESULT			  Receive					( D_IMediaSample *pSample ) ;

			HRESULT			  CheckInputType			( const D_CMediaType* mtIn ) ;
			HRESULT			  CheckTransform			( const D_CMediaType *mtIn, const D_CMediaType *mtOut ) ;
			HRESULT			  GetMediaType				( int iPosition, D_CMediaType *pMediaType ) ;

			HRESULT			  DecideBufferSize			( D_IMemAllocator *pAlloc, D_ALLOCATOR_PROPERTIES *pProperties ) ;

			HRESULT			  StartStreaming			() ;
			HRESULT			  StopStreaming				() ;

			HRESULT			  CompleteConnect			( D_PIN_DIRECTION /*direction*/, D_IPin * /*pReceivePin*/ ){ return S_OK; }

private:
			HRESULT			  Copy						( D_IMediaSample *pSource, D_IMediaSample *pDest ) const ;
			HRESULT			  Transform					( D_IMediaSample *pMediaSample ) ;
			HRESULT			  Transform					( D_AM_MEDIA_TYPE *pType, const signed char ContrastLevel ) const ;

	ULONG										m_cbWavData;
	ULONG										m_cbHeader;
};

class D_CMediaSample : public D_IMediaSample2    // The interface we support
{
protected:
	friend class D_CBaseAllocator;

	enum
	{
		Sample_SyncPoint       = 0x01,
		Sample_Preroll         = 0x02,
		Sample_Discontinuity   = 0x04,
		Sample_TypeChanged     = 0x08,
		Sample_TimeValid       = 0x10,
		Sample_MediaTimeValid  = 0x20,
		Sample_TimeDiscontinuity = 0x40,
		Sample_StopValid       = 0x100,
		Sample_ValidFlags      = 0x1FF
	};

	DWORD										m_dwFlags;
	DWORD										m_dwTypeSpecificFlags;
	LPBYTE										m_pBuffer;
	LONG										m_lActual;
	LONG										m_cbBuffer;
	class D_CBaseAllocator						*m_pAllocator;
	D_CMediaSample								*m_pNext;
	D_REFERENCE_TIME							m_Start;
	D_REFERENCE_TIME							m_End;
	LONGLONG									m_MediaStart;
	LONG										m_MediaEnd;
	D_AM_MEDIA_TYPE								*m_pMediaType;
	DWORD										m_dwStreamId;
public:
	LONG										m_cRef;

public:
	D_CMediaSample( const TCHAR *pName, class D_CBaseAllocator *pAllocator, HRESULT *phr, LPBYTE pBuffer = NULL, LONG length = 0);
#ifdef UNICODE
	D_CMediaSample( const CHAR *pName, class D_CBaseAllocator *pAllocator, HRESULT *phr, LPBYTE pBuffer = NULL, LONG length = 0);
#endif

	virtual ~D_CMediaSample();

	STDMETHODIMP			  QueryInterface			( REFIID riid, void **ppv );
	STDMETHODIMP_(ULONG)	  AddRef					();
	STDMETHODIMP_(ULONG)	  Release					();

	HRESULT					  SetPointer				( BYTE * ptr, LONG cBytes );
	STDMETHODIMP			  GetPointer				( BYTE ** ppBuffer ) ;
	STDMETHODIMP_(LONG)		  GetSize					();
	STDMETHODIMP			  GetTime					( D_REFERENCE_TIME * pTimeStart, D_REFERENCE_TIME * pTimeEnd );
	STDMETHODIMP			  SetTime					( D_REFERENCE_TIME * pTimeStart, D_REFERENCE_TIME * pTimeEnd );
	STDMETHODIMP			  IsSyncPoint				();
	STDMETHODIMP			  SetSyncPoint				( BOOL bIsSyncPoint );
	STDMETHODIMP			  IsPreroll					();
	STDMETHODIMP			  SetPreroll				( BOOL bIsPreroll );

	STDMETHODIMP_(LONG)		  GetActualDataLength		();
	STDMETHODIMP			  SetActualDataLength		( LONG lActual );
	STDMETHODIMP			  GetMediaType				( D_AM_MEDIA_TYPE **ppMediaType);
	STDMETHODIMP			  SetMediaType				( D_AM_MEDIA_TYPE *pMediaType);
	STDMETHODIMP			  IsDiscontinuity			();
	STDMETHODIMP			  SetDiscontinuity			( BOOL bDiscontinuity);
	STDMETHODIMP			  GetMediaTime				( LONGLONG * pTimeStart, LONGLONG * pTimeEnd );
	STDMETHODIMP			  SetMediaTime				( LONGLONG * pTimeStart, LONGLONG * pTimeEnd );
	STDMETHODIMP			  GetProperties				( DWORD cbProperties, BYTE * pbProperties );
	STDMETHODIMP			  SetProperties				( DWORD cbProperties, const BYTE * pbProperties );
};

class
#ifndef DX_GCC_COMPILE
__declspec(novtable)
#endif
D_CBaseAllocator : public D_CUnknown, public D_IMemAllocatorCallbackTemp, public D_CCritSec
{
	class D_CSampleList;
	friend class D_CSampleList;

	static	D_CMediaSample*	  &NextSample				( D_CMediaSample *pSample ){ return pSample->m_pNext; };

	class D_CSampleList
	{
	public:
		D_CSampleList() : m_List(NULL), m_nOnList(0) {};
			D_CMediaSample	  *Head						() const { return m_List; };
			D_CMediaSample	  *Next						( D_CMediaSample *pSample) const { return D_CBaseAllocator::NextSample(pSample); };
			int				  GetCount					() const { return m_nOnList; };
			void			  Add						( D_CMediaSample *pSample) ;
			D_CMediaSample	  *RemoveHead				() ;
			void			  Remove					( D_CMediaSample *pSample);

	public:
		D_CMediaSample							*m_List;
		int										m_nOnList;
	};
protected:

	D_CSampleList								m_lFree;
	HANDLE										m_hSem;
	long										m_lWaiting;
	long										m_lCount;
	long										m_lAllocated;
	long										m_lSize;
	long										m_lAlignment;
	long										m_lPrefix;
	BOOL										m_bChanged;
	BOOL										m_bCommitted;
	BOOL										m_bDecommitInProgress;
	D_IMemAllocatorNotifyCallbackTemp			*m_pNotify;
	BOOL										m_fEnableReleaseCallback;

	virtual void			  Free						() = 0 ;
	virtual HRESULT			  Alloc						() ;

public:
	D_CBaseAllocator( const TCHAR *, D_IUnknown *, HRESULT *, BOOL bEvent = TRUE, BOOL fEnableReleaseCallback = FALSE);
#ifdef UNICODE
	D_CBaseAllocator( const CHAR *, D_IUnknown *, HRESULT *, BOOL bEvent = TRUE, BOOL fEnableReleaseCallback = FALSE);
#endif
	virtual ~D_CBaseAllocator();

	virtual HRESULT	__stdcall QueryInterface			( REFIID riid, void **ppv )	{ return GetOwner()->QueryInterface( riid,ppv ) ; } ;
	virtual ULONG	__stdcall AddRef					()							{ return GetOwner()->AddRef();  } ;
	virtual ULONG	__stdcall Release					()							{ return GetOwner()->Release(); } ;

	STDMETHODIMP			  NonDelegatingQueryInterface( REFIID riid, void **ppv );
	STDMETHODIMP			  SetProperties				( D_ALLOCATOR_PROPERTIES* pRequest, D_ALLOCATOR_PROPERTIES* pActual);
	STDMETHODIMP			  GetProperties				( D_ALLOCATOR_PROPERTIES* pProps);
	STDMETHODIMP			  Commit					();
	STDMETHODIMP			  Decommit					();
	STDMETHODIMP			  GetBuffer					( D_IMediaSample **ppBuffer, D_REFERENCE_TIME * pStartTime, D_REFERENCE_TIME * pEndTime, DWORD dwFlags);
	STDMETHODIMP			  ReleaseBuffer				( D_IMediaSample *pBuffer);
	STDMETHODIMP			  SetNotify					( D_IMemAllocatorNotifyCallbackTemp *pNotify);
	STDMETHODIMP			  GetFreeCount				( LONG *plBuffersFree);

	void					  NotifySample				();
	void					  SetWaiting				() { m_lWaiting++; };
};

class D_CMemAllocator : public D_CBaseAllocator
{
protected:
	LPBYTE										m_pBuffer;

			void			  Free						();
			void			  ReallyFree				();
			HRESULT			  Alloc						();

public:
	static D_CUnknown		 *CreateInstance			( D_IUnknown *, HRESULT * );
	STDMETHODIMP			  SetProperties				( D_ALLOCATOR_PROPERTIES* pRequest, D_ALLOCATOR_PROPERTIES* pActual);
	D_CMemAllocator( const TCHAR *, D_IUnknown *, HRESULT *);
#ifdef UNICODE
	D_CMemAllocator( const CHAR *, D_IUnknown *, HRESULT *);
#endif
	~D_CMemAllocator();
};

class D_CAsyncRequest
{
	class D_CAsyncIo							*m_pIo;
	class D_CAsyncStream						*m_pStream;
	LONGLONG									m_llPos;
	BOOL										m_bAligned;
	LONG										m_lLength;
	BYTE										*m_pBuffer;
	LPVOID										m_pContext;
	DWORD										m_dwUser;
	HRESULT										m_hr;

public:
			HRESULT			  Request					( class D_CAsyncIo *pIo, class D_CAsyncStream *pStream, LONGLONG llPos, LONG lLength, BOOL bAligned, BYTE* pBuffer, LPVOID pContext, DWORD dwUser);
			HRESULT			  Complete					();
			HRESULT			  Cancel					(){ return S_OK; };
			LPVOID			  GetContext				(){ return m_pContext; };
			DWORD			  GetUser					(){ return m_dwUser; };
			HRESULT			  GetHResult				() { return m_hr;	};
			LONG			  GetActualLength			() { return m_lLength; };
			LONGLONG		  GetStart					() { return m_llPos; };
};

class D_CAsyncStream
{
public:
	virtual ~D_CAsyncStream() {};
	virtual HRESULT			  SetPointer( LONGLONG llPos ) = 0;
	virtual HRESULT			  Read( PBYTE pbBuffer, DWORD dwBytesToRead, BOOL bAlign, LPDWORD pdwBytesRead ) = 0;

	virtual LONGLONG		  Size( LONGLONG *pSizeAvailable = NULL ) = 0;
	virtual DWORD			  Alignment() = 0 ;
	virtual void			  Lock() = 0;
	virtual void			  Unlock() = 0;
};

typedef D_CGenericList< D_CAsyncRequest > D_CRequestList;

class D_CAsyncIo
{
	D_CCritSec									m_csReader;
	D_CAsyncStream								*m_pStream;

	D_CCritSec									m_csLists;
	BOOL										m_bFlushing;

	D_CRequestList								m_listWork;
	D_CRequestList								m_listDone;

	D_CAMEvent									m_evWork;
	D_CAMEvent									m_evDone;

	LONG										m_cItemsOut;
	BOOL										m_bWaiting;
	D_CAMEvent									m_evAllDone;

	D_CAMEvent									m_evStop;
	HANDLE										m_hThread;

			LONGLONG		  Size						(){ /*ASSERT(m_pStream != NULL);*/ return m_pStream->Size(); };
			HRESULT			  StartThread				() ;
			HRESULT			  CloseThread				();
			D_CAsyncRequest	 *GetWorkItem				();
			D_CAsyncRequest	 *GetDoneItem				();
			HRESULT			  PutWorkItem				( D_CAsyncRequest* pRequest ) ;
			HRESULT			  PutDoneItem				( D_CAsyncRequest* pRequest ) ;
			void			  ProcessRequests			() ;

	static	DWORD	WINAPI	  InitialThreadProc			(LPVOID pv){ D_CAsyncIo * pThis = (D_CAsyncIo*) pv; return pThis->ThreadProc(); };

			DWORD			  ThreadProc				() ;

public:

	D_CAsyncIo( D_CAsyncStream *pStream );
	~D_CAsyncIo();

			HRESULT			  Open						( LPCTSTR pName);
			HRESULT			  AsyncActive				();
			HRESULT			  AsyncInactive				();
			HRESULT			  Request					( LONGLONG llPos, LONG lLength, BOOL bAligned, BYTE* pBuffer, LPVOID pContext, DWORD dwUser);
			HRESULT			  WaitForNext				( DWORD dwTimeout, LPVOID *ppContext, DWORD * pdwUser, LONG * pcbActual);
			HRESULT			  SyncReadAligned			( LONGLONG llPos, LONG lLength, BYTE* pBuffer, LONG* pcbActual, PVOID pvContext);
			HRESULT			  SyncRead					( LONGLONG llPos, LONG lLength, BYTE* pBuffer);
			HRESULT			  Length					( LONGLONG *pllTotal, LONGLONG* pllAvailable);
			HRESULT			  Alignment					( LONG* pl);
			HRESULT			  BeginFlush				();
			HRESULT			  EndFlush					();

			LONG			  Alignment					(){	return ( LONG )( m_pStream->Alignment() );};
			BOOL			  IsAligned					( LONG l)      {if ((l & (Alignment() -1)) == 0){return TRUE;} else {return FALSE;}};
			BOOL			  IsAligned					( LONGLONG ll) {return IsAligned( (LONG) (ll & 0xffffffff));};
			HANDLE			  StopEvent					() const { return m_evDone; }
};

class D_CMemStream : public D_CAsyncStream
{
public:
	static	D_CUnknown *WINAPI CreateInstance			( LPBYTE pbData, LONGLONG llLength, DWORD dwKBPerSec = INFINITE );
			void			   DeleteInstance			() ;

	D_CMemStream( LPBYTE pbData, LONGLONG llLength, DWORD dwKBPerSec = INFINITE ) ;

			HRESULT			  SetPointer				(LONGLONG llPos) ;
			HRESULT			  Read						(PBYTE pbBuffer, DWORD dwBytesToRead, BOOL bAlign, LPDWORD pdwBytesRead ) ;
			LONGLONG		  Size						(LONGLONG *pSizeAvailable) ;
			DWORD			  Alignment					() ;
			void			  Lock						() ;
			void			  Unlock					() ;
			LONGLONG		  GetPosition				() ;
			LONGLONG		  GetLength					() ;

private:
	D_CCritSec									m_csLock;
	const PBYTE									m_pbData;
	const LONGLONG								m_llLength;
	LONGLONG									m_llPosition;
	DWORD										m_dwKBPerSec;
	DWORD										m_dwTimeStart;
};

class D_IAsyncReader : public D_IUnknown
{
public:
    virtual HRESULT __stdcall RequestAllocator			( D_IMemAllocator *pPreferred, D_ALLOCATOR_PROPERTIES *pProps, D_IMemAllocator **ppActual) = 0;
    virtual HRESULT __stdcall Request					( D_IMediaSample *pSample, DWORD dwUser) = 0;
    virtual HRESULT __stdcall WaitForNext				( DWORD dwTimeout, D_IMediaSample **ppSample, DWORD *pdwUser) = 0;
    virtual HRESULT __stdcall SyncReadAligned			( D_IMediaSample *pSample) = 0;
    virtual HRESULT __stdcall SyncRead					( LONGLONG llPosition, LONG lLength, BYTE *pBuffer) = 0;
    virtual HRESULT __stdcall Length					( LONGLONG *pTotal, LONGLONG *pAvailable) = 0;
    virtual HRESULT __stdcall BeginFlush				() = 0;
    virtual HRESULT __stdcall EndFlush					() = 0;
};

class D_CAsyncOutputPin : public D_IAsyncReader, public D_CBasePin
{
protected:
	class D_CAsyncReader						*m_pReader;
	D_CAsyncIo									*m_pIo;
	BOOL										m_bQueriedForAsyncReader;

			HRESULT				  InitAllocator			( D_IMemAllocator **ppAlloc ) ;

public:
	D_CAsyncOutputPin( HRESULT * phr, class D_CAsyncReader *pReader, D_CAsyncIo *pIo, D_CCritSec * pLock);
	~D_CAsyncOutputPin();

	// --- CUnknown ---

	virtual HRESULT	__stdcall QueryInterface			( REFIID riid, void **ppv )	{ return GetOwner()->QueryInterface( riid,ppv ) ; } ;
	virtual ULONG	__stdcall AddRef					()							{ return GetOwner()->AddRef();  } ;
	virtual ULONG	__stdcall Release					()							{ return GetOwner()->Release(); } ;
			STDMETHODIMP	  NonDelegatingQueryInterface( REFIID, void** );

	// --- IPin methods ---
			STDMETHODIMP	  Connect					( D_IPin * pReceivePin, const D_AM_MEDIA_TYPE *pmt );

	// --- CBasePin methods ---
			HRESULT			  GetMediaType				( int iPosition, D_CMediaType *pMediaType ) ;
			HRESULT			  CheckMediaType			( const D_CMediaType* pType ) ;
			HRESULT			  CheckConnect				( D_IPin *pPin ){ m_bQueriedForAsyncReader = FALSE; return D_CBasePin::CheckConnect(pPin); }
			HRESULT			  CompleteConnect			( D_IPin *pReceivePin ) ;
			HRESULT			  BreakConnect				() ;
			STDMETHODIMP	  RequestAllocator			( D_IMemAllocator* pPreferred, D_ALLOCATOR_PROPERTIES* pProps, D_IMemAllocator ** ppActual);
			STDMETHODIMP	  Request					( D_IMediaSample* pSample, DWORD dwUser);
			STDMETHODIMP	  WaitForNext				( DWORD dwTimeout, D_IMediaSample** ppSample, DWORD * pdwUser);
			STDMETHODIMP	  SyncReadAligned			( D_IMediaSample* pSample);
			STDMETHODIMP	  SyncRead					( LONGLONG llPosition, LONG lLength, BYTE* pBuffer);
			STDMETHODIMP	  Length					( LONGLONG* pTotal, LONGLONG* pAvailable);
			STDMETHODIMP	  BeginFlush				();
			STDMETHODIMP	  EndFlush					();
};

class D_CAsyncReader : public D_CBaseFilter
{

protected:
	D_CCritSec									m_csFilter;
	D_CAsyncIo									m_Io;
	D_CAsyncOutputPin							m_OutputPin;
	D_CMediaType								m_mt;

public:
    
	D_CAsyncReader( const TCHAR *pName, D_IUnknown * pUnk, D_CAsyncStream *pStream, HRESULT *phr);
	~D_CAsyncReader();

	// --- CBaseFilter methods ---
			int				  GetPinCount				();
			D_CBasePin		 *GetPin					(int n);

	// --- Access our media type
	const	D_CMediaType	 *LoadType					() const { return &m_mt;}
	virtual HRESULT			  Connect					( D_IPin * pReceivePin, const D_AM_MEDIA_TYPE *pmt ){ return m_OutputPin.D_CBasePin::Connect( pReceivePin, pmt ) ; }
} ;

class D_CMemReader : public D_CAsyncReader
{
public:
	D_CMemReader( D_CMemStream *pStream, D_CMediaType *pmt, HRESULT *phr) : D_CAsyncReader(TEXT("Mem Reader\0"), NULL, pStream, phr){ m_mt = *pmt; }

			STDMETHODIMP	  Register					(){ return S_OK; }
			STDMETHODIMP	  Unregister				(){ return S_OK; }
	
	static	D_CUnknown *WINAPI CreateInstance			(D_CMemStream *pStream, D_CMediaType *pmt, HRESULT *phr);
			void			  DeleteInstance			() ;
};






typedef HRESULT ( *D_SAMPLECALLBACK )( D_IMediaSample * pSample, D_REFERENCE_TIME * StartTime, D_REFERENCE_TIME * StopTime, BOOL TypeChanged, void *CallbackData );

class D_CTransInPlaceInputPin : public D_CTransformInputPin
{

protected:
	class D_CTransInPlaceFilter * const			m_pTIPFilter;
	BOOL										m_bReadOnly;

public:

	D_CTransInPlaceInputPin( TCHAR              *pObjectName, D_CTransInPlaceFilter *pFilter, HRESULT             *phr, LPCWSTR              pName);
#ifdef UNICODE
	D_CTransInPlaceInputPin( char               *pObjectName, D_CTransInPlaceFilter *pFilter, HRESULT             *phr, LPCWSTR              pName);
#endif

			STDMETHODIMP	  EnumMediaTypes			( D_IEnumMediaTypes **ppEnum );
			HRESULT			  CheckMediaType			(const D_CMediaType* pmt);
			STDMETHODIMP	  GetAllocator				(D_IMemAllocator ** ppAllocator);
			STDMETHODIMP	  NotifyAllocator			(D_IMemAllocator * pAllocator, BOOL bReadOnly);
			D_IMemAllocator	  *PeekAllocator			() const {  return m_pAllocator; }
			STDMETHODIMP	  GetAllocatorRequirements	(D_ALLOCATOR_PROPERTIES *pProps);
			HRESULT			  CompleteConnect			(D_IPin *pReceivePin);
			BOOL			  ReadOnly					() { return m_bReadOnly ; }
};

class D_CTransInPlaceOutputPin : public D_CTransformOutputPin
{

protected:
	D_CTransInPlaceFilter * const				m_pTIPFilter;

public:

	D_CTransInPlaceOutputPin( TCHAR *pObjectName, D_CTransInPlaceFilter *pFilter, HRESULT *phr, LPCWSTR pName);
#ifdef UNICODE
	D_CTransInPlaceOutputPin( char *pObjectName, D_CTransInPlaceFilter *pFilter, HRESULT *phr, LPCWSTR pName);
#endif

			STDMETHODIMP	  EnumMediaTypes			( D_IEnumMediaTypes **ppEnum );
			HRESULT			  CheckMediaType			(const D_CMediaType* pmt);
			void			  SetAllocator				(D_IMemAllocator * pAllocator);
			D_IMemInputPin	  *ConnectedIMemInputPin	(){ return m_pInputPin; }
			D_IMemAllocator	  *PeekAllocator			() const {  return m_pAllocator; }
			HRESULT			  CompleteConnect			(D_IPin *pReceivePin);
};

class
#ifndef DX_GCC_COMPILE
__declspec(novtable)
#endif
D_CTransInPlaceFilter : public D_CTransformFilter
{
public:
	virtual D_CBasePin		  *GetPin					(int n);

public:
	D_CTransInPlaceFilter(TCHAR *, D_IUnknown *, REFCLSID clsid, HRESULT *, bool bModifiesData = true);
#ifdef UNICODE
	D_CTransInPlaceFilter(CHAR *, D_IUnknown *, REFCLSID clsid, HRESULT *, bool bModifiesData = true);
#endif

	virtual ~D_CTransInPlaceFilter();

			HRESULT			  GetMediaType				(int /*iPosition*/, D_CMediaType * /*pMediaType*/){ return E_UNEXPECTED;	}
			HRESULT			  DecideBufferSize			(D_IMemAllocator*, D_ALLOCATOR_PROPERTIES *);
			HRESULT			  CheckTransform			(const D_CMediaType * /*mtIn*/, const D_CMediaType * /*mtOut*/){ return S_OK; };
			HRESULT			  CompleteConnect			(D_PIN_DIRECTION dir,D_IPin *pReceivePin);
	virtual HRESULT			  Receive					(D_IMediaSample *pSample);
	virtual HRESULT			  Transform					(D_IMediaSample *pSample ) = 0 ;

#ifdef PERF
	virtual void			  RegisterPerfId			(){ m_idTransInPlace = MSR_REGISTER(TEXT("TransInPlace"));}
#endif

protected:

	D_IMediaSample								*Copy(D_IMediaSample *pSource);

#ifdef PERF
	int											m_idTransInPlace;
#endif
	bool										m_bModifiesData;

	friend class D_CTransInPlaceInputPin;
	friend class D_CTransInPlaceOutputPin;

			D_CTransInPlaceInputPin  *InputPin					() const { return (D_CTransInPlaceInputPin *)m_pInput; };
			D_CTransInPlaceOutputPin *OutputPin					() const { return (D_CTransInPlaceOutputPin *)m_pOutput; };
			BOOL					  TypesMatch				() { return InputPin()->CurrentMediaType() == OutputPin()->CurrentMediaType(); }
			BOOL					  UsingDifferentAllocators	() const { return InputPin()->PeekAllocator() != OutputPin()->PeekAllocator(); }
};

class D_IGrabberSample : public D_IUnknown
{
public:
    virtual HRESULT __stdcall		SetAcceptedMediaType		( const D_CMediaType *pType) = 0;
    virtual HRESULT __stdcall		GetConnectedMediaType		( D_CMediaType *pType) = 0;
    virtual HRESULT __stdcall		SetCallback					( D_SAMPLECALLBACK Callback, void *CallbackData) = 0;
    virtual HRESULT __stdcall		SetDeliveryBuffer			( D_ALLOCATOR_PROPERTIES props, BYTE *pBuffer) = 0;
};

class D_CSampleGrabberInPin;
class D_CSampleGrabber;

class D_CSampleGrabberAllocator : public D_CMemAllocator
{
	friend class D_CSampleGrabberInPin;
	friend class D_CSampleGrabber;

protected:
	D_CSampleGrabberInPin						*m_pPin;

public:
//		D_CSampleGrabberAllocator( D_CSampleGrabberInPin * pParent, HRESULT *phr ) : D_CMemAllocator( TEXT("SampleGrabberAllocator\0"), NULL, phr ), m_pPin( pParent ){};
	D_CSampleGrabberAllocator( D_CSampleGrabberInPin * pParent, HRESULT *phr ) : D_CMemAllocator( "SampleGrabberAllocator\0", NULL, phr ), m_pPin( pParent ){};
	~D_CSampleGrabberAllocator( ){ m_pBuffer = NULL; }

			HRESULT			  Alloc						();
			void			  ReallyFree				();
			STDMETHODIMP	  SetProperties				(D_ALLOCATOR_PROPERTIES *pRequest, D_ALLOCATOR_PROPERTIES *pActual);
};

class D_CSampleGrabberInPin : public D_CTransInPlaceInputPin
{
	friend class D_CSampleGrabberAllocator;
	friend class D_CSampleGrabber;

	D_CSampleGrabberAllocator					*m_pPrivateAllocator;
	D_ALLOCATOR_PROPERTIES						m_allocprops;
	BYTE										*m_pBuffer;
	BOOL										m_bMediaTypeChanged;

protected:
			D_CSampleGrabber  *SampleGrabber			() { return (D_CSampleGrabber*) m_pFilter; }
			HRESULT			  SetDeliveryBuffer			( D_ALLOCATOR_PROPERTIES props, BYTE * m_pBuffer );

public:
	D_CSampleGrabberInPin( D_CTransInPlaceFilter * pFilter, HRESULT * pHr ) ;
	~D_CSampleGrabberInPin() ;

			HRESULT			  GetMediaType				( int iPosition, D_CMediaType *pMediaType );
			STDMETHODIMP	  EnumMediaTypes			( D_IEnumMediaTypes **ppEnum );
			STDMETHODIMP	  NotifyAllocator			( D_IMemAllocator *pAllocator, BOOL bReadOnly );
			STDMETHODIMP	  GetAllocator				( D_IMemAllocator **ppAllocator );
			HRESULT			  SetMediaType				( const D_CMediaType *pmt );
			STDMETHODIMP	  GetAllocatorRequirements	( D_ALLOCATOR_PROPERTIES *pProps );
};

class D_CSampleGrabber : public D_CTransInPlaceFilter, public D_IGrabberSample
{
	friend class D_CSampleGrabberInPin;
	friend class D_CSampleGrabberAllocator;

protected:

	D_CMediaType								m_mtAccept;
	D_SAMPLECALLBACK							m_callback;
	D_CCritSec									m_Lock;
	void										*m_CallbackData ;

			BOOL			  IsReadOnly				( ) { return !m_bModifiesData; }
			HRESULT			  CheckInputType			( const D_CMediaType * pmt );
			HRESULT			  Transform					( D_IMediaSample * pms );
			HRESULT			  Receive					( D_IMediaSample * pms );

public:
	D_CSampleGrabber( D_IUnknown * pOuter, HRESULT * pHr, BOOL ModifiesData );

	virtual ~D_CSampleGrabber();

			STDMETHODIMP	  NonDelegatingQueryInterface(REFIID riid, void ** ppv);
	virtual HRESULT	__stdcall QueryInterface			( REFIID riid, void **ppv )	{ return GetOwner()->QueryInterface( riid,ppv ) ; } ;
	virtual ULONG	__stdcall AddRef					()							{ return GetOwner()->AddRef();  } ;
	virtual ULONG	__stdcall Release					()							{ return GetOwner()->Release(); } ;

			STDMETHODIMP	  SetAcceptedMediaType		( const D_CMediaType * pmt );
			STDMETHODIMP	  GetConnectedMediaType		( D_CMediaType * pmt );
			STDMETHODIMP	  SetCallback				( D_SAMPLECALLBACK Callback, void *CallbackData );
			STDMETHODIMP	  SetDeliveryBuffer			( D_ALLOCATOR_PROPERTIES props, BYTE * m_pBuffer );

	static	D_CUnknown *WINAPI CreateInstance			(D_IUnknown * punk, HRESULT *phr);
			void			   DeleteInstance			() ;
};

#endif // DX_NON_DSHOW_MP3

extern	D_CMovieRender *New_D_CMovieRender( D_IUnknown * pUnk, HRESULT *phr ) ;


#ifndef DX_NON_NAMESPACE

//}

#endif // DX_NON_NAMESPACE

#endif	/* DX_NON_MOVIE */
#endif  /* DX_NON_DSHOW_MOVIE */










#ifndef DX_NON_MEDIA_FOUNDATION

class D_SoundConvertMFByteStream : public D_IMFByteStream
{
public:
#ifndef DX_NON_NAMESPACE
	D_SoundConvertMFByteStream					( DxLib::STREAMDATA *_Stream, ULONGLONG _StreamSize ) ;
#else // DX_NON_NAMESPACE
	D_SoundConvertMFByteStream					( STREAMDATA *_Stream, ULONGLONG _StreamSize ) ;
#endif // DX_NON_NAMESPACE
	virtual ~D_SoundConvertMFByteStream			() ;

	// IUnknown
	HRESULT	__stdcall QueryInterface			( REFIID riid, void **ppv ) ;
	ULONG	__stdcall AddRef					( void ) ;
	ULONG	__stdcall Release					( void ) ;

	//IMFByteStream
	HRESULT __stdcall GetCapabilities			( DWORD *pdwCapabilities ) ;
	HRESULT __stdcall GetLength					( ULONGLONG *pqwLength ) ;
	HRESULT __stdcall SetLength					( ULONGLONG qwLength ) ;
	HRESULT __stdcall GetCurrentPosition		( ULONGLONG *pqwPosition ) ;
	HRESULT __stdcall SetCurrentPosition		( ULONGLONG qwPosition ) ;
	HRESULT __stdcall IsEndOfStream				( BOOL *pfEndOfStream ) ;
	HRESULT __stdcall Read						( BYTE *pb, ULONG cb, ULONG *pcbRead ) ;
	HRESULT __stdcall BeginRead					( BYTE *pb, ULONG cb, D_IMFAsyncCallback *pCallback, D_IUnknown *punkState ) ;
	HRESULT __stdcall EndRead					( D_IMFAsyncResult *pResult, LONG *pcbRead ) ;
	HRESULT __stdcall Write						( const BYTE *pb, ULONG cb, ULONG *pcbWritten ) ;
	HRESULT __stdcall BeginWrite				( const BYTE *pb, ULONG cb, D_IMFAsyncCallback *pCallback, D_IUnknown *punkState ) ;
	HRESULT __stdcall EndWrite					( D_IMFAsyncResult *pResult, ULONG *pcbWritten ) ;
	HRESULT __stdcall Seek						( D_MFBYTESTREAM_SEEK_ORIGIN SeekOrigin, LONGLONG llSeekOffset, DWORD dwSeekFlags, ULONGLONG *pqwCurrentPosition ) ;
	HRESULT __stdcall Flush						( void ) ;
	HRESULT __stdcall Close						( void ) ;

	HRESULT Initialize							( void ) ;

private:
	static DWORD WINAPI	AsyncThread				( void *pContext ) ;

	volatile LONG				ReferenceCount;

	volatile int				ASyncReadRequest ;
	         D_IMFAsyncResult	*pMFASyncResult ;
	volatile BYTE				*pASyncReadBuffer ;
	volatile ULONG				ASyncReadSize ;
	volatile LONGLONG			ASyncReadPoint ;
	volatile LONG				ASyncReadResultSize ;
     
#ifndef DX_NON_NAMESPACE
	DxLib::STREAMDATA			*Stream ;
#else // DX_NON_NAMESPACE
	STREAMDATA					*Stream ;
#endif // DX_NON_NAMESPACE
	ULONGLONG					StreamSize ;
	D_IMFAttributes				*pMFAttributes ;
#ifndef DX_NON_NAMESPACE
	DxLib::DX_CRITICAL_SECTION	ASyncReadCriticalSection ;
#else // DX_NON_NAMESPACE
	DX_CRITICAL_SECTION			ASyncReadCriticalSection ;
#endif // DX_NON_NAMESPACE
	HANDLE						ASyncThread ;
	HANDLE						ASyncThreadEvent ;
	volatile int				ASyncThreadEndRequestFlag ;
	volatile int				ASyncThreadEndFlag ;
	int							SeekRequest ;
	LONGLONG					SeekRequestPoint ;
} ;

// Media Foundation データ変換用構造体
typedef struct tagSOUNDCONV_MF
{
	D_IMFSourceReader			*pReader ;						// ソースリーダー
	D_IMFMediaType				*pMediaTypeAudioStream ;		// 変換前のオーディオストリームメディア属性
	D_IMFMediaType				*pMediaTypeOutputAudioStream ;	// 変換後のオーディオストリームメディア属性
	D_SoundConvertMFByteStream	*pByteStream ;					// バイトストリーム
	LONGLONG					Duration ;						// 長さ
	DWORD						SampleBufferBytes ;				// 読み込んだサンプルを保存しているバッファのサイズ
	void						*pSampleBuffer ;				// 読み込んだサンプルを保存しているバッファ
	DWORD						OutSampleBufferBytes ;			// 出力済みのサイズ
	DWORD						UseSampleBufferBytes ;			// 実データが格納されているサイズ
} SOUNDCONV_MF ;

#ifndef DX_NON_NAMESPACE
extern	D_SoundConvertMFByteStream *New_D_SoundConvertMFByteStream( DxLib::STREAMDATA *_Stream, ULONGLONG _StreamSize ) ;
#else // DX_NON_NAMESPACE
extern	D_SoundConvertMFByteStream *New_D_SoundConvertMFByteStream( STREAMDATA *_Stream, ULONGLONG _StreamSize ) ;
#endif // DX_NON_NAMESPACE

#endif // DX_NON_MEDIA_FOUNDATION


#endif // DX_USECSTRMBASEFILTER_H

