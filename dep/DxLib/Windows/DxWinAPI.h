// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		WinAPIプログラムヘッダファイル
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

#ifndef DX_WINAPI_H
#define DX_WINAPI_H

// インクルード ------------------------------------------------------------------
#include "../DxCompileConfig.h"

#if !defined(CINTERFACE) && defined(__c2__) &&  __clang_major__ == 3 && __clang_minor__ == 8
//To avoid compile error
//C:\Program Files (x86)\Windows Kits\8.1\Include\um\combaseapi.h(229,21): error : unknown type name 'IUnknown'
//          static_cast<IUnknown*>(*pp);    // make sure everyone derives from IUnknown
#define CINTERFACE
#endif
#include <windows.h>

#if _MSC_VER == 1200
#include <multimon.h>
#endif

#include <winsock.h>
#include <wtypes.h>
#include "../DxNetwork.h"
#include "DxDirectX.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

#define D_HTOUCHINPUT		void *

// API が初期化されていなかったら初期化するマクロ
#ifndef DX_NON_NAMESPACE

#define SETUP_WIN_API		\
	if( DxLib::WinAPIData.InitializeFlag == FALSE )\
	{\
		DxLib::LoadWinAPI() ;\
	\
		/* DPI設定には対応している、に設定する */\
		if( DxLib::WinAPIData.Win32Func.SetProcessDpiAwarenessFunc != NULL )\
		{\
			DxLib::WinAPIData.Win32Func.SetProcessDpiAwarenessFunc( DxLib::D_PROCESS_SYSTEM_DPI_AWARE ) ;\
		}\
	}

#else // DX_NON_NAMESPACE

#define SETUP_WIN_API		\
	if( WinAPIData.InitializeFlag == FALSE )\
	{\
		LoadWinAPI() ;\
	\
		/* DPI設定には対応している、に設定する */\
		if( WinAPIData.Win32Func.SetProcessDpiAwarenessFunc != NULL )\
		{\
			WinAPIData.Win32Func.SetProcessDpiAwarenessFunc( D_PROCESS_SYSTEM_DPI_AWARE ) ;\
		}\
	}

#endif // DX_NON_NAMESPACE

// デフォルトのDPI
#define DEFAULT_DPI			96

// DPI対応度設定
typedef enum tagD_PROCESS_DPI_AWARENESS
{
	D_PROCESS_DPI_UNAWARE				= 0,
	D_PROCESS_SYSTEM_DPI_AWARE			= 1,
	D_PROCESS_PER_MONITOR_DPI_AWARE		= 2
} D_PROCESS_DPI_AWARENESS ;

// DPI対応度設定
typedef enum tagD_MONITOR_DPI_TYPE
{
	D_MDT_EFFECTIVE_DPI					= 0,
	D_MDT_ANGULAR_DPI					= 1,
	D_MDT_RAW_DPI						= 2,
	D_MDT_DEFAULT						= D_MDT_EFFECTIVE_DPI
} D_MONITOR_DPI_TYPE ;

// コールバック関数定義 ----------------------------------------------------------

#if _MSC_VER > 1200 || defined( DX_GCC_COMPILE_4_9_2 )
typedef void ( CALLBACK D_TIMECALLBACK )( UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2 ) ;
typedef VOID ( CALLBACK* D_TIMERPROC )( HWND, UINT, UINT_PTR, DWORD ) ;
#else
typedef void ( CALLBACK D_TIMECALLBACK )( UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2 ) ;
typedef VOID ( CALLBACK* D_TIMERPROC )( HWND, UINT, UINT, DWORD ) ;
#endif
typedef D_TIMECALLBACK FAR *LPD_TIMECALLBACK ;

// 構造体定義 --------------------------------------------------------------------

typedef struct D_tagWCRANGE
{
	WCHAR					wcLow ;
	USHORT					cGlyphs ;
} D_WCRANGE, *D_PWCRANGE,FAR *D_LPWCRANGE ;

typedef struct D_tagGLYPHSET
{
	DWORD					cbThis ;
	DWORD					flAccel ;
	DWORD					cGlyphsSupported ;
	DWORD					cRanges ;
	D_WCRANGE				ranges[ 1 ] ;
} D_GLYPHSET, *D_PGLYPHSET, FAR *D_LPGLYPHSET;

typedef struct D__TOUCHINPUT
{
	LONG					x ;
	LONG					y ;
	HANDLE					hSource ;
	DWORD					dwID ;
	DWORD					dwFlags ;
	DWORD					dwMask ;
	DWORD					dwTime ;
#ifdef _WIN64
	ULONGLONG				dwExtraInfo ;
#else
	DWORD					dwExtraInfo;
#endif
	DWORD					cxContact ;
	DWORD					cyContact ;
} D_TOUCHINPUT, *D_PTOUCHINPUT ;
typedef D_TOUCHINPUT const * D_PCTOUCHINPUT;

typedef struct D__SCRIPT_CONTROL
{
	DWORD					uDefaultLanguage    :16 ;
	DWORD					fContextDigits      :1  ;
	DWORD					fInvertPreBoundDir  :1  ;
	DWORD					fInvertPostBoundDir :1  ;
	DWORD					fLinkStringBefore   :1  ;
	DWORD					fLinkStringAfter    :1  ;
	DWORD					fNeutralOverride    :1  ;
	DWORD					fNumericOverride    :1  ;
	DWORD					fLegacyBidiClass    :1  ;
	DWORD					fMergeNeutralItems  :1  ;
	DWORD					fUseStandardBidi    :1  ;
	DWORD					fReserved           :6  ;
} D_SCRIPT_CONTROL ;

typedef struct D__SCRIPT_STATE
{
	WORD					uBidiLevel         :5 ;
	WORD					fOverrideDirection :1 ;
	WORD					fInhibitSymSwap    :1 ;
	WORD					fCharShape         :1 ;
	WORD					fDigitSubstitute   :1 ;
	WORD					fInhibitLigate     :1 ;
	WORD					fDisplayZWG        :1 ;
	WORD					fArabicNumContext  :1 ;
	WORD					fGcpClusters       :1 ;
	WORD					fReserved          :1 ;
	WORD					fEngineReserved    :2 ;
} D_SCRIPT_STATE ;

typedef struct D__SCRIPT_ANALYSIS
{
	WORD					eScript         :10 ;
	WORD					fRTL            :1  ;
	WORD					fLayoutRTL      :1  ;
	WORD					fLinkBefore     :1  ;
	WORD					fLinkAfter      :1  ;
	WORD					fLogicalOrder   :1  ;
	WORD					fNoGlyphIndex   :1  ;
	D_SCRIPT_STATE			s ;
} D_SCRIPT_ANALYSIS ;

typedef struct D__SCRIPT_ITEM
{
	int						iCharPos ;
	D_SCRIPT_ANALYSIS		a ;
} D_SCRIPT_ITEM ;

typedef struct D__SCRIPT_VISATTR
{
	WORD					uJustification   :4 ;
	WORD					fClusterStart    :1 ;
	WORD					fDiacritic       :1 ;
	WORD					fZeroWidth       :1 ;
	WORD					fReserved        :1 ;
	WORD					fShapeReserved   :8 ;
} D_SCRIPT_VISATTR ;

typedef struct tagD_MONITORINFO
{
	DWORD					cbSize ;
	RECT					rcMonitor ;
	RECT					rcWork ;
	DWORD					dwFlags ;
} D_MONITORINFO ;

#define D_CCHDEVICENAME 32
#define D_CCHFORMNAME   32

typedef struct tagD_MONITORINFOEXA
{
	DWORD					cbSize ;
	RECT					rcMonitor ;
	RECT					rcWork ;
	DWORD					dwFlags ;
	CHAR					szDevice[ D_CCHDEVICENAME ] ;
} D_MONITORINFOEXA ;

typedef struct tagD_MONITORINFOEXW
{
	DWORD					cbSize ;
	RECT					rcMonitor ;
	RECT					rcWork ;
	DWORD					dwFlags ;
	WCHAR					szDevice[ D_CCHDEVICENAME ] ;
} D_MONITORINFOEXW ;

typedef struct D__OSVERSIONINFOEXA
{
	DWORD					dwOSVersionInfoSize ;
	DWORD					dwMajorVersion ;
	DWORD					dwMinorVersion ;
	DWORD					dwBuildNumber ;
	DWORD					dwPlatformId ;
	CHAR					szCSDVersion[ 128 ] ;
	WORD					wServicePackMajor ;
	WORD					wServicePackMinor ;
	WORD					wSuiteMask ;
	BYTE					wProductType ;
	BYTE					wReserved ;
} D_OSVERSIONINFOEXA ;

typedef struct D__OSVERSIONINFOEXW 
{
	DWORD					dwOSVersionInfoSize ;
	DWORD					dwMajorVersion ;
	DWORD					dwMinorVersion ;
	DWORD					dwBuildNumber ;
	DWORD					dwPlatformId ;
	WCHAR					szCSDVersion[ 128 ] ;
	WORD					wServicePackMajor ;
	WORD					wServicePackMinor ;
	WORD					wSuiteMask ;
	BYTE					wProductType ;
	BYTE					wReserved ;
} D_OSVERSIONINFOEXW ;

