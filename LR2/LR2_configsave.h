/*
 * Not existing 'filepath' when reading is not an error.
 */

#include "structure.h"
#include "Engine.h"

int Read_JukeboxPath(CONFIG_JUKEBOX *box, TiXmlDocument *xml);

int ReadKeyConfig(game * game, const char * FilePath);
void WriteXML_KeyConfig(FILE * hFile, CONFIG_INPUT cfg_in, char * tag, int num);
int WriteKeyConfig(game * g, const char * filepath, int key);

int ReadMIDI(game *gs, const char *filepath);
int WriteMidiXml(game * g, const char * filename);

int ReadConfig(game* gs, const char* filepath);
int WriteConfigXml(game * g, const char * filename);

int ReadOpenLr2Config(game* gs, const char* filepath);
int WriteOpenLr2ConfigXml(game * g, const char * filename);

int ReadSkinCustomize(SkinUser * sku, char * FilePath);
int WriteSkinCustomizeXml(SkinUser * sku, char * filepath);

