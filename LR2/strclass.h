#pragma once
#pragma warning(disable:4996)
#include <stdio.h>
#include <malloc.h>
typedef unsigned int uint;
using DWORD = unsigned long;

class CSTR {
	public:
		char* body;
	public:
		CSTR(int size);
		~CSTR();
		int length() const;
		unsigned int CRC32() const;
		int msize();
		bool resize(size_t size);
		//CSTR* add(const char *str, int len);
		CSTR& add(const char *str, int len);
		uint checkValidPos(int *pos, int *len);
		CSTR& replace(const char *str1, const char *str2);
		void resize2(int size);
		CSTR& fillzero();
		char* writeAtPos(int pos, const char ch);
		CSTR& upper();
		CSTR& lower();
		CSTR& nullAtPos(int pos);
		int lastSpaceCount();
		int findStrPos(const char *str);
		int findChrBackPos(const char ch);

		int toFile(const char *filepath);
		char* outstr(); operator char*(); operator const char*(); //duplicated by converting man. delete one later
		CSTR& add(CSTR *param);
		CSTR& add(const char *str);
		bool isSame(const char *str);
		bool isSame(CSTR *str);
		int isDiff(const char *str);
		int isDiff(CSTR *str);
		int isDiffLow(CSTR *str);
		int isDiffHigh(CSTR *str);
		char* atPos(int pos);
		bool canOpenFile();
		CSTR();
		CSTR(const CSTR &copy, int len = 0);
		CSTR(const char *str, int len = 0);
		CSTR& assign(const char *str, int len);
		int icmp(CSTR *param_1);
		CSTR left (int len);
		CSTR right(int len);
		CSTR getSliced(int pos, int len);
		//CSTR& getSliced(CSTR *oBuf, int pos, int len);
		CSTR& assign(const CSTR *iBuf); CSTR& operator=(const CSTR *iBuf); CSTR& operator=(CSTR &iBuf);//duplicated by converting man. delete one later
		CSTR& assign(const char *str); CSTR& operator=(const char *str);//duplicated by converting man. delete one later
		CSTR getDirectory();
		CSTR getParentDirectory();
		CSTR& cutDirectorySeparator();
		CSTR& lastCut(int len);
		CSTR& trimWhiteSpace();
		CSTR getFilename();

		// Methods with same interface as std::string for easier transition later
		const char* c_str() const {
			return body;
		};
		bool starts_with(const char*);
};

char *cstrSprintf(CSTR *str, const char *format, ...)
#ifndef _MSC_VER
	__attribute__((format(printf, 2, 3)))
#endif // _MSC_VER
	;
