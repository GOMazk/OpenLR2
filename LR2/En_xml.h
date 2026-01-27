#include "tinyxml/tinyxml.h"
#include "sqlite/sqlite3.h"

#include "structure.h"

void file_utf_to_ansi(const char* filepath);
bool parse_cp932_xml(TiXmlDocument* xml, const char* filepath);

int ReadXml_Int(const char *level1, const char *level2, const char *level3, int initvalue, int *oBuf, TiXmlDocument *xmlData);
int ReadXml_PositiveIntAsBool(const char *level1, const char *level2, const char *level3, bool initvalue, bool *oBuf, TiXmlDocument *xmlData);
int ReadXml_Str(const char *level1, const char *level2, const char *level3, const CSTR initvalue, CSTR* oBuf, TiXmlDocument *xmlData);
int ReadXml_Int_Multi(const char * level1, const char * level2, const char * level3, int * oBuf, TiXmlDocument * xmlData);

void WriteXML_Tab2Int(FILE * hFile, const char * tag, int value);
void WriteXML_Tab2Str(FILE * hFile, const char * tag, CSTR str);
