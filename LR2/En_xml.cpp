#include "En_xml.h"
#include "tinyxml/tinyxml.h"

//43c060
int ReadXml_Int(const char *level1, const char *level2, const char *level3, int initvalue, int *oBuf, TiXmlDocument *xmlData){
	TiXmlElement *cur;

	if (xmlData == NULL) {
		*oBuf = initvalue;
		return 0;
	}
	
	cur = xmlData->FirstChildElement(level1);
	if (cur) {
		cur = cur->FirstChildElement(level2);
		if (cur) {
			cur = cur->FirstChildElement(level3);
			if (cur) {
				*oBuf = atol(cur->ToElement()->GetText());
				return 1;
			}
		}
	}
	*oBuf = initvalue;
	return -1;
}

//43c0e0
int ReadXml_Str(const char *level1, const char *level2, const char *level3, const CSTR initvalue, CSTR* oBuf, TiXmlDocument *xmlData) {
	TiXmlElement *cur;

	if (xmlData == NULL) {
		oBuf->assign(&initvalue);
		return 0;
	}

	cur = xmlData->FirstChildElement(level1);
	if (cur) {
		cur = cur->FirstChildElement(level2);
		if (cur) {
			cur = cur->FirstChildElement(level3);
			if (cur) {
				if (cur->ToElement() == NULL || cur->ToElement()->GetText() == 0) {
					oBuf->assign(&initvalue);
				}
				else {
					cstrSprintf( oBuf, "%s", cur->ToElement()->GetText() );
				}
				return 1;
			}
		}
	}
	oBuf->assign(&initvalue);
	return -1;
}

//43c440
int ReadXml_Int_Multi(const char *level1, const char *level2, const char *level3, int *oBuf, TiXmlDocument *xmlData){
	TiXmlElement *cur;

	oBuf[0] = 0;
	oBuf[1] = 0;
	oBuf[2] = 0;
	oBuf[3] = 0;
	oBuf[4] = 0;
	oBuf[5] = 0;
	oBuf[6] = 0;
	oBuf[7] = 0;
	oBuf[8] = 0;
	oBuf[9] = 0;
	oBuf[10] = 0;
	oBuf[0xb] = 0;
	oBuf[0xc] = 0;
	oBuf[0xd] = 0;
	oBuf[0xe] = 0;
	oBuf[0xf] = 0;

	cur = xmlData->FirstChildElement(level1);
	if (cur) {
		cur = cur->FirstChildElement(level2);
		if (cur) {
			cur = cur->FirstChildElement(level3);
			if (cur) {
				oBuf[0] = atol(cur->ToElement()->GetText());

				for (int i = 1; i < 16; i++) {
					cur = cur->NextSiblingElement();
					if (cur == NULL) {
						return 1;
					}
					oBuf[i] = atol(cur->ToElement()->GetText());
				}
				return 1;

			}
		}
	}
	return -1;
}


//43cef0
void WriteXML_Tab2Int(FILE *hFile, const char *tag, int value){
	char buf[256];

	sprintf(buf, "\t\t<%s>%d</%s>\n", tag, value, tag);
	fputs(buf, hFile);
	return;
}

//43cf50
void WriteXML_Tab2Str(FILE *hFile, const char *tag, CSTR str){
	char buf[256];

	str.replace("&", "&amp;");
	str.replace("<", "&lt;");
	str.replace(">", "&gt;");
	str.replace("\'", "&apos;");
	str.replace("\"", "&quot;");
	sprintf(buf, "\t\t<%s>%s</%s>\n", tag, str, tag);
	fputs(buf, hFile);
	return;
}
