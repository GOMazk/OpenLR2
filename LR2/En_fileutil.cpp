#include "En_fileutil.h"
#include <md5.h>
#include <DxLib.h>

#ifdef _WIN32
#include <codecvt>
#else
#include <cassert>
#include <chrono>
#include <filesystem>

#include <sys/stat.h>
#include <iconv.h>
#endif // _WIN32

#ifdef _WIN32

static std::string convert(std::string_view str, int from, int to) {
	int wide_buf_size = MultiByteToWideChar(from, 0, str.data(), static_cast<int>(str.size()), nullptr, 0);
	auto wstr = std::make_unique_for_overwrite<wchar_t[]>(wide_buf_size);
	MultiByteToWideChar(from, 0, str.data(), static_cast<int>(str.size()), wstr.get(), wide_buf_size);

	int narrow_buf_size = WideCharToMultiByte(to, 0, wstr.get(), wide_buf_size, nullptr, 0, nullptr, nullptr);
	auto lstr = std::make_unique_for_overwrite<char[]>(narrow_buf_size);
	WideCharToMultiByte(to, 0, wstr.get(), wide_buf_size, lstr.get(), narrow_buf_size, nullptr, nullptr);

	std::string out;
	// NOTE: narrow_buf_size doesn't include null-terminator as we are passing size to WideCharToMultiByte explicitly.
	out.assign(lstr.get(), narrow_buf_size);
	return out;
}

std::string utf2ansi(const std::string_view str, unsigned int codepage) {
	return convert(str, CP_UTF8, codepage);
}

std::string ansi2utf(const std::string_view str, unsigned int codepage) {
	return convert(str, codepage, CP_UTF8);
}

#else

struct IcdDeleter {
	void operator()(iconv_t icd) {
		int ret = iconv_close(icd);
		if (ret == -1) {
			const int error = errno;
			// LOG_ERROR << "[Encoding] iconv_close() error: " << safe_strerror(error) << " (" << error << ")";
		}
	}
};
using IcdPtr = std::unique_ptr<std::remove_pointer_t<iconv_t>, IcdDeleter>;

template <typename T = char>
static void convert(std::string_view input, std::basic_string<T>& out, const char* from, const char* to)
{
	if (input.empty()) {
		out.clear();
		return;
	}

	auto icd = IcdPtr(iconv_open(to, from));
	assert(icd != nullptr);
	// > The following error can occur, among others:
	// > EINVAL The conversion from fromcode to tocode is not supported by the
	// implementation. Don't bother with such implementations.
	assert(reinterpret_cast<size_t>(icd.get()) != static_cast<size_t>(-1));

	// PERF: this buffer is MASSIVE. Don't initialize it or memset will dominate runtime.
	std::array<T, 1024L * 32L / sizeof(T)> out_buf;

	auto* buf_ptr = const_cast<char*>(input.data()); // SAFETY: iconv *shouldn't* modify the buffer. *shouldn't*.
	size_t buf_len = input.length();
	auto* out_ptr = reinterpret_cast<char*>(out_buf.data()); // SAFETY: using 'char' to read bytes is always safe.
	size_t out_len = sizeof(out_buf);
	const size_t initial_out_len = out_len;

	size_t iconv_ret = iconv(icd.get(), &buf_ptr, &buf_len, &out_ptr, &out_len);
	if (iconv_ret == static_cast<size_t>(-1))
	{
		const int error = errno;
		// LOG_ERROR << "[Encoding] iconv() error: " << safe_strerror(error) << " (" << error << ")";
		out.clear();
		return;
	}
	const size_t bytes_written = initial_out_len - out_len;

	assert(bytes_written % sizeof(T) == 0);
	out.assign(out_buf.data(), bytes_written / sizeof(T));

	// "In each series of calls to iconv(), the last should be one with inbuf or *inbuf equal to NULL, in order to flush
	// out any partially converted input".
	iconv_ret = iconv(icd.get(), nullptr, nullptr, nullptr, nullptr);
	if (iconv_ret == static_cast<size_t>(-1))
	{
		const int error = errno;
		// LOG_ERROR << "[Encoding] iconv() error: " << safe_strerror(error) << " (" << error << ")";
		out.clear();
	}
}

std::string ansi2utf(const std::string_view str, unsigned int codepage) {
	if (codepage != 932) {
		assert(false && "yep we ain't using the type system here lol");
		return std::string{str};
	}
	std::string out;
	convert(str, out, "CP932", "UTF-8");
	return out;
}

