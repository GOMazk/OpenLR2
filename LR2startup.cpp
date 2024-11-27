#pragma once

#include "LR2startup.h"
#include "tinyxml/tinyxml.h"
#include "sqlite/sqlite3.h"
//extern "C"{
#include "md5.h" //md5
//}


//43c060
int ReadXml_Int(const char *level1, const char *level2, const char *level3, int initvalue, int *oBuf, TiXmlDocument *xmlData){
	TiXmlElement *cur;
	int iVar1;
	long lVar2;

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

	if (!xmlData) {
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

//43c220
int Read_JukeboxPath(CONFIG_JUKEBOX *box, TiXmlDocument *xml){
	char *str;
	TiXmlElement *cur;

	if (xml == NULL) {
		return 0;
	}
	box->numOfPath = 0;
	
	if ((cur = xml->FirstChildElement("config")) && (cur = cur->FirstChildElement("jukebox")) &&
		(cur = cur->FirstChildElement("path")) && cur->ToElement()) {
		
		cstrSprintf(&box->path[0], "%s", cur->ToElement()->GetText());
		CSTR tp2 = CSTR();
		CSTR tp3 = CSTR();
		tp2.assign(&box->path[0]);
		if (tp2.right(2).isSame("\\\\") ) {
			CSTR tp4 = CSTR();
			tp2.left(tp2.length() -1);
			str = tp4.outstr();
			box->path[0].assign(str);
		}
		box->numOfPath = 1;

		while (box->numOfPath < 1000 && (cur = cur->NextSiblingElement()) && cur->ToElement()) {
			cstrSprintf(box->path + box->numOfPath, "%s", cur->ToElement()->GetText());
			CSTR tp = CSTR();
			tp.assign(box->path + box->numOfPath);
			box->numOfPath++;
		}
		return 1;
	}
	return -1;
}

//43c440
int ReadXml_Int_Multi(const char *level1, const char *level2, const char *level3, int *oBuf, TiXmlDocument *xmlData){
	TiXmlElement *cur;
	
	void *pvVar1;
	int iVar2;
	long lVar3;
	int iVar4;

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
					if (cur == (void *)0x0) {
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

//43c510
int ReadKeyConfig(game *game, const char *FilePath) {
	TiXmlDocument *hXml;

	memset(game->config.input.buttonMap, 0, 16 * 40 * sizeof(int));

	hXml = new TiXmlDocument(FilePath);
	if (hXml->LoadFile(TIXML_ENCODING_UNKNOWN) == false) {
		if (hXml) {
			delete(hXml);
		}
		hXml = NULL;
	}

	ReadXml_Int_Multi("keyconfig", "key01", "id", (game->config).input.buttonMap[1], hXml);
	ReadXml_Int_Multi("keyconfig", "key02", "id", (game->config).input.buttonMap[2], hXml);
	ReadXml_Int_Multi("keyconfig", "key03", "id", (game->config).input.buttonMap[3], hXml);
	ReadXml_Int_Multi("keyconfig", "key04", "id", (game->config).input.buttonMap[4], hXml);
	ReadXml_Int_Multi("keyconfig", "key05", "id", (game->config).input.buttonMap[5], hXml);
	ReadXml_Int_Multi("keyconfig", "key06", "id", (game->config).input.buttonMap[6], hXml);
	ReadXml_Int_Multi("keyconfig", "key07", "id", (game->config).input.buttonMap[7], hXml);
	ReadXml_Int_Multi("keyconfig", "key08", "id", (game->config).input.buttonMap[8], hXml);
	ReadXml_Int_Multi("keyconfig", "key09", "id", (game->config).input.buttonMap[9], hXml);
	ReadXml_Int_Multi("keyconfig", "key10", "id", (game->config).input.buttonMap[10], hXml);
	ReadXml_Int_Multi("keyconfig", "key11", "id", (game->config).input.buttonMap[0xb], hXml);
	ReadXml_Int_Multi("keyconfig", "key12", "id", (game->config).input.buttonMap[0xc], hXml);
	ReadXml_Int_Multi("keyconfig", "key13", "id", (game->config).input.buttonMap[0xd], hXml);
	ReadXml_Int_Multi("keyconfig", "key21", "id", (game->config).input.buttonMap[0x15], hXml);
	ReadXml_Int_Multi("keyconfig", "key22", "id", (game->config).input.buttonMap[0x16], hXml);
	ReadXml_Int_Multi("keyconfig", "key23", "id", (game->config).input.buttonMap[0x17], hXml);
	ReadXml_Int_Multi("keyconfig", "key24", "id", (game->config).input.buttonMap[0x18], hXml);
	ReadXml_Int_Multi("keyconfig", "key25", "id", (game->config).input.buttonMap[0x19], hXml);
	ReadXml_Int_Multi("keyconfig", "key26", "id", (game->config).input.buttonMap[0x1a], hXml);
	ReadXml_Int_Multi("keyconfig", "key27", "id", (game->config).input.buttonMap[0x1b], hXml);
	ReadXml_Int_Multi("keyconfig", "key30", "id", (game->config).input.buttonMap[0x1e], hXml);
	ReadXml_Int_Multi("keyconfig", "key31", "id", (game->config).input.buttonMap[0x1f], hXml);
	ReadXml_Int_Multi("keyconfig", "key32", "id", (game->config).input.buttonMap[0x20], hXml);
	ReadXml_Int_Multi("keyconfig", "key33", "id", (game->config).input.buttonMap[0x21], hXml);

	return 1;
}

//43c890
int ReadMIDI(game *gs, const char *filepath){
	TiXmlDocument *hXml;

	(gs->config).input.midi_control[0] = 0;
	(gs->config).input.midi_control[1] = 0;
	(gs->config).input.midi_control[2] = 0;
	(gs->config).input.midi_control[3] = 0;
	(gs->config).input.midi_control[4] = 0;
	(gs->config).input.midi_control[5] = 0;
	(gs->config).input.midi_control[6] = 0;
	(gs->config).input.midi_control[7] = 0;
	(gs->config).input.midi_control[8] = 0;
	(gs->config).input.midi_control[9] = 0;
	(gs->config).input.midi_control[10] = 0;
	(gs->config).input.midi_control[0xb] = 0;
	(gs->config).input.midi_control[0xc] = 0;
	(gs->config).input.midi_control[0xd] = 0;
	(gs->config).input.midi_control[0xe] = 0;
	(gs->config).input.midi_control[0xf] = 0;
	(gs->config).input.midi_control[0x10] = 0;
	(gs->config).input.midi_control[0x11] = 0;
	(gs->config).input.midi_control[0x12] = 0;
	(gs->config).input.midi_control[0x13] = 0;
	(gs->config).input.midi_control[0x14] = 0;
	(gs->config).input.midi_control[0x15] = 0;
	(gs->config).input.midi_control[0x16] = 0;
	(gs->config).input.midi_control[0x17] = 0;
	(gs->config).input.midi_control[0x18] = 0;
	(gs->config).input.midi_control[0x19] = 0;
	(gs->config).input.midi_control[0x1a] = 0;
	(gs->config).input.midi_control[0x1b] = 0;
	(gs->config).input.midi_control[0x1c] = 0;
	(gs->config).input.midi_control[0x1d] = 0;
	(gs->config).input.midi_control[0x1e] = 0;
	(gs->config).input.midi_control[0x1f] = 0;
	(gs->config).input.midi_control[0x20] = 0;
	(gs->config).input.midi_control[0x21] = 0;
	(gs->config).input.midi_control[0x22] = 0;
	(gs->config).input.midi_control[0x23] = 0;
	(gs->config).input.midi_control[0x24] = 0;
	(gs->config).input.midi_control[0x25] = 0;
	(gs->config).input.midi_control[0x26] = 0;
	(gs->config).input.midi_control[0x27] = 0;

	hXml = new TiXmlDocument(filepath);
	if (hXml->LoadFile(TIXML_ENCODING_UNKNOWN) == false) {
		if (hXml) {
			delete(hXml);
		}
		hXml = NULL;
	}
	ReadXml_Int("midi", "control", "S01", 0, (gs->config).input.midi_control + 1, hXml);
	ReadXml_Int("midi", "control", "S02", 0, (gs->config).input.midi_control + 2, hXml);
	ReadXml_Int("midi", "control", "S03", 0, (gs->config).input.midi_control + 3, hXml);
	ReadXml_Int("midi", "control", "S04", 0, (gs->config).input.midi_control + 4, hXml);
	ReadXml_Int("midi", "control", "S05", 0, (gs->config).input.midi_control + 5, hXml);
	ReadXml_Int("midi", "control", "S06", 0, (gs->config).input.midi_control + 6, hXml);
	ReadXml_Int("midi", "control", "S07", 0, (gs->config).input.midi_control + 7, hXml);
	ReadXml_Int("midi", "control", "S08", 0, (gs->config).input.midi_control + 8, hXml);
	ReadXml_Int("midi", "control", "S09", 0, (gs->config).input.midi_control + 9, hXml);
	ReadXml_Int("midi", "control", "S10", 0, (gs->config).input.midi_control + 10, hXml);
	ReadXml_Int("midi", "control", "S11", 0, (gs->config).input.midi_control + 0xb, hXml);
	ReadXml_Int("midi", "control", "S12", 0, (gs->config).input.midi_control + 0xc, hXml);
	ReadXml_Int("midi", "control", "S13", 0, (gs->config).input.midi_control + 0xd, hXml);
	ReadXml_Int("midi", "control", "S14", 0, (gs->config).input.midi_control + 0xe, hXml);
	ReadXml_Int("midi", "control", "S15", 0, (gs->config).input.midi_control + 0xf, hXml);
	ReadXml_Int("midi", "control", "S16", 0, (gs->config).input.midi_control + 0x10, hXml);
	ReadXml_Int("midi", "control", "S17", 0, (gs->config).input.midi_control + 0x11, hXml);
	ReadXml_Int("midi", "control", "S18", 0, (gs->config).input.midi_control + 0x12, hXml);
	ReadXml_Int("midi", "control", "S19", 0, (gs->config).input.midi_control + 0x13, hXml);
	ReadXml_Int("midi", "control", "S20", 0, (gs->config).input.midi_control + 0x14, hXml);
	ReadXml_Int("midi", "control", "S21", 0, (gs->config).input.midi_control + 0x15, hXml);
	ReadXml_Int("midi", "control", "S22", 0, (gs->config).input.midi_control + 0x16, hXml);
	ReadXml_Int("midi", "control", "S23", 0, (gs->config).input.midi_control + 0x17, hXml);
	ReadXml_Int("midi", "control", "S24", 0, (gs->config).input.midi_control + 0x18, hXml);
	ReadXml_Int("midi", "control", "S25", 0, (gs->config).input.midi_control + 0x19, hXml);
	ReadXml_Int("midi", "control", "S26", 0, (gs->config).input.midi_control + 0x1a, hXml);
	ReadXml_Int("midi", "control", "S27", 0, (gs->config).input.midi_control + 0x1b, hXml);
	ReadXml_Int("midi", "control", "S28", 0, (gs->config).input.midi_control + 0x1c, hXml);
	ReadXml_Int("midi", "control", "S29", 0, (gs->config).input.midi_control + 0x1d, hXml);
	ReadXml_Int("midi", "control", "S30", 0, (gs->config).input.midi_control + 0x1e, hXml);
	ReadXml_Int("midi", "control", "S31", 0, (gs->config).input.midi_control + 0x1f, hXml);
	ReadXml_Int("midi", "control", "S32", 0, (gs->config).input.midi_control + 0x20, hXml);
	ReadXml_Int("midi", "control", "S33", 0, (gs->config).input.midi_control + 0x21, hXml);
	ReadXml_Int("midi", "control", "S34", 0, (gs->config).input.midi_control + 0x22, hXml);
	ReadXml_Int("midi", "control", "S35", 0, (gs->config).input.midi_control + 0x23, hXml);
	ReadXml_Int("midi", "control", "S36", 0, (gs->config).input.midi_control + 0x25, hXml);
	ReadXml_Int("midi", "control", "S37", 0, (gs->config).input.midi_control + 0x24, hXml);
	ReadXml_Int("midi", "control", "S38", 0, (gs->config).input.midi_control + 0x26, hXml);
	ReadXml_Int("midi", "control", "S39", 0, (gs->config).input.midi_control + 0x27, hXml);
	delete(hXml);
	return 1;
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

//43d080
int WriteConfigXml(game *g, const char *filename){
	FILE *pFile;
	char buf[256];
	
	pFile = fopen(filename, "w");
	if (pFile == NULL) return 0; 
	
	fputs("<?xml version=\"1.0\" encoding=\"shift_jis\"?>\n", pFile);
	fputs("<config>\n", pFile);

	fputs("\t<system>\n", pFile);
	WriteXML_Tab2Int(pFile, "screenmode", (g->config).system.screenmode);
	WriteXML_Tab2Int(pFile, "vsync", (g->config).system.vsync);
	WriteXML_Tab2Int(pFile, "directdraw", (g->config).system.directdraw);
	WriteXML_Tab2Int(pFile, "highcolor", (g->config).system.highcolor);
	WriteXML_Tab2Int(pFile, "autoreload", (g->config).jukebox.autoreload);
	WriteXML_Tab2Int(pFile, "customfolder", (g->config).jukebox.customfolder);
	WriteXML_Tab2Int(pFile, "mainsleep", (g->config).system.mainsleep);
	WriteXML_Tab2Int(pFile, "bmssleep", (g->config).system.bmssleep);
	WriteXML_Tab2Int(pFile, "screenexrate", (g->config).system.screenexrate);
	WriteXML_Tab2Int(pFile, "inputinterval", (g->config).input.sys_inputinterval);
	WriteXML_Tab2Int(pFile, "disablesystemkey", (g->config).system.disablesystemkey);
	WriteXML_Tab2Int(pFile, "outputlog", (g->config).system.outputlog);
	WriteXML_Tab2Int(pFile, "thread", (g->config).system.thread);
	WriteXML_Tab2Int(pFile, "eventmode", (g->config).system.eventmode);
	WriteXML_Tab2Int(pFile, "disableskinpreview", (g->config).system.disableskinpreview);
	WriteXML_Tab2Str(pFile, "newsongfolder", (g->config).jukebox.newsongfolder);
	WriteXML_Tab2Int(pFile, "titleflash", (g->config).jukebox.titleflash);
	WriteXML_Tab2Int(pFile, "hptimer", (g->config).system.hptimer);
	WriteXML_Tab2Int(pFile, "disablebmsthread", (g->config).system.isablebmsthread);
	WriteXML_Tab2Int(pFile, "disablefolderthread", (g->config).system.disablefolderthread);
	WriteXML_Tab2Int(pFile, "maindisplay", (g->config).system.maindisplay);
	WriteXML_Tab2Int(pFile, "language", (g->config).system.language);
	WriteXML_Tab2Int(pFile, "windowsize_x", (g->config).system.windowsize_x);
	WriteXML_Tab2Int(pFile, "windowsize_y", (g->config).system.windowsize_y);
	WriteXML_Tab2Int(pFile, "softwarerendering", (g->config).system.softwarerendering);
	fputs("\t</system>\n", pFile);

	fputs("\t<jukebox>\n", pFile);
	for (int i = 0; i < g->config.jukebox.numOfPath; i++) {
		WriteXML_Tab2Str(pFile, "path", g->config.jukebox.path[i]);
	}
	fputs("\t</jukebox>\n", pFile);

	fputs("\t<play>\n", pFile);
	sprintf(buf, "\t\t<%s>%d</%s>\n", "hs", (g->config).play.hiSpeed[0], "hs");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "hstype", (g->config).play.hsfix, "hstype");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "hsmax", (g->config).play.hsmax, "hsmax");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "hsmin", (g->config).play.hsmin, "hsmin");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "hsmargin", (g->config).play.hsmargin, "hsmargin");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "shutter", (g->config).play.p1_lanecoverv, "shutter");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "shuttertype", (g->config).play.p1_lanecover, "shuttertype");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "shuttermargin", (g->config).play.shuttermargin, "shuttermargin");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "gauge", (g->config).play.gaugeOption[0], "gauge");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "random", (g->config).play.random[0], "random");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "effect", (g->config).play.m_HIDSUD1, "effect");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "autoscratch", (g->config).play.p1_assist, "autoscratch");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "autokey", (g->config).play.autokey, "autokey");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "autojudgeadjust", (g->config).play.autojudge, "autojudgeadjust");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "judgetime", (g->config).play.judgetiming, "judgetime");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "bga", (g->config).play.bga, "bga");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "bgasize", (g->config).play.bgasize, "bgasize");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "poorbga", (g->config).play.poorbga, "poorbga");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "ghost", (g->config).play.play_ghost, "ghost");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "scoregraph", (g->config).play.scoregraph, "scoregraph");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "target", (g->config).play.p1_target, "target");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "defaulttarget", (g->config).play.target_percent, "defaulttarget");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "replaysave", (g->config).play.replay, "replaysave");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "basespeed", (g->config).play.basespeed, "basespeed");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "gomiscore", (g->config).play.gomiscore, "gomiscore");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "disableleftclickexit", (g->config).play.disableleftclickexit, "disableleftclickexit");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "disablecurspeedchange", (g->config).play.disablecurspeedchange, "disablecurspeedchange");
	fputs(buf, pFile);
	fputs("\t</play>\n", pFile);

	fputs("\t<sound>\n", pFile);
	sprintf(buf, "\t\t<%s>%d</%s>\n", "bufferlength", (g->config).sound.bufferlength, "bufferlength");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "numbuffers", (g->config).sound.numbuffers, "numbuffers");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "disabledsp", (g->config).sound.disabledsp, "disabledsp");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "output", (g->config).sound.output, "output");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "driver", (g->config).sound.driver, "driver");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "disablefmod", (g->config).sound.disablefmod, "disablefmod");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "volumeflag", (g->config).sound.volumeflag, "volumeflag");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "volumebgm", (g->config).sound.volumebgm, "volumebgm");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "volumekey", (g->config).sound.volumekey, "volumekey");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "volumemaster", (g->config).sound.volumemaster, "volumemaster");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "eqflag", (g->config).sound.eqflag, "eqflag");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "eqp0", (g->config).sound.eqp0, "eqp0");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "eqp1", (g->config).sound.eqp1, "eqp1");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "eqp2", (g->config).sound.eqp2, "eqp2");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "eqp3", (g->config).sound.eqp3, "eqp3");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "eqp4", (g->config).sound.eqp4, "eqp4");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "eqp5", (g->config).sound.eqp5, "eqp5");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "eqp6", (g->config).sound.eqp6, "eqp6");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "pitchflag", (g->config).sound.pitchflag, "pitchflag");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "pitchtype", (g->config).sound.pitchtype, "pitchtype");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "pitchp", (g->config).sound.pitchp, "pitchp");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "fxflag_0", (g->config).sound.fxflag_0, "fxflag_0");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "fxtype_0", (g->config).sound.fxtype_0, "fxtype_0");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "fxtarget_0", (g->config).sound.fxtarget_0, "fxtarget_0");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "fxp1_0", (g->config).sound.fxp1_0, "fxp1_0");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "fxp2_0", (g->config).sound.fxp2_0, "fxp2_0");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "fxflag_1", (g->config).sound.fxflag_1, "fxflag_1");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "fxtype_1", (g->config).sound.fxtype_1, "fxtype_1");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "fxtarget_1", (g->config).sound.fxtarget_1, "fxtarget_1");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "fxp1_1", (g->config).sound.fxp1_1, "fxp1_1");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "fxp2_1", (g->config).sound.fxp2_1, "fxp2_1");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "fxflag_2", (g->config).sound.fxflag_2, "fxflag_2");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "fxtype_2", (g->config).sound.fxtype_2, "fxtype_2");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "fxtarget_2", (g->config).sound.fxtarget_2, "fxtarget_2");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "fxp1_2", (g->config).sound.fxp1_2, "fxp1_2");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "fxp2_2", (g->config).sound.fxp2_2, "fxp2_2");
	fputs(buf, pFile);
	fputs("\t</sound>\n", pFile);

	fputs("\t<player>\n", pFile);
	WriteXML_Tab2Str(pFile, "id", (g->config).player.id);
	WriteXML_Tab2Str(pFile, "pass", (g->config).player.pass);
	WriteXML_Tab2Str(pFile, "name", (g->config).player.name);
	WriteXML_Tab2Str(pFile, "irid", (g->config).player.lrid);
	WriteXML_Tab2Str(pFile, "irpass", (g->config).player.irpass);
	fputs("\t</player>\n", pFile);

	fputs("\t<select>\n", pFile);
	sprintf(buf, "\t\t<%s>%d</%s>\n", "sort", (g->config).select.sort, "sort");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "key", (g->config).select.key, "key");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "difficulty", (g->config).select.difficulty, "difficulty");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "searchmax", (g->config).select.searchmax, "searchmax");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "randomclose", (g->config).select.randomclose, "randomclose");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "speedfirst", (g->config).select.speedfirst, "speedfirst");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "speednext", (g->config).select.speednext, "speednext");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "control", (g->config).select.control, "control");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "buttonselect", (g->config).select.buttonselect, "buttonselect");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "folderlamp", (g->config).select.folderlamp, "folderlamp");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "difficultychangetype", (g->config).select.difficultychangetype, "difficultychangetype");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "ignorekeyall", (g->config).select.ignorekeyall, "ignorekeyall");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "ignorekeysingle", (g->config).select.ignorekeysingle, "ignorekeysingle");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "ignorekeydouble", (g->config).select.ignorekeydouble, "ignorekeydouble");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "ignoredp", (g->config).select.ignoredp, "ignoredp");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "ignorepms", (g->config).select.ignorepms, "ignorepms");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "ignoredifficultyall", (g->config).select.ignoredifficultyall, "ignoredifficultyall");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "ignore5key", (g->config).select.ignore5key, "ignore5key");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "levelbarflash_7", (g->config).select.levelbarflash_7, "levelbarflash_7");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "levelbarflash_5", (g->config).select.levelbarflash_5, "levelbarflash_5");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "levelbarflash_9", (g->config).select.levelbarflash_9, "levelbarflash_9");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "disabledifficultyfilter", (g->config).select.disabledifficultyfilter, "disabledifficultyfilter");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "preview", (g->config).select.preview, "preview");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "disablesubtitle", (g->config).select.disablesubtitle, "disablesubtitle");
	fputs(buf, pFile);
	fputs("\t</select>\n", pFile);
	
	fputs("\t<skin>\n", pFile);
	WriteXML_Tab2Str(pFile, "play_7", (g->config).skin.skinFilePath[0]);
	WriteXML_Tab2Str(pFile, "play_5", (g->config).skin.skinFilePath[1]);
	WriteXML_Tab2Str(pFile, "play_14", (g->config).skin.skinFilePath[2]);
	WriteXML_Tab2Str(pFile, "play_10", (g->config).skin.skinFilePath[3]);
	WriteXML_Tab2Str(pFile, "play_9", (g->config).skin.skinFilePath[4]);
	WriteXML_Tab2Str(pFile, "select", (g->config).skin.skinFilePath[5]);
	WriteXML_Tab2Str(pFile, "decide", (g->config).skin.skinFilePath[6]);
	WriteXML_Tab2Str(pFile, "result", (g->config).skin.skinFilePath[7]);
	WriteXML_Tab2Str(pFile, "keyconfig", (g->config).skin.skinFilePath[8]);
	WriteXML_Tab2Str(pFile, "skinselect", (g->config).skin.skinFilePath[9]);
	WriteXML_Tab2Str(pFile, "soundset", (g->config).skin.skinFilePath[10]);
	WriteXML_Tab2Str(pFile, "theme", (g->config).skin.skinFilePath[11]);
	WriteXML_Tab2Str(pFile, "play_7_b", (g->config).skin.skinFilePath[12]);
	WriteXML_Tab2Str(pFile, "play_5_b", (g->config).skin.skinFilePath[13]);
	WriteXML_Tab2Str(pFile, "play_9_b", (g->config).skin.skinFilePath[14]);
	WriteXML_Tab2Str(pFile, "fontname", (g->config).skin.fontname);
	sprintf(buf, "\t\t<%s>%d</%s>\n", "disableimagefont", (g->config).skin.disableimagefont, "disableimagefont");
	fputs(buf, pFile);
	fputs("\t</skin>\n", pFile);

	fputs("\t<network>\n", pFile);
	sprintf(buf, "\t\t<%s>%d</%s>\n", "lr1ir", (g->config).network.lr1ir, "lr1ir");
	fputs(buf, pFile);

	WriteXML_Tab2Str(pFile, "lr1id", (g->config).network.lr1id);
	WriteXML_Tab2Str(pFile, "lr1pass", (g->config).network.lr1pass);
	sprintf(buf, "\t\t<%s>%d</%s>\n", "lr2ir", (g->config).network.lr2ir, "lr2ir");
	fputs(buf, pFile);

	WriteXML_Tab2Str(pFile, "mail", (g->config).network.mail);
	sprintf(buf, "\t\t<%s>%d</%s>\n", "autoupdate", (g->config).network.autoupdate, "autoupdate");
	fputs(buf, pFile);

	sprintf(buf, "\t\t<%s>%d</%s>\n", "getrival", (g->config).network.getrival, "getrival");
	fputs(buf, pFile);
	fputs("\t</network>\n", pFile);

	//below this, seems like all uses different buf[256]. but I think there is no reason
	fputs("\t<course>\n", pFile);
	sprintf(buf, "\t\t<%s>%d</%s>\n", "optimumlevel_7", (g->config).course.optimumlevel_7, "optimumlevel_7");
	fputs(buf, pFile);
	sprintf(buf, "\t\t<%s>%d</%s>\n", "optimumlevel_5", (g->config).course.optimumlevel_5, "optimumlevel_5");
	fputs(buf, pFile);
	sprintf(buf, "\t\t<%s>%d</%s>\n", "optimumlevel_10", (g->config).course.optimumlevel_10, "optimumlevel_10");
	fputs(buf, pFile);
	sprintf(buf, "\t\t<%s>%d</%s>\n", "optimumlevel_14", (g->config).course.optimumlevel_14, "optimumlevel_14");
	fputs(buf, pFile);
	sprintf(buf, "\t\t<%s>%d</%s>\n", "optimumlevel_9", (g->config).course.optimumlevel_9, "optimumlevel_9");
	fputs(buf, pFile);
	sprintf(buf, "\t\t<%s>%d</%s>\n", "defaultconnection", (g->config).course.defaultconnection, "defaultconnection");
	fputs(buf, pFile);
	sprintf(buf, "\t\t<%s>%d</%s>\n", "defaultgauge", (g->config).course.defaultgauge, "defaultgauge");
	fputs(buf, pFile);
	sprintf(buf, "\t\t<%s>%d</%s>\n", "maxbpm", (g->config).course.maxbpm, "maxbpm");
	fputs(buf, pFile);
	sprintf(buf, "\t\t<%s>%d</%s>\n", "minbpm", (g->config).course.minbpm, "minbpm");
	fputs(buf, pFile);
	sprintf(buf, "\t\t<%s>%d</%s>\n", "bpmrange", (g->config).course.bpmrange, "bpmrange");
	fputs(buf, pFile);
	sprintf(buf, "\t\t<%s>%d</%s>\n", "maxlevel", (g->config).course.maxlevel, "maxlevel");
	fputs(buf, pFile);
	sprintf(buf, "\t\t<%s>%d</%s>\n", "minlevel", (g->config).course.minlevel, "minlevel");
	fputs(buf, pFile);
	sprintf(buf, "\t\t<%s>%d</%s>\n", "stage", (g->config).course.stage, "stage");
	fputs(buf, pFile);
	fputs("\t</course>\n", pFile);

	fputs("\t<tools>\n", pFile);
	WriteXML_Tab2Str(pFile, "bmse_body", (g->config).tools.bmse_body);
	WriteXML_Tab2Str(pFile, "bmse_option", (g->config).tools.bmse_option);
	sprintf(buf, "\t\t<%s>%d</%s>\n", "movie_audio", (g->config).tools.movie_audio, "movie_audio");
	fputs(buf, pFile);
	sprintf(buf, "\t\t<%s>%d</%s>\n", "movie_framerate", (g->config).tools.movie_framerate, "movie_framerate");
	fputs(buf, pFile);
	sprintf(buf, "\t\t<%s>%d</%s>\n", "mp3_volume", (g->config).tools.mp3_volume, "mp3_volume");
	fputs(buf, pFile);
	WriteXML_Tab2Str(pFile, "mp3enc_body", (g->config).tools.mp3enc_body);
	WriteXML_Tab2Str(pFile, "mp3enc_option_movie", (g->config).tools.mp3enc_option_movie);
	WriteXML_Tab2Str(pFile, "mp3enc_option_normal", (g->config).tools.mp3enc_option_normal);
	WriteXML_Tab2Str(pFile, "oggdec_body", (g->config).tools.oggdec_body);
	WriteXML_Tab2Str(pFile, "oggdec_option", (g->config).tools.oggdec_option);
	WriteXML_Tab2Str(pFile, "oggenc_body", (g->config).tools.oggenc_body);
	WriteXML_Tab2Str(pFile, "oggenc_option", (g->config).tools.oggenc_option);
	sprintf(buf, "\t\t<%s>%d</%s>\n", "autowavtoogg", (g->config).tools.autowavtoogg, "autowavtoogg");
	fputs(buf, pFile);
	sprintf(buf, "\t\t<%s>%d</%s>\n", "autobmptopng", (g->config).tools.autobmptopng,	"autobmptopng");
	fputs(buf, pFile);
	sprintf(buf, "\t\t<%s>%d</%s>\n", "autofumensearch", (g->config).tools.autofumensearch, "autofumensearch");
	fputs(buf, pFile);
	fputs("\t</tools>\n", pFile);

	fputs("</config>\n", pFile);
	fclose(pFile);
	return 1;
}