typedef struct D__DISPLAY_DEVICEA
{
	DWORD					cb ;
	CHAR					DeviceName[ 32 ] ;
	CHAR					DeviceString[ 128 ] ;
	DWORD					StateFlags ;
	CHAR					DeviceID[ 128 ] ;
	CHAR					DeviceKey[ 128 ] ;
} D_DISPLAY_DEVICEA ;

typedef struct D__DISPLAY_DEVICEW
{
	DWORD					cb ;
	WCHAR					DeviceName[ 32 ] ;
	WCHAR					DeviceString[ 128 ] ;
	DWORD					StateFlags ;
	WCHAR					DeviceID[ 128 ] ;
	WCHAR					DeviceKey[ 128 ] ;
} D_DISPLAY_DEVICEW ;

typedef struct D__devicemodeA
{
	BYTE					dmDeviceName[ D_CCHDEVICENAME ] ;
	WORD					dmSpecVersion ;
	WORD					dmDriverVersion ;
	WORD					dmSize ;
	WORD					dmDriverExtra ;
	DWORD					dmFields ;
	union 
	{
		struct 
		{
			short			dmOrientation ;
			short			dmPaperSize ;
			short			dmPaperLength ;
			short			dmPaperWidth ;
			short			dmScale ;
			short			dmCopies ;
			short			dmDefaultSource ;
			short			dmPrintQuality ;
		} s1 ;
		struct
		{
			POINTL			dmPosition ;
			DWORD			dmDisplayOrientation ;
			DWORD			dmDisplayFixedOutput ;
		} s2 ;
	} ;
	short					dmColor ;
	short					dmDuplex ;
	short					dmYResolution ;
	short					dmTTOption ;
	short					dmCollate ;
	BYTE					dmFormName[ D_CCHFORMNAME ] ;
	WORD					dmLogPixels ;
	DWORD					dmBitsPerPel ;
	DWORD					dmPelsWidth ;
	DWORD					dmPelsHeight ;
	union 
	{
		DWORD				dmDisplayFlags ;
		DWORD				dmNup ;
	} ;
	DWORD					dmDisplayFrequency ;
#if(WINVER >= 0x0400)
	DWORD					dmICMMethod ;
	DWORD					dmICMIntent ;
	DWORD					dmMediaType ;
	DWORD					dmDitherType ;
	DWORD					dmReserved1 ;
	DWORD					dmReserved2 ;
#if (WINVER >= 0x0500) || (_WIN32_WINNT >= _WIN32_WINNT_NT4)
	DWORD					dmPanningWidth ;
	DWORD					dmPanningHeight ;
#endif
#endif /* WINVER >= 0x0400 */
} D_DEVMODEA ;

typedef struct D__devicemodeW
{
	WCHAR					dmDeviceName[ D_CCHDEVICENAME ] ;
	WORD					dmSpecVersion ;
	WORD					dmDriverVersion ;
	WORD					dmSize ;
	WORD					dmDriverExtra ;
	DWORD					dmFields ;
	union 
	{
		struct 
		{
			short			dmOrientation ;
			short			dmPaperSize ;
			short			dmPaperLength ;
			short			dmPaperWidth ;
			short			dmScale ;
			short			dmCopies ;
			short			dmDefaultSource ;
			short			dmPrintQuality ;
		} s1 ;
		struct
		{
			POINTL			dmPosition ;
			DWORD			dmDisplayOrientation ;
			DWORD			dmDisplayFixedOutput ;
		} s2 ;
	} ;
	short					dmColor ;
	short					dmDuplex ;
	short					dmYResolution ;
	short					dmTTOption ;
	short					dmCollate ;
	WCHAR					dmFormName[ D_CCHFORMNAME ] ;
	WORD					dmLogPixels ;
	DWORD					dmBitsPerPel ;
	DWORD					dmPelsWidth ;
	DWORD					dmPelsHeight ;
	union 
	{
		DWORD				dmDisplayFlags ;
		DWORD				dmNup ;
	} ;
	DWORD					dmDisplayFrequency ;
#if(WINVER >= 0x0400)
	DWORD					dmICMMethod ;
	DWORD					dmICMIntent ;
	DWORD					dmMediaType ;
	DWORD					dmDitherType ;
	DWORD					dmReserved1 ;
	DWORD					dmReserved2 ;
#if (WINVER >= 0x0500) || (_WIN32_WINNT >= _WIN32_WINNT_NT4)
	DWORD					dmPanningWidth ;
	DWORD					dmPanningHeight ;
#endif
#endif /* WINVER >= 0x0400 */
} D_DEVMODEW ;

/*
 * Raw Input Device Information
 */
#define D_RIDI_PREPARSEDDATA      0x20000005
#define D_RIDI_DEVICENAME         0x20000007  // the return valus is the character length, not the byte size
#define D_RIDI_DEVICEINFO         0x2000000b

typedef struct tagD_RAWINPUTDEVICELIST
{
    HANDLE hDevice;
    DWORD dwType;
} D_RAWINPUTDEVICELIST ;

#ifndef DX_NON_NETWORK

// WinSock の DLL のポインタや中のAPIのポインタなど
struct WINSOCKFUNCTION
{
	HMODULE					WinSockDLL ;						// WinSockDLL
	int						( WINAPI *WSAGetLastErrorFunc )( void ) ;
	int						( WINAPI *WSAStartupFunc )( WORD wVersionRequested, LPWSADATA lpWSAData ) ;
	int						( WINAPI *WSACleanupFunc )( void ) ;
	int						( WINAPI *WSAAsyncSelectFunc )( SOCKET s, HWND hWnd, unsigned int wMsg, long lEvent ) ;
	int						( WINAPI *getaddrinfoFunc )( const char *nodename, const char *servname, const _addrinfo *hints, _addrinfo **res ) ;
	void					( WINAPI *freeaddrinfoFunc )( _addrinfo *pAddrInfo ) ;
	struct hostent*			( WINAPI *gethostbyaddrFunc )( const char *addr, int len, int type ) ;
	struct hostent*			( WINAPI *gethostbynameFunc )( const char *name ) ;
	int						( WINAPI *gethostnameFunc )( char *name, int namelen ) ;
	u_short					( WINAPI *ntohsFunc )( u_short netshort ) ;
	u_short					( WINAPI *htonsFunc )( u_short hostshort ) ;
	int						( WINAPI *connectFunc )( SOCKET s, const struct sockaddr *name, int namelen ) ;
	SOCKET					( WINAPI *socketFunc )( int af, int type, int protocol ) ;
	int						( WINAPI *sendFunc )( SOCKET s, const char *buf, int len, int flags ) ;
	int						( WINAPI *sendtoFunc )( SOCKET s, const char *buf, int len, int flags, const struct sockaddr *to, int tolen ) ;
	int						( WINAPI *recvfromFunc )( SOCKET s, char *buf, int len, int flags, struct sockaddr *from, int *fromlen ) ;
	SOCKET					( WINAPI *acceptFunc )( SOCKET s, struct sockaddr *addr, int *addrlen ) ;
	int						( WINAPI *closesocketFunc )( SOCKET s ) ;
	int						( WINAPI *shutdownFunc )( SOCKET s, int how ) ;
	int						( WINAPI *listenFunc )( SOCKET s, int backlog ) ;
	int						( WINAPI *bindFunc )( SOCKET s, const struct sockaddr *name, int namelen ) ;
	unsigned long			( WINAPI *inet_addrFunc )( const char *cp ) ;
	int						( WINAPI *recvFunc )( SOCKET s, char *buf, int len, int flags ) ;
	int						( WINAPI *setsockoptFunc )( SOCKET s, int level, int optname, const char *optval, int optlen ) ;
} ;

#endif // DX_NON_NETWORK

// Input Method Manager DLL のポインタや中のAPIのポインタなど
struct IMMFUNCTION
{
	HMODULE					Imm32DLL ;
	HIMC					( WINAPI *ImmAssociateContextFunc )( HWND hWnd, HIMC hIMC ) ;
	HIMC					( WINAPI *ImmGetContextFunc )( HWND hWnd );
	BOOL					( WINAPI *ImmReleaseContextFunc )( HWND hWnd, HIMC hIMC ) ;
	BOOL					( WINAPI *ImmGetOpenStatusFunc )( HIMC hIMC ) ;
	BOOL					( WINAPI *ImmGetConversionStatusFunc )( HIMC hIMC, LPDWORD lpfdwConversion, LPDWORD lpfdwSentence ) ;
	BOOL					( WINAPI *ImmNotifyIMEFunc )( HIMC hIMC, DWORD dwAction, DWORD dwIndex, DWORD dwValue ) ;
	BOOL					( WINAPI *ImmSetOpenStatusFunc )( HIMC hIMC, BOOL fOpen ) ;

	DWORD					( WINAPI *ImmGetCandidateListFunc )( HIMC hIMC, DWORD deIndex, LPCANDIDATELIST lpCandList, DWORD dwBufLen ) ;
	DWORD					( WINAPI *ImmGetCandidateListCountFunc )( HIMC hIMC, LPDWORD lpdwListCount ) ;
	LONG					( WINAPI *ImmGetCompositionStringFunc )( HIMC hIMC, DWORD dwIndex, LPVOID lpBuf, DWORD dwBufLen ) ;
	BOOL					( WINAPI *ImmSetCompositionStringFunc )( HIMC hIMC, DWORD dwIndex, LPCVOID lpComp, DWORD dwCompLen, LPCVOID lpRead, DWORD dwReadLen ) ;
} ;