std::string utf2ansi(const std::string_view str, unsigned int codepage) {
	if (codepage != 932) {
		assert(false && "yep we ain't using the type system here lol");
		return std::string{str};
	}
	std::string out;
	convert(str, out, "UTF-8", "CP932");
	return out;
}

#endif // _WIN32

std::u32string utf8_to_utf32(const std::string_view str) {
	std::u32string out;
	// FIXME: std::use_facet is already deprecated and removed in C++26, and is not supported well in Wine.
	static const auto locale = std::locale("ja_JP.UTF8");
	static const auto& facet_u32_u8 = std::use_facet<std::codecvt<char32_t, char, std::mbstate_t>>(locale);
	out.resize(str.size() * facet_u32_u8.max_length(), '\0');

	std::mbstate_t s;
	const char* from_next = str.data();
	char32_t* to_next = out.data();

	std::codecvt_base::result res;
	do
	{
		res = facet_u32_u8.in(s, from_next, &str.data()[str.size()], from_next, to_next, &out[out.size()], to_next);

		// skip unconvertiable chars (which is impossible though)
		if (res == std::codecvt_base::error)
			from_next++;

	} while (res == std::codecvt_base::error);

	out.resize(to_next - &out[0]);
	return out;
}

int makeFileHash(LPCSTR filepath, LPCSTR oBuf) {
	FILE* pFile;
	pFile = fopen(filepath, "rb");
	if (!pFile)  return -1;
	unsigned char* md5buf = (unsigned char*)md5File(pFile);
	fclose(pFile);

	sprintf((char*)oBuf, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", md5buf[0], md5buf[1], md5buf[2], md5buf[3],
		md5buf[4], md5buf[5], md5buf[6], md5buf[7], md5buf[8], md5buf[9], md5buf[10], md5buf[11], md5buf[12], md5buf[13], md5buf[14], md5buf[15]);

	free(md5buf);

	return 1;
}

//TODO : posix 2038y problem
// Seconds since the Unix Epoch
time_t GetNowUnixtime() {
#ifdef _WIN32
	SYSTEMTIME systime;
	GetSystemTime((LPSYSTEMTIME)&systime);

	_FILETIME filetime;
	SystemTimeToFileTime(&systime, &filetime);

	return GetUnixtimeFromFiletime(filetime);
#else
	return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch())
		.count();
#endif // _WIN32
}

// Seconds since the Unix Epoch
time_t GetUnixtimeFromFiletime(FILETIME &filetime) {
	ULARGE_INTEGER ul{ filetime.dwLowDateTime, filetime.dwHighDateTime };
	return (unsigned int)((ul.QuadPart - 116444736000000000ULL) / 10000000);
}

// Seconds since the Unix Epoch
time_t GetFileUnixtime(CSTR str) {
	if (str.ends_with('\\') ||  str.ends_with('/')) {
		str.nullAtPos(str.length() - 1);
	}

#ifdef _WIN32
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFindFile = FindFirstFileA(str, &FindFileData);
	if (hFindFile == (HANDLE)-1) {
		ErrorLogFmtAdd("ファイルのLR2TIME取得エラー:%sが見つからない\n", str.body);
		return -1;
	}

	FindClose(hFindFile);
	return GetUnixtimeFromFiletime(FindFileData.ftLastWriteTime);
#else
	// FindFirstFile could also take in wildcards, but that sounds plain wrong.
	// Could consolidate both branches with std::filesystem::last_write_time();
	struct stat sb;
	int ret = stat(str.body, &sb);
	if (ret != 0) {
		ErrorLogFmtAdd("ファイルのLR2TIME取得エラー:%sが見つからない\n", str.body);
		return -1;
	}
	return static_cast<time_t>(sb.st_mtim.tv_sec);
#endif
}

