#pragma once
#include "strclass.h"

//43ad60
CSTR::CSTR(int size) {
	body = (char*)calloc(1, size);
}

//43ad80
CSTR::~CSTR() {
	if (body) free(body);
}

//43ad90
int CSTR::length() {
	if (body)
		return strlen(body);
	else
		return 0;
}

//43adb0
DWORD CSTR::CRC32() {
	char cVar1;
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
			if ((uVar4 & 1) == 0) {
				uVar4 = uVar4 >> 1;
			}
			else {
				uVar4 = uVar4 >> 1 ^ 0xedb88320;
			}
			if ((uVar4 & 1) == 0) {
				uVar4 = uVar4 >> 1;
			}
			else {
				uVar4 = uVar4 >> 1 ^ 0xedb88320;
			}
			if ((uVar4 & 1) == 0) {
				uVar4 = uVar4 >> 1;
			}
			else {
				uVar4 = uVar4 >> 1 ^ 0xedb88320;
			}
			if ((uVar4 & 1) == 0) {
				uVar4 = uVar4 >> 1;
			}
			else {
				uVar4 = uVar4 >> 1 ^ 0xedb88320;
			}
			if ((uVar4 & 1) == 0) {
				uVar4 = uVar4 >> 1;
			}
			else {
				uVar4 = uVar4 >> 1 ^ 0xedb88320;
			}
			if ((uVar4 & 1) == 0) {
				uVar4 = uVar4 >> 1;
			}
			else {
				uVar4 = uVar4 >> 1 ^ 0xedb88320;
			}
			if ((uVar4 & 1) == 0) {
				uVar4 = uVar4 >> 1;
			}
			else {
				uVar4 = uVar4 >> 1 ^ 0xedb88320;
			}
			if ((uVar4 & 1) == 0) {
				uVar4 = uVar4 >> 1;
			}
			else {
				uVar4 = uVar4 >> 1 ^ 0xedb88320;
			}
			iVar5 = iVar5 + 1;
		} while (iVar5 < (int)(pcVar3 + 1));
	}
	return ~uVar4;
}

//43ae70
int CSTR::msize() {
	if (body)	return _msize(body);
	else	return 0;
}

//43ae90
bool CSTR::resize(size_t size) {
	if (body) {
		body = (char *)realloc(body, size);
	}
	else {
		body = (char *)calloc(1, size);
	}
	return body != NULL;
}