// Win32 API DLL のポインタや API のポインタなど
struct WIN32APIFUNCTION
{
	HMODULE					WinMMDLL ;
	MMRESULT				( WINAPI *timeSetEventFunc )( UINT uDelay, UINT uResolution, LPD_TIMECALLBACK lpTimeProc, DWORD_PTR dwUser, UINT fuEvent ) ;
	MMRESULT				( WINAPI *timeKillEventFunc )( UINT uTimerID ) ;
	MMRESULT				( WINAPI *timeBeginPeriodFunc )( UINT uPeriod ) ;
	MMRESULT				( WINAPI *timeEndPeriodFunc )( UINT uPeriod ) ;
	DWORD					( WINAPI *timeGetTimeFunc )( VOID ) ;
	MMRESULT				( WINAPI *timeGetDevCapsFunc )( LPTIMECAPS ptc, UINT cbtc ) ;
	MMRESULT				( WINAPI *joyGetPosExFunc )( UINT uJoyID, LPJOYINFOEX pji ) ;

	MMRESULT				( WINAPI *joyGetDevCapsFunc )( UINT uJoyID, LPJOYCAPSW pjc, UINT cbjc ) ;
	MCIERROR				( WINAPI *mciSendCommandFunc )( MCIDEVICEID IDDevice, UINT uMsg, DWORD_PTR fdwCommand, DWORD_PTR dwParam ) ;

	MMRESULT				( WINAPI *waveOutOpenFunc )( LPHWAVEOUT phwo, UINT uDeviceID, LPCWAVEFORMATEX pwfx, DWORD_PTR dwCallback, DWORD_PTR dwInstance, DWORD fdwOpen ) ;
	MMRESULT				( WINAPI *waveOutCloseFunc )( HWAVEOUT hwo ) ;
	MMRESULT				( WINAPI *waveOutPrepareHeaderFunc )( HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh ) ;
	MMRESULT				( WINAPI *waveOutUnprepareHeaderFunc )( HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh ) ;
	MMRESULT				( WINAPI *waveOutWriteFunc )( HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh ) ;
	MMRESULT				( WINAPI *waveOutResetFunc )( HWAVEOUT hwo ) ;


	HMODULE					Kernel32DLL ;
	HMODULE					( WINAPI *GetModuleHandleAFunc )( LPCSTR lpModuleName ) ;
	HMODULE					( WINAPI *GetModuleHandleWFunc )( LPCWSTR lpModuleName ) ;
	BOOL					( WINAPI *VerifyVersionInfoAFunc )( D_OSVERSIONINFOEXA *lpVersionInformation, DWORD dwTypeMask, DWORDLONG dwlConditionMask ) ;
	BOOL					( WINAPI *VerifyVersionInfoWFunc )( D_OSVERSIONINFOEXW *lpVersionInformation, DWORD dwTypeMask, DWORDLONG dwlConditionMask ) ;
	ULONGLONG				( WINAPI *VerSetConditionMaskFunc )( ULONGLONG ConditionMask, DWORD TypeMask, BYTE Condition ) ;
	HANDLE					( WINAPI *CreateEventAFunc )( LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCSTR lpName ) ;
	BOOL					( WINAPI *SetEventFunc )( HANDLE hEvent ) ;
	BOOL					( WINAPI *ResetEventFunc )( HANDLE hEvent ) ;
	HANDLE					( WINAPI *CreateThreadFunc )( LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId ) ;
	BOOL					( WINAPI *SetThreadPriorityFunc )( HANDLE hThread, int nPriority );
	int						( WINAPI *GetThreadPriorityFunc )( HANDLE hThread ) ;
	VOID					( WINAPI *ExitThreadFunc )( DWORD dwExitCode ) ;
	BOOL					( WINAPI *GetExitCodeThreadFunc )( HANDLE hThread, LPDWORD lpExitCode ) ;
	HANDLE					( WINAPI *GetCurrentThreadFunc )( VOID ) ;
	DWORD					( WINAPI *GetCurrentThreadIdFunc )( VOID ) ;
	DWORD					( WINAPI *SuspendThreadFunc )( HANDLE hThread ) ;
	DWORD					( WINAPI *ResumeThreadFunc )( HANDLE hThread ) ;
	DWORD					( WINAPI *WaitForSingleObjectFunc )( HANDLE hHandle, DWORD dwMilliseconds ) ;
	DWORD					( WINAPI *WaitForMultipleObjectsFunc )( DWORD nCount, CONST HANDLE * lpHandles, BOOL bWaitAll, DWORD dwMilliseconds ) ;
	HANDLE					( WINAPI *GetProcessHeapFunc )( VOID ) ;
	LPVOID					( WINAPI *HeapAllocFunc )( HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes ) ;
	BOOL					( WINAPI *HeapFreeFunc )( HANDLE hHeap, DWORD dwFlags, LPVOID lpMem ) ;
	LPVOID					( WINAPI *VirtualAllocFunc )( LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect ) ;
	BOOL					( WINAPI *VirtualFreeFunc )( LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType ) ;
	SIZE_T					( WINAPI *VirtualQueryFunc )( LPCVOID lpAddress, PMEMORY_BASIC_INFORMATION lpBuffer, SIZE_T dwLength ) ;
	HGLOBAL					( WINAPI *GlobalAllocFunc )( UINT uFlags, SIZE_T dwBytes ) ;
	HGLOBAL					( WINAPI *GlobalFreeFunc )( HGLOBAL hMem ) ;
	LPVOID					( WINAPI *GlobalLockFunc )( HGLOBAL hMem ) ;
	BOOL					( WINAPI *GlobalUnlockFunc )( HGLOBAL hMem ) ;
	SIZE_T					( WINAPI *GlobalSizeFunc )( HGLOBAL hMem ) ;
	HANDLE					( WINAPI *CreateSemaphoreAFunc )( LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, LONG lInitialCount, LONG lMaximumCount, LPCSTR lpName ) ;
	BOOL					( WINAPI *ReleaseSemaphoreFunc )( HANDLE hSemaphore, LONG lReleaseCount, LPLONG lpPreviousCount ) ;
	BOOL					( WINAPI *GetVersionExAFunc )( LPOSVERSIONINFOA lpVersionInformation ) ;
	BOOL					( WINAPI *GetVersionExWFunc )( LPOSVERSIONINFOW lpVersionInformation ) ;
	VOID					( WINAPI *GetSystemInfoFunc )( LPSYSTEM_INFO lpSystemInfo ) ;
	int						( WINAPI *lstrlenAFunc )( LPCSTR lpString ) ;
	int						( WINAPI *lstrlenWFunc )( WORD const *lpString ) ;
	int						( WINAPI *lstrcmpAFunc )( LPCSTR lpString1, LPCSTR lpString2 ) ;
	int						( WINAPI *lstrcmpWFunc )( WORD const * lpString1, WORD const * lpString2 ) ;
	LPSTR					( WINAPI *lstrcpyAFunc )( LPSTR lpString1, LPCSTR lpString2 ) ;
	WORD *					( WINAPI *lstrcpyWFunc )( WORD * lpString1, WORD const * lpString2 ) ;
	LPSTR					( WINAPI *lstrcpynAFunc )( LPSTR lpString1, LPCSTR lpString2, int iMaxLength ) ;
	WORD *					( WINAPI *lstrcpynWFunc )( WORD * lpString1, WORD const * lpString2, int iMaxLength ) ;
	DWORD					( WINAPI *GetTickCountFunc )( VOID ) ;
	BOOL					( WINAPI *QueryPerformanceCounterFunc )( LARGE_INTEGER * lpPerformanceCount ) ;
	BOOL					( WINAPI *QueryPerformanceFrequencyFunc )( LARGE_INTEGER * lpFrequency ) ;
	BOOL					( WINAPI *CloseHandleFunc )( HANDLE hObject ) ;
	DWORD					( WINAPI *GetTempPathAFunc )( DWORD nBufferLength, LPSTR lpBuffer ) ;
	DWORD					( WINAPI *GetTempPathWFunc )( DWORD nBufferLength, LPWSTR lpBuffer ) ;
	UINT					( WINAPI *GetTempFileNameAFunc )( LPCSTR  lpPathName, LPCSTR  lpPrefixString, UINT uUnique, LPSTR  lpTempFileName ) ;
	UINT					( WINAPI *GetTempFileNameWFunc )( LPCWSTR lpPathName, LPCWSTR lpPrefixString, UINT uUnique, LPWSTR lpTempFileName ) ;
	HANDLE					( WINAPI *CreateFileAFunc )( LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile ) ;
	HANDLE					( WINAPI *CreateFileWFunc )( LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile ) ;
	BOOL					( WINAPI *DeleteFileAFunc )( LPCSTR lpFileName ) ;
	BOOL					( WINAPI *DeleteFileWFunc )( LPCWSTR lpFileName ) ;
	DWORD					( WINAPI *GetFileSizeFunc )( HANDLE hFile, LPDWORD lpFileSizeHigh ) ;
	BOOL					( WINAPI *WriteFileFunc )( HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped ) ;
	BOOL					( WINAPI *ReadFileFunc )( HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped ) ;
	DWORD					( WINAPI *SetFilePointerFunc )( HANDLE hFile, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod ) ;
	HANDLE					( WINAPI *FindFirstFileAFunc )( LPCSTR  lpFileName, LPWIN32_FIND_DATAA lpFindFileData ) ;
	HANDLE					( WINAPI *FindFirstFileWFunc )( LPCWSTR lpFileName, LPWIN32_FIND_DATAW lpFindFileData ) ;
	BOOL					( WINAPI *FindNextFileAFunc )( HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData ) ;
	BOOL					( WINAPI *FindNextFileWFunc )( HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData ) ;
	BOOL					( WINAPI *FindCloseFunc )( HANDLE hFindFile ) ;
	BOOL					( WINAPI *SetCurrentDirectoryAFunc )( LPCSTR  lpPathName ) ;
	BOOL					( WINAPI *SetCurrentDirectoryWFunc )( LPCWSTR lpPathName ) ;
	DWORD					( WINAPI *GetCurrentDirectoryAFunc )( DWORD nBufferLength, LPSTR  lpBuffer ) ;
	DWORD					( WINAPI *GetCurrentDirectoryWFunc )( DWORD nBufferLength, LPWSTR lpBuffer ) ;
	VOID					( WINAPI *SleepFunc )( DWORD dwMilliseconds ) ;
	VOID					( WINAPI *InitializeCriticalSectionFunc )( LPCRITICAL_SECTION lpCriticalSection ) ;
	VOID					( WINAPI *DeleteCriticalSectionFunc )( LPCRITICAL_SECTION lpCriticalSection ) ;
	VOID					( WINAPI *EnterCriticalSectionFunc )( LPCRITICAL_SECTION lpCriticalSection ) ;
	VOID					( WINAPI *LeaveCriticalSectionFunc )( LPCRITICAL_SECTION lpCriticalSection ) ;
	VOID					( WINAPI *ExitProcessFunc )( UINT uExitCode ) ;
	HGLOBAL					( WINAPI *LoadResourceFunc )( HMODULE hModule, HRSRC hResInfo ) ;
	HRSRC					( WINAPI *FindResourceAFunc )( HMODULE hModule, LPCSTR  lpName, LPCSTR  lpType ) ;
	HRSRC					( WINAPI *FindResourceWFunc )( HMODULE hModule, LPCWSTR lpName, LPCWSTR lpType ) ;
	LPVOID					( WINAPI *LockResourceFunc )( HGLOBAL hResData ) ;
	DWORD					( WINAPI *SizeofResourceFunc )( HMODULE hModule, HRSRC hResInfo ) ;
	UINT					( WINAPI *GetOEMCPFunc )( void ) ;
	VOID					( WINAPI *OutputDebugStringAFunc )( LPCSTR  lpOutputString ) ;
	VOID					( WINAPI *OutputDebugStringWFunc )( LPCWSTR lpOutputString ) ;
	VOID					( WINAPI *GlobalMemoryStatusFunc )( LPMEMORYSTATUS lpBuffer ) ;
	int						( WINAPI *MulDivFunc )( int nNumber, int nNumerator, int nDenominator ) ;
	VOID					( WINAPI *GetLocalTimeFunc )( LPSYSTEMTIME lpSystemTime ) ;
	BOOL					( WINAPI *GetSystemPowerStatusFunc )( LPSYSTEM_POWER_STATUS lpSystemPowerStatus ) ;
	BOOL					( WINAPI *FileTimeToLocalFileTimeFunc )( FILETIME * lpFileTime, LPFILETIME lpLocalFileTime ) ;
	BOOL					( WINAPI *FileTimeToSystemTimeFunc )( FILETIME * lpFileTime, LPSYSTEMTIME lpSystemTime ) ;
	DWORD					( WINAPI *GetLastErrorFunc )( VOID ) ;
	int						( WINAPI *MultiByteToWideCharFunc )( UINT CodePage, DWORD dwFlags, LPCCH lpMultiByteStr, int cbMultiByte, LPWSTR lpWideCharStr, int cchWideChar ) ;