CSTR GetRandomFileOnDir(CSTR path, char fOnlyName) {
#ifdef _WIN32
	CSTR oBuf;
	//CSTR str1,str2,str3;
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFindFile;
	int fileCount = 0;
	CSTR str1( path.left(path.findStrPos("*")) );
	CSTR str2( path.right(path.length() - str1.length() - 1) );
	CSTR str3( str1 );
	str3.add("*");
	hFindFile = FindFirstFileA(str3, &FindFileData);
	if (hFindFile == (HANDLE)-1) {
		return CSTR("ERROR");
	}
	do {
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (strcmp("..", FindFileData.cFileName) && strcmp(".", FindFileData.cFileName)) fileCount++;
		}
	} while (FindNextFileA(hFindFile, &FindFileData));
	FindClose(hFindFile);
	if (fileCount > 0) {
		fileCount = GetRand(fileCount - 1);

		hFindFile = FindFirstFileA(str3, &FindFileData);
		if (hFindFile != (HANDLE)-1) {
			do {
				if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					if (strcmp("..", FindFileData.cFileName) && strcmp(".", FindFileData.cFileName)) {
						int i = 0;
						while (i < fileCount) {
							FindNextFileA(hFindFile, &FindFileData);
							if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
								if (strcmp("..", FindFileData.cFileName) && strcmp(".", FindFileData.cFileName)) i++;
							}
						}
						FindClose(hFindFile);
						path.assign(&str1);
						path.add(FindFileData.cFileName);
						path.add(&str2);
						if (fOnlyName) {
							return CSTR(FindFileData.cFileName);
						}
						return CSTR(path);
					}
				}
				FindNextFileA(hFindFile, &FindFileData);
			} while (true);
		}
	}
	return CSTR("ERROR");
#else
	// FIXME(linux): stub
	return CSTR("ERROR");
#endif // _WIN32
}

bool CheckStringHead(CSTR *bigS, CSTR *head){
	if (bigS->length() <= head->length()) {
		return false;
	}
	return (bigS->left(head->length()).icmp(head) == 0);
}

bool GetStringBodyStr(CSTR *str, CSTR head, CSTR *oBuf){
	if (CheckStringHead(str, &head)) {
		oBuf->assign(str->right(str->length() - head.length() - 1) );
		return true;
	}
	return false;
}

bool GetStringBodyInt(CSTR *str, CSTR haed, int *oBuf) {
	if (CheckStringHead(str, &haed)) {
		*oBuf = atol(str->right(str->length() - haed.length() - 1));
		return true;
	}
	return false;
}

bool GetDifficultyFromToken(CSTR str, CSTR *oLeft, CSTR *oRight, CSTR tokenL, CSTR tokenR, int *oBuf) {
	
	int posL = -1;
	CSTR str1(str);

	//check token existence from rightside
	for (str1.nullAtPos(str1.length() - 1); str1.length() > 1; str1.nullAtPos(str1.length() - 1)) {
		if (posL != -1) break;
		if (str1.ends_with(tokenL.body)) {
			posL = str1.length() - tokenL.length();
		}
	}
	if (posL < 1) return false;
	if (!str.ends_with(tokenR.body)) return false;

	oLeft->assign( str.left(posL).trimWhiteSpace() );
	oRight->assign( str.right(str.length() - oLeft->length()).trimWhiteSpace() );
	str1.assign(oRight);
	str1.upper();
	if (str1.findStrPos("BEG") > 0) *oBuf = 1;
	else if (str1.findStrPos("HARD") > 0) *oBuf = 3;
	else if (str1.findStrPos("HYPE") > 0) *oBuf = 3;
	else if (str1.findStrPos("HD") > 0) *oBuf = 3;
	else if (str1.findStrPos("5H") > 0) *oBuf = 3;
	else if (str1.findStrPos("7H") > 0) *oBuf = 3;
	else if (str1.findStrPos("10H") > 0) *oBuf = 3;
	else if (str1.findStrPos("14H") > 0) *oBuf = 3;
	else if (str1.findStrPos("9H") > 0) *oBuf = 3;
	else if (str1.findStrPos("DIF") > 0) *oBuf = 3;
	else if (str1.findStrPos("VERYHARD") > 0) *oBuf = 4;
	else if (str1.findStrPos("EX") > 0) *oBuf = 4;
	else if (str1.findStrPos("AN") > 0) *oBuf = 4;
	else if (str1.findStrPos("SHD") > 0) *oBuf = 4;
	else if (str1.findStrPos("5A") > 0) *oBuf = 4;
	else if (str1.findStrPos("7A") > 0) *oBuf = 4;
	else if (str1.findStrPos("10A") > 0) *oBuf = 4;
	else if (str1.findStrPos("14A") > 0) *oBuf = 4;
	else if (str1.findStrPos("9A") > 0) *oBuf = 4;
	else if (str1.findStrPos("ULT") > 0) *oBuf = 4;
	else if (str1.findStrPos("MANI") > 0) *oBuf = 4;
	else if (str1.findStrPos("LUNA") > 0) *oBuf = 4;
	else if (str1.findStrPos("AHO") > 0) *oBuf = 4;
	else if (str1.findStrPos("AFO") > 0) *oBuf = 4;
	else if (str1.findStrPos("ASDF") > 0) *oBuf = 4;
	else if (str1.findStrPos("HELL") > 0) *oBuf = 4;

	return 1;
}

