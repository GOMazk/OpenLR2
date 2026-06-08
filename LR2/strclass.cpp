#include "strclass.h"
#include "filesystem.h"
#include <cstring>
#include <span>
#include <string>
#include <string_view>
#include <cstdarg>
#include <cctype>
#include <filesystem>

#include "En_fileutil.h"

typedef unsigned char byte;

#ifndef _WIN32

#include <malloc.h>
#include <strings.h>
static unsigned long long _msize(void* ptr) {
	return malloc_usable_size(ptr);
}
static int _stricmp(const char* s1, const char* s2) {
	return strcasecmp(s1,s2);
}

#endif // _WIN32

CSTR::CSTR(int size) {
	body = (char*)calloc(1, size);
}

CSTR::~CSTR() {
	if (body) free(body);
}

int CSTR::length() {
	if (body)
		return strlen(body);
	else
		return 0;
}

DWORD CSTR::CRC32() {
	char *pcVar2;
	int pcVar3;
	unsigned int uVar4;
	int iVar5;

	pcVar2 = body;
	pcVar3 = length();
	uVar4 = 0xffffffff;
	iVar5 = 0;
	if (0 < (int)(pcVar3 + 1)) {
		do {
			uVar4 = uVar4 ^ (int)pcVar2[iVar5];
			for (int i = 0; i < 8; i++) {
				if ((uVar4 & 1) == 0) {
					uVar4 = uVar4 >> 1;
				}
				else {
					uVar4 = uVar4 >> 1 ^ 0xedb88320;
				}
			}
			iVar5 = iVar5 + 1;
		} while (iVar5 < (int)(pcVar3 + 1));
	}
	return ~uVar4;
}

int CSTR::msize() {
	if (body)	return _msize(body);
	return 0;
}

bool CSTR::resize(size_t size) {
	body = (char *)realloc(body, size);
	return body != nullptr;
}

// TODO: wrong usage?
CSTR& CSTR::add(const char *str, int len) {
	int size;
	int size2;

	if (len == 0) {
		len = strlen(str);
	}

	size = msize();
	size2 = length() + 1 + len;

	if (size < size2) {
		if (resize(size2) == false) return *this;
	}
	strncat(body, str, len);
	return *this;
}
//CSTR* CSTR::add(const char *str, int len){
//	char *pStr;
//	char *nextbody;
//	int size;
//	int size2;
//	char ch;
//
//	if (len == 0) {
//		/*pStr = str;
//		do {
//			ch = *pStr;
//			pStr = pStr++;
//		} while (ch != '\0');
//		len = (int)(pStr - (str + 1));*/
//		len = strlen(str);
//	}
//
//	//pStr = body;
//	//if (pStr == NULL) {
//	//	/*nextbody = NULL;*/
//	//	size2 = 0;
//	//}
//	//else {
//	//	/*nextbody = pStr;
//	//	do {
//	//		ch = *nextbody;
//	//		nextbody++;
//	//	} while (ch != '\0');
//	//	size2 = nextbody - (pStr + 1);*/
//	//	size2 = strlen(pStr);
//	//}
//	size2 = length();
//
//	//if (pStr == NULL) {
//	//	size = 0;
//	//}
//	//else {
//	//	size = _msize(pStr);
//	//}
//	size = msize();
//
//	size2 = size2 + 1 + len;
//	if (size < size2) {
//		/*if (body == NULL) {
//			pStr = (char *)calloc(1, size2);
//		}
//		else {
//			pStr = (char *)realloc(body, size2);
//		}
//		body = pStr;
//		if (pStr == NULL) {
//			return this;
//		}*/
//		if (resize(size2) == false) return this;
//	}
//	strncat(body, str, len);
//	return this;
//}

uint CSTR::checkValidPos(int *pos, int *len) {
	int bodylen;

	if (body == NULL) {
		return 0;
	}
	if (*pos < 0) {
		*len = *len + *pos;
		*pos = 0;
	}
	if (*len < 1) {
		return 0;
	}

	if (body != NULL) {
		bodylen = strlen(body);
		//return bodylen & (bodylen < *pos) - 1;
		if (bodylen < *pos) return 0;
		else return bodylen;
	}
	return 0;
}

inline void replace_all(std::string& str, std::string_view from, std::string_view to)
{
	for (size_t pos = str.find(from); pos != std::string::npos;)
	{
		str.replace(pos, from.length(), to);
		pos = str.find(from, pos + to.length());
	}
}

CSTR& CSTR::replace(const char *str1, const char *str2) {
	std::string string = this->body;
	replace_all(string, str1, str2);
	this->assign(string.c_str());
	return *this;
}

void CSTR::resize2(int size) {
	if (size != 0) {
		if (body != NULL) {
			body = (char *)realloc(body, size);
			return;
		}
		body = (char *)calloc(1, size);
	}
}

