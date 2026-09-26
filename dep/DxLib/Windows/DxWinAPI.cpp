// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		WinAPIプログラム
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

// インクルード ------------------------------------------------------------------
#include "DxWinAPI.h"
#include <tchar.h>
#include "../DxMemory.h"
#include "../DxBaseFunc.h"
#include "../DxASyncLoad.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

// 構造体定義 --------------------------------------------------------------------

// 内部大域変数宣言 --------------------------------------------------------------

WINAPIDATA WinAPIData ;

static const char *D3DX9_DllNameTable[] =
{
	"d3dx9_50.dll",
	"d3dx9_49.dll",
	"d3dx9_48.dll",
	"d3dx9_47.dll",
	"d3dx9_46.dll",
	"d3dx9_45.dll",
	"d3dx9_44.dll",
	"d3dx9_43.dll",
	"d3dx9_42.dll",
	"d3dx9_41.dll",
	"d3dx9_40.dll",
	"d3dx9_39.dll",
	"d3dx9_38.dll",
	"d3dx9_37.dll",
	"d3dx9_36.dll",
	"d3dx9_35.dll",
	"d3dx9_34.dll",
	"d3dx9_33.dll",
	"d3dx9_32.dll",
	"d3dx9_31.dll",
	"d3dx9_30.dll",
	"d3dx9_29.dll",
	"d3dx9_28.dll",
	"d3dx9_27.dll",
	"d3dx9_26.dll",
	"d3dx9_25.dll",
	"d3dx9_24.dll",
	NULL,
} ;

static const char *D3DCompiler_DllNameTable[] =
{
	"D3DCompiler_47.dll",
	"D3DCompiler_46.dll",
	"D3DCompiler_45.dll",
	"D3DCompiler_44.dll",
	"D3DCompiler_43.dll",
	"D3DCompiler_42.dll",
	"D3DCompiler_41.dll",
	"D3DCompiler_40.dll",
	"D3DCompiler_39.dll",
	"D3DCompiler_38.dll",
	"D3DCompiler_37.dll",
	"D3DCompiler_36.dll",
	"D3DCompiler_35.dll",
	"D3DCompiler_34.dll",
	"D3DCompiler_33.dll",
	NULL,
} ;

// 関数プロトタイプ宣言-----------------------------------------------------------

// プログラム --------------------------------------------------------------------