bool GetDifficulty(CSTR *str, CSTR head, CSTR *oLeft, CSTR *oRight, int *pDifficulty) {
	
	if (!CheckStringHead(str, &head)) return false;

	CSTR upstr(*str);
	upstr.upper();

	if (upstr.ends_with("HARD")) *pDifficulty = 2;
	if (upstr.ends_with("HYPER")) *pDifficulty = 3;
	if (upstr.ends_with("ANOTHER")) *pDifficulty = 4;
	if (upstr.ends_with("EASY")) *pDifficulty = 1;
	if (upstr.ends_with("EX")) *pDifficulty = 4;
	if (upstr.ends_with("MANIAC")) *pDifficulty = 4;

	str->lastCut(str->length() - head.length() - 1);
	if (GetDifficultyFromToken(*str, oLeft, oRight, CSTR("("), CSTR(")"), pDifficulty)) {} //test this method
	else if (GetDifficultyFromToken(*str, oLeft, oRight, CSTR("["), CSTR("]"), pDifficulty)) {}
	else if (GetDifficultyFromToken(*str, oLeft, oRight, CSTR("-"), CSTR("-"), pDifficulty)) {}
	else if (GetDifficultyFromToken(*str, oLeft, oRight, CSTR("\""), CSTR("\""), pDifficulty)) {}
	else if (GetDifficultyFromToken(*str, oLeft, oRight, CSTR("<"), CSTR(">"), pDifficulty)) {}
	else if (GetDifficultyFromToken(*str, oLeft, oRight, CSTR("～"), CSTR("～"), pDifficulty)) {} //? 81 60
	else if (GetDifficultyFromToken(*str, oLeft, oRight, CSTR("【"), CSTR("】"), pDifficulty)) {} //【 81 79, 】81 7a
	else { 
		oLeft->assign(str);
		oRight->fillzero();
	}
	return true;
}

bool IsBmsFile(CSTR str) {
	if (str.length() > 4) {
		str.lastCut(4);
		str.lower();
		if (str.isSame(".bme")) return true;
		if (str.isSame(".bms")) return true;
		if (str.isSame(".bml")) return true;
		if (str.isSame(".pms")) return true;
	}
	return false;
}

bool IsMediaFile(CSTR str) {
	if (str.length() > 4) {
		str.lastCut(4);
		str.lower();
		if (str.isSame(".mp3")) return true;
		if (str.isSame(".wav")) return true;
		if (str.isSame(".avi")) return true;
	}
	return false;
}

bool IsSndFile(CSTR str) {
	if (str.length() > 4) {
		str.lastCut(4);
		str.lower();
		if (str.isSame(".mp3")) return true;
		if (str.isSame(".wav")) return true;
	}
	return false;
}

bool IsAviFile(CSTR str) {
	if (str.length() > 4) {
		str.lastCut(4);
		str.lower();
		if (str.isSame(".avi")) return true;
	}
	return false;
}

bool IsLR2Folder(CSTR str) {
	if (str.length() > 10) {
		str.lastCut(10);
		str.lower();
		if (str.isSame(".lr2folder")) return true;
	}
	return false;
}

// May include wildcard, like "LR2files/CustomFolder/*.txt
bool IsFileExist(CSTR path) {
	if (path.ends_with('\\') || path.ends_with('/')) {
		path.nullAtPos(path.length() - 1);
	}

#ifdef _WIN32
	HANDLE hFindFile;
	_WIN32_FIND_DATAA findFileData;
	char dirFlag = 0;

	hFindFile = FindFirstFileA(path, &findFileData);
	FindClose(hFindFile);
	return hFindFile != (HANDLE)-1;
#else
	// FIXME(linux): wildcard support
	return std::filesystem::exists(path.body);
#endif // _WIN32
}