CSTR& CSTR::fillzero() {
	size_t size;

	if (body != NULL) {
		size = _msize(body);
		memset(body, 0, size);
	}
	return *this;
}

char* CSTR::writeAtPos(int pos, char ch) {
	size_t sVar1;

	if (pos < 0) {
		*body = ch;
		return body;
	}
	if (body == NULL) {
		sVar1 = 0;
	}
	else {
		sVar1 = _msize(body);
	}
	if ((int)sVar1 <= pos) {
		resize(pos + 1);
	}
	body[pos] = ch;
	return body + pos;
}

CSTR& CSTR::upper() {
	if (body) {
		for (auto& c : std::span{body, strlen(body)}) {
			c = std::toupper(c);
		}
	}
	return *this;
}

CSTR& CSTR::lower() {
	if (body) {
		for (auto& c : std::span{body, strlen(body)}) {
			c = std::tolower(c);
		}
	}
	return *this;
}

CSTR& CSTR::nullAtPos(int pos) {
	writeAtPos(pos, '\0');
	return *this;
}

int CSTR::lastSpaceCount() {
	int len;
	if (body == nullptr) {
		len = 0;
	}
	else {
		len = strlen(body);
	}
	if (body && len) {
		char* pcVar2;
		for (pcVar2 = body + len - 1; ((body <= pcVar2 && (*pcVar2 == ' ')) || (*pcVar2 == '\t')); pcVar2--) {

		}
		return body - pcVar2 + len - 1;
	}
	return 0;
}

int CSTR::findStrPos(const char *str) {
	if (body) {
		if (char *chrPos = strstr(body, str); chrPos) {
			return static_cast<int>(chrPos - body);
		}
	}
	return -1;
}

int CSTR::findChrBackPos(const char ch) {
	if (body) {
		if (char* chrPos = strrchr(body, ch); chrPos) {
			return static_cast<int>(chrPos - body);
		}
	}
	return -1;
}

char * cstrSprintf(CSTR *str, const char *format, ...) {
	if (str->body == nullptr && !str->resize(64)) {
		return nullptr;
	}

	const size_t capacity = _msize(str->body);

	va_list ap;
	va_start(ap, format);
	auto ret = vsnprintf(str->body, capacity, format, ap);
	va_end(ap);

	if (ret < 0) {
		// (void)errno;
		return nullptr;
	}

	if (ret >= capacity)
	{
		const auto new_capacity = ret + 1;
		if (!str->resize(new_capacity)) {
			return nullptr;
		}
		va_start(ap, format);
		ret = vsnprintf(str->body, new_capacity, format, ap);
		va_end(ap);
	}

	if (ret < 0) {
		// (void)errno;
		return nullptr;
	}

	return str->body;
}

int CSTR::toFile(const char *filepath) {
	FILE *f = fopen(filepath, "w");
	if (f == nullptr) {
		return -1;
	}
	fwrite(body, 1, body ? strlen(body) : 0, f);
	fclose(f);
	return 1;
}

char* CSTR::outstr() {
	return body;
}
CSTR::operator char*() {
	return body;
}
CSTR::operator const char*() {
	return body;
}

CSTR& CSTR::add(CSTR *param) {
	return add(param->body, 0);
}

CSTR& CSTR::add(const char *str) {
	return add(str, 0);
}

bool CSTR::isSame(const char *str) {
	if (body == NULL) return false;
	return (strcmp(body, str) == 0);
}

bool CSTR::starts_with(const char *str) {
	return left(std::string_view{str}.length()).isSame(str);
}

bool CSTR::isSame(CSTR *str) {
	return (strcmp(body, str->body) == 0);
}

int CSTR::isDiff(const char *str) {
	return (strcmp(body, str) != 0);
}

int CSTR::isDiff(CSTR *str) {
	return (strcmp(body, str->body) != 0);
}

int CSTR::isDiffLow(CSTR *str) {
	return (strcmp(body, str->body) < 0);
}

int CSTR::isDiffHigh(CSTR *str) {
	return (strcmp(body, str->body) > 0);
}

char* CSTR::atPos(int pos) {
	size_t sVar1;

	if (pos < 0) {
		return body;
	}
	if (body) {
		sVar1 = _msize(body);
	}
	else {
		sVar1 = 0;
	}
	if ((int)sVar1 <= pos) {
		resize(pos + 1);
	}
	return body + pos;
}

bool CSTR::canOpenFile() {
	FILE *_File;

	_File = fopen(body, "r");
	if (_File == NULL) {
		return false;
	}
	fclose(_File);
	return true;
}

CSTR::CSTR() {
	body = (char *)calloc(1, 0x40);
}