	HMODULE					Old32DLL ;
	HRESULT					( WINAPI *CoCreateInstanceFunc )( REFCLSID rclsid, D_IUnknown * pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID *ppv ) ;
	LPVOID					( WINAPI *CoTaskMemAllocFunc )( SIZE_T cb ) ;
	void					( WINAPI *CoTaskMemFreeFunc )( LPVOID pv ) ;
	HRESULT					( WINAPI *CoInitializeExFunc )( LPVOID pvReserved, DWORD dwCoInit ) ;
	void					( WINAPI *CoFreeUnusedLibrariesFunc )( void ) ;
	void					( WINAPI *CoUninitializeFunc )( void ) ;
	HRESULT					( WINAPI *CLSIDFromStringFunc )( LPCOLESTR lpsz, LPCLSID pclsid ) ;



	HMODULE					OleAut32DLL ;
	BSTR					( WINAPI *SysAllocStringFunc )( const OLECHAR *psz ) ;
	void					( WINAPI *SysFreeStringFunc )( BSTR bstrString ) ;
	HRESULT					( WINAPI *VariantClearFunc )( VARIANTARG *pvarg ) ;
	void					( WINAPI *VariantInitFunc )( VARIANTARG *pvarg ) ;



	HMODULE					Comctl32DLL ;
	void					( WINAPI *InitCommonControlsFunc )( VOID ) ;