// \param oldUnixtime Seconds since Unix epoch
// \param iNewtime Non-null out parameter, seconds since Unix epoch
// \retval 0 already_exist
// \retval 1 not_exist
// \retval 2 changed
int IsFileChanged(unsigned int oldUnixtime, CSTR filepath, int *oNewtime) { 
	if (filepath.ends_with('\\') || filepath.ends_with('/')) {
		filepath.nullAtPos(filepath.length() - 1);
	}

	time_t filetime = GetFileUnixtime(filepath);
	if (filetime == -1) {
		*oNewtime = -1;
		return 1;
	}

	if (oldUnixtime < filetime) {
		*oNewtime = filetime;
		return 2;
	}

	*oNewtime = oldUnixtime;
	return 0;
}

int DealWhiteSpace(CSTR *str) {
	bool bFlag = false;
	while (str->ends_with(' ') || str->ends_with('\t') || str->ends_with('\n') || str->ends_with('\r')) {
		str->nullAtPos(str->length() - 1);
	}
	while (true) {
		if (str->starts_with(',')) {
			if (str->length() < 3) return 1;
		}
		else {
			return 1;
		}
		str->lastCut(str->length() - 1);
	}
}

// SplitCSV
int SplitCSV(CSTR csvStr, CSVbuf *oBuf, const char */*splitter*/) {
	int pos,i;
	bool bEnd = false;
	
	for (int i = 0; i < 30; i++) {
		oBuf->val[i] = 0;
		oBuf->str[i].fillzero();
	}

	i = 0;
	do {
		pos = csvStr.findStrPos(",");
		if (pos == 0) {
			oBuf->val[i] = 0;
			csvStr.lastCut(csvStr.length() - 1);
		}
		else if (pos < 0) {
			oBuf->str[i].assign(&csvStr);
			if (oBuf->str[i].starts_with('\"')) {
				if (oBuf->str[i].ends_with("\"")) {
					oBuf->str[i].nullAtPos(oBuf->str[i].length() - 1);
					oBuf->str[i].lastCut(oBuf->str[i].length() - 1);
				}
			}
			if (oBuf->str[i].starts_with('!')) {
				oBuf->str[i].lastCut(oBuf->str[i].length() - 1);
				oBuf->val[i] = -atol(oBuf->str[i]);
			}
			else {
				oBuf->val[i] = atol(oBuf->str[i]);
			}
			csvStr.lastCut(csvStr.length() - pos - 1);
			bEnd = true;
		}
		else {
			//logic arranged
			oBuf->str[i].assign( csvStr.left(pos) );
			if ( oBuf->str[i].starts_with('\"') ){
				if (oBuf->str[i].ends_with('\"')) {
					oBuf->str[i].nullAtPos(oBuf->str[i].length() - 1);
					oBuf->str[i].lastCut(oBuf->str[i].length() - 1);
				}
			}
			if (oBuf->str[i].starts_with('!')) {
				oBuf->str[i].lastCut(oBuf->str[i].length() - 1);
				oBuf->val[i] = -atol(oBuf->str[i]);
			}
			else {
				oBuf->val[i] = atol(oBuf->str[i]);
			}
			csvStr.lastCut(csvStr.length() - pos - 1);
		}
		i++;
		if(i>29 || bEnd){
			return 1;
		}
	} while (true);
}

int Base62ToInt(char ch1, char ch2) {
	int ret = 0;

	if (0x61 <= ch1 && ch1 < 0x61 + 26) ret = ch1 - 0x61 + 36;
	else if (0x31 <= ch1 && ch1 < 0x31 + 10) ret = ch1 - 0x30;
	else if (0x41 <= ch1 && ch1 < 0x41 + 26) ret = ch1 - 0x41 + 10;
	ret = ret * 62;

	if (0x61 <= ch2 && ch2 < 0x61 + 26) ret += ch2 - 0x61 + 36;
	else if (0x31 <= ch2 && ch2 < 0x31 + 10) ret += ch2 - 0x30;
	else if (0x41 <= ch2 && ch2 < 0x41 + 26) ret += ch2 - 0x41 + 10;

	return ret;
}

int Base36ToInt(char ch1, char ch2) {
	int ret = 0;

	if (0x61 <= ch1 && ch1 < 0x61 + 26) ret = ch1 - 0x61 + 10;
	else if (0x31 <= ch1 && ch1 < 0x31 + 10) ret = ch1 - 0x30;
	else if (0x41 <= ch1 && ch1 < 0x41 + 26) ret = ch1 - 0x41 + 10;
	ret = ret * 36;

	if (0x61 <= ch2 && ch2 < 0x61 + 26) ret += ch2 - 0x61 + 10;
	else if (0x31 <= ch2 && ch2 < 0x31 + 10) ret += ch2 - 0x30;
	else if (0x41 <= ch2 && ch2 < 0x41 + 26) ret += ch2 - 0x41 + 10;

	return ret;
}