// copy construct
CSTR::CSTR(const CSTR &str, int len) {
	char *pStr;

	pStr = str.body;
	if (pStr == NULL) {
		this->body = NULL;
		this->body = (char *)calloc(1, 0x40);
		return;
	}
	if (len == 0) {
		len = strlen(pStr);
	}
	else if (len < 0) {
		len = 0;
	}
	this->body = (char *)calloc(1, len + 1);
	if (this->body != NULL) {
		strncpy(this->body, str.body, len);
	}
}

CSTR::CSTR(const char *str, int len) {
	if (str == NULL) {
		this->body = NULL;
		this->body = (char *)calloc(1, 0x40);
		return;
	}

	if (len == 0) {
		len = strlen(str);
	}
	else if (len < 0) {
		len = 0;
	}

	this->body = (char *)calloc(1, len + 1);
	if (this->body != NULL) {
		strncpy(this->body, str, len);
	}
}

CSTR& CSTR::assign(const char *str, int len) {
	if (str == NULL) {
		fillzero();
		return *this;
	}
	if (len == 0) {
		len = strlen(str);
	}
	size_t sVar3 = msize();
	if ((len + 1 <= (int)sVar3) || resize(len + 1) != false) {
		sVar3 = msize();
		memset(this->body, 0, sVar3);
		strncpy(this->body, str, len);
	}
	return *this;
}

int CSTR::icmp(CSTR *param_1) {
	if (body != nullptr) {
		return _stricmp(body, param_1->body);
	}
	return 0;
}

CSTR CSTR::left(int len) {
	return CSTR(body, len);
}

CSTR CSTR::right(int len) {
	if (len > -1 && length() >= len) {
		return CSTR(body - len + length(), len);
	}
	return CSTR(body, length()); //more simple than original code, deleting some repeated code.
}

CSTR CSTR::getSliced(int pos, int len) {
	if (checkValidPos(&pos, &len)) {
		return CSTR(body + pos, len);
	}
	return CSTR("", 0); //more simple than original code, deleting some repeated code.
}

CSTR& CSTR::assign(const CSTR *iBuf) {
	return assign(iBuf->body, 0);
}
CSTR& CSTR::operator=(const CSTR *iBuf) {
	return assign(iBuf->body, 0);
}
CSTR& CSTR::operator=(CSTR &iBuf) {
	return assign(iBuf.body, 0);
}

CSTR& CSTR::assign(const char *str) {
	return assign(str, 0);
}
CSTR& CSTR::operator=(const char *str) {
	return assign(str, 0);
}

CSTR CSTR::getDirectory() {
	CSTR out;
	out.assign(std::filesystem::path{this->body}.parent_path().string().c_str());
	*out.atPos(out.length() + 1) = '\0';
	*out.atPos(out.length()) = std::filesystem::path::preferred_separator;
	return out;
}

CSTR CSTR::getParentDirectory() {
	CSTR dir(getDirectory());
	dir.cutDirectorySeparator();
	return dir.getDirectory();
}

CSTR& CSTR::cutDirectorySeparator() {
	if( !strcmp(right(1), "/") || !strcmp(right(1), "\\")){
		writeAtPos(strlen(body) - 1, 0);
	}
	return *this;
}

CSTR& CSTR::lastCut(int len) {
	return assign( right(len).body, 0 );
}

CSTR& CSTR::trimWhiteSpace() {
	int fSpaceCount;
	char cVar1;
	char *pcVar4 = body;
	char *pcVar3 = pcVar4;
	if (pcVar4 == (char *)0x0) {
		fSpaceCount = 0;
	}
	else {
		for (; ((cVar1 = *pcVar3, cVar1 != '\0' && (cVar1 == ' ')) || (cVar1 == '\t')); pcVar3++) {
		}
		fSpaceCount = static_cast<int>(pcVar3 - pcVar4);
	}
	int bodylen = length();

	int lSpaceCount = lastSpaceCount();
	//CSTR tempCSTR = getSliced(fSpaceCount, bodylen - lSpaceCount - fSpaceCount);//getSliced(&tempCSTR, fSpaceCount, bodylen - lSpaceCount - fSpaceCount);
	assign(getSliced(fSpaceCount, bodylen - lSpaceCount - fSpaceCount).body, 0);
	//if ((CSTR)tempCSTR != (CSTR)0x0) {
	//	_free(tempCSTR);
	//}
	return *this;
}

CSTR CSTR::getFilename() {
	byte ch;
	int len;
	char *str;
	int pos;
	int i;

	i = 0;
	str = this->body;
	pos = -1;
	len = length();
	while (str[i] != '\0') {
		if (i >= len - 1) break;
		ch = str[i];
		if ((byte)((ch ^ 0x20) + 0x5f) < 0x3c) {
			i = i + 2;
		}
		else {
			if ((ch == '\\') || (ch == '/')) {
				pos = i;
			}
			i = i + 1;
		}
	}
	return right(length() - pos - 1).cutDirectorySeparator();
}
