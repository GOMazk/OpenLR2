#include <fstream>
#include <sstream>
#include <string>
#pragma comment(lib,"ws2_32.lib")
#include "LR2_ir.h"
#include "DxLib/DxLib.h"
#include "tinyxml/tinyxml.h"
#include "En_dbio.h"
#include "En_fileutil.h"
#include "En_timer.h"
#include "filesystem.h"

#ifdef _WIN32
#include <shellapi.h>
#endif // _WIN32

void MYRANKING::InitRanking() {
	this->songMD5.fillzero();
	this->unused.fillzero();
	this->passMD5.fillzero();
	this->title.fillzero();
	this->genre.fillzero();
	this->artist.fillzero();
	this->ghost.fillzero();
	this->maxbpm = 0;
	this->minbpm = 0;
	this->playlevel = 0;
	this->clear = 0;
	this->exscore = 0;
	this->pg = 0;
	this->gr = 0;
	this->gd = 0;
	this->bd = 0;
	this->pr = 0;
	this->maxcombo = 0;
	this->playcount = 0;
	this->clearcount = 0;
	this->rate = 0;
	this->minbp = 0;
	this->totalnotes = 0;
	this->opt_history = 0;
	this->rseed = 0;
	this->clear_sd = 0;
	this->clear_db = 0;
	this->clear_ex = 0;
	this->line = 0;
	this->judge = 0;
	this->inputtype = 0;
	return;
}

int CMP_PlayerByExscore(const void *p1, const void *p2) {
	
	RANKINGPLAYER* s1 = (RANKINGPLAYER*)p1;
	RANKINGPLAYER* s2 = (RANKINGPLAYER*)p2;

	return (s2->pg * 2 + s2->gr) - (s1->pg * 2 + s1->gr);
}

void RANKING::ExpandRankingBuffer(int add) {
	
	this->ranking = (RANKINGPLAYER*)realloc(this->ranking, (this->rankingMax + add) * sizeof(RANKINGPLAYER));
	assert(this->ranking != nullptr);

	for (int i = this->rankingMax; i < this->rankingMax + add; i++) {
		memset(&this->ranking[i], 0, sizeof(RANKINGPLAYER)); // FIXME: bad memset
	}

	this->rankingMax += add;
	return;
}

void RANKING::Init() {
	this->lastupdate.fillzero();
	this->clearPlayers[0] = 0;
	this->clearPlayers[1] = 0;
	this->clearPlayers[2] = 0;
	this->clearPlayers[3] = 0;
	this->clearPlayers[4] = 0;
	this->clearPlayers[5] = 0;
	this->rankingCursor = 0;
	this->totalPlaycount = 0;
	this->rankingCount = 0;
	this->myRanking = 0;
	this->showRanking = '\0';
	for (int i = 0; i < this->rankingMax; i++) {
		this->ranking[i].name.fillzero();
		this->ranking[i].id = 0;
		this->ranking[i].sp = 0;
		this->ranking[i].dp = 0;
		this->ranking[i].clear = 0;
		this->ranking[i].notes = 0;
		this->ranking[i].combo = 0;
		this->ranking[i].pg = 0;
		this->ranking[i].gr = 0;
		this->ranking[i].gd = 0;
		this->ranking[i].bd = 0;
		this->ranking[i].pr = 0;
		this->ranking[i].minbp = 0;
		this->ranking[i].option = 0;
		this->ranking[i].sussussuspected = 0;
		this->ranking[i].playcount = 0;
		this->ranking[i].ranking = 0;
		this->ranking[i].comment.fillzero();
	}

	return;
}