	HMODULE					User32DLL ;
	BOOL					( WINAPI *WINNLSEnableIME_Func )( HWND hwnd, BOOL bFlag ) ;	// WINNLSEnableIME APIのアドレス
	BOOL					( WINAPI *UpdateLayeredWindow )( HWND, HDC, POINT*, SIZE*, HDC, POINT*, COLORREF, BLENDFUNCTION*, DWORD ) ;		// UpdateLayeredWindow のＡＰＩポインタ
	HWND					( WINAPI *CreateWindowExAFunc )( DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam ) ;
	HWND					( WINAPI *CreateWindowExWFunc )( DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam ) ;
	BOOL					( WINAPI *DestroyWindowFunc )( HWND hWnd ) ;
	BOOL					( WINAPI *IsWindowFunc )( HWND hWnd ) ;
	BOOL					( WINAPI *ShowWindowFunc )( HWND hWnd, int nCmdShow ) ;
	BOOL					( WINAPI *UpdateWindowFunc )( HWND hWnd ) ;
	BOOL					( WINAPI *BringWindowToTopFunc )( HWND hWnd ) ;
	BOOL					( WINAPI *SetForegroundWindowFunc )( HWND hWnd ) ;
	DWORD					( WINAPI *GetWindowThreadProcessIdFunc )( HWND hWnd, LPDWORD lpdwProcessId ) ;
	HWND					( WINAPI *SetActiveWindowFunc )( HWND hWnd ) ;
	HWND					( WINAPI *GetForegroundWindowFunc )( VOID ) ;
	HWND					( WINAPI *GetDesktopWindowFunc )( VOID ) ;
	HWND					( WINAPI *FindWindowAFunc )( LPCSTR lpClassName, LPCSTR lpWindowName ) ;
	HWND					( WINAPI *FindWindowWFunc )( LPCWSTR lpClassName, LPCWSTR lpWindowName ) ;
	BOOL					( WINAPI *MoveWindowFunc )( HWND hWnd, int X, int Y, int nWidth, int nHeight, BOOL bRepaint ) ;
	BOOL					( WINAPI *SetWindowPosFunc )( HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags ) ;
	BOOL					( WINAPI *GetClientRectFunc )( HWND hWnd, LPRECT lpRect ) ;
	BOOL					( WINAPI *AdjustWindowRectExFunc )( LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle ) ;
	BOOL					( WINAPI *GetWindowRectFunc )( HWND hWnd, LPRECT lpRect ) ;
	LONG					( WINAPI *SetWindowLongAFunc )( HWND hWnd, int nIndex, LONG dwNewLong ) ;
	LONG					( WINAPI *SetWindowLongWFunc )( HWND hWnd, int nIndex, LONG dwNewLong ) ;
	LONG_PTR				( WINAPI *SetWindowLongPtrAFunc )( HWND hWnd, int nIndex, LONG_PTR dwNewLong ) ;
	LONG_PTR				( WINAPI *SetWindowLongPtrWFunc )( HWND hWnd, int nIndex, LONG_PTR dwNewLong ) ;
	LONG					( WINAPI *GetWindowLongAFunc )( HWND hWnd, int nIndex ) ;
	LONG					( WINAPI *GetWindowLongWFunc )( HWND hWnd, int nIndex ) ;
	LONG_PTR				( WINAPI *GetWindowLongPtrAFunc )( HWND hWnd, int nIndex ) ;
	LONG_PTR				( WINAPI *GetWindowLongPtrWFunc )( HWND hWnd, int nIndex ) ;
	BOOL					( WINAPI *SetWindowTextAFunc )( HWND hWnd, LPCSTR lpString ) ;
	BOOL					( WINAPI *SetWindowTextWFunc )( HWND hWnd, LPCWSTR lpString ) ;
	BOOL					( WINAPI *ClientToScreenFunc )( HWND hWnd, LPPOINT lpPoint ) ;
	BOOL					( WINAPI *EnumDisplayDevicesAFunc )( LPCSTR lpDevice, DWORD iDevNum, D_DISPLAY_DEVICEA *lpDisplayDevice, DWORD dwFlags ) ;
	BOOL					( WINAPI *EnumDisplayDevicesWFunc )( LPCWSTR lpDevice, DWORD iDevNum, D_DISPLAY_DEVICEW *lpDisplayDevice, DWORD dwFlags ) ;
	BOOL					( WINAPI *CloseTouchInputHandleFunc )( D_HTOUCHINPUT hTouchInput ) ;
	BOOL					( WINAPI *GetTouchInputInfoFunc )( D_HTOUCHINPUT hTouchInput, UINT cInputs, D_PTOUCHINPUT pInputs, int cbSize ) ;
	BOOL					( WINAPI *IsTouchWindowFunc )( HWND hWnd, ULONG *pulFlags ) ;
	BOOL					( WINAPI *RegisterTouchWindowFunc )( HWND hWnd, ULONG ulFlags ) ;
	BOOL					( WINAPI *UnregisterTouchWindowFunc )( HWND hWnd ) ;
	BOOL					( WINAPI *ShutdownBlockReasonCreateFunc )( HWND hWnd, LPCWSTR pwszReason ) ;
	BOOL					( WINAPI *ShutdownBlockReasonDestroyFunc )( HWND hWnd ) ;
	UINT_PTR				( WINAPI *SetTimerFunc )( HWND hWnd, UINT_PTR nIDEvent, UINT uElapse, D_TIMERPROC lpTimerFunc ) ;
	BOOL					( WINAPI *PostThreadMessageAFunc )( DWORD idThread, UINT Msg, WPARAM wParam, LPARAM lParam ) ;
	BOOL					( WINAPI *PostThreadMessageWFunc )( DWORD idThread, UINT Msg, WPARAM wParam, LPARAM lParam ) ;
	BOOL					( WINAPI *PeekMessageAFunc )( LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg ) ;
	BOOL					( WINAPI *PeekMessageWFunc )( LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg ) ;
	BOOL					( WINAPI *GetMessageAFunc )( LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax ) ;
	BOOL					( WINAPI *GetMessageWFunc )( LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax ) ;
	LRESULT					( WINAPI *SendMessageAFunc )( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam ) ;
	LRESULT					( WINAPI *SendMessageWFunc )( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam ) ;
	BOOL					( WINAPI *PostMessageAFunc )( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam ) ;
	BOOL					( WINAPI *PostMessageWFunc )( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam ) ;
	VOID					( WINAPI *PostQuitMessageFunc )( int nExitCode ) ;
	LRESULT					( WINAPI *DefWindowProcAFunc )( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam ) ;
	LRESULT					( WINAPI *DefWindowProcWFunc )( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam ) ;
	int						( WINAPI *TranslateAcceleratorAFunc )( HWND hWnd, HACCEL hAccTable, LPMSG lpMsg ) ;
	int						( WINAPI *TranslateAcceleratorWFunc )( HWND hWnd, HACCEL hAccTable, LPMSG lpMsg ) ;
	BOOL					( WINAPI *TranslateMessageFunc )( MSG *lpMsg ) ;
	LRESULT					( WINAPI *DispatchMessageAFunc )( MSG *lpMsg ) ;
	LRESULT					( WINAPI *DispatchMessageWFunc )( MSG *lpMsg ) ;
	DWORD					( WINAPI *MsgWaitForMultipleObjectsFunc )( DWORD nCount, CONST HANDLE *pHandles, BOOL fWaitAll, DWORD dwMilliseconds, DWORD dwWakeMask ) ;
	DWORD					( WINAPI *GetQueueStatusFunc )( UINT flags ) ;
	UINT					( WINAPI *RegisterWindowMessageAFunc )( LPCSTR lpString ) ;
	UINT					( WINAPI *RegisterWindowMessageWFunc )( LPCWSTR lpString ) ;
	ATOM					( WINAPI *RegisterClassAFunc )( CONST WNDCLASSA *lpWndClass ) ;
	ATOM					( WINAPI *RegisterClassWFunc )( CONST WNDCLASSW *lpWndClass ) ;
	ATOM					( WINAPI *RegisterClassExAFunc )( CONST WNDCLASSEXA * ) ;
	ATOM					( WINAPI *RegisterClassExWFunc )( CONST WNDCLASSEXW * ) ;
	BOOL					( WINAPI *UnregisterClassAFunc )( LPCSTR lpClassName, HINSTANCE hInstance ) ;
	BOOL					( WINAPI *UnregisterClassWFunc )( LPCWSTR lpClassName, HINSTANCE hInstance ) ;
	DWORD					( WINAPI *SetClassLongAFunc )( HWND hWnd, int nIndex, LONG dwNewLong ) ;
	DWORD					( WINAPI *SetClassLongWFunc )( HWND hWnd, int nIndex, LONG dwNewLong ) ;
	unsigned LONG_PTR		( WINAPI *SetClassLongPtrAFunc )( HWND hWnd, int nIndex, LONG_PTR dwNewLong ) ;
	unsigned LONG_PTR		( WINAPI *SetClassLongPtrWFunc )( HWND hWnd, int nIndex, LONG_PTR dwNewLong ) ;
	HDC						( WINAPI *GetDCFunc )( HWND hWnd ) ;
	int						( WINAPI *ReleaseDCFunc )( HWND hWnd, HDC hDC ) ;
	HDC						( WINAPI *BeginPaintFunc )( HWND hWnd, LPPAINTSTRUCT lpPaint ) ;
	BOOL					( WINAPI *EndPaintFunc )( HWND hWnd, PAINTSTRUCT *lpPaint ) ;
	int						( WINAPI *MessageBoxAFunc )( HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType ) ;
	int						( WINAPI *MessageBoxWFunc )( HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType ) ;
	int						( WINAPI *GetSystemMetricsFunc )( int nIndex ) ;
	LONG					( WINAPI *ChangeDisplaySettingsAFunc )( D_DEVMODEA* lpDevMode, DWORD dwFlags ) ;
	LONG					( WINAPI *ChangeDisplaySettingsWFunc )( D_DEVMODEW* lpDevMode, DWORD dwFlags ) ;
	int						( WINAPI *ShowCursorFunc )( BOOL bShow ) ;
	BOOL					( WINAPI *SetCursorPosFunc )( int X, int Y ) ;
	BOOL					( WINAPI *GetCursorPosFunc )( LPPOINT lpPoint ) ;
	HCURSOR					( WINAPI *SetCursorFunc )( HCURSOR hCursor ) ;
	HCURSOR					( WINAPI *LoadCursorAFunc )( HINSTANCE hInstance, LPCSTR  lpCursorName ) ;
	HCURSOR					( WINAPI *LoadCursorWFunc )( HINSTANCE hInstance, LPCWSTR lpCursorName ) ;
	HICON					( WINAPI *LoadIconAFunc )( HINSTANCE hInstance, LPCSTR  lpIconName ) ;
	HICON					( WINAPI *LoadIconWFunc )( HINSTANCE hInstance, LPCWSTR lpIconName ) ;
	BOOL					( WINAPI *ClipCursorFunc )( CONST RECT *lpRect ) ;
	HMENU					( WINAPI *LoadMenuAFunc )( HINSTANCE hInstance, LPCSTR  lpMenuName ) ;
	HMENU					( WINAPI *LoadMenuWFunc )( HINSTANCE hInstance, LPCWSTR lpMenuName ) ;
	HMENU					( WINAPI *CreateMenuFunc )( VOID ) ;
	BOOL					( WINAPI *DeleteMenuFunc )( HMENU hMenu, UINT uPosition, UINT uFlags ) ;
	BOOL					( WINAPI *DestroyMenuFunc )( HMENU hMenu ) ;
	BOOL					( WINAPI *EnableMenuItemFunc )( HMENU hMenu, UINT uIDEnableItem, UINT uEnable ) ;
	HMENU					( WINAPI *GetSubMenuFunc )( HMENU hMenu, int nPos ) ;
	BOOL					( WINAPI *SetMenuItemInfoAFunc )( HMENU hmenu, UINT item, BOOL fByPositon, LPCMENUITEMINFOA lpmii ) ;
	BOOL					( WINAPI *SetMenuItemInfoWFunc )( HMENU hmenu, UINT item, BOOL fByPositon, LPCMENUITEMINFOW lpmii ) ;
	BOOL					( WINAPI *GetMenuItemInfoAFunc )( HMENU hmenu, UINT item, BOOL fByPosition, LPMENUITEMINFOA lpmii ) ;
	BOOL					( WINAPI *GetMenuItemInfoWFunc )( HMENU hmenu, UINT item, BOOL fByPosition, LPMENUITEMINFOW lpmii ) ;
	BOOL					( WINAPI *SetMenuFunc )( HWND hWnd, HMENU hMenu ) ;
	BOOL					( WINAPI *DrawMenuBarFunc )( HWND hWnd ) ;
	int						( WINAPI *GetMenuItemCountFunc )( HMENU hMenu ) ;
	BOOL					( WINAPI *InsertMenuItemAFunc )( HMENU hmenu, UINT item, BOOL fByPosition, LPCMENUITEMINFOA lpmi ) ;
	BOOL					( WINAPI *InsertMenuItemWFunc )( HMENU hmenu, UINT item, BOOL fByPosition, LPCMENUITEMINFOW lpmi ) ;
	int						( WINAPI *SetWindowRgnFunc )( HWND hWnd, HRGN hRgn, BOOL bRedraw ) ;
	int						( WINAPI *FillRectFunc )( HDC hDC, CONST RECT *lprc, HBRUSH hbr ) ;
	BOOL					( WINAPI *SystemParametersInfoAFunc )( UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni ) ;
	BOOL					( WINAPI *SystemParametersInfoWFunc )( UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni ) ;
	BOOL					( WINAPI *AttachThreadInputFunc )( DWORD idAttach, DWORD idAttachTo, BOOL fAttach ) ;
#ifndef DX_NON_STOPTASKSWITCH
	HHOOK					( WINAPI *SetWindowsHookExWFunc )( int idHook, HOOKPROC lpfn, HINSTANCE hmod, DWORD dwThreadId ) ;
	BOOL					( WINAPI *UnhookWindowsHookExFunc )( HHOOK hhk ) ;
#endif // DX_NON_STOPTASKSWITCH
	BOOL					( WINAPI *GetKeyboardStateFunc )( PBYTE lpKeyState ) ;
	BOOL					( WINAPI *KillTimerFunc )( HWND hWnd, UINT_PTR uIDEvent ) ;
	BOOL					( WINAPI *EnumDisplaySettingsAFunc )( LPCSTR  lpszDeviceName, DWORD iModeNum, D_DEVMODEA* lpDevMode ) ;
	BOOL					( WINAPI *EnumDisplaySettingsWFunc )( LPCWSTR lpszDeviceName, DWORD iModeNum, D_DEVMODEW* lpDevMode ) ;
	BOOL					( WINAPI *EnumDisplayMonitorsFunc )( HDC hdc, LPCRECT lprcClip, MONITORENUMPROC  lpfnEnum, LPARAM dwData ) ;
	BOOL					( WINAPI *IsDialogMessageAFunc )( HWND hDlg, LPMSG lpMsg ) ;
	BOOL					( WINAPI *IsDialogMessageWFunc )( HWND hDlg, LPMSG lpMsg ) ;
	BOOL					( WINAPI *OpenClipboardFunc )( HWND hWndNewOwner ) ;
	BOOL					( WINAPI *CloseClipboardFunc )( VOID ) ;
	BOOL					( WINAPI *EmptyClipboardFunc )( VOID ) ;
	HANDLE					( WINAPI *SetClipboardDataFunc )( UINT uFormat, HANDLE hMem ) ;
	HANDLE					( WINAPI *GetClipboardDataFunc )( UINT uFormat ) ;
	BOOL					( WINAPI *IsClipboardFormatAvailableFunc )( UINT format ) ;
	HACCEL					( WINAPI *CreateAcceleratorTableAFunc )( LPACCEL paccel, int cAccel ) ;
	HACCEL					( WINAPI *CreateAcceleratorTableWFunc )( LPACCEL paccel, int cAccel ) ;
	BOOL					( WINAPI *DestroyAcceleratorTableFunc )( HACCEL hAccel ) ;
	int						( WINAPI *CopyAcceleratorTableAFunc )( HACCEL hAccelSrc, LPACCEL lpAccelDst, int cAccelEntries ) ;
	int						( WINAPI *CopyAcceleratorTableWFunc )( HACCEL hAccelSrc, LPACCEL lpAccelDst, int cAccelEntries ) ;
	BOOL					( WINAPI *GetMonitorInfoAFunc )( HMONITOR hMonitor, D_MONITORINFO *lpmi ) ;
	BOOL					( WINAPI *GetMonitorInfoWFunc )( HMONITOR hMonitor, D_MONITORINFO *lpmi ) ;
	DWORD					( WINAPI *CharLowerBuffAFunc )( LPSTR  lpsz, DWORD cchLength ) ;
	DWORD					( WINAPI *CharLowerBuffWFunc )( LPWSTR lpsz, DWORD cchLength ) ;
	UINT					( WINAPI *GetRawInputDeviceListFunc )( D_RAWINPUTDEVICELIST *pRawInputDeviceList, PUINT puiNumDevices, UINT cbSize ) ;
	UINT					( WINAPI *GetRawInputDeviceInfoAFunc )( HANDLE hDevice, UINT uiCommand, LPVOID pData, PUINT pcbSize ) ;