//43ebd0
void WriteXML_KeyConfig(FILE *hFile, CONFIG_INPUT cfg_in, const char *tag, int num){
	char buf[256];

	fputs("\t<", hFile);
	fputs(tag, hFile);
	fputs(">\n", hFile);
	for (int i = 0; i < 16; i++) {
		if (cfg_in.buttonMap[num][i]) {
			sprintf(buf, "\t\t<%s>%d</%s>\n", "id", cfg_in.buttonMap[num][i], "id");
			fputs(buf, hFile);
		}
	}
	fputs("\t</", hFile);
	fputs(tag, hFile);
	fputs(">\n", hFile);
	return;
}

//43ecb0
int WriteKeyConfig(game *g, const char *filepath, int key) {
	FILE *pFile;
	CONFIG_INPUT cfg_in;

	pFile = fopen(filepath, "w");
	if (pFile == (FILE *)0x0) {
		return 0;
	}

	fputs("<?xml version=\"1.0\" encoding=\"shift_jis\"?>\n", pFile);
	fputs("<keyconfig>\n", pFile);
	
	if (key == 9) {
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key01", 1);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key02", 2);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key03", 3);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key04", 4);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key05", 5);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key06", 6);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key07", 7);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key08", 8);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key09", 9);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key12", 12);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key13", 13);
	}
	else if (key == 5) {
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key01", 1);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key02", 2);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key03", 3);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key04", 4);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key05", 5);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key10", 10);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key11", 11);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key12", 12);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key13", 13);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key21", 21);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key22", 22);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key23", 23);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key24", 24);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key25", 25);
	}
	else {
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key01", 1);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key02", 2);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key03", 3);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key04", 4);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key05", 5);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key06", 6);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key07", 7);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key10", 10);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key11", 11);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key12", 12);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key13", 13);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key21", 21);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key22", 22);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key23", 23);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key24", 24);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key25", 25);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key26", 26);
		memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
		WriteXML_KeyConfig(pFile, cfg_in, "key27", 27);
	}
	memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
	WriteXML_KeyConfig(pFile, cfg_in, "key30", 30);
	memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
	WriteXML_KeyConfig(pFile, cfg_in, "key31", 31); 
	memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
	WriteXML_KeyConfig(pFile, cfg_in, "key32", 32);
	memcpy(&cfg_in, &g->config.input, sizeof(CONFIG_INPUT));
	WriteXML_KeyConfig(pFile, cfg_in, "key33", 33);

	fputs("</keyconfig>\n", pFile);
	fclose(pFile);
	return 1;
}

//43f370
int WriteMidiXml(game *g, const char *filename) {
	FILE *pFile;

	pFile = fopen(filename, "w");
	if (pFile == NULL) {
		return 0;
	}
	fputs("<?xml version=\"1.0\" encoding=\"shift_jis\"?>\n", pFile);
	fputs("<midi>\n", pFile);
	fputs("\t<control>\n", pFile);
	WriteXML_Tab2Int(pFile, "S01", (g->config).input.midi_control[1]);
	WriteXML_Tab2Int(pFile, "S02", (g->config).input.midi_control[2]);
	WriteXML_Tab2Int(pFile, "S03", (g->config).input.midi_control[3]);
	WriteXML_Tab2Int(pFile, "S04", (g->config).input.midi_control[4]);
	WriteXML_Tab2Int(pFile, "S05", (g->config).input.midi_control[5]);
	WriteXML_Tab2Int(pFile, "S06", (g->config).input.midi_control[6]);
	WriteXML_Tab2Int(pFile, "S07", (g->config).input.midi_control[7]);
	WriteXML_Tab2Int(pFile, "S08", (g->config).input.midi_control[8]);
	WriteXML_Tab2Int(pFile, "S09", (g->config).input.midi_control[9]);
	WriteXML_Tab2Int(pFile, "S10", (g->config).input.midi_control[10]);
	WriteXML_Tab2Int(pFile, "S11", (g->config).input.midi_control[0xb]);
	WriteXML_Tab2Int(pFile, "S12", (g->config).input.midi_control[0xc]);
	WriteXML_Tab2Int(pFile, "S13", (g->config).input.midi_control[0xd]);
	WriteXML_Tab2Int(pFile, "S14", (g->config).input.midi_control[0xe]);
	WriteXML_Tab2Int(pFile, "S15", (g->config).input.midi_control[0xf]);
	WriteXML_Tab2Int(pFile, "S16", (g->config).input.midi_control[0x10]);
	WriteXML_Tab2Int(pFile, "S17", (g->config).input.midi_control[0x11]);
	WriteXML_Tab2Int(pFile, "S18", (g->config).input.midi_control[0x12]);
	WriteXML_Tab2Int(pFile, "S19", (g->config).input.midi_control[0x13]);
	WriteXML_Tab2Int(pFile, "S20", (g->config).input.midi_control[0x14]);
	WriteXML_Tab2Int(pFile, "S21", (g->config).input.midi_control[0x15]);
	WriteXML_Tab2Int(pFile, "S22", (g->config).input.midi_control[0x16]);
	WriteXML_Tab2Int(pFile, "S23", (g->config).input.midi_control[0x17]);
	WriteXML_Tab2Int(pFile, "S24", (g->config).input.midi_control[0x18]);
	WriteXML_Tab2Int(pFile, "S25", (g->config).input.midi_control[0x19]);
	WriteXML_Tab2Int(pFile, "S26", (g->config).input.midi_control[0x1a]);
	WriteXML_Tab2Int(pFile, "S27", (g->config).input.midi_control[0x1b]);
	WriteXML_Tab2Int(pFile, "S28", (g->config).input.midi_control[0x1c]);
	WriteXML_Tab2Int(pFile, "S29", (g->config).input.midi_control[0x1d]);
	WriteXML_Tab2Int(pFile, "S30", (g->config).input.midi_control[0x1e]);
	WriteXML_Tab2Int(pFile, "S31", (g->config).input.midi_control[0x1f]);
	WriteXML_Tab2Int(pFile, "S32", (g->config).input.midi_control[0x20]);
	WriteXML_Tab2Int(pFile, "S33", (g->config).input.midi_control[0x21]);
	WriteXML_Tab2Int(pFile, "S34", (g->config).input.midi_control[0x22]);
	WriteXML_Tab2Int(pFile, "S35", (g->config).input.midi_control[0x23]);
	WriteXML_Tab2Int(pFile, "S36", (g->config).input.midi_control[0x24]);
	WriteXML_Tab2Int(pFile, "S37", (g->config).input.midi_control[0x25]);
	WriteXML_Tab2Int(pFile, "S38", (g->config).input.midi_control[0x26]);
	WriteXML_Tab2Int(pFile, "S39", (g->config).input.midi_control[0x27]);
	fputs("\t</control>\n", pFile);
	fputs("\t<program>\n", pFile);
	fputs("\t</program>\n", pFile);
	fputs("</midi>\n", pFile);
	fclose(pFile);
	return 1;
}

//43f6d0
int ReadSkinCustomize(SkinUser *sku, char *FilePath) {
	TiXmlDocument *hXml;
	TiXmlElement *xmlCur;

	(sku->adjust).dark_type = 0;
	(sku->adjust).judge_x = 0;
	(sku->adjust).judge_y = 0;
	(sku->adjust).note_1p_x = 0;
	(sku->adjust).note_1p_y = 0;
	(sku->adjust).note_2p_x = 0;
	(sku->adjust).note_2p_y = 0;
	(sku->adjust).unk18 = 0;
	(sku->adjust).unk1c = 0;
	(sku->adjust).size_x = 0;
	(sku->adjust).size_y = 0;
	sku->customize_value[0] = 0;
	sku->customize_value[1] = 0;
	sku->customize_value[2] = 0;
	sku->customize_value[3] = 0;
	sku->customize_value[4] = 0;
	sku->customize_value[5] = 0;
	sku->customize_value[6] = 0;
	sku->customize_value[7] = 0;
	sku->customize_value[8] = 0;
	sku->customize_value[9] = 0;
	sku->customize_value[10] = 0;
	sku->customize_value[0xb] = 0;
	sku->customize_value[0xc] = 0;
	sku->customize_value[0xd] = 0;
	sku->customize_value[0xe] = 0;
	sku->customize_value[0xf] = 0;
	sku->customize_value[0x10] = 0;
	sku->customize_value[0x11] = 0;
	sku->customize_value[0x12] = 0;
	sku->customize_value[0x13] = 0;
	(sku->adjust).shift_x = 0;
	(sku->adjust).shift_y = 0;
	(sku->adjust).rate_x = 100;
	(sku->adjust).rate_y = 100;
	
	hXml = new TiXmlDocument(FilePath);
	if (hXml->LoadFile(TIXML_ENCODING_UNKNOWN) == false) {
		if (hXml) {
			delete(hXml);
		}
		hXml = NULL;
	}

	ReadXml_Int("skincustomize", "rate", "x", 100, &(sku->adjust).rate_x, hXml);
	ReadXml_Int("skincustomize", "rate", "y", 100, &(sku->adjust).rate_y, hXml);
	ReadXml_Int("skincustomize", "shift", "x", 0, &(sku->adjust).shift_x, hXml);
	ReadXml_Int("skincustomize", "shift", "y", 0, &(sku->adjust).shift_y, hXml);
	ReadXml_Int("skincustomize", "judge", "x", 0, &(sku->adjust).judge_x, hXml);
	ReadXml_Int("skincustomize", "judge", "y", 0, &(sku->adjust).judge_y, hXml);
	ReadXml_Int("skincustomize", "size", "x", 0, &(sku->adjust).size_x, hXml);
	ReadXml_Int("skincustomize", "size", "y", 0, &(sku->adjust).size_y, hXml);
	ReadXml_Int("skincustomize", "dark", "type", 0, &(sku->adjust).dark_type, hXml);
	ReadXml_Int("skincustomize", "note_1p", "x", 0, &(sku->adjust).note_1p_x, hXml);
	ReadXml_Int("skincustomize", "note_1p", "y", 0, &(sku->adjust).note_1p_y, hXml);
	ReadXml_Int("skincustomize", "note_2p", "x", 0, &(sku->adjust).note_2p_x, hXml);
	ReadXml_Int("skincustomize", "note_2p", "y", 0, &(sku->adjust).note_2p_y, hXml);

	//it's doubled code, so changed a little
	for (int i = 0; i < 40; i++) {
		sku->customize_value[i];
		CSTR tmp;

		cstrSprintf(&tmp, "config_%d", i); //cstrSprintf(&tmp, "confit_%d", i);
		if( hXml && (xmlCur = hXml->FirstChildElement("skincustomize")) && (xmlCur = xmlCur->FirstChildElement("skinconfig")) && (xmlCur = xmlCur->FirstChildElement(tmp)) )
			sku->customize_value[i] = atol(xmlCur->ToElement()->GetText());
		else
			sku->customize_value[i] = 0;

		cstrSprintf(&tmp, "filename_%d", i);
		ReadXml_Str("skincustomize", "customfile", tmp, CSTR(""), &sku->customize_filename[i], hXml);
	}

	////0~39 repeat
	//if (hXml && (xmlCur = hXml->FirstChildElement("skincustomize")) && (xmlCur = xmlCur->FirstChildElement("skinconfig")) && (xmlCur->FirstChildElement("config_0")))
	//	sku->customize_value[0] = atol(xmlCur->ToElement()->GetText());
	//else
	//	sku->customize_value[0] = 0;
	////0~39 repeat
	//ReadXml_Str("skincustomize", "customfile", "filename_0", CSTR(""), &sku->customize_filename[0], hXml);

	if (hXml) {
		delete(hXml);
	}

	return 1;
}

//441340 //test
int WriteSkinCustomizeXml(SkinUser *sku, char *filepath) {
	
	FILE *pFile;

	pFile = fopen(filepath, "w");
	if (pFile == (FILE *)0x0) {
		return 0;
	}
	fputs("<?xml version=\"1.0\" encoding=\"shift_jis\"?>\n", pFile);
	fputs("<skincustomize>\n", pFile);
	
	fputs("\t<shift>\n", pFile);
	WriteXML_Tab2Int(pFile, "x", (sku->adjust).shift_x);
	WriteXML_Tab2Int(pFile, "y", (sku->adjust).shift_y);
	fputs("\t</shift>\n", pFile);
	
	fputs("\t<judge>\n", pFile);
	WriteXML_Tab2Int(pFile, "x", (sku->adjust).judge_x);
	WriteXML_Tab2Int(pFile, "y", (sku->adjust).judge_y);
	fputs("\t</judge>\n", pFile);
	
	fputs("\t<rate>\n", pFile);
	WriteXML_Tab2Int(pFile, "x", (sku->adjust).rate_x);
	WriteXML_Tab2Int(pFile, "y", (sku->adjust).rate_y);
	fputs("\t</rate>\n", pFile);
	
	fputs("\t<size>\n", pFile);
	WriteXML_Tab2Int(pFile, "x", (sku->adjust).size_x);
	WriteXML_Tab2Int(pFile, "y", (sku->adjust).size_y);
	fputs("\t</size>\n", pFile);
	
	fputs("\t<dark>\n", pFile);
	WriteXML_Tab2Int(pFile, "type", (sku->adjust).dark_type);
	fputs("\t</dark>\n", pFile);
	
	fputs("\t<note_1p>\n", pFile);
	WriteXML_Tab2Int(pFile, "x", (sku->adjust).note_1p_x);
	WriteXML_Tab2Int(pFile, "y", (sku->adjust).note_1p_y);
	fputs("\t</note_1p>\n", pFile);
	
	fputs("\t<note_2p>\n", pFile);
	WriteXML_Tab2Int(pFile, "x", (sku->adjust).note_2p_x);
	WriteXML_Tab2Int(pFile, "y", (sku->adjust).note_2p_y);
	fputs("\t</note_2p>\n", pFile);
	
	fputs("\t<skinconfig>\n", pFile);
	WriteXML_Tab2Int(pFile, "config_0", sku->customize_value[0]);
	WriteXML_Tab2Int(pFile, "config_1", sku->customize_value[1]);
	WriteXML_Tab2Int(pFile, "config_2", sku->customize_value[2]);
	WriteXML_Tab2Int(pFile, "config_3", sku->customize_value[3]);
	WriteXML_Tab2Int(pFile, "config_4", sku->customize_value[4]);
	WriteXML_Tab2Int(pFile, "config_5", sku->customize_value[5]);
	WriteXML_Tab2Int(pFile, "config_6", sku->customize_value[6]);
	WriteXML_Tab2Int(pFile, "config_7", sku->customize_value[7]);
	WriteXML_Tab2Int(pFile, "config_8", sku->customize_value[8]);
	WriteXML_Tab2Int(pFile, "config_9", sku->customize_value[9]);
	WriteXML_Tab2Int(pFile, "config_10", sku->customize_value[10]);
	WriteXML_Tab2Int(pFile, "config_11", sku->customize_value[0xb]);
	WriteXML_Tab2Int(pFile, "config_12", sku->customize_value[0xc]);
	WriteXML_Tab2Int(pFile, "config_13", sku->customize_value[0xd]);
	WriteXML_Tab2Int(pFile, "config_14", sku->customize_value[0xe]);
	WriteXML_Tab2Int(pFile, "config_15", sku->customize_value[0xf]);
	WriteXML_Tab2Int(pFile, "config_16", sku->customize_value[0x10]);
	WriteXML_Tab2Int(pFile, "config_17", sku->customize_value[0x11]);
	WriteXML_Tab2Int(pFile, "config_18", sku->customize_value[0x12]);
	WriteXML_Tab2Int(pFile, "config_19", sku->customize_value[0x13]);
	WriteXML_Tab2Int(pFile, "config_20", sku->customize_value[0x14]);
	WriteXML_Tab2Int(pFile, "config_21", sku->customize_value[0x15]);
	WriteXML_Tab2Int(pFile, "config_22", sku->customize_value[0x16]);
	WriteXML_Tab2Int(pFile, "config_23", sku->customize_value[0x17]);
	WriteXML_Tab2Int(pFile, "config_24", sku->customize_value[0x18]);
	WriteXML_Tab2Int(pFile, "config_25", sku->customize_value[0x19]);
	WriteXML_Tab2Int(pFile, "config_26", sku->customize_value[0x1a]);
	WriteXML_Tab2Int(pFile, "config_27", sku->customize_value[0x1b]);
	WriteXML_Tab2Int(pFile, "config_28", sku->customize_value[0x1c]);
	WriteXML_Tab2Int(pFile, "config_29", sku->customize_value[0x1d]);
	WriteXML_Tab2Int(pFile, "config_30", sku->customize_value[0x1e]);
	WriteXML_Tab2Int(pFile, "config_31", sku->customize_value[0x1f]);
	WriteXML_Tab2Int(pFile, "config_32", sku->customize_value[0x20]);
	WriteXML_Tab2Int(pFile, "config_33", sku->customize_value[0x21]);
	WriteXML_Tab2Int(pFile, "config_34", sku->customize_value[0x22]);
	WriteXML_Tab2Int(pFile, "config_35", sku->customize_value[0x23]);
	WriteXML_Tab2Int(pFile, "config_36", sku->customize_value[0x24]);
	WriteXML_Tab2Int(pFile, "config_37", sku->customize_value[0x25]);
	WriteXML_Tab2Int(pFile, "config_38", sku->customize_value[0x26]);
	WriteXML_Tab2Int(pFile, "config_39", sku->customize_value[0x27]);
	fputs("\t</skinconfig>\n", pFile);
	
	fputs("\t<customfile>\n", pFile);
	WriteXML_Tab2Str(pFile, "filename_0", sku->customize_filename[0]);
	WriteXML_Tab2Str(pFile, "filename_1", sku->customize_filename[1]);
	WriteXML_Tab2Str(pFile, "filename_2", sku->customize_filename[2]);
	WriteXML_Tab2Str(pFile, "filename_3", sku->customize_filename[3]);
	WriteXML_Tab2Str(pFile, "filename_4", sku->customize_filename[4]);
	WriteXML_Tab2Str(pFile, "filename_5", sku->customize_filename[5]);
	WriteXML_Tab2Str(pFile, "filename_6", sku->customize_filename[6]);
	WriteXML_Tab2Str(pFile, "filename_7", sku->customize_filename[7]);
	WriteXML_Tab2Str(pFile, "filename_8", sku->customize_filename[8]);
	WriteXML_Tab2Str(pFile, "filename_9", sku->customize_filename[9]);
	WriteXML_Tab2Str(pFile, "filename_10", sku->customize_filename[10]);
	WriteXML_Tab2Str(pFile, "filename_11", sku->customize_filename[11]);
	WriteXML_Tab2Str(pFile, "filename_12", sku->customize_filename[12]);
	WriteXML_Tab2Str(pFile, "filename_13", sku->customize_filename[13]);
	WriteXML_Tab2Str(pFile, "filename_14", sku->customize_filename[14]);
	WriteXML_Tab2Str(pFile, "filename_15", sku->customize_filename[15]);
	WriteXML_Tab2Str(pFile, "filename_16", sku->customize_filename[16]);
	WriteXML_Tab2Str(pFile, "filename_17", sku->customize_filename[17]);
	WriteXML_Tab2Str(pFile, "filename_18", sku->customize_filename[18]);
	WriteXML_Tab2Str(pFile, "filename_19", sku->customize_filename[19]);
	WriteXML_Tab2Str(pFile, "filename_20", sku->customize_filename[20]);
	WriteXML_Tab2Str(pFile, "filename_21", sku->customize_filename[21]);
	WriteXML_Tab2Str(pFile, "filename_22", sku->customize_filename[22]);
	WriteXML_Tab2Str(pFile, "filename_23", sku->customize_filename[23]);
	WriteXML_Tab2Str(pFile, "filename_24", sku->customize_filename[24]);
	WriteXML_Tab2Str(pFile, "filename_25", sku->customize_filename[25]);
	WriteXML_Tab2Str(pFile, "filename_26", sku->customize_filename[26]);
	WriteXML_Tab2Str(pFile, "filename_27", sku->customize_filename[27]);
	WriteXML_Tab2Str(pFile, "filename_28", sku->customize_filename[28]);
	WriteXML_Tab2Str(pFile, "filename_29", sku->customize_filename[29]);
	WriteXML_Tab2Str(pFile, "filename_30", sku->customize_filename[30]);
	WriteXML_Tab2Str(pFile, "filename_31", sku->customize_filename[31]);
	WriteXML_Tab2Str(pFile, "filename_32", sku->customize_filename[32]);
	WriteXML_Tab2Str(pFile, "filename_33", sku->customize_filename[33]);
	WriteXML_Tab2Str(pFile, "filename_34", sku->customize_filename[34]);
	WriteXML_Tab2Str(pFile, "filename_35", sku->customize_filename[35]);
	WriteXML_Tab2Str(pFile, "filename_36", sku->customize_filename[36]);
	WriteXML_Tab2Str(pFile, "filename_37", sku->customize_filename[37]);
	WriteXML_Tab2Str(pFile, "filename_38", sku->customize_filename[38]);
	WriteXML_Tab2Str(pFile, "filename_39", sku->customize_filename[39]);
	fputs("\t</customfile>\n", pFile);

	fputs("</skincustomize>\n", pFile);
	fclose(pFile);

	return 1;
}

//441d30
int ReadConfig(game *g, const char *filepath){
	int *piVar1;
	bool bVar2;
	TiXmlDocument *hXml;
	TiXmlDocument *pTVar7;
	uint uStack32;
	int *local_c;

	memset(&g->config.play, 0, sizeof(g->config.play));
	g->config.play.hiSpeed[0] = 200;
	g->config.play.hiSpeed[1] = 200;
	g->config.play.p1_lanecoverv = 0;
	g->config.play.p2_lanecoverv = 0;
	g->config.play.unknown_2 = 100;

	hXml = new TiXmlDocument(filepath);

	hXml->SetCondenseWhiteSpace(false);
	if (hXml->LoadFile(TIXML_ENCODING_UNKNOWN) == false) {
		if (hXml) {
			delete(hXml);
		}
		hXml = NULL;
	}

	ReadXml_Int("config", "system", "screenmode", 0, &g->config.system.screenmode, hXml);
	ReadXml_Int("config", "system", "vsync", 0, &g->config.system.vsync, hXml);
	ReadXml_Int("config", "system", "directdraw", 0, &g->config.system.directdraw, hXml);
	ReadXml_Int("config", "system", "maindisplay", 0, &g->config.system.maindisplay, hXml);
	Read_JukeboxPath(&g->config.jukebox, hXml);
	ReadXml_Str("config", "system", "newsongfolder", "NEW SONG\\", &g->config.jukebox.newsongfolder, hXml);
	ReadXml_Int("config", "system", "titleflash", 24, &g->config.jukebox.titleflash, hXml);
	ReadXml_Int("config", "system", "softwarerendering", 0, &g->config.system.softwarerendering, hXml);
	ReadXml_Int("config", "system", "autoreload", 2, &g->config.jukebox.autoreload, hXml);
	ReadXml_Int("config", "system", "customfolder", 0, &g->config.jukebox.customfolder, hXml);
	ReadXml_Int("config", "system", "mainsleep", 3, &g->config.system.mainsleep, hXml);
	ReadXml_Int("config", "system", "bmssleep", 3, &g->config.system.bmssleep, hXml);
	ReadXml_Int("config", "system", "screenexrate", 100, &g->config.system.screenexrate, hXml);
	ReadXml_Int("config", "system", "inputinterval", 16, &g->config.input.sys_inputinterval, hXml);
	ReadXml_Int("config", "system", "highcolor", 0, &g->config.system.highcolor, hXml);
	ReadXml_Int("config", "system", "disablesystemkey", 0, &g->config.system.disablesystemkey, hXml);
	ReadXml_Int("config", "system", "disableskinpreview", 0, &g->config.system.disableskinpreview, hXml);
	ReadXml_Int("config", "system", "hptimer", 0, &g->config.system.hptimer, hXml);
	ReadXml_Int("config", "system", "disablefolderthread", 0, &g->config.system.disablefolderthread, hXml);
	ReadXml_Int("config", "system", "disablebmsthread", 0, &g->config.system.isablebmsthread, hXml);
	ReadXml_Int("config", "system", "outputlog", 0, &g->config.system.outputlog, hXml);
	ReadXml_Int("config", "system", "eventmode", 0, &g->config.system.eventmode, hXml);
	ReadXml_Int("config", "system", "language", 0, &g->config.system.language, hXml);
	ReadXml_Int("config", "system", "windowsize_x", 640, &g->config.system.windowsize_x, hXml);
	ReadXml_Int("config", "system", "windowsize_y", 480, &g->config.system.windowsize_y, hXml);
	ReadXml_Int("config", "select", "sort", 0, &g->config.select.sort, hXml);
	ReadXml_Int("config", "select", "key", 1, &g->config.select.key, hXml);
	ReadXml_Int("config", "select", "difficulty", 0, &g->config.select.difficulty, hXml);

	ReadXml_Int("config", "select", "searchmax", 1000, &g->config.select.searchmax, hXml);
	if (g->config.select.searchmax <= 0) g->config.select.searchmax = 0;

	ReadXml_Int("config", "select", "speedfirst", 300, &g->config.select.speedfirst, hXml);
	if (g->config.select.speedfirst <= 0) g->config.select.speedfirst = 300;

	ReadXml_Int("config", "select", "speednext", 70, &g->config.select.speednext, hXml);
	if (g->config.select.speednext <= 0) g->config.select.speednext = 70;

	ReadXml_Int("config", "select", "control", 0, &g->config.select.control, hXml);
	ReadXml_Int("config", "select", "buttonselect", 0, &g->config.select.buttonselect, hXml);
	ReadXml_Int("config", "select", "randomclose", 0, &g->config.select.randomclose, hXml);
	ReadXml_Int("config", "select", "ignorekeyall", 0, &g->config.select.ignorekeyall, hXml);
	ReadXml_Int("config", "select", "ignorekeysingle", 0, &g->config.select.ignorekeysingle, hXml);
	ReadXml_Int("config", "select", "ignorekeydouble", 0, &g->config.select.ignorekeydouble, hXml);
	ReadXml_Int("config", "select", "ignoredp", 0, &g->config.select.ignoredp, hXml);
	ReadXml_Int("config", "select", "ignorepms", 0, &g->config.select.ignorepms, hXml);
	ReadXml_Int("config", "select", "ignoredifficultyall", 0, &g->config.select.ignoredifficultyall,	hXml);
	ReadXml_Int("config", "select", "ignore5key", 0, &g->config.select.ignore5key, hXml);
	ReadXml_Int("config", "select", "levelbarflash_7", 12, &g->config.select.levelbarflash_7, hXml);
	ReadXml_Int("config", "select", "levelbarflash_5", 9, &g->config.select.levelbarflash_5, hXml);
	ReadXml_Int("config", "select", "levelbarflash_9", 42, &g->config.select.levelbarflash_9, hXml);
	ReadXml_Int("config", "select", "preview", 1, &g->config.select.preview, hXml);
	ReadXml_Int("config", "select", "disablesubtitle", 0, &g->config.select.disablesubtitle, hXml);
	ReadXml_Int("config", "select", "difficultychangetype", 0, &g->config.select.difficultychangetype, hXml);
	ReadXml_Int("config", "select", "folderlamp", 0, &g->config.select.folderlamp, hXml);
	ReadXml_Int("config", "select", "disabledifficultyfilter", 0, &g->config.select.disabledifficultyfilter, hXml);
	ReadXml_Int("config", "system", "thread", 0, &g->config.system.thread, hXml);
	ReadXml_Int("config", "sound", "bufferlength", 384, &g->config.sound.bufferlength, hXml);
	ReadXml_Int("config", "sound", "numbuffers", 4, &g->config.sound.numbuffers, hXml);
	ReadXml_Int("config", "sound", "output", 0, &g->config.sound.output, hXml);
	ReadXml_Int("config", "sound", "driver", 0, &g->config.sound.driver, hXml);
	ReadXml_Int("config", "sound", "disablefmod", 0, &g->config.sound.disablefmod, hXml);
	ReadXml_Int("config", "sound", "volumeflag", 1, &g->config.sound.volumeflag, hXml);
	ReadXml_Int("config", "sound", "volumekey", 100, &g->config.sound.volumekey, hXml);
	ReadXml_Int("config", "sound", "volumebgm", 100, &g->config.sound.volumebgm, hXml);
	ReadXml_Int("config", "sound", "volumemaster", 100, &g->config.sound.volumemaster, hXml);
	ReadXml_Int("config", "sound", "eqflag", 0, &g->config.sound.eqflag, hXml);
	ReadXml_Int("config", "sound", "eqp0", 0, &g->config.sound.eqp0, hXml);
	ReadXml_Int("config", "sound", "eqp1", 0, &g->config.sound.eqp1, hXml);
	ReadXml_Int("config", "sound", "eqp2", 0, &g->config.sound.eqp2, hXml);
	ReadXml_Int("config", "sound", "eqp3", 0, &g->config.sound.eqp3, hXml);
	ReadXml_Int("config", "sound", "eqp4", 0, &g->config.sound.eqp4, hXml);
	ReadXml_Int("config", "sound", "eqp5", 0, &g->config.sound.eqp5, hXml);
	ReadXml_Int("config", "sound", "eqp6", 0, &g->config.sound.eqp6, hXml);
	ReadXml_Int("config", "sound", "pitchflag", 0, &g->config.sound.pitchflag, hXml);
	ReadXml_Int("config", "sound", "pitchp", 0, &g->config.sound.pitchp, hXml);
	ReadXml_Int("config", "sound", "pitchtype", 0, &g->config.sound.pitchtype, hXml);
	ReadXml_Int("config", "sound", "fxflag_0", 0, &g->config.sound.fxflag_0, hXml);
	ReadXml_Int("config", "sound", "fxp1_0", 0, &g->config.sound.fxp1_0, hXml);
	ReadXml_Int("config", "sound", "fxp2_0", 0, &g->config.sound.fxp2_0, hXml);
	ReadXml_Int("config", "sound", "fxtarget_0", 0, &g->config.sound.fxtarget_0, hXml);
	ReadXml_Int("config", "sound", "fxtype_0", 0, &g->config.sound.fxtype_0, hXml);
	ReadXml_Int("config", "sound", "fxflag_1", 0, &g->config.sound.fxflag_1, hXml);
	ReadXml_Int("config", "sound", "fxp1_1", 0, &g->config.sound.fxp1_1, hXml);
	ReadXml_Int("config", "sound", "fxp2_1", 0, &g->config.sound.fxp2_1, hXml);
	ReadXml_Int("config", "sound", "fxtarget_1", 0, &g->config.sound.fxtarget_1, hXml);
	ReadXml_Int("config", "sound", "fxtype_1", 0, &g->config.sound.fxtype_1, hXml);
	ReadXml_Int("config", "sound", "fxflag_2", 0, &g->config.sound.fxflag_2, hXml);
	ReadXml_Int("config", "sound", "fxp1_2", 0, &g->config.sound.fxp1_2, hXml);
	ReadXml_Int("config", "sound", "fxp2_2", 0, &g->config.sound.fxp2_2, hXml);
	ReadXml_Int("config", "sound", "fxtarget_2", 0, &g->config.sound.fxtarget_2, hXml);
	ReadXml_Int("config", "sound", "fxtype_2", 0, &g->config.sound.fxtype_2, hXml);
	
	g->config.sound.disabledsp = (uint)(g->config.sound.output != 2);
	if (g->config.sound.bufferlength == 0) g->config.sound.bufferlength = 256;
	if (g->config.sound.bufferlength < 16) g->config.sound.bufferlength = 16;
	if (g->config.sound.numbuffers <= 0) g->config.sound.numbuffers = 1;

	ReadXml_Int("config", "play", "hs", 100, &g->config.play.hiSpeed[0], hXml);
	g->config.play.hiSpeed[1] = g->config.play.hiSpeed[0];
	ReadXml_Int("config", "play", "gauge", 0, g->config.play.gaugeOption, hXml);
	ReadXml_Int("config", "play", "random", 0, &g->config.play.random[0], hXml);
	ReadXml_Int("config", "play", "effect", 0, &g->config.play.m_HIDSUD1, hXml);
	ReadXml_Int("config", "play", "hstype", 0, &g->config.play.hsfix, hXml);
	ReadXml_Int("config", "play", "hsmin", 10, &g->config.play.hsmin, hXml);
	ReadXml_Int("config", "play", "hsmax", 900, &g->config.play.hsmax, hXml);
	ReadXml_Int("config", "play", "hsmargin", 10, &g->config.play.hsmargin, hXml);
	ReadXml_Int("config", "play", "shuttertype", 0, &g->config.play.p1_lanecover, hXml);
	g->config.play.p2_lanecover = g->config.play.p1_lanecover;
	ReadXml_Int("config", "play", "shuttermargin", 10, &g->config.play.shuttermargin, hXml);
	ReadXml_Int("config", "play", "replaysave", 0, &g->config.play.replay, hXml);
	ReadXml_Int("config", "play", "autoscratch", 0, &g->config.play.p1_assist, hXml);
	ReadXml_Int("config", "play", "autokey", 0, &g->config.play.autokey, hXml);
	ReadXml_Int("config", "play", "shutter", 0, &g->config.play.p1_lanecoverv, hXml);
	g->config.play.p2_lanecoverv = g->config.play.p1_lanecoverv;
	ReadXml_Int("config", "play", "autojudgeadjust", 0, &g->config.play.autojudge, hXml);
	ReadXml_Int("config", "play", "judgetime", 0, &g->config.play.judgetiming, hXml);
	ReadXml_Int("config", "play", "bga", 1, &g->config.play.bga, hXml);
	ReadXml_Int("config", "play", "poorbga", 500, &g->config.play.poorbga, hXml);
	ReadXml_Int("config", "play", "bgasize", 0, &g->config.play.bgasize, hXml);
	ReadXml_Int("config", "play", "ghost", 0, &g->config.play.play_ghost, hXml);
	ReadXml_Int("config", "play", "scoregraph", 0, &g->config.play.scoregraph, hXml);
	ReadXml_Int("config", "play", "defaulttarget", 90, &g->config.play.target_percent, hXml);
	ReadXml_Int("config", "play", "target", 0, &g->config.play.p1_target, hXml);
	ReadXml_Int("config", "play", "basespeed", 100, &g->config.play.basespeed, hXml);
	ReadXml_Int("config", "play", "gomiscore", 0, &g->config.play.gomiscore, hXml);
	ReadXml_Int("config", "play", "disableleftclickexit", 0, &g->config.play.disableleftclickexit, hXml);
	ReadXml_Int("config", "play", "disablecurspeedchange", 0, &g->config.play.disablecurspeedchange,	hXml);
	ReadXml_Str("config", "skin", "play_7", "", &g->config.skin.skinFilePath[0], hXml);
	ReadXml_Str("config", "skin", "play_5", "", &g->config.skin.skinFilePath[1], hXml);
	ReadXml_Str("config", "skin", "play_14", "", &g->config.skin.skinFilePath[2], hXml);
	ReadXml_Str("config", "skin", "play_10", "", &g->config.skin.skinFilePath[3], hXml);
	ReadXml_Str("config", "skin", "play_9", "", &g->config.skin.skinFilePath[4], hXml);
	ReadXml_Str("config", "skin", "select", "", &g->config.skin.skinFilePath[5], hXml);
	ReadXml_Str("config", "skin", "decide", "", &g->config.skin.skinFilePath[6], hXml);
	ReadXml_Str("config", "skin", "result", "", &g->config.skin.skinFilePath[7], hXml);
	ReadXml_Str("config", "skin", "keyconfig", "", &g->config.skin.skinFilePath[8], hXml);
	ReadXml_Str("config", "skin", "skinselect", "", &g->config.skin.skinFilePath[9], hXml);
	ReadXml_Str("config", "skin", "soundset", "", &g->config.skin.skinFilePath[10], hXml);
	ReadXml_Str("config", "skin", "theme", "", &g->config.skin.skinFilePath[11], hXml);
	ReadXml_Str("config", "skin", "play_7_b", "", &g->config.skin.skinFilePath[12], hXml);
	ReadXml_Str("config", "skin", "play_5_b", "", &g->config.skin.skinFilePath[13], hXml);
	ReadXml_Str("config", "skin", "play_9_b", "", &g->config.skin.skinFilePath[14], hXml);
	ReadXml_Str("config", "skin", "fontname", "Ariel", &g->config.skin.fontname, hXml);
	ReadXml_Int("config", "skin", "disableimagefont", 0, &g->config.skin.disableimagefont, hXml);
	ReadXml_Str("config", "player", "id", "", &g->config.player.id, hXml);
	ReadXml_Str("config", "player", "pass", "", &g->config.player.pass, hXml);
	g->config.player.passMD5 = MD5str(g->config.player.pass) ;

	ReadXml_Str("config", "player", "name", "", &g->config.player.name, hXml);
	ReadXml_Str("config", "player", "irid", "", &g->config.player.lrid, hXml);
	ReadXml_Str("config", "player", "irpass", "", &g->config.player.irpass, hXml);
	ReadXml_Str("config", "network", "lr1id", "", &g->config.network.lr1id, hXml);
	ReadXml_Str("config", "network", "lr1pass", "", &g->config.network.lr1pass, hXml);
	ReadXml_Int("config", "network", "autoupdate", 0, &g->config.network.autoupdate, hXml);
	ReadXml_Int("config", "network", "getrival", 0, &g->config.network.getrival, hXml);
	ReadXml_Int("config", "network", "lr1ir", 0, &g->config.network.lr1ir, hXml);
	ReadXml_Int("config", "network", "lr2ir", 0, &g->config.network.lr2ir, hXml);
	ReadXml_Str("config", "network", "mail", "", &g->config.network.mail, hXml);
	g->config.player.irpassMD5 = MD5str(g->config.player.irpass);

	ReadXml_Int("config", "course", "optimumlevel_7", 0, &g->config.course.optimumlevel_7, hXml);
	ReadXml_Int("config", "course", "optimumlevel_5", 0, &g->config.course.optimumlevel_5, hXml);
	ReadXml_Int("config", "course", "optimumlevel_10", 0, &g->config.course.optimumlevel_10, hXml);
	ReadXml_Int("config", "course", "optimumlevel_14", 0, &g->config.course.optimumlevel_14, hXml);
	ReadXml_Int("config", "course", "optimumlevel_9", 0, &g->config.course.optimumlevel_9, hXml);
	ReadXml_Int("config", "course", "defaultconnection", 0, &g->config.course.defaultconnection, hXml);
	ReadXml_Int("config", "course", "defaultgauge", 0, &g->config.course.defaultgauge, hXml);
	ReadXml_Int("config", "course", "maxbpm", 0, &g->config.course.maxbpm, hXml);
	ReadXml_Int("config", "course", "minbpm", 0, &g->config.course.minbpm, hXml);
	ReadXml_Int("config", "course", "bpmrange", 10, &g->config.course.bpmrange, hXml);
	ReadXml_Int("config", "course", "maxlevel", 0, &g->config.course.maxlevel, hXml);
	ReadXml_Int("config", "course", "minlevel", 0, &g->config.course.minlevel, hXml);
	ReadXml_Int("config", "course", "stage", 5, &g->config.course.stage, hXml);
	ReadXml_Str("config", "tools", "bmse_body", "bmse.exe", &g->config.tools.bmse_body, hXml);
	ReadXml_Str("config", "tools", "bmse_option", "", &g->config.tools.bmse_option, hXml);
	ReadXml_Int("config", "tools", "movie_audio", 0, &g->config.tools.movie_audio, hXml);
	ReadXml_Int("config", "tools", "movie_framerate", 30, &g->config.tools.movie_framerate, hXml);
	ReadXml_Int("config", "tools", "mp3_volume", 50, &g->config.tools.mp3_volume, hXml);
	ReadXml_Str("config", "tools", "mp3enc_body", "lame.exe", &g->config.tools.mp3enc_body, hXml);
	ReadXml_Str("config", "tools", "mp3enc_option_movie","--preset cbr 192", &g->config.tools.mp3enc_option_movie, hXml);
	ReadXml_Str("config", "tools", "mp3enc_option_normal","--preset fast extreme", &g->config.tools.mp3enc_option_normal, hXml);
	ReadXml_Str("config", "tools", "oggdec_body", "oggdec.exe", &g->config.tools.oggdec_body, hXml);
	ReadXml_Str("config", "tools", "oggdec_option", "", &g->config.tools.oggdec_option, hXml);
	ReadXml_Str("config", "tools", "oggenc_body","oggenc2.exe", &g->config.tools.oggenc_body, hXml);
	ReadXml_Str("config", "tools", "oggenc_option", "-q 6", &g->config.tools.oggenc_option, hXml);
	ReadXml_Int("config", "tools", "autowavtoogg", 0, &g->config.tools.autowavtoogg, hXml);
	ReadXml_Int("config", "tools", "autobmptopng", 0, &g->config.tools.autobmptopng, hXml);
	ReadXml_Int("config", "tools", "autofumensearch", 0, &g->config.tools.autofumensearch, hXml);
	
	delete(hXml);
	return 1;
}

//443550 _ need simplification
void MD5byte(char **iStr, uint len, char *oByte){
	int iVar1;
	int iVar2;
	int iVar3;
	int iVar4;
	int iVar5;
	int iVar6;
	int iVar7;
	int iVar8;
	int iVar9;
	int iVar10;
	int iVar11;
	int *piVar12;
	uint uVar13;
	uint uVar14;
	uint uVar15;
	int *piVar16;
	uint uVar17;
	uint uVar18;
	uint uVar19;
	int iVar20;
	int iVar21;
	uint uVar22;
	uint uVar23;
	uint uVar24;

	iVar20 = 0 -(uint)(((len & 0x3f) < 0x38) & 0xffffffc0) - (len & 0x3f);
	iVar21 = iVar20 + 0x78;
	uVar13 = iVar20 + 0x80 + len;
	piVar12 = (int *)realloc(*iStr, uVar13);
	*iStr = (char *)piVar12;
	memset((undefined *)((int)piVar12 + len), 0, iVar21);
	piVar16 = (int *)((int)piVar12 + iVar21 + len);
	*(undefined *)((int)piVar12 + len) = 0x80;
	*piVar16 = len * 8;
	piVar16[1] = len >> 0x1d;
	iVar20 = 0x67452301;
	uVar23 = 0xefcdab89;
	uVar19 = 0x98badcfe;
	uVar17 = 0x10325476;
	for (uVar13 = uVar13 >> 6; uVar13 != 0; uVar13 = uVar13 - 1) {
		iVar21 = iVar20 + (~uVar23 & uVar17 | uVar19 & uVar23) + *piVar12;
		uVar14 = (iVar21 + 0xd76aa478U >> 0x19 | (iVar21 + -0x955b88) * 0x80) + uVar23;
		iVar21 = uVar17 + (~uVar14 & uVar19 | uVar23 & uVar14) + piVar12[1];
		uVar15 = (iVar21 + 0xe8c7b756U >> 0x14 | (iVar21 + 0x7b756) * 0x1000) + uVar14;
		iVar21 = uVar19 + (~uVar15 & uVar23 | uVar15 & uVar14) + piVar12[2];
		uVar22 = (iVar21 + 0x242070dbU >> 0xf | (iVar21 + -0xf25) * 0x20000) + uVar15;
		iVar21 = uVar23 + (~uVar22 & uVar14 | uVar15 & uVar22) + piVar12[3];
		uVar18 = ((iVar21 + -0x112) * 0x400000 | iVar21 + 0xc1bdceeeU >> 10) + uVar22;
		iVar21 = uVar14 + (~uVar18 & uVar15 | uVar22 & uVar18) + piVar12[4];
		uVar24 = (iVar21 + 0xf57c0fafU >> 0x19 | (iVar21 + -0x83f051) * 0x80) + uVar18;
		iVar21 = uVar15 + (~uVar24 & uVar22 | uVar18 & uVar24) + piVar12[5];
		uVar14 = (iVar21 + 0x4787c62aU >> 0x14 | (iVar21 + 0x7c62a) * 0x1000) + uVar24;
		iVar21 = uVar22 + (~uVar14 & uVar18 | uVar14 & uVar24) + piVar12[6];
		uVar15 = (iVar21 + 0xa8304613U >> 0xf | (iVar21 + -0x39ed) * 0x20000) + uVar14;
		iVar21 = uVar18 + (~uVar15 & uVar24 | uVar14 & uVar15) + piVar12[7];
		uVar18 = ((iVar21 + 0x101) * 0x400000 | iVar21 + 0xfd469501U >> 10) + uVar15;
		iVar21 = uVar24 + (~uVar18 & uVar14 | uVar15 & uVar18) + piVar12[8];
		uVar22 = (iVar21 + 0x698098d8U >> 0x19 | (iVar21 + -0x7f6728) * 0x80) + uVar18;
		iVar21 = uVar14 + (~uVar22 & uVar15 | uVar18 & uVar22) + piVar12[9];
		uVar24 = (iVar21 + 0x8b44f7afU >> 0x14 | (iVar21 + 0x4f7af) * 0x1000) + uVar22;
		iVar21 = uVar15 + (~uVar24 & uVar18 | uVar24 & uVar22) + piVar12[10];
		uVar14 = (iVar21 - 0xa44fU >> 0xf | (iVar21 + -0x244f) * 0x20000) + uVar24;
		iVar21 = uVar18 + (~uVar14 & uVar22 | uVar24 & uVar14) + piVar12[0xb];
		uVar15 = (iVar21 + 0x895cd7beU >> 10 | (iVar21 + -0x42) * 0x400000) + uVar14;
		iVar21 = uVar22 + (~uVar15 & uVar24 | uVar14 & uVar15) + piVar12[0xc];
		uVar18 = (iVar21 + 0x6b901122U >> 0x19 | (iVar21 + -0x6feede) * 0x80) + uVar15;
		iVar21 = uVar24 + (~uVar18 & uVar14 | uVar15 & uVar18) + piVar12[0xd];
		uVar22 = ((iVar21 + -0x78e6d) * 0x1000 | iVar21 + 0xfd987193U >> 0x14) + uVar18;
		iVar21 = uVar14 + (~uVar22 & uVar15 | uVar22 & uVar18) + piVar12[0xe];
		uVar24 = (iVar21 + 0xa679438eU >> 0xf | (iVar21 + -0x3c72) * 0x20000) + uVar22;
		iVar21 = uVar15 + (~uVar24 & uVar18 | uVar22 & uVar24) + piVar12[0xf];
		uVar14 = (iVar21 + 0x49b40821U >> 10 | (iVar21 + 0x21) * 0x400000) + uVar24;
		iVar21 = uVar18 + (~uVar22 & uVar24 | uVar22 & uVar14) + piVar12[1];
		uVar18 = ((iVar21 + -0x1e1da9e) * 0x20 | iVar21 + 0xf61e2562U >> 0x1b) + uVar14;
		iVar21 = uVar22 + (~uVar24 & uVar14 | uVar24 & uVar18) + piVar12[6];
		uVar15 = (iVar21 + 0xc040b340U >> 0x17 | (iVar21 + -0x3f4cc0) * 0x200) + uVar18;
		iVar21 = uVar24 + (~uVar14 & uVar18 | uVar15 & uVar14) + piVar12[0xb];
		uVar22 = ((iVar21 + -0x1a5af) * 0x4000 | iVar21 + 0x265e5a51U >> 0x12) + uVar15;
		iVar21 = uVar14 + (~uVar18 & uVar15 | uVar22 & uVar18) + *piVar12;
		uVar24 = (iVar21 + 0xe9b6c7aaU >> 0xc | (iVar21 + 0x7aa) * 0x100000) + uVar22;
		iVar1 = piVar12[5];
		iVar21 = uVar18 + (~uVar15 & uVar22 | uVar15 & uVar24) + iVar1;
		uVar14 = ((iVar21 + -0x1d0efa3) * 0x20 | iVar21 + 0xd62f105dU >> 0x1b) + uVar24;
		iVar2 = piVar12[10];
		iVar21 = uVar15 + (~uVar22 & uVar24 | uVar22 & uVar14) + iVar2;
		uVar15 = ((iVar21 + -0x3bebad) * 0x200 | iVar21 + 0x2441453U >> 0x17) + uVar14;
		iVar3 = piVar12[0xf];
		iVar21 = uVar22 + (~uVar24 & uVar14 | uVar15 & uVar24) + iVar3;
		uVar18 = (iVar21 + 0xd8a1e681U >> 0x12 | (iVar21 + 0x1e681) * 0x4000) + uVar15;
		iVar4 = piVar12[4];
		iVar21 = uVar24 + (~uVar14 & uVar15 | uVar18 & uVar14) + iVar4;
		uVar22 = (iVar21 + 0xe7d3fbc8U >> 0xc | (iVar21 + -0x438) * 0x100000) + uVar18;
		iVar21 = uVar14 + (~uVar15 & uVar18 | uVar15 & uVar22) + piVar12[9];
		uVar14 = (iVar21 + 0x21e1cde6U >> 0x1b | (iVar21 + 0x1e1cde6) * 0x20) + uVar22;
		iVar5 = piVar12[0xe];
		iVar21 = uVar15 + (~uVar18 & uVar22 | uVar18 & uVar14) + iVar5;
		uVar24 = (iVar21 + 0xc33707d6U >> 0x17 | (iVar21 + 0x3707d6) * 0x200) + uVar14;
		iVar6 = piVar12[3];
		iVar21 = uVar18 + (~uVar22 & uVar14 | uVar24 & uVar22) + iVar6;
		uVar15 = ((iVar21 + 68999) * 0x4000 | iVar21 + 0xf4d50d87U >> 0x12) + uVar24;
		iVar7 = piVar12[8];
		iVar21 = uVar22 + (~uVar14 & uVar24 | uVar15 & uVar14) + iVar7;
		uVar18 = (iVar21 + 0x455a14edU >> 0xc | (iVar21 + 0x4ed) * 0x100000) + uVar15;
		iVar8 = piVar12[0xd];
		iVar21 = (~uVar24 & uVar15 | uVar24 & uVar18) + iVar8 + uVar14;
		uVar14 = ((iVar21 + 0x1e3e905) * 0x20 | iVar21 + 0xa9e3e905U >> 0x1b) + uVar18;
		iVar9 = piVar12[2];
		iVar21 = uVar24 + (~uVar15 & uVar18 | uVar15 & uVar14) + iVar9;
		uVar22 = (iVar21 + 0xfcefa3f8U >> 0x17 | (iVar21 + -0x105c08) * 0x200) + uVar14;
		iVar10 = piVar12[7];
		iVar21 = uVar15 + (~uVar18 & uVar14 | uVar22 & uVar18) + iVar10;
		uVar24 = (iVar21 + 0x676f02d9U >> 0x12 | (iVar21 + -0xfd27) * 0x4000) + uVar22;
		iVar11 = piVar12[0xc];
		iVar21 = uVar18 + (~uVar14 & uVar22 | uVar24 & uVar14) + iVar11;
		uVar15 = (iVar21 + 0x8d2a4c8aU >> 0xc | (iVar21 + -0x376) * 0x100000) + uVar24;
		uVar14 = (uVar22 ^ uVar24 ^ uVar15) + iVar1 + -0x5c6be + uVar14;
		uVar18 = (uVar14 * 0x10 | uVar14 >> 0x1c) + uVar15;
		iVar21 = uVar22 + (uVar24 ^ uVar15 ^ uVar18) + iVar7;
		uVar22 = ((iVar21 + -0xe097f) * 0x800 | iVar21 + 0x8771f681U >> 0x15) + uVar18;
		iVar21 = uVar24 + (uVar22 ^ uVar15 ^ uVar18) + piVar12[0xb];
		uVar24 = ((iVar21 + 0x6122) * 0x10000 | iVar21 + 0x6d9d6122U >> 0x10) + uVar22;
		iVar21 = (uVar22 ^ uVar24 ^ uVar18) + iVar5 + uVar15;
		uVar14 = (iVar21 + 0xfde5380cU >> 9 | (iVar21 + 0xc) * 0x800000) + uVar24;
		iVar21 = (uVar22 ^ uVar24 ^ uVar14) + piVar12[1] + uVar18;
		uVar15 = (iVar21 + 0xa4beea44U >> 0x1c | (iVar21 + 0x4beea44) * 0x10) + uVar14;
		iVar21 = uVar22 + (uVar24 ^ uVar14 ^ uVar15) + iVar4;
		uVar22 = ((iVar21 + -0x13057) * 0x800 | iVar21 + 0x4bdecfa9U >> 0x15) + uVar15;
		iVar21 = uVar24 + (uVar22 ^ uVar14 ^ uVar15) + iVar10;
		uVar18 = ((iVar21 + 0x4b60) * 0x10000 | iVar21 + 0xf6bb4b60U >> 0x10) + uVar22;
		iVar21 = (uVar22 ^ uVar18 ^ uVar15) + iVar2 + uVar14;
		uVar14 = (iVar21 + 0xbebfbc70U >> 9 | (iVar21 + 0x70) * 0x800000) + uVar18;
		iVar21 = (uVar22 ^ uVar18 ^ uVar14) + iVar8 + uVar15;
		uVar15 = (iVar21 + 0x289b7ec6U >> 0x1c | (iVar21 + -0x764813a) * 0x10) + uVar14;
		iVar21 = uVar22 + (uVar18 ^ uVar14 ^ uVar15) + *piVar12;
		uVar22 = (iVar21 + 0xeaa127faU >> 0x15 | (iVar21 + 0x127fa) * 0x800) + uVar15;
		iVar21 = uVar18 + (uVar22 ^ uVar14 ^ uVar15) + iVar6;
		uVar18 = ((iVar21 + 0x3085) * 0x10000 | iVar21 + 0xd4ef3085U >> 0x10) + uVar22;
		iVar21 = uVar14 + (uVar22 ^ uVar18 ^ uVar15) + piVar12[6];
		uVar14 = (iVar21 + 0x4881d05U >> 9 | (iVar21 + -0xfb) * 0x800000) + uVar18;
		iVar21 = (uVar22 ^ uVar18 ^ uVar14) + piVar12[9] + uVar15;
		uVar15 = ((iVar21 + -0x62b2fc7) * 0x10 | iVar21 + 0xd9d4d039U >> 0x1c) + uVar14;
		iVar21 = (uVar18 ^ uVar14 ^ uVar15) + iVar11 + uVar22;
		uVar22 = (iVar21 + 0xe6db99e5U >> 0x15 | (iVar21 + -0x4661b) * 0x800) + uVar15;
		iVar21 = (uVar22 ^ uVar14 ^ uVar15) + iVar3 + uVar18;
		uVar24 = ((iVar21 + 0x7cf8) * 0x10000 | iVar21 + 0x1fa27cf8U >> 0x10) + uVar22;
		iVar21 = uVar14 + (uVar22 ^ uVar24 ^ uVar15) + iVar9;
		uVar18 = (iVar21 + 0xc4ac5665U >> 9 | (iVar21 + 0x65) * 0x800000) + uVar24;
		iVar21 = uVar15 + ((~uVar22 | uVar18) ^ uVar24) + *piVar12;
		uVar14 = (iVar21 + 0xf4292244U >> 0x1a | (iVar21 + 0x292244) * 0x40) + uVar18;
		iVar21 = uVar22 + ((~uVar24 | uVar14) ^ uVar18) + iVar10;
		uVar15 = (iVar21 + 0x432aff97U >> 0x16 | (iVar21 + -0x150069) * 0x400) + uVar14;
		iVar21 = uVar24 + ((~uVar18 | uVar15) ^ uVar14) + iVar5;
		uVar22 = ((iVar21 + 0x23a7) * 0x8000 | iVar21 + 0xab9423a7U >> 0x11) + uVar15;
		iVar21 = uVar18 + ((~uVar14 | uVar22) ^ uVar15) + iVar1;
		uVar24 = ((iVar21 + 0x39) * 0x200000 | iVar21 + 0xfc93a039U >> 0xb) + uVar22;
		iVar21 = uVar14 + ((~uVar15 | uVar24) ^ uVar22) + iVar11;
		uVar18 = (iVar21 + 0x655b59c3U >> 0x1a | (iVar21 + 0x15b59c3) * 0x40) + uVar24;
		iVar21 = uVar15 + ((~uVar22 | uVar18) ^ uVar24) + iVar6;
		uVar14 = ((iVar21 + 0xccc92) * 0x400 | iVar21 + 0x8f0ccc92U >> 0x16) + uVar18;
		iVar21 = uVar22 + ((~uVar24 | uVar14) ^ uVar18) + iVar2;
		uVar15 = (iVar21 - 0x100b83U >> 0x11 | (iVar21 + -0xb83) * 0x8000) + uVar14;
		iVar21 = uVar24 + ((~uVar18 | uVar15) ^ uVar14) + piVar12[1];
		uVar22 = ((iVar21 + -0x22f) * 0x200000 | iVar21 + 0x85845dd1U >> 0xb) + uVar15;
		iVar21 = uVar18 + ((~uVar14 | uVar22) ^ uVar15) + iVar7;
		uVar24 = (iVar21 + 0x6fa87e4fU >> 0x1a | (iVar21 + -0x5781b1) * 0x40) + uVar22;
		iVar21 = uVar14 + ((~uVar15 | uVar24) ^ uVar22) + iVar3;
		uVar18 = ((iVar21 + -0x131920) * 0x400 | iVar21 + 0xfe2ce6e0U >> 0x16) + uVar24;
		iVar21 = uVar15 + ((~uVar22 | uVar18) ^ uVar24) + piVar12[6];
		uVar14 = ((iVar21 + -0xbcec) * 0x8000 | iVar21 + 0xa3014314U >> 0x11) + uVar18;
		iVar21 = uVar22 + ((~uVar24 | uVar14) ^ uVar18) + iVar8;
		uVar15 = ((iVar21 + 0x1a1) * 0x200000 | iVar21 + 0x4e0811a1U >> 0xb) + uVar14;
		iVar21 = uVar24 + ((~uVar18 | uVar15) ^ uVar14) + iVar4;
		uVar22 = (iVar21 + 0xf7537e82U >> 0x1a | (iVar21 + -0xac817e) * 0x40) + uVar15;
		iVar21 = uVar18 + ((~uVar14 | uVar22) ^ uVar15) + piVar12[0xb];
		uVar18 = ((iVar21 + -0x50dcb) * 0x400 | iVar21 + 0xbd3af235U >> 0x16) + uVar22;
		iVar21 = uVar14 + ((~uVar15 | uVar18) ^ uVar22) + iVar9;
		uVar14 = ((iVar21 + -0x2d45) * 0x8000 | iVar21 + 0x2ad7d2bbU >> 0x11) + uVar18;
		iVar20 = uVar22 + iVar20;
		uVar17 = uVar18 + uVar17;
		iVar21 = uVar15 + ((~uVar22 | uVar14) ^ uVar18) + piVar12[9];
		uVar23 = (iVar21 + 0xeb86d391U >> 0xb | (iVar21 + 0x391) * 0x200000) + uVar14 + uVar23;
		uVar19 = uVar14 + uVar19;
		piVar12 = piVar12 + 0x10;
	}
	*(uint *)(oByte + 4) = uVar23;
	*(int *)oByte = iVar20;
	*(uint *)(oByte + 8) = uVar19;
	*(uint *)(oByte + 0xc) = uVar17;
	return;
}

//443ff0
char md5str[32];
char* MD5str(char *iStr) {
	char* buf;
	unsigned char md5buf[16];

	/*buf = (char*)malloc(strlen(iStr));
	strcpy(buf, iStr);
	MD5byte(&buf, strlen(buf), (char*)md5buf);
	sprintf(md5str,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", md5buf[0], md5buf[1], md5buf[2], md5buf[3], 
		md5buf[4], md5buf[5], md5buf[6], md5buf[7], md5buf[8], md5buf[9], md5buf[10], md5buf[11], md5buf[12], md5buf[13], md5buf[14], md5buf[15]);
	free(buf);*/
	buf = (char*)md5String(iStr);
	memcpy(md5buf, buf, 16);
	sprintf(md5str, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", md5buf[0], md5buf[1], md5buf[2], md5buf[3],
		md5buf[4], md5buf[5], md5buf[6], md5buf[7], md5buf[8], md5buf[9], md5buf[10], md5buf[11], md5buf[12], md5buf[13], md5buf[14], md5buf[15]);

	return md5str;
}
