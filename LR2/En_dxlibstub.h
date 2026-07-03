#pragma once

#ifndef _WIN32
#include <DxLib.h>

#include <cstddef>
#include <cstdio>

using std::size_t;

inline int DeleteKeyInput(int InputHandle) {
  ErrorLogFmtAdd("STUB!!! DeleteKeyInput(InputHandle=%d)", InputHandle);
  return -1;
}

inline int InitKeyInput(void) {
  ErrorLogFmtAdd("STUB!!! InitKeyInput()");
  return -1;
}

inline int MakeKeyInput(size_t MaxStrLength, int CancelValidFlag,
                        int SingleCharOnlyFlag, int NumCharOnlyFlag,
                        int DoubleCharOnlyFlag = 0, int EnableNewLineFlag = 0) {
  ErrorLogFmtAdd(
    "STUB!!! MakeKeyInput(MaxStrLength=%zu, CancelValidFlag=%d, SingleCharOnlyFlag=%d, NumCharOnlyFlag=%d, DoubleCharOnlyFlag=%d, EnableNewLineFlag=%d)",
    MaxStrLength, CancelValidFlag, SingleCharOnlyFlag, NumCharOnlyFlag,
    DoubleCharOnlyFlag, EnableNewLineFlag
  );
  return -1;
}

inline int SetActiveKeyInput(int InputHandle) {
  ErrorLogFmtAdd("STUB!!! SetActiveKeyInput(InputHandle=%d)", InputHandle);
  return -1;
}

inline int SetKeyInputString(const char *String, int InputHandle) {
  ErrorLogFmtAdd("STUB!!! SetKeyInputString(String=%s, InputHandle=%d)", String ? String : "(null)", InputHandle);
  return -1;
}

inline int GetKeyInputString(char *StrBuffer, int InputHandle) {
  ErrorLogFmtAdd("STUB!!! GetKeyInputString(StrBuffer=%%p, InputHandle=%d)", InputHandle);
  if (StrBuffer)
	  StrBuffer[0] = '\0';
  return -1;
}

inline int CheckKeyInput(int InputHandle) {
  ErrorLogFmtAdd("STUB!!! CheckKeyInput(InputHandle=%d)", InputHandle);
  return -1;
}

inline const IMEINPUTDATA *GetIMEInputData(void) {
  ErrorLogFmtAdd("STUB!!! GetIMEInputData()");
  return nullptr;
}

inline int GetKeyInputCursorPosition(int InputHandle) {
  ErrorLogFmtAdd("STUB!!! GetKeyInputCursorPosition(InputHandle=%d)", InputHandle);
  return -1;
}

inline int SetKeyInputStringFont(int FontHandle) {
  ErrorLogFmtAdd("STUB!!! SetKeyInputStringFont(FontHandle=%d)", FontHandle);
  return -1;
}

inline int DrawKeyInputString(int x, int y, int InputHandle,
                               int DrawCandidateList = 0) {
  ErrorLogFmtAdd("STUB!!! DrawKeyInputString(x=%d, y=%d, InputHandle=%d, DrawCandidateList=%d)",
              x, y, InputHandle, DrawCandidateList);
  return -1;
}

inline int SetWindowSizeChangeEnableFlag(int Flag, int FitScreen) {
  ErrorLogFmtAdd("STUB!!! SetWindowSizeChangeEnableFlag(Flag=%d, FitScreen=%d)", Flag, FitScreen);
  return -1;
}

inline int SetNotSoundFlag(int Flag) {
  ErrorLogFmtAdd("STUB!!! SetNotSoundFlag(Flag=%d)", Flag);
  return -1;
}

inline int SetWindowSizeExtendRate(double ExRateX, double ExRateY = -1.0) {
  ErrorLogFmtAdd("STUB!!! SetWindowSizeExtendRate(ExRateX=%f, ExRateY=%f)", ExRateX, ExRateY);
  return -1;
}

inline int SetWindowPosition(int x, int y) {
  ErrorLogFmtAdd("STUB!!! SetWindowPosition(x=%d, y=%d)", x, y);
  return -1;
}

inline int ChangeWindowMode(int Flag) {
  ErrorLogFmtAdd("STUB!!! ChangeWindowMode(Flag=%d)", Flag);
  return -1;
}

inline int GetDirectDrawDeviceNum(void) {
  ErrorLogFmtAdd("STUB!!! GetDirectDrawDeviceNum()");
  return -1;
}

inline int SetUseDirectDrawDeviceIndex(int Index) {
  ErrorLogFmtAdd("STUB!!! SetUseDirectDrawDeviceIndex(Index=%d)", Index);
  return -1;
}

inline int SetMultiThreadFlag(int Flag) {
  ErrorLogFmtAdd("STUB!!! SetMultiThreadFlag(Flag=%d)", Flag);
  return -1;
}

inline int SetUseFPUPreserveFlag(int Flag) {
  ErrorLogFmtAdd("STUB!!! SetUseFPUPreserveFlag(Flag=%d)", Flag);
  return -1;
}

inline int SetUseDirectInputFlag(int UseFlag) {
  ErrorLogFmtAdd("STUB!!! SetUseDirectInputFlag(UseFlag=%d)", UseFlag);
  return -1;
}

inline int SetUseTimeStampFlag(int UseFlag) {
  ErrorLogFmtAdd("STUB!!! SetUseTimeStampFlag(UseFlag=%d)", UseFlag);
  return -1;
}

inline int SetUseDirect3DVersion(int Version) {
  ErrorLogFmtAdd("STUB!!! SetUseDirect3DVersion(Version=%d)", Version);
  return -1;
}

inline int SetSysCommandOffFlag(int Flag, const char *HookDllPath = nullptr) {
  ErrorLogFmtAdd("STUB!!! SetSysCommandOffFlag(Flag=%d, HookDllPath=%s)", Flag, HookDllPath ? HookDllPath : "(null)");
  return -1;
}

inline int GetIMEInputModeStr(char *GetBuffer) {
  ErrorLogFmtAdd("STUB!!! GetIMEInputModeStr(GetBuffer=%%p)");
  if (GetBuffer)
    GetBuffer[0] = '\0';
  return -1;
}

inline int GetWindowPosition(int *x, int *y) {
  ErrorLogFmtAdd("STUB!!! GetWindowPosition(x=%%p, y=%%p)");
  if (x)
	  *x = 0;
  if (y)
	  *y = 0;
  return -1;
}

// \retval 0 None
inline int GetUseDirect3DVersion() { return 0; }
// \retval 0 Inactive
// \retval 1 Active
inline int GetWindowActiveFlag() { return 1; }
// we are windowed
inline int GetWindowModeFlag(void) { return 1; }
#endif // _WIN32