	HMODULE					GDI32DLL ;
	HANDLE					( WINAPI *AddFontMemResourceExFunc )( LPVOID pbFont, DWORD cbFont, void /* DESIGNVECTOR */ *pdv, DWORD *pcFonts ) ;
	int						( WINAPI *RemoveFontMemResourceExFunc )( HANDLE fh ) ;
	DWORD					( WINAPI *GetFontUnicodeRangesFunc )( HDC hdc, D_LPGLYPHSET lpgs ) ;
	int						( WINAPI *GetDeviceCapsFunc )( HDC hdc, int index ) ;
	int						( WINAPI *EnumFontFamiliesExAFunc )( HDC hdc, LPLOGFONTA lpLogfont, FONTENUMPROCA lpProc, LPARAM lParam, DWORD dwFlags ) ;
	int						( WINAPI *EnumFontFamiliesExWFunc )( HDC hdc, LPLOGFONTW lpLogfont, FONTENUMPROCW lpProc, LPARAM lParam, DWORD dwFlags ) ;
	HFONT					( WINAPI *CreateFontAFunc )( int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic, DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName ) ;
	HFONT					( WINAPI *CreateFontWFunc )( int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic, DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCWSTR pszFaceName ) ;
	BOOL					( WINAPI *GetTextMetricsAFunc )( HDC hdc, LPTEXTMETRICA lptm ) ;
	BOOL					( WINAPI *GetTextMetricsWFunc )( HDC hdc, LPTEXTMETRICW lptm ) ;
	BOOL					( WINAPI *GetTextExtentPoint32AFunc )( HDC hdc, LPCSTR  lpString, int c, LPSIZE psizl ) ;
	BOOL					( WINAPI *GetTextExtentPoint32WFunc )( HDC hdc, LPCWSTR lpString, int c, LPSIZE psizl ) ;
	DWORD					( WINAPI *GetCharacterPlacementAFunc )( HDC hdc, LPCSTR  lpString, int nCount, int nMexExtent, LPGCP_RESULTSA lpResults, DWORD dwFlags ) ;
	DWORD					( WINAPI *GetCharacterPlacementWFunc )( HDC hdc, LPCWSTR lpString, int nCount, int nMexExtent, LPGCP_RESULTSW lpResults, DWORD dwFlags ) ;
	DWORD					( WINAPI *GetGlyphOutlineAFunc )( HDC hdc, UINT uChar, UINT fuFormat, LPGLYPHMETRICS lpgm, DWORD cjBuffer, LPVOID pvBuffer, CONST MAT2 *lpmat2 ) ;
	DWORD					( WINAPI *GetGlyphOutlineWFunc )( HDC hdc, UINT uChar, UINT fuFormat, LPGLYPHMETRICS lpgm, DWORD cjBuffer, LPVOID pvBuffer, CONST MAT2 *lpmat2 ) ;
	BOOL					( WINAPI *TextOutAFunc )( HDC hdc, int x, int y, LPCSTR  lpString, int c ) ;
	BOOL					( WINAPI *TextOutWFunc )( HDC hdc, int x, int y, LPCWSTR lpString, int c ) ;
	COLORREF				( WINAPI *SetTextColorFunc )( HDC hdc, COLORREF color ) ;
	COLORREF				( WINAPI *SetBkColorFunc )( HDC hdc, COLORREF color ) ;
	int						( WINAPI *SetBkModeFunc )( HDC hdc, int mode ) ;
	HDC						( WINAPI *CreateCompatibleDCFunc )( HDC hdc ) ;
	HDC						( WINAPI *CreateDCAFunc )( LPCSTR  pwszDriver, LPCSTR  pwszDevice, LPCSTR  pszPort, CONST D_DEVMODEA * pdm ) ;
	HDC						( WINAPI *CreateDCWFunc )( LPCWSTR pwszDriver, LPCWSTR pwszDevice, LPCWSTR pszPort, CONST D_DEVMODEW * pdm ) ;
	int						( WINAPI *GetObjectAFunc )( HANDLE h, int c, LPVOID pv ) ;
	int						( WINAPI *GetObjectWFunc )( HANDLE h, int c, LPVOID pv ) ;
	HGDIOBJ					( WINAPI *GetStockObjectFunc )( int i ) ;
	HGDIOBJ					( WINAPI *SelectObjectFunc )( HDC hdc, HGDIOBJ h ) ;
	BOOL					( WINAPI *DeleteObjectFunc )( HGDIOBJ ho ) ;
	BOOL					( WINAPI *DeleteDCFunc )( HDC hdc ) ;
	HBRUSH					( WINAPI *CreateSolidBrushFunc )( COLORREF color ) ;
	BOOL					( WINAPI *RectangleFunc )( HDC hdc, int left, int top, int right, int bottom ) ;
	BOOL					( WINAPI *BitBltFunc )( HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop ) ;
	HBITMAP					( WINAPI *CreateDIBSectionFunc )( HDC hdc, CONST BITMAPINFO *pbmi, UINT usage, VOID **ppvBits, HANDLE hSection, DWORD offset ) ;
	int						( WINAPI *StretchDIBitsFunc )( HDC hdc, int xDest, int yDest, int DestWidth, int DestHeight, int xSrc, int ySrc, int SrcWidth, int SrcHeight, CONST VOID * lpBits, CONST BITMAPINFO * lpbmi, UINT iUsage, DWORD rop ) ;
	int						( WINAPI *SetDIBitsToDeviceFunc )( HDC hdc, int xDest, int yDest, DWORD w, DWORD h, int xSrc, int ySrc, UINT StartScan, UINT cLines, CONST VOID * lpvBits, CONST BITMAPINFO * lpbmi, UINT ColorUse ) ;
	HRGN					( WINAPI *CreateRectRgnFunc )( int x1, int y1, int x2, int y2 ) ;
	int						( WINAPI *CombineRgnFunc )( HRGN hrgnDst, HRGN hrgnSrc1, HRGN hrgnSrc2, int iMode ) ;
	DWORD					( WINAPI *GetKerningPairsFunc )( HDC hdc, DWORD nPairs, LPKERNINGPAIR lpKernPair ) ;