int Base36or62ToInt(char ch1, char ch2, bool is62) {
	return is62 ? Base62ToInt(ch1, ch2) : Base36ToInt(ch1, ch2);
}

int HEXcharToInt(char ch1, char ch2) {
	int ret = 0;

	if (0x61 <= ch1 && ch1 < 0x61 + 6) ret = ch1 - 0x61 + 10;
	else if (0x31 <= ch1 && ch1 < 0x31 + 10) ret = ch1 - 0x30;
	else if (0x41 <= ch1 && ch1 < 0x41 + 6) ret = ch1 - 0x41 + 10;
	ret = ret << 4;

	if (0x61 <= ch2 && ch2 < 0x61 + 6) ret += ch2 - 0x61 + 10;
	else if (0x31 <= ch2 && ch2 < 0x31 + 10) ret += ch2 - 0x30;
	else if (0x41 <= ch2 && ch2 < 0x41 + 6) ret += ch2 - 0x41 + 10;

	return ret;
}

int RoundUp(double val) {
	int ret = val;
	if (ret != val) ret++; //TEST : if not work properly, use ceil from math.h
	return ret;
}

CSTR AssignCRC32(CSTR str) {
	CSTR tmp;
	unsigned cp_user_had_in_lr2 = 932; // avoid song.db rebuilding. if you've had korean locale sry not sry
	cstrSprintf(&tmp, "%x", CSTR{utf2ansi(str.body, cp_user_had_in_lr2).c_str()}.CRC32());
	return tmp;
}

CSTR AutomationFactory(){
	switch (GetRand(15)) {
		case 0:
			return CSTR("職権を乱用するRainbow");
		case 1:
			return CSTR("またお前らただし女性限定");
		case 2:
			return CSTR("我こそは通訳つきスピリチュアルズ");
		case 3:
			return CSTR("End of the は食後に使おう");
		case 4:
			return CSTR("ジャンバラヤとyou");
		case 5:
			return CSTR("錆び付いたホタル狩り");
		case 6:
			return CSTR("：！寛大な IN COLOR");
		case 7:
			return CSTR("蹴りたい転落");
		case 8:
			return CSTR("Rainy Placement");
		case 9:
			return CSTR("デビュー戦で奥手");
		case 10:
			return CSTR("秋葉never been mellow");
		case 11:
			return CSTR("静電気焼き鳥ムービー");
		case 12:
			return CSTR("人生とビル・ゲイツ");
		case 13:
			return CSTR("黒鍵の上も歩くRED");
		case 14:
			return CSTR("風雲！寝る");
		case 15:
			return CSTR("オペラ：達人、または解雇");
		default:
			return CSTR("※無断使用ばっかりですが訴えないでください");
	}
}

int CountDigit(int num){
	uint ret;

	if (num == 0) {
		return 1;
	}
	if (num < 0) num = -num;
	ret = (num < 0);
	for (; num != 0; num = num / 10) {
		ret++;
	}
	return ret;
}

int GetDigitNum(int num, int pos) {
	if ((0 < num) && (0 < pos)) {
		if (1 < pos) {
			pos = pos + -1;
			do {
				num = num / 10;
				pos = pos + -1;
			} while (pos != 0);
		}
		return num % 10;
	}
	return 0;
}

int FindAltImage(CSTR filename, CSTR dir, CSTR *oBuf) {

	CSTR path;

	path.assign(dir).add(filename);
	if (IsFileExist(path)) {
		oBuf->assign(path);
		return 1;
	}
	if (filename.findStrPos(".png") == -1) {
		path.nullAtPos(path.length() - 3);
		path.add("png");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}
	if (filename.findStrPos(".jpg") == -1) {
		path.nullAtPos(path.length() - 3);
		path.add("jpg");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}
	if (filename.findStrPos(".bmp") == -1) {
		path.nullAtPos(path.length() - 3);
		path.add("bmp");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}

	path.assign(dir).add("../").add(filename);
	if (IsFileExist(path)) {
		oBuf->assign(path);
		return 1;
	}
	if (filename.findStrPos(".png") == -1) {
		path.nullAtPos(path.length() - 3);
		path.add("png");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}
	if (filename.findStrPos(".jpg") == -1) {
		path.nullAtPos(path.length() - 3);
		path.add("jpg");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}
	if (filename.findStrPos(".bmp") == -1) {
		path.nullAtPos(path.length() - 3);
		path.add("bmp");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}

	oBuf->fillzero();
	return 0;
}

