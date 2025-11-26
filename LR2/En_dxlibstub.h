#pragma once

#ifndef _WIN32
#include <DxLib/DxLib.h>

#include <cstddef>

using std::size_t;

// DxLib stubs
inline int DeleteKeyInput(int InputHandle) {
  return -1;
}
inline int InitKeyInput(void) {
  return -1;
}
inline int MakeKeyInput(size_t MaxStrLength, int CancelValidFlag,
                 int SingleCharOnlyFlag, int NumCharOnlyFlag,
                 int DoubleCharOnlyFlag = 0, int EnableNewLineFlag = 0) {
  return -1;
}
inline int SetActiveKeyInput(int InputHandle) {
  return -1;
}
inline int SetKeyInputString(const TCHAR *String, int InputHandle) {
  return -1;
}
inline int GetKeyInputString(TCHAR *StrBuffer, int InputHandle) {
  return -1;
}
inline int CheckKeyInput(int InputHandle) {
  return -1;
}
inline const IMEINPUTDATA *GetIMEInputData(void) {
  return nullptr;
}
inline int GetKeyInputCursorPosition(int InputHandle) {
  return -1;
}
inline int SetKeyInputStringFont(int FontHandle) {
  return -1;
}
inline int DrawKeyInputString(int x, int y, int InputHandle,
                              int DrawCandidateList = 0) {
  return -1;
}
inline int GetIMEInputModeStr(TCHAR *GetBuffer) {
  return -1;
}

// \retval 0 Inactive
// \retval 1 Active
inline int GetWindowActiveFlag() {
	return 1;
}
#endif // _WIN32