	HMODULE					ADVAPI32DLL ;
	LONG					( WINAPI *RegOpenKeyAFunc )( HKEY hKey, LPCSTR  lpSubKey, PHKEY phkResult ) ;
	LONG					( WINAPI *RegOpenKeyWFunc )( HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult ) ;
	LONG					( WINAPI *RegEnumKeyAFunc )( HKEY hKey, DWORD dwIndex, LPSTR  lpName, DWORD cchName ) ;
	LONG					( WINAPI *RegEnumKeyWFunc )( HKEY hKey, DWORD dwIndex, LPWSTR lpName, DWORD cchName ) ;
	LONG					( WINAPI *RegOpenKeyExAFunc )( HKEY hKey, LPCSTR  lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult ) ;
	LONG					( WINAPI *RegOpenKeyExWFunc )( HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult ) ;
	LONG					( WINAPI *RegQueryValueExAFunc )( HKEY hKey, LPCSTR  lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData ) ;
	LONG					( WINAPI *RegQueryValueExWFunc )( HKEY hKey, LPCWSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData ) ;
  

	HMODULE					SHELL32DLL ;
	UINT					( WINAPI *DragQueryFileAFunc )( HDROP hDrop, UINT iFile, LPSTR  lpszFile, UINT cch ) ;
	UINT					( WINAPI *DragQueryFileWFunc )( HDROP hDrop, UINT iFile, LPWSTR lpszFile, UINT cch ) ;
	void					( WINAPI *DragFinishFunc )( HDROP hDrop ) ;
	void					( WINAPI *DragAcceptFilesFunc )( HWND hWnd, BOOL fAccept ) ;


	HMODULE					NTDLL ;
	void					( WINAPI *RtlGetVersionFunc )( D_OSVERSIONINFOEXW *lpVersionInfomation ) ;


	HMODULE					MFPLATDLL ;
	HRESULT					( WINAPI *MFStartupFunc )( ULONG Version, DWORD dwFlags ) ;
	HRESULT					( WINAPI *MFShutdownFunc )( void ) ;
	HRESULT					( WINAPI *MFCreateMediaTypeFunc )( D_IMFMediaType **ppMFType ) ;
	HRESULT					( WINAPI *MFCreateWaveFormatExFromMFMediaTypeFunc )( D_IMFMediaType *pMFType, WAVEFORMATEX **ppWF, UINT32 *pcbSize, UINT32 Flags ) ;
	HRESULT					( WINAPI *MFCreateAttributesFunc )( D_IMFAttributes** ppMFAttributes, UINT32 cInitialSize ) ;
	HRESULT					( WINAPI *MFCreateAsyncResultFunc )( D_IUnknown * punkObject, D_IMFAsyncCallback * pCallback, D_IUnknown * punkState, D_IMFAsyncResult ** ppAsyncResult ) ;
	HRESULT					( WINAPI *MFInvokeCallbackFunc )( D_IMFAsyncResult * pAsyncResult ) ;


	HMODULE					MFREADWRITEDLL ;
	HRESULT					( WINAPI *MFCreateSourceReaderFromURLFunc )( LPCWSTR pwszURL, D_IMFAttributes *pAttributes, D_IMFSourceReader **ppSourceReader ) ;
	HRESULT					( WINAPI *MFCreateSourceReaderFromByteStreamFunc )( D_IMFByteStream *pByteStream, D_IMFAttributes *pAttributes, D_IMFSourceReader **ppSourceReader ) ;


	HMODULE					PROPSYSDLL ;
	HRESULT					( WINAPI *PropVariantToInt64Func )( const D_PROPVARIANT &propvarIn, LONGLONG *pllRet ) ;


	HMODULE					USP10DLL ;
	HRESULT					( WINAPI *ScriptItemizeFunc )( const WCHAR *pwcInChars, int cInChars, int cMaxItems, const D_SCRIPT_CONTROL *psControl, const D_SCRIPT_STATE *psState, D_SCRIPT_ITEM *pItems, int *pcItems ) ;
	HRESULT					( WINAPI *ScriptShapeFunc )( HDC hdc, void **psc, const WCHAR *pwcChars, int cChars, int cMaxGlyphs, D_SCRIPT_ANALYSIS *psa, WORD *pwOutGlyphs, WORD *pwLogClust, D_SCRIPT_VISATTR *psva, int *pcGlyphs ) ;


	HMODULE					SHCOREDLL ;
	HRESULT					( WINAPI *SetProcessDpiAwarenessFunc )( D_PROCESS_DPI_AWARENESS value ) ;
	HRESULT					( WINAPI *GetProcessDpiAwarenessFunc )( HANDLE hprocess, D_PROCESS_DPI_AWARENESS *value ) ;
	HRESULT					( WINAPI *GetDpiForMonitorFunc )( HMONITOR hmonitor, D_MONITOR_DPI_TYPE dpiType, UINT *dpiX, UINT *dpiY ) ;


	HMODULE					D3DX9DLL ;
	HRESULT					( WINAPI *D3DXCompileShaderFunc )( LPCTSTR pSrcData, UINT srcDataLen, CONST D_D3DXMACRO* pDefines, D_ID3DInclude *pInclude, LPCSTR pFunctionName, LPCSTR pProfile, DWORD Flags, D_ID3DXBuffer **ppShader, D_ID3DXBuffer **ppErrorMsgs, D_ID3DXConstantTable **ppConstantTable ) ;


	HMODULE					D3DCOMPILERDLL ;
	HRESULT					( WINAPI *D3DCompileFunc )( LPCVOID pSrcData, SIZE_T SrcDataSize, LPCSTR pFileName, CONST D_D3DXMACRO *pDefines, D_ID3DInclude *pInclude, LPCSTR pEntrypoint, LPCSTR pTarget, UINT Flags1, UINT Flags2, D_ID3DBlob **ppCode, D_ID3DBlob **ppErrorMsgs ) ;

	HMODULE					Ole32DLL ;
	HRESULT					( WINAPI *CoSetProxyBlanketFunc )( D_IUnknown *pProxy, DWORD dwAuthnSvc, DWORD dwAuthzSvc, OLECHAR *pServerPrincName, DWORD dwAuthnLevel, DWORD dwImpLevel, RPC_AUTH_IDENTITY_HANDLE pAuthInfo, DWORD dwCapabilities );
	HRESULT					( WINAPI *PropVariantClearFunc )( D_PROPVARIANT *pvar ) ;
} ;

// WinAPI 情報構造体
struct WINAPIDATA
{
	volatile int			InitializeFlag ;
	volatile int			EnableWideCharaFunction ;			// Wide文字の関数が有効かどうか

#ifndef DX_NON_NETWORK
	WINSOCKFUNCTION			WinSockFunc ;						// WinSock API 関係のデータ
#endif

#ifndef DX_NON_KEYEX
	IMMFUNCTION				ImmFunc ;							// IMM API 関係のデータ
#endif

	WIN32APIFUNCTION		Win32Func ;							// Win32 API 関係のデータ

	HMODULE					DwmApiDLL ;							// Desktop Window Manager API DLL
	HRESULT					( WINAPI *DF_DwmEnableComposition )( UINT uCompositionAction ) ;	// DwmEnableComposition API のアドレス
	HRESULT					( WINAPI *DwmGetWindowAttributeFunc )( HWND hwnd, DWORD dwAttribute, PVOID pvAttribute, DWORD cbAttribute ) ;
} ;

// 内部大域変数宣言 --------------------------------------------------------------

extern WINAPIDATA WinAPIData ;

// 関数プロトタイプ宣言-----------------------------------------------------------

extern int LoadWinAPI( void ) ;				// WindowsOS の DLL を読み込む
extern int ReleaseWinAPI( void ) ;			// WindowsOS の DLL を解放する

extern HRESULT WinAPI_CoCreateInstance_ASync( REFCLSID rclsid, D_IUnknown * pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID *ppv, int ASyncThread = FALSE ) ;

extern int LoadDirect3DShaderCompiler( void ) ;					// Direct3D 9 用シェーダーコンパイラーを読み込む
extern int LoadDirect3D11ShaderCompiler( void ) ;				// Direct3D 11 用シェーダーコンパイラーを読み込む

// ラッパー関数
extern	HMODULE				WINAPI GetModuleHandleWFunc( LPCWSTR lpModuleName ) ;
extern	BOOL				WINAPI VerifyVersionInfoWFunc( D_OSVERSIONINFOEXW *lpVersionInformation, DWORD dwTypeMask, DWORDLONG dwlConditionMask ) ;
extern	BOOL				WINAPI GetVersionExWFunc( LPOSVERSIONINFOW lpVersionInformation ) ;
extern	int					WINAPI lstrlenWFunc( WORD const * lpString ) ;
extern	int					WINAPI lstrcmpWFunc( WORD const * lpString1, WORD const * lpString2 ) ;
extern	WORD *				WINAPI lstrcpyWFunc( WORD * lpString1, WORD const * lpString2 ) ;
extern	WORD *				WINAPI lstrcpynWFunc( WORD * lpString1, WORD const * lpString2, int iMaxLength ) ;
extern	DWORD				WINAPI GetTempPathWFunc( DWORD nBufferLength, LPWSTR lpBuffer ) ;
extern	UINT				WINAPI GetTempFileNameWFunc( LPCWSTR lpPathName, LPCWSTR lpPrefixString, UINT uUnique, LPWSTR lpTempFileName ) ;
extern	HANDLE				WINAPI CreateFileWFunc( LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile ) ;
extern	BOOL				WINAPI DeleteFileWFunc( LPCWSTR lpFileName ) ;
extern	HANDLE				WINAPI FindFirstFileWFunc( LPCWSTR lpFileName, LPWIN32_FIND_DATAW lpFindFileData ) ;
extern	BOOL				WINAPI FindNextFileWFunc( HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData ) ;
extern	BOOL				WINAPI SetCurrentDirectoryWFunc( LPCWSTR lpPathName ) ;
extern	DWORD				WINAPI GetCurrentDirectoryWFunc( DWORD nBufferLength, LPWSTR lpBuffer ) ;
extern	HRSRC				WINAPI FindResourceWFunc( HMODULE hModule, LPCWSTR lpName, LPCWSTR lpType ) ;
extern	VOID				WINAPI OutputDebugStringWFunc( LPCWSTR lpOutputString ) ;
extern	HWND				WINAPI CreateWindowExWFunc( DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam ) ;
extern	HWND				WINAPI FindWindowWFunc( LPCWSTR lpClassName, LPCWSTR lpWindowName ) ;
extern	LONG				WINAPI SetWindowLongWFunc( HWND hWnd, int nIndex, LONG dwNewLong ) ;
extern	LONG_PTR			WINAPI SetWindowLongPtrWFunc( HWND hWnd, int nIndex, LONG_PTR dwNewLong ) ;
extern	LONG				WINAPI GetWindowLongWFunc( HWND hWnd, int nIndex ) ;
extern	LONG_PTR			WINAPI GetWindowLongPtrWFunc( HWND hWnd, int nIndex ) ;
extern	BOOL				WINAPI SetWindowTextWFunc( HWND hWnd, LPCWSTR lpString ) ;
extern	BOOL				WINAPI EnumDisplayDevicesWFunc( LPCWSTR lpDevice, DWORD iDevNum, D_DISPLAY_DEVICEW *lpDisplayDevice, DWORD dwFlags ) ;
extern	BOOL				WINAPI PostThreadMessageWFunc( DWORD idThread, UINT Msg, WPARAM wParam, LPARAM lParam ) ;
extern	BOOL				WINAPI PeekMessageWFunc( LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg ) ;
extern	BOOL				WINAPI GetMessageWFunc( LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax ) ;
extern	LRESULT				WINAPI SendMessageWFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam ) ;
extern	BOOL				WINAPI PostMessageWFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam ) ;
extern	LRESULT				WINAPI DefWindowProcWFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam ) ;
extern	int					WINAPI TranslateAcceleratorWFunc( HWND hWnd, HACCEL hAccTable, LPMSG lpMsg ) ;
extern	LRESULT				WINAPI DispatchMessageWFunc( MSG *lpMsg ) ;
extern	UINT				WINAPI RegisterWindowMessageWFunc( LPCWSTR lpString ) ;
extern	ATOM				WINAPI RegisterClassWFunc( CONST WNDCLASSW *lpWndClass ) ;
extern	ATOM				WINAPI RegisterClassExWFunc( CONST WNDCLASSEXW * ) ;
extern	BOOL				WINAPI UnregisterClassWFunc( LPCWSTR lpClassName, HINSTANCE hInstance ) ;
extern	DWORD				WINAPI SetClassLongWFunc( HWND hWnd, int nIndex, LONG dwNewLong ) ;
extern	unsigned LONG_PTR	WINAPI SetClassLongPtrWFunc( HWND hWnd, int nIndex, LONG_PTR dwNewLong ) ;
extern	int					WINAPI MessageBoxWFunc( HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType ) ;
extern	LONG				WINAPI ChangeDisplaySettingsWFunc( D_DEVMODEW* lpDevMode, DWORD dwFlags ) ;
extern	HCURSOR				WINAPI LoadCursorWFunc( HINSTANCE hInstance, LPCWSTR lpCursorName ) ;
extern	HICON				WINAPI LoadIconWFunc( HINSTANCE hInstance, LPCWSTR lpIconName ) ;
extern	HMENU				WINAPI LoadMenuWFunc( HINSTANCE hInstance, LPCWSTR lpMenuName ) ;
extern	BOOL				WINAPI SetMenuItemInfoWFunc( HMENU hmenu, UINT item, BOOL fByPositon, LPCMENUITEMINFOW lpmii ) ;
extern	BOOL				WINAPI GetMenuItemInfoWFunc( HMENU hmenu, UINT item, BOOL fByPosition, LPMENUITEMINFOW lpmii ) ;
extern	BOOL				WINAPI InsertMenuItemWFunc( HMENU hmenu, UINT item, BOOL fByPosition, LPCMENUITEMINFOW lpmi ) ;
extern	BOOL				WINAPI SystemParametersInfoWFunc( UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni ) ;
extern	BOOL				WINAPI EnumDisplaySettingsWFunc( LPCWSTR lpszDeviceName, DWORD iModeNum, D_DEVMODEW* lpDevMode ) ;
extern	BOOL				WINAPI IsDialogMessageWFunc( HWND hDlg, LPMSG lpMsg ) ;
extern	HACCEL				WINAPI CreateAcceleratorTableWFunc( LPACCEL paccel, int cAccel ) ;
extern	int					WINAPI CopyAcceleratorTableWFunc( HACCEL hAccelSrc, LPACCEL lpAccelDst, int cAccelEntries ) ;
extern	BOOL				WINAPI GetMonitorInfoWFunc( HMONITOR hMonitor, D_MONITORINFO * lpmi ) ;
extern	DWORD				WINAPI CharLowerBuffWFunc( LPWSTR lpsz, DWORD cchLength ) ;
extern	int					WINAPI EnumFontFamiliesExWFunc( HDC hdc, LPLOGFONTW lpLogfont, FONTENUMPROCW lpProc, LPARAM lParam, DWORD dwFlags ) ;
extern	HFONT				WINAPI CreateFontWFunc( int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic, DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCWSTR pszFaceName ) ;
extern	BOOL				WINAPI GetTextMetricsWFunc( HDC hdc, LPTEXTMETRICW lptm ) ;
extern	BOOL				WINAPI GetTextExtentPoint32WFunc( HDC hdc, LPCWSTR lpString, int c, LPSIZE psizl ) ;
extern	DWORD				WINAPI GetCharacterPlacementWFunc( HDC hdc, LPCWSTR lpString, int nCount, int nMexExtent, LPGCP_RESULTSW lpResults, DWORD dwFlags ) ;
extern	DWORD				WINAPI GetGlyphOutlineWFunc( HDC hdc, UINT uChar, UINT fuFormat, LPGLYPHMETRICS lpgm, DWORD cjBuffer, LPVOID pvBuffer, CONST MAT2 *lpmat2 ) ;
extern	BOOL				WINAPI TextOutWFunc( HDC hdc, int x, int y, LPCWSTR lpString, int c ) ;
extern	HDC					WINAPI CreateDCWFunc( LPCWSTR pwszDriver, LPCWSTR pwszDevice, LPCWSTR pszPort, CONST D_DEVMODEW * pdm ) ;
extern	LONG				WINAPI RegOpenKeyWFunc( HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult ) ;
extern	LONG				WINAPI RegEnumKeyWFunc( HKEY hKey, DWORD dwIndex, LPWSTR lpName, DWORD cchName ) ;
extern	LONG				WINAPI RegOpenKeyExWFunc( HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult ) ;
extern	LONG				WINAPI RegQueryValueExWFunc( HKEY hKey, LPCWSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData ) ;
extern	UINT				WINAPI DragQueryFileWFunc( HDROP hDrop, UINT iFile, LPWSTR lpszFile, UINT cch ) ;

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_WINAPI_H