//43aed0 //TODO: wrong usage?
CSTR& CSTR::add(const char *str, int len) {
	int size;
	int size2;
	char ch;

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

//43af60
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

//43afc0
CSTR& CSTR::replace(int pos, int len1, const char *str2, int len2) {
	char *_Src;
	int iVar1;
	char cVar2;
	int _Size;
	bool bVar3;
	uint uVar4;
	size_t sVar5;
	char *pcVar6;

	uVar4 = checkValidPos(&pos, &len1);
	if (uVar4 != 0) {
		/*if (body == NULL) {
			sVar5 = 0;
		}
		else {
			sVar5 = _msize(body);
		}*/
		sVar5 = msize();
		_Size = len2;
		iVar1 = (uVar4 - len1) + 1 + len2;
		if ((iVar1 <= (int)sVar5) || (bVar3 = resize(iVar1), bVar3 != false)) {
			iVar1 = pos;
			_Src = body + pos + len1;

			/*len1 = (int)(_Src + 1);
			pcVar6 = _Src;
			do {
				cVar2 = *pcVar6;
				pcVar6 = pcVar6 + 1;
			} while (cVar2 != '\0');

			memmove(body + pos + _Size, _Src, (size_t)(pcVar6 + (1 - len1)));
			memmove(body + iVar1, str2, _Size);*/

			memmove(body + pos + _Size, _Src, strlen(_Src) + 1);
			memmove(body + iVar1, str2, _Size);
		}
	}
	return *this;
}

#include <string>
inline void replace_all(std::string& str, std::string_view from, std::string_view to)
{
	for (size_t pos = str.find(from); pos != std::string::npos;)
	{
		str.replace(pos, from.length(), to);
		pos = str.find(from, pos + to.length());
	}
}

//43b060
CSTR& CSTR::replace(const char *str1, const char *str2) {
	//char cVar1;
	//char *pcVar2;
	//char *pcVar3;
	//char *_Str1;
	//int iVar4;
	//int len1;
	//int len2;

	//if (body != NULL) {
	//	/*pcVar2 = str1;
	//	do {
	//		cVar1 = *pcVar2;
	//		pcVar2 = pcVar2 + 1;
	//	} while (cVar1 != '\0');*/
	//	len1 = strlen(str1);
	//	/*pcVar3 = str2;
	//	do {
	//		cVar1 = *pcVar3;
	//		pcVar3 = pcVar3 + 1;
	//	} while (cVar1 != '\0');*/
	//	len2 = strlen(str2);
	//	_Str1 = strchr(body, (int)*str1);
	//	while (_Str1 != NULL && !strncmp(_Str1, str1, len1)) {
	//		replace((int)(_Str1 + -(int)body), len1, str2, len2);
	//		_Str1 = strchr(_Str1 + len2, (int)*str1);
	//	}
	//}
	std::string string = this->body;
	replace_all(string, str1, str2);
	this->assign(string.c_str());
	return *this;
}

//43b100
void CSTR::resize2(int size) {
	char *pcVar1;

	if (size != 0) {
		if (body != NULL) {
			body = (char *)realloc(body, size);
			return;
		}
		body = (char *)calloc(1, size);
	}
	return;
}

//43b140
CSTR& CSTR::fillzero() {
	size_t size;

	if (body != NULL) {
		size = _msize(body);
		memset(body, 0, size);
	}
	return *this;
}

//43b170
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

//43b1d0
CSTR& CSTR::upper() {
	if (body) {
		_strupr(body);
	}
	return *this;
}

//43b1f0
CSTR& CSTR::lower() {
	if (body) {
		_strlwr(body);
	}
	return *this;
}

//43b210
CSTR& CSTR::nullAtPos(int pos) {
	writeAtPos(pos, '\0');
	return *this;
}

//43b230
int CSTR::lastSpaceCount() {
	char cVar1;
	char *pStr;
	int len;
	char *pcVar2;
	char *pBody;

	pBody = body;
	if (pBody == (char *)0x0) {
		len = 0;
	}
	else {
		/*pStr = pBody;
		do {
			cVar1 = *pStr;
			pStr = pStr + 1;
		} while (cVar1 != '\0');
		len = (int)(pStr + -(int)(pBody + 1));*/
		len = strlen(pBody);
	}
	if (pBody && len) {
		for (pcVar2 = pBody + len - 1; ((pBody <= pcVar2 && (*pcVar2 == ' ')) || (*pcVar2 == '\t')); pcVar2--) {

		}
		return (int)pBody - (int)pcVar2 + len - 1;
	}
	return 0;
}

//43b280
int CSTR::findStrPos(const char *str) {
	char *pcVar1;
	if (body) {
		pcVar1 = strstr(body, str);
		if (pcVar1) {
			return (int)(pcVar1 - (int)body);
		}
	}
	return -1;
}

int CSTR::findChrBackPos(const char ch) {
	char* chrPos;
	if (body) {
		chrPos = strrchr(body, ch);
		if (chrPos) {
			return (int)(chrPos - (int)body);
		}
	}
	return -1;
}

//43b2b0 ****not in class****
char * cstrSprintf(CSTR *str, const char *format, ...) {
	size_t bSize;
	char *pStr;
	int iVar1;
	size_t _Size;

	va_list ap;

	if (str->body == NULL) {
		bSize = 0;
	}
	else {
		bSize = _msize(str->body);
	}

	if (str->body == (char *)0x0) {
		pStr = (char *)calloc(1, 0x40);
		str->body = pStr;
		if (pStr == (char *)0x0) {
			return (char *)str;
		}
	}
	do {
		_Size = bSize + 0x80;
		va_start(ap, format);
		iVar1 = _vsnprintf(str->body, bSize - 1, format, ap);
		va_end(ap);
		if (-1 < iVar1) {
			return (char *)str;
		}
		if (str->body == (char *)0x0) {
			pStr = (char *)calloc(1, _Size);
		}
		else {
			pStr = (char *)realloc(str->body, _Size);
		}
		str->body = pStr;
		bSize = _Size;
	} while (pStr != (char *)0x0);
	return (char *)str;
}

//43b330
int CSTR::toFile(const char *filepath) {
	char cVar1;
	char *_Str;
	FILE *_File;
	char *pcVar2;

	_File = fopen(filepath, "w");
	if (_File == (FILE *)0x0) {
		return -1;
	}
	_Str = body;
	if (_Str == (char *)0x0) {
		pcVar2 = (char *)0x0;
	}
	else {
		pcVar2 = (char*)strlen(_Str);
	}
	fwrite(_Str, 1, (size_t)(pcVar2 + 1), _File);
	fclose(_File);
	return 1;
}

//43b390
char* CSTR::outstr() {
	return body;
}
//43b390_
CSTR::operator char*() {
	return body;
}
CSTR::operator const char*() {
	return body;
}

//43b3a0
CSTR& CSTR::add(CSTR *param) {
	return add(param->body, 0);
}

//43b3c0
CSTR& CSTR::add(const char *str) {
	return add(str, 0);
}

//43b3d0
bool CSTR::isSame(const char *str) {
	if (body == NULL) return false;
	return (strcmp(body, str) == 0);
}

//43b420
bool CSTR::isSame(CSTR *str) {
	return (strcmp(body, str->body) == 0);
}

//43b470
int CSTR::isDiff(const char *str) {
	return (strcmp(body, str) != 0);
}

//43b4c0
int CSTR::isDiff(CSTR *str) {
	return (strcmp(body, str->body) != 0);
}

//43b510
int CSTR::isDiffLow(CSTR *str) {
	return (strcmp(body, str->body) < 0);
}

//43b560
int CSTR::isDiffHigh(CSTR *str) {
	return (strcmp(body, str->body) > 0);
}

//43b5b0
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

//43b600
bool CSTR::canOpenFile() {
	FILE *_File;

	_File = fopen(body, "r");
	if (_File == NULL) {
		return false;
	}
	fclose(_File);
	return true;
}

//43b630
CSTR::CSTR() {
	body = NULL;
	body = (char *)calloc(1, 0x40);
	return;
}

//43b650 copy construct
CSTR::CSTR(const CSTR &str, int len) {
	char *pStr;
	char ch;
	char *cur;

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

//43b6c0
CSTR::CSTR(const char *str, int len) {
	char *pStr;
	char ch;

	if (str == NULL) {
		this->body = NULL;
		this->body = (char *)calloc(1, 0x40);
		return;
	}

	if (len == 0) {
		/*pStr = str;
		do {
			ch = *pStr;
			pStr = pStr + 1;
		} while (ch != '\0');
		len = (int)(pStr + -(int)(str + 1));*/
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

//43b730
CSTR CSTR::makeCRCstr() {
	char cVar1;
	char* str;
	DWORD dVar2;
	int pcVar3;
	char* CVar4;
	char *pstr;
	char* local_4;
	char* ret;

	local_4 = (char *)calloc(1, 0x40);
	dVar2 = CRC32();
	sprintf(local_4, "%x", dVar2);
	str = local_4;
	if (local_4 == NULL) {
		pcVar3 = (int)calloc(1, 0x40);
		ret = (char*)pcVar3;
		return ret;
	}
	CVar4 = local_4;
	do {
		cVar1 = *CVar4;
		CVar4 = CVar4 + 1;
	} while (cVar1 != '\0');
	pcVar3 = (int)CVar4 - (int)(local_4 + 1);
	pstr = (char *)calloc(1, (size_t)(pcVar3 + 1));
	ret = pstr;
	if (pstr != (char *)0x0) {
		strncpy(pstr, (char *)str, pcVar3);
	}
	free((void *)str);
	return ret;
}

//43b7e0
CSTR& CSTR::assign(const char *str, int len) {
	char cVar1;
	bool bVar2;
	size_t sVar3;
	char *pcVar4;

	if (str == NULL) {
		fillzero();
		return *this;
	}
	else {
		if (len == 0) {
			len = strlen(str);
		}
		sVar3 = msize();

		if (((int)(char *)(len + 1) <= (int)sVar3) || resize(len + 1) != false) {
			sVar3 = msize();
			memset(this->body, 0, sVar3);
			strncpy(this->body, str, len);
		}
	}
	return *this;
}

//43b890
int CSTR::icmp(CSTR *param_1) {
	if (body != (char *)0x0) {
		return _stricmp(body, param_1->body);
	}
	return 0;
}

//43b8b0
CSTR CSTR::left(int len) {
	//char cVar1;
	//CSTR *pCVar2;
	//char *pcVar3;
	//int pcVar4;
	//char *pstr;
	//CSTR ret;

	//pCVar2 = new CSTR(body, len);
	//pcVar3 = pCVar2->body;
	//if (pcVar3 == (char *)0x0) {
	//	pcVar3 = (char *)calloc(1, 0x40);
	//	ret.body = pcVar3;
	//}
	//else {
	//	/*pcVar4 = pcVar3;
	//	do {
	//		cVar1 = *pcVar4;
	//		pcVar4 = pcVar4 + 1;
	//	} while (cVar1 != '\0');*/
	//	pcVar4 = strlen(pcVar3);
	//	pstr = (char *)calloc(1, (size_t)(pcVar4 + 1));
	//	ret.body = pstr;
	//	if (pstr != (char *)0x0) {
	//		strncpy(pstr, pcVar3, pcVar4);
	//	}
	//}
	//delete(pCVar2);

	//return (ret);

	return CSTR(body, len);
}

//43b940
CSTR CSTR::right(int len) {
	//char cVar1;
	//int bodylen;
	//char *pcVar2;
	//CSTR *pCVar3;
	//char *pStr1;
	//char *pcVar4;
	//CSTR ret;

	//if (-1 < len) {
	//	/*pStr1 = this->body;
	//	if (pStr1 == (char *)0x0) {
	//		pcVar2 = (char *)0x0;
	//	}
	//	else {
	//		pcVar2 = pStr1;
	//		do {
	//			cVar1 = *pcVar2;
	//			pcVar2 = pcVar2 + 1;
	//		} while (cVar1 != '\0');
	//		pcVar2 = pcVar2 + -(int)(pStr1 + 1);
	//	}*/
	//	bodylen = length();
	//	if (len <= bodylen) {
	//		/*if (pStr1 == (char *)0x0) {
	//			pcVar2 = (char *)0x0;
	//		}
	//		else {
	//			pcVar2 = pStr1;
	//			do {
	//				cVar1 = *pcVar2;
	//				pcVar2 = pcVar2 + 1;
	//			} while (cVar1 != '\0');
	//			pcVar2 = pcVar2 + -(int)(pStr1 + 1);
	//		}*/
	//		pCVar3 = new CSTR(body - len + length(), len);
	//		pStr1 = pCVar3->body;
	//		if (pStr1 == (char *)0x0) {
	//			pStr1 = (char *)calloc(1, 0x40);
	//			ret.body = pStr1;
	//		}
	//		else {
	//			/*pcVar2 = pStr1;
	//			do {
	//				cVar1 = *pcVar2;
	//				pcVar2 = pcVar2 + 1;
	//			} while (cVar1 != '\0');
	//			pcVar4 = (char *)calloc(1, (size_t)(pcVar2 + -(int)(pStr1 + 1) + 1));
	//			obuf->body = pcVar4;
	//			if (pcVar4 != (char *)0x0) {
	//				strncpy(pcVar4, pStr1, pcVar2 + -(int)(pStr1 + 1));
	//			}*/
	//			pcVar4 = (char *)calloc(1, (size_t)(pCVar3->length() + 1));
	//			ret.body = pcVar4;
	//			if (pcVar4 != (char *)0x0) {
	//				strncpy(pcVar4, pStr1, pCVar3->length());
	//			}
	//		}

	//		/*if (pCVar3 == NULL) {
	//			return obuf;
	//		}*/
	//		delete(pCVar3);
	//		return (ret);
	//	}
	//}
	//pStr1 = this->body;
	//if (pStr1 == (char *)0x0) {
	//	pcVar4 = (char *)calloc(1, 0x40);
	//}
	//else {
	//	//pcVar2 = pStr1;
	//	//do {
	//	//	cVar1 = *pcVar2;
	//	//	pcVar2 = pcVar2 + 1;
	//	//} while (cVar1 != '\0');
	//	//pcVar4 = (char *)calloc(1, (size_t)(pcVar2 + -(int)(pStr1 + 1) + 1));
	//	//if (pcVar4 == (char *)0x0) {
	//	//	pStr1 = (char *)_calloc(1, 0x40);
	//	//	obuf->body = pStr1;
	//	//	return obuf;
	//	//}
	//	//strncpy(pcVar4, pStr1, pcVar2 + -(int)(pStr1 + 1));
	//	pcVar4 = (char *)calloc(1, length() + 1);
	//	if (pcVar4 == (char *)0x0) {
	//		pStr1 = (char *)calloc(1, 0x40);
	//		ret.body = pStr1;
	//		return (ret);
	//	}
	//	strncpy(pcVar4, pStr1, length());
	//}

	//if (pcVar4 != (char *)0x0) {
	//	/*pStr1 = pcVar4;
	//	do {
	//		cVar1 = *pStr1;
	//		pStr1 = pStr1 + 1;
	//	} while (cVar1 != '\0');
	//	pcVar2 = (char *)calloc(1, (size_t)(pStr1 + -(int)(pcVar4 + 1) + 1));
	//	obuf->body = pcVar2;
	//	if (pcVar2 != (char *)0x0) {
	//		strncpy(pcVar2, pcVar4, pStr1 + -(int)(pcVar4 + 1));
	//	}
	//	free(pcVar4);
	//	return obuf;*/
	//	pcVar2 = (char *)calloc(1, strlen(pcVar4) + 1);
	//	ret.body = pcVar2;
	//	if (pcVar2 != (char *)0x0) {
	//		strncpy(pcVar2, pcVar4, strlen(pcVar4));
	//	}
	//	free(pcVar4);
	//	return (ret);
	//}
	///*pStr1 = (char *)_calloc(1, 0x40);
	//obuf->body = pStr1;*/
	//ret = CSTR();
	//return (ret);

	
	if (len > -1 && length() >= len) {
		return CSTR(body - len + length(), len);
	}
	return CSTR(body, length()); //more simple than original code, deleting some repeated code.
}

//43bac0
CSTR CSTR::getSliced(int pos, int len) {
	/*CSTR oBuf;
	char cVar1;
	uint uVar2;
	char *pcVar3;
	char *pcVar4;
	char *pcVar5;

	uVar2 = checkValidPos(&pos, &len);
	if (uVar2) {
		CSTR pCVar6 = CSTR(body + pos, len);
		pcVar3 = pCVar6.body;
		if (pcVar3 == (char *)0x0) {
			pcVar3 = (char *)calloc(1, 0x40);
			oBuf.body = pcVar3;
		}
		else {
			pcVar5 = (char *)calloc(1, (size_t)(pCVar6.length() + 1));
			oBuf.body = pcVar5;
			if (pcVar5 != (char *)0x0) {
				strncpy(pcVar5, pcVar3, pCVar6.length());
			}
		}
		return oBuf;
	}
	pcVar3 = (char *)calloc(1, 1);
	if (pcVar3 != (char *)0x0) {
		strncpy(pcVar3, "", 0);
		pcVar4 = pcVar3;
		do {
			cVar1 = *pcVar4;
			pcVar4 = pcVar4 + 1;
		} while (cVar1 != '\0');

		pcVar5 = (char *)calloc(1, strlen(pcVar3) + 1);
		oBuf.body = pcVar5;
		if (pcVar5 != (char *)0x0) {
			strncpy(pcVar5, pcVar3, strlen(pcVar3));
		}
		free(pcVar3);
		return oBuf;
	}
	pcVar3 = (char *)calloc(1, 0x40);
	oBuf.body = pcVar3;
	return oBuf;*/

	CSTR oBuf;
	char cVar1;
	uint uVar2;
	char *pcVar3;
	char *pcVar4;
	char *pcVar5;

	if (checkValidPos(&pos, &len)) {
		return CSTR(body + pos, len);
	}
	return CSTR("", 0); //more simple than original code, deleting some repeated code.
}
//CSTR& CSTR::getSliced(CSTR *oBuf, int pos, int len) {
//	char cVar1;
//	uint uVar2;
//	char *pcVar3;
//	char *pcVar4;
//	char *pcVar5;
//	CSTR pCVar6;
//
//	uVar2 = checkValidPos(&pos, &len);
//	if (uVar2) {
//		pCVar6 = CSTR(body + pos, len);
//		pcVar3 = pCVar6.body;
//		if (pcVar3 == (char *)0x0) {
//			pcVar3 = (char *)calloc(1, 0x40);
//			oBuf->body = pcVar3;
//		}
//		else {
//			pcVar5 = (char *)calloc(1, (size_t)(pCVar6.length() + 1));
//			oBuf->body = pcVar5;
//			if (pcVar5 != (char *)0x0) {
//				strncpy(pcVar5, pcVar3, pCVar6.length());
//			}
//		}
//		/*if ((CSTR)pCVar6 != (CSTR)0x0) {
//			free((void *)pCVar6);
//		}*/
//		return *oBuf;
//	}
//	pcVar3 = (char *)calloc(1, 1);
//	if (pcVar3 != (char *)0x0) {
//		strncpy(pcVar3, "", 0);
//		pcVar4 = pcVar3;
//		do {
//			cVar1 = *pcVar4;
//			pcVar4 = pcVar4 + 1;
//		} while (cVar1 != '\0');
//
//		pcVar5 = (char *)calloc(1, strlen(pcVar3) + 1);
//		oBuf->body = pcVar5;
//		if (pcVar5 != (char *)0x0) {
//			strncpy(pcVar5, pcVar3, strlen(pcVar3));
//		}
//		free(pcVar3);
//		return *oBuf;
//	}
//	pcVar3 = (char *)calloc(1, 0x40);
//	oBuf->body = pcVar3;
//	return *oBuf;
//}

//43bbf0
CSTR& CSTR::assign(const CSTR *iBuf) {
	return assign(iBuf->body, 0);
}
//43bbf0_
CSTR& CSTR::operator=(const CSTR *iBuf) {
	return assign(iBuf->body, 0);
}
CSTR& CSTR::operator=(CSTR &iBuf) {
	return assign(iBuf.body, 0);
}

/*
'= fuctionOutput' is const CSTR, so goes to 43bc10() with casting (const char*)
'= normal CSTR' is not const CSTR, goes to 43bbf0.
*/

//43bc10
CSTR& CSTR::assign(const char *str) {
	return assign(str, 0);
}
//43bc10_
CSTR& CSTR::operator=(const char *str) {
	return assign(str, 0);
}

//43bc20
CSTR CSTR::getDirectory() {
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
		if ( i >= len - 1 ) break;
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
	return left(pos + 1);
}

//43bce0
CSTR CSTR::getParaentDirectory() {
	return getDirectory().getDirectory();
}

//43bd80
CSTR& CSTR::cutDirectorySeparator() {
	if( !strcmp(right(1), "/") || !strcmp(right(1), "\\")){
		writeAtPos(strlen(body) - 1, 0);
	}
	return *this;
}

//43beb0
CSTR& CSTR::lastCut(int len) {
	return assign( right(len).body, 0 );
}

//43bef0
CSTR& CSTR::trimWhiteSpace() {
	char cVar1;
	int lSpaceCount;
	char *pcVar3;
	char *pcVar4;
	int bodylen;
	int fSpaceCount;
	//CSTR tempCSTR;

	//fSpaeCount = frontSpaceCount()
	pcVar4 = body;
	pcVar3 = pcVar4;
	if (pcVar4 == (char *)0x0) {
		fSpaceCount = 0;
	}
	else {
		for (; ((cVar1 = *pcVar3, cVar1 != '\0' && (cVar1 == ' ')) || (cVar1 == '\t')); pcVar3++) {
		}
		fSpaceCount = (int)(pcVar3 + -(int)pcVar4);
	}
	/*if (pcVar4 == (char *)0x0) {
		bodylen = 0;
	}
	else {
		pcVar3 = pcVar4 + 1;
		do {
			cVar1 = *pcVar4;
			pcVar4 = pcVar4 + 1;
		} while (cVar1 != '\0');
		bodylen = (int)(pcVar4 + -(int)pcVar3);
	}*/
	bodylen = length();

	lSpaceCount = lastSpaceCount();
	//tempCSTR = getSliced(fSpaceCount, bodylen - lSpaceCount - fSpaceCount);//getSliced(&tempCSTR, fSpaceCount, bodylen - lSpaceCount - fSpaceCount);
	assign(getSliced(fSpaceCount, bodylen - lSpaceCount - fSpaceCount).body, 0);
	//if ((CSTR)tempCSTR != (CSTR)0x0) {
	//	_free(tempCSTR);
	//}
	return *this;
}

//43bf70
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