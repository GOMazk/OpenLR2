#include "structure.h"
#include "Engine.h"
#include "LR2.h"
#include "Scenes.h"
#include "filesystem.h"
#include "LR2_customir.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <thread>

#include <DxLib/DxLib.h>
extern "C" {
#include <sqlite/sqlite3.h>
}

#define LR2TITLE "OpenLR2 version "
#define LR2VERSIONSTRING "OpenLR2 version "

#ifdef _WIN32

#include <windows.h>

int main(int, char**);
int WINAPI WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/)
{
	return main(__argc, __argv);
}

static std::filesystem::path GetExecutablePath()
{
	wchar_t fullpath[MAX_PATH]{};
	if (!GetModuleFileNameW(nullptr, fullpath, std::size(fullpath)))
		return {};
	return std::filesystem::path(fullpath).parent_path();
}

#else

#include <iostream>

static void MessageBoxA(const char*, const char* title, const char* desc, const char*)
{
	std::cout << "\n" << title << "\n\n" << desc << "\n" << std::flush;
}

static std::filesystem::path GetExecutablePath()
{
	char fullpath[256]{};

	char process_path[] = "/proc/self/exe";
	const auto bytes =
		std::min(readlink(process_path, fullpath, sizeof(fullpath)), static_cast<ssize_t>(sizeof(fullpath) - 1));
	if (bytes >= 0)
		fullpath[bytes] = '\0';

	return std::filesystem::path(fullpath).parent_path();
}

int DxLib::SetMouseDispFlag(int) { return {}; }

#endif // _WIN32

static consteval bool is_linux()
{
#ifdef _WIN32
	return false;
#else
	return true;
#endif
}

static bool run_tests() {
	if (CSTR fp = "C:\\a\\b\\c\\d.bms"; fp.getDirectory().body != std::string_view{"C:\\a\\b\\c\\"}) {
		ErrorLogFmtAdd("1: %s\n", fp.getDirectory().body);
		return false;
	}
	if (CSTR fp = "C:\\a\\b\\c\\d.bms"; fp.getParentDirectory().body != std::string_view{"C:\\a\\b\\"}) {
		ErrorLogFmtAdd("2: %s\n", fp.getParentDirectory().body);
		return false;
	}
	return true;
}