// WindowsOS の DLL を読み込む
extern int LoadWinAPI( void )
{
	// 初期化済みの場合は何もしない
	if( WinAPIData.InitializeFlag )
	{
		return 0 ;
	}

#ifndef DX_NON_NETWORK
	// ws2_32.dll をロードする
	if( WinAPIData.WinSockFunc.WinSockDLL == NULL )
	{
		WinAPIData.WinSockFunc.WinSockDLL = LoadLibraryA( "ws2_32.dll" ) ;
		WinAPIData.WinSockFunc.WSAGetLastErrorFunc = ( int ( WINAPI * )( void ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "WSAGetLastError" ) ;
		WinAPIData.WinSockFunc.WSAStartupFunc = ( int ( WINAPI * )( WORD , LPWSADATA  ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "WSAStartup" ) ;
		WinAPIData.WinSockFunc.WSACleanupFunc = ( int ( WINAPI * )( void ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "WSACleanup" ) ;
		WinAPIData.WinSockFunc.WSAAsyncSelectFunc = ( int ( WINAPI * )( SOCKET, HWND, unsigned int, long ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "WSAAsyncSelect" ) ;
		WinAPIData.WinSockFunc.getaddrinfoFunc = ( int ( WINAPI * )( const char *, const char *, const _addrinfo *, _addrinfo ** ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "getaddrinfo" ) ;
		WinAPIData.WinSockFunc.freeaddrinfoFunc = ( void ( WINAPI * )( _addrinfo * ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "freeaddrinfo" ) ;
		WinAPIData.WinSockFunc.gethostbyaddrFunc = ( struct hostent* ( WINAPI * )( const char *, int, int ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "gethostbyaddr" ) ;
		WinAPIData.WinSockFunc.gethostbynameFunc = ( struct hostent* ( WINAPI * )( const char * ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "gethostbyname" ) ;
		WinAPIData.WinSockFunc.gethostnameFunc = ( int ( WINAPI * )( char *name, int namelen ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "gethostname" ) ;
		WinAPIData.WinSockFunc.ntohsFunc = ( u_short ( WINAPI * )( u_short ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "ntohs" ) ;
		WinAPIData.WinSockFunc.htonsFunc = ( u_short ( WINAPI * )( u_short ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "htons" ) ;
		WinAPIData.WinSockFunc.connectFunc = ( int ( WINAPI * )( SOCKET s, const struct sockaddr *, int ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "connect" ) ;
		WinAPIData.WinSockFunc.socketFunc = ( SOCKET ( WINAPI * )( int, int, int ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "socket" ) ;
		WinAPIData.WinSockFunc.sendFunc = ( int ( WINAPI * )( SOCKET s, const char *, int, int ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "send" ) ;
		WinAPIData.WinSockFunc.sendtoFunc = ( int ( WINAPI * )( SOCKET s, const char *, int, int, const struct sockaddr *, int ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "sendto" ) ;
		WinAPIData.WinSockFunc.recvfromFunc = ( int ( WINAPI * )( SOCKET s, char *, int , int , struct sockaddr *, int * ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "recvfrom" ) ;
		WinAPIData.WinSockFunc.acceptFunc = ( SOCKET ( WINAPI * )( SOCKET s, struct sockaddr *, int * ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "accept" ) ;
		WinAPIData.WinSockFunc.closesocketFunc = ( int ( WINAPI * )( SOCKET ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "closesocket" ) ;
		WinAPIData.WinSockFunc.shutdownFunc = ( int ( WINAPI * )( SOCKET , int  ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "shutdown" ) ;
		WinAPIData.WinSockFunc.listenFunc = ( int ( WINAPI * )( SOCKET , int  ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "listen" ) ;
		WinAPIData.WinSockFunc.bindFunc = ( int ( WINAPI * )( SOCKET , const struct sockaddr *, int  ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "bind" ) ;
		WinAPIData.WinSockFunc.inet_addrFunc = ( unsigned long ( WINAPI * )( const char * ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "inet_addr" ) ;
		WinAPIData.WinSockFunc.recvFunc = ( int ( WINAPI * )( SOCKET, char *, int, int ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "recv" ) ;
		WinAPIData.WinSockFunc.setsockoptFunc = ( int ( WINAPI * )( SOCKET, int, int, const char *, int ) )GetProcAddress( WinAPIData.WinSockFunc.WinSockDLL, "setsockopt" ) ;
	}
#endif

#ifndef DX_NON_KEYEX
	// Imm32DLL をロードする
	if( WinAPIData.ImmFunc.Imm32DLL == NULL )
	{
		WinAPIData.ImmFunc.Imm32DLL = LoadLibraryA( "Imm32.dll" ) ;
		WinAPIData.ImmFunc.ImmAssociateContextFunc = ( HIMC ( WINAPI * )( HWND, HIMC ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmAssociateContext" ) ;
		WinAPIData.ImmFunc.ImmGetContextFunc = ( HIMC ( WINAPI * )( HWND  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmGetContext" ) ;
		WinAPIData.ImmFunc.ImmReleaseContextFunc = ( BOOL ( WINAPI * )( HWND , HIMC  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmReleaseContext" ) ;
		WinAPIData.ImmFunc.ImmGetOpenStatusFunc = ( BOOL ( WINAPI * )( HIMC  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmGetOpenStatus" ) ;
		WinAPIData.ImmFunc.ImmGetConversionStatusFunc = ( BOOL ( WINAPI * )( HIMC , LPDWORD , LPDWORD  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmGetConversionStatus" ) ;
		WinAPIData.ImmFunc.ImmNotifyIMEFunc = ( BOOL ( WINAPI * )( HIMC , DWORD , DWORD , DWORD  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmNotifyIME" ) ;
		WinAPIData.ImmFunc.ImmSetOpenStatusFunc = ( BOOL ( WINAPI * )( HIMC , BOOL  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmSetOpenStatus" ) ;

//#ifdef UNICODE
		WinAPIData.ImmFunc.ImmGetCandidateListFunc = ( DWORD ( WINAPI * )( HIMC , DWORD , LPCANDIDATELIST , DWORD  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmGetCandidateListW" ) ;
		WinAPIData.ImmFunc.ImmGetCandidateListCountFunc = ( DWORD ( WINAPI * )( HIMC , LPDWORD  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmGetCandidateListCountW" ) ;
		WinAPIData.ImmFunc.ImmGetCompositionStringFunc = ( LONG ( WINAPI * )( HIMC , DWORD , LPVOID , DWORD  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmGetCompositionStringW" ) ;
		WinAPIData.ImmFunc.ImmSetCompositionStringFunc = ( BOOL ( WINAPI * )( HIMC , DWORD , LPCVOID , DWORD , LPCVOID , DWORD  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmSetCompositionStringW" ) ;
//#else
//		WinAPIData.ImmFunc.ImmGetCandidateListFunc = ( DWORD ( WINAPI * )( HIMC , DWORD , LPCANDIDATELIST , DWORD  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmGetCandidateListA" ) ;
//		WinAPIData.ImmFunc.ImmGetCandidateListCountFunc = ( DWORD ( WINAPI * )( HIMC , LPDWORD  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmGetCandidateListCountA" ) ;
//		WinAPIData.ImmFunc.ImmGetCompositionStringFunc = ( LONG ( WINAPI * )( HIMC , DWORD , LPVOID , DWORD  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmGetCompositionStringA" ) ;
//		WinAPIData.ImmFunc.ImmSetCompositionStringFunc = ( BOOL ( WINAPI * )( HIMC , DWORD , LPCVOID , DWORD , LPCVOID , DWORD  ) )GetProcAddress( WinAPIData.ImmFunc.Imm32DLL, "ImmSetCompositionStringA" ) ;
//#endif
	}
#endif

	// WinMM DLL をロードする
	if( WinAPIData.Win32Func.WinMMDLL == NULL )
	{
		WinAPIData.Win32Func.WinMMDLL = LoadLibraryA( "winmm.dll" ) ;
		WinAPIData.Win32Func.timeSetEventFunc = ( MMRESULT	( WINAPI * )( UINT , UINT , LPD_TIMECALLBACK , DWORD_PTR , UINT  ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "timeSetEvent" ) ;
		WinAPIData.Win32Func.timeKillEventFunc = ( MMRESULT ( WINAPI * )( UINT  ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "timeKillEvent" ) ;
		WinAPIData.Win32Func.timeBeginPeriodFunc = ( MMRESULT ( WINAPI * )( UINT  ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "timeBeginPeriod" ) ;
		WinAPIData.Win32Func.timeEndPeriodFunc = ( MMRESULT ( WINAPI * )( UINT  ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "timeEndPeriod" ) ;
		WinAPIData.Win32Func.timeGetTimeFunc = ( DWORD ( WINAPI * )( VOID ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "timeGetTime" ) ;
		WinAPIData.Win32Func.timeGetDevCapsFunc = ( MMRESULT ( WINAPI * )( LPTIMECAPS , UINT  ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "timeGetDevCaps" ) ;
		WinAPIData.Win32Func.joyGetPosExFunc = ( MMRESULT ( WINAPI * )( UINT , LPJOYINFOEX  ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "joyGetPosEx" ) ;

//#ifdef UNICODE
		WinAPIData.Win32Func.joyGetDevCapsFunc = ( MMRESULT ( WINAPI * )( UINT , LPJOYCAPSW , UINT  ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "joyGetDevCapsW" ) ;
		WinAPIData.Win32Func.mciSendCommandFunc = ( MCIERROR ( WINAPI * )( MCIDEVICEID , UINT , DWORD_PTR , DWORD_PTR  ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "mciSendCommandW" ) ;
//#else
//		WinAPIData.Win32Func.joyGetDevCapsFunc = ( MMRESULT ( WINAPI * )( UINT , LPJOYCAPSA , UINT  ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "joyGetDevCapsA" ) ;
//		WinAPIData.Win32Func.mciSendCommandFunc = ( MCIERROR ( WINAPI * )( MCIDEVICEID , UINT , DWORD_PTR , DWORD_PTR  ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "mciSendCommandA" ) ;
//#endif

		WinAPIData.Win32Func.waveOutOpenFunc = ( MMRESULT ( WINAPI * )( LPHWAVEOUT, UINT, LPCWAVEFORMATEX, DWORD_PTR, DWORD_PTR, DWORD ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "waveOutOpen" ) ;
		WinAPIData.Win32Func.waveOutCloseFunc = ( MMRESULT ( WINAPI * )( HWAVEOUT ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "waveOutClose" ) ;
		WinAPIData.Win32Func.waveOutPrepareHeaderFunc = ( MMRESULT ( WINAPI * )( HWAVEOUT, LPWAVEHDR, UINT ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "waveOutPrepareHeader" ) ;
		WinAPIData.Win32Func.waveOutUnprepareHeaderFunc = ( MMRESULT ( WINAPI * )( HWAVEOUT, LPWAVEHDR, UINT ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "waveOutUnprepareHeader" ) ;
		WinAPIData.Win32Func.waveOutWriteFunc = ( MMRESULT ( WINAPI * )( HWAVEOUT, LPWAVEHDR, UINT ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "waveOutWrite" ) ;
		WinAPIData.Win32Func.waveOutResetFunc = ( MMRESULT ( WINAPI * )( HWAVEOUT ) )GetProcAddress( WinAPIData.Win32Func.WinMMDLL, "waveOutReset" ) ;
	}

	// Kernel32 DLL をロードする
	if( WinAPIData.Win32Func.Kernel32DLL == NULL )
	{
		WinAPIData.Win32Func.Kernel32DLL = LoadLibraryA( "kernel32.dll" ) ;
		WinAPIData.Win32Func.GetModuleHandleAFunc = ( HMODULE ( WINAPI * )( LPCSTR ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GetModuleHandleA" ) ;
		WinAPIData.Win32Func.GetModuleHandleWFunc = ( HMODULE ( WINAPI * )( LPCWSTR ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GetModuleHandleW" ) ;
		WinAPIData.Win32Func.VerifyVersionInfoAFunc = ( BOOL ( WINAPI * )( D_OSVERSIONINFOEXA *, DWORD, DWORDLONG ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "VerifyVersionInfoA" ) ;
		WinAPIData.Win32Func.VerifyVersionInfoWFunc = ( BOOL ( WINAPI * )( D_OSVERSIONINFOEXW *, DWORD, DWORDLONG ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "VerifyVersionInfoW" ) ;
		WinAPIData.Win32Func.VerSetConditionMaskFunc = ( ULONGLONG ( WINAPI * )( ULONGLONG, DWORD, BYTE ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "VerSetConditionMask" ) ;
		WinAPIData.Win32Func.CreateEventAFunc = ( HANDLE ( WINAPI * )( LPSECURITY_ATTRIBUTES, BOOL, BOOL, LPCSTR ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "CreateEventA" ) ;
		WinAPIData.Win32Func.SetEventFunc = ( BOOL ( WINAPI * )( HANDLE ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "SetEvent" ) ;
		WinAPIData.Win32Func.ResetEventFunc = ( BOOL ( WINAPI * )( HANDLE ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "ResetEvent" ) ;
		WinAPIData.Win32Func.CreateThreadFunc = ( HANDLE ( WINAPI * )( LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE, LPVOID, DWORD, LPDWORD ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "CreateThread" ) ;
		WinAPIData.Win32Func.SetThreadPriorityFunc = ( BOOL ( WINAPI * )( HANDLE, int ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "SetThreadPriority" ) ;
		WinAPIData.Win32Func.GetThreadPriorityFunc = ( int ( WINAPI * )( HANDLE ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GetThreadPriority" ) ;
		WinAPIData.Win32Func.ExitThreadFunc = ( VOID ( WINAPI * )( DWORD ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "ExitThread" ) ;
		WinAPIData.Win32Func.GetExitCodeThreadFunc = ( BOOL ( WINAPI * )( HANDLE, LPDWORD ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GetExitCodeThread" ) ;
		WinAPIData.Win32Func.GetCurrentThreadFunc = ( HANDLE ( WINAPI * )( VOID ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GetCurrentThread" ) ;
		WinAPIData.Win32Func.GetCurrentThreadIdFunc = ( DWORD ( WINAPI * )( VOID ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GetCurrentThreadId" ) ;
		WinAPIData.Win32Func.SuspendThreadFunc = ( DWORD ( WINAPI * )( HANDLE ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "SuspendThread" ) ;
		WinAPIData.Win32Func.ResumeThreadFunc = ( DWORD ( WINAPI * )( HANDLE ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "ResumeThread" ) ;
		WinAPIData.Win32Func.WaitForSingleObjectFunc = ( DWORD ( WINAPI * )( HANDLE, DWORD ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "WaitForSingleObject" ) ;
		WinAPIData.Win32Func.WaitForMultipleObjectsFunc = ( DWORD ( WINAPI * )( DWORD, CONST HANDLE *, BOOL, DWORD ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "WaitForMultipleObjects" ) ;
		WinAPIData.Win32Func.GetProcessHeapFunc = ( HANDLE ( WINAPI * )( VOID ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GetProcessHeap" ) ;
		WinAPIData.Win32Func.HeapAllocFunc = ( LPVOID ( WINAPI * )( HANDLE, DWORD, SIZE_T ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "HeapAlloc" ) ;
		WinAPIData.Win32Func.HeapFreeFunc = ( BOOL ( WINAPI * )( HANDLE, DWORD, LPVOID ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "HeapFree" ) ;
		WinAPIData.Win32Func.VirtualAllocFunc = ( LPVOID ( WINAPI * )( LPVOID, SIZE_T, DWORD, DWORD ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "VirtualAlloc" ) ;
		WinAPIData.Win32Func.VirtualFreeFunc = ( BOOL ( WINAPI * )( LPVOID, SIZE_T, DWORD ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "VirtualFree" ) ;
		WinAPIData.Win32Func.VirtualQueryFunc = ( SIZE_T ( WINAPI * )( LPCVOID, PMEMORY_BASIC_INFORMATION, SIZE_T ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "VirtualQuery" ) ;
		WinAPIData.Win32Func.GlobalAllocFunc = ( HGLOBAL ( WINAPI * )( UINT, SIZE_T ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GlobalAlloc" ) ;
		WinAPIData.Win32Func.GlobalFreeFunc = ( HGLOBAL ( WINAPI * )( HGLOBAL ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GlobalFree" ) ;
		WinAPIData.Win32Func.GlobalLockFunc = ( HGLOBAL ( WINAPI * )( HGLOBAL ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GlobalLock" ) ;
		WinAPIData.Win32Func.GlobalUnlockFunc = ( BOOL ( WINAPI * )( HGLOBAL ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GlobalUnlock" ) ;
		WinAPIData.Win32Func.GlobalSizeFunc = ( SIZE_T ( WINAPI * )( HGLOBAL ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GlobalSize" ) ;
		WinAPIData.Win32Func.CreateSemaphoreAFunc = ( HANDLE ( WINAPI * )( LPSECURITY_ATTRIBUTES, LONG, LONG, LPCSTR ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "CreateSemaphoreA" ) ;
		WinAPIData.Win32Func.ReleaseSemaphoreFunc = ( BOOL ( WINAPI * )( HANDLE, LONG, LPLONG ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "ReleaseSemaphore" ) ;
		WinAPIData.Win32Func.GetVersionExAFunc = ( BOOL ( WINAPI * )( LPOSVERSIONINFOA ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GetVersionExA" ) ;
		WinAPIData.Win32Func.GetVersionExWFunc = ( BOOL ( WINAPI * )( LPOSVERSIONINFOW ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GetVersionExW" ) ;
		WinAPIData.Win32Func.GetSystemInfoFunc = ( VOID ( WINAPI * )( LPSYSTEM_INFO ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GetSystemInfo" ) ;
		WinAPIData.Win32Func.lstrlenAFunc = ( int ( WINAPI * )( LPCSTR ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "lstrlenA" ) ;
		WinAPIData.Win32Func.lstrlenWFunc = ( int ( WINAPI * )( WORD const * ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "lstrlenW" ) ;
		WinAPIData.Win32Func.lstrcmpAFunc = ( int ( WINAPI * )( LPCSTR, LPCSTR ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "lstrcmpA" ) ;
		WinAPIData.Win32Func.lstrcmpWFunc = ( int ( WINAPI * )( WORD const *, WORD const * ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "lstrcmpW" ) ;
		WinAPIData.Win32Func.lstrcpyAFunc = ( LPSTR ( WINAPI * )( LPSTR, LPCSTR ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "lstrcpyA" ) ;
		WinAPIData.Win32Func.lstrcpyWFunc = ( WORD * ( WINAPI * )( WORD *, WORD const * ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "lstrcpyW" ) ;
		WinAPIData.Win32Func.lstrcpynAFunc = ( LPSTR ( WINAPI * )( LPSTR, LPCSTR, int ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "lstrcpynA" ) ;
		WinAPIData.Win32Func.lstrcpynWFunc = ( WORD * ( WINAPI * )( WORD *, WORD const *, int ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "lstrcpynW" ) ;
		WinAPIData.Win32Func.GetTickCountFunc = ( DWORD ( WINAPI * )( VOID ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GetTickCount" ) ;
		WinAPIData.Win32Func.QueryPerformanceCounterFunc = ( BOOL ( WINAPI * )( LARGE_INTEGER * ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "QueryPerformanceCounter" ) ;
		WinAPIData.Win32Func.QueryPerformanceFrequencyFunc = ( BOOL ( WINAPI * )( LARGE_INTEGER * ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "QueryPerformanceFrequency" ) ;
		WinAPIData.Win32Func.CloseHandleFunc = ( BOOL ( WINAPI * )( HANDLE ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "CloseHandle" ) ;
		WinAPIData.Win32Func.GetTempPathAFunc = ( DWORD ( WINAPI * )( DWORD, LPSTR ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GetTempPathA" ) ;
		WinAPIData.Win32Func.GetTempPathWFunc = ( DWORD ( WINAPI * )( DWORD, LPWSTR ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GetTempPathW" ) ;
		WinAPIData.Win32Func.GetTempFileNameAFunc = ( UINT ( WINAPI * )( LPCSTR, LPCSTR, UINT, LPSTR ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GetTempFileNameA" ) ;
		WinAPIData.Win32Func.GetTempFileNameWFunc = ( UINT ( WINAPI * )( LPCWSTR, LPCWSTR, UINT, LPWSTR ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GetTempFileNameW" ) ;
		WinAPIData.Win32Func.CreateFileAFunc = ( HANDLE ( WINAPI * )( LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "CreateFileA" ) ;
		WinAPIData.Win32Func.CreateFileWFunc = ( HANDLE ( WINAPI * )( LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "CreateFileW" ) ;
		WinAPIData.Win32Func.DeleteFileAFunc = ( BOOL ( WINAPI * )( LPCSTR ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "DeleteFileA" ) ;
		WinAPIData.Win32Func.DeleteFileWFunc = ( BOOL ( WINAPI * )( LPCWSTR ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "DeleteFileW" ) ;
		WinAPIData.Win32Func.GetFileSizeFunc = ( DWORD ( WINAPI * )( HANDLE, LPDWORD ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GetFileSize" ) ;
		WinAPIData.Win32Func.WriteFileFunc = ( BOOL ( WINAPI * )( HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "WriteFile" ) ;
		WinAPIData.Win32Func.ReadFileFunc = ( BOOL ( WINAPI * )( HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "ReadFile" ) ;
		WinAPIData.Win32Func.SetFilePointerFunc = ( DWORD ( WINAPI * )( HANDLE, LONG, PLONG, DWORD ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "SetFilePointer" ) ;
		WinAPIData.Win32Func.FindFirstFileAFunc = ( HANDLE ( WINAPI * )( LPCSTR, LPWIN32_FIND_DATAA ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "FindFirstFileA" ) ;
		WinAPIData.Win32Func.FindFirstFileWFunc = ( HANDLE ( WINAPI * )( LPCWSTR, LPWIN32_FIND_DATAW ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "FindFirstFileW" ) ;
		WinAPIData.Win32Func.FindNextFileAFunc = ( BOOL ( WINAPI * )( HANDLE, LPWIN32_FIND_DATAA ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "FindNextFileA" ) ;
		WinAPIData.Win32Func.FindNextFileWFunc = ( BOOL ( WINAPI * )( HANDLE, LPWIN32_FIND_DATAW ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "FindNextFileW" ) ;
		WinAPIData.Win32Func.FindCloseFunc = ( BOOL ( WINAPI * )( HANDLE ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "FindClose" ) ;
		WinAPIData.Win32Func.SetCurrentDirectoryAFunc = ( BOOL ( WINAPI * )( LPCSTR ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "SetCurrentDirectoryA" ) ;
		WinAPIData.Win32Func.SetCurrentDirectoryWFunc = ( BOOL ( WINAPI * )( LPCWSTR ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "SetCurrentDirectoryW" ) ;
		WinAPIData.Win32Func.GetCurrentDirectoryAFunc = ( DWORD ( WINAPI * )( DWORD, LPSTR ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GetCurrentDirectoryA" ) ;
		WinAPIData.Win32Func.GetCurrentDirectoryWFunc = ( DWORD ( WINAPI * )( DWORD, LPWSTR ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GetCurrentDirectoryW" ) ;
		WinAPIData.Win32Func.SleepFunc = ( VOID ( WINAPI * )( DWORD ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "Sleep" ) ;
		WinAPIData.Win32Func.InitializeCriticalSectionFunc = ( VOID ( WINAPI * )( LPCRITICAL_SECTION ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "InitializeCriticalSection" ) ;
		WinAPIData.Win32Func.DeleteCriticalSectionFunc = ( VOID ( WINAPI * )( LPCRITICAL_SECTION ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "DeleteCriticalSection" ) ;
		WinAPIData.Win32Func.EnterCriticalSectionFunc = ( VOID ( WINAPI * )( LPCRITICAL_SECTION ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "EnterCriticalSection" ) ;
		WinAPIData.Win32Func.LeaveCriticalSectionFunc = ( VOID ( WINAPI * )( LPCRITICAL_SECTION ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "LeaveCriticalSection" ) ;
		WinAPIData.Win32Func.ExitProcessFunc = ( VOID ( WINAPI * )( UINT ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "ExitProcess" ) ;
		WinAPIData.Win32Func.LoadResourceFunc = ( HGLOBAL ( WINAPI * )( HMODULE, HRSRC ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "LoadResource" ) ;
		WinAPIData.Win32Func.FindResourceAFunc = ( HRSRC ( WINAPI * )( HMODULE, LPCSTR, LPCSTR ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "FindResourceA" ) ;
		WinAPIData.Win32Func.FindResourceWFunc = ( HRSRC ( WINAPI * )( HMODULE, LPCWSTR, LPCWSTR ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "FindResourceW" ) ;
		WinAPIData.Win32Func.LockResourceFunc = ( LPVOID ( WINAPI * )( HGLOBAL ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "LockResource" ) ;
		WinAPIData.Win32Func.SizeofResourceFunc = ( DWORD ( WINAPI * )( HMODULE, HRSRC ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "SizeofResource" ) ;
		WinAPIData.Win32Func.GetOEMCPFunc = ( UINT ( WINAPI * )( VOID ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GetOEMCP" ) ;
		WinAPIData.Win32Func.OutputDebugStringAFunc = ( VOID ( WINAPI * )( LPCSTR ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "OutputDebugStringA" ) ;
		WinAPIData.Win32Func.OutputDebugStringWFunc = ( VOID ( WINAPI * )( LPCWSTR ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "OutputDebugStringW" ) ;
		WinAPIData.Win32Func.GlobalMemoryStatusFunc = ( VOID ( WINAPI * )( LPMEMORYSTATUS ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GlobalMemoryStatus" ) ;
		WinAPIData.Win32Func.MulDivFunc = ( int ( WINAPI * )( int, int, int ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "MulDiv" ) ;
		WinAPIData.Win32Func.GetLocalTimeFunc = ( VOID ( WINAPI * )( LPSYSTEMTIME ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GetLocalTime" ) ;
		WinAPIData.Win32Func.GetSystemPowerStatusFunc = ( BOOL ( WINAPI * )( LPSYSTEM_POWER_STATUS ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GetSystemPowerStatus" ) ;
		WinAPIData.Win32Func.FileTimeToLocalFileTimeFunc = ( BOOL ( WINAPI * )( FILETIME *, LPFILETIME ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "FileTimeToLocalFileTime" ) ;
		WinAPIData.Win32Func.FileTimeToSystemTimeFunc = ( BOOL ( WINAPI * )( FILETIME *, LPSYSTEMTIME ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "FileTimeToSystemTime" ) ;
		WinAPIData.Win32Func.GetLastErrorFunc = ( DWORD ( WINAPI * )( VOID ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "GetLastError" ) ;
		WinAPIData.Win32Func.MultiByteToWideCharFunc = ( int ( WINAPI * )( UINT, DWORD, LPCCH, int, LPWSTR, int ) )GetProcAddress( WinAPIData.Win32Func.Kernel32DLL, "MultiByteToWideChar" ) ;
	}

	// Old32 DLL をロードする
	if( WinAPIData.Win32Func.Old32DLL == NULL )
	{
		WinAPIData.Win32Func.Old32DLL = LoadLibraryA( "ole32.dll" ) ;
		WinAPIData.Win32Func.CoCreateInstanceFunc = ( HRESULT ( WINAPI * )( REFCLSID , D_IUnknown * , DWORD , REFIID , LPVOID * ) )GetProcAddress( WinAPIData.Win32Func.Old32DLL, "CoCreateInstance" ) ;
		WinAPIData.Win32Func.CoTaskMemAllocFunc = ( LPVOID ( WINAPI * )( SIZE_T  ) )GetProcAddress( WinAPIData.Win32Func.Old32DLL, "CoTaskMemAlloc" ) ;
		WinAPIData.Win32Func.CoTaskMemFreeFunc = ( void ( WINAPI * )( LPVOID  ) )GetProcAddress( WinAPIData.Win32Func.Old32DLL, "CoTaskMemFree" ) ;
		WinAPIData.Win32Func.CoInitializeExFunc = ( HRESULT ( WINAPI * )( LPVOID, DWORD ) )GetProcAddress( WinAPIData.Win32Func.Old32DLL, "CoInitializeEx" ) ;
		WinAPIData.Win32Func.CoFreeUnusedLibrariesFunc = ( void ( WINAPI * )( void ) )GetProcAddress( WinAPIData.Win32Func.Old32DLL, "CoFreeUnusedLibraries" ) ;
		WinAPIData.Win32Func.CoUninitializeFunc = ( void ( WINAPI * )( void ) )GetProcAddress( WinAPIData.Win32Func.Old32DLL, "CoUninitialize" ) ;
		WinAPIData.Win32Func.CLSIDFromStringFunc = ( HRESULT ( WINAPI * )( LPCOLESTR, LPCLSID ) )GetProcAddress( WinAPIData.Win32Func.Old32DLL, "CLSIDFromString" ) ;
	}

	// OleAut32 DLL をロードする
	if( WinAPIData.Win32Func.OleAut32DLL == NULL )
	{
		WinAPIData.Win32Func.OleAut32DLL = LoadLibraryA( "oleaut32.dll" ) ;
		WinAPIData.Win32Func.SysAllocStringFunc = ( BSTR ( WINAPI * )( const OLECHAR * ) )GetProcAddress( WinAPIData.Win32Func.OleAut32DLL, "SysAllocString" ) ;
		WinAPIData.Win32Func.SysFreeStringFunc = ( void ( WINAPI * )( BSTR ) )GetProcAddress( WinAPIData.Win32Func.OleAut32DLL, "SysFreeString" ) ;
		WinAPIData.Win32Func.VariantClearFunc = ( HRESULT (WINAPI * )( VARIANTARG * ) )GetProcAddress( WinAPIData.Win32Func.OleAut32DLL, "VariantClear" ) ;
		WinAPIData.Win32Func.VariantInitFunc = ( void (WINAPI * )( VARIANTARG * ) )GetProcAddress( WinAPIData.Win32Func.OleAut32DLL, "VariantInit" ) ;
	}

	// Comctl32 DLL をロードする
	if( WinAPIData.Win32Func.Comctl32DLL == NULL )
	{
		WinAPIData.Win32Func.Comctl32DLL = LoadLibraryA( "comctl32.dll" ) ;
		if( WinAPIData.Win32Func.Comctl32DLL != NULL )
		{
			WinAPIData.Win32Func.InitCommonControlsFunc = ( void ( WINAPI * )( VOID ) )GetProcAddress( WinAPIData.Win32Func.Comctl32DLL, "InitCommonControls" ) ;
		}
	}

	// Ole32 DLL をロードする
	if( WinAPIData.Win32Func.Ole32DLL == NULL )
	{
		WinAPIData.Win32Func.Ole32DLL = LoadLibraryA( "Ole32.dll" ) ;
		if( WinAPIData.Win32Func.Ole32DLL != NULL )
		{
			WinAPIData.Win32Func.CoSetProxyBlanketFunc = ( HRESULT ( WINAPI * )( D_IUnknown *, DWORD, DWORD, OLECHAR *, DWORD, DWORD, RPC_AUTH_IDENTITY_HANDLE, DWORD ) )GetProcAddress( WinAPIData.Win32Func.Ole32DLL, "CoSetProxyBlanket" ) ;
			WinAPIData.Win32Func.PropVariantClearFunc = ( HRESULT ( WINAPI * )( D_PROPVARIANT * ) )GetProcAddress( WinAPIData.Win32Func.Ole32DLL, "PropVariantClear" ) ;
		}
	}

	// User32DLL をロードする
	if( WinAPIData.Win32Func.User32DLL == NULL )
	{
		WinAPIData.Win32Func.User32DLL = LoadLibraryA( "User32.dll" ) ;
		WinAPIData.Win32Func.WINNLSEnableIME_Func = ( BOOL ( WINAPI * )( HWND, BOOL ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "WINNLSEnableIME" ) ;
		WinAPIData.Win32Func.UpdateLayeredWindow = ( BOOL ( WINAPI * )( HWND, HDC, POINT*, SIZE*, HDC, POINT*, COLORREF, BLENDFUNCTION*, DWORD ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "UpdateLayeredWindow" ) ;
		WinAPIData.Win32Func.CreateWindowExAFunc = ( HWND ( WINAPI * )( DWORD, LPCSTR, LPCSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "CreateWindowExA" ) ;
		WinAPIData.Win32Func.CreateWindowExWFunc = ( HWND ( WINAPI * )( DWORD, LPCWSTR, LPCWSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "CreateWindowExW" ) ;
		WinAPIData.Win32Func.DestroyWindowFunc = ( BOOL ( WINAPI * )( HWND ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "DestroyWindow" ) ;
		WinAPIData.Win32Func.IsWindowFunc = ( BOOL ( WINAPI * )( HWND ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "IsWindow" ) ;
		WinAPIData.Win32Func.ShowWindowFunc = ( BOOL ( WINAPI * )( HWND, int ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "ShowWindow" ) ;
		WinAPIData.Win32Func.UpdateWindowFunc = ( BOOL ( WINAPI * )( HWND ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "UpdateWindow" ) ;
		WinAPIData.Win32Func.BringWindowToTopFunc = ( BOOL ( WINAPI * )( HWND ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "BringWindowToTop" ) ;
		WinAPIData.Win32Func.SetForegroundWindowFunc = ( BOOL ( WINAPI * )( HWND ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "SetForegroundWindow" ) ;
		WinAPIData.Win32Func.GetWindowThreadProcessIdFunc = ( DWORD ( WINAPI * )( HWND, LPDWORD ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetWindowThreadProcessId" ) ;
		WinAPIData.Win32Func.SetActiveWindowFunc = ( HWND ( WINAPI * )( HWND ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "SetActiveWindow" ) ;
		WinAPIData.Win32Func.GetForegroundWindowFunc = ( HWND ( WINAPI * )( VOID ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetForegroundWindow" ) ;
		WinAPIData.Win32Func.GetDesktopWindowFunc = ( HWND ( WINAPI * )( VOID ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetDesktopWindow" ) ;
		WinAPIData.Win32Func.FindWindowAFunc = ( HWND ( WINAPI * )( LPCSTR, LPCSTR ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "FindWindowA" ) ;
		WinAPIData.Win32Func.FindWindowWFunc = ( HWND ( WINAPI * )( LPCWSTR, LPCWSTR ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "FindWindowW" ) ;
		WinAPIData.Win32Func.MoveWindowFunc = ( BOOL ( WINAPI * )( HWND, int, int, int, int, BOOL ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "MoveWindow" ) ;
		WinAPIData.Win32Func.SetWindowPosFunc = ( BOOL ( WINAPI * )( HWND, HWND, int, int, int, int, UINT ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "SetWindowPos" ) ;
		WinAPIData.Win32Func.GetClientRectFunc = ( BOOL ( WINAPI * )( HWND, LPRECT ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetClientRect" ) ;
		WinAPIData.Win32Func.AdjustWindowRectExFunc = ( BOOL ( WINAPI * )( LPRECT, DWORD, BOOL, DWORD ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "AdjustWindowRectEx" ) ;
		WinAPIData.Win32Func.GetWindowRectFunc = ( BOOL ( WINAPI * )( HWND, LPRECT ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetWindowRect" ) ;
		WinAPIData.Win32Func.SetWindowLongAFunc = ( LONG ( WINAPI * )( HWND, int, LONG ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "SetWindowLongA" ) ;
		WinAPIData.Win32Func.SetWindowLongWFunc = ( LONG ( WINAPI * )( HWND, int, LONG ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "SetWindowLongW" ) ;
		WinAPIData.Win32Func.SetWindowLongPtrAFunc = ( LONG_PTR ( WINAPI * )( HWND, int, LONG_PTR ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "SetWindowLongPtrA" ) ;
		WinAPIData.Win32Func.SetWindowLongPtrWFunc = ( LONG_PTR ( WINAPI * )( HWND, int, LONG_PTR ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "SetWindowLongPtrW" ) ;
		WinAPIData.Win32Func.GetWindowLongAFunc = ( LONG ( WINAPI * )( HWND, int ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetWindowLongA" ) ;
		WinAPIData.Win32Func.GetWindowLongWFunc = ( LONG ( WINAPI * )( HWND, int ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetWindowLongW" ) ;
		WinAPIData.Win32Func.GetWindowLongPtrAFunc = ( LONG_PTR ( WINAPI * )( HWND, int ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetWindowLongPtrA" ) ;
		WinAPIData.Win32Func.GetWindowLongPtrWFunc = ( LONG_PTR ( WINAPI * )( HWND, int ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetWindowLongPtrW" ) ;
		WinAPIData.Win32Func.SetWindowTextAFunc = ( BOOL ( WINAPI * )( HWND, LPCSTR ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "SetWindowTextA" ) ;
		WinAPIData.Win32Func.SetWindowTextWFunc = ( BOOL ( WINAPI * )( HWND, LPCWSTR ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "SetWindowTextW" ) ;
		WinAPIData.Win32Func.ClientToScreenFunc = ( BOOL ( WINAPI * )( HWND, LPPOINT ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "ClientToScreen" ) ;
		WinAPIData.Win32Func.EnumDisplayDevicesAFunc = ( BOOL ( WINAPI * )( LPCSTR, DWORD, D_DISPLAY_DEVICEA *, DWORD ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "EnumDisplayDevicesA" ) ;
		WinAPIData.Win32Func.EnumDisplayDevicesWFunc = ( BOOL ( WINAPI * )( LPCWSTR, DWORD, D_DISPLAY_DEVICEW *, DWORD ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "EnumDisplayDevicesW" ) ;
		WinAPIData.Win32Func.ShutdownBlockReasonCreateFunc = ( BOOL ( WINAPI * )( HWND, LPCWSTR ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "ShutdownBlockReasonCreate" ) ;
		WinAPIData.Win32Func.ShutdownBlockReasonDestroyFunc = ( BOOL ( WINAPI * )( HWND ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "ShutdownBlockReasonDestroy" ) ;
		WinAPIData.Win32Func.SetTimerFunc = ( UINT_PTR ( WINAPI * )( HWND, UINT_PTR, UINT, D_TIMERPROC ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "SetTimer" ) ;
		WinAPIData.Win32Func.PostThreadMessageAFunc = ( BOOL ( WINAPI * )( DWORD, UINT, WPARAM, LPARAM ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "PostThreadMessageA" ) ;
		WinAPIData.Win32Func.PostThreadMessageWFunc = ( BOOL ( WINAPI * )( DWORD, UINT, WPARAM, LPARAM ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "PostThreadMessageW" ) ;
		WinAPIData.Win32Func.PeekMessageAFunc = ( BOOL ( WINAPI * )( LPMSG, HWND, UINT, UINT, UINT ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "PeekMessageA" ) ;
		WinAPIData.Win32Func.PeekMessageWFunc = ( BOOL ( WINAPI * )( LPMSG, HWND, UINT, UINT, UINT ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "PeekMessageW" ) ;
		WinAPIData.Win32Func.GetMessageAFunc = ( BOOL ( WINAPI * )( LPMSG, HWND, UINT, UINT ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetMessageA" ) ;
		WinAPIData.Win32Func.GetMessageWFunc = ( BOOL ( WINAPI * )( LPMSG, HWND, UINT, UINT ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetMessageW" ) ;
		WinAPIData.Win32Func.SendMessageAFunc = ( LRESULT ( WINAPI * )( HWND, UINT, WPARAM, LPARAM ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "SendMessageA" ) ;
		WinAPIData.Win32Func.SendMessageWFunc = ( LRESULT ( WINAPI * )( HWND, UINT, WPARAM, LPARAM ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "SendMessageW" ) ;
		WinAPIData.Win32Func.PostMessageAFunc = ( BOOL ( WINAPI * )( HWND, UINT, WPARAM, LPARAM ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "PostMessageA" ) ;
		WinAPIData.Win32Func.PostMessageWFunc = ( BOOL ( WINAPI * )( HWND, UINT, WPARAM, LPARAM ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "PostMessageW" ) ;
		WinAPIData.Win32Func.PostQuitMessageFunc = ( VOID ( WINAPI * )( int ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "PostQuitMessage" ) ;
		WinAPIData.Win32Func.DefWindowProcAFunc = ( LRESULT ( WINAPI * )( HWND, UINT, WPARAM, LPARAM ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "DefWindowProcA" ) ;
		WinAPIData.Win32Func.DefWindowProcWFunc = ( LRESULT ( WINAPI * )( HWND, UINT, WPARAM, LPARAM ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "DefWindowProcW" ) ;
		WinAPIData.Win32Func.TranslateAcceleratorAFunc = ( int ( WINAPI * )( HWND, HACCEL, LPMSG ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "TranslateAcceleratorA" ) ;
		WinAPIData.Win32Func.TranslateAcceleratorWFunc = ( int ( WINAPI * )( HWND, HACCEL, LPMSG ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "TranslateAcceleratorW" ) ;
		WinAPIData.Win32Func.TranslateMessageFunc = ( BOOL ( WINAPI * )( MSG * ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "TranslateMessage" ) ;
		WinAPIData.Win32Func.DispatchMessageAFunc = ( LRESULT ( WINAPI * )( MSG * ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "DispatchMessageA" ) ;
		WinAPIData.Win32Func.DispatchMessageWFunc = ( LRESULT ( WINAPI * )( MSG * ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "DispatchMessageW" ) ;
		WinAPIData.Win32Func.MsgWaitForMultipleObjectsFunc = ( DWORD ( WINAPI * )( DWORD, CONST HANDLE *, BOOL, DWORD, DWORD ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "MsgWaitForMultipleObjects" ) ;
		WinAPIData.Win32Func.GetQueueStatusFunc = ( DWORD ( WINAPI * )( UINT ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetQueueStatus" ) ;
		WinAPIData.Win32Func.RegisterWindowMessageAFunc = ( UINT ( WINAPI * )( LPCSTR ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "RegisterWindowMessageA" ) ;
		WinAPIData.Win32Func.RegisterWindowMessageWFunc = ( UINT ( WINAPI * )( LPCWSTR ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "RegisterWindowMessageW" ) ;
		WinAPIData.Win32Func.RegisterClassAFunc = ( ATOM ( WINAPI * )( CONST WNDCLASSA * ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "RegisterClassA" ) ;
		WinAPIData.Win32Func.RegisterClassWFunc = ( ATOM ( WINAPI * )( CONST WNDCLASSW * ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "RegisterClassW" ) ;
		WinAPIData.Win32Func.RegisterClassExAFunc = ( ATOM ( WINAPI * )( CONST WNDCLASSEXA * ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "RegisterClassExA" ) ;
		WinAPIData.Win32Func.RegisterClassExWFunc = ( ATOM ( WINAPI * )( CONST WNDCLASSEXW * ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "RegisterClassExW" ) ;
		WinAPIData.Win32Func.UnregisterClassAFunc = ( BOOL ( WINAPI * )( LPCSTR, HINSTANCE ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "UnregisterClassA" ) ;
		WinAPIData.Win32Func.UnregisterClassWFunc = ( BOOL ( WINAPI * )( LPCWSTR, HINSTANCE ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "UnregisterClassW" ) ;
		WinAPIData.Win32Func.SetClassLongAFunc = ( DWORD ( WINAPI * )( HWND, int, LONG ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "SetClassLongA" ) ;
		WinAPIData.Win32Func.SetClassLongWFunc = ( DWORD ( WINAPI * )( HWND, int, LONG ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "SetClassLongW" ) ;
		WinAPIData.Win32Func.SetClassLongPtrAFunc = ( unsigned LONG_PTR ( WINAPI * )( HWND, int, LONG_PTR ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "SetClassLongPtrA" ) ;
		WinAPIData.Win32Func.SetClassLongPtrWFunc = ( unsigned LONG_PTR ( WINAPI * )( HWND, int, LONG_PTR ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "SetClassLongPtrW" ) ;
		WinAPIData.Win32Func.GetDCFunc = ( HDC ( WINAPI * )( HWND ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetDC" ) ;
		WinAPIData.Win32Func.ReleaseDCFunc = ( int ( WINAPI * )( HWND, HDC ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "ReleaseDC" ) ;
		WinAPIData.Win32Func.BeginPaintFunc = ( HDC ( WINAPI * )( HWND, LPPAINTSTRUCT ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "BeginPaint" ) ;
		WinAPIData.Win32Func.EndPaintFunc = ( BOOL ( WINAPI * )( HWND, PAINTSTRUCT * ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "EndPaint" ) ;
		WinAPIData.Win32Func.MessageBoxAFunc = ( int ( WINAPI * )( HWND, LPCSTR, LPCSTR, UINT ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "MessageBoxA" ) ;
		WinAPIData.Win32Func.MessageBoxWFunc = ( int ( WINAPI * )( HWND, LPCWSTR, LPCWSTR, UINT ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "MessageBoxW" ) ;
		WinAPIData.Win32Func.GetSystemMetricsFunc = ( int ( WINAPI * )( int ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetSystemMetrics" ) ;
		WinAPIData.Win32Func.ChangeDisplaySettingsAFunc = ( LONG ( WINAPI * )( D_DEVMODEA*, DWORD ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "ChangeDisplaySettingsA" ) ;
		WinAPIData.Win32Func.ChangeDisplaySettingsWFunc = ( LONG ( WINAPI * )( D_DEVMODEW*, DWORD ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "ChangeDisplaySettingsW" ) ;
		WinAPIData.Win32Func.ShowCursorFunc = ( int ( WINAPI * )( BOOL ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "ShowCursor" ) ;
		WinAPIData.Win32Func.SetCursorPosFunc = ( BOOL ( WINAPI * )( int, int ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "SetCursorPos" ) ;
		WinAPIData.Win32Func.GetCursorPosFunc = ( BOOL ( WINAPI * )( LPPOINT ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetCursorPos" ) ;
		WinAPIData.Win32Func.SetCursorFunc = ( HCURSOR ( WINAPI * )( HCURSOR ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "SetCursor" ) ;
		WinAPIData.Win32Func.LoadCursorAFunc = ( HCURSOR ( WINAPI * )( HINSTANCE, LPCSTR ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "LoadCursorA" ) ;
		WinAPIData.Win32Func.LoadCursorWFunc = ( HCURSOR ( WINAPI * )( HINSTANCE, LPCWSTR ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "LoadCursorW" ) ;
		WinAPIData.Win32Func.LoadIconAFunc = ( HICON ( WINAPI * )( HINSTANCE, LPCSTR ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "LoadIconA" ) ;
		WinAPIData.Win32Func.LoadIconWFunc = ( HICON ( WINAPI * )( HINSTANCE, LPCWSTR ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "LoadIconW" ) ;
		WinAPIData.Win32Func.ClipCursorFunc = ( BOOL ( WINAPI * )( CONST RECT * ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "ClipCursor" ) ;
		WinAPIData.Win32Func.LoadMenuAFunc = ( HMENU ( WINAPI * )( HINSTANCE, LPCSTR ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "LoadMenuA" ) ;
		WinAPIData.Win32Func.LoadMenuWFunc = ( HMENU ( WINAPI * )( HINSTANCE, LPCWSTR ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "LoadMenuW" ) ;
		WinAPIData.Win32Func.CreateMenuFunc = ( HMENU ( WINAPI * )( VOID ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "CreateMenu" ) ;
		WinAPIData.Win32Func.DeleteMenuFunc = ( BOOL ( WINAPI * )( HMENU, UINT, UINT ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "DeleteMenu" ) ;
		WinAPIData.Win32Func.DestroyMenuFunc = ( BOOL ( WINAPI * )( HMENU ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "DestroyMenu" ) ;
		WinAPIData.Win32Func.EnableMenuItemFunc = ( BOOL ( WINAPI * )( HMENU, UINT, UINT ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "EnableMenuItem" ) ;
		WinAPIData.Win32Func.GetSubMenuFunc = ( HMENU ( WINAPI * )( HMENU, int ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetSubMenu" ) ;
		WinAPIData.Win32Func.SetMenuItemInfoAFunc = ( BOOL ( WINAPI * )( HMENU, UINT, BOOL, LPCMENUITEMINFOA ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "SetMenuItemInfoA" ) ;
		WinAPIData.Win32Func.SetMenuItemInfoWFunc = ( BOOL ( WINAPI * )( HMENU, UINT, BOOL, LPCMENUITEMINFOW ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "SetMenuItemInfoW" ) ;
		WinAPIData.Win32Func.GetMenuItemInfoAFunc = ( BOOL ( WINAPI * )( HMENU, UINT, BOOL, LPMENUITEMINFOA ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetMenuItemInfoA" ) ;
		WinAPIData.Win32Func.GetMenuItemInfoWFunc = ( BOOL ( WINAPI * )( HMENU, UINT, BOOL, LPMENUITEMINFOW ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetMenuItemInfoW" ) ;
		WinAPIData.Win32Func.SetMenuFunc = ( BOOL ( WINAPI * )( HWND, HMENU ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "SetMenu" ) ;
		WinAPIData.Win32Func.DrawMenuBarFunc = ( BOOL ( WINAPI * )( HWND ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "DrawMenuBar" ) ;
		WinAPIData.Win32Func.GetMenuItemCountFunc = ( int ( WINAPI * )( HMENU ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetMenuItemCount" ) ;
		WinAPIData.Win32Func.InsertMenuItemAFunc = ( BOOL ( WINAPI * )( HMENU, UINT, BOOL, LPCMENUITEMINFOA ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "InsertMenuItemA" ) ;
		WinAPIData.Win32Func.InsertMenuItemWFunc = ( BOOL ( WINAPI * )( HMENU, UINT, BOOL, LPCMENUITEMINFOW ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "InsertMenuItemW" ) ;
		WinAPIData.Win32Func.SetWindowRgnFunc = ( int ( WINAPI * )( HWND, HRGN, BOOL ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "SetWindowRgn" ) ;
		WinAPIData.Win32Func.FillRectFunc = ( int ( WINAPI * )( HDC, CONST RECT *, HBRUSH ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "FillRect" ) ;
		WinAPIData.Win32Func.SystemParametersInfoAFunc = ( BOOL ( WINAPI * )( UINT, UINT, PVOID, UINT ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "SystemParametersInfoA" ) ;
		WinAPIData.Win32Func.SystemParametersInfoWFunc = ( BOOL ( WINAPI * )( UINT, UINT, PVOID, UINT ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "SystemParametersInfoW" ) ;
		WinAPIData.Win32Func.AttachThreadInputFunc = ( BOOL ( WINAPI * )( DWORD, DWORD, BOOL ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "AttachThreadInput" ) ;
#ifndef DX_NON_STOPTASKSWITCH
		WinAPIData.Win32Func.SetWindowsHookExWFunc = ( HHOOK ( WINAPI * )( int, HOOKPROC, HINSTANCE, DWORD ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "SetWindowsHookExW" ) ;
		WinAPIData.Win32Func.UnhookWindowsHookExFunc = ( BOOL ( WINAPI * )( HHOOK ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "UnhookWindowsHookEx" ) ;
#endif // DX_NON_STOPTASKSWITCH
		WinAPIData.Win32Func.GetKeyboardStateFunc = ( BOOL ( WINAPI * )( PBYTE ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetKeyboardState" ) ;
		WinAPIData.Win32Func.KillTimerFunc = ( BOOL ( WINAPI * )( HWND, UINT_PTR ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "KillTimer" ) ;
		WinAPIData.Win32Func.EnumDisplaySettingsAFunc = ( BOOL ( WINAPI * )( LPCSTR, DWORD, D_DEVMODEA* ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "EnumDisplaySettingsA" ) ;
		WinAPIData.Win32Func.EnumDisplaySettingsWFunc = ( BOOL ( WINAPI * )( LPCWSTR, DWORD, D_DEVMODEW* ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "EnumDisplaySettingsW" ) ;
		WinAPIData.Win32Func.EnumDisplayMonitorsFunc = ( BOOL ( WINAPI * )( HDC, LPCRECT, MONITORENUMPROC , LPARAM ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "EnumDisplayMonitors" ) ;
		WinAPIData.Win32Func.IsDialogMessageAFunc = ( BOOL ( WINAPI * )( HWND, LPMSG ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "IsDialogMessageA" ) ;
		WinAPIData.Win32Func.IsDialogMessageWFunc = ( BOOL ( WINAPI * )( HWND, LPMSG ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "IsDialogMessageW" ) ;
		WinAPIData.Win32Func.OpenClipboardFunc = ( BOOL ( WINAPI * )( HWND ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "OpenClipboard" ) ;
		WinAPIData.Win32Func.CloseClipboardFunc = ( BOOL ( WINAPI * )( VOID ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "CloseClipboard" ) ;
		WinAPIData.Win32Func.EmptyClipboardFunc = ( BOOL ( WINAPI * )( VOID ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "EmptyClipboard" ) ;
		WinAPIData.Win32Func.SetClipboardDataFunc = ( HANDLE ( WINAPI * )( UINT, HANDLE ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "SetClipboardData" ) ;
		WinAPIData.Win32Func.GetClipboardDataFunc = ( HANDLE ( WINAPI * )( UINT ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetClipboardData" ) ;
		WinAPIData.Win32Func.IsClipboardFormatAvailableFunc = ( BOOL ( WINAPI * )( UINT ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "IsClipboardFormatAvailable" ) ;
		WinAPIData.Win32Func.CreateAcceleratorTableAFunc = ( HACCEL ( WINAPI * )( LPACCEL, int ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "CreateAcceleratorTableA" ) ;
		WinAPIData.Win32Func.CreateAcceleratorTableWFunc = ( HACCEL ( WINAPI * )( LPACCEL, int ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "CreateAcceleratorTableW" ) ;
		WinAPIData.Win32Func.DestroyAcceleratorTableFunc = ( BOOL ( WINAPI * )( HACCEL ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "DestroyAcceleratorTable" ) ;
		WinAPIData.Win32Func.CopyAcceleratorTableAFunc = ( int ( WINAPI * )( HACCEL, LPACCEL, int ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "CopyAcceleratorTableA" ) ;
		WinAPIData.Win32Func.CopyAcceleratorTableWFunc = ( int ( WINAPI * )( HACCEL, LPACCEL, int ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "CopyAcceleratorTableW" ) ;
		WinAPIData.Win32Func.GetMonitorInfoAFunc = ( BOOL ( WINAPI * )( HMONITOR, D_MONITORINFO * ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetMonitorInfoA" ) ;
		WinAPIData.Win32Func.GetMonitorInfoWFunc = ( BOOL ( WINAPI * )( HMONITOR, D_MONITORINFO * ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetMonitorInfoW" ) ;
		WinAPIData.Win32Func.CharLowerBuffAFunc = ( DWORD ( WINAPI * )( LPSTR, DWORD ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "CharLowerBuffA" ) ;
		WinAPIData.Win32Func.CharLowerBuffWFunc = ( DWORD ( WINAPI * )( LPWSTR, DWORD ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "CharLowerBuffW" ) ;
		WinAPIData.Win32Func.GetRawInputDeviceListFunc = ( UINT ( WINAPI * )( D_RAWINPUTDEVICELIST *, PUINT, UINT ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetRawInputDeviceList" ) ;
		WinAPIData.Win32Func.GetRawInputDeviceInfoAFunc = ( UINT ( WINAPI * )( HANDLE, UINT, LPVOID, PUINT ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetRawInputDeviceInfoA" ) ;



		WinAPIData.Win32Func.CloseTouchInputHandleFunc = ( BOOL ( WINAPI * )( D_HTOUCHINPUT ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "CloseTouchInputHandle" ) ;
		WinAPIData.Win32Func.GetTouchInputInfoFunc     = ( BOOL ( WINAPI * )( D_HTOUCHINPUT, UINT, D_PTOUCHINPUT, int ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "GetTouchInputInfo" ) ;
		WinAPIData.Win32Func.IsTouchWindowFunc         = ( BOOL ( WINAPI * )( HWND, ULONG *) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "IsTouchWindow" ) ;
		WinAPIData.Win32Func.RegisterTouchWindowFunc   = ( BOOL ( WINAPI * )( HWND, ULONG ) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "RegisterTouchWindow" ) ;
		WinAPIData.Win32Func.UnregisterTouchWindowFunc = ( BOOL ( WINAPI * )( HWND) )GetProcAddress( WinAPIData.Win32Func.User32DLL, "UnregisterTouchWindow" ) ;
	}

	// Gdi32DLL をロードする
	if( WinAPIData.Win32Func.GDI32DLL == NULL )
	{
		WinAPIData.Win32Func.GDI32DLL = LoadLibraryA( "gdi32.dll" ) ;
		if( WinAPIData.Win32Func.GDI32DLL != NULL )
		{
			WinAPIData.Win32Func.AddFontMemResourceExFunc = ( HANDLE ( WINAPI * )( LPVOID, DWORD, void /* DESIGNVECTOR */ *, DWORD * ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "AddFontMemResourceEx" ) ;
			WinAPIData.Win32Func.RemoveFontMemResourceExFunc = ( int ( WINAPI * )( HANDLE ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "RemoveFontMemResourceEx" ) ;
			WinAPIData.Win32Func.GetFontUnicodeRangesFunc = ( DWORD  ( WINAPI * )( HDC, D_LPGLYPHSET ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "GetFontUnicodeRanges" ) ;
			WinAPIData.Win32Func.GetDeviceCapsFunc = ( int ( WINAPI * )( HDC, int ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "GetDeviceCaps" ) ;
			WinAPIData.Win32Func.EnumFontFamiliesExAFunc = ( int ( WINAPI * )( HDC, LPLOGFONTA, FONTENUMPROCA, LPARAM, DWORD ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "EnumFontFamiliesExA" ) ;
			WinAPIData.Win32Func.EnumFontFamiliesExWFunc = ( int ( WINAPI * )( HDC, LPLOGFONTW, FONTENUMPROCW, LPARAM, DWORD ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "EnumFontFamiliesExW" ) ;
			WinAPIData.Win32Func.CreateFontAFunc = ( HFONT ( WINAPI * )( int, int, int, int, int, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, LPCSTR ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "CreateFontA" ) ;
			WinAPIData.Win32Func.CreateFontWFunc = ( HFONT ( WINAPI * )( int, int, int, int, int, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, LPCWSTR ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "CreateFontW" ) ;
			WinAPIData.Win32Func.GetTextMetricsAFunc = ( BOOL ( WINAPI * )( HDC, LPTEXTMETRICA ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "GetTextMetricsA" ) ;
			WinAPIData.Win32Func.GetTextMetricsWFunc = ( BOOL ( WINAPI * )( HDC, LPTEXTMETRICW ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "GetTextMetricsW" ) ;
			WinAPIData.Win32Func.GetTextExtentPoint32AFunc = ( BOOL ( WINAPI * )( HDC, LPCSTR, int, LPSIZE ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "GetTextExtentPoint32A" ) ;
			WinAPIData.Win32Func.GetTextExtentPoint32WFunc = ( BOOL ( WINAPI * )( HDC, LPCWSTR, int, LPSIZE ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "GetTextExtentPoint32W" ) ;
			WinAPIData.Win32Func.GetCharacterPlacementAFunc = ( DWORD ( WINAPI * )( HDC, LPCSTR, int, int, LPGCP_RESULTSA, DWORD ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "GetCharacterPlacementA" ) ;
			WinAPIData.Win32Func.GetCharacterPlacementWFunc = ( DWORD ( WINAPI * )( HDC, LPCWSTR, int, int, LPGCP_RESULTSW, DWORD ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "GetCharacterPlacementW" ) ;
			WinAPIData.Win32Func.GetGlyphOutlineAFunc = ( DWORD ( WINAPI * )( HDC, UINT, UINT, LPGLYPHMETRICS, DWORD, LPVOID, CONST MAT2 * ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "GetGlyphOutlineA" ) ;
			WinAPIData.Win32Func.GetGlyphOutlineWFunc = ( DWORD ( WINAPI * )( HDC, UINT, UINT, LPGLYPHMETRICS, DWORD, LPVOID, CONST MAT2 * ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "GetGlyphOutlineW" ) ;
			WinAPIData.Win32Func.TextOutAFunc = ( BOOL ( WINAPI * )( HDC, int, int, LPCSTR, int ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "TextOutA" ) ;
			WinAPIData.Win32Func.TextOutWFunc = ( BOOL ( WINAPI * )( HDC, int, int, LPCWSTR, int ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "TextOutW" ) ;
			WinAPIData.Win32Func.SetTextColorFunc = ( COLORREF ( WINAPI * )( HDC, COLORREF ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "SetTextColor" ) ;
			WinAPIData.Win32Func.SetBkColorFunc = ( COLORREF ( WINAPI * )( HDC, COLORREF ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "SetBkColor" ) ;
			WinAPIData.Win32Func.SetBkModeFunc = ( int ( WINAPI * )( HDC, int ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "SetBkMode" ) ;
			WinAPIData.Win32Func.CreateCompatibleDCFunc = ( HDC ( WINAPI * )( HDC ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "CreateCompatibleDC" ) ;
			WinAPIData.Win32Func.CreateDCAFunc = ( HDC ( WINAPI * )( LPCSTR, LPCSTR, LPCSTR, CONST D_DEVMODEA * ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "CreateDCA" ) ;
			WinAPIData.Win32Func.CreateDCWFunc = ( HDC ( WINAPI * )( LPCWSTR, LPCWSTR, LPCWSTR, CONST D_DEVMODEW * ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "CreateDCW" ) ;
			WinAPIData.Win32Func.GetObjectAFunc = ( int ( WINAPI * )( HANDLE, int, LPVOID ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "GetObjectA" ) ;
			WinAPIData.Win32Func.GetObjectWFunc = ( int ( WINAPI * )( HANDLE, int, LPVOID ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "GetObjectW" ) ;
			WinAPIData.Win32Func.GetStockObjectFunc = ( HGDIOBJ ( WINAPI * )( int ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "GetStockObject" ) ;
			WinAPIData.Win32Func.SelectObjectFunc = ( HGDIOBJ ( WINAPI * )( HDC, HGDIOBJ ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "SelectObject" ) ;
			WinAPIData.Win32Func.DeleteObjectFunc = ( BOOL ( WINAPI * )( HGDIOBJ ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "DeleteObject" ) ;
			WinAPIData.Win32Func.DeleteDCFunc = ( BOOL ( WINAPI * )( HDC ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "DeleteDC" ) ;
			WinAPIData.Win32Func.CreateSolidBrushFunc = ( HBRUSH ( WINAPI * )( COLORREF ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "CreateSolidBrush" ) ;
			WinAPIData.Win32Func.RectangleFunc = ( BOOL ( WINAPI * )( HDC, int, int, int, int ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "Rectangle" ) ;
			WinAPIData.Win32Func.BitBltFunc = ( BOOL ( WINAPI * )( HDC, int, int, int, int, HDC, int, int, DWORD ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "BitBlt" ) ;
			WinAPIData.Win32Func.CreateDIBSectionFunc = ( HBITMAP ( WINAPI * )( HDC, CONST BITMAPINFO *, UINT, VOID **, HANDLE, DWORD ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "CreateDIBSection" ) ;
			WinAPIData.Win32Func.StretchDIBitsFunc = ( int ( WINAPI * )( HDC, int, int, int, int, int, int, int, int, CONST VOID *, CONST BITMAPINFO *, UINT, DWORD ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "StretchDIBits" ) ;
			WinAPIData.Win32Func.SetDIBitsToDeviceFunc = ( int ( WINAPI * )( HDC, int, int, DWORD, DWORD, int, int, UINT, UINT, CONST VOID *, CONST BITMAPINFO *, UINT ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "SetDIBitsToDevice" ) ;
			WinAPIData.Win32Func.CreateRectRgnFunc = ( HRGN ( WINAPI * )( int, int, int, int ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "CreateRectRgn" ) ;
			WinAPIData.Win32Func.CombineRgnFunc = ( int ( WINAPI * )( HRGN, HRGN, HRGN, int ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "CombineRgn" ) ;
			WinAPIData.Win32Func.GetKerningPairsFunc = ( DWORD ( WINAPI * )( HDC, DWORD, LPKERNINGPAIR ) )GetProcAddress( WinAPIData.Win32Func.GDI32DLL, "GetKerningPairsW" ) ;
		}
	}

	// Advapi32DLL をロードする
	if( WinAPIData.Win32Func.ADVAPI32DLL == NULL )
	{
		WinAPIData.Win32Func.ADVAPI32DLL = LoadLibraryA( "advapi32.dll" ) ;
		if( WinAPIData.Win32Func.ADVAPI32DLL != NULL )
		{
			WinAPIData.Win32Func.RegOpenKeyAFunc = ( LONG ( WINAPI * )( HKEY, LPCSTR, PHKEY ) )GetProcAddress( WinAPIData.Win32Func.ADVAPI32DLL, "RegOpenKeyA" ) ;
			WinAPIData.Win32Func.RegOpenKeyWFunc = ( LONG ( WINAPI * )( HKEY, LPCWSTR, PHKEY ) )GetProcAddress( WinAPIData.Win32Func.ADVAPI32DLL, "RegOpenKeyW" ) ;
			WinAPIData.Win32Func.RegEnumKeyAFunc = ( LONG ( WINAPI * )( HKEY, DWORD, LPSTR, DWORD ) )GetProcAddress( WinAPIData.Win32Func.ADVAPI32DLL, "RegEnumKeyA" ) ;
			WinAPIData.Win32Func.RegEnumKeyWFunc = ( LONG ( WINAPI * )( HKEY, DWORD, LPWSTR, DWORD ) )GetProcAddress( WinAPIData.Win32Func.ADVAPI32DLL, "RegEnumKeyW" ) ;
			WinAPIData.Win32Func.RegOpenKeyExAFunc = ( LONG ( WINAPI * )( HKEY, LPCSTR, DWORD, REGSAM, PHKEY ) )GetProcAddress( WinAPIData.Win32Func.ADVAPI32DLL, "RegOpenKeyExA" ) ;
			WinAPIData.Win32Func.RegOpenKeyExWFunc = ( LONG ( WINAPI * )( HKEY, LPCWSTR, DWORD, REGSAM, PHKEY ) )GetProcAddress( WinAPIData.Win32Func.ADVAPI32DLL, "RegOpenKeyExW" ) ;
			WinAPIData.Win32Func.RegQueryValueExAFunc = ( LONG ( WINAPI * )( HKEY, LPCSTR, LPDWORD, LPDWORD, LPBYTE, LPDWORD ) )GetProcAddress( WinAPIData.Win32Func.ADVAPI32DLL, "RegQueryValueExA" ) ;
			WinAPIData.Win32Func.RegQueryValueExWFunc = ( LONG ( WINAPI * )( HKEY, LPCWSTR, LPDWORD, LPDWORD, LPBYTE, LPDWORD ) )GetProcAddress( WinAPIData.Win32Func.ADVAPI32DLL, "RegQueryValueExW" ) ;
		}
	}

	// Shell32DLL をロードする
	if( WinAPIData.Win32Func.SHELL32DLL == NULL )
	{
		WinAPIData.Win32Func.SHELL32DLL = LoadLibraryA( "shell32.dll" ) ;
		if( WinAPIData.Win32Func.SHELL32DLL != NULL )
		{
			WinAPIData.Win32Func.DragQueryFileAFunc = ( UINT ( WINAPI * )( HDROP, UINT, LPSTR, UINT ) )GetProcAddress( WinAPIData.Win32Func.SHELL32DLL, "DragQueryFileA" ) ;
			WinAPIData.Win32Func.DragQueryFileWFunc = ( UINT ( WINAPI * )( HDROP, UINT, LPWSTR, UINT ) )GetProcAddress( WinAPIData.Win32Func.SHELL32DLL, "DragQueryFileW" ) ;
			WinAPIData.Win32Func.DragFinishFunc = ( void ( WINAPI * )( HDROP ) )GetProcAddress( WinAPIData.Win32Func.SHELL32DLL, "DragFinish" ) ;
			WinAPIData.Win32Func.DragAcceptFilesFunc = ( void ( WINAPI * )( HWND, BOOL ) )GetProcAddress( WinAPIData.Win32Func.SHELL32DLL, "DragAcceptFiles" ) ;
		}
	}

	// NTDLL をロードする
	if( WinAPIData.Win32Func.NTDLL == NULL )
	{
		WinAPIData.Win32Func.NTDLL = LoadLibraryA( "ntdll.dll" ) ;
		if( WinAPIData.Win32Func.NTDLL != NULL )
		{
			WinAPIData.Win32Func.RtlGetVersionFunc = ( void ( WINAPI * )( D_OSVERSIONINFOEXW * ) )GetProcAddress( WinAPIData.Win32Func.NTDLL, "RtlGetVersion" ) ;
		}
	}

	// MfplatDLL をロードする
	if( WinAPIData.Win32Func.MFPLATDLL == NULL )
	{
		WinAPIData.Win32Func.MFPLATDLL = LoadLibraryA( "Mfplat.dll" ) ;
		if( WinAPIData.Win32Func.MFPLATDLL != NULL )
		{
			WinAPIData.Win32Func.MFStartupFunc								= ( HRESULT ( WINAPI * )( ULONG, DWORD ) )GetProcAddress( WinAPIData.Win32Func.MFPLATDLL, "MFStartup" ) ;
			WinAPIData.Win32Func.MFShutdownFunc								= ( HRESULT ( WINAPI * )( void ) )GetProcAddress( WinAPIData.Win32Func.MFPLATDLL, "MFShutdown" ) ;
			WinAPIData.Win32Func.MFCreateMediaTypeFunc						= ( HRESULT ( WINAPI * )( D_IMFMediaType ** ) )GetProcAddress( WinAPIData.Win32Func.MFPLATDLL, "MFCreateMediaType" ) ;
			WinAPIData.Win32Func.MFCreateWaveFormatExFromMFMediaTypeFunc	= ( HRESULT ( WINAPI * )( D_IMFMediaType *, WAVEFORMATEX **, UINT32 *, UINT32 ) )GetProcAddress( WinAPIData.Win32Func.MFPLATDLL, "MFCreateWaveFormatExFromMFMediaType" ) ;
			WinAPIData.Win32Func.MFCreateAttributesFunc						= ( HRESULT	( WINAPI * )( D_IMFAttributes**, UINT32 ) )GetProcAddress( WinAPIData.Win32Func.MFPLATDLL, "MFCreateAttributes" ) ;
			WinAPIData.Win32Func.MFCreateAsyncResultFunc					= ( HRESULT ( WINAPI * )( D_IUnknown *, D_IMFAsyncCallback *, D_IUnknown *, D_IMFAsyncResult ** ) )GetProcAddress( WinAPIData.Win32Func.MFPLATDLL, "MFCreateAsyncResult" ) ;
			WinAPIData.Win32Func.MFInvokeCallbackFunc						= ( HRESULT ( WINAPI * )( D_IMFAsyncResult * ) )GetProcAddress( WinAPIData.Win32Func.MFPLATDLL, "MFInvokeCallback" ) ;
		}
	}

	// MfreadwriteDLL をロードする
	if( WinAPIData.Win32Func.MFREADWRITEDLL == NULL )
	{
		WinAPIData.Win32Func.MFREADWRITEDLL = LoadLibraryA( "Mfreadwrite.dll" ) ;
		if( WinAPIData.Win32Func.MFREADWRITEDLL != NULL )
		{
			WinAPIData.Win32Func.MFCreateSourceReaderFromURLFunc			= ( HRESULT ( WINAPI * )( LPCWSTR, D_IMFAttributes *, D_IMFSourceReader ** ) )GetProcAddress( WinAPIData.Win32Func.MFREADWRITEDLL, "MFCreateSourceReaderFromURL" ) ;
			WinAPIData.Win32Func.MFCreateSourceReaderFromByteStreamFunc		= ( HRESULT	( WINAPI * )( D_IMFByteStream *, D_IMFAttributes *, D_IMFSourceReader ** ) )GetProcAddress( WinAPIData.Win32Func.MFREADWRITEDLL, "MFCreateSourceReaderFromByteStream" ) ;
		}
	}

	// PropsysDLL をロードする
	if( WinAPIData.Win32Func.PROPSYSDLL == NULL )
	{
		WinAPIData.Win32Func.PROPSYSDLL = LoadLibraryA( "Propsys.dll" ) ;
		if( WinAPIData.Win32Func.PROPSYSDLL != NULL )
		{
			WinAPIData.Win32Func.PropVariantToInt64Func = ( HRESULT	( WINAPI * )( const D_PROPVARIANT &, LONGLONG * ) )GetProcAddress( WinAPIData.Win32Func.PROPSYSDLL, "PropVariantToInt64" ) ;
		}
	}

	// usp10.dll をロードする
	if( WinAPIData.Win32Func.USP10DLL == NULL )
	{
		WinAPIData.Win32Func.USP10DLL = LoadLibraryA( "usp10.dll" ) ;
		if( WinAPIData.Win32Func.USP10DLL != NULL )
		{
			WinAPIData.Win32Func.ScriptItemizeFunc	= ( HRESULT	( WINAPI * )( const WCHAR *, int, int, const D_SCRIPT_CONTROL *, const D_SCRIPT_STATE *, D_SCRIPT_ITEM *, int * ) )GetProcAddress( WinAPIData.Win32Func.USP10DLL, "ScriptItemize" ) ;
			WinAPIData.Win32Func.ScriptShapeFunc	= ( HRESULT ( WINAPI * )( HDC, void **, const WCHAR *, int, int, D_SCRIPT_ANALYSIS *, WORD *, WORD *, D_SCRIPT_VISATTR *, int * ) )GetProcAddress( WinAPIData.Win32Func.USP10DLL, "ScriptShape" ) ;
		}
	}

	// shcore.dll をロードする
	if( WinAPIData.Win32Func.SHCOREDLL == NULL )
	{
		WinAPIData.Win32Func.SHCOREDLL = LoadLibraryA( "shcore.dll" ) ;
		if( WinAPIData.Win32Func.SHCOREDLL != NULL )
		{
			WinAPIData.Win32Func.SetProcessDpiAwarenessFunc	= ( HRESULT ( WINAPI * )( D_PROCESS_DPI_AWARENESS ) )GetProcAddress( WinAPIData.Win32Func.SHCOREDLL, "SetProcessDpiAwareness" ) ;
			WinAPIData.Win32Func.GetProcessDpiAwarenessFunc	= ( HRESULT ( WINAPI * )( HANDLE, D_PROCESS_DPI_AWARENESS * ) )GetProcAddress( WinAPIData.Win32Func.SHCOREDLL, "GetProcessDpiAwareness" ) ;
			WinAPIData.Win32Func.GetDpiForMonitorFunc = ( HRESULT ( WINAPI * )( HMONITOR, D_MONITOR_DPI_TYPE, UINT *, UINT * ) )GetProcAddress( WinAPIData.Win32Func.SHCOREDLL, "GetDpiForMonitor" ) ;
		}
	}

	// DwmApiDLL をロードする
	if( WinAPIData.DwmApiDLL == NULL )
	{
		WinAPIData.DwmApiDLL = LoadLibraryA( "dwmapi.dll" ) ;
		if( WinAPIData.DwmApiDLL != NULL )
		{
			WinAPIData.DF_DwmEnableComposition = ( HRESULT	( WINAPI * )( UINT ) )GetProcAddress( WinAPIData.DwmApiDLL, "DwmEnableComposition" ) ;
			WinAPIData.DwmGetWindowAttributeFunc = ( HRESULT ( WINAPI * )( HWND, DWORD, PVOID, DWORD ) )GetProcAddress( WinAPIData.DwmApiDLL, "DwmGetWindowAttribute" ) ;
		}
	}

	// ワイド文字のAPIが有効かどうかを GetCurrentDirectoryW で確認する
	{
		wchar_t TempBuffer[ 1024 ] ;

		TempBuffer[ 0 ] = ( wchar_t )0xffff ;
		WinAPIData.Win32Func.GetCurrentDirectoryWFunc( 1024, TempBuffer ) ;
		WinAPIData.EnableWideCharaFunction = TempBuffer[ 0 ] != 0xffff ;
	}

	// 初期化完了フラグを立てる
	WinAPIData.InitializeFlag = TRUE ;

	// 終了
	return 0 ;
}

// WindowsOS の DLL を解放する
extern int ReleaseWinAPI( void )
{
	// 初期化されていなかったら何もしない
	if( WinAPIData.InitializeFlag == FALSE )
	{
		return 0 ;
	}

	// DWMAPI.DLL の解放
	if( WinAPIData.DwmApiDLL )
	{
		FreeLibrary( WinAPIData.DwmApiDLL ) ;
		WinAPIData.DwmApiDLL = NULL ;
		WinAPIData.DF_DwmEnableComposition = NULL ;
	}

	// WinMM DLL を解放する
	if( WinAPIData.Win32Func.WinMMDLL )
	{
		FreeLibrary( WinAPIData.Win32Func.WinMMDLL ) ;
		WinAPIData.Win32Func.WinMMDLL = NULL ;
		WinAPIData.Win32Func.timeSetEventFunc = NULL ;
		WinAPIData.Win32Func.timeKillEventFunc = NULL ;
		WinAPIData.Win32Func.timeBeginPeriodFunc = NULL ;
		WinAPIData.Win32Func.timeEndPeriodFunc = NULL ;
		WinAPIData.Win32Func.timeGetTimeFunc = NULL ;
		WinAPIData.Win32Func.timeGetDevCapsFunc = NULL ;
		WinAPIData.Win32Func.joyGetPosExFunc = NULL ;

		WinAPIData.Win32Func.joyGetDevCapsFunc = NULL ;
		WinAPIData.Win32Func.mciSendCommandFunc = NULL ;

		WinAPIData.Win32Func.waveOutOpenFunc = NULL ;
		WinAPIData.Win32Func.waveOutCloseFunc = NULL ;
		WinAPIData.Win32Func.waveOutPrepareHeaderFunc = NULL ;
		WinAPIData.Win32Func.waveOutUnprepareHeaderFunc = NULL ;
		WinAPIData.Win32Func.waveOutWriteFunc = NULL ;
		WinAPIData.Win32Func.waveOutResetFunc = NULL ;
	}

	// Kernel32 DLL を解放する
	if( WinAPIData.Win32Func.Kernel32DLL )
	{
		FreeLibrary( WinAPIData.Win32Func.Kernel32DLL ) ;
		WinAPIData.Win32Func.Kernel32DLL = NULL ;
		WinAPIData.Win32Func.GetModuleHandleAFunc = NULL ;
		WinAPIData.Win32Func.GetModuleHandleWFunc = NULL ;
		WinAPIData.Win32Func.VerifyVersionInfoAFunc = NULL ;
		WinAPIData.Win32Func.VerifyVersionInfoWFunc = NULL ;
		WinAPIData.Win32Func.VerSetConditionMaskFunc = NULL ;
		WinAPIData.Win32Func.CreateEventAFunc = NULL ;
		WinAPIData.Win32Func.SetEventFunc = NULL ;
		WinAPIData.Win32Func.ResetEventFunc = NULL ;
		WinAPIData.Win32Func.CreateThreadFunc = NULL ;
		WinAPIData.Win32Func.SetThreadPriorityFunc = NULL ;
		WinAPIData.Win32Func.GetThreadPriorityFunc = NULL ;
		WinAPIData.Win32Func.ExitThreadFunc = NULL ;
		WinAPIData.Win32Func.GetExitCodeThreadFunc = NULL ;
		WinAPIData.Win32Func.GetCurrentThreadFunc = NULL ;
		WinAPIData.Win32Func.GetCurrentThreadIdFunc = NULL ;
		WinAPIData.Win32Func.SuspendThreadFunc = NULL ;
		WinAPIData.Win32Func.ResumeThreadFunc = NULL ;
		WinAPIData.Win32Func.WaitForSingleObjectFunc = NULL ;
		WinAPIData.Win32Func.WaitForMultipleObjectsFunc = NULL ;
		WinAPIData.Win32Func.GetProcessHeapFunc = NULL ;
		WinAPIData.Win32Func.HeapAllocFunc = NULL ;
		WinAPIData.Win32Func.HeapFreeFunc = NULL ;
		WinAPIData.Win32Func.VirtualAllocFunc = NULL ;
		WinAPIData.Win32Func.VirtualFreeFunc = NULL ;
		WinAPIData.Win32Func.VirtualQueryFunc = NULL ;
		WinAPIData.Win32Func.GlobalAllocFunc = NULL ;
		WinAPIData.Win32Func.GlobalFreeFunc = NULL ;
		WinAPIData.Win32Func.GlobalLockFunc = NULL ;
		WinAPIData.Win32Func.GlobalUnlockFunc = NULL ;
		WinAPIData.Win32Func.GlobalSizeFunc = NULL ;
		WinAPIData.Win32Func.CreateSemaphoreAFunc = NULL ;
		WinAPIData.Win32Func.ReleaseSemaphoreFunc = NULL ;
		WinAPIData.Win32Func.GetVersionExAFunc = NULL ;
		WinAPIData.Win32Func.GetVersionExWFunc = NULL ;
		WinAPIData.Win32Func.GetSystemInfoFunc = NULL ;
		WinAPIData.Win32Func.lstrlenAFunc = NULL ;
		WinAPIData.Win32Func.lstrlenWFunc = NULL ;
		WinAPIData.Win32Func.lstrcmpAFunc = NULL ;
		WinAPIData.Win32Func.lstrcmpWFunc = NULL ;
		WinAPIData.Win32Func.lstrcpyAFunc = NULL ;
		WinAPIData.Win32Func.lstrcpyWFunc = NULL ;
		WinAPIData.Win32Func.lstrcpynAFunc = NULL ;
		WinAPIData.Win32Func.lstrcpynWFunc = NULL ;
		WinAPIData.Win32Func.GetTickCountFunc = NULL ;
		WinAPIData.Win32Func.QueryPerformanceCounterFunc = NULL ;
		WinAPIData.Win32Func.QueryPerformanceFrequencyFunc = NULL ;
		WinAPIData.Win32Func.CloseHandleFunc = NULL ;
		WinAPIData.Win32Func.GetTempPathAFunc = NULL ;
		WinAPIData.Win32Func.GetTempPathWFunc = NULL ;
		WinAPIData.Win32Func.GetTempFileNameAFunc = NULL ;
		WinAPIData.Win32Func.GetTempFileNameWFunc = NULL ;
		WinAPIData.Win32Func.CreateFileAFunc = NULL ;
		WinAPIData.Win32Func.CreateFileWFunc = NULL ;
		WinAPIData.Win32Func.DeleteFileAFunc = NULL ;
		WinAPIData.Win32Func.DeleteFileWFunc = NULL ;
		WinAPIData.Win32Func.GetFileSizeFunc = NULL ;
		WinAPIData.Win32Func.WriteFileFunc = NULL ;
		WinAPIData.Win32Func.ReadFileFunc = NULL ;
		WinAPIData.Win32Func.SetFilePointerFunc = NULL ;
		WinAPIData.Win32Func.FindFirstFileAFunc = NULL ;
		WinAPIData.Win32Func.FindFirstFileWFunc = NULL ;
		WinAPIData.Win32Func.FindNextFileAFunc = NULL ;
		WinAPIData.Win32Func.FindNextFileWFunc = NULL ;
		WinAPIData.Win32Func.FindCloseFunc = NULL ;
		WinAPIData.Win32Func.SetCurrentDirectoryAFunc = NULL ;
		WinAPIData.Win32Func.SetCurrentDirectoryWFunc = NULL ;
		WinAPIData.Win32Func.GetCurrentDirectoryAFunc = NULL ;
		WinAPIData.Win32Func.GetCurrentDirectoryWFunc = NULL ;
		WinAPIData.Win32Func.SleepFunc = NULL ;
		WinAPIData.Win32Func.InitializeCriticalSectionFunc = NULL ;
		WinAPIData.Win32Func.DeleteCriticalSectionFunc = NULL ;
		WinAPIData.Win32Func.EnterCriticalSectionFunc = NULL ;
		WinAPIData.Win32Func.LeaveCriticalSectionFunc = NULL ;
		WinAPIData.Win32Func.ExitProcessFunc = NULL ;
		WinAPIData.Win32Func.LoadResourceFunc = NULL ;
		WinAPIData.Win32Func.FindResourceAFunc = NULL ;
		WinAPIData.Win32Func.FindResourceWFunc = NULL ;
		WinAPIData.Win32Func.LockResourceFunc = NULL ;
		WinAPIData.Win32Func.SizeofResourceFunc = NULL ;
		WinAPIData.Win32Func.GetOEMCPFunc = NULL ;
		WinAPIData.Win32Func.OutputDebugStringAFunc = NULL ;
		WinAPIData.Win32Func.OutputDebugStringWFunc = NULL ;
		WinAPIData.Win32Func.GlobalMemoryStatusFunc = NULL ;
		WinAPIData.Win32Func.MulDivFunc = NULL ;
		WinAPIData.Win32Func.GetLocalTimeFunc = NULL ;
		WinAPIData.Win32Func.GetSystemPowerStatusFunc = NULL ;
		WinAPIData.Win32Func.FileTimeToLocalFileTimeFunc = NULL ;
		WinAPIData.Win32Func.FileTimeToSystemTimeFunc = NULL ;
		WinAPIData.Win32Func.GetLastErrorFunc = NULL ;
		WinAPIData.Win32Func.MultiByteToWideCharFunc = NULL ;
	}

	// Old32 DLL を解放する
	if( WinAPIData.Win32Func.Old32DLL )
	{
		FreeLibrary( WinAPIData.Win32Func.Old32DLL ) ;
		WinAPIData.Win32Func.Old32DLL = NULL ;
		WinAPIData.Win32Func.CoCreateInstanceFunc = NULL ;
		WinAPIData.Win32Func.CoTaskMemAllocFunc = NULL ;
		WinAPIData.Win32Func.CoTaskMemFreeFunc = NULL ;
		WinAPIData.Win32Func.CoInitializeExFunc = NULL ;
		WinAPIData.Win32Func.CoFreeUnusedLibrariesFunc = NULL ;
		WinAPIData.Win32Func.CoUninitializeFunc = NULL ;
		WinAPIData.Win32Func.CLSIDFromStringFunc = NULL ;
	}

	// OleAut32 DLL を解放する
	if( WinAPIData.Win32Func.OleAut32DLL )
	{
		FreeLibrary( WinAPIData.Win32Func.OleAut32DLL ) ;
		WinAPIData.Win32Func.OleAut32DLL = NULL ;
		WinAPIData.Win32Func.SysAllocStringFunc = NULL ;
		WinAPIData.Win32Func.SysFreeStringFunc = NULL ;
		WinAPIData.Win32Func.VariantClearFunc = NULL ;
		WinAPIData.Win32Func.VariantInitFunc = NULL ;
	}

	// Comctl32 DLL を解放する
	if( WinAPIData.Win32Func.Comctl32DLL )
	{
		FreeLibrary( WinAPIData.Win32Func.Comctl32DLL ) ;
		WinAPIData.Win32Func.Comctl32DLL = NULL ;
		WinAPIData.Win32Func.InitCommonControlsFunc = NULL ;
	}

	// Ole32 DLL を解放する
	if( WinAPIData.Win32Func.Ole32DLL )
	{
		FreeLibrary( WinAPIData.Win32Func.Ole32DLL ) ;
		WinAPIData.Win32Func.Ole32DLL = NULL ;
		WinAPIData.Win32Func.CoSetProxyBlanketFunc = NULL ;
		WinAPIData.Win32Func.PropVariantClearFunc = NULL ;
	}

	// User32.DLL の解放
	if( WinAPIData.Win32Func.User32DLL )
	{
		FreeLibrary( WinAPIData.Win32Func.User32DLL ) ;
		WinAPIData.Win32Func.User32DLL = NULL ;
		WinAPIData.Win32Func.WINNLSEnableIME_Func = NULL ;
		WinAPIData.Win32Func.UpdateLayeredWindow = NULL ;
		WinAPIData.Win32Func.CreateWindowExAFunc = NULL ;
		WinAPIData.Win32Func.CreateWindowExWFunc = NULL ;
		WinAPIData.Win32Func.DestroyWindowFunc = NULL ;
		WinAPIData.Win32Func.ShowWindowFunc = NULL ;
		WinAPIData.Win32Func.UpdateWindowFunc = NULL ;
		WinAPIData.Win32Func.BringWindowToTopFunc = NULL ;
		WinAPIData.Win32Func.SetForegroundWindowFunc = NULL ;
		WinAPIData.Win32Func.GetWindowThreadProcessIdFunc = NULL ;
		WinAPIData.Win32Func.SetActiveWindowFunc = NULL ;
		WinAPIData.Win32Func.GetForegroundWindowFunc = NULL ;
		WinAPIData.Win32Func.GetDesktopWindowFunc = NULL ;
		WinAPIData.Win32Func.FindWindowAFunc = NULL ;
		WinAPIData.Win32Func.FindWindowWFunc = NULL ;
		WinAPIData.Win32Func.MoveWindowFunc = NULL ;
		WinAPIData.Win32Func.SetWindowPosFunc = NULL ;
		WinAPIData.Win32Func.GetClientRectFunc = NULL ;
		WinAPIData.Win32Func.AdjustWindowRectExFunc = NULL ;
		WinAPIData.Win32Func.GetWindowRectFunc = NULL ;
		WinAPIData.Win32Func.SetWindowLongAFunc = NULL ;
		WinAPIData.Win32Func.SetWindowLongWFunc = NULL ;
		WinAPIData.Win32Func.SetWindowLongPtrAFunc = NULL ;
		WinAPIData.Win32Func.SetWindowLongPtrWFunc = NULL ;
		WinAPIData.Win32Func.GetWindowLongAFunc = NULL ;
		WinAPIData.Win32Func.GetWindowLongWFunc = NULL ;
		WinAPIData.Win32Func.GetWindowLongPtrAFunc = NULL ;
		WinAPIData.Win32Func.GetWindowLongPtrWFunc = NULL ;
		WinAPIData.Win32Func.SetWindowTextAFunc = NULL ;
		WinAPIData.Win32Func.SetWindowTextWFunc = NULL ;
		WinAPIData.Win32Func.ClientToScreenFunc = NULL ;
		WinAPIData.Win32Func.EnumDisplayDevicesAFunc = NULL ;
		WinAPIData.Win32Func.EnumDisplayDevicesWFunc = NULL ;
		WinAPIData.Win32Func.CloseTouchInputHandleFunc = NULL ;
		WinAPIData.Win32Func.GetTouchInputInfoFunc = NULL ;
		WinAPIData.Win32Func.IsTouchWindowFunc = NULL ;
		WinAPIData.Win32Func.RegisterTouchWindowFunc = NULL ;
		WinAPIData.Win32Func.UnregisterTouchWindowFunc = NULL ;
		WinAPIData.Win32Func.ShutdownBlockReasonCreateFunc = NULL ;
		WinAPIData.Win32Func.ShutdownBlockReasonDestroyFunc = NULL ;
		WinAPIData.Win32Func.SetTimerFunc = NULL ;
		WinAPIData.Win32Func.PostThreadMessageAFunc = NULL ;
		WinAPIData.Win32Func.PostThreadMessageWFunc = NULL ;
		WinAPIData.Win32Func.PeekMessageAFunc = NULL ;
		WinAPIData.Win32Func.PeekMessageWFunc = NULL ;
		WinAPIData.Win32Func.GetMessageAFunc = NULL ;
		WinAPIData.Win32Func.GetMessageWFunc = NULL ;
		WinAPIData.Win32Func.SendMessageAFunc = NULL ;
		WinAPIData.Win32Func.SendMessageWFunc = NULL ;
		WinAPIData.Win32Func.PostMessageAFunc = NULL ;
		WinAPIData.Win32Func.PostMessageWFunc = NULL ;
		WinAPIData.Win32Func.PostQuitMessageFunc = NULL ;
		WinAPIData.Win32Func.DefWindowProcAFunc = NULL ;
		WinAPIData.Win32Func.DefWindowProcWFunc = NULL ;
		WinAPIData.Win32Func.TranslateAcceleratorAFunc = NULL ;
		WinAPIData.Win32Func.TranslateAcceleratorWFunc = NULL ;
		WinAPIData.Win32Func.TranslateMessageFunc = NULL ;
		WinAPIData.Win32Func.DispatchMessageAFunc = NULL ;
		WinAPIData.Win32Func.DispatchMessageWFunc = NULL ;
		WinAPIData.Win32Func.MsgWaitForMultipleObjectsFunc = NULL ;
		WinAPIData.Win32Func.GetQueueStatusFunc = NULL ;
		WinAPIData.Win32Func.RegisterWindowMessageAFunc = NULL ;
		WinAPIData.Win32Func.RegisterWindowMessageWFunc = NULL ;
		WinAPIData.Win32Func.RegisterClassAFunc = NULL ;
		WinAPIData.Win32Func.RegisterClassWFunc = NULL ;
		WinAPIData.Win32Func.RegisterClassExAFunc = NULL ;
		WinAPIData.Win32Func.RegisterClassExWFunc = NULL ;
		WinAPIData.Win32Func.UnregisterClassAFunc = NULL ;
		WinAPIData.Win32Func.UnregisterClassWFunc = NULL ;
		WinAPIData.Win32Func.SetClassLongAFunc = NULL ;
		WinAPIData.Win32Func.SetClassLongWFunc = NULL ;
		WinAPIData.Win32Func.SetClassLongPtrAFunc = NULL ;
		WinAPIData.Win32Func.SetClassLongPtrWFunc = NULL ;
		WinAPIData.Win32Func.GetDCFunc = NULL ;
		WinAPIData.Win32Func.ReleaseDCFunc = NULL ;
		WinAPIData.Win32Func.BeginPaintFunc = NULL ;
		WinAPIData.Win32Func.EndPaintFunc = NULL ;
		WinAPIData.Win32Func.MessageBoxAFunc = NULL ;
		WinAPIData.Win32Func.MessageBoxWFunc = NULL ;
		WinAPIData.Win32Func.GetSystemMetricsFunc = NULL ;
		WinAPIData.Win32Func.ChangeDisplaySettingsAFunc = NULL ;
		WinAPIData.Win32Func.ChangeDisplaySettingsWFunc = NULL ;
		WinAPIData.Win32Func.ShowCursorFunc = NULL ;
		WinAPIData.Win32Func.SetCursorPosFunc = NULL ;
		WinAPIData.Win32Func.GetCursorPosFunc = NULL ;
		WinAPIData.Win32Func.SetCursorFunc = NULL ;
		WinAPIData.Win32Func.LoadCursorAFunc = NULL ;
		WinAPIData.Win32Func.LoadCursorWFunc = NULL ;
		WinAPIData.Win32Func.LoadIconAFunc = NULL ;
		WinAPIData.Win32Func.LoadIconWFunc = NULL ;
		WinAPIData.Win32Func.ClipCursorFunc = NULL ;
		WinAPIData.Win32Func.LoadMenuAFunc = NULL ;
		WinAPIData.Win32Func.LoadMenuWFunc = NULL ;
		WinAPIData.Win32Func.CreateMenuFunc = NULL ;
		WinAPIData.Win32Func.DeleteMenuFunc = NULL ;
		WinAPIData.Win32Func.DestroyMenuFunc = NULL ;
		WinAPIData.Win32Func.EnableMenuItemFunc = NULL ;
		WinAPIData.Win32Func.GetSubMenuFunc = NULL ;
		WinAPIData.Win32Func.SetMenuItemInfoAFunc = NULL ;
		WinAPIData.Win32Func.SetMenuItemInfoWFunc = NULL ;
		WinAPIData.Win32Func.GetMenuItemInfoAFunc = NULL ;
		WinAPIData.Win32Func.GetMenuItemInfoWFunc = NULL ;
		WinAPIData.Win32Func.SetMenuFunc = NULL ;
		WinAPIData.Win32Func.DrawMenuBarFunc = NULL ;
		WinAPIData.Win32Func.GetMenuItemCountFunc = NULL ;
		WinAPIData.Win32Func.InsertMenuItemAFunc = NULL ;
		WinAPIData.Win32Func.InsertMenuItemWFunc = NULL ;
		WinAPIData.Win32Func.SetWindowRgnFunc = NULL ;
		WinAPIData.Win32Func.FillRectFunc = NULL ;
		WinAPIData.Win32Func.SystemParametersInfoAFunc = NULL ;
		WinAPIData.Win32Func.SystemParametersInfoWFunc = NULL ;
		WinAPIData.Win32Func.AttachThreadInputFunc = NULL ;
#ifndef DX_NON_STOPTASKSWITCH
		WinAPIData.Win32Func.SetWindowsHookExWFunc = NULL ;
		WinAPIData.Win32Func.UnhookWindowsHookExFunc = NULL ;
#endif // DX_NON_STOPTASKSWITCH
		WinAPIData.Win32Func.GetKeyboardStateFunc = NULL ;
		WinAPIData.Win32Func.KillTimerFunc = NULL ;
		WinAPIData.Win32Func.EnumDisplaySettingsAFunc = NULL ;
		WinAPIData.Win32Func.EnumDisplaySettingsWFunc = NULL ;
		WinAPIData.Win32Func.EnumDisplayMonitorsFunc = NULL ;
		WinAPIData.Win32Func.IsDialogMessageAFunc = NULL ;
		WinAPIData.Win32Func.IsDialogMessageWFunc = NULL ;
		WinAPIData.Win32Func.OpenClipboardFunc = NULL ;
		WinAPIData.Win32Func.CloseClipboardFunc = NULL ;
		WinAPIData.Win32Func.EmptyClipboardFunc = NULL ;
		WinAPIData.Win32Func.SetClipboardDataFunc = NULL ;
		WinAPIData.Win32Func.GetClipboardDataFunc = NULL ;
		WinAPIData.Win32Func.IsClipboardFormatAvailableFunc = NULL ;
		WinAPIData.Win32Func.CreateAcceleratorTableAFunc = NULL ;
		WinAPIData.Win32Func.CreateAcceleratorTableWFunc = NULL ;
		WinAPIData.Win32Func.DestroyAcceleratorTableFunc = NULL ;
		WinAPIData.Win32Func.CopyAcceleratorTableAFunc = NULL ;
		WinAPIData.Win32Func.CopyAcceleratorTableWFunc = NULL ;
		WinAPIData.Win32Func.GetMonitorInfoAFunc = NULL ;
		WinAPIData.Win32Func.GetMonitorInfoWFunc = NULL ;
		WinAPIData.Win32Func.CharLowerBuffAFunc = NULL ;
		WinAPIData.Win32Func.CharLowerBuffWFunc = NULL ;
		WinAPIData.Win32Func.GetRawInputDeviceListFunc = NULL ;
		WinAPIData.Win32Func.GetRawInputDeviceInfoAFunc = NULL ;
	}

	// GDI32 DLL を解放する
	if( WinAPIData.Win32Func.GDI32DLL )
	{
		FreeLibrary( WinAPIData.Win32Func.GDI32DLL ) ;
		WinAPIData.Win32Func.GDI32DLL = NULL ;
		WinAPIData.Win32Func.AddFontMemResourceExFunc = NULL ;
		WinAPIData.Win32Func.RemoveFontMemResourceExFunc = NULL ;
		WinAPIData.Win32Func.GetFontUnicodeRangesFunc = NULL ;
		WinAPIData.Win32Func.GetDeviceCapsFunc = NULL ;
		WinAPIData.Win32Func.EnumFontFamiliesExAFunc = NULL ;
		WinAPIData.Win32Func.EnumFontFamiliesExWFunc = NULL ;
		WinAPIData.Win32Func.CreateFontAFunc = NULL ;
		WinAPIData.Win32Func.CreateFontWFunc = NULL ;
		WinAPIData.Win32Func.GetTextMetricsAFunc = NULL ;
		WinAPIData.Win32Func.GetTextMetricsWFunc = NULL ;
		WinAPIData.Win32Func.GetTextExtentPoint32AFunc = NULL ;
		WinAPIData.Win32Func.GetTextExtentPoint32WFunc = NULL ;
		WinAPIData.Win32Func.GetCharacterPlacementAFunc = NULL ;
		WinAPIData.Win32Func.GetCharacterPlacementWFunc = NULL ;
		WinAPIData.Win32Func.GetGlyphOutlineAFunc = NULL ;
		WinAPIData.Win32Func.GetGlyphOutlineWFunc = NULL ;
		WinAPIData.Win32Func.TextOutAFunc = NULL ;
		WinAPIData.Win32Func.TextOutWFunc = NULL ;
		WinAPIData.Win32Func.SetTextColorFunc = NULL ;
		WinAPIData.Win32Func.SetBkColorFunc = NULL ;
		WinAPIData.Win32Func.SetBkModeFunc = NULL ;
		WinAPIData.Win32Func.CreateCompatibleDCFunc = NULL ;
		WinAPIData.Win32Func.CreateDCAFunc = NULL ;
		WinAPIData.Win32Func.CreateDCWFunc = NULL ;
		WinAPIData.Win32Func.GetObjectAFunc = NULL ;
		WinAPIData.Win32Func.GetObjectWFunc = NULL ;
		WinAPIData.Win32Func.GetStockObjectFunc = NULL ;
		WinAPIData.Win32Func.SelectObjectFunc = NULL ;
		WinAPIData.Win32Func.DeleteObjectFunc = NULL ;
		WinAPIData.Win32Func.DeleteDCFunc = NULL ;
		WinAPIData.Win32Func.CreateSolidBrushFunc = NULL ;
		WinAPIData.Win32Func.RectangleFunc = NULL ;
		WinAPIData.Win32Func.BitBltFunc = NULL ;
		WinAPIData.Win32Func.CreateDIBSectionFunc = NULL ;
		WinAPIData.Win32Func.StretchDIBitsFunc = NULL ;
		WinAPIData.Win32Func.SetDIBitsToDeviceFunc = NULL ;
		WinAPIData.Win32Func.CreateRectRgnFunc = NULL ;
		WinAPIData.Win32Func.CombineRgnFunc = NULL ;
		WinAPIData.Win32Func.GetKerningPairsFunc = NULL ;
	}

	// ADVAPI32 DLL を解放する
	if( WinAPIData.Win32Func.ADVAPI32DLL )
	{
		FreeLibrary( WinAPIData.Win32Func.ADVAPI32DLL ) ;
		WinAPIData.Win32Func.ADVAPI32DLL = NULL ;
		WinAPIData.Win32Func.RegOpenKeyAFunc = NULL ;
		WinAPIData.Win32Func.RegOpenKeyWFunc = NULL ;
		WinAPIData.Win32Func.RegEnumKeyAFunc = NULL ;
		WinAPIData.Win32Func.RegEnumKeyWFunc = NULL ;
		WinAPIData.Win32Func.RegOpenKeyExAFunc = NULL ;
		WinAPIData.Win32Func.RegOpenKeyExWFunc = NULL ;
		WinAPIData.Win32Func.RegQueryValueExAFunc = NULL ;
		WinAPIData.Win32Func.RegQueryValueExWFunc = NULL ;
	}

	// SHELL32 DLL を解放する
	if( WinAPIData.Win32Func.SHELL32DLL )
	{
		FreeLibrary( WinAPIData.Win32Func.SHELL32DLL ) ;
		WinAPIData.Win32Func.SHELL32DLL = NULL ;
		WinAPIData.Win32Func.DragQueryFileAFunc = NULL ;
		WinAPIData.Win32Func.DragQueryFileWFunc = NULL ;
		WinAPIData.Win32Func.DragFinishFunc = NULL ;
		WinAPIData.Win32Func.DragAcceptFilesFunc = NULL ;
	}

	// NT DLL を解放する
	if( WinAPIData.Win32Func.NTDLL )
	{
		FreeLibrary( WinAPIData.Win32Func.NTDLL ) ;
		WinAPIData.Win32Func.NTDLL = NULL ;
		WinAPIData.Win32Func.RtlGetVersionFunc = NULL ;
	}

	// Mfplat DLL を解放する
	if( WinAPIData.Win32Func.MFPLATDLL )
	{
		FreeLibrary( WinAPIData.Win32Func.MFPLATDLL ) ;
		WinAPIData.Win32Func.MFPLATDLL = NULL ;
		WinAPIData.Win32Func.MFStartupFunc = NULL ;
		WinAPIData.Win32Func.MFShutdownFunc = NULL ;
		WinAPIData.Win32Func.MFCreateMediaTypeFunc = NULL ;
		WinAPIData.Win32Func.MFCreateWaveFormatExFromMFMediaTypeFunc = NULL ;
		WinAPIData.Win32Func.MFCreateAttributesFunc = NULL ;
		WinAPIData.Win32Func.MFCreateAsyncResultFunc = NULL ;
		WinAPIData.Win32Func.MFInvokeCallbackFunc = NULL ;
	}

	// Mfreadwrite DLL を解放する
	if( WinAPIData.Win32Func.MFREADWRITEDLL )
	{
		FreeLibrary( WinAPIData.Win32Func.MFREADWRITEDLL ) ;
		WinAPIData.Win32Func.MFREADWRITEDLL = NULL ;
		WinAPIData.Win32Func.MFCreateSourceReaderFromURLFunc = NULL ;
		WinAPIData.Win32Func.MFCreateSourceReaderFromByteStreamFunc = NULL ;
	}

	// Propsys DLL を解放する
	if( WinAPIData.Win32Func.PROPSYSDLL )
	{
		FreeLibrary( WinAPIData.Win32Func.PROPSYSDLL ) ;
		WinAPIData.Win32Func.PROPSYSDLL = NULL ;
		WinAPIData.Win32Func.PropVariantToInt64Func = NULL ;
	}

	// USP10 DLL を解放する
	if( WinAPIData.Win32Func.USP10DLL )
	{
		FreeLibrary( WinAPIData.Win32Func.USP10DLL ) ;
		WinAPIData.Win32Func.USP10DLL = NULL ;
		WinAPIData.Win32Func.ScriptItemizeFunc = NULL ;
		WinAPIData.Win32Func.ScriptShapeFunc = NULL ;
	}

	// SHCORE DLL を解放する
	if( WinAPIData.Win32Func.SHCOREDLL )
	{
		FreeLibrary( WinAPIData.Win32Func.SHCOREDLL ) ;
		WinAPIData.Win32Func.SHCOREDLL = NULL ;
		WinAPIData.Win32Func.SetProcessDpiAwarenessFunc = NULL ;
		WinAPIData.Win32Func.GetProcessDpiAwarenessFunc = NULL ;
	}

#ifndef DX_NON_NETWORK
	// ws2_32.dll の解放
	if( WinAPIData.WinSockFunc.WinSockDLL )
	{
		FreeLibrary( WinAPIData.WinSockFunc.WinSockDLL ) ;
		WinAPIData.WinSockFunc.WinSockDLL = NULL ;
		WinAPIData.WinSockFunc.WSAGetLastErrorFunc = NULL ;
		WinAPIData.WinSockFunc.WSAStartupFunc = NULL ;
		WinAPIData.WinSockFunc.WSACleanupFunc = NULL ;
		WinAPIData.WinSockFunc.WSAAsyncSelectFunc = NULL ;
		WinAPIData.WinSockFunc.getaddrinfoFunc = NULL ;
		WinAPIData.WinSockFunc.gethostbyaddrFunc = NULL ;
		WinAPIData.WinSockFunc.gethostbynameFunc = NULL ;
		WinAPIData.WinSockFunc.gethostnameFunc = NULL ;
		WinAPIData.WinSockFunc.ntohsFunc = NULL ;
		WinAPIData.WinSockFunc.htonsFunc = NULL ;
		WinAPIData.WinSockFunc.connectFunc = NULL ;
		WinAPIData.WinSockFunc.socketFunc = NULL ;
		WinAPIData.WinSockFunc.sendFunc = NULL ;
		WinAPIData.WinSockFunc.sendtoFunc = NULL ;
		WinAPIData.WinSockFunc.recvfromFunc = NULL ;
		WinAPIData.WinSockFunc.acceptFunc = NULL ;
		WinAPIData.WinSockFunc.closesocketFunc = NULL ;
		WinAPIData.WinSockFunc.shutdownFunc = NULL ;
		WinAPIData.WinSockFunc.listenFunc = NULL ;
		WinAPIData.WinSockFunc.bindFunc = NULL ;
		WinAPIData.WinSockFunc.inet_addrFunc = NULL ;
		WinAPIData.WinSockFunc.recvFunc = NULL ;
		WinAPIData.WinSockFunc.setsockoptFunc = NULL ;
	}
#endif

#ifndef DX_NON_KEYEX
	// Imm32DLL を解放する
	if( WinAPIData.ImmFunc.Imm32DLL )
	{
		FreeLibrary( WinAPIData.ImmFunc.Imm32DLL ) ;
		WinAPIData.ImmFunc.Imm32DLL = NULL ;
		WinAPIData.ImmFunc.ImmGetContextFunc = NULL ;
		WinAPIData.ImmFunc.ImmReleaseContextFunc = NULL ;
		WinAPIData.ImmFunc.ImmGetOpenStatusFunc = NULL ;
		WinAPIData.ImmFunc.ImmGetConversionStatusFunc = NULL ;
		WinAPIData.ImmFunc.ImmNotifyIMEFunc = NULL ;
		WinAPIData.ImmFunc.ImmSetOpenStatusFunc = NULL ;

		WinAPIData.ImmFunc.ImmGetCandidateListFunc = NULL ;
		WinAPIData.ImmFunc.ImmGetCompositionStringFunc = NULL ;
		WinAPIData.ImmFunc.ImmSetCompositionStringFunc = NULL ;
	}
#endif

	// D3DX9 DLL を解放する
	if( WinAPIData.Win32Func.D3DX9DLL )
	{
		FreeLibrary( WinAPIData.Win32Func.D3DX9DLL ) ;
		WinAPIData.Win32Func.D3DX9DLL = NULL ;
		WinAPIData.Win32Func.D3DXCompileShaderFunc = NULL ;
	}

	// D3DCompiler DLL を解放する
	if( WinAPIData.Win32Func.D3DCOMPILERDLL )
	{
		FreeLibrary( WinAPIData.Win32Func.D3DCOMPILERDLL ) ;
		WinAPIData.Win32Func.D3DCOMPILERDLL = NULL ;
		WinAPIData.Win32Func.D3DCompileFunc = NULL ;
	}

	// 初期化フラグを倒す
	WinAPIData.InitializeFlag = FALSE ;

	// 終了
	return 0 ;
}








// Direct3D 9 用シェーダーコンパイラーを読み込む
extern int LoadDirect3DShaderCompiler( void )
{
	int i ;

	// D3DX9 の DLL を読み込む
	if( WinAPIData.Win32Func.D3DX9DLL == NULL )
	{
		for( i = 0 ; WinAPIData.Win32Func.D3DX9DLL == NULL && D3DX9_DllNameTable[ i ] ; i ++ )
		{
			WinAPIData.Win32Func.D3DX9DLL = LoadLibraryA( D3DX9_DllNameTable[ i ] ) ;
		}
		if( WinAPIData.Win32Func.D3DX9DLL != NULL )
		{
			WinAPIData.Win32Func.D3DXCompileShaderFunc = ( HRESULT ( WINAPI * )( LPCTSTR, UINT, CONST D_D3DXMACRO *, D_ID3DInclude *, LPCSTR, LPCSTR, DWORD, D_ID3DXBuffer **, D_ID3DXBuffer **, D_ID3DXConstantTable ** ) )GetProcAddress( WinAPIData.Win32Func.D3DX9DLL, "D3DXCompileShader" ) ;
		}
	}

	// D3DX9 の DLL の読み込みに失敗したら D3DCompiler を読み込む
	if( WinAPIData.Win32Func.D3DX9DLL == NULL )
	{
		return LoadDirect3D11ShaderCompiler() ;
	}

	// 正常終了
	return 0 ;
}

// Direct3D 11 用シェーダーコンパイラーを読み込む
extern int LoadDirect3D11ShaderCompiler( void )
{
	int i ;

	// D3DCompiler_xx.dll を読み込む
	if( WinAPIData.Win32Func.D3DCOMPILERDLL == NULL )
	{
		for( i = 0 ; WinAPIData.Win32Func.D3DCOMPILERDLL == NULL && D3DCompiler_DllNameTable[ i ] ; i ++ )
		{
			WinAPIData.Win32Func.D3DCOMPILERDLL = LoadLibraryA( D3DCompiler_DllNameTable[ i ] ) ;
		}
		if( WinAPIData.Win32Func.D3DCOMPILERDLL != NULL )
		{
			WinAPIData.Win32Func.D3DCompileFunc = ( HRESULT ( WINAPI * )( LPCVOID, SIZE_T, LPCSTR, CONST D_D3DXMACRO *, D_ID3DInclude *, LPCSTR, LPCSTR, UINT, UINT, D_ID3DBlob **, D_ID3DBlob ** ) )GetProcAddress( WinAPIData.Win32Func.D3DCOMPILERDLL, "D3DCompile" ) ;
		}
	}

	// 正常終了
	return 0 ;
}













#ifndef DX_NON_ASYNCLOAD
extern int WinAPI_CoCreateInstance_ASyncCallback( ASYNCLOAD_MAINTHREAD_REQUESTINFO *Info )
{
	GUID rclsid ;
	GUID riid ;

	rclsid.Data1      = ( unsigned long  )    Info->Data[ 0 ] ;
	rclsid.Data2      = ( unsigned short )(   Info->Data[ 1 ] & 0xffff ) ;
	rclsid.Data3      = ( unsigned short )( ( Info->Data[ 1 ] >> 16 ) & 0xffff ) ;
	rclsid.Data4[ 0 ] = ( unsigned char  )(   Info->Data[ 2 ]         & 0xff ) ;
	rclsid.Data4[ 1 ] = ( unsigned char  )( ( Info->Data[ 2 ] >>  8 ) & 0xff ) ;
	rclsid.Data4[ 2 ] = ( unsigned char  )( ( Info->Data[ 2 ] >> 16 ) & 0xff ) ;
	rclsid.Data4[ 3 ] = ( unsigned char  )( ( Info->Data[ 2 ] >> 24 ) & 0xff ) ;
	rclsid.Data4[ 4 ] = ( unsigned char  )(   Info->Data[ 3 ]         & 0xff ) ;
	rclsid.Data4[ 5 ] = ( unsigned char  )( ( Info->Data[ 3 ] >>  8 ) & 0xff ) ;
	rclsid.Data4[ 6 ] = ( unsigned char  )( ( Info->Data[ 3 ] >> 16 ) & 0xff ) ;
	rclsid.Data4[ 7 ] = ( unsigned char  )( ( Info->Data[ 3 ] >> 24 ) & 0xff ) ;

	riid.Data1        = ( unsigned long  )    Info->Data[ 6 ] ;
	riid.Data2        = ( unsigned short )(   Info->Data[ 7 ] & 0xffff ) ;
	riid.Data3        = ( unsigned short )( ( Info->Data[ 7 ] >> 16 ) & 0xffff ) ;
	riid.Data4[ 0 ]   = ( unsigned char  )(   Info->Data[ 8 ]         & 0xff ) ;
	riid.Data4[ 1 ]   = ( unsigned char  )( ( Info->Data[ 8 ] >>  8 ) & 0xff ) ;
	riid.Data4[ 2 ]   = ( unsigned char  )( ( Info->Data[ 8 ] >> 16 ) & 0xff ) ;
	riid.Data4[ 3 ]   = ( unsigned char  )( ( Info->Data[ 8 ] >> 24 ) & 0xff ) ;
	riid.Data4[ 4 ]   = ( unsigned char  )(   Info->Data[ 9 ]         & 0xff ) ;
	riid.Data4[ 5 ]   = ( unsigned char  )( ( Info->Data[ 9 ] >>  8 ) & 0xff ) ;
	riid.Data4[ 6 ]   = ( unsigned char  )( ( Info->Data[ 9 ] >> 16 ) & 0xff ) ;
	riid.Data4[ 7 ]   = ( unsigned char  )( ( Info->Data[ 9 ] >> 24 ) & 0xff ) ;

	return WinAPI_CoCreateInstance_ASync(
		rclsid,
		( D_IUnknown * )Info->Data[ 4 ],
		( DWORD )Info->Data[ 5 ],
		riid,
		( LPVOID * )Info->Data[ 10 ],
		FALSE ) ;
}
#endif // DX_NON_ASYNCLOAD

extern HRESULT WinAPI_CoCreateInstance_ASync( REFCLSID rclsid, D_IUnknown * pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID *ppv, int ASyncThread )
{
#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		ASYNCLOAD_MAINTHREAD_REQUESTINFO AInfo ;

		AInfo.Function = WinAPI_CoCreateInstance_ASyncCallback ;
		AInfo.Data[  0 ] = ( DWORD_PTR )rclsid.Data1 ;
		AInfo.Data[  1 ] = ( DWORD_PTR )( rclsid.Data2 | ( ( ( DWORD )rclsid.Data3 ) ) << 16 );
		AInfo.Data[  2 ] = ( DWORD_PTR )( rclsid.Data4[ 0 ] | ( ( ( DWORD )rclsid.Data4[ 1 ] ) << 8 ) | ( ( ( DWORD )rclsid.Data4[ 2 ] ) << 16 ) | ( ( ( DWORD )rclsid.Data4[ 3 ] ) << 24 ) ) ;
		AInfo.Data[  3 ] = ( DWORD_PTR )( rclsid.Data4[ 4 ] | ( ( ( DWORD )rclsid.Data4[ 5 ] ) << 8 ) | ( ( ( DWORD )rclsid.Data4[ 6 ] ) << 16 ) | ( ( ( DWORD )rclsid.Data4[ 7 ] ) << 24 ) ) ;
		AInfo.Data[  4 ] = ( DWORD_PTR )pUnkOuter ;
		AInfo.Data[  5 ] = ( DWORD_PTR )dwClsContext ;
		AInfo.Data[  6 ] = ( DWORD_PTR )riid.Data1 ;
		AInfo.Data[  7 ] = ( DWORD_PTR )( riid.Data2 | ( ( ( DWORD )riid.Data3 ) ) << 16 );
		AInfo.Data[  8 ] = ( DWORD_PTR )( riid.Data4[ 0 ] | ( ( ( DWORD )riid.Data4[ 1 ] ) << 8 ) | ( ( ( DWORD )riid.Data4[ 2 ] ) << 16 ) | ( ( ( DWORD )riid.Data4[ 3 ] ) << 24 ) ) ;
		AInfo.Data[  9 ] = ( DWORD_PTR )( riid.Data4[ 4 ] | ( ( ( DWORD )riid.Data4[ 5 ] ) << 8 ) | ( ( ( DWORD )riid.Data4[ 6 ] ) << 16 ) | ( ( ( DWORD )riid.Data4[ 7 ] ) << 24 ) ) ;
		AInfo.Data[ 10 ] = ( DWORD_PTR )ppv ;
		return AddASyncLoadRequestMainThreadInfo( &AInfo ) ;
	}
#endif // DX_NON_ASYNCLOAD

	return WinAPIData.Win32Func.CoCreateInstanceFunc( rclsid, pUnkOuter, dwClsContext, riid, ppv );
}

















































// ラッパー関数
extern HMODULE WINAPI GetModuleHandleWFunc( LPCWSTR lpModuleName )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.GetModuleHandleWFunc( lpModuleName ) ;
	}
	else
	{
		HMODULE Result ;
		WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( lpModuleName, return NULL, CHAR_CHARCODEFORMAT )
		Result = WinAPIData.Win32Func.GetModuleHandleAFunc( UselpModuleNameBuffer ) ;
		WCHAR_T_TO_CHAR_STRING_END( lpModuleName )
		return Result ;
	}
}

extern BOOL WINAPI VerifyVersionInfoWFunc( D_OSVERSIONINFOEXW *lpVersionInformation, DWORD dwTypeMask, DWORDLONG dwlConditionMask )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.VerifyVersionInfoWFunc( lpVersionInformation, dwTypeMask, dwlConditionMask ) ;
	}
	else
	{
		BOOL Result ;
		D_OSVERSIONINFOEXA VersionInformation ;
		VersionInformation.dwOSVersionInfoSize = lpVersionInformation->dwOSVersionInfoSize ;
		VersionInformation.dwMajorVersion      = lpVersionInformation->dwMajorVersion      ;
		VersionInformation.dwMinorVersion      = lpVersionInformation->dwMinorVersion      ;
		VersionInformation.dwBuildNumber       = lpVersionInformation->dwBuildNumber       ;
		VersionInformation.dwPlatformId        = lpVersionInformation->dwPlatformId        ;
		VersionInformation.wServicePackMajor   = lpVersionInformation->wServicePackMajor   ;
		VersionInformation.wServicePackMinor   = lpVersionInformation->wServicePackMinor   ;
		VersionInformation.wSuiteMask          = lpVersionInformation->wSuiteMask          ;
		VersionInformation.wProductType        = lpVersionInformation->wProductType        ;
		VersionInformation.wReserved           = lpVersionInformation->wReserved           ;
		ConvString( ( char * )lpVersionInformation->szCSDVersion, -1, WCHAR_T_CHARCODEFORMAT, VersionInformation.szCSDVersion, sizeof( VersionInformation.szCSDVersion ), CHAR_CHARCODEFORMAT ) ;
		Result = WinAPIData.Win32Func.VerifyVersionInfoAFunc( &VersionInformation, dwTypeMask, dwlConditionMask ) ;
		return Result ;
	}
}

extern BOOL WINAPI GetVersionExWFunc( LPOSVERSIONINFOW lpVersionInformation )
{
	if( lpVersionInformation == NULL )
	{
		return FALSE;
	}

	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.GetVersionExWFunc( lpVersionInformation ) ;
	}
	else
	{
		BOOL Result ;
		OSVERSIONINFOA VersionInformation ;
		VersionInformation.dwOSVersionInfoSize = sizeof( VersionInformation ) ;
		Result = WinAPIData.Win32Func.GetVersionExAFunc( &VersionInformation ) ;
		lpVersionInformation->dwOSVersionInfoSize = VersionInformation.dwOSVersionInfoSize ;
		lpVersionInformation->dwMajorVersion      = VersionInformation.dwMajorVersion      ;
		lpVersionInformation->dwMinorVersion      = VersionInformation.dwMinorVersion      ;
		lpVersionInformation->dwBuildNumber       = VersionInformation.dwBuildNumber       ;
		lpVersionInformation->dwPlatformId        = VersionInformation.dwPlatformId        ;
		ConvString( VersionInformation.szCSDVersion, -1, CHAR_CHARCODEFORMAT, ( char * )lpVersionInformation->szCSDVersion, sizeof( lpVersionInformation->szCSDVersion ), WCHAR_T_CHARCODEFORMAT ) ;
		return Result ;
	}
}

extern int WINAPI lstrlenWFunc( WORD const * lpString )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.lstrlenWFunc( lpString ) ;
	}
	else
	{
		return ( int )CL_strlen( WCHAR_T_CHARCODEFORMAT, ( char * )lpString ) ;
	}
}

extern int WINAPI lstrcmpWFunc( WORD const * lpString1, WORD const * lpString2 )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.lstrcmpWFunc( lpString1, lpString2 ) ;
	}
	else
	{
		return CL_strcmp( WCHAR_T_CHARCODEFORMAT, ( char * )lpString1, ( char * )lpString2 ) ;
	}
}

extern WORD * WINAPI lstrcpyWFunc( WORD * lpString1, WORD const * lpString2 )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.lstrcpyWFunc( lpString1, lpString2 ) ;
	}
	else
	{
		CL_strcpy( WCHAR_T_CHARCODEFORMAT, ( char * )lpString1, ( char * )lpString2 ) ;
		return lpString1 ;
	}
}

extern WORD * WINAPI lstrcpynWFunc( WORD * lpString1, WORD const * lpString2, int iMaxLength )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.lstrcpynWFunc( (WORD *)lpString1, (WORD const *)lpString2, iMaxLength ) ;
	}
	else
	{
		CL_strncpy( WCHAR_T_CHARCODEFORMAT, ( char * )lpString1, ( char * )lpString2, iMaxLength ) ;
		return lpString1 ;
	}
}

extern DWORD WINAPI GetTempPathWFunc( DWORD nBufferLength, LPWSTR lpBuffer )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.GetTempPathWFunc( nBufferLength, lpBuffer ) ;
	}
	else
	{
		DWORD Result ;
		char TempBuffer[ MAX_PATH + 1 ] ;
		Result = WinAPIData.Win32Func.GetTempPathAFunc( sizeof( TempBuffer ), TempBuffer ) ;
		ConvString( TempBuffer, -1, CHAR_CHARCODEFORMAT, ( char * )lpBuffer, nBufferLength, WCHAR_T_CHARCODEFORMAT ) ;
		return Result ;
	}
}

extern UINT WINAPI GetTempFileNameWFunc( LPCWSTR lpPathName, LPCWSTR lpPrefixString, UINT uUnique, LPWSTR lpTempFileName )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.GetTempFileNameWFunc( lpPathName, lpPrefixString, uUnique, lpTempFileName ) ;
	}
	else
	{
		DWORD Result ;
		WCHAR_T_TO_CHAR_STRING_BEGIN( lpPathName )
		WCHAR_T_TO_CHAR_STRING_BEGIN( lpPrefixString )
		WCHAR_T_TO_CHAR_STRING_SETUP( lpPathName, return 0, CHAR_CHARCODEFORMAT )
		WCHAR_T_TO_CHAR_STRING_SETUP( lpPrefixString, return 0, CHAR_CHARCODEFORMAT )
		char TempBuffer[ MAX_PATH + 1 ] ;
		Result = WinAPIData.Win32Func.GetTempFileNameAFunc( UselpPathNameBuffer, UselpPrefixStringBuffer, uUnique, TempBuffer ) ;
		ConvString( TempBuffer, -1, CHAR_CHARCODEFORMAT, ( char * )lpTempFileName, sizeof( MAX_PATH ) * sizeof( wchar_t ), WCHAR_T_CHARCODEFORMAT ) ;
		WCHAR_T_TO_CHAR_STRING_END( lpPathName )
		WCHAR_T_TO_CHAR_STRING_END( lpPrefixString )
		return Result ;
	}
}

extern HANDLE WINAPI CreateFileWFunc( LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.CreateFileWFunc( lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile ) ;
	}
	else
	{
		HANDLE Result ;
		WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( lpFileName, return NULL, CHAR_CHARCODEFORMAT )
		Result = WinAPIData.Win32Func.CreateFileAFunc( UselpFileNameBuffer, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile ) ;
		WCHAR_T_TO_CHAR_STRING_END( lpFileName )
		return Result ;
	}
}

extern BOOL WINAPI DeleteFileWFunc( LPCWSTR lpFileName )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.DeleteFileWFunc( lpFileName ) ;
	}
	else
	{
		BOOL Result ;
		WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( lpFileName, return FALSE, CHAR_CHARCODEFORMAT )
		Result = WinAPIData.Win32Func.DeleteFileAFunc( UselpFileNameBuffer ) ;
		WCHAR_T_TO_CHAR_STRING_END( lpFileName )
		return Result ;
	}
}

extern HANDLE WINAPI FindFirstFileWFunc( LPCWSTR lpFileName, LPWIN32_FIND_DATAW lpFindFileData )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.FindFirstFileWFunc( lpFileName, lpFindFileData ) ;
	}
	else
	{
		HANDLE Result ;
		WIN32_FIND_DATAA FindFileData ;
		WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( lpFileName, return NULL, CHAR_CHARCODEFORMAT )
		Result = WinAPIData.Win32Func.FindFirstFileAFunc( UselpFileNameBuffer, &FindFileData ) ;
		if( lpFindFileData )
		{
			lpFindFileData->dwFileAttributes = FindFileData.dwFileAttributes ;
			lpFindFileData->ftCreationTime = FindFileData.ftCreationTime   ;
			lpFindFileData->ftLastAccessTime = FindFileData.ftLastAccessTime ;
			lpFindFileData->ftLastWriteTime = FindFileData.ftLastWriteTime  ;
			lpFindFileData->nFileSizeHigh = FindFileData.nFileSizeHigh    ;
			lpFindFileData->nFileSizeLow = FindFileData.nFileSizeLow     ;
			lpFindFileData->dwReserved0 = FindFileData.dwReserved0      ;
			lpFindFileData->dwReserved1 = FindFileData.dwReserved1      ;
			ConvString( FindFileData.cFileName, -1, CHAR_CHARCODEFORMAT, ( char * )lpFindFileData->cFileName, sizeof( lpFindFileData->cFileName ), WCHAR_T_CHARCODEFORMAT ) ;
			ConvString( FindFileData.cAlternateFileName, -1, CHAR_CHARCODEFORMAT, ( char * )lpFindFileData->cAlternateFileName, sizeof( lpFindFileData->cAlternateFileName ), WCHAR_T_CHARCODEFORMAT ) ;
		}
		WCHAR_T_TO_CHAR_STRING_END( lpFileName )
		return Result ;
	}
}

extern BOOL WINAPI FindNextFileWFunc( HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.FindNextFileWFunc( hFindFile, lpFindFileData ) ;
	}
	else
	{
		BOOL Result ;
		WIN32_FIND_DATAA FindFileData ;
		Result = WinAPIData.Win32Func.FindNextFileAFunc( hFindFile, &FindFileData ) ;
		if( lpFindFileData )
		{
			lpFindFileData->dwFileAttributes = FindFileData.dwFileAttributes ;
			lpFindFileData->ftCreationTime = FindFileData.ftCreationTime   ;
			lpFindFileData->ftLastAccessTime = FindFileData.ftLastAccessTime ;
			lpFindFileData->ftLastWriteTime = FindFileData.ftLastWriteTime  ;
			lpFindFileData->nFileSizeHigh = FindFileData.nFileSizeHigh    ;
			lpFindFileData->nFileSizeLow = FindFileData.nFileSizeLow     ;
			lpFindFileData->dwReserved0 = FindFileData.dwReserved0      ;
			lpFindFileData->dwReserved1 = FindFileData.dwReserved1      ;
			ConvString( FindFileData.cFileName, -1, CHAR_CHARCODEFORMAT, ( char * )lpFindFileData->cFileName, sizeof( lpFindFileData->cFileName ), WCHAR_T_CHARCODEFORMAT ) ;
			ConvString( FindFileData.cAlternateFileName, -1, CHAR_CHARCODEFORMAT, ( char * )lpFindFileData->cAlternateFileName, sizeof( lpFindFileData->cAlternateFileName ), WCHAR_T_CHARCODEFORMAT ) ;
		}
		return Result ;
	}
}

extern BOOL WINAPI SetCurrentDirectoryWFunc( LPCWSTR lpPathName )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.SetCurrentDirectoryWFunc( lpPathName ) ;
	}
	else
	{
		BOOL Result ;
		WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( lpPathName, return FALSE, CHAR_CHARCODEFORMAT )
		Result = WinAPIData.Win32Func.SetCurrentDirectoryAFunc( UselpPathNameBuffer ) ;
		WCHAR_T_TO_CHAR_STRING_END( lpPathName )
		return Result ;
	}
}

extern DWORD WINAPI GetCurrentDirectoryWFunc( DWORD nBufferLength, LPWSTR lpBuffer )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.GetCurrentDirectoryWFunc( nBufferLength, lpBuffer ) ;
	}
	else
	{
		char Buffer[ 1024 ] ;
		DWORD Result ;
		Result = WinAPIData.Win32Func.GetCurrentDirectoryAFunc( sizeof( Buffer ), Buffer ) ;
		ConvString( Buffer, -1, CHAR_CHARCODEFORMAT, ( char * )lpBuffer, nBufferLength, WCHAR_T_CHARCODEFORMAT ) ;
		return Result * sizeof( wchar_t ) ;
	}
}

extern HRSRC WINAPI FindResourceWFunc( HMODULE hModule, LPCWSTR lpName, LPCWSTR lpType )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.FindResourceWFunc( hModule, lpName, lpType ) ;
	}
	else
	{
		HRSRC Result ;
		WCHAR_T_TO_CHAR_STRING_BEGIN( lpName )
		WCHAR_T_TO_CHAR_STRING_BEGIN( lpType )
		WCHAR_T_TO_CHAR_STRING_SETUP( lpName, return NULL, CHAR_CHARCODEFORMAT )
		WCHAR_T_TO_CHAR_STRING_SETUP( lpType, return NULL, CHAR_CHARCODEFORMAT )
		Result = WinAPIData.Win32Func.FindResourceAFunc( hModule, UselpNameBuffer, UselpTypeBuffer ) ;
		WCHAR_T_TO_CHAR_STRING_END( lpName )
		WCHAR_T_TO_CHAR_STRING_END( lpType )
		return Result ;
	}
}

extern VOID WINAPI OutputDebugStringWFunc( LPCWSTR lpOutputString )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		WinAPIData.Win32Func.OutputDebugStringWFunc( lpOutputString ) ;
	}
	else
	{
		WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( lpOutputString, return, CHAR_CHARCODEFORMAT )
		WinAPIData.Win32Func.OutputDebugStringAFunc( UselpOutputStringBuffer ) ;
		WCHAR_T_TO_CHAR_STRING_END( lpOutputString )
	}
}

extern HWND WINAPI CreateWindowExWFunc( DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.CreateWindowExWFunc( dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam ) ;
	}
	else
	{
		HWND Result ;
		WCHAR_T_TO_CHAR_STRING_BEGIN( lpClassName )
		WCHAR_T_TO_CHAR_STRING_BEGIN( lpWindowName )
		WCHAR_T_TO_CHAR_STRING_SETUP( lpClassName, return NULL, CHAR_CHARCODEFORMAT )
		WCHAR_T_TO_CHAR_STRING_SETUP( lpWindowName, return NULL, CHAR_CHARCODEFORMAT )
		Result = WinAPIData.Win32Func.CreateWindowExAFunc( dwExStyle, UselpClassNameBuffer, UselpWindowNameBuffer, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam ) ;
		WCHAR_T_TO_CHAR_STRING_END( lpClassName )
		WCHAR_T_TO_CHAR_STRING_END( lpWindowName )
		return Result ;
	}
}

extern HWND WINAPI FindWindowWFunc( LPCWSTR lpClassName, LPCWSTR lpWindowName )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.FindWindowWFunc( lpClassName, lpWindowName ) ;
	}
	else
	{
		HWND Result ;
		WCHAR_T_TO_CHAR_STRING_BEGIN( lpClassName )
		WCHAR_T_TO_CHAR_STRING_BEGIN( lpWindowName )
		WCHAR_T_TO_CHAR_STRING_SETUP( lpClassName, return NULL, CHAR_CHARCODEFORMAT )
		WCHAR_T_TO_CHAR_STRING_SETUP( lpWindowName, return NULL, CHAR_CHARCODEFORMAT )
		Result = WinAPIData.Win32Func.FindWindowAFunc( UselpClassNameBuffer, UselpWindowNameBuffer ) ;
		WCHAR_T_TO_CHAR_STRING_END( lpClassName )
		WCHAR_T_TO_CHAR_STRING_END( lpWindowName )
		return Result ;
	}
}

extern LONG WINAPI SetWindowLongWFunc( HWND hWnd, int nIndex, LONG dwNewLong )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.SetWindowLongWFunc( hWnd, nIndex, dwNewLong ) ;
	}
	else
	{
		LONG Result ;
		Result = WinAPIData.Win32Func.SetWindowLongAFunc( hWnd, nIndex, dwNewLong ) ;
		return Result ;
	}
}

extern LONG_PTR WINAPI SetWindowLongPtrWFunc( HWND hWnd, int nIndex, LONG_PTR dwNewLong )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.SetWindowLongPtrWFunc( hWnd, nIndex, dwNewLong ) ;
	}
	else
	{
		LONG_PTR Result ;
		Result = WinAPIData.Win32Func.SetWindowLongPtrAFunc( hWnd, nIndex, dwNewLong ) ;
		return Result ;
	}
}

extern LONG WINAPI GetWindowLongWFunc( HWND hWnd, int nIndex )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.GetWindowLongWFunc( hWnd, nIndex ) ;
	}
	else
	{
		LONG Result ;
		Result = WinAPIData.Win32Func.GetWindowLongAFunc( hWnd, nIndex ) ;
		return Result ;
	}
}

extern LONG_PTR WINAPI GetWindowLongPtrWFunc( HWND hWnd, int nIndex )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.GetWindowLongPtrWFunc( hWnd, nIndex ) ;
	}
	else
	{
		LONG_PTR Result ;
		Result = WinAPIData.Win32Func.GetWindowLongPtrAFunc( hWnd, nIndex ) ;
		return Result ;
	}
}

extern BOOL WINAPI SetWindowTextWFunc( HWND hWnd, LPCWSTR lpString )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.SetWindowTextWFunc( hWnd, lpString ) ;
	}
	else
	{
		BOOL Result ;
		WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( lpString, return FALSE, CHAR_CHARCODEFORMAT )
		Result = WinAPIData.Win32Func.SetWindowTextAFunc( hWnd, UselpStringBuffer ) ;
		WCHAR_T_TO_CHAR_STRING_END( lpString )
		return Result ;
	}
}

extern BOOL WINAPI EnumDisplayDevicesWFunc( LPCWSTR lpDevice, DWORD iDevNum, D_DISPLAY_DEVICEW *lpDisplayDevice, DWORD dwFlags )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.EnumDisplayDevicesWFunc( lpDevice, iDevNum, lpDisplayDevice, dwFlags ) ;
	}
	else
	{
		BOOL Result ;
		D_DISPLAY_DEVICEA DisplayDevice ;
		_MEMSET( &DisplayDevice, 0, sizeof( DisplayDevice ) ) ;
		DisplayDevice.cb = sizeof( DisplayDevice ) ;
		if( lpDevice != NULL )
		{
			WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( lpDevice, return NULL, CHAR_CHARCODEFORMAT )
			Result = WinAPIData.Win32Func.EnumDisplayDevicesAFunc( UselpDeviceBuffer, iDevNum, &DisplayDevice, dwFlags ) ;
			WCHAR_T_TO_CHAR_STRING_END( lpDevice )
		}
		else
		{
			Result = WinAPIData.Win32Func.EnumDisplayDevicesAFunc( NULL, iDevNum, &DisplayDevice, dwFlags ) ;
		}
		if( lpDisplayDevice != NULL )
		{
			lpDisplayDevice->cb         = DisplayDevice.cb         ;
			lpDisplayDevice->StateFlags = DisplayDevice.StateFlags ;
			ConvString( ( char * )DisplayDevice.DeviceName, -1, CHAR_CHARCODEFORMAT, ( char * )lpDisplayDevice->DeviceName, sizeof( lpDisplayDevice->DeviceName ), WCHAR_T_CHARCODEFORMAT ) ;
			ConvString( ( char * )DisplayDevice.DeviceString, -1, CHAR_CHARCODEFORMAT, ( char * )lpDisplayDevice->DeviceString, sizeof( lpDisplayDevice->DeviceString ), WCHAR_T_CHARCODEFORMAT ) ;
			ConvString( DisplayDevice.DeviceID, -1, CHAR_CHARCODEFORMAT, ( char * )lpDisplayDevice->DeviceID, sizeof( lpDisplayDevice->DeviceID ), WCHAR_T_CHARCODEFORMAT ) ;
			ConvString( DisplayDevice.DeviceKey, -1, CHAR_CHARCODEFORMAT, ( char * )lpDisplayDevice->DeviceKey, sizeof( lpDisplayDevice->DeviceKey ), WCHAR_T_CHARCODEFORMAT ) ;
		}
		return Result ;
	}
}

extern BOOL WINAPI PostThreadMessageWFunc( DWORD idThread, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.PostThreadMessageWFunc( idThread, Msg, wParam, lParam ) ;
	}
	else
	{
		return WinAPIData.Win32Func.PostThreadMessageAFunc( idThread, Msg, wParam, lParam ) ;
	}
}

extern BOOL WINAPI PeekMessageWFunc( LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.PeekMessageWFunc( lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg ) ;
	}
	else
	{
		return WinAPIData.Win32Func.PeekMessageAFunc( lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg ) ;
	}
}

extern BOOL WINAPI GetMessageWFunc( LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.GetMessageWFunc( lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax ) ;
	}
	else
	{
		return WinAPIData.Win32Func.GetMessageAFunc( lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax ) ;
	}
}

extern LRESULT WINAPI SendMessageWFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.SendMessageWFunc( hWnd, Msg, wParam, lParam ) ;
	}
	else
	{
		return WinAPIData.Win32Func.SendMessageAFunc( hWnd, Msg, wParam, lParam ) ;
	}
}

extern BOOL WINAPI PostMessageWFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.PostMessageWFunc( hWnd, Msg, wParam, lParam ) ;
	}
	else
	{
		return WinAPIData.Win32Func.PostMessageAFunc( hWnd, Msg, wParam, lParam ) ;
	}
}

extern LRESULT WINAPI DefWindowProcWFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.DefWindowProcWFunc( hWnd, Msg, wParam, lParam ) ;
	}
	else
	{
		return WinAPIData.Win32Func.DefWindowProcAFunc( hWnd, Msg, wParam, lParam ) ;
	}
}

extern int WINAPI TranslateAcceleratorWFunc( HWND hWnd, HACCEL hAccTable, LPMSG lpMsg )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.TranslateAcceleratorWFunc( hWnd, hAccTable, lpMsg ) ;
	}
	else
	{
		return WinAPIData.Win32Func.TranslateAcceleratorAFunc( hWnd, hAccTable, lpMsg ) ;
	}
}

extern LRESULT WINAPI DispatchMessageWFunc( MSG *lpMsg )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.DispatchMessageWFunc( lpMsg ) ;
	}
	else
	{
		return WinAPIData.Win32Func.DispatchMessageAFunc( lpMsg ) ;
	}
}

extern UINT WINAPI RegisterWindowMessageWFunc( LPCWSTR lpString )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.RegisterWindowMessageWFunc( lpString ) ;
	}
	else
	{
		UINT Result ;
		WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( lpString, return 0, CHAR_CHARCODEFORMAT )
		Result = WinAPIData.Win32Func.RegisterWindowMessageAFunc( UselpStringBuffer ) ;
		WCHAR_T_TO_CHAR_STRING_END( lpString )
		return Result ;
	}
}

extern ATOM WINAPI RegisterClassWFunc( CONST WNDCLASSW *lpWndClass )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.RegisterClassWFunc( lpWndClass ) ;
	}
	else
	{
		ATOM Result ;
		WNDCLASSA WndClass ;
		WndClass.style         = lpWndClass->style         ;
		WndClass.lpfnWndProc   = lpWndClass->lpfnWndProc   ;
		WndClass.cbClsExtra    = lpWndClass->cbClsExtra    ;
		WndClass.cbWndExtra    = lpWndClass->cbWndExtra    ;
		WndClass.hInstance     = lpWndClass->hInstance     ;
		WndClass.hIcon         = lpWndClass->hIcon         ;
		WndClass.hCursor       = lpWndClass->hCursor       ;
		WndClass.hbrBackground = lpWndClass->hbrBackground ;
		const wchar_t *lpszMenuName = lpWndClass->lpszMenuName ;
		const wchar_t *lpszClassName = lpWndClass->lpszClassName ;
		WCHAR_T_TO_CHAR_STRING_BEGIN( lpszMenuName )
		WCHAR_T_TO_CHAR_STRING_BEGIN( lpszClassName )
		WCHAR_T_TO_CHAR_STRING_SETUP( lpszMenuName, return 0, CHAR_CHARCODEFORMAT )
		WCHAR_T_TO_CHAR_STRING_SETUP( lpszClassName, return 0, CHAR_CHARCODEFORMAT )
		WndClass.lpszMenuName = UselpszMenuNameBuffer ;
		WndClass.lpszClassName = UselpszClassNameBuffer ;
		Result = WinAPIData.Win32Func.RegisterClassAFunc( &WndClass ) ;
		WCHAR_T_TO_CHAR_STRING_END( lpszMenuName )
		WCHAR_T_TO_CHAR_STRING_END( lpszClassName )
		return Result ;
	}
}

extern ATOM WINAPI RegisterClassExWFunc( CONST WNDCLASSEXW *lpWndClass )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.RegisterClassExWFunc( lpWndClass ) ;
	}
	else
	{
		ATOM Result ;
		WNDCLASSEXA WndClass ;
		WndClass.cbSize        = lpWndClass->cbSize        ;
		WndClass.style         = lpWndClass->style         ;
		WndClass.lpfnWndProc   = lpWndClass->lpfnWndProc   ;
		WndClass.cbClsExtra    = lpWndClass->cbClsExtra    ;
		WndClass.cbWndExtra    = lpWndClass->cbWndExtra    ;
		WndClass.hInstance     = lpWndClass->hInstance     ;
		WndClass.hIcon         = lpWndClass->hIcon         ;
		WndClass.hCursor       = lpWndClass->hCursor       ;
		WndClass.hbrBackground = lpWndClass->hbrBackground ;
		WndClass.hIconSm       = lpWndClass->hIconSm       ;
		const wchar_t *lpszMenuName = lpWndClass->lpszMenuName ;
		const wchar_t *lpszClassName = lpWndClass->lpszClassName ;
		WCHAR_T_TO_CHAR_STRING_BEGIN( lpszMenuName )
		WCHAR_T_TO_CHAR_STRING_BEGIN( lpszClassName )
		WCHAR_T_TO_CHAR_STRING_SETUP( lpszMenuName, return 0, CHAR_CHARCODEFORMAT )
		WCHAR_T_TO_CHAR_STRING_SETUP( lpszClassName, return 0, CHAR_CHARCODEFORMAT )
		WndClass.lpszMenuName = UselpszMenuNameBuffer ;
		WndClass.lpszClassName = UselpszClassNameBuffer ;
		Result = WinAPIData.Win32Func.RegisterClassExAFunc( &WndClass ) ;
		WCHAR_T_TO_CHAR_STRING_END( lpszMenuName )
		WCHAR_T_TO_CHAR_STRING_END( lpszClassName )
		return Result ;
	}
}

extern BOOL WINAPI UnregisterClassWFunc( LPCWSTR lpClassName, HINSTANCE hInstance )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.UnregisterClassWFunc( lpClassName, hInstance ) ;
	}
	else
	{
		BOOL Result ;
		WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( lpClassName, return FALSE, CHAR_CHARCODEFORMAT )
		Result = WinAPIData.Win32Func.UnregisterClassAFunc( UselpClassNameBuffer, hInstance ) ;
		WCHAR_T_TO_CHAR_STRING_END( lpClassName )
		return Result ;
	}
}

extern DWORD WINAPI SetClassLongWFunc( HWND hWnd, int nIndex, LONG dwNewLong )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.SetClassLongWFunc( hWnd, nIndex, dwNewLong ) ;
	}
	else
	{
		return WinAPIData.Win32Func.SetClassLongAFunc( hWnd, nIndex, dwNewLong ) ;
	}
}

extern unsigned LONG_PTR WINAPI SetClassLongPtrWFunc( HWND hWnd, int nIndex, LONG_PTR dwNewLong )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.SetClassLongPtrWFunc( hWnd, nIndex, dwNewLong ) ;
	}
	else
	{
		return WinAPIData.Win32Func.SetClassLongPtrAFunc( hWnd, nIndex, dwNewLong ) ;
	}
}

extern int WINAPI MessageBoxWFunc( HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.MessageBoxWFunc( hWnd, lpText, lpCaption, uType ) ;
	}
	else
	{
		int Result ;
		WCHAR_T_TO_CHAR_STRING_BEGIN( lpText )
		WCHAR_T_TO_CHAR_STRING_BEGIN( lpCaption )
		WCHAR_T_TO_CHAR_STRING_SETUP( lpText, return 0, CHAR_CHARCODEFORMAT )
		WCHAR_T_TO_CHAR_STRING_SETUP( lpCaption, return 0, CHAR_CHARCODEFORMAT )
		Result = WinAPIData.Win32Func.MessageBoxAFunc( hWnd, UselpTextBuffer, UselpCaptionBuffer, uType ) ;
		WCHAR_T_TO_CHAR_STRING_END( lpText )
		WCHAR_T_TO_CHAR_STRING_END( lpCaption )
		return Result ;
	}
}

extern LONG WINAPI ChangeDisplaySettingsWFunc( D_DEVMODEW* lpDevMode, DWORD dwFlags )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.ChangeDisplaySettingsWFunc( lpDevMode, dwFlags ) ;
	}
	else
	{
		D_DEVMODEA DevMode ;
		LONG Result ;
		DevMode.dmSpecVersion           = lpDevMode->dmSpecVersion        ;
		DevMode.dmDriverVersion         = lpDevMode->dmDriverVersion      ;
		DevMode.dmSize                  = lpDevMode->dmSize               ;
		DevMode.dmDriverExtra           = lpDevMode->dmDriverExtra        ;
		DevMode.dmFields                = lpDevMode->dmFields             ;
		DevMode.s1.dmOrientation        = lpDevMode->s1.dmOrientation        ;
		DevMode.s1.dmPaperSize          = lpDevMode->s1.dmPaperSize          ;
		DevMode.s1.dmPaperLength        = lpDevMode->s1.dmPaperLength        ;
		DevMode.s1.dmPaperWidth         = lpDevMode->s1.dmPaperWidth         ;
		DevMode.s1.dmScale              = lpDevMode->s1.dmScale              ;
		DevMode.s1.dmCopies             = lpDevMode->s1.dmCopies             ;
		DevMode.s1.dmDefaultSource      = lpDevMode->s1.dmDefaultSource      ;
		DevMode.s1.dmPrintQuality       = lpDevMode->s1.dmPrintQuality       ;
		DevMode.s2.dmPosition           = lpDevMode->s2.dmPosition           ;
		DevMode.s2.dmDisplayOrientation = lpDevMode->s2.dmDisplayOrientation ;
		DevMode.s2.dmDisplayFixedOutput = lpDevMode->s2.dmDisplayFixedOutput ;
		DevMode.dmColor                 = lpDevMode->dmColor              ;
		DevMode.dmDuplex                = lpDevMode->dmDuplex             ;
		DevMode.dmYResolution           = lpDevMode->dmYResolution        ;
		DevMode.dmTTOption              = lpDevMode->dmTTOption           ;
		DevMode.dmCollate               = lpDevMode->dmCollate            ;
		DevMode.dmLogPixels             = lpDevMode->dmLogPixels          ;
		DevMode.dmBitsPerPel            = lpDevMode->dmBitsPerPel         ;
		DevMode.dmPelsWidth             = lpDevMode->dmPelsWidth          ;
		DevMode.dmPelsHeight            = lpDevMode->dmPelsHeight         ;
		DevMode.dmDisplayFlags          = lpDevMode->dmDisplayFlags       ;
		DevMode.dmNup                   = lpDevMode->dmNup                ;
		DevMode.dmDisplayFrequency      = lpDevMode->dmDisplayFrequency   ;
#if(WINVER >= 0x0400)
		DevMode.dmICMMethod             = lpDevMode->dmICMMethod          ;
		DevMode.dmICMIntent             = lpDevMode->dmICMIntent          ;
		DevMode.dmMediaType             = lpDevMode->dmMediaType          ;
		DevMode.dmDitherType            = lpDevMode->dmDitherType         ;
		DevMode.dmReserved1             = lpDevMode->dmReserved1          ;
		DevMode.dmReserved2             = lpDevMode->dmReserved2          ;
#if (WINVER >= 0x0500) || (_WIN32_WINNT >= _WIN32_WINNT_NT4)
		DevMode.dmPanningWidth          = lpDevMode->dmPanningWidth       ;
		DevMode.dmPanningHeight         = lpDevMode->dmPanningHeight      ;
#endif
#endif /* WINVER >= 0x0400 */
		ConvString( ( char * )lpDevMode->dmDeviceName, -1, WCHAR_T_CHARCODEFORMAT, ( char * )DevMode.dmDeviceName, sizeof( DevMode.dmDeviceName ), CHAR_CHARCODEFORMAT ) ;
		ConvString( ( char * )lpDevMode->dmFormName, -1, WCHAR_T_CHARCODEFORMAT, ( char * )DevMode.dmFormName, sizeof( DevMode.dmFormName ), CHAR_CHARCODEFORMAT ) ;
		Result = WinAPIData.Win32Func.ChangeDisplaySettingsAFunc( &DevMode, dwFlags ) ;
		return Result ;
	}
}

extern HCURSOR WINAPI LoadCursorWFunc( HINSTANCE hInstance, LPCWSTR lpCursorName )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.LoadCursorWFunc( hInstance, lpCursorName ) ;
	}
	else
	{
		return WinAPIData.Win32Func.LoadCursorAFunc( hInstance, ( LPCSTR )lpCursorName ) ;
	}
}

extern HICON WINAPI LoadIconWFunc( HINSTANCE hInstance, LPCWSTR lpIconName )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.LoadIconWFunc( hInstance, lpIconName ) ;
	}
	else
	{
		return WinAPIData.Win32Func.LoadIconAFunc( hInstance, ( LPCSTR )lpIconName ) ;
	}
}

extern HMENU WINAPI LoadMenuWFunc( HINSTANCE hInstance, LPCWSTR lpMenuName )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.LoadMenuWFunc( hInstance, lpMenuName ) ;
	}
	else
	{
		return WinAPIData.Win32Func.LoadMenuAFunc( hInstance, ( LPCSTR )lpMenuName ) ;
	}
}

extern BOOL WINAPI SetMenuItemInfoWFunc( HMENU hmenu, UINT item, BOOL fByPositon, LPCMENUITEMINFOW lpmii )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.SetMenuItemInfoWFunc( hmenu, item, fByPositon, lpmii ) ;
	}
	else
	{
		BOOL Result ;
		MENUITEMINFOA mii ;
		mii.cbSize        = lpmii->cbSize        ;
		mii.fMask         = lpmii->fMask         ;
		mii.fType         = lpmii->fType         ;
		mii.fState        = lpmii->fState        ;
		mii.wID           = lpmii->wID           ;
		mii.hSubMenu      = lpmii->hSubMenu      ;
		mii.hbmpChecked   = lpmii->hbmpChecked   ;
		mii.hbmpUnchecked = lpmii->hbmpUnchecked ;
		mii.dwItemData    = lpmii->dwItemData    ;
		mii.cch           = lpmii->cch           ;
#if(WINVER >= 0x0500)
		mii.hbmpItem      = lpmii->hbmpItem      ;
#endif /* WINVER >= 0x0500 */
		if( ( mii.fMask & MIIM_TYPE ) != 0 && mii.fType == MFT_STRING )
		{
			wchar_t *dwTypeData = lpmii->dwTypeData ;
			WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( dwTypeData, return FALSE, CHAR_CHARCODEFORMAT )
			mii.dwTypeData = UsedwTypeDataBuffer ;
			Result = WinAPIData.Win32Func.SetMenuItemInfoAFunc( hmenu, item, fByPositon, &mii ) ;
			WCHAR_T_TO_CHAR_STRING_END( dwTypeData )
		}
		else
		{
			mii.dwTypeData = ( char * )lpmii->dwTypeData ;
			Result = WinAPIData.Win32Func.SetMenuItemInfoAFunc( hmenu, item, fByPositon, &mii ) ;
		}
		return Result ;
	}
}

extern BOOL WINAPI GetMenuItemInfoWFunc( HMENU hmenu, UINT item, BOOL fByPosition, LPMENUITEMINFOW lpmii )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.GetMenuItemInfoWFunc( hmenu, item, fByPosition, lpmii ) ;
	}
	else
	{
		BOOL Result ;
		MENUITEMINFOA mii ;
		static char NameBuffer[ 1024 ] ;
		mii.cbSize        = lpmii->cbSize        ;
		mii.fMask         = lpmii->fMask         ;
		mii.fType         = lpmii->fType         ;
		mii.fState        = lpmii->fState        ;
		mii.wID           = lpmii->wID           ;
		mii.hSubMenu      = lpmii->hSubMenu      ;
		mii.hbmpChecked   = lpmii->hbmpChecked   ;
		mii.hbmpUnchecked = lpmii->hbmpUnchecked ;
		mii.dwItemData    = lpmii->dwItemData    ;
		mii.cch           = lpmii->cch           ;
#if(WINVER >= 0x0500)
		mii.hbmpItem      = lpmii->hbmpItem      ;
#endif /* WINVER >= 0x0500 */
		if( ( mii.fMask & MIIM_TYPE ) != 0 && mii.fType == MFT_STRING )
		{
			mii.dwTypeData = NameBuffer ;
			Result = WinAPIData.Win32Func.GetMenuItemInfoAFunc( hmenu, item, fByPosition, &mii ) ;
			ConvString( NameBuffer, -1, CHAR_CHARCODEFORMAT, ( char * )lpmii->dwTypeData, lpmii->cch * sizeof( wchar_t ), WCHAR_T_CHARCODEFORMAT ) ;
		}
		else
		{
			mii.dwTypeData = ( char * )lpmii->dwTypeData ;
			Result = WinAPIData.Win32Func.GetMenuItemInfoAFunc( hmenu, item, fByPosition, &mii ) ;
			lpmii->dwTypeData = ( wchar_t * )mii.dwTypeData ;
		}
		lpmii->cbSize        = mii.cbSize        ;
		lpmii->fMask         = mii.fMask         ;
		lpmii->fType         = mii.fType         ;
		lpmii->fState        = mii.fState        ;
		lpmii->wID           = mii.wID           ;
		lpmii->hSubMenu      = mii.hSubMenu      ;
		lpmii->hbmpChecked   = mii.hbmpChecked   ;
		lpmii->hbmpUnchecked = mii.hbmpUnchecked ;
		lpmii->dwItemData    = mii.dwItemData    ;
		lpmii->cch           = mii.cch           ;
#if(WINVER >= 0x0500)
		lpmii->hbmpItem      = mii.hbmpItem      ;
#endif /* WINVER >= 0x0500 */
		return Result ;
	}
}

extern BOOL WINAPI InsertMenuItemWFunc( HMENU hmenu, UINT item, BOOL fByPosition, LPCMENUITEMINFOW lpmi )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.InsertMenuItemWFunc( hmenu, item, fByPosition, lpmi ) ;
	}
	else
	{
		BOOL Result ;
		MENUITEMINFOA mii ;
		mii.cbSize        = lpmi->cbSize        ;
		mii.fMask         = lpmi->fMask         ;
		mii.fType         = lpmi->fType         ;
		mii.fState        = lpmi->fState        ;
		mii.wID           = lpmi->wID           ;
		mii.hSubMenu      = lpmi->hSubMenu      ;
		mii.hbmpChecked   = lpmi->hbmpChecked   ;
		mii.hbmpUnchecked = lpmi->hbmpUnchecked ;
		mii.dwItemData    = lpmi->dwItemData    ;
		mii.cch           = lpmi->cch           ;
#if(WINVER >= 0x0500)
		mii.hbmpItem      = lpmi->hbmpItem      ;
#endif /* WINVER >= 0x0500 */
		if( ( mii.fMask & MIIM_TYPE ) != 0 && mii.fType == MFT_STRING )
		{
			wchar_t *dwTypeData = lpmi->dwTypeData ;
			WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( dwTypeData, return FALSE, CHAR_CHARCODEFORMAT )
			mii.dwTypeData = UsedwTypeDataBuffer ;
			Result = WinAPIData.Win32Func.InsertMenuItemAFunc( hmenu, item, fByPosition, &mii ) ;
			WCHAR_T_TO_CHAR_STRING_END( dwTypeData )
		}
		else
		{
			mii.dwTypeData = ( char * )lpmi->dwTypeData ;
			Result = WinAPIData.Win32Func.InsertMenuItemAFunc( hmenu, item, fByPosition, &mii ) ;
		}
		return Result ;
	}
}

extern BOOL WINAPI SystemParametersInfoWFunc( UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.SystemParametersInfoWFunc( uiAction, uiParam, pvParam, fWinIni ) ;
	}
	else
	{
		return WinAPIData.Win32Func.SystemParametersInfoAFunc( uiAction, uiParam, pvParam, fWinIni ) ;
	}
}

extern BOOL WINAPI EnumDisplaySettingsWFunc( LPCWSTR lpszDeviceName, DWORD iModeNum, D_DEVMODEW* lpDevMode )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.EnumDisplaySettingsWFunc( lpszDeviceName, iModeNum, lpDevMode ) ;
	}
	else
	{
		BOOL Result ;
		D_DEVMODEA DevMode ;
		WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( lpszDeviceName, return FALSE, CHAR_CHARCODEFORMAT )
		Result = WinAPIData.Win32Func.EnumDisplaySettingsAFunc( UselpszDeviceNameBuffer, iModeNum, &DevMode ) ;
		WCHAR_T_TO_CHAR_STRING_END( lpszDeviceName )
		lpDevMode->dmSpecVersion           = DevMode.dmSpecVersion        ;
		lpDevMode->dmDriverVersion         = DevMode.dmDriverVersion      ;
		lpDevMode->dmSize                  = DevMode.dmSize               ;
		lpDevMode->dmDriverExtra           = DevMode.dmDriverExtra        ;
		lpDevMode->dmFields                = DevMode.dmFields             ;
		lpDevMode->s1.dmOrientation        = DevMode.s1.dmOrientation        ;
		lpDevMode->s1.dmPaperSize          = DevMode.s1.dmPaperSize          ;
		lpDevMode->s1.dmPaperLength        = DevMode.s1.dmPaperLength        ;
		lpDevMode->s1.dmPaperWidth         = DevMode.s1.dmPaperWidth         ;
		lpDevMode->s1.dmScale              = DevMode.s1.dmScale              ;
		lpDevMode->s1.dmCopies             = DevMode.s1.dmCopies             ;
		lpDevMode->s1.dmDefaultSource      = DevMode.s1.dmDefaultSource      ;
		lpDevMode->s1.dmPrintQuality       = DevMode.s1.dmPrintQuality       ;
		lpDevMode->s2.dmPosition           = DevMode.s2.dmPosition           ;
		lpDevMode->s2.dmDisplayOrientation = DevMode.s2.dmDisplayOrientation ;
		lpDevMode->s2.dmDisplayFixedOutput = DevMode.s2.dmDisplayFixedOutput ;
		lpDevMode->dmColor                 = DevMode.dmColor              ;
		lpDevMode->dmDuplex                = DevMode.dmDuplex             ;
		lpDevMode->dmYResolution           = DevMode.dmYResolution        ;
		lpDevMode->dmTTOption              = DevMode.dmTTOption           ;
		lpDevMode->dmCollate               = DevMode.dmCollate            ;
		lpDevMode->dmLogPixels             = DevMode.dmLogPixels          ;
		lpDevMode->dmBitsPerPel            = DevMode.dmBitsPerPel         ;
		lpDevMode->dmPelsWidth             = DevMode.dmPelsWidth          ;
		lpDevMode->dmPelsHeight            = DevMode.dmPelsHeight         ;
		lpDevMode->dmDisplayFlags          = DevMode.dmDisplayFlags       ;
		lpDevMode->dmNup                   = DevMode.dmNup                ;
		lpDevMode->dmDisplayFrequency      = DevMode.dmDisplayFrequency   ;
#if(WINVER >= 0x0400)
		lpDevMode->dmICMMethod             = DevMode.dmICMMethod          ;
		lpDevMode->dmICMIntent             = DevMode.dmICMIntent          ;
		lpDevMode->dmMediaType             = DevMode.dmMediaType          ;
		lpDevMode->dmDitherType            = DevMode.dmDitherType         ;
		lpDevMode->dmReserved1             = DevMode.dmReserved1          ;
		lpDevMode->dmReserved2             = DevMode.dmReserved2          ;
#if (WINVER >= 0x0500) || (_WIN32_WINNT >= _WIN32_WINNT_NT4)
		lpDevMode->dmPanningWidth          = DevMode.dmPanningWidth       ;
		lpDevMode->dmPanningHeight         = DevMode.dmPanningHeight      ;
#endif
#endif /* WINVER >= 0x0400 */
		ConvString( ( char * )DevMode.dmDeviceName, -1, CHAR_CHARCODEFORMAT, ( char * )lpDevMode->dmDeviceName, sizeof( lpDevMode->dmDeviceName ), WCHAR_T_CHARCODEFORMAT ) ;
		ConvString( ( char * )DevMode.dmFormName, -1, CHAR_CHARCODEFORMAT, ( char * )lpDevMode->dmFormName, sizeof( lpDevMode->dmFormName ), WCHAR_T_CHARCODEFORMAT ) ;
		return Result ;
	}
}

extern BOOL WINAPI IsDialogMessageWFunc( HWND hDlg, LPMSG lpMsg )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.IsDialogMessageWFunc( hDlg, lpMsg ) ;
	}
	else
	{
		return WinAPIData.Win32Func.IsDialogMessageAFunc( hDlg, lpMsg ) ;
	}
}

extern HACCEL WINAPI CreateAcceleratorTableWFunc( LPACCEL paccel, int cAccel )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.CreateAcceleratorTableWFunc( paccel, cAccel ) ;
	}
	else
	{
		return WinAPIData.Win32Func.CreateAcceleratorTableAFunc( paccel, cAccel ) ;
	}
}

extern int WINAPI CopyAcceleratorTableWFunc( HACCEL hAccelSrc, LPACCEL lpAccelDst, int cAccelEntries )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.CopyAcceleratorTableWFunc( hAccelSrc, lpAccelDst, cAccelEntries ) ;
	}
	else
	{
		return WinAPIData.Win32Func.CopyAcceleratorTableAFunc( hAccelSrc, lpAccelDst, cAccelEntries ) ;
	}
}

extern BOOL WINAPI GetMonitorInfoWFunc( HMONITOR hMonitor, D_MONITORINFO * lpmi )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.GetMonitorInfoWFunc( hMonitor, lpmi ) ;
	}
	else
	{
		if( lpmi->cbSize >= sizeof( D_MONITORINFOEXW ) )
		{
			BOOL Result ;
			D_MONITORINFOEXA miex ;
			_MEMSET( &miex, 0, sizeof( miex ) ) ;
			miex.cbSize = sizeof( D_MONITORINFOEXA ) ;
			Result = WinAPIData.Win32Func.GetMonitorInfoAFunc( hMonitor, ( D_MONITORINFO * )&miex ) ;
			lpmi->rcMonitor = miex.rcMonitor ;
			lpmi->rcWork    = miex.rcWork    ;
			lpmi->dwFlags   = miex.dwFlags   ;
			ConvString( miex.szDevice, -1, CHAR_CHARCODEFORMAT, ( char * )( ( D_MONITORINFOEXW * )lpmi )->szDevice, sizeof( ( ( D_MONITORINFOEXW * )lpmi )->szDevice ), WCHAR_T_CHARCODEFORMAT ) ;
			return Result ;
		}
		else
		{
			return WinAPIData.Win32Func.GetMonitorInfoAFunc( hMonitor, lpmi ) ;
		}
	}
}

extern DWORD WINAPI CharLowerBuffWFunc( LPWSTR lpsz, DWORD cchLength )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.CharLowerBuffWFunc( lpsz, cchLength ) ;
	}
	else
	{
		CL_strlwr( WCHAR_T_CHARCODEFORMAT, ( char * )lpsz ) ;
		return cchLength ;
	}
}

extern int WINAPI EnumFontFamiliesExWFunc( HDC hdc, LPLOGFONTW lpLogfont, FONTENUMPROCW lpProc, LPARAM lParam, DWORD dwFlags )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.EnumFontFamiliesExWFunc( hdc, lpLogfont, lpProc, lParam, dwFlags ) ;
	}
	else
	{
		return -1 ;
	}
}

extern HFONT WINAPI CreateFontWFunc( int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic, DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCWSTR pszFaceName )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.CreateFontWFunc( cHeight, cWidth, cEscapement, cOrientation, cWeight, bItalic, bUnderline, bStrikeOut, iCharSet, iOutPrecision, iClipPrecision, iQuality, iPitchAndFamily, pszFaceName ) ;
	}
	else
	{
		HFONT Result ;
		WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( pszFaceName, return NULL, CHAR_CHARCODEFORMAT )
		Result = WinAPIData.Win32Func.CreateFontAFunc( cHeight, cWidth, cEscapement, cOrientation, cWeight, bItalic, bUnderline, bStrikeOut, iCharSet, iOutPrecision, iClipPrecision, iQuality, iPitchAndFamily, UsepszFaceNameBuffer ) ;
		WCHAR_T_TO_CHAR_STRING_END( pszFaceName )
		return Result ;
	}
}

extern BOOL WINAPI GetTextMetricsWFunc( HDC hdc, LPTEXTMETRICW lptm )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.GetTextMetricsWFunc( hdc, lptm ) ;
	}
	else
	{
		BOOL Result ;
		TEXTMETRICA tm ;
		_MEMSET( &tm, 0, sizeof( tm ) ) ;
		Result = WinAPIData.Win32Func.GetTextMetricsAFunc( hdc, &tm ) ;
		lptm->tmHeight				= tm.tmHeight			;
		lptm->tmAscent				= tm.tmAscent			;
		lptm->tmDescent				= tm.tmDescent			;
		lptm->tmInternalLeading		= tm.tmInternalLeading	;
		lptm->tmExternalLeading		= tm.tmExternalLeading	;
		lptm->tmAveCharWidth		= tm.tmAveCharWidth		;
		lptm->tmMaxCharWidth		= tm.tmMaxCharWidth		;
		lptm->tmWeight				= tm.tmWeight			;
		lptm->tmOverhang			= tm.tmOverhang			;
		lptm->tmDigitizedAspectX	= tm.tmDigitizedAspectX	;
		lptm->tmDigitizedAspectY	= tm.tmDigitizedAspectY	;
		lptm->tmFirstChar			= ( BYTE )tm.tmFirstChar	;
		lptm->tmLastChar			= ( BYTE )tm.tmLastChar		;
		lptm->tmDefaultChar			= ( BYTE )tm.tmDefaultChar	;
		lptm->tmBreakChar			= ( BYTE )tm.tmBreakChar	;
		lptm->tmItalic				= tm.tmItalic			;
		lptm->tmUnderlined			= tm.tmUnderlined		;
		lptm->tmStruckOut			= tm.tmStruckOut		;
		lptm->tmPitchAndFamily		= tm.tmPitchAndFamily	;
		lptm->tmCharSet				= tm.tmCharSet			;
		return Result ;
	}
}

extern BOOL WINAPI GetTextExtentPoint32WFunc( HDC hdc, LPCWSTR lpString, int c, LPSIZE psizl )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.GetTextExtentPoint32WFunc( hdc, lpString, c, psizl ) ;
	}
	else
	{
		BOOL Result ;
		WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( lpString, return FALSE, CHAR_CHARCODEFORMAT )
		Result = WinAPIData.Win32Func.GetTextExtentPoint32AFunc( hdc, UselpStringBuffer, c, psizl ) ;
		WCHAR_T_TO_CHAR_STRING_END( lpString )
		return Result ;
	}
}

extern DWORD WINAPI GetCharacterPlacementWFunc( HDC hdc, LPCWSTR lpString, int nCount, int nMexExtent, LPGCP_RESULTSW lpResults, DWORD dwFlags )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.GetCharacterPlacementWFunc( hdc, lpString, nCount, nMexExtent, lpResults, dwFlags ) ;
	}
	else
	{
		return 0 ;
	}
}

extern DWORD WINAPI GetGlyphOutlineWFunc( HDC hdc, UINT uChar, UINT fuFormat, LPGLYPHMETRICS lpgm, DWORD cjBuffer, LPVOID pvBuffer, CONST MAT2 *lpmat2 )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.GetGlyphOutlineWFunc( hdc, uChar, fuFormat, lpgm, cjBuffer, pvBuffer, lpmat2 ) ;
	}
	else
	{
		DWORD Result ;
		Result = WinAPIData.Win32Func.GetGlyphOutlineAFunc( hdc, ConvCharCode( uChar, WCHAR_T_CHARCODEFORMAT, CHAR_CHARCODEFORMAT ), fuFormat, lpgm, cjBuffer, pvBuffer, lpmat2 ) ; ;
		return Result ;
	}
}

extern BOOL WINAPI TextOutWFunc( HDC hdc, int x, int y, LPCWSTR lpString, int c )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.TextOutWFunc( hdc, x, y, lpString, c ) ;
	}
	else
	{
		BOOL Result ;
		WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( lpString, return FALSE, CHAR_CHARCODEFORMAT )
		Result = WinAPIData.Win32Func.TextOutAFunc( hdc, x, y, UselpStringBuffer, c ) ;
		WCHAR_T_TO_CHAR_STRING_END( lpString )
		return Result ;
	}
}

extern HDC WINAPI CreateDCWFunc( LPCWSTR pwszDriver, LPCWSTR pwszDevice, LPCWSTR pszPort, CONST D_DEVMODEW * pdm )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.CreateDCWFunc( pwszDriver, pwszDevice, pszPort, pdm ) ;
	}
	else
	{
		HDC Result ;
		WCHAR_T_TO_CHAR_STRING_BEGIN( pwszDriver )
		WCHAR_T_TO_CHAR_STRING_BEGIN( pwszDevice )
		WCHAR_T_TO_CHAR_STRING_BEGIN( pszPort )
		WCHAR_T_TO_CHAR_STRING_SETUP( pwszDriver, return NULL, CHAR_CHARCODEFORMAT )
		WCHAR_T_TO_CHAR_STRING_SETUP( pwszDevice, return NULL, CHAR_CHARCODEFORMAT )
		WCHAR_T_TO_CHAR_STRING_SETUP( pszPort, return NULL, CHAR_CHARCODEFORMAT )
		if( pdm != NULL )
		{
			D_DEVMODEA dm ;
			dm.dmSpecVersion           = pdm->dmSpecVersion        ;
			dm.dmDriverVersion         = pdm->dmDriverVersion      ;
			dm.dmSize                  = pdm->dmSize               ;
			dm.dmDriverExtra           = pdm->dmDriverExtra        ;
			dm.dmFields                = pdm->dmFields             ;
			dm.s1.dmOrientation        = pdm->s1.dmOrientation        ;
			dm.s1.dmPaperSize          = pdm->s1.dmPaperSize          ;
			dm.s1.dmPaperLength        = pdm->s1.dmPaperLength        ;
			dm.s1.dmPaperWidth         = pdm->s1.dmPaperWidth         ;
			dm.s1.dmScale              = pdm->s1.dmScale              ;
			dm.s1.dmCopies             = pdm->s1.dmCopies             ;
			dm.s1.dmDefaultSource      = pdm->s1.dmDefaultSource      ;
			dm.s1.dmPrintQuality       = pdm->s1.dmPrintQuality       ;
			dm.s2.dmPosition           = pdm->s2.dmPosition           ;
			dm.s2.dmDisplayOrientation = pdm->s2.dmDisplayOrientation ;
			dm.s2.dmDisplayFixedOutput = pdm->s2.dmDisplayFixedOutput ;
			dm.dmColor                 = pdm->dmColor              ;
			dm.dmDuplex                = pdm->dmDuplex             ;
			dm.dmYResolution           = pdm->dmYResolution        ;
			dm.dmTTOption              = pdm->dmTTOption           ;
			dm.dmCollate               = pdm->dmCollate            ;
			dm.dmLogPixels             = pdm->dmLogPixels          ;
			dm.dmBitsPerPel            = pdm->dmBitsPerPel         ;
			dm.dmPelsWidth             = pdm->dmPelsWidth          ;
			dm.dmPelsHeight            = pdm->dmPelsHeight         ;
			dm.dmDisplayFlags          = pdm->dmDisplayFlags       ;
			dm.dmNup                   = pdm->dmNup                ;
			dm.dmDisplayFrequency      = pdm->dmDisplayFrequency   ;
	#if(WINVER >= 0x0400)
			dm.dmICMMethod             = pdm->dmICMMethod          ;
			dm.dmICMIntent             = pdm->dmICMIntent          ;
			dm.dmMediaType             = pdm->dmMediaType          ;
			dm.dmDitherType            = pdm->dmDitherType         ;
			dm.dmReserved1             = pdm->dmReserved1          ;
			dm.dmReserved2             = pdm->dmReserved2          ;
	#if (WINVER >= 0x0500) || (_WIN32_WINNT >= _WIN32_WINNT_NT4)
			dm.dmPanningWidth          = pdm->dmPanningWidth       ;
			dm.dmPanningHeight         = pdm->dmPanningHeight      ;
	#endif
	#endif /* WINVER >= 0x0400 */
			ConvString( ( char * )pdm->dmDeviceName, -1, WCHAR_T_CHARCODEFORMAT, ( char * )dm.dmDeviceName, sizeof( dm.dmDeviceName ), CHAR_CHARCODEFORMAT ) ;
			ConvString( ( char * )pdm->dmFormName, -1, WCHAR_T_CHARCODEFORMAT, ( char * )dm.dmFormName, sizeof( dm.dmFormName ), CHAR_CHARCODEFORMAT ) ;
			Result = WinAPIData.Win32Func.CreateDCAFunc( UsepwszDriverBuffer, UsepwszDeviceBuffer, UsepszPortBuffer, &dm ) ; ;
		}
		else
		{
			Result = WinAPIData.Win32Func.CreateDCAFunc( UsepwszDriverBuffer, UsepwszDeviceBuffer, UsepszPortBuffer, NULL ) ; ;
		}
		WCHAR_T_TO_CHAR_STRING_END( pwszDriver )
		WCHAR_T_TO_CHAR_STRING_END( pwszDevice )
		WCHAR_T_TO_CHAR_STRING_END( pszPort )
		return Result ;
	}
}

extern LONG WINAPI RegOpenKeyWFunc( HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.RegOpenKeyWFunc( hKey, lpSubKey, phkResult ) ;
	}
	else
	{
		LONG Result ;
		WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( lpSubKey, return -1, CHAR_CHARCODEFORMAT )
		Result = WinAPIData.Win32Func.RegOpenKeyAFunc( hKey, UselpSubKeyBuffer, phkResult ) ;
		WCHAR_T_TO_CHAR_STRING_END( lpSubKey )
		return Result ;
	}
}

extern LONG WINAPI RegEnumKeyWFunc( HKEY hKey, DWORD dwIndex, LPWSTR lpName, DWORD cchName )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.RegEnumKeyWFunc( hKey, dwIndex, lpName, cchName ) ;
	}
	else
	{
		LONG Result ;
		WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( lpName, return -1, CHAR_CHARCODEFORMAT )
		Result = WinAPIData.Win32Func.RegEnumKeyAFunc( hKey, dwIndex, UselpNameBuffer, cchName ) ;
		WCHAR_T_TO_CHAR_STRING_END( lpName )
		return Result ;
	}
}

extern LONG WINAPI RegOpenKeyExWFunc( HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.RegOpenKeyExWFunc( hKey, lpSubKey, ulOptions, samDesired, phkResult ) ;
	}
	else
	{
		LONG Result ;
		WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( lpSubKey, return -1, CHAR_CHARCODEFORMAT )
		Result = WinAPIData.Win32Func.RegOpenKeyExAFunc( hKey, UselpSubKeyBuffer, ulOptions, samDesired, phkResult ) ;
		WCHAR_T_TO_CHAR_STRING_END( lpSubKey )
		return Result ;
	}
}

extern LONG WINAPI RegQueryValueExWFunc( HKEY hKey, LPCWSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.RegQueryValueExWFunc( hKey, lpValueName, lpReserved, lpType, lpData, lpcbData ) ;
	}
	else
	{
		LONG Result ;
		WCHAR_T_TO_CHAR_STRING_ONE_BEGIN( lpValueName, return -1, CHAR_CHARCODEFORMAT )
		Result = WinAPIData.Win32Func.RegQueryValueExAFunc( hKey, UselpValueNameBuffer, lpReserved, lpType, lpData, lpcbData ) ;
		WCHAR_T_TO_CHAR_STRING_END( lpValueName )
		return Result ;
	}
}

extern UINT WINAPI DragQueryFileWFunc( HDROP hDrop, UINT iFile, LPWSTR lpszFile, UINT cch )
{
	if( WinAPIData.EnableWideCharaFunction )
	{
		return WinAPIData.Win32Func.DragQueryFileWFunc( hDrop, iFile, lpszFile, cch ) ;
	}
	else
	{
		UINT Result ;
		if( lpszFile == NULL )
		{
			Result = WinAPIData.Win32Func.DragQueryFileAFunc( hDrop, iFile, NULL, cch ) ;
		}
		else
		{
			char szFile[ 1024 ] ;
			Result = WinAPIData.Win32Func.DragQueryFileAFunc( hDrop, iFile, szFile, cch ) ;
			ConvString( szFile, -1, CHAR_CHARCODEFORMAT, ( char * )lpszFile, cch * sizeof( wchar_t ), WCHAR_T_CHARCODEFORMAT ) ;
		}
		return Result ;
	}
}









#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