int RANKING::ParseXML(const char* path) {
	Init();
	ErrorLogFmtAdd("ランキングデータのパースを開始します。パス%s\n", path);

	TiXmlDocument *hXml = new TiXmlDocument(path);

	std::string s;
	{
		std::ifstream ifs{path};
		std::stringstream ss;
		ss << ifs.rdbuf();
		s = ss.str();
	}
	s = ansi2utf(s.c_str(), 932);
	// NOTE: TIXML_ENCODING_UTF8 or for whatever other reason it ignores `encoding='shift_jis'` in the header.
	hXml->Parse(s.c_str(), nullptr, TIXML_ENCODING_UTF8);

	TiXmlElement *cur;
	cur = hXml->FirstChildElement("lastupdate");
	if (cur && cur->ToElement()) {
		cstrSprintf(&this->lastupdate, "%s", cur->ToElement()->GetText());
	}

	cur = hXml->FirstChildElement("ranking");
	if (!cur) {
		if (hXml) delete(hXml);
		ErrorLogFmtAdd("ランキングの読み込みに失敗しました。xmlの書式をミスってるかも。\n");
		return 0;
	}

	cur = cur->FirstChildElement("score");
	if (!cur) {
		if (hXml) delete(hXml);
		ErrorLogFmtAdd("スコアの更新がありません。\n");
		return 0;
	}

	while (cur) {
		TiXmlElement *val;
		val = cur->FirstChildElement("name");
		if (val && val->ToElement()) {
			cstrSprintf(&ranking[rankingCount].name, "%s", val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("id");
		if (val) {
			ranking[rankingCount].id = atol(val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("sp");
		if (val) {
			ranking[rankingCount].sp = atol(val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("dp");
		if (val) {
			ranking[rankingCount].dp = atol(val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("clear");
		if (val) {
			ranking[rankingCount].clear = atol(val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("notes");
		if (val) {
			ranking[rankingCount].notes = atol(val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("combo");
		if (val) {
			ranking[rankingCount].combo = atol(val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("pg");
		if (val) {
			ranking[rankingCount].pg = atol(val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("gr");
		if (val) {
			ranking[rankingCount].gr = atol(val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("gd");
		if (val) {
			ranking[rankingCount].gd = atol(val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("bd");
		if (val) {
			ranking[rankingCount].bd = atol(val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("pr");
		if (val) {
			ranking[rankingCount].pr = atol(val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("minbp");
		if (val) {
			ranking[rankingCount].minbp = atol(val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("option");
		if (val) {
			ranking[rankingCount].option = atol(val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("comment");
		if (val && val->ToElement()) {
			cstrSprintf(&ranking[rankingCount].comment, "%s", val->ToElement()->GetText());
		}
		val = cur->FirstChildElement("playcount");
		if (val) {
			ranking[rankingCount].playcount = atol(val->ToElement()->GetText());
		}

		this->rankingCount++;
		if (this->rankingCount == this->rankingMax) ExpandRankingBuffer(1000);

		cur = cur->NextSiblingElement();
	}

	if (hXml) delete(hXml);
	ErrorLogFmtAdd("ランキングデータのパースに成功しました。 合計プレイヤー %d\n", rankingCount);
	
	qsort(ranking, rankingCount, sizeof(RANKINGPLAYER), CMP_PlayerByExscore);

	for (int i = 0; i < rankingCount; i++) {
		totalPlaycount += ranking[i].playcount;
		if (0 <= ranking[i].clear && ranking[i].clear <= 5) clearPlayers[ranking[i].clear]++;
	}

	int fpg = ranking[0].pg;
	int fgr = ranking[0].gr;
	int rank = 1;
	for (int i = 0; i < rankingCount; i++) {
		if (ranking[i].gr + ranking[i].pg * 2 < fgr + fpg * 2) rank = i + 1;
		ranking[i].ranking = rank;
		if (ranking[i].id == myID) myRanking = rank;
		if (ranking[i].id == rivalID) rivalRanking = rank;
	}
	return 1;
}

int CheckRivaldataNew(int rivalID) {

	sqlite3 *pDb;
	sqlite3_stmt *pStmt;
	CSTR path;
	int ret = 0;
	cstrSprintf(&path, fs::make_preferred("LR2files/Rival/%d.db").data(), rivalID);
	
	if (!IsFileExist(path)) return 0;

	sqlite3_open(path, &pDb);

	SQL_prepare("SELECT r_lastupdate FROM rival ORDER BY r_lastupdate DESC LIMIT 0,1", pDb, &pStmt);
	if (sqlite3_step(pStmt) == 100) {
		ret = sqlite3_column_int(pStmt, 0);
	}
	sqlite3_finalize(pStmt);
	sqlite3_close(pDb);
	return ret;
}

int ParseRivalData(long ID) {

	CSTR name;
	CSTR path;
	TiXmlDocument *hXml;
	TiXmlElement *cur, *val;
	sqlite3 *pRivalDB;

	cstrSprintf(&path, fs::make_preferred("LR2files/Rival/%d.db").data(), ID);
	sqlite3_open(path, &pRivalDB);
	SQL_Run("CREATE TABLE rival(hash TEXT primary key,r_clear INTEGER,r_totalnotes INTEGER,r_maxcombo INTEGER,r_perfect INTEGER,r_great INTEGER,r_good INTEGER,r_bad INTEGER,r_poor INTEGER,r_minbp INTEGER,r_option INTEGER,r_lastupdate INGEGER)", pRivalDB);
	
	cstrSprintf(&path, fs::make_preferred("LR2files/Rival/%d.xml").data(), ID);
	hXml = new TiXmlDocument(path);
	if (hXml->LoadFile(TIXML_ENCODING_UNKNOWN) == false) {
		if (hXml) {
			delete(hXml);
		}
		printfDx("ID%4d:ライバルデータにアクセスできません。\n", ID);
		return 0;
	}

	cur = hXml->FirstChildElement("rivalname");
	if(cur){
		if (cur->ToElement()) {
			cstrSprintf(&name, "%s", cur->ToElement()->GetText());
		}
		ErrorLogFmtAdd("ライバル名:%s\n", name.body);
	}

	cur = hXml->FirstChildElement("scorelist");
	if (!cur) {
		if (hXml) {
			delete(hXml);
		}
		ErrorLogFmtAdd("ライバルデータ読み込みに失敗しました。xmlが変かも\n");
		printfDx("ライバルデータの読み込みに失敗しました。ID%06d\n", ID);
		return 0;
	}

	cur = cur->FirstChildElement("score");
	if (!cur) {
		if (hXml) {
			delete(hXml);
		}
		ErrorLogFmtAdd("ライバルデータの読み込みに失敗しました。スコアが存在しないかも\n");
		printfDx("ID%06d:ライバルデータ[%s]の更新はありません。\n", ID, name.body);
		return 0;
	}

	CSTR hash, query;
	int count = 0;
	int clear, notes, combo, pg, gr, gd, bd, pr, minbp, option, lastupdate;
	SQL_Run("BEGIN", pRivalDB);

	while (cur) {
		count++;

		val = cur->FirstChildElement("hash");
		if(val && val->ToElement()){
			cstrSprintf(&hash, "%s", val->ToElement()->GetText());
		}

		val = cur->FirstChildElement("clear");
		if (val && val->ToElement()) {
			clear = atol(val->ToElement()->GetText());
		}

		val = cur->FirstChildElement("notes");
		if (val && val->ToElement()) {
			notes = atol(val->ToElement()->GetText());
		}

		val = cur->FirstChildElement("combo");
		if (val && val->ToElement()) {
			combo = atol(val->ToElement()->GetText());
		}

		val = cur->FirstChildElement("pg");
		if (val && val->ToElement()) {
			pg = atol(val->ToElement()->GetText());
		}

		val = cur->FirstChildElement("gr");
		if (val && val->ToElement()) {
			gr = atol(val->ToElement()->GetText());
		}

		val = cur->FirstChildElement("gd");
		if (val && val->ToElement()) {
			gd = atol(val->ToElement()->GetText());
		}

		val = cur->FirstChildElement("bd");
		if (val && val->ToElement()) {
			bd = atol(val->ToElement()->GetText());
		}

		val = cur->FirstChildElement("pr");
		if (val && val->ToElement()) {
			pr = atol(val->ToElement()->GetText());
		}

		val = cur->FirstChildElement("minbp");
		if (val && val->ToElement()) {
			minbp = atol(val->ToElement()->GetText());
		}

		val = cur->FirstChildElement("option");
		if (val && val->ToElement()) {
			option = atol(val->ToElement()->GetText());
		}

		val = cur->FirstChildElement("lastupdate");
		if (val && val->ToElement()) {
			lastupdate = atol(val->ToElement()->GetText());
		}

		cstrSprintf(&query, "INSERT INTO rival (hash,r_clear,r_totalnotes,r_maxcombo,r_perfect,r_great,r_good,r_bad,r_poor,r_minbp,r_option,r_lastupdate) VALUES(\'%s\',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
			hash.body, clear, notes, combo, pg, gr, gd, bd, pr, minbp, option, lastupdate);
		if (SQL_Run(query, pRivalDB)) {
			cstrSprintf(&query, "UPDATE rival SET r_clear=%d,r_totalnotes=%d,r_maxcombo=%d,r_perfect=%d,r_great=%d,r_good=%d,r_bad=%d,r_poor=%d,r_minbp=%d,r_option=%d,r_lastupdate=%d WHERE hash=\'%s\'",
				clear, notes, combo, pg, gr, gd, bd, pr, minbp, option, lastupdate, hash.body);
			SQL_Run(query, pRivalDB);
		}

		cur = cur->NextSiblingElement();
	}
	if (hXml) {
		delete(hXml);
	}
	remove(path);

	SQL_Run("COMMIT", pRivalDB);
	sqlite3_close(pRivalDB);

	CSTR cfolder;
	cstrSprintf(&cfolder, "#COMMAND __RIVAL__\n#MAXTRACKS %d\n#CATEGORY ライバルフォルダ\n#TITLE %s\n#INFORMATION_A %sのプレイした曲を表示します\n#INFORMAION_B\n", ID, name.body, name.body);
	cstrSprintf(&path, fs::make_preferred("LR2files/Rival/%d.lr2folder").data(), ID);
	cfolder.toFile(path);
	printfDx("ID%06d:ライバルデータ[%s]を更新しました。更新スコア数%d\n", ID, name.body, count);
	return 1;
}

int NETWORK::GetInsaneList() {

	TiXmlDocument *hXml;
	TiXmlElement *cur;
	sqlite3 *pSongDB;

	cstrSprintf(&this->param," ");
	this->target_URL = "http://www.dream-pro.info/~lavalse/LR2IR/2/getinsanelist.cgi";
	if (HTTPrequest() == 1) {
		this->httpResult.toFile(fs::make_preferred("LR2files/Database/exlevel.xml").data());
		printfDx("発狂難度リストをダウンロードしました。\n");
		ErrorLogFmtAdd("発狂難度リストをダウンロードしました。\n");
	}
	else{
		printfDx("発狂難度リストの更新に失敗しました。");
		ErrorLogFmtAdd("発狂難度リストの更新に失敗しました。\n");
	}
	ScreenFlip();

	hXml = new TiXmlDocument(fs::make_preferred("LR2files/Database/exlevel.xml").data());
	if (hXml->LoadFile(TIXML_ENCODING_UNKNOWN) == false) {
		if (hXml) {
			delete(hXml);
		}
		printfDx("発狂レベルリストにアクセスできません。\n");
		ErrorLogFmtAdd("発狂難度リストにアクセスできません。\n");
		return 0;
	}
	cur = hXml->FirstChildElement("list");
	if (!cur) {
		if (hXml) {
			delete(hXml);
		}
		printfDx("発狂レベルリストの読み込みに失敗しました。\n");
		return 0;
	}
	cur = cur->FirstChildElement("song");
	if (!cur) {
		if (hXml) {
			delete(hXml);
		}
		printfDx("発狂レベルリストの更新はありません。\n");
		return 0;
	}

	CSTR query;
	CSTR hash;
	sqlite3_open(fs::make_preferred("LR2files/Database/song.db").data(), &pSongDB);
	SQL_Run("BEGIN", pSongDB);
	while (cur) {
		if (TiXmlElement *val = cur->FirstChildElement("hash"); val && val->ToElement()) {
			cstrSprintf(&hash, "%s", val->ToElement()->GetText());
		}

		if (TiXmlElement *val = cur->FirstChildElement("exlevel"); val) {
			int exlevel = atol(val->ToElement()->GetText());
			cstrSprintf(&query, "UPDATE song SET exlevel=%d WHERE hash=\'%s\'",exlevel,hash.body);
			SQL_Run(query, pSongDB);
		}

		cur = cur->NextSiblingElement();
	}

	if (hXml) delete(hXml);
	SQL_Run("COMMIT", pSongDB);
	sqlite3_close(pSongDB);
	return 1;
}

CSTR UrlEncode(CSTR in) {

	CSTR ret;

	for (int i = 0; i < in.length(); i++) {
		CSTR buf;
		if (isalnum((uchar)*in.atPos(i)) || (*in.atPos(i) == '_') || (*in.atPos(i) == '.') || (*in.atPos(i) == '-')) {
			*buf.atPos(0) = *in.atPos(i);
			*buf.atPos(1) = '\0';

		}
		else if ((*in.atPos(i) == ' ')) {
			*buf.atPos(0) = '+';
			*buf.atPos(1) = '\0';
		}
		else {
			cstrSprintf(&buf, "%%%02X", (uchar)*in.atPos(i));
		}

		ret.add(buf);
	}
	return ret;
}

RANKING::RANKING() {
	// FIXME: replace with std::vector. Currently this leaks memory.
	ranking = (RANKINGPLAYER*)malloc(sizeof(RANKINGPLAYER) * 1000);
	assert(ranking != nullptr);
	rankingCount = 0;
	rankingMax = 1000;
	for (int i = 0; i < rankingMax; i++) memset(&ranking[i], 0, sizeof(RANKINGPLAYER));
	Init();
}

void NETWORK::ParseRankingXml(const char* path)
{
	std::unique_lock l{criticalSection};
	rankingData.ParseXML(path);
}

int NETWORK::HTTPrequest() {
#ifdef _WIN32
	SOCKET s;
	struct sockaddr_in server;
	hostent* host;
	CSTR request(2048);
	CSTR recvBuf(2048);

	{
		std::unique_lock l{criticalSection};

		s = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
		if (s == 0xffffffff) {
			cstrSprintf(&this->request_debug, "ソケットエラー : %d\n", WSAGetLastError());
			ErrorLogAdd(this->request_debug);
			closesocket(s);
			return 0;
		}

		server.sin_family = AF_INET;
		server.sin_port = htons(80);
		server.sin_addr.S_un.S_addr = inet_addr(this->domain);
		if (server.sin_addr.S_un.S_addr == 0xffffffff) {
			host = gethostbyname(this->domain);
			if (host == NULL) {
				if (WSAGetLastError() == 11001) cstrSprintf(&this->request_debug, "ホストが見つかりません : %d\n", WSAGetLastError());
				else cstrSprintf(&this->request_debug, "その他のエラーです : %d\n", WSAGetLastError());
				ErrorLogAdd(this->request_debug);
				closesocket(s);
				return 0;
			}

			for (int i = 0; host->h_addr_list[i]; i++) {
				memcpy(&server.sin_addr.S_un, host->h_addr_list[i], 4);
				if (connect(s, (sockaddr*)&server, sizeof(struct sockaddr_in)) == 0) {
					if (host->h_addr_list[i] == NULL) {
						cstrSprintf(&this->request_debug, "接続に失敗しました : %d\n", WSAGetLastError());
						ErrorLogAdd(this->request_debug);
						closesocket(s);
						return 0;
					}
					break;
				}
			}
		}
		else {
			if (connect(s, (sockaddr*)&server, sizeof(struct sockaddr_in)) == 0) {
				cstrSprintf(&this->request_debug, "接続に失敗しました : %d\n", WSAGetLastError());
				ErrorLogAdd(this->request_debug);
				closesocket(s);
				return 0;
			}
		}

		unsigned long argp = 1;
		ioctlsocket(s, 0x8004667e, &argp);

		request.fillzero();
		cstrSprintf(&request, "POST %s HTTP/1.0\r\nContent-Length:%d\n\n%s", this->target_URL.body, this->param.length(), this->param.body);

		if (send(s, request, request.length() + 1, 0) < 0) {
			cstrSprintf(&this->request_debug, "データの送信に失敗しました : %d\n", WSAGetLastError());
			ErrorLogAdd(this->request_debug);
			closesocket(s);
			return 0;
		}

		request.fillzero(); //from this time, request is used as response result

		int timeout = this->timeout / 10;
		int i = 0;
		for (; i != timeout; i++) {
			recvBuf = "";
			int recvSize = recv(s, recvBuf, recvBuf.msize() - 1, 0);
			if (this->waitForHandle) {
				request = "DISCONNECT";
				break;
			}

			if (recvSize < 0) {
				if (WSAGetLastError() != 10035) { //WSAEWOULDBLOCK
					cstrSprintf(&this->request_debug, "データの受信に失敗しました : %d\n", WSAGetLastError());
					if (this->rankingData.myID == 1) ErrorLogAdd(this->request_debug);
					request.fillzero();
					break;
				}
			}
			else if (recvSize == 0) {
				break;
			}
			else {
				request.add(recvBuf);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		if (i == timeout) {
			request = "TIMEOUT";
		}

		closesocket(s);
	}

	if (request.findStrPos("#") != -1) {
		this->httpResult = request.right(request.length() - (request.findStrPos("#") + 1));
		return 1;
	}

	if (request.findStrPos("503") != -1) {
		cstrSprintf(&this->request_debug, "503エラーです : %d\n", WSAGetLastError());
		ErrorLogAdd(this->request_debug);
		return 0;
	}
	if (request.findStrPos("404") != -1) {
		cstrSprintf(&this->request_debug, "404エラーです : %d\n", WSAGetLastError());
		ErrorLogAdd(this->request_debug);
		return 0;
	}
	if (this->waitForHandle) {
		cstrSprintf(&this->request_debug, "接続を中断しました : %d\n", WSAGetLastError());
		ErrorLogAdd(this->request_debug);
		return -1;
	}
	if (request.isSame("TIMEOUT")) {
		cstrSprintf(&this->request_debug, "タイムアウト : %d\n", WSAGetLastError());
		ErrorLogAdd(this->request_debug);
		return 0;
	}
	cstrSprintf(&this->request_debug, "その他のエラーです : %d\n", WSAGetLastError());
	ErrorLogAdd(this->request_debug);
	return 0;
#else
	return -1; // TODO(linux): stub
#endif // _WIN32
}

void NETWORK::WaitAndInitRanking() {
	GetTimeWrap();
	this->waitForHandle = true;
	if (hHandle.joinable()) {
		hHandle.join();
	}
	this->waitForHandle = false;
	this->rankingData.Init();
}

int NETWORK::GetRanking(CSTR hash, char flagInit) {

	CSTR path;
	
	ErrorLogAdd("IRxmlをダウンロードします\n");
	if (hash.length() <= 50) {
		cstrSprintf(&path, fs::make_preferred("LR2files/Ir/%s.xml").data(), hash.body);
	}
	else {
		cstrSprintf(&path, fs::make_preferred("LR2files/Ir/%s.xml").data(), AssignCRC32(hash).body);
	}

	if (flagInit) this->rankingData.Init();

	cstrSprintf(&this->param, "songmd5=%s&id=%d&lastupdate=%s", hash.body, this->IR_ID, this->rankingData.lastupdate.body);
	this->target_URL = "http://www.dream-pro.info/~lavalse/LR2IR/2/getrankingxml.cgi";
	if (HTTPrequest() == 1) {
		ErrorLogAdd("xmlを保存します\n");
		this->httpResult.toFile(path);
		rankingData.ParseXML(path);
		return 1;
	}
	else if (this->rankingData.lastupdate.length() <= 0) {
		rankingData.Init();
		return -1;
	}
	return 1;
}

int NETWORK::GetRivalInfo(int rivalID) {
	CSTR pathXML;
	cstrSprintf(&pathXML, fs::make_preferred("LR2files/Rival/%d.xml").data(), rivalID);
	CSTR pathDB;
	cstrSprintf(&pathDB, fs::make_preferred("LR2files/Rival/%d.db").data(), rivalID);
	cstrSprintf(&this->param, "id=%d&lastupdate=%d", rivalID, CheckRivaldataNew(rivalID));
	this->target_URL = "http://www.dream-pro.info/~lavalse/LR2IR/2/getplayerxml.cgi";
	if (this->HTTPrequest() == 1) {
		this->httpResult.toFile(pathXML);
		ParseRivalData(rivalID);
		ErrorLogFmtAdd("ライバルデータを更新しました。ID:%06d\n", rivalID);
	}
	else {
		printfDx("ライバルデータのダウンロードに失敗しました。ID:%06d\n", rivalID);
		ErrorLogFmtAdd("ライバルデータのダウンロードに失敗しました。ID:%06d\n", rivalID);
	}
	ScreenFlip();
	ProcessMessage();
	return 1;
}

int OpenWebRanking(CSTR songmd5){
#ifdef _WIN32
	CSTR url;
	cstrSprintf(&url, "\"http://www.dream-pro.info/~lavalse/LR2IR/search.cgi?mode=ranking&bmsmd5=%s#status&\"", songmd5.body);
	ShellExecuteA(NULL, "open", url, NULL, NULL, 1);
	return 1;
#else
	CSTR url;
	cstrSprintf(&url, "xdg-open \"http://www.dream-pro.info/~lavalse/LR2IR/search.cgi?mode=ranking&bmsmd5=%s#status&\" &", songmd5.body);
	return system(url.body) == 0;
#endif
}

static void ThreadProc_IRsendScore(NETWORK *ir) {
	ErrorLogAdd("LR2IRにスコアを送信します。\n");
	CSTR scorehash;
	cstrSprintf(&scorehash, "%s%s%d%d", ir->IR_passMD5.body, ir->myRanking.songMD5.body,ir->myRanking.exscore, ir->myRanking.clear);
	scorehash = MD5str(scorehash);

	cstrSprintf(&ir->param, "songmd5=%s&id=%d&passmd5=%s&title=%s&genre=%s&artist=%s&maxbpm=%d&minbpm=%d&&playlevel=%d&clear=%d&exscore=%d&pg=%d&gr=%d&gd=%d&bd=%d&pr=%d&maxcombo=%d&playcount=%d&clearcount=%d&rate=%d&minbp=%d&totalnotes=%d&opt_history=%d&opt_this=%d&line=%d&judge=%d&inputtype=%d&ghost=%s&rseed=%d&clear_db=%d&clear_ex=%d&clear_sd=%d&scorehash=%s"
		, ir->myRanking.songMD5.body, ir->IR_ID, ir->IR_passMD5.body, UrlEncode(ir->myRanking.title).body, UrlEncode(ir->myRanking.genre).body, UrlEncode(ir->myRanking.artist).body,
		ir->myRanking.maxbpm, ir->myRanking.minbpm, ir->myRanking.playlevel, ir->myRanking.clear, ir->myRanking.exscore, ir->myRanking.pg, ir->myRanking.gr, ir->myRanking.gd, ir->myRanking.bd, ir->myRanking.pr, ir->myRanking.maxcombo,
		ir->myRanking.playcount, ir->myRanking.clearcount, ir->myRanking.rate, ir->myRanking.minbp, ir->myRanking.totalnotes, ir->myRanking.opt_history, ir->myRanking.opt_this, ir->myRanking.line, ir->myRanking.judge,
		ir->myRanking.inputtype, ir->myRanking.ghost.body, ir->myRanking.rseed,	ir->myRanking.clear_db, ir->myRanking.clear_ex, ir->myRanking.clear_sd, UrlEncode(scorehash).body);

	ir->target_URL = "http://www.dream-pro.info/~lavalse/LR2IR/2/score.cgi";
	int httpResponse;
	if constexpr (true) { // DEBUG: do not send IR before test is done enough.
		httpResponse = 0;
	} else {
		httpResponse = ir->HTTPrequest();
	}
	if (httpResponse == 1) {
		ir->IRresultMessage = "スコアを送信しました";
		ir->GetRanking(ir->myRanking.songMD5, 1);
	}
	else if (httpResponse == 0) {
		ir->IRresultMessage = "サーバーとの接続に失敗しました";
	}
	ErrorLogAdd("送信終了\n");

	ir->hHandle.detach(); // Detach ourselves TODO: refactor surrounding code to avoid this
}

int NETWORK::GetTargetInfo(int mode, CSTR songmd5, CSTR *oData, CSTR *oName, int *oDigit1, int *oDigit2, int *oDigit3, int *oDigit4, int *oSeed, int *oExscore) {

	CSTR search("top");

	if (mode == 6) {
		search = "top";
	}
	else if (mode == 7) {
		search = "next";
	}
	else if (mode == 8) {
		search = "average";
	}

	cstrSprintf(&param, "songmd5=%s&mode=%s&playerid=%d&targetid=%d", songmd5.body, search.body, IR_ID, rankingData.target_ID);
	target_URL = "http://www.dream-pro.info/~lavalse/LR2IR/2/getghost.cgi";

	if (HTTPrequest() != 1) {
		ErrorLogAdd("ゴーストのダウンロードに失敗しました\n");
		return 0;
	}

	CSVbuf csv;
	SplitCSV(httpResult, &csv, ",");
	if (csv.str[1].length()) {
		if (mode == 8) {
			*oName = "AVERAGE";
			*oExscore = csv.val[1];
			ErrorLogFmtAdd("EXスコアを取得しました:%d\n", csv.val[1]);
		}
		else {
			*oName = csv.str[0];
			*oData = csv.str[3];
			*oDigit1 = GetDigitNum(csv.val[1], 1);
			*oDigit2 = GetDigitNum(csv.val[1], 2);
			*oDigit3 = GetDigitNum(csv.val[1], 3);
			*oDigit4 = GetDigitNum(csv.val[1], 4);
			*oSeed = csv.val[2];
			if (oName->isSame("NOPLAYER")) {
				ErrorLogAdd("ゴーストのダウンロードに失敗しました\n");
				return 0;
			}
			ErrorLogFmtAdd("ゴーストをダウンロードしました\nプレイヤー名%s\n", oName);
		}
		return 1;
	}
	return 0;
}

NETWORK::NETWORK() {
	this->rankingData.target_ID = 0;
	this->rankingData.Init();
}

int NETWORK::WS_clean() {
#ifdef _WIN32
	WSACleanup();
#endif // _WIN32
	return 1;
}

int NETWORK::Login(int isDirectPlay) {
#ifdef _WIN32
	if (WSAStartup(2, &this->wsa)) {
		this->request_debug = "WinSockの初期化に失敗しました\n";
		this->request_result = "WinSockの初期化に失敗しました。ネットワーク機能は使用できません・\n";
		this->isOnline = false;
		ErrorLogAdd(this->request_debug);
		WSACleanup();
		return -99;
	}
#else
	if (true) { // TODO(linux): stub
		this->request_debug = "linux\n";
		this->request_result = "happy with yourself?\n";
		this->isOnline = false;
		ErrorLogAdd(this->request_debug);
		return -99;
	}
#endif // _WIN32

	cstrSprintf(&this->param, "passmd5=%s&id=%d&name=%s&version=%d", this->IR_passMD5.body, this->IR_ID, this->IR_name.body, 100130); //version 100130
	this->target_URL = "http://www.dream-pro.info/~lavalse/LR2IR/2/login.cgi";
	if (this->HTTPrequest() != 1) {
		this->request_result = "サーバーとの接続に失敗しました。\n";
		this->isOnline = false;
		return -99;
	}

	if (this->httpResult.left(3).isSame("NEW")) {
		this->isOnline = true;
		this->rankingData.myID = atol(this->httpResult.right(this->httpResult.length() - 4));
		cstrSprintf(&this->request_result, "LR2IRに新規登録しました。\nLR2ID:%06d\n\n", this->rankingData.myID);
		this->IR_ID = this->rankingData.myID;
		return 1;
	}

	if (this->httpResult.left(2).isSame("OK") || this->httpResult.left(2).isSame("B1") || this->httpResult.left(2).isSame("B2") || this->httpResult.left(2).isSame("B3")){
		this->isOnline = true;
		this->rankingData.myID = atol(this->httpResult.right(this->httpResult.length() - 3));

		if (this->httpResult.left(2).isSame("B1")) {
			cstrSprintf(&this->request_result, "LR2IRにログインしました。\nLR2ID:%06d\nアクセス集中のため、ランキング自動更新の待ち時間は20秒となります。\n\n", this->rankingData.myID);
			this->rankUpdateDelayLevel = 1;
			this->waitTime = 20000;
		}
		else if (this->httpResult.left(2).isSame("B2")) {
			cstrSprintf(&this->request_result, "LR2IRにログインしました。\nLR2ID:%06d\nアクセス集中のため、ランキング自動更新の待ち時間は30秒となります。\n\n", this->rankingData.myID);
			this->rankUpdateDelayLevel = 2;
			this->waitTime = 30000;
		}
		else if (this->httpResult.left(2).isSame("B3")) {
			cstrSprintf(&this->request_result, "LR2IRにログインしました。\nLR2ID:%06d\nアクセス集中のため、ランキング自動更新は行われません。\n\n", this->rankingData.myID);
			this->rankUpdateDelayLevel = 3;
			this->waitTime = -1;
		}
		else {
			cstrSprintf(&this->request_result, "LR2IRにログインしました。\nLR2ID:%06d\n\n", this->rankingData.myID);
		}

		this->IR_ID = this->rankingData.myID;

		if (isDirectPlay == 0) {
			CSVbuf csv;
			SplitCSV(this->httpResult, &csv, ",");
			for (int i = 0; i < 20; i++) this->rivals[i] = 0;
			this->rivalcount = 0;

			if (this->getrival == 1) {
				for(int cur= 0 ; cur < 20; cur++) {
					if (GetMouseInput()) {
						printfDx("ライバルデータの取得をスキップしました。\n");
						break;
					}
					if (cur == 20 || csv.val[2 + cur] < 1) break;
					if (cur == 0) printfDx("ライバルデータの取得は左クリック押しっぱなしでスキップできます。\n");

					ErrorLogFmtAdd("ライバル登録されています:%d\n", this->rivals[cur]); //TOFIX: 
					this->rivals[cur] = csv.val[2+cur];
					this->rivalcount++;
					this->GetRivalInfo(this->rivals[cur]);
					if (this->getrival != 1) break;
				}
			}
		}
		return 1;
	}

	else if (this->httpResult.left(4).isSame("MAIL")) {
		this->request_result = "無効なメールアドレスです。\n";
		this->isOnline = false;
		return -2;
	}
	else if (this->httpResult.left(2).isSame("DB")) {
		this->request_result = "データベースに接続できません。\n";
		this->isOnline = false;
		return -99;
	}
	else if (this->httpResult.left(7).isSame("VERSION")) {
		this->request_result = "最新版に更新して下さい。\n";
		this->isOnline = false;
		return -99;
	}
	else if (this->httpResult.left(3).isSame("BAN")) {
		this->request_result = "あなたのアカウントは凍結されました。\n";
		this->isOnline = false;
		return -99;
	}
	else if (this->httpResult.left(5).isSame("SORRY")) {
		this->request_result = "現在サーバーメンテナンス中です。\n";
		this->isOnline = false;
		return -99;
	}
	else if (this->httpResult.left(3).isSame("END")) {
		this->request_result = "LR2IRは終了しました。\n";
		this->isOnline = false;
		return -99;
	}
	cstrSprintf(&this->request_result, "パスワードが違うか、その他のエラーです。\nLR2ID:%d\nLR2IDがいつの間にか変わってログイン出来ない！という方は#lunaticraveまでどうぞ。個別に対応します。", this->IR_ID);
	this->isOnline = false;
	return -3;
}

int NETWORK::MakeIRsendScoreThread() {
	GetTimeWrap();
	this->waitForHandle = true;
	if (hHandle.joinable()) {
		hHandle.join();
	}
	this->waitForHandle = false;
	this->rankingData.Init();
	this->IRstatus = 1;
	this->hHandle = std::jthread(ThreadProc_IRsendScore, this);
	return 0;
}


int SaveIRID(int IRID, CSTR ID) {

	CSTR scorefile;
	sqlite3 *pDb;
	cstrSprintf(&scorefile, fs::make_preferred("LR2files/Database/Score/%s.db").data(), ID.body);
	sqlite3_open(scorefile, &pDb);
	CSTR query;
	cstrSprintf(&query, "UPDATE player SET irid = %d", IRID);
	SQL_Run(query, pDb);
	sqlite3_close(pDb);
	ErrorLogAdd("IRIDをセーブしました\n");
	return 1;
}