int main(int argc, char** argv) {
#ifdef _DEBUG
	while (!IsDebuggerPresent()) std::this_thread::sleep_for(std::chrono::milliseconds(200));
#endif

	sqlite3* sql3;

	int wSizeY;
	int wSizeX;
	game gs;

	gs.config.system.coreCount = std::thread::hardware_concurrency();
	if (gs.config.system.coreCount == 0) gs.config.system.coreCount = 2;

	int tmp;

	SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
	SetFontCharCodeFormat(DX_CHARCODEFORMAT_UTF8);

	{
		auto curDir = GetExecutablePath();
		std::filesystem::current_path(curDir);
		gs.baseDirectory.assign(curDir.string().c_str(), 0).add("/");
	}

	gs.is_starter = false;
	auto copy_if_not_exists = [](auto&& from, auto&& to_) {
		std::filesystem::path to = to_;
		std::error_code ec; // ignore errors
		if (!std::filesystem::exists(to, ec))
			std::filesystem::copy(from, to, ec);
	};
	copy_if_not_exists("LR2files/Config/keyconfig_def.xml", "LR2files/Config/keyconfig.xml");
	copy_if_not_exists("LR2files/Config/keyconfig_5_def.xml", "LR2files/Config/keyconfig_5.xml");
	copy_if_not_exists("LR2files/Config/keyconfig_p_def.xml", "LR2files/Config/keyconfig_p.xml");
	copy_if_not_exists("LR2files/Config/midi_def.xml", "LR2files/Config/midi.xml");
	ErrorLogAdd("コンフィグを読み込みます…");

	if (!ReadConfig(&gs, fs::make_preferred("LR2files/Config/config.xml").data()) && gs.is_starter == false) {
		MessageBoxA(NULL, "Failed to read main config", "エラー", NULL);
		return -1;
	}
	if (!ReadOpenLr2Config(&gs, fs::make_preferred("LR2files/Config/openlr2-config.xml").data()) && gs.is_starter == false) {
		MessageBoxA(NULL, "Failed to read OpenLR2 config", "エラー", NULL);
		return -1;
	}
	if (gs.config.jukebox.numOfPath <= 0 && gs.is_starter == false) {
		MessageBoxA(NULL, "設定プログラムのJUKEBOX1タブで、\n曲を検索するフォルダの登録を行ってください。", "エラー", NULL);
		return -1;
	}
	ErrorLogAdd("成功しました\n");
	if (gs.is_starter) {
		gs.config.sound.disablefmod = 1;
		gs.config.skin.fontname.assign("HG丸ｺﾞｼｯｸM-PRO");
		gs.config.skin.disableimagefont = 1;
		gs.config.system.isablebmsthread = 1;
		gs.config.play.gaugeOption[0] = 0;
		gs.config.play.random[0] = 0;
		gs.config.play.hsfix = 4;
		gs.config.player.passMD5.assign("STARTERMODE");
		gs.config.player.id.assign("STARTERMODE");
		gs.config.jukebox.newsongfolder.assign("./");
		gs.config.jukebox.titleflash = 0;
		gs.config.select.sort = 1;
		gs.config.select.key = 1;
		gs.config.jukebox.numOfPath = 1;
		gs.config.jukebox.path[0].assign("BeatVocaloids/");
	}
	int lr1ir = gs.config.network.lr1ir;
	int lr2ir = gs.config.network.lr2ir;
	ReadMIDI(&gs, fs::make_preferred("LR2files/Config/midi.xml").data());
	if (gs.config.system.softwarerendering == 1) {
		SetUse3DFlag(0);
	}
	gs.directoryPath.fillzero();
	gs.cmd_directplay = false;
	gs.cmd_auto = '\0';
	gs.cmd_nosave = '\0';
	gs.is_recordmode = '\0';
	gs.auto2avi = '\0';
	gs.directoryFilename.fillzero();
	gs.audio.cmd_mediaOut = false;
	gs.rec.recMode = 0;
	gs.audio.replay2avi = false;
	gs.skstruct.drBuf.isDisabled = '\0';
	bool test_mode = false;
	bool use_dx9 = false;
	//commandline
	for (int i = 1; i < argc; i++) {
		CSTR tStr1;
		tStr1.assign(argv[i]);
		CSTR tStr2(tStr1);
		tStr2.lower();
		if (IsBmsFile(tStr1)) {
			gs.cmd_directplay = true;
			gs.directoryPath.assign(&tStr1);
		}
		else if (IsMediaFile(tStr1)) {
			gs.directoryFilename.assign(&tStr1);
			gs.config.system.vsync = 1;
			gs.config.system.screenmode = 1;
			gs.config.system.screenexrate = 100;
			gs.config.play.autojudge = 0;
			if (IsSndFile(tStr1)) {
				gs.cmd_nosave = '\x01';
				gs.auto2avi = '\x01';
				gs.cmd_auto = '\x01';
				gs.config.system.isablebmsthread = 1;
				gs.audio.cmd_mediaOut = true;
			}
			else if (IsAviFile(tStr1)) {
				gs.cmd_nosave = '\x01';
				gs.cmd_auto = '\x01';
				gs.config.system.isablebmsthread = 1;
				gs.gameplay.isPreviewLoad = '\0';
				gs.config.play.bga = 1;
				gs.audio.cmd_mediaOut = true;
				gs.config.system.vsync = 1;
			}
		}
		else if (tStr2.starts_with("-auto2avi")) {
			gs.rec.recMode = 1;
			gs.is_recordmode = '\x01';
			gs.config.select.preview = 0;
		}
		else if (tStr2.starts_with("-replay2avi")) {
			gs.rec.recMode = 2;
			gs.audio.replay2avi = true;
			gs.is_recordmode = '\x01';
			gs.config.select.preview = 0;
		}
		else if (tStr2.starts_with("-bga2avi")) {
			gs.rec.recMode = 3;
			gs.skstruct.drBuf.isDisabled = '\x01';
			gs.is_recordmode = '\x01';
			gs.config.select.preview = 0;
		}
		else if (tStr2.starts_with("-movie")) {
			gs.rec.recMode = 4;
			gs.config.select.preview = 0;
		}
		else if (tStr2.starts_with("-ns")) {
			gs.cmd_nosave = '\x01';
		}
		else if (tStr2.starts_with("-a")) {
			gs.cmd_auto = '\x01';
		}
		else if (tStr2.starts_with("-n")) {
			gs.cmd_n = atol(tStr1.right(tStr1.length() - 2)); //TOFIX : never used
		}
		else if (tStr2.starts_with("-dx9")) {
			use_dx9 = true;
		}
		else if (tStr2.starts_with("-test")) {
			test_mode = true;
		}
	}
	gs.config.system.thread = 0;
	if (test_mode) {
		if(!run_tests()) {
			ErrorLogAdd("tests failed\n");
			return 1;
		}
		ErrorLogAdd("tests passed\n");
		return 0;
	}
	CSTR pathScoreDB;
	cstrSprintf(&pathScoreDB, "LR2files/Database/Score/%s.db", gs.config.player.id.body);
	if (!gs.is_starter) {
		if (!IsFileExist(pathScoreDB)) {
			MessageBoxA(NULL, "スコアデータベースが見つかりません。\nconfig.exeで作成して下さい。", "エラー", 0);
			return -1;
		}
		if (ReadPlayerScore(gs.config.player.id, gs.config.player.pass, &gs.gameplay.playerstat) == 0) {
			return -1;
		}
	}
	{
		// make beta3 score backup
		CSTR pathScoreDBBackUp;
		cstrSprintf(&pathScoreDBBackUp, "LR2files/Database/Score/%s.db_backup", gs.config.player.id.body);
		copy_if_not_exists(pathScoreDB.body, pathScoreDBBackUp.body);
	}

	gs.sSelect.playerPassMD5.assign(&gs.config.player.passMD5);
	gs.sSelect.playerID.assign(&gs.config.player.id);
	gs.sSelect.newsongfolder.assign(&gs.config.jukebox.newsongfolder);
	gs.sSelect.titleflash = gs.config.jukebox.titleflash;
	gs.config.select.titleflash = gs.config.jukebox.titleflash;
	if (gs.config.play.bga == 3) gs.config.play.bga = 1;
	if (gs.config.select.disabledifficultyfilter == 1) gs.config.select.ignoredifficultyall = 0;
	memcpy(&gs.sSelect.filter, &gs.config.select, sizeof(CONFIG_SELECT));
	{
		CSTR newPath;
		std::error_code ec; // ignore errors
		cstrSprintf(&newPath, "LR2files/Replay/%s", gs.config.player.id.body);
		std::filesystem::create_directories(newPath.body, ec);
		cstrSprintf(&newPath, "LR2files/Ghost/%s", gs.config.player.id.body);
		std::filesystem::create_directories(newPath.body, ec);
		std::filesystem::create_directories("LR2files/SkinCustomize", ec);
		std::filesystem::create_directories("screenshot", ec);
	}
	ReadKeyConfig(&gs, fs::make_preferred("LR2files/Config/keyconfig.xml").data());
	gs.is_clicked_screenModeChange = 0;
	gs.flag_Screenshot = false;
	ReadOptionstrFile(gs.txtStruct.option_str, fs::make_preferred("LR2files/Config/optionstr.csv").data());
	gs.audio.is_fmod_disabled = gs.config.sound.disablefmod;
	if (gs.config.sound.disablefmod != 0) {
		gs.config.select.preview = 0;
	}
	SetHPtimerFlag(gs.config.system.hptimer == 1);
	SetManualTimerFlag(&gs.timer1, 0);
	gs.timer1.movieFramerate = (double)gs.config.tools.movie_framerate;
	gs.timer1.movieTimer = 0.0;

	SetGraphMode(640, 480, (gs.config.system.highcolor == 0) ? 32 : 16, 60);
	if (gs.rec.recMode == 3) {
		SetGraphMode(256, 256, 32, 60);
	}
#ifdef _WIN32
	SetWindowSizeChangeEnableFlag(1, 1);
	if (gs.audio.is_fmod_disabled == 0) {
		SetNotSoundFlag(1);
	}

	if (gs.is_starter) {
		if (MessageBoxA(NULL, "フルスクリーンモードで起動しますか？", "確認", 4) == 6) {
			gs.config.system.screenmode = 0;
			ChangeWindowMode(0);
		}
		else {
			gs.config.system.screenmode = 1;
			ChangeWindowMode(1);
		}
	}
	else {
		if ((gs.is_recordmode == '\0') && (gs.rec.recMode == 0)) {
			SetWindowSizeExtendRate((double)gs.config.system.windowsize_x / 640.0, (double)gs.config.system.windowsize_y / 480.0); //TODO_RESOULUTION
			if (gs.is_starter) { //unreachable duplicated code
				if (MessageBoxA(NULL, "フルスクリーンモードで起動しますか？", "確認", 4) == 6) {
					gs.config.system.screenmode = 0;
					ChangeWindowMode(0);
				}
				else {
					gs.config.system.screenmode = 1;
					ChangeWindowMode(1);
				}
			}
		}
		SetWaitVSyncFlag(0); //VSYNC
		ChangeWindowMode(1);
		SetWaitVSyncFlag(0); //VSYNC
	}

	if ((gs.config.system.maindisplay < 1) || (GetDirectDrawDeviceNum() <= gs.config.system.maindisplay)) {
		gs.config.system.maindisplay = 0;
	}
	else {
		SetUseDirectDrawDeviceIndex(gs.config.system.maindisplay);
	}
#endif // _WIN32

	// DxLib-for-Linux can only set title of an already existing window
	if constexpr (!is_linux()) { SetMainWindowText(LR2TITLE LR2BUILDDATE); }
	// DxLib-for-Linux only writes to stderr when writing to the log file.
	SetOutApplicationLogValidFlag(gs.config.system.outputlog || is_linux());
#ifdef _WIN32
	SetMultiThreadFlag(1);
#endif // _WIN32
	if ((gs.is_recordmode == '\0') && (gs.rec.recMode == 0)) {
		SetWaitVSyncFlag(0); //VSYNC
	}
	else {
		SetWaitVSyncFlag(1); //VSYNC
		ErrorLogFmtAdd("動画作成モードなのでVSyncを待ちます。\n");
	}
#ifdef _WIN32
	SetMultiThreadFlag(1);
	SetUseFPUPreserveFlag(1);
	SetUseDirectInputFlag(1); //DXLIBVER: not in original, but we need it to make same reaction.
	// NOTE(chown2): with DX9, in a virtual machine crashes on DxLib_Init
	if (use_dx9) {
		SetUseDirect3DVersion(DX_DIRECT3D_9); //DXLIBVER: if not set, it's DX11 (over 3.13e)
	}
	SetUseDisplayIndex(-1);
#endif // _WIN32
	if (DxLib_Init() == -1) return 0;
	if constexpr (is_linux()) { SetMainWindowText(LR2TITLE LR2BUILDDATE); }
	ChangeFont("", 0);
	SetLogFontSize(14); //DXLIBVER: change this for further dxlib version
#ifdef _WIN32
	SetSysCommandOffFlag(gs.config.system.disablesystemkey, 0);
#endif // _WIN32
	SetDrawScreen(DX_SCREEN_BACK);
	SetAlwaysRunFlag(1);
	SetMouseDispFlag(0);
	InitInputStructure(&gs.KeyInput);
	SetFirstSkins(&gs);
	clsDx();
	if (gs.config.network.lr2ir == 1 && gs.is_starter == 0 && gs.cmd_nosave == 0) {
		gs.net.IR_pass = gs.config.player.pass;
		gs.net.IR_name = gs.config.player.id;
		gs.net.IR_passMD5 = MD5str(gs.config.player.pass);
		gs.net.getrival = gs.config.network.getrival;
		gs.net.IR_ID = gs.gameplay.playerstat.irid;
		if (gs.net.Login(gs.cmd_directplay) == 1) {
			SaveIRID(gs.net.rankingData.myID, gs.config.player.id);
		}
		else {
			gs.config.network.lr2ir = 0;
		}
		printfDx(gs.net.request_result);
		ErrorLogAdd(gs.net.request_result);
	}
	{
		std::error_code ec; // ignore errors
		std::filesystem::path path("LR2files/CustomIRs");
		std::filesystem::create_directories(path, ec);
		gs.net.customIR.Initialize(path);
	}
	gs.net.customIR.Login();

	int loadingGrHandle = LoadGraph(fs::make_preferred("LR2files/Config/loading.bmp").data(), 0);

	int backgroundGrHandle = MakeScreen(640, 480, 0); //TODO_RESOULUTION
	SetDrawScreen(backgroundGrHandle);
	DrawBox(0, 0, 640, 480, GetColor(0, 0, 0), 1); //TODO_RESOULUTION
	SetDrawScreen(DX_SCREEN_BACK);

	memcpy(gs.config.jukebox.rival, gs.net.rivals, 4 * 20);
	sqlite3_open(gs.is_starter
			? fs::make_preferred("LR2files/Database.db" ).data()
			: fs::make_preferred("LR2files/Database/song.db").data(), &sql3);
	LoadLR2CustomFolder(sql3, &gs.config.jukebox, pathScoreDB, gs.is_starter, gs.cmd_directplay);
	if (gs.cmd_directplay == false) {
		if (gs.config.network.lr2ir == 1 && (((unsigned char)gs.config.jukebox.customfolder & 0x80) != 0)) {
			gs.net.GetInsaneList();
		}
		if (gs.is_starter == false) {
			printfDx(LR2VERSIONSTRING LR2BUILDDATE"\n");
			printfDx("PUSH ANY KEY\n");
			if (loadingGrHandle > 0) {
				DrawGraph(0, 0, loadingGrHandle, 0);
			}
			ScreenFlip();
			if (WaitInput(&gs.KeyInput) == -1) return 0;
			printfDx("READY");
		}
		if (loadingGrHandle > 0) {
			DrawGraph(0, 0, loadingGrHandle, 0);
		}
		ScreenFlip();
	}

	//mainphase
	if ((gs.is_recordmode == '\0') && (gs.auto2avi == '\0')) {
		SetWaitVSyncFlag(0); //VSYNC
#ifdef _WIN32
		ChangeWindowMode(gs.config.system.screenmode);
#endif // _WIN32
		SetWaitVSyncFlag(0); //VSYNC
		SetDrawScreen(DX_SCREEN_BACK);
	}
	gs.procSelecter = 2;
	gs.procPhase = 0;
	gs.isSkipDrawTick = 0;
	gs.gameplay.flag_threadExist = 0;
	gs.gameplay.flag_gameinput = false;
	InitBmsList(&gs.sSelect);
	gs.sSelect.maniac_cursor = 0;
	gs.sSelect.flag_maniacPanel = '\0';
	if (gs.cmd_directplay && !gs.is_starter) { //logic arranged
		gs.sSelect.cur = 0;
		cstrSprintf(&gs.sSelect.stack_query[gs.sSelect.cur], "SELECT * FROM folder WHERE parent = \'%s\'", AssignCRC32("ROOT").body);
		gs.sSelect.stack_isFolder[gs.sSelect.cur] = 1;
		gs.sSelect.stack_rivalID[gs.sSelect.cur] = 0;
		gs.sSelect.stack_searchTitle[gs.sSelect.cur] = "検索語句を入力";
		gs.sSelect.directory = gs.directoryPath.getDirectory();
		gs.sSelect.bmsListCount = 1;

		tmp = GetSongDataFromPath(gs.directoryPath, gs.sSelect.bmsList, sql3, &gs.sSelect);
		if (tmp == -1) return -1;
		if (tmp == 2) gs.cmd_nosave = 1;
	}
	else {
		if (gs.is_starter) {
			ErrorLogFmtAdd("スターターモードなので最初のジュークボックスのみ使用します。\n");
			gs.sSelect.cur = 0;
			cstrSprintf(&gs.sSelect.stack_query[gs.sSelect.cur], "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE parent = \'%s\'", AssignCRC32(gs.config.jukebox.path[0]).body);
			gs.sSelect.stack_isFolder[gs.sSelect.cur] = 0;
			gs.sSelect.stack_rivalID[gs.sSelect.cur] = 0;
			gs.sSelect.stack_searchTitle[gs.sSelect.cur] = "検索語句を入力";
			gs.sSelect.directory = gs.config.jukebox.path;
		}
		else {
			gs.sSelect.cur = 0;
			cstrSprintf(&gs.sSelect.stack_query[gs.sSelect.cur], "SELECT * FROM folder WHERE parent = \'%s\'", AssignCRC32("ROOT").body);
			gs.sSelect.stack_isFolder[gs.sSelect.cur] = 1;
			gs.sSelect.stack_rivalID[gs.sSelect.cur] = 0;
			gs.sSelect.stack_searchTitle[gs.sSelect.cur] = "検索語句を入力";
			gs.sSelect.directory = "ROOT";
		}
		LoadBmsListFromDB(gs.sSelect.stack_query[gs.sSelect.cur],sql3, &gs.sSelect, &gs.config.select.difficulty, &gs.config.select.key, 0, 0);
		SwapBmsList(&gs.sSelect);
	}

	if (gs.rec.recMode == 4) {
		gs.sSelect.stack_searchTitle[0].assign("オートプレイかリプレイを録画します");
	}
	for (int i = 0; i < 200; i++) gs.skstruct.caption[i].fillzero();
	for (int i = 0; i < 10; i++) gs.skstruct.helpfilePath[i].fillzero();
	for (int i = 0; i < 20; i++) gs.skstruct.customfileRANDOM[i].fillzero();
	for (int i = 0; i < 20; i++) gs.skstruct.customfile[i].fillzero();
	gs.skstruct.skinMD5.fillzero();
	gs.skstruct.skFontname.fillzero();
	for (int i = 0; i < 200; i++) gs.skstruct.caption[i].fillzero();
	for (int i = 0; i < 200; i++) gs.skstruct.caption[i].assign("(null)");
	for (int i = 0; i < 200; i++) gs.skstruct.GrHandle[i] = -1;
	for (int i = 0; i < 10; i++) gs.skstruct.helpfilePath[i].fillzero();
	for (int i = 0; i < 10; i++) gs.skstruct.helpfilePath[i].assign("(null)");
	gs.skstruct.skFontname.assign(&gs.config.skin.fontname);
	gs.skstruct.disableimagefont = (gs.config.skin.disableimagefont != 0);
	gs.skstruct.skinMD5.fillzero();
	gs.skstruct.skinMD5.resize2(34);
	if(AllocDrawingBuffer(&gs.skstruct.drBuf) == -1){
		DxLib_End();
		MessageBoxA(NULL, "スキン描画用のメモリ取得に失敗しました。", "エラー", 0);
		return -1;
	}
	gs.skstruct.drBuf.flagImageFont = (gs.config.skin.disableimagefont != 0);
	gs.gameplay.bmsobj.notes = NULL;
	gs.gameplay.bmsobj.count = 0;
	gs.gameplay.bmsobj.size = 0;
	gs.gameplay.bmsobj.note_count = 0;
	gs.gameplay.bmsobj.draw_count = 0;
	gs.gameplay.bmsobj.noteVal = 0;
	gs.gameplay.bmsobj.autoplay = 0;
	gs.gameplay.player[0].flag_active = 1;
	gs.gameplay.player[1].flag_active = 0;
	memset(gs.gameplay.bmsobj_note, '\0', sizeof(LaneStruct)*20);
	gs.gameplay.bmsobj_line.notes = NULL;
	gs.gameplay.bmsobj_line.count = 0;
	gs.gameplay.bmsobj_line.size = 0;
	gs.gameplay.bmsobj_line.note_count = 0;
	gs.gameplay.bmsobj_line.draw_count = 0;
	gs.gameplay.bmsobj_line.noteVal = 0;
	gs.gameplay.bmsobj_line.autoplay = 0;
	gs.gameplay.bpmt_buffersize = 0;
	gs.gameplay.isCourse = 0;
	gs.gameplay.isPreviewLoad = 0;
	gs.gameplay.previewStatus = 0;
	gs.gameplay.courseType = -1;
	gs.gameplay.courseStageNow = 0;
	gs.gameplay.timetick = GetTimeWrap();
	gs.gameplay.flag_threadDoingProcGame = 0;
	InitSkin(&gs.skstruct, 0, 0);
	gs.skstruct.fontname.assign(&gs.config.skin.fontname);
	for (int i = 0; i < SLOTS; i++) gs.gameplay.keysound->load = 0;
	for (int i = 0; i < 200; i++) gs.skstruct2.caption[i].fillzero();
	for (int i = 0; i < 10; i++) gs.skstruct2.helpfilePath[i].fillzero();
	for (int i = 0; i < 20; i++) gs.skstruct2.customfileRANDOM[i].fillzero();
	for (int i = 0; i < 20; i++) gs.skstruct2.customfile[i].fillzero();
	gs.skstruct2.skinMD5.fillzero();
	gs.skstruct2.skFontname.fillzero();
	for (int i = 0; i < 200; i++) gs.skstruct2.caption[i].fillzero();
	for (int i = 0; i < 200; i++) gs.skstruct2.caption[i].assign("(null)");
	for (int i = 0; i < 200; i++) gs.skstruct2.GrHandle[i] = -1;
	for (int i = 0; i < 10; i++) gs.skstruct2.helpfilePath[i].fillzero();
	for (int i = 0; i < 10; i++) gs.skstruct2.helpfilePath[i].assign("(null)");
	gs.skstruct2.skFontname.assign(&gs.config.skin.fontname);
	gs.skstruct2.disableimagefont = (gs.config.skin.disableimagefont != 0);
	gs.skstruct2.skinMD5.fillzero();
	gs.skstruct2.skinMD5.resize2(34);
	if (AllocDrawingBuffer(&gs.skstruct2.drBuf) == -1) {
		DxLib_End();
		MessageBoxA(NULL, "スキン描画用のメモリ取得に失敗しました。", "エラー", 0);
		return -1;
	}
	gs.skstruct2.drBuf.flagImageFont = gs.config.skin.disableimagefont != 0;
	InitSkin(&gs.skstruct2, 0, 0);
	gs.skstruct2.fontname.assign(&gs.config.skin.fontname);

	gs.sSelect.toRoot = 1;
	gs.sSelect.is_buttonIRpage = 0;
	gs.sSelect.is_clicked_tagedit = 0;
	gs.sSelect.is_tag_edited = 0;
	gs.sSelect.panel_unk = -1;
	gs.sSelect.panel = -1;
	InitObjectString(&gs.txtStruct);
	SetTarget(&gs);
	SetObjectStrings_SongSelect(&gs);
	gs.audio.param.fx_volume_on = gs.config.sound.volumeflag;
	gs.audio.param.volume_BGM = gs.config.sound.volumebgm;
	gs.audio.param.volume_key = gs.config.sound.volumekey;
	gs.audio.param.volume_master = gs.config.sound.volumemaster;
	gs.audio.param.eq_on = gs.config.sound.eqflag;
	gs.audio.param.eq_gain[0] = gs.config.sound.eqp0;
	gs.audio.param.eq_gain[1] = gs.config.sound.eqp1;
	gs.audio.param.eq_gain[2] = gs.config.sound.eqp2;
	gs.audio.param.eq_gain[3] = gs.config.sound.eqp3;
	gs.audio.param.eq_gain[4] = gs.config.sound.eqp4;
	gs.audio.param.eq_gain[5] = gs.config.sound.eqp5;
	gs.audio.param.eq_gain[6] = gs.config.sound.eqp6;
	gs.audio.param.pitch_amount = gs.config.sound.pitchp;
	gs.audio.param.pitch_on = gs.config.sound.pitchflag;
	gs.audio.param.pitch_type = gs.config.sound.pitchtype;
	gs.audio.param.fx_on[0] = gs.config.sound.fxflag_0;
	gs.audio.param.fxType[0] = gs.config.sound.fxtype_0;
	gs.audio.param.fxChannel[0] = gs.config.sound.fxtarget_0;
	gs.audio.param.fxParam[0][0] = gs.config.sound.fxp1_0;
	gs.audio.param.fxParam[0][1] = gs.config.sound.fxp2_0;
	gs.audio.param.fx_on[1] = gs.config.sound.fxflag_1;
	gs.audio.param.fxType[1] = gs.config.sound.fxtype_1;
	gs.audio.param.fxChannel[1] = gs.config.sound.fxtarget_1;
	gs.audio.param.fxParam[1][0] = gs.config.sound.fxp1_1;
	gs.audio.param.fxParam[1][1] = gs.config.sound.fxp2_1;
	gs.audio.param.fx_on[2] = gs.config.sound.fxflag_2;
	gs.audio.param.fxType[2] = gs.config.sound.fxtype_2;
	gs.audio.param.fxChannel[2] = gs.config.sound.fxtarget_2;
	gs.audio.param.fxParam[2][0] = gs.config.sound.fxp1_2;
	gs.audio.param.fxParam[2][1] = gs.config.sound.fxp2_2;
	InitSound(&gs.audio,gs.config.sound.bufferlength,gs.config.sound.numbuffers,gs.config.sound.disabledsp,gs.config.sound.output,gs.config.sound.driver);
	ReadLR2SoundSet(&gs, gs.config.skin.skinFilePath[10], 0);
	if (gs.is_starter == false) {
		if (LoadSound(&gs.audio, &gs.gameplay.muon, fs::make_preferred("LR2files/Config/muon.wav").data(), 1, gs.config.sound.disabledsp, 0) == -1) {
			ErrorLogAdd("muon.wavがありません\n");
			gs.procSelecter = 0;
		}
	}
	clsDx();
	gs.procSelecter = 2;
	gs.po4MainMenuCursor = 0;
	gs.procPhase = 0;
	if (gs.cmd_directplay) {
		gs.procSelecter = 4;
		gs.gameplay.flag_closingPhase = 1;
		gs.gameplay.isPreviewLoad = 0;
		gs.gameplay.flag_gameinput = 0;
		for (int i = 0; i < SLOTS; i++) {
			StopSound(&gs.audio, &gs.gameplay.keysound[i]);
		}
		gs.gameplay.previewStatus = 0;
		gs.gameplay.isCourse = 0;
		gs.gameplay.courseStageCount = 1;
		for (int i = 0; i < 5; i++) {
			gs.gameplay.courseFilepath[i].fillzero();
		}
		gs.gameplay.courseStageNow = 0;
		gs.gameplay.courseType = -1;
		gs.isSkipDrawTick = 1;
		gs.net.rankingData.target_ID = 0;
		gs.gameplay.ghostBattle = '\0';
		gs.gameplay.flag_retry = 0;
		gs.sSelect.listCalculatedBar = 0;
		gs.sSelect.barMoveStartTime = 0;
		gs.sSelect.barMoveEndTime = 0;
		gs.sSelect.oldBar = 0;
		gs.sSelect.nowBar = 0;
		gs.sSelect.listTopbar = 0;
		gs.sSelect.listSelectedBarFromScreenTop = 0;
		gs.sSelect.flag_folderlamp = '\0';
		gs.sSelect.cur_song = 0;
		ProcS_Select(&gs);
		gs.gameplay.replay.status = 0;
		gs.gameplay.isAutoplay = (gs.cmd_auto != 0);
		if ((gs.auto2avi != '\0') || (gs.is_recordmode != '\0')) {
			gs.gameplay.flag_closingPhase = 0;
			gs.gameplay.isAutoplay = 1;
			gs.gameplay.replay.status = 0;
			if (gs.rec.recMode == 2) {
				gs.gameplay.replay.status = gs.rec.recMode;
				gs.gameplay.isAutoplay = 0;
			}
			else if (gs.rec.recMode == 3) {
				gs.skstruct.drBuf.isDisabled = 1;
			}
			if (gs.auto2avi) {
				Proc_Auto2avi(&gs, gs.directoryPath, gs.directoryFilename);
				gs.procSelecter = 0;
			}
		}
	}
	int startTime = GetTimeWrap();

	GetTimeWrap();
	if (gs.is_starter) {
		gs.procSelecter = 11;
		gs.po4procSelecter = 0;
	}

	gs.sSelect.cur_song = 0;//DEBUG: cur_song no init in original code, this is temporary init
	gs.txtStruct.readme.show = 0; //DEBUG: readme.show no init in original code, this is temporary init
	gs.sSelect.searchFocused = 0; //DEBUG: searchFocused no init in original code, this is temporary init
	gs.sSelect.isRandomFolder = 0; //DEBUG: isRandomFolder no init in original code, this is temporary init
	gs.sSelect.unk5000 = 0; //DEBUG: no init in original code, this is temporary init

	while (true) { //main loop
		if (ProcessMessage() || !gs.procSelecter || gs.auto2avi) break;

#ifdef _WIN32
		if (GetWindowModeFlag()) { // windowed
			GetWindowSize(&wSizeX, &wSizeY);
			if (0 < wSizeX && wSizeX < 9999 && gs.config.system.windowsize_x != wSizeX) {
				gs.config.system.windowsize_x = wSizeX;
			}
			if (0 < wSizeY && wSizeY < 9999 && gs.config.system.windowsize_y != wSizeY) {
				gs.config.system.windowsize_y = wSizeY;
			}
		}
#endif // _WIN32
		DrawGraph(0, 0, backgroundGrHandle, 0);
		if (gs.cmd_directplay && gs.procSelecter != 4 && gs.procSelecter != 5 && gs.procSelecter != 13 && gs.procPhase != 2 && gs.procPhase != 3) {
			ErrorLogFmtAdd("break\n");
			break;
		}

		if (GetTimeWrap() >= startTime + 6) {
			GetTimeWrap();
			GetTimeWrap();
			GetTimeWrap();
			GetTimeWrap();
		}

		enum {
			PROC_PHASE_ENTERING = 0,
		};
		enum {
			SCENE_SELECT = 2,
			SCENE_DECIDE = 3,
			SCENE_PLAY = 4,
			SCENE_RESULT = 5,
			SCENE_KEYCONFIG = 6,
			SCENE_SKINSELECT = 7,
			SCENE_LUNARIS = 8,
			SCENE_PO4MENU = 9,
			SCENE_PO4DECIDE = 10,
			SCENE_PO4SELECT = 11,
			SCENE_COURSERESULT = 13,
		};
		static const auto get_scene_name_for_nowplayingtxt = [](int procSelecter) -> const char* {
			switch (procSelecter) {
			case SCENE_SELECT: return "select";
			case SCENE_DECIDE: return "decide";
			case SCENE_PLAY: return "play";
			case SCENE_RESULT: return "result";
			case SCENE_KEYCONFIG: return "keyconfig";
			case SCENE_SKINSELECT: return "skinselect";
			case SCENE_LUNARIS: return "lunaris";
			case SCENE_PO4MENU: return "po4menu";
			case SCENE_PO4DECIDE: return "po4decide";
			case SCENE_PO4SELECT: return "po4select";
			case SCENE_COURSERESULT: return "courseresult";
			default: return "unknown";
			}
		};
		if (gs.procPhase == PROC_PHASE_ENTERING) {
			static const auto put_json_escaped = [](std::string& sink, std::string_view s) {
				// Not multi-byte aware. Cry about it.
				for (const auto c : s)
					switch (c) { // https://www.json.org/json-en.html
					case '"':
					case '\\':
					case '/':
					case '\b':
					case '\f':
					case '\n':
					case '\r':
					case '\t': sink += '\\'; [[fallthrough]];
					default: sink += c; break;
					}
			};
			static const auto put_json_k_atom = [](std::string& sink, std::string_view k) {
				sink += '"';
				sink += k;
				sink += R"(": )";
			};
			// \param v nullable c-string since CSTR::body may be nullptr
			static const auto put_json_kv_atom = [](std::string& sink, std::string_view k, const char* v) {
				put_json_k_atom(sink, k);
				sink += '"';
				put_json_escaped(sink, v ? v : "");
				sink += '"';
			};
			static const auto put_song_data = [](std::string& sink, game& g) {
				put_json_k_atom(sink, "song");
				sink += '{';
				put_json_kv_atom(sink, "artist", g.sSelect.bmsList->artist.c_str());
				sink += ',';
				put_json_kv_atom(sink, "search", g.sSelect.stack_searchTitle[g.sSelect.cur].c_str());
				sink += ',';
				put_json_kv_atom(sink, "subartist", g.sSelect.bmsList->subartist.c_str());
				sink += ',';
				put_json_kv_atom(sink, "subtitle", g.sSelect.bmsList->subtitle.c_str());
				sink += ',';
				put_json_kv_atom(sink, "tag", g.sSelect.bmsList->tag.c_str());
				sink += ',';
				put_json_kv_atom(sink, "title", g.sSelect.bmsList->title.c_str());
				sink += '}';
			};
			static const auto get_scene_status_string = [](game& g) -> std::string {
				std::string ret;
				ret += '{';
				put_json_kv_atom(ret, "state", get_scene_name_for_nowplayingtxt(g.procSelecter));
				switch (g.procSelecter) {
				case SCENE_PLAY:
				case SCENE_RESULT:
				case SCENE_COURSERESULT:
					ret += ',';
					put_song_data(ret, g);
					break;
				}
				ret += '}';
				return ret;
			};
			std::ofstream("nowstate.json") << get_scene_status_string(gs);

			InitFade(&gs.audio);
			gs.gameplay.flag_closingPhase = 1;
			gs.gameplay.isPreviewLoad = 0;
			gs.gameplay.flag_gameinput = 0;
			gs.gameplay.previewStatus = 0;
			for (int i = 0; i < 900; i++) {
				gs.skstruct.op[i] = GetOptionFlag_dst(&gs, i);
				gs.skstruct2.op[i] = GetOptionFlag_dst(&gs, i);
			}
			for (int i = 900; i < 1000; i++) {
				gs.skstruct.op[i] = 0;
				gs.skstruct2.op[i] = 0;
			}
			if (gs.sSelect.bmsList[gs.sSelect.cur_song].hash.isSame("9cbea4427d7e8caff0d28f9d7600cdec")) {
				gs.sSelect.bmsList[gs.sSelect.cur_song].title = AutomationFactory();
				gs.sSelect.bmsList[gs.sSelect.cur_song].fulltitle = gs.sSelect.bmsList[gs.sSelect.cur_song].title;
				ProcS_Select(&gs);
			}

			switch (gs.procSelecter) {
				case SCENE_SELECT:
					if (gs.gameplay.replay.status == 2) {
						memcpy(&gs.config.play, &gs.gameplay.replay.cfg, sizeof(CONFIG_PLAY));
					}
					if (gs.net.rankingData.target_ID != 0) {
						memcpy(&gs.config.play, &gs.gameplay.targetCfg, sizeof(CONFIG_PLAY));
					}
					gs.gameplay.ghostBattle = '\0';
					ReadKeyConfig(&gs, (gs.config.select.control == 0)
							? fs::make_preferred("LR2files/Config/keyconfig.xml" ).data()
							: fs::make_preferred("LR2files/Config/keyconfig_p.xml").data());
					DeleteGraph(gs.skstruct.GrHandle[GrH_Stage]);
					gs.skstruct.GrHandle[GrH_Stage] = -1;
					DeleteGraph(gs.skstruct.GrHandle[GrH_BackBMP]);
					gs.skstruct.GrHandle[GrH_BackBMP] = -1;
					gs.sSelect.is_clicked_autoplay_replay = '\0';
					gs.sSelect.is_clicked_keyconfig = '\0';
					gs.sSelect.is_clicked_skinselect = '\0';
					gs.sSelect.course.isCourseCreated = 0;
					gs.gameplay.replay.status = 0;
					if (gs.po4MainMenuCursor == 3) {
						LoadSceneG(&gs, &gs.skstruct, SKINTYPE_COURSESELECT);
						if (gs.sSelect.toRoot) {
							gs.sSelect.bmsListCount = 1;
							InitSongData(gs.sSelect.bmsList);
							gs.sSelect.bmsList->title = "NEW COURSE";
							gs.sSelect.bmsList->fulltitle = "NEW COURSE";
							gs.sSelect.bmsList->artist = "新規コースを作成します";
							gs.sSelect.bmsList->folderType = 0;
							gs.sSelect.listCalculatedBar = 0;
							gs.sSelect.barMoveStartTime = GetTimeWrap();
							gs.sSelect.barMoveEndTime = GetTimeWrap();
							gs.sSelect.flag_folderlamp = gs.config.select.folderlamp != 0;
							gs.sSelect.oldBar = 0;
							gs.sSelect.nowBar = 0;
							gs.sSelect.listTopbar = 0;
							gs.sSelect.listSelectedBarFromScreenTop = gs.skstruct.BAR_CENTER;
							if (gs.cmd_directplay == 0) {
								int intTemp = gs.sSelect.listCalculatedBar / 1000;
								if (gs.sSelect.listCalculatedBar != intTemp * 1000 && gs.sSelect.scrollDirection == 2) 
									intTemp++;
								gs.sSelect.cur_song = (intTemp + gs.sSelect.bmsListCount * 30) % gs.sSelect.bmsListCount;
							}
							else {
								gs.sSelect.cur_song = 0;
							}
						}
					}
					else {
						LoadSceneG(&gs, &gs.skstruct, SKINTYPE_SELECT);
						if (gs.sSelect.toRoot) {
							gs.sSelect.cur = 0;
							if (gs.is_starter == 0) {
								cstrSprintf(&gs.sSelect.stack_query[gs.sSelect.cur], "SELECT * FROM folder WHERE parent = \'%s\'", AssignCRC32("ROOT").body);
								gs.sSelect.stack_isFolder[gs.sSelect.cur] = 1;
								gs.sSelect.stack_rivalID[gs.sSelect.cur] = 0;
								gs.sSelect.stack_searchTitle[gs.sSelect.cur] = "検索語句を入力";
								gs.sSelect.directory = "ROOT";
								LoadBmsListFromDB(gs.sSelect.stack_query[gs.sSelect.cur], sql3, &gs.sSelect, &gs.config.select.difficulty, &gs.config.select.key, 0, 0);
								SwapBmsList(&gs.sSelect);
							}
							else {
								cstrSprintf(&gs.sSelect.stack_query[gs.sSelect.cur], "SELECT * FROM song LEFT JOIN score ON song.hash = score.hash WHERE parent = \'%s\'", AssignCRC32(gs.config.jukebox.path[0]).body);
								gs.sSelect.stack_isFolder[gs.sSelect.cur] = 0;
								gs.sSelect.stack_rivalID[gs.sSelect.cur] = 0;
								gs.sSelect.stack_searchTitle[gs.sSelect.cur] = "検索語句を入力";
								gs.sSelect.directory = "ROOT";
								LoadFilteredBmsListFromDB(gs.sSelect.stack_query[gs.sSelect.cur], sql3, &gs.sSelect, &gs.config.select.difficulty, &gs.config.select.key, gs.config.select.sort, 0, 0);
								SwapBmsList(&gs.sSelect);
							}
							if (gs.rec.recMode == 4) {
								gs.sSelect.stack_searchTitle->assign("オートプレイかリプレイを録画します");
							}
							gs.sSelect.listCalculatedBar = 0;
							gs.sSelect.barMoveStartTime = GetTimeWrap();
							gs.sSelect.barMoveEndTime = GetTimeWrap();
							gs.sSelect.flag_folderlamp = gs.config.select.folderlamp != 0;
							gs.sSelect.oldBar = 0;
							gs.sSelect.nowBar = 0;
							gs.sSelect.listTopbar = 0;
							gs.sSelect.listSelectedBarFromScreenTop = gs.skstruct.BAR_CENTER;
							if (gs.cmd_directplay == 0) {
								tmp = gs.sSelect.listCalculatedBar / 1000;
								if ((gs.sSelect.listCalculatedBar != tmp * 1000) &&	(gs.sSelect.scrollDirection == 2))
									tmp++;
								gs.sSelect.cur_song = (tmp + gs.sSelect.bmsListCount * 30) % gs.sSelect.bmsListCount;
								gs.sSelect.cur_song = ((gs.sSelect.listCalculatedBar / 1000) + ((gs.sSelect.listCalculatedBar != gs.sSelect.listCalculatedBar / 1000 * 1000) && (gs.sSelect.scrollDirection == 2)) + gs.sSelect.bmsListCount * 30) % gs.sSelect.bmsListCount;
							}
							else {
								gs.sSelect.cur_song = 0;
							}
						}
						gs.sSelect.course.count = -1;
						gs.sSelect.course.isMakingCourse = '\0';
					}
					gs.sSelect.toRoot = 0;
					LoadFontForSongs(&gs, 0);

					StopSysSound(&gs);
					if (gs.is_recordmode == 0) {
						for (int i = 0; i < SLOTS; i++) {
							StopSound(&gs.audio, &gs.gameplay.keysound[i]);
							ReleaseSound(&gs.audio, &gs.gameplay.keysound[i]);
						}
						ErrorLogAdd("BMSの音を初期化しました\n");
					}
					ReleaseBGA(&gs);

					gs.sSelect.panel_unk = -1;
					ReadLR2SoundSet(&gs, gs.config.skin.skinFilePath[10], 0);
					if (gs.config.play.is_extra && gs.audio.sysSound.exselect.load)
						PlaySound(&gs.audio, &gs.audio.sysSound.exselect, gs.audio.chnBgm, -1);
					else
						PlaySound(&gs.audio, &gs.audio.sysSound.select, gs.audio.chnBgm, -1);

					ProcS_Select(&gs);
					break;
							
				case SCENE_DECIDE:{
					DeleteGraph(gs.skstruct.GrHandle[GrH_Stage]);
					gs.skstruct.GrHandle[GrH_Stage] = -1;
					DeleteGraph(gs.skstruct.GrHandle[GrH_BackBMP]);
					gs.skstruct.GrHandle[GrH_BackBMP] = -1;
					DeleteGraph(gs.skstruct.GrHandle[GrH_Banner]);
					gs.skstruct.GrHandle[GrH_Banner] = -1;
					SetTransColor(0, 255, 0);
					CSTR dir(gs.sSelect.bmsList[gs.sSelect.cur_song].filepath.getDirectory());
					if (gs.sSelect.bmsList[gs.sSelect.cur_song].isStagefile) {
						CSTR oBuf;
						if (FindAltImage(gs.sSelect.bmsList[gs.sSelect.cur_song].stagefile, dir, &oBuf) != 1)
							gs.sSelect.bmsList[gs.sSelect.cur_song].isStagefile = 0;
						gs.skstruct.GrHandle[GrH_Stage] = LoadGraph(oBuf, 0);
						if (gs.skstruct.GrHandle[GrH_Stage] == -1) gs.sSelect.bmsList[gs.sSelect.cur_song].isStagefile = 0;
					}
					if (gs.sSelect.bmsList[gs.sSelect.cur_song].isBackBMP) {
						CSTR oBuf;
						if (FindAltImage(gs.sSelect.bmsList[gs.sSelect.cur_song].backBMP, dir, &oBuf) != 1)
							gs.sSelect.bmsList[gs.sSelect.cur_song].isBackBMP = 0;
						gs.skstruct.GrHandle[GrH_BackBMP] = LoadGraph(oBuf, 0);
						if (gs.skstruct.GrHandle[GrH_BackBMP] == -1) gs.sSelect.bmsList[gs.sSelect.cur_song].isBackBMP = 0;
					}
					if (gs.sSelect.bmsList[gs.sSelect.cur_song].isBanner) {
						CSTR oBuf;
						if (FindAltImage(gs.sSelect.bmsList[gs.sSelect.cur_song].banner, dir, &oBuf) != 1) 
							gs.sSelect.bmsList[gs.sSelect.cur_song].isBanner = 0;
						gs.skstruct.GrHandle[GrH_Banner] = LoadGraph(oBuf, 0); //TOFIX : when banner size is not 300 80, this will break graph size and banner is not displayed until next song
						if (gs.skstruct.GrHandle[GrH_Banner] == -1) gs.sSelect.bmsList[gs.sSelect.cur_song].isBanner = 0;
					}
					SetTransColor(0, 255, 0);
							
					LoadSceneG(&gs, &gs.skstruct, SKINTYPE_DECIDE);
							
					StopSysSound(&gs);
					if (gs.config.play.is_extra && gs.audio.sysSound.exdecide.load)
						PlaySound(&gs.audio, &gs.audio.sysSound.exselect, gs.audio.chnBgm, -1);
					else
						PlaySound(&gs.audio, &gs.audio.sysSound.decide, gs.audio.chnBgm, -1);
					break; }
				case SCENE_PLAY:
					if (gs.gameplay.courseType==0 || gs.gameplay.courseType==2) {
						SONGDATA sd;
						GetSongData(gs.sSelect.bmsList[gs.sSelect.cur_song].courseHash[gs.gameplay.courseStageNow], &sd, sql3, &gs.sSelect);
						SetObjectString(10, sd.title, gs.txtStruct.objectStr);
						SetObjectString(11, sd.subtitle, gs.txtStruct.objectStr);
						SetObjectString(12, sd.fulltitle, gs.txtStruct.objectStr);
						SetObjectString(13, sd.genre, gs.txtStruct.objectStr);
						SetObjectString(14, sd.artist, gs.txtStruct.objectStr);
						SetObjectString(15, sd.subartist, gs.txtStruct.objectStr);
					}
					gs.skstruct.flag_flip = false;
					if (gs.gameplay.replay.status == 2 || gs.config.play.replay == 0) {
						if (gs.gameplay.replay.status == 1) {
							AllocReplayBuffer(&gs.gameplay.replay);
							//TOFIX : replay option mismatch, when use ghostbattle. ProcS_Play()->GetTargetInfo() changes option (gauge, random)
							AddReplayDataHeader(&gs.config.play, &gs.gameplay.replay, &gs.audio, &gs.gameplay);
						}
						else if (gs.gameplay.replay.status == 2) {
							int flagLoadSuccess;
							if (gs.sSelect.bmsList[gs.sSelect.cur_song].courseType == 0 || gs.sSelect.bmsList[gs.sSelect.cur_song].courseType == 2) {
								flagLoadSuccess = LoadReplayFileCourse(&gs.gameplay.replay, gs.sSelect.bmsList[gs.sSelect.cur_song].hash,gs.gameplay.courseStageNow, gs.config.player.id);
							}
							else {
								flagLoadSuccess = LoadReplayFile(&gs.gameplay.replay, gs.sSelect.bmsList[gs.sSelect.cur_song].hash, gs.config.player.id);
							}
							if (flagLoadSuccess == -1 && gs.rec.recMode == 2 && gs.is_recordmode) return 0;
							SetReplayConfig(&gs.gameplay.replay, &gs, &gs.audio, &gs.gameplay, &gs.KeyInput, &gs.timer1);
						}
					}
					else {
						gs.gameplay.replay.status = 1;
						AllocReplayBuffer(&gs.gameplay.replay);
						//TOFIX : replay option mismatch, when use ghostbattle. ProcS_Play()->GetTargetInfo() changes option (gauge, random)
						AddReplayDataHeader(&gs.config.play, &gs.gameplay.replay, &gs.audio, &gs.gameplay);
					}
					if (gs.gameplay.ghostBattle == 1) {
						if (gs.sSelect.bmsList[gs.sSelect.cur_song].keymode == 5) {
							LoadSceneG(&gs, &gs.skstruct, SKINTYPE_5KEYSBATTLE);
							if (gs.skinData.Data[gs.skinData.skinID[13]].type != SKINTYPE_5KEYSBATTLE)
								ReadKeyConfig(&gs, fs::make_preferred("LR2files/Config/keyconfig.xml").data());
							else 
								ReadKeyConfig(&gs, fs::make_preferred("LR2files/Config/keyconfig_5.xml").data());
						}
						else {
							LoadSceneG(&gs, &gs.skstruct, SKINTYPE_7KEYSBATTLE);
							ReadKeyConfig(&gs, fs::make_preferred("LR2files/Config/keyconfig.xml").data());
						}
					}
					else if (gs.config.select.control == 1 && (gs.sSelect.metaSelected.keymode == 5 || gs.sSelect.metaSelected.keymode == 7)) {
						if (gs.config.play.battle == 0 || gs.config.play.battle == 2) {
							LoadSceneG(&gs, &gs.skstruct, SKINTYPE_9KEYS);
						}
						else if (gs.config.play.battle == 1) {
							LoadSceneG(&gs, &gs.skstruct, SKINTYPE_9KEYSBATTLE);
						}
						ReadKeyConfig(&gs,fs::make_preferred("LR2files/Config/keyconfig_p.xml").data());
					}
					else {
						switch (gs.sSelect.metaSelected.keymode) {
							case 5:
								if (gs.config.play.battle == 0) {
									LoadSceneG(&gs, &gs.skstruct, SKINTYPE_5KEYS);
									if (gs.skinData.Data[gs.skinData.skinID[1]].type != SKINTYPE_5KEYS)
										ReadKeyConfig(&gs, fs::make_preferred("LR2files/Config/keyconfig.xml").data());
									else
										ReadKeyConfig(&gs, fs::make_preferred("LR2files/Config/keyconfig_5.xml").data());
								}
								else if (gs.config.play.battle == 1) {
									LoadSceneG(&gs, &gs.skstruct, SKINTYPE_5KEYSBATTLE);
									if (gs.skinData.Data[gs.skinData.skinID[13]].type != SKINTYPE_5KEYSBATTLE)
										ReadKeyConfig(&gs, fs::make_preferred("LR2files/Config/keyconfig.xml").data());
									else
										ReadKeyConfig(&gs, fs::make_preferred("LR2files/Config/keyconfig_5.xml").data());
								}
								else if (gs.config.play.battle == 2 || gs.config.play.battle == 3) {
									LoadSceneG(&gs, &gs.skstruct, SKINTYPE_10KEYS);
									if (gs.skinData.Data[gs.skinData.skinID[3]].type != SKINTYPE_10KEYS)
										ReadKeyConfig(&gs, fs::make_preferred("LR2files/Config/keyconfig.xml").data());
									else
										ReadKeyConfig(&gs, fs::make_preferred("LR2files/Config/keyconfig_5.xml").data());
								}
								break;

							default:
								if (gs.config.play.battle == 0) {
									LoadSceneG(&gs, &gs.skstruct, SKINTYPE_7KEYS);
								}
								else if (gs.config.play.battle == 1) {
									LoadSceneG(&gs, &gs.skstruct, SKINTYPE_7KEYSBATTLE);
								}
								else if (gs.config.play.battle == 2 || gs.config.play.battle == 3) {
									LoadSceneG(&gs, &gs.skstruct, SKINTYPE_14KEYS);
								}
								ReadKeyConfig(&gs, fs::make_preferred("LR2files/Config/keyconfig.xml").data());
								break;

							case 9:
								if (gs.config.play.battle == 3) {
									LoadSceneG(&gs, &gs.skstruct, SKINTYPE_7KEYS);
									ReadKeyConfig(&gs, fs::make_preferred("LR2files/Config/keyconfig.xml").data());
									break;
								}
								else if (gs.config.play.battle == 0 || gs.config.play.battle == 2) {
									LoadSceneG(&gs, &gs.skstruct, SKINTYPE_9KEYS);
								}
								else if (gs.config.play.battle == 1) {
									LoadSceneG(&gs, &gs.skstruct, SKINTYPE_9KEYSBATTLE);
								}
								ReadKeyConfig(&gs, fs::make_preferred("LR2files/Config/keyconfig_p.xml").data());
								break;

							case 10:
								if (gs.config.play.battle == 0) {
									LoadSceneG(&gs, &gs.skstruct, SKINTYPE_10KEYS);
									if (gs.skinData.Data[gs.skinData.skinID[3]].type == SKINTYPE_10KEYS)
										ReadKeyConfig(&gs, fs::make_preferred("LR2files/Config/keyconfig_5.xml").data()); 
									else
										ReadKeyConfig(&gs, fs::make_preferred("LR2files/Config/keyconfig.xml").data());
								}
								else if (gs.config.play.battle == 1 || gs.config.play.battle == 2) {
									LoadSceneG(&gs, &gs.skstruct, SKINTYPE_5KEYSBATTLE);
									if (gs.skinData.Data[gs.skinData.skinID[13]].type == SKINTYPE_5KEYSBATTLE)
										ReadKeyConfig(&gs, fs::make_preferred("LR2files/Config/keyconfig_5.xml").data());
									else
										ReadKeyConfig(&gs, fs::make_preferred("LR2files/Config/keyconfig.xml").data());
								}
								else if (gs.config.play.battle == 3) {
									LoadSceneG(&gs, &gs.skstruct, SKINTYPE_5KEYS);
									if (gs.skinData.Data[gs.skinData.skinID[1]].type == SKINTYPE_5KEYS)
										ReadKeyConfig(&gs, fs::make_preferred("LR2files/Config/keyconfig_5.xml").data());
									else
										ReadKeyConfig(&gs, fs::make_preferred("LR2files/Config/keyconfig.xml").data());
								}
								break;

							case 14:
								if (gs.config.play.battle == 0 || gs.config.play.battle == 2) {
									LoadSceneG(&gs, &gs.skstruct, SKINTYPE_14KEYS);
								}
								else if (gs.config.play.battle == 1) {
									LoadSceneG(&gs, &gs.skstruct, SKINTYPE_7KEYSBATTLE);
								}
								else if(gs.config.play.battle == 3) {
									LoadSceneG(&gs, &gs.skstruct, SKINTYPE_7KEYS);
								}
								ReadKeyConfig(&gs, fs::make_preferred("LR2files/Config/keyconfig.xml").data());
								break;
						}
					}

					if (gs.config.play.m_lunaris) {
						LoadSceneG(&gs, &gs.skstruct, SKINTYPE_7KEYS);
						ReadKeyConfig(&gs, fs::make_preferred("LR2files/Config/keyconfig.xml").data());
						gs.gameplay.isAutoplay = 0;
					}
					for (int i = 0; i < gs.skstruct.num_of_ImageFont; i++) {
						LoadFontForText(&gs.skstruct.ImageFonts[i], &gs.sSelect.bmsList[gs.sSelect.cur_song].artist);
						LoadFontForText(&gs.skstruct.ImageFonts[i], &gs.sSelect.bmsList[gs.sSelect.cur_song].genre);
						LoadFontForText(&gs.skstruct.ImageFonts[i], &gs.sSelect.bmsList[gs.sSelect.cur_song].subartist);
						LoadFontForText(&gs.skstruct.ImageFonts[i], &gs.sSelect.bmsList[gs.sSelect.cur_song].title);
						LoadFontForText(&gs.skstruct.ImageFonts[i], &gs.sSelect.bmsList[gs.sSelect.cur_song].subtitle);
					}
					ProcS_Play(&gs, sql3);
					break;
				case SCENE_RESULT:
					ProcS_Result(&gs);
					break;
				case SCENE_KEYCONFIG:
					LoadSceneG(&gs, &gs.skstruct, SKINTYPE_KEYCONFIG);
					gs.KeyInput.config_keymode = 0;
					gs.KeyInput.config_button = 1;
					gs.KeyInput.config_button_inMap = 1;
					gs.KeyInput.config_key = -1;
					ProcS_Keyconfig(&gs);
					break;
				case SCENE_SKINSELECT:
					LoadSceneG(&gs, &gs.skstruct, SKINTYPE_SKINSELECT);
					ProcS_SkinSelect(&gs);
					break;
				case SCENE_LUNARIS:
					LoadSceneG(&gs, &gs.skstruct, SKINTYPE_7KEYS);
					ReadKeyConfig(&gs, fs::make_preferred("LR2files/Config/keyconfig.xml").data());
					LUNARIS_START(&gs);
					break;
				case SCENE_PO4MENU:
					LoadSceneG(&gs, &gs.skstruct, SKINTYPE_MODESELECT);
					gs.sSelect.listCalculatedBar = gs.po4MainMenuCursor * 1000;
					gs.sSelect.barMoveStartTime = GetTimeWrap();
					gs.sSelect.barMoveEndTime = GetTimeWrap();
					gs.sSelect.listTopbar = gs.po4MainMenuCursor * 1000;
					gs.sSelect.flag_folderlamp = (gs.config.select.folderlamp != 0);
					gs.sSelect.listSelectedBarFromScreenTop = gs.skstruct.BAR_CENTER;
					if (gs.cmd_directplay == 0) {
						int intTemp = gs.sSelect.listCalculatedBar / 1000;
						if (gs.sSelect.listCalculatedBar != intTemp * 1000 && gs.sSelect.scrollDirection == 2) 
							intTemp++;
						gs.sSelect.cur_song =(intTemp + gs.sSelect.bmsListCount * 0x1e) % gs.sSelect.bmsListCount;
					}
					else {
						gs.sSelect.cur_song = 0;
					}
					gs.sSelect.toRoot = 1;
					gs.sSelect.bmsListCount = 7;
					gs.sSelect.nowBar = gs.sSelect.listTopbar;
					gs.sSelect.oldBar = gs.sSelect.listTopbar;

					gs.sSelect.bmsList[0].title = "FREE";
					gs.sSelect.bmsList[0].fulltitle = "FREE";
					gs.sSelect.bmsList[0].artist = "全曲から自由に選曲できます。";
					gs.sSelect.bmsList[0].level = 0;

					gs.sSelect.bmsList[1].title = "ARCADE";
					gs.sSelect.bmsList[1].fulltitle = "ARCADE";
					gs.sSelect.bmsList[1].artist = "全3ステージ+αをプレイするメインモードです。";
					gs.sSelect.bmsList[1].level = 1;

					gs.sSelect.bmsList[2].title = "CHARANGE";
					gs.sSelect.bmsList[2].fulltitle = "CHARANGE";
					gs.sSelect.bmsList[2].artist = "最高難度に挑戦するモードです。";
					gs.sSelect.bmsList[2].level = 2;

					gs.sSelect.bmsList[3].title = "COURSE";
					gs.sSelect.bmsList[3].fulltitle = "COURSE";
					gs.sSelect.bmsList[3].artist = "決められた5曲を連続でプレイするモードです。";
					gs.sSelect.bmsList[3].level = 3;

					gs.sSelect.bmsList[4].title = "GRADE";
					gs.sSelect.bmsList[4].fulltitle = "GRADE";
					gs.sSelect.bmsList[4].artist = "プレイヤーの腕前を判定するモードです。";
					gs.sSelect.bmsList[4].level = 4;

					gs.sSelect.bmsList[5].title = "OPTION";
					gs.sSelect.bmsList[5].fulltitle = "OPTION";
					gs.sSelect.bmsList[5].artist = "ゲームの設定を行います。";
					gs.sSelect.bmsList[5].level = 5;

					gs.sSelect.bmsList[6].title = "EXIT";
					gs.sSelect.bmsList[6].fulltitle = "EXIT";
					gs.sSelect.bmsList[6].artist = "ゲームを終了します。";
					gs.sSelect.bmsList[6].level = 6;

					ProcS_Select(&gs);
					break;
				case SCENE_PO4DECIDE:
					LoadSceneG(&gs, &gs.skstruct, SKINTYPE_MODEDECIDE);
					break;
				case SCENE_PO4SELECT:
					LoadScene(&gs.skstruct, fs::make_preferred("LR2files/event.csv").data(), 0, 0);
					break;
				case SCENE_COURSERESULT:
					ProcS_CourseResult(&gs,sql3);
					break;
			}
			InitTimer(&gs.timer1);
			SetTimeLapse(0, &gs.timer1);
			gs.procPhase = 1;
		}
		GetTimeWrap();

		GetTimeWrap();
		if (gs.skstruct.startinput_start < GetTimeLapse(0, &gs.timer1) && GetTimeLapse(1, &gs.timer1) == -1.0) {
			InitInputStructure(&gs.KeyInput);
			SetTimeLapse(1, &gs.timer1);
			SetTimeLapse(11, &gs.timer1);
		}
		if (((gs.procPhase == 1) || (gs.procPhase == 2)) || (gs.procPhase == 3)) {
			GetTimeWrap();
			int procPrev = gs.procSelecter;
			switch (gs.procSelecter) {
				case 2:
					ProcI_Select(&gs, sql3);
					break;
				case 3:
					ProcI_Decide(&gs);
					break;
				case 4:
					ProcI_Play(&gs);
					break;
				case 5:
				case 13:
					ProcI_Result(&gs);
					break;
				case 6:
					ProcI_Keyconfig(&gs);
					break;
				case 7:
					ProcI_SkinSelect(&gs);
					break;
				case 8:
					ProcI_Lunaris(&gs);
					break;
				case 9:
					ProcI_PO4Menu(&gs, sql3);
					break;
				case 10:
					ProcI_PO4Decide(&gs);
					break;
				case 11:
					ProcI_PO4Select(&gs, sql3);
					break;
				default:
					//goto LAB_00434b4b; //we don't need this JMP. because gs.procSelecter doesn't change in this case.
					break;
			}
			if (procPrev != gs.procSelecter) { //This is Close of Scene
				PlayerCheckAndSwap(&gs.gameplay);
				switch (procPrev) {
				case 2:
					gs.gameplay.flag_closingPhase = 1;
					gs.gameplay.isPreviewLoad = 0;
					gs.gameplay.flag_gameinput = 0;
					for (int i = 0; i < SLOTS; i++) {
						StopSound(&gs.audio, &gs.gameplay.keysound[i]);
					}
					gs.gameplay.previewStatus = 0;
					gs.gameplay.isCourse = 0;
					gs.gameplay.courseStageCount = 1;
					gs.gameplay.randomseed = 0;
					for (int i = 0; i < 5; i++) {
						gs.gameplay.courseFilepath[i].fillzero();
					}
					gs.gameplay.courseStageNow = 0;
					gs.gameplay.courseType = -1;
					if (gs.sSelect.bmsList[gs.sSelect.cur_song].coursePlayable == 1) {
						SONGDATA sd;
						gs.gameplay.isCourse = 1;
						gs.gameplay.courseStageCount = gs.sSelect.bmsList[gs.sSelect.cur_song].courseStageCount;
						for (int i = 0; i < gs.sSelect.bmsList[gs.sSelect.cur_song].courseStageCount; i++) {
							GetSongData(gs.sSelect.bmsList[gs.sSelect.cur_song].courseHash[i], &sd, sql3, &gs.sSelect);
							gs.gameplay.courseFilepath[i] = sd.filepath;
						}
						gs.gameplay.courseType = gs.sSelect.bmsList[gs.sSelect.cur_song].courseType;
						gs.gameplay.courseConnection[0] =	gs.sSelect.bmsList[gs.sSelect.cur_song].courseKeys[0];
						gs.gameplay.courseConnection[1] =	gs.sSelect.bmsList[gs.sSelect.cur_song].courseKeys[1];
						gs.gameplay.courseConnection[2] =	gs.sSelect.bmsList[gs.sSelect.cur_song].courseKeys[2];
						gs.gameplay.courseConnection[3] =	gs.sSelect.bmsList[gs.sSelect.cur_song].courseKeys[3];
						gs.gameplay.courseConnection[4] =	gs.sSelect.bmsList[gs.sSelect.cur_song].courseKeys[4];
						gs.gameplay.courseConnection[5] =	gs.sSelect.bmsList[gs.sSelect.cur_song].courseKeys[5];
						gs.gameplay.courseConnection[6] =	gs.sSelect.bmsList[gs.sSelect.cur_song].courseKeys[6];
						gs.gameplay.courseConnection[7] =	gs.sSelect.bmsList[gs.sSelect.cur_song].courseKeys[7];
						gs.gameplay.courseConnection[8] =	gs.sSelect.bmsList[gs.sSelect.cur_song].courseKeys[8];
						if (gs.sSelect.bmsList[gs.sSelect.cur_song].courseType == 2) {
							gs.config.play.random[0] = 0;
							gs.config.play.random[1] = 0;
							if (gs.config.play.gaugeOption[0] == 3) {
								gs.config.play.gaugeOption[0] = 0;
							}
							if (gs.config.play.gaugeOption[1] == 3) {
								gs.config.play.gaugeOption[1] = 0;
							}
							if (gs.config.play.gaugeOption[0] == 5) {
								gs.config.play.gaugeOption[0] = 0;
							}
							if (gs.config.play.gaugeOption[1] == 5) {
								gs.config.play.gaugeOption[1] = 0;
							}
							gs.config.play.m_HIDSUD1 = 0;
							gs.config.play.m_HIDSUD2 = 0;
							gs.config.play.p1_assist = 0;
							gs.config.play.p2_assist = 0;
							gs.config.play.battle = 0;
							gs.config.play.is_extra = 0;
							gs.config.play.m_accel = 0;
							gs.config.play.m_addnote = 0;
							gs.config.play.autokey = 0;
							gs.config.play.m_char = 0;
							gs.config.play.m_earthquake = 0;
							gs.config.play.m_extra = 0;
							gs.config.play.dpflip = 0;
							gs.config.play.m_lunaris = '\0';
							gs.config.play.m_gambol = 0;
							gs.config.play.m_sidejump = 0;
							gs.config.play.m_nabeatsu = 0;
							gs.config.play.m_heartbeat = 0;
							gs.config.play.m_sincurve = 0;
							gs.config.play.m_softlanding = 0;
							gs.config.play.m_spiral = 0;
							gs.config.play.m_loudness = 0;
							gs.config.play.m_wave = 0;
							gs.config.play.m_tornado = 0;
							gs.audio.param.pitch_amount = 0;
							gs.audio.param.pitch_on = 0;
							ApplySoundFX(&gs.audio, 1, 0);
						}
						if (gs.sSelect.bmsList[gs.sSelect.cur_song].courseType == 1) {
							gs.audio.param.pitch_amount = 0;
							gs.audio.param.pitch_on = 0;
							ApplySoundFX(&gs.audio, 1, 0);
						}
					}
					gs.isSkipDrawTick = 1;
					gs.net.rankingData.target_ID = 0;
					gs.gameplay.ghostBattle = 0;
					if (gs.net.rankingData.showRanking == 0) {
						if (gs.sSelect.stack_query[gs.sSelect.cur].findStrPos("__RIVAL__") >= 0) {
							gs.net.rankingData.target_ID = gs.sSelect.stack_rivalID[gs.sSelect.cur];
							memcpy(&gs.gameplay.targetCfg, &gs.config.play, sizeof(CONFIG_PLAY)); // need check
							if (gs.config.play.battle == 4 && gs.sSelect.bmsList[gs.sSelect.cur_song].keymode < 8) gs.gameplay.ghostBattle = 1;
							gs.config.play.battle = 0;
						}
						else if (gs.config.play.battle == 4) {
							gs.config.play.battle = 0;
						}
					}
					else {
						memcpy(&gs.gameplay.targetCfg, &gs.config.play, sizeof(CONFIG_PLAY)); // need check
						if (gs.config.play.battle == 4 && gs.sSelect.bmsList[gs.sSelect.cur_song].keymode < 8) gs.gameplay.ghostBattle = 1;
						gs.config.play.battle = 0;
						gs.net.rankingData.target_ID = gs.net.rankingData.ranking[gs.sSelect.cur_song].id;
						gs.net.rankingData.target_number = gs.sSelect.cur_song;
						ResetTimeLapse(175, &gs.timer1);
						SetTimeLapse(176, &gs.timer1);
						SwapBmsList(&gs.sSelect);
						if (gs.sSelect.prevSelectedBarFromScreenTop != gs.sSelect.listSelectedBarFromScreenTop) {
							gs.sSelect.listTopbar += (gs.sSelect.prevSelectedBarFromScreenTop - gs.sSelect.listSelectedBarFromScreenTop) * 1000;
						}
						gs.sSelect.listCalculatedBar = gs.sSelect.listTopbar;
						while (gs.sSelect.listCalculatedBar < 0) {
							gs.sSelect.listCalculatedBar += gs.sSelect.bmsListCount * 1000;
						}
						while (gs.sSelect.listCalculatedBar >= gs.sSelect.bmsListCount * 1000) {
							gs.sSelect.listCalculatedBar -= gs.sSelect.bmsListCount * 1000;
						}
						gs.sSelect.barMoveStartTime = GetTimeWrap();
						gs.sSelect.barMoveEndTime = GetTimeWrap();
						gs.sSelect.oldBar = gs.sSelect.listTopbar;
						gs.sSelect.nowBar = gs.sSelect.listTopbar;
						if (gs.cmd_directplay == 0) { //same GetSongCursor(g)
							gs.sSelect.cur_song = (gs.sSelect.bmsListCount * 30 - gs.skstruct.BAR_CENTER + gs.sSelect.listSelectedBarFromScreenTop +
								((gs.sSelect.listCalculatedBar % 1000 == 0 || gs.sSelect.scrollDirection != 2) ? (gs.sSelect.listCalculatedBar / 1000) : (gs.sSelect.listCalculatedBar / 1000 + 1)))
								% gs.sSelect.bmsListCount;
						}
						else {
							gs.sSelect.cur_song = 0;
						}
						gs.net.rankingData.showRanking = 0;
						ProcS_Select(&gs);
					}

					if (gs.sSelect.bmsList[gs.sSelect.cur_song].keymode == 10 || gs.sSelect.bmsList[gs.sSelect.cur_song].keymode == 14) {
						if ( gs.config.play.battle >= 2 && gs.config.play.battle != 3) gs.config.play.battle = 0;
					}
					else if (gs.sSelect.bmsList[gs.sSelect.cur_song].keymode == 9) {
						if (gs.config.play.battle >= 1 && gs.config.play.battle != 3) gs.config.play.battle = 0;
					}
					gs.gameplay.flag_retry = 0;
					break;
				case 3:
					gs.config.play.randSC[0] = 0;
					gs.config.play.randSC[1] = 0;
					gs.config.play.randFix[0] = 0;
					gs.config.play.randFix[1] = 0;
					InitInputStructure2(&gs.KeyInput);
					InputToButton(&gs.KeyInput, &gs.config.input, (int)(uint)(gs.sSelect.metaSelected.keymode < 10), 0);
					if (gs.KeyInput.p1_buttonInput[12] == 1 && gs.KeyInput.p1_buttonInput[13] == 1) {
						gs.config.play.randSC[0] = 1;
					}
					else if (gs.KeyInput.p2_buttonInput[12] == 1 && gs.KeyInput.p2_buttonInput[13] == 1) {
						gs.config.play.randSC[1] = 1;
					}
					else if (gs.KeyInput.p1_buttonInput[12] == 1 || gs.KeyInput.p1_buttonInput[13] == 1 || gs.KeyInput.p2_buttonInput[12] == 1 || gs.KeyInput.p2_buttonInput[13] == 1) {
						for (int i = 1; i <= 9; i++) {
							if (gs.KeyInput.p1_buttonInput[i] == 1) {
								gs.config.play.randFix[0] = i;
							}
							if (gs.KeyInput.p2_buttonInput[i] == 1) {
								gs.config.play.randFix[1] = i;
							}
						}
					}
					break;
				case 4: {
					CSTR skinMD5;
					cstrSprintf(&skinMD5, fs::make_preferred("LR2files/SkinCustomize/%s.xml").data(), gs.skstruct.skinMD5.body);
					SkinUser tmpSk;
					ReadSkinCustomize(&tmpSk, skinMD5);
					tmpSk.adjust.shift_x = gs.skstruct.adjust.shift_x;
					tmpSk.adjust.size_y = gs.skstruct.adjust.size_y;
					tmpSk.adjust.shift_y = gs.skstruct.adjust.shift_y;
					tmpSk.adjust.size_x = gs.skstruct.adjust.size_x;
					tmpSk.adjust.judge_x = gs.skstruct.adjust.judge_x;
					tmpSk.adjust.rate_x = gs.skstruct.adjust.rate_x;
					tmpSk.adjust.rate_y = gs.skstruct.adjust.rate_y;
					tmpSk.adjust.note_1p_x = gs.skstruct.adjust.note_1p_x;
					tmpSk.adjust.judge_y = gs.skstruct.adjust.judge_y;
					tmpSk.adjust.dark_type = gs.skstruct.adjust.dark_type;
					tmpSk.adjust.note_2p_y = gs.skstruct.adjust.note_2p_y;
					tmpSk.adjust.note_1p_y = gs.skstruct.adjust.note_1p_y;
					tmpSk.adjust.note_2p_x = gs.skstruct.adjust.note_2p_x;
					WriteSkinCustomizeXml(&tmpSk, skinMD5);
					SetTarget(&gs);

					if (gs.gameplay.isAutoplay) {
						gs.procSelecter = 2;
						if(gs.is_recordmode == 0){
							for (int i = 0; i < SLOTS; i++) {
								StopSound(&gs.audio, &gs.gameplay.keysound[i]);
								ReleaseSound(&gs.audio, &gs.gameplay.keysound[i]);
							}
							ErrorLogAdd("BMSの音を初期化しました\n");
						}
					}
					else if (gs.gameplay.player[0].note_current == 0 && gs.gameplay.player[1].note_current == 0 && gs.config.play.m_lunaris == 0) {
						gs.procSelecter = 2;
						for (int i = 0; i < SLOTS; i++) {
							StopSound(&gs.audio, &gs.gameplay.keysound[i]);
							ReleaseSound(&gs.audio, &gs.gameplay.keysound[i]);
						}
						ErrorLogAdd("BMSの音を初期化しました\n");
					}
					else if (gs.gameplay.player[0].judgecount[3] + gs.gameplay.player[0].judgecount[4] + gs.gameplay.player[0].judgecount[5] != 0) {
						SaveResult(&gs, sql3);
						gs.net.customIR.SendScore(gs, sql3, 0);
					}
					else if (gs.config.play.m_lunaris == 0 && gs.config.play.battle != 1) {
						gs.procSelecter = 2;
						for (int i = 0; i < SLOTS; i++) {
							StopSound(&gs.audio, &gs.gameplay.keysound[i]);
							ReleaseSound(&gs.audio, &gs.gameplay.keysound[i]);
						}
						ErrorLogAdd("BMSの音を初期化しました\n");
					}
					else if ((gs.gameplay.player[1].judgecount[3] + gs.gameplay.player[1].judgecount[4] + gs.gameplay.player[1].judgecount[5] != 0) || gs.config.play.battle != 1) {
						SaveResult(&gs, sql3);
						gs.net.customIR.SendScore(gs, sql3, 1);
					}
					else {
						gs.procSelecter = 2;
						for (int i = 0; i < SLOTS; i++) {
							StopSound(&gs.audio, &gs.gameplay.keysound[i]);
							ReleaseSound(&gs.audio, &gs.gameplay.keysound[i]);
						}
						ErrorLogAdd("BMSの音を初期化しました\n");
					}

					ReadKeyConfig(&gs, fs::make_preferred("LR2files/Config/keyconfig.xml").data());
					if (gs.gameplay.replay.status == 2) {
						ReleaseReplayBuffer(&gs.gameplay.replay);
						gs.audio.param.eq_gain[0] = gs.gameplay.replay.aud.eq_gain[0];
						gs.audio.param.eq_gain[1] = gs.gameplay.replay.aud.eq_gain[1];
						gs.audio.param.eq_gain[3] = gs.gameplay.replay.aud.eq_gain[3];
						gs.audio.param.eq_gain[4] = gs.gameplay.replay.aud.eq_gain[4];
						gs.audio.param.eq_gain[2] = gs.gameplay.replay.aud.eq_gain[2];
						gs.audio.param.eq_gain[6] = gs.gameplay.replay.aud.eq_gain[6];
						gs.audio.param.eq_on = gs.gameplay.replay.aud.eq_on;
						gs.audio.param.eq_gain[5] = gs.gameplay.replay.aud.eq_gain[5];
						for (int i = 0; i < 3; i++) {
							gs.audio.param.fxParam[i][0] = gs.gameplay.replay.aud.fxParam[i][0];
							gs.audio.param.fxParam[i][1] = gs.gameplay.replay.aud.fxParam[i][1];
							gs.audio.param.fxChannel[i] = gs.gameplay.replay.aud.fxChannel[i];
							gs.audio.param.fxType[i] = gs.gameplay.replay.aud.fxType[i];
							gs.audio.param.fx_on[i] = gs.gameplay.replay.aud.fx_on[i];
						}
						gs.audio.param.pitch_on = gs.gameplay.replay.aud.pitch_on;
						gs.audio.param.pitch_type = gs.gameplay.replay.aud.pitch_type;
						gs.audio.param.volume_BGM = gs.gameplay.replay.aud.volume_BGM;
						gs.audio.param.pitch_amount = gs.gameplay.replay.aud.pitch_amount;
						gs.audio.param.volume_key = gs.gameplay.replay.aud.volume_key;
						gs.audio.param.fx_volume_on = gs.gameplay.replay.aud.fx_volume_on;
						gs.audio.param.volume_master = gs.gameplay.replay.aud.volume_master;
						ApplySoundFX(&gs.audio, 1, gs.config.sound.disabledsp);
					}
					else if (gs.gameplay.replay.status == 1) {
						if (gs.gameplay.isAutoplay == 0) {
							if (gs.sSelect.bmsList[gs.sSelect.cur_song].courseType == 2 || gs.sSelect.bmsList[gs.sSelect.cur_song].courseType == 0) {
								CSTR tmp;
								cstrSprintf(&tmp, "__%d", gs.gameplay.courseStageNow);
								OverwriteReplayData(&gs.gameplay.replay, 0, 0x65, static_cast<short>(gs.gameplay.player[0].clearGaugeTypeCourse));
								OverwriteReplayData(&gs.gameplay.replay, 0, 0x97, static_cast<short>(gs.gameplay.player[1].clearGaugeTypeCourse));
								SaveReplay(&gs.gameplay.replay, tmp, gs.config.player.id);
							}
							else if (gs.config.play.replay == 1) {
								OverwriteReplayData(&gs.gameplay.replay, 0, 0x65, static_cast<short>(gs.gameplay.player[0].gaugeType));
								OverwriteReplayData(&gs.gameplay.replay, 0, 0x97, static_cast<short>(gs.gameplay.player[1].gaugeType));
								if (SaveReplay(&gs.gameplay.replay, gs.sSelect.bmsList[gs.sSelect.cur_song].hash, gs.config.player.id) == 1) 
									gs.sSelect.bmsList[gs.sSelect.cur_song].replayExist = 1;
							}
							else if (gs.config.play.replay == 2) {
								if (gs.sSelect.bmsList[gs.sSelect.cur_song].mybest.stat_exscore <= gs.gameplay.player[0].exscore && gs.gameplay.player[0].exscore > 0) {
									OverwriteReplayData(&gs.gameplay.replay, 0, 0x65, static_cast<short>(gs.gameplay.player[0].gaugeType));
									OverwriteReplayData(&gs.gameplay.replay, 0, 0x97, static_cast<short>(gs.gameplay.player[1].gaugeType));
									if (SaveReplay(&gs.gameplay.replay, gs.sSelect.bmsList[gs.sSelect.cur_song].hash, gs.config.player.id) == 1)
										gs.sSelect.bmsList[gs.sSelect.cur_song].replayExist = 1;
								}
							}
							else if (gs.config.play.replay == 3) {
								if (gs.gameplay.player[0].clearType >= 2 && gs.gameplay.player[0].exscore > 0) {
									OverwriteReplayData(&gs.gameplay.replay, 0, 0x65, static_cast<short>(gs.gameplay.player[0].gaugeType));
									OverwriteReplayData(&gs.gameplay.replay, 0, 0x97, static_cast<short>(gs.gameplay.player[1].gaugeType));
									if (SaveReplay(&gs.gameplay.replay, gs.sSelect.bmsList[gs.sSelect.cur_song].hash, gs.config.player.id) == 1)
										gs.sSelect.bmsList[gs.sSelect.cur_song].replayExist = 1;
								}
							}
							else if (gs.config.play.replay == 4) {
								if (gs.gameplay.player[0].clearType == 5 && gs.gameplay.player[0].exscore > 0) {
									OverwriteReplayData(&gs.gameplay.replay, 0, 0x65, static_cast<short>(gs.gameplay.player[0].gaugeType));
									OverwriteReplayData(&gs.gameplay.replay, 0, 0x97, static_cast<short>(gs.gameplay.player[1].gaugeType));
									if (SaveReplay(&gs.gameplay.replay, gs.sSelect.bmsList[gs.sSelect.cur_song].hash, gs.config.player.id) == 1)
										gs.sSelect.bmsList[gs.sSelect.cur_song].replayExist = 1;
								}
							}
						}
						ReleaseReplayBuffer(&gs.gameplay.replay);
					}

					if (gs.gameplay.isAutoplay == 1 && (gs.gameplay.courseType == 0 || gs.gameplay.courseType == 2) && gs.gameplay.courseStageNow < gs.gameplay.courseStageCount -1  && gs.gameplay.flag_closingPhase == 0) {
						gs.gameplay.courseStageNow++;
						gs.procSelecter = 4;
					}
					break;
				}
				case 5:
					if (gs.is_recordmode == 0) {
						if (gs.procSelecter == 4) {
							for (int i = 0; i < SLOTS; i++) {
								StopSound(&gs.audio, &gs.gameplay.keysound[i]);
							}
						}
						else {
							for (int i = 0; i < SLOTS; i++) {
								StopSound(&gs.audio, &gs.gameplay.keysound[i]);
								ReleaseSound(&gs.audio, &gs.gameplay.keysound[i]);
							}
							ErrorLogAdd("BMSの音を初期化しました\n");
							ReleaseBGA(&gs);
						}
					}
					StopSysSound(&gs);
					if (gs.gameplay.courseType == 0 || gs.gameplay.courseType == 2) {
						gs.gameplay.player[0].gaugeType = gs.gameplay.player[0].lastCourseGaugeType;
						gs.gameplay.player[1].gaugeType = gs.gameplay.player[1].lastCourseGaugeType;
						if (gs.gameplay.courseStageNow < gs.gameplay.courseStageCount -1 && gs.gameplay.player[0].clearType != 0 && (gs.gameplay.player[1].clearType != 0 || gs.config.play.battle != 1) && gs.gameplay.player[0].HP[gs.gameplay.player[0].gaugeType] >= 2.0) {
							gs.gameplay.courseStageNow++;
							gs.procSelecter = 4;
						}
						else gs.procSelecter = 13;
					}
					break;
				case 6:
					ReadKeyConfig(&gs, fs::make_preferred("LR2files/Config/keyconfig.xml").data());
					break;
				case 7:
					ClearSkinGraph(&gs.skstruct2);
					ReadLR2SoundSet(&gs, gs.config.skin.skinFilePath[10], 0);
					break;
				case 13:
					if (gs.gameplay.replay.status == 1 && gs.gameplay.isAutoplay == 0) {
						if (gs.config.play.replay == 1) {
							if (MoveReplayFile(gs.sSelect.bmsList[gs.sSelect.cur_song].hash, gs.config.player.id) == 1)
								gs.sSelect.bmsList[gs.sSelect.cur_song].replayExist = 1;
						}
						else if (gs.config.play.replay == 2) {
							if (gs.sSelect.bmsList[gs.sSelect.cur_song].mybest.stat_exscore <= gs.gameplay.player[0].exscore && gs.gameplay.player[0].exscore > 0) {
								if (MoveReplayFile(gs.sSelect.bmsList[gs.sSelect.cur_song].hash, gs.config.player.id) == 1)
									gs.sSelect.bmsList[gs.sSelect.cur_song].replayExist = 1;
							}
						}
						else if (gs.config.play.replay == 3) {
							if (gs.gameplay.player[0].clearType > 1 && gs.gameplay.player[0].exscore > 0) {
								if (MoveReplayFile(gs.sSelect.bmsList[gs.sSelect.cur_song].hash, gs.config.player.id) == 1)
									gs.sSelect.bmsList[gs.sSelect.cur_song].replayExist = 1;
							}
						}
						else if (gs.config.play.replay == 4) {
							if (gs.gameplay.player[0].clearType == 5 && gs.gameplay.player[0].exscore > 0) {
								if (MoveReplayFile(gs.sSelect.bmsList[gs.sSelect.cur_song].hash, gs.config.player.id) == 1)
									gs.sSelect.bmsList[gs.sSelect.cur_song].replayExist = 1;
							}
						}
					}	
					gs.procSelecter = 2;
					StopSysSound(&gs);
					break;
				}
				InitInputStructure2(&gs.KeyInput);
				gs.procPhase = 0;
			}
		}
		GetTimeWrap();
		if (gs.procSelecter == 4 && gs.procPhase == 1 && GetTimeLapse(41,&gs.timer1) > 0 && gs.config.system.thread ==0){
			ProcGame(&gs);
		}
		GetTimeWrap();

		GetTimeWrap();
		for (int i = 0; i < gs.skstruct.image.srcSize; i++) {

			if (gs.skstruct.image.dst[i].dstCount && GetOptionFlag_dst(&gs, gs.skstruct.image.dst[i].opt1) 
				&& GetOptionFlag_dst(&gs, gs.skstruct.image.dst[i].opt2) && GetOptionFlag_dst(&gs, gs.skstruct.image.dst[i].opt3)) {

				if ( (gs.skstruct.adjust.dark_type == 1 && gs.skstruct.image.dst[i].timer)
					|| (gs.skstruct.adjust.dark_type != 1 && (gs.skstruct.adjust.dark_type != 2
															|| gs.skstruct.image.dst[i].timer == 2 
															|| gs.skstruct.image.dst[i].timer == 3
															|| gs.skstruct.image.dst[i].timer == 48
															|| gs.skstruct.image.dst[i].timer == 49
															|| (50 <= gs.skstruct.image.dst[i].timer && gs.skstruct.image.dst[i].timer < 70)
															|| gs.skstruct.image.dst[i].timer == 140))
					) {
					int objx = 0, objy = 0;
					if ((gs.skstruct.adjust.note_1p_x || gs.skstruct.adjust.note_1p_y || gs.skstruct.adjust.note_2p_x || gs.skstruct.adjust.note_2p_y) && gs.procSelecter == 4) {
						int t = gs.skstruct.image.dst[i].timer;
						//refactored
						if ((50 <= t && t < 60) || (70 <= t && t < 80) || t == 48) {
							objx = gs.skstruct.adjust.note_1p_x;
							objy = gs.skstruct.adjust.note_1p_y;
						}
						else if((60 <= t && t < 70) || (80 <= t && t < 90) || t == 49) {
							objx = gs.skstruct.adjust.note_2p_x;
							objy = gs.skstruct.adjust.note_2p_y;
						}
						else if ((100 <= t && t < 110) || (120 <= t && t < 130)) {
							objx = gs.skstruct.adjust.note_1p_x;
							objy = gs.skstruct.adjust.note_1p_y;
							if (-100.0 < gs.skstruct.image.dst[i].draw[gs.skstruct.image.dst[i].dstCount - 1].h && gs.skstruct.image.dst[i].draw[gs.skstruct.image.dst[i].dstCount - 1].h < 100.0
								&& -100.0 < gs.skstruct.image.dst[i].draw[0].h && gs.skstruct.image.dst[i].draw[0].h < 100.0) {

								objx = 0;
								objy = 0;
							}
						}
						else if ((110 <= t && t < 120) || (130 <= t && t < 140)) {
							objx = gs.skstruct.adjust.note_2p_x;
							objy = gs.skstruct.adjust.note_2p_y;
							if (-100.0 < gs.skstruct.image.dst[i].draw[gs.skstruct.image.dst[i].dstCount - 1].h && gs.skstruct.image.dst[i].draw[gs.skstruct.image.dst[i].dstCount - 1].h < 100.0
								&& -100.0 < gs.skstruct.image.dst[i].draw[0].h && gs.skstruct.image.dst[i].draw[0].h < 100.0) {

								objx = 0;
								objy = 0;
							}
						}
						else {
							objx = 0;
							objy = 0;
							if (gs.skstruct.dst_JUDGELINE[0].dstCount > 0) {
								if ( abs(gs.skstruct.dst_JUDGELINE[0].draw->w - gs.skstruct.image.dst[i].draw[gs.skstruct.image.dst[i].dstCount - 1].w) <= 10.0
									&& abs(gs.skstruct.dst_JUDGELINE[0].draw->x - gs.skstruct.image.dst[i].draw[gs.skstruct.image.dst[i].dstCount - 1].x) <= 5.0
									&& (gs.skstruct.dst_JUDGELINE[0].draw->y >= gs.skstruct.image.dst[i].draw[gs.skstruct.image.dst[i].dstCount - 1].y || gs.skstruct.image.dst[i].draw[gs.skstruct.image.dst[i].dstCount - 1].h < 0.0)) {

									objx = gs.skstruct.adjust.note_1p_x;
									objy = gs.skstruct.adjust.note_1p_y;
								}

								else if (gs.skstruct.dst_JUDGELINE[1].dstCount > 0) {
									if ( abs(gs.skstruct.dst_JUDGELINE[1].draw->w - gs.skstruct.image.dst[i].draw[gs.skstruct.image.dst[i].dstCount - 1].w) <= 10.0 
										&& abs(gs.skstruct.dst_JUDGELINE[1].draw->x - gs.skstruct.image.dst[i].draw[gs.skstruct.image.dst[i].dstCount - 1].x) <= 5.0
										&& (gs.skstruct.dst_JUDGELINE[1].draw->y >= gs.skstruct.image.dst[i].draw[gs.skstruct.image.dst[i].dstCount - 1].y || gs.skstruct.image.dst[i].draw[gs.skstruct.image.dst[i].dstCount - 1].h < 0.0)) {

										objx = gs.skstruct.adjust.note_2p_x;
										objy = gs.skstruct.adjust.note_2p_y;
									}
								}
							}
						}
					}

					if (gs.skstruct.image.dst[i].opt4 == 1) {
						AddDrawingBuffer_Scratch(&gs.skstruct.drBuf, &gs.skstruct.image.src[i], &gs.skstruct.image.dst[i], &gs.timer1, gs.skstruct.scratchAngle_1);
					}
					else if (gs.skstruct.image.dst[i].opt4 == 2) {
						AddDrawingBuffer_Scratch(&gs.skstruct.drBuf, &gs.skstruct.image.src[i], &gs.skstruct.image.dst[i], &gs.timer1, gs.skstruct.scratchAngle_2);
					}
					else {
						AddDrawingBuffer_Object(&gs.skstruct.drBuf, &gs.skstruct.image.src[i], &gs.skstruct.image.dst[i], &gs.timer1, objx, objy);
					}
				}
			}

		}

		if (gs.gameplay.courseType == 1) {
			if (gs.procSelecter == 4) {
				if (GetTimeLapse(41, &gs.timer1) >= 0.0 && gs.config.play.bga > 0) {
					int intTemp = 0;
					while (gs.gameplay.bgaMixer[intTemp] <= 0) {
						intTemp++;
						if (intTemp >= gs.sSelect.bmsList[gs.sSelect.cur_song].courseStageCount - 1) break;
					}

					if (GetRand(gs.gameplay.bgaMixer[intTemp+1] + gs.gameplay.bgaMixer[intTemp]) > gs.gameplay.bgaMixer[intTemp]) {
						gs.gameplay.missLayer = gs.gameplay.courseMissLayer[intTemp + 1];
						gs.gameplay.bgaLayer1 = gs.gameplay.courseBgaLayer1[intTemp + 1];
						gs.gameplay.bgaLayer2 = gs.gameplay.courseBgaLayer2[intTemp + 1];
					}
					else {
						gs.gameplay.missLayer = gs.gameplay.courseMissLayer[intTemp];
						gs.gameplay.bgaLayer1 = gs.gameplay.courseBgaLayer1[intTemp];
						gs.gameplay.bgaLayer2 = gs.gameplay.courseBgaLayer2[intTemp];
					}
				}
			}

		}

		if (gs.procSelecter == 4 || gs.is_starter) {
			if (GetTimeLapse(41, &gs.timer1) >= 0.0 && gs.config.play.bga > 0) {
				for (int i = 0; i < gs.skstruct.otherObject[4].srcSize; i++) {
					if (GetOptionFlag_dst(&gs, gs.skstruct.otherObject[4].dst[i].opt1) && GetOptionFlag_dst(&gs, gs.skstruct.otherObject[4].dst[i].opt2)
						&& GetOptionFlag_dst(&gs, gs.skstruct.otherObject[4].dst[i].opt3) && gs.skstruct.adjust.dark_type != 2) {

						if (gs.config.play.poorbga > GetTimeWrap() - gs.gameplay.lastMissTime && gs.gameplay.missLayer >= 0 && gs.config.play.bga != 3) {
							AddDrawingBuffer_BGA(&gs.skstruct.drBuf, &gs.skstruct.otherObject[4].src[i], &gs.skstruct.otherObject[4].dst[i], &gs.timer1, gs.gameplay.bgaHandle[gs.gameplay.missLayer], 0);
						}
						else {
							if (gs.gameplay.bgaLayer1 >= 0) 
								AddDrawingBuffer_BGA(&gs.skstruct.drBuf, &gs.skstruct.otherObject[4].src[i], &gs.skstruct.otherObject[4].dst[i], &gs.timer1, gs.gameplay.bgaHandle[gs.gameplay.bgaLayer1], 1);
							if(gs.gameplay.bgaLayer2 >= 0)
								AddDrawingBuffer_BGA(&gs.skstruct.drBuf, &gs.skstruct.otherObject[4].src[i], &gs.skstruct.otherObject[4].dst[i], &gs.timer1, gs.gameplay.bgaHandle[gs.gameplay.bgaLayer2], 0);
						}
					}
				}
			}
		}

		for (int i = 0; i < gs.skstruct.otherObject[1].srcSize; i++) {
			if (GetOptionFlag_dst(&gs, gs.skstruct.otherObject[1].dst[i].opt1) && GetOptionFlag_dst(&gs, gs.skstruct.otherObject[1].dst[i].opt2)
				&& GetOptionFlag_dst(&gs, gs.skstruct.otherObject[1].dst[i].opt3) && gs.skstruct.adjust.dark_type != 2) {

				AddDrawingBuffer_Image(&gs.skstruct.drBuf, &gs.skstruct.otherObject[1].src[i], &gs.skstruct.otherObject[1].dst[i], &gs.timer1);

			}
		}
		for (int i = 0; i < gs.skstruct.otherObject[2].srcSize; i++) {
			if (GetOptionFlag_dst(&gs, gs.skstruct.otherObject[2].dst[i].opt1) && GetOptionFlag_dst(&gs, gs.skstruct.otherObject[2].dst[i].opt2)
				&& GetOptionFlag_dst(&gs, gs.skstruct.otherObject[2].dst[i].opt3) && gs.skstruct.adjust.dark_type != 2) {

				AddDrawingBuffer_Slider(&gs.skstruct.drBuf, &gs.skstruct.otherObject[2].src[i], &gs.skstruct.otherObject[2].dst[i], &gs.timer1);

			}
		}
		for (int i = 0; i < gs.skstruct.otherObject[6].srcSize; i++) {
			if (GetOptionFlag_dst(&gs, gs.skstruct.otherObject[6].dst[i].opt1) && GetOptionFlag_dst(&gs, gs.skstruct.otherObject[6].dst[i].opt2)
				&& GetOptionFlag_dst(&gs, gs.skstruct.otherObject[6].dst[i].opt3) && gs.skstruct.adjust.dark_type != 2) {

				AddDrawingBuffer_Numbers(&gs.skstruct.drBuf, &gs.skstruct.otherObject[6].src[i], &gs.skstruct.otherObject[6].dst[i], &gs.timer1, SetObjectValue_Num(&gs, gs.skstruct.otherObject[6].src[i].op1), 0, 0);

			}
		}
		if (gs.txtStruct.readme.show != true) {
			for (int i = 0; i < gs.skstruct.otherObject[3].srcSize; i++) {
				if (GetOptionFlag_dst(&gs, gs.skstruct.otherObject[3].dst[i].opt1) && GetOptionFlag_dst(&gs, gs.skstruct.otherObject[3].dst[i].opt2)
					&& GetOptionFlag_dst(&gs, gs.skstruct.otherObject[3].dst[i].opt3) && gs.skstruct.adjust.dark_type != 2) {

					AddDrawingBuffer_OnMouse(&gs.skstruct.drBuf, &gs.skstruct.otherObject[3].src[i], &gs.skstruct.otherObject[3].dst[i], &gs.timer1, &gs.KeyInput, gs.sSelect.panel);

				}
			}
		}
		if (gs.skstruct.adjust.dark_type != 2) {
			Proc_Text(&gs, sql3, 0);
			if (gs.skstruct.adjust.dark_type != 2) {
				SetObjectValue_Bargraph(&gs);
			}
		}
		if(GetTimeWrap() < gs.KeyInput.mouse_recentMoveTime + 10000)
			AddDrawingBuffer_Object(&gs.skstruct.drBuf, &gs.skstruct.src_MOUSECURSOR, &gs.skstruct.dst_MOUSECURSOR, &gs.timer1, gs.KeyInput.mouse_oldX,	gs.KeyInput.mouse_oldY);
		else if(GetTimeWrap() < gs.KeyInput.mouse_recentMoveTime + 10500)
			AddDrawingBuffer_ObjectAlpha(&gs.skstruct.drBuf, &gs.skstruct.src_MOUSECURSOR, &gs.skstruct.dst_MOUSECURSOR, &gs.timer1, gs.KeyInput.mouse_oldX, gs.KeyInput.mouse_oldY,
				(((gs.KeyInput.mouse_recentMoveTime - GetTimeWrap()) + 10500) * 255) / 500);

		if (gs.procSelecter == 2) {
			int h = gs.txtStruct.readme.h;
			for (int i = 0; i < gs.txtStruct.readme.lines; i++) {
				int y = gs.skstruct.src_README[0].op1 * i + h;
				if ( y < 480 && (gs.skstruct.src_README[0].op1*(i + 1) + h > 0) ) {
					AddDrawingBuffer_TextXY(&gs.skstruct.drBuf, &gs.skstruct.src_README[0], &gs.skstruct.dst_README[0], &gs.timer1, i + 1000, gs.txtStruct.readme.w, y);
					AddDrawingBuffer_TextXY(&gs.skstruct.drBuf, &gs.skstruct.src_README[1],	&gs.skstruct.dst_README[1], &gs.timer1, i + 1000, gs.txtStruct.readme.w, gs.skstruct.src_README[1].op1 * i + h);
					h = gs.txtStruct.readme.h;
				}
			}
		}
		for (int i = 0; i < gs.skstruct.drBuf.count; i++) {
			int quake_x = 0, quake_y = 0;
			if (((gs.procPhase == 1) && (gs.procSelecter == 4)) && (0 < gs.config.play.m_earthquake)) {
				quake_x = (double)gs.gameplay.earthquake_x;
				quake_y = (double)gs.gameplay.earthquake_y;
			}
			LRDraw(&gs.skstruct.drBuf, &gs.txtStruct, &gs.sSelect, &gs.skstruct, i, quake_x, quake_y);
			if (gs.config.system.thread == 0 && gs.gameplay.flag_gameinput != 0) {
				//ProcGame(&gs); //why this is here
			}
		}
		//TEST
		if (gs.config.system.thread == 0 && gs.gameplay.flag_gameinput != 0) {
			ProcGame(&gs); //why this is here
		}
		//TEST END

		InitDrawingBuffer(&gs.skstruct.drBuf);
		GetTimeWrap();

		GetTimeWrap();
		if (gs.procSelecter == 4) {
			if (gs.KeyInput.inputID[KEY_INPUT_1] == 2) {
				printfDx("スキン位置の変更(カーソルキーで調節)\nx:%d\ny:%d\n", gs.skstruct.adjust.shift_x, gs.skstruct.adjust.shift_y);
			}
			else if (gs.KeyInput.inputID[KEY_INPUT_2] == 2) {
				printfDx("スキン拡大率の変更(カーソルキーで調節)\nx:%d\ny:%d\n", gs.skstruct.adjust.rate_x, gs.skstruct.adjust.rate_y);
			}
			else if (gs.KeyInput.inputID[KEY_INPUT_3] == 2) {
				printfDx("ジャッジ表示位置の変更(カーソルキーで調節)\nx:%d\ny:%d\n", gs.skstruct.adjust.judge_x, gs.skstruct.adjust.judge_y);
			}
			else if (gs.KeyInput.inputID[KEY_INPUT_4] == 2) {
				printfDx("ノートサイズの変更(カーソルキーで調節)\nx:%d\ny:%d\n", gs.skstruct.adjust.size_x, gs.skstruct.adjust.size_y);
			}
			else if (gs.KeyInput.inputID[KEY_INPUT_5] == 2) {
				if (gs.skstruct.adjust.dark_type == 1) {
					printfDx("スキン描画制限(カーソルキーで調節)\nDARK 1\n");
				}
				else if (gs.skstruct.adjust.dark_type == 2) {
					printfDx("スキン描画制限(カーソルキーで調節)\nDARK 2\n");
				}
				else {
					printfDx("スキン描画制限(カーソルキーで調節)\nOFF\n");
				}
			}
			else if (gs.KeyInput.inputID[KEY_INPUT_6] == '\x02') {
				printfDx("ノート位置(1P)の変更(カーソルキーで調節)\nx:%d\ny:%d\n", gs.skstruct.adjust.note_1p_x, gs.skstruct.adjust.note_1p_y);
			}
			else if (gs.KeyInput.inputID[KEY_INPUT_7] == '\x02'){
				printfDx("ノート位置(2P)の変更(カーソルキーで調節)\nx:%d\ny:%d\n", gs.skstruct.adjust.note_2p_x, gs.skstruct.adjust.note_2p_y);
			}
		}
		if ( gs.KeyInput.inputID[KEY_INPUT_F1] == '\x02' && gs.sSelect.flag_maniacPanel == '\0' && gs.is_starter == '\0') {
			printfDx( (gs.sSelect.bmsList[gs.sSelect.cur_song].folderType == 8) ?
						"F2 マニアックオプション F3 コースのソート変更\nF4 ウインドウモード切り替え F5 IRに接続\nF6  スクリーンショット F7 FPS表示\nF8 フォルダのリロード\n" 
						: "F2 マニアックオプション F3 レベルの変更\nF4 ウインドウモード切り替え F5 IRに接続\nF6 スク リーンショット F7 FPS表示\nF8 フォルダのリロード\n");
		}
		if (gs.flag_showFPS) {
			printfDx("FPS %d\n", (int)gs.timer1.FPS);
			//TEST
			printfDx("maxGAP %.3f\n", gs.timer1.maxGAP);
			printfDx("avgGAP %.3f\n", gs.timer1.avgOnlyGAP);
			printfDx("GAP ticks %d / %d\n", gs.timer1.GAPcount, gs.timer1.GAPtick);
#ifdef _WIN32
			printfDx("%s ", GetUseDirect3DVersion() == 3? "DX11" : "DX9"); //none:0 DX_DIRECT3D_9:1 9EX:2 11:3 default 2? //DEBUG
#endif // _WIN32
			printfDx("%s ", gs.config.system.screenmode? "windowed":"fullscreen");
			if (GetWaitVSyncFlag()) SetWaitVSyncFlag(0); //TEST
			printfDx("%s\n", DxLib::GetWaitVSyncFlag() ? "Vsync" : "");
			int dx, dy;
			GetDrawScreenSize(&dx, &dy);
			GetWindowSize(&screenSizeX, &screenSizeY);
			printfDx("skin %d %d >> scrn %d %d\n", dx, dy, screenSizeX, screenSizeY);
			//TEST END
		}
		gs.sSelect.flag_maniacPanel = 0;
		if(gs.procSelecter == 2){
			if ( (gs.KeyInput.inputID[KEY_INPUT_F5] == 1 || gs.sSelect.is_buttonIRpage != 0) && gs.sSelect.bmsList[gs.sSelect.cur_song].keymode > 4 && gs.config.network.lr2ir == 1) {
				if (gs.config.system.screenmode == 0) {
					gs.config.system.screenmode = 1;
					SetObjectStrings_SongSelect(&gs);
					for (int i = 0; i < 200; i++) {
						gs.skstruct.caption[i].fillzero();
					}
					for (int i = 0; i < 10; i++) {
						gs.skstruct.ImageFonts[i].filepath[0] = 0;
					}
					SetGraphMode(640, 480, (gs.config.system.highcolor == 0 ? 32 : 16), 60); //TODO_RESOULUTION
					SetWaitVSyncFlag(0); //VSYNC
#ifdef _WIN32
					ChangeWindowMode(gs.config.system.screenmode);
#endif // _WIN32
					SetWaitVSyncFlag(0); //VSYNC
					SetDrawScreen(DX_SCREEN_BACK);
					LoadSceneG(&gs, &gs.skstruct, SKINTYPE_SELECT);
					SetMouseDispFlag(0);
					gs.is_clicked_screenModeChange = 0;
#ifdef _WIN32
					if (gs.config.system.screenmode == 0) {
						ChangeWindowMode(1);
						ErrorLogAdd("ウインドウを閉じます\n");
						CloseWindow(GetMainWindowHandle());
						ErrorLogAdd("成功\n");
					}
#endif // _WIN32
					if (gs.config.network.lr2ir == 1) {
						//same as below
						ErrorLogAdd("IRを出します\n");
						OpenWebRanking(gs.sSelect.bmsList[gs.sSelect.cur_song].hash);
					}
				}
				else {
					ErrorLogAdd("IRを出します\n");
					OpenWebRanking(gs.sSelect.bmsList[gs.sSelect.cur_song].hash);
				}

				if (gs.config.system.screenmode == 0) {
					ErrorLogAdd("アイコン化が終わるまで待ちます\n");
					while (ProcessMessage() == 0) {
#ifdef _WIN32
						if (IsIconic(GetMainWindowHandle()) == 0) break;
#endif // _WIN32
						std::this_thread::sleep_for(std::chrono::milliseconds(16));
					}
					SetObjectStrings_SongSelect(&gs);
					for (int i = 0; i < 200; i++) {
						gs.skstruct.caption[i].fillzero();
					}
					for (int i = 0; i < 10; i++) {
						gs.skstruct.ImageFonts[i].filepath[0] = 0;
					}
					SetGraphMode(640, 480, (gs.config.system.highcolor == 0 ? 32 : 16), 60); //TODO_RESOULUTION
					SetWaitVSyncFlag(0); //VSYNC
#ifdef _WIN32
					ChangeWindowMode(gs.config.system.screenmode);
#endif // _WIN32
					SetWaitVSyncFlag(0); //VSYNC
					SetDrawScreen(DX_SCREEN_BACK);
					LoadSceneG(&gs, &gs.skstruct, SKINTYPE_SELECT);
					SetMouseDispFlag(0);
					gs.is_clicked_screenModeChange = 0;
				}
				gs.KeyInput.inputID[KEY_INPUT_F1] = 0; //why F1?
				gs.sSelect.is_buttonIRpage = 0;
				InitInputStructure2(&gs.KeyInput);
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				if (gs.sSelect.flag_maniacPanel) ClsDrawScreen();
			}
			else if (gs.KeyInput.inputID[KEY_INPUT_F2] == 2) {
				gs.sSelect.flag_maniacPanel = 1;
				Print_ManiacOptions(&gs);
				if (gs.sSelect.flag_maniacPanel) ClsDrawScreen();
			}
			else if (gs.KeyInput.inputID[KEY_INPUT_F3] == 2) {
				if (gs.sSelect.bmsList[gs.sSelect.cur_song].folderType == 8) {
					printfDx("カーソルキー↑↓ コースの表示順変更\n");
					if (gs.KeyInput.inputID[KEY_INPUT_UP] == 1) {
						if (gs.sSelect.cur_song > -1) {
							ChangeCourseID(sql3, gs.sSelect.bmsList[gs.sSelect.cur_song].courseID, gs.sSelect.bmsList[gs.sSelect.cur_song + -1].courseID, gs.sSelect.course.type);
							SetBmsFilter(&gs, sql3);
							gs.sSelect.selKey = gs.sSelect.bmsList[gs.sSelect.cur_song + -1].courseID;
							gs.sSelect.filter_clicked = 10;
						}
					}
					else if (gs.KeyInput.inputID[KEY_INPUT_DOWN] == 1 && gs.sSelect.cur_song < gs.sSelect.bmsListCount + -1 && gs.sSelect.bmsList[gs.sSelect.cur_song + 1].courseID > 0) {
						ChangeCourseID(sql3, gs.sSelect.bmsList[gs.sSelect.cur_song].courseID, gs.sSelect.bmsList[gs.sSelect.cur_song + 1].courseID, gs.sSelect.course.type);
						SetBmsFilter(&gs, sql3);
						gs.sSelect.selKey = gs.sSelect.bmsList[gs.sSelect.cur_song + 1].courseID;
						gs.sSelect.filter_clicked = 10;
					}
				}
				else {
					if (gs.config.select.disabledifficultyfilter == 0) printfDx("カーソルキー↑↓ 難度カテゴリの変更\nカーソルキー←→ レベルの変更\n");
					else printfDx("カーソルキー←→ レベルの変更\n");

					if (gs.KeyInput.inputID[KEY_INPUT_UP] == 1 && gs.config.select.disabledifficultyfilter == 0) {
						gs.sSelect.bmsList[gs.sSelect.cur_song].difficulty--;

						if (gs.sSelect.bmsList[gs.sSelect.cur_song].difficulty >= 6)
							gs.sSelect.bmsList[gs.sSelect.cur_song].difficulty = 1;
						else if (gs.sSelect.bmsList[gs.sSelect.cur_song].difficulty < 1)
							gs.sSelect.bmsList[gs.sSelect.cur_song].difficulty = 5;

						gs.sSelect.is_tag_edited = 1;
						gs.sSelect.is_clicked_filter = 1;

						if (gs.sSelect.bmsList[gs.sSelect.cur_song].difficulty != gs.config.select.difficulty && gs.config.select.difficulty)
							gs.config.select.difficulty = gs.sSelect.bmsList[gs.sSelect.cur_song].difficulty;
					}
					else if (gs.KeyInput.inputID[KEY_INPUT_DOWN] == 1 && gs.config.select.disabledifficultyfilter == 0) {
						gs.sSelect.bmsList[gs.sSelect.cur_song].difficulty++;

						if (gs.sSelect.bmsList[gs.sSelect.cur_song].difficulty >= 6)
							gs.sSelect.bmsList[gs.sSelect.cur_song].difficulty = 1;
						else if (gs.sSelect.bmsList[gs.sSelect.cur_song].difficulty < 1)
							gs.sSelect.bmsList[gs.sSelect.cur_song].difficulty = 5;

						gs.sSelect.is_tag_edited = 1;
						gs.sSelect.is_clicked_filter = 1;

						if (gs.sSelect.bmsList[gs.sSelect.cur_song].difficulty != gs.config.select.difficulty && gs.config.select.difficulty)
							gs.config.select.difficulty = gs.sSelect.bmsList[gs.sSelect.cur_song].difficulty;
					}
					else {
						if (gs.KeyInput.inputID[KEY_INPUT_LEFT] == 1) {
							gs.sSelect.bmsList[gs.sSelect.cur_song].level--;

							if (gs.sSelect.bmsList[gs.sSelect.cur_song].level >= 100)
								gs.sSelect.bmsList[gs.sSelect.cur_song].level = 0;
							else if (gs.sSelect.bmsList[gs.sSelect.cur_song].level < 0)
								gs.sSelect.bmsList[gs.sSelect.cur_song].level = 99;

							gs.sSelect.is_tag_edited = 1;
							gs.sSelect.is_clicked_filter = 1;
						}
						else if(gs.KeyInput.inputID[KEY_INPUT_RIGHT] == 1){
							gs.sSelect.bmsList[gs.sSelect.cur_song].level++;

							if (gs.sSelect.bmsList[gs.sSelect.cur_song].level >= 100)
								gs.sSelect.bmsList[gs.sSelect.cur_song].level = 0;
							else if (gs.sSelect.bmsList[gs.sSelect.cur_song].level < 0)
								gs.sSelect.bmsList[gs.sSelect.cur_song].level = 99;

							gs.sSelect.is_tag_edited = 1;
							gs.sSelect.is_clicked_filter = 1;
						}
					}
				}
				if (gs.sSelect.flag_maniacPanel) ClsDrawScreen();
			}
		}
		GetTimeWrap();

		GetTimeWrap();
		if (gs.isSkipDrawTick == 0) {
			if (gs.gameplay.flag_gameinput != 0 && gs.config.system.thread == 0 && gs.config.system.vsync == 1 && gs.is_recordmode == 0) {
				//TODO : Get appropriate device
				double a = DxLib::GetRefreshRate();
				double m_lMillisecPerFrame = 1000 / a - 1.0;

				while (GetTimeWrap() - gs.timer1.vSyncTick < m_lMillisecPerFrame) {
					ProcGame(&gs);
				}
			}
			gs.timer1.vSyncTick = GetTimeWrap();
			if (gs.flag_Screenshot) {
				gs.flag_Screenshot = false;
				DATEDATA date;
				GetDateTime(&date);
				CSTR captureFilename;
				cstrSprintf(&captureFilename, "screenshot/LR2 %04d-%02d-%02d %02d-%02d-%02d.png", date.Year, date.Mon, date.Day, date.Hour, date.Min, date.Sec);
				SaveDrawScreenToPNG(0, 0, skinSizeX, skinSizeY, captureFilename, -1);
				PlaySound(&gs.audio, &gs.audio.sysSound.screenshot, gs.audio.chnKey, -1);
			}

			if (gs.is_recordmode) {
				if (gs.rec.recMode == 3) {
					if (gs.gameplay.bgaLayer1 >= 0)
						DrawBGA(gs.gameplay.bgaHandle[gs.gameplay.bgaLayer1]);
					if (gs.gameplay.bgaLayer2 >= 0)
						DrawBGA(gs.gameplay.bgaHandle[gs.gameplay.bgaLayer2]);
				}
				if (gs.audio.replay2avi) {
					gs.audio.aviTimer = GetTimeWrap();
				}
				gs.rec.CpyScreenToAVI();
				if (gs.timer1.flagMovieTimer) {
					double time1, time2;
					time1 = GetTimeWrap();//
					MovieTimer(&gs.timer1);
					gs.audio.aviTimer = GetTimeWrap();
					if(gs.audio.replay2avi)
						gs.audio.aviTimer = GetTimeWrap();
					time2 = GetTimeWrap();
					if (gs.gameplay.flag_gameinput) {

						while (time2 - 1.0 < time1) {
							ProcGame(&gs);
							SetManualTimer(&gs.timer1, time1);
							gs.audio.aviTimer = time1;
							time1 += 1.0;
						}
						SetManualTimer(&gs.timer1, time2);
						if (gs.audio.replay2avi) {
							gs.audio.aviTimer = time2;
						}
					}
				}
				if (gs.gameplay.courseType == 1) {
					for (int i = 0; i < 10; i++) {
						if (gs.gameplay.courseBgaLayer1[i] > 0) {
							SeekMovieToGraph(gs.gameplay.bgaHandle[gs.gameplay.courseBgaLayer1[i]], GetTimeWrap() - gs.gameplay.courseLayer1ChangeTime[i]);
							PlayMovieToGraph(gs.gameplay.bgaHandle[gs.gameplay.courseBgaLayer1[i]], 1, 0);
						}
						if (gs.gameplay.courseBgaLayer2[i] > 0) {
							SeekMovieToGraph(gs.gameplay.bgaHandle[gs.gameplay.courseBgaLayer2[i]], GetTimeWrap() - gs.gameplay.courseLayer2ChangeTime[i]);
							PlayMovieToGraph(gs.gameplay.bgaHandle[gs.gameplay.courseBgaLayer2[i]], 1, 0);
						}
					}
				}
				else {
					if (1 <= gs.gameplay.bgaLayer1 && gs.gameplay.bgaLayer1 < 6479) {
						SeekMovieToGraph(gs.gameplay.bgaHandle[gs.gameplay.bgaLayer1], GetTimeWrap() - gs.gameplay.layer1ChangeTime);
						PlayMovieToGraph(gs.gameplay.bgaHandle[gs.gameplay.bgaLayer1], 1, 0);
					}
					if (1 <= gs.gameplay.bgaLayer2 && gs.gameplay.bgaLayer2 < 6479) {
						SeekMovieToGraph(gs.gameplay.bgaHandle[gs.gameplay.bgaLayer2], GetTimeWrap() - gs.gameplay.layer2ChangeTime);
						PlayMovieToGraph(gs.gameplay.bgaHandle[gs.gameplay.bgaLayer2], 1, 0);
					}
				}
			}


			if (GetWaitVSyncFlag()) SetWaitVSyncFlag(0); //TEST

			ScreenFlip(); //DXlib Vsync works on here
			GetTimeWrap();

			GetTimeWrap();
			clsDx();
			CalcFPS(&gs.timer1);
			gs.timer1.tickTime = GetTimeWrap() - gs.timer1.gameTick;
			gs.timer1.gameTick = GetTimeWrap();
			gs.timer2.tickTime = gs.timer1.tickTime;
			gs.timer2.gameTick = gs.timer1.gameTick;
		}
		else gs.isSkipDrawTick = 0;
		ClsDrawScreen();
		GetTimeWrap();

		GetTimeWrap();
		if (gs.gameplay.flag_gameinput == 0 || gs.gameplay.isPreviewLoad) {
			ReactInput(&gs);
		}
		GetTimeWrap();

		GetTimeWrap();
		if (gs.sSelect.is_clicked_tagedit) {
			EditTag(&gs.sSelect.bmsList[gs.sSelect.cur_song],sql3);
			gs.sSelect.is_clicked_tagedit = 0;
			ProcS_Select(&gs);
			gs.sSelect.is_filter_changed = true;
			gs.sSelect.filter_clicked = 4;
			SetObjectStrings_SongSelect(&gs);
		}
		if (gs.sSelect.is_tag_edited) {
			if (gs.sSelect.bmsList[gs.sSelect.cur_song].folderType < 3) {
				UpdateSongDataTag(&gs.sSelect.bmsList[gs.sSelect.cur_song], sql3);
			}
			gs.sSelect.is_tag_edited = 0;
			ProcS_Select(&gs);
			if (gs.sSelect.bmsList[gs.sSelect.cur_song].favorite != 2) {
				gs.sSelect.is_filter_changed = true;
				gs.sSelect.filter_clicked = 4;
			}
			SetObjectStrings_SongSelect(&gs);
		}
		if (gs.KeyInput.inputID[KEY_INPUT_F4] == 1 && gs.procSelecter == 2) {
			LoopInRange(0, 1, 1, &gs.config.system.screenmode);
			gs.is_clicked_screenModeChange = 1;
		}
		if (gs.sSelect.is_filter_changed) {
			SetBmsFilter(&gs, sql3);
			gs.sSelect.is_filter_changed = false;
		}
		if (gs.is_clicked_screenModeChange == 1) {
			for (int i = 0; i < 200; i++) {
				gs.skstruct.caption[i].fillzero();
			}
			for (int i = 0; i < 10; i++) {
				gs.skstruct.ImageFonts[i].filepath[0] = 0;
			}
			SetGraphMode(640, 480, (gs.config.system.highcolor == 0 ? 32 : 16), 60); //TODO_RESOULUTION
			SetWaitVSyncFlag(0); //VSYNC
#ifdef _WIN32
			ChangeWindowMode(gs.config.system.screenmode);
#endif // _WIN32
			SetWaitVSyncFlag(0); //VSYNC
			SetDrawScreen(DX_SCREEN_BACK);
			for (int i = 0; i < 900; i++) {
				gs.skstruct.op[i] = GetOptionFlag_dst(&gs, i);
				gs.skstruct2.op[i] = GetOptionFlag_dst(&gs, i);
			}
			for (int i = 900; i < 1000; i++) {
				gs.skstruct.op[i] = 0;
				gs.skstruct2.op[i] = 0;
			}
			LoadSceneG(&gs, &gs.skstruct, SKINTYPE_SELECT);
			SetWaitVSyncFlag(0); //VSYNC
			SetMouseDispFlag(0);
			gs.is_clicked_screenModeChange = 0;
			SetObjectStrings_SongSelect(&gs);
		}
#ifdef _WIN32
		else if(GetWindowModeFlag() != gs.config.system.screenmode){
			for (int i = 0; i < 200; i++) {
				gs.skstruct.caption[i].fillzero();
			}
			for (int i = 0; i < 10; i++) {
				gs.skstruct.ImageFonts[i].filepath[0] = 0;
			}
			SetGraphMode(640, 480, (gs.config.system.highcolor == 0 ? 32 : 16), 60); //TODO_RESOULUTION
			SetDrawScreen(DX_SCREEN_BACK);
			LoadSceneG(&gs, &gs.skstruct, SKINTYPE_SELECT);
			SetWaitVSyncFlag(0); //VSYNC
			SetMouseDispFlag(0);
			gs.is_clicked_screenModeChange = 0;
			gs.config.system.screenmode = GetWindowModeFlag();
			SetObjectStrings_SongSelect(&gs);
		}
#endif // _WIN32
		SetMouseDispFlag( (gs.KeyInput.mouse_oldX < skinSizeX && gs.KeyInput.mouse_oldY < skinSizeY) ? 0:1  ); //TODO_RESOULUTION
		if ( (gs.procSelecter == 2 || gs.procSelecter == 9) && gs.KeyInput.inputID[KEY_INPUT_ESCAPE]
				&& (GetTimeLapse(4,&gs.timer1) < 0.0 || GetTimeLapse(4, &gs.timer1) > 100.0) 
				&& gs.txtStruct.st_text_num == -1 
				&& GetTimeLapse(0, &gs.timer1) > gs.skstruct.startinput_start ){
			gs.procSelecter = 0;
			ErrorLogAdd("選曲中ESCが押されました。LR2を終了します\n");
		}
	}
	//main loop end

	//phase_exit game
	if (gs.is_recordmode) {
		RecordBmsSound(&gs, gs.directoryFilename);
		remove("LR2files/movie_temp.mp3");
		remove("LR2files/movie_temp.wav");
	}
	gs.net.WaitAndInitRanking();
	gs.gameplay.flag_closingPhase = 1;
	gs.gameplay.isPreviewLoad = 0;
	gs.gameplay.flag_gameinput = 0;
	for (int i = 0; i < SLOTS; i++) {
		StopSound(&gs.audio, &gs.gameplay.keysound[i]);
	}
	gs.procPhase = 3;
	gs.procSelecter = 0;
	gs.gameplay.previewStatus = 0;
	gs.net.WS_clean();
	gs.gameplay.flag_closingPhase = 1;
	double threadExitTimer = GetTimeWrap();
	while (gs.gameplay.flag_threadExist) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		if (GetTimeWrap() - threadExitTimer > 5000.0) break;
	}
	DxLib_End();
	SQL_Run("COMMIT", sql3);
	SQL_Run("DETACH rivaldb", sql3);
	sqlite3_close(sql3);

	gs.config.sound.volumeflag = gs.audio.param.fx_volume_on;
	gs.config.sound.volumebgm = gs.audio.param.volume_BGM;
	gs.config.sound.volumekey = gs.audio.param.volume_key;
	gs.config.sound.volumemaster = gs.audio.param.volume_master;
	gs.config.sound.eqflag = gs.audio.param.eq_on;
	gs.config.sound.eqp0 = gs.audio.param.eq_gain[0];
	gs.config.sound.eqp1 = gs.audio.param.eq_gain[1];
	gs.config.sound.eqp2 = gs.audio.param.eq_gain[2];
	gs.config.sound.eqp3 = gs.audio.param.eq_gain[3];
	gs.config.sound.eqp4 = gs.audio.param.eq_gain[4];
	gs.config.sound.eqp5 = gs.audio.param.eq_gain[5];
	gs.config.sound.eqp6 = gs.audio.param.eq_gain[6];
	gs.config.sound.pitchp = gs.audio.param.pitch_amount;
	gs.config.sound.pitchflag = gs.audio.param.pitch_on;
	gs.config.sound.pitchtype = gs.audio.param.pitch_type;
	gs.config.sound.fxflag_0 = gs.audio.param.fx_on[0];
	gs.config.sound.fxtype_0 = gs.audio.param.fxType[0];
	gs.config.sound.fxtarget_0 = gs.audio.param.fxChannel[0];
	gs.config.sound.fxp1_0 = gs.audio.param.fxParam[0][0];
	gs.config.sound.fxp2_0 = gs.audio.param.fxParam[0][1];
	gs.config.sound.fxflag_1 = gs.audio.param.fx_on[1];
	gs.config.sound.fxtype_1 = gs.audio.param.fxType[1];
	gs.config.sound.fxtarget_1 = gs.audio.param.fxChannel[1];
	gs.config.sound.fxp1_1 = gs.audio.param.fxParam[1][0];
	gs.config.sound.fxp2_1 = gs.audio.param.fxParam[1][1];
	gs.config.sound.fxflag_2 = gs.audio.param.fx_on[2];
	gs.config.sound.fxtype_2 = gs.audio.param.fxType[2];
	gs.config.sound.fxtarget_2 = gs.audio.param.fxChannel[2];
	gs.config.sound.fxp1_2 = gs.audio.param.fxParam[2][0];
	gs.config.sound.fxp2_2 = gs.audio.param.fxParam[2][1];
	gs.config.network.lr1ir = lr1ir;
	gs.config.network.lr2ir = lr2ir;
	if (gs.auto2avi == 0 && gs.is_recordmode == 0 && gs.rec.recMode == 0) {
		WriteConfigXml(&gs, fs::make_preferred("LR2files/Config/config.xml").data());
		WriteOpenLr2ConfigXml(&gs, fs::make_preferred("LR2files/Config/openlr2-config.xml").data());
		WriteMidiXml(&gs, fs::make_preferred("LR2files/Config/midi.xml").data());
	}
	CloseMIDI();
	for (int i = 0; i < SLOTS; i++) {
		StopSound(&gs.audio, &gs.gameplay.keysound[i]);
	}
	for (int i = 0; i < SLOTS; i++) {
		StopSound(&gs.audio, &gs.gameplay.keysound[i]);
		ReleaseSound(&gs.audio, &gs.gameplay.keysound[i]);
	}
	ErrorLogAdd("BMSの音を初期化しました\n");
	StopSysSound(&gs);
	ReleaseSysSound(&gs);
	EndSound(&gs.audio);
	return 0;
	//phase_exit game end
}