int FindAltSound(CSTR filename, CSTR dir, CSTR *oBuf) {

	CSTR path;

	path.assign(dir).add(filename);
	if (IsFileExist(path)) {
		oBuf->assign(path);
		return 1;
	}

	int pos = path.findChrBackPos('.')+1;
	if (pos == 0) {
		path.add(".");
		pos = path.length();
	}

	if (filename.findStrPos(".ogg") == -1) {
		path.nullAtPos(pos);
		path.add("ogg");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}
	if (filename.findStrPos(".wav") == -1) {
		path.nullAtPos(pos);
		path.add("wav");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}
	if (filename.findStrPos(".flac") == -1) {
		path.nullAtPos(pos);
		path.add("flac");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}
	if (filename.findStrPos(".mp3") == -1) {
		path.nullAtPos(pos);
		path.add("mp3");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}

	path.assign(dir).add("../").add(filename);
	if (IsFileExist(path)) {
		oBuf->assign(path);
		return 1;
	}

	pos = path.findChrBackPos('.' + 1);
	if (filename.findStrPos(".wav") == -1) {
		path.nullAtPos(pos);
		path.add("wav");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}
	if (filename.findStrPos(".ogg") == -1) {
		path.nullAtPos(pos);
		path.add("ogg");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}
	if (filename.findStrPos(".flac") == -1) {
		path.nullAtPos(pos);
		path.add("flac");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}
	if (filename.findStrPos(".mp3") == -1) {
		path.nullAtPos(pos);
		path.add("mp3");
		if (IsFileExist(path)) {
			oBuf->assign(path);
			return 1;
		}
	}

	ErrorLogFmtAdd("FindAltSound: '%s' not found in '%s'\n", filename.c_str(), dir.c_str());

	oBuf->fillzero();
	return 0;
}

CSTR GetRandomFile(CSTR path, char fOnlyName) {
	CSTR oBuf;
	int count;

	//call function if wildcard is on directory
	if (path.findStrPos("*/") != -1 || path.findStrPos("*\\") != -1 || path.ends_with("*")) {
		return GetRandomFileOnDir(path, fOnlyName);
	}

#ifdef _WIN32
	WIN32_FIND_DATAA FindFileData;
	//count files for random
	HANDLE hFindFile = FindFirstFileA(path, &FindFileData);
	if (hFindFile == (HANDLE)-1) return CSTR("ERROR");
	
	count = 0;
	do {
		count++;
	} while (FindNextFileA(hFindFile, &FindFileData));
	FindClose(hFindFile);
	if (count < 1) return CSTR("ERROR");

	count = GetRand(count - 1);

	//get file by random
	hFindFile = FindFirstFileA(path, &FindFileData);
	if (hFindFile == (HANDLE)-1) return CSTR("ERROR");

	for (int i = 0; i < count; i++) {
		FindNextFileA(hFindFile, &FindFileData);
	}
	FindClose(hFindFile);
	path.assign(path.getDirectory());
	path.add(FindFileData.cFileName);
	if (fOnlyName) {
		path.assign(FindFileData.cFileName);
		path.nullAtPos(path.findStrPos("."));
	}
	return path;
#else
	// FIXME(linux): stub
	return path;
#endif // _WIN32
}

CSTR GetRandomFileNoError(CSTR path, CSTR /*dir*/) {
	// TODO: 'dir' suspiciously not used
	CSTR filepath = GetRandomFile(path, 0);
	if (filepath.isDiff("ERROR")) return filepath;
	return path;
}

char md5str[33];
char* MD5str(char *iStr) {
	char* buf = (char*)md5String(iStr);
	unsigned char md5buf[16];
	memcpy(md5buf, buf, 16);
	sprintf(md5str, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", md5buf[0], md5buf[1], md5buf[2], md5buf[3],
		md5buf[4], md5buf[5], md5buf[6], md5buf[7], md5buf[8], md5buf[9], md5buf[10], md5buf[11], md5buf[12], md5buf[13], md5buf[14], md5buf[15]);
	free(buf);
	return md5str;
}
