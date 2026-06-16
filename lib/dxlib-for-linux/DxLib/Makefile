SHELL := /usr/bin/env bash

DXLIB_VER := 3_24f

# 環境によっては変更してください
UNZIP := unar -e cp932
CXX := g++

# 環境、要不要に応じて変更してください
CXXFLAGS += -g -O2 \
-DDX_NON_BULLET_PHYSICS \
-DDX_NON_LIVE2D_CUBISM4 \
-I insufficient_include \
-I /usr/include/opus \
#-I clib/prefix/include \
#-I clib/prefix/include/opus \
#-DDX_NON_JPEGREAD \
#-DDX_NON_TIFFREAD \
#-DDX_NON_PNGREAD \
#-DDX_NON_MODEL \
#-DDX_NON_OGGVORBIS \
#-DDX_NON_SOUND \

# 要不要に応じて変更してください
DEP_LIBS := \
    EGL GL X11 \
    jpeg ogg openal opus opusfile opusurl \
    png pthread theora theoradec theoraenc tiff tiffxx \
    vorbis vorbisenc vorbisfile z \

LDFLAG_LIBS := $(addprefix -l, $(DEP_LIBS))

DXLIB_A_OUT_NAME := libDxLib.a

OBJ_DIR := .

SRCS := $(addprefix DxLibMake/,\
        DxASyncLoad \
        DxArchive_ \
        DxChar \
        DxCharCodeTable \
        DxGateway \
        DxHandle \
        DxHeap \
        DxInputString \
        DxLive2DCubism4 \
        DxMath \
        DxMemImg \
        DxMemImgDrawFunction0 \
        DxMemImgDrawFunction1 \
        DxMemImgDrawFunction2 \
        DxMemImgDrawFunction3 \
        DxModelLoader0 \
        DxModelLoader1 \
        DxModelLoader2 \
        DxModelLoader3 \
        DxModelLoader4 \
        DxModelLoaderVMD \
        DxModelRead \
        DxRingBuffer \
        DxSoftImage \
        DxUseCLib \
        DxUseCLibPhysics \
        Linux/DxBaseFuncLinux       DxBaseFunc \
        Linux/DxBaseImageLinux      DxBaseImage \
        Linux/DxFileLinux           DxFile \
        Linux/DxFontLinux           DxFont \
        Linux/DxGraphicsLinux       DxGraphics \
        Linux/DxGraphicsFilterLinux DxGraphicsFilter \
        Linux/DxInputLinux          DxInput \
        Linux/DxLive2DCubism4Linux  DxUseCLibLive2DCubism4 \
        Linux/DxLogLinux            DxLog \
        Linux/DxMaskLinux           DxMask \
        Linux/DxMemoryLinux         DxMemory \
        Linux/DxModelLinux          DxModel \
        Linux/DxMovieLinux          DxMovie \
        Linux/DxSoundLinux          DxSound \
        Linux/DxSoundConvertLinux   DxSoundConvert \
        Linux/DxSystemLinux         DxSystem \
        Linux/DxThreadLinux         DxThread \
        Linux/DxUseCLibOggLinux     DxUseCLibOgg \
        Linux/DxShaderCodeBin_Base3D_Linux \
        Linux/DxShaderCodeBin_Base_Linux \
        Linux/DxShaderCodeBin_Filter_Linux \
        Linux/DxShaderCodeBin_Model_Linux \
        Linux/DxShaderCodeBin_RgbaMix_Linux \
        Linux/DxWindowLinux \
    )

SRCS_CPP := $(addprefix $(OBJ_DIR)/,$(addsuffix .cpp,$(SRCS)))
OBJS := $(addprefix $(OBJ_DIR)/,$(addsuffix .o,$(SRCS)))

SAMPLES := $(addprefix samples/,\
        sample01_minimum \
        sample02_string_and_shape \
        sample03_3d_shape \
        sample04_mainloop \
        sample05_sound \
        sample06_graph \
        sample07_printfdx \
        sample08_input \
        sample09_window \
        sample10_dnd \
        sample11_change_font \
        sample12_clipboard \
    )

.PHONY: all
all: $(SAMPLES)

.PHONY: samples
samples: $(SAMPLES)

#$(SAMPLES): lib

$(SAMPLES): %: %.cpp $(OBJ_DIR)/DxLibMake/$(DXLIB_A_OUT_NAME)
	$(CXX) -o $@ $< \
    -I DxLibMake $(CXXFLAGS) \
    $(OBJ_DIR)/DxLibMake/$(DXLIB_A_OUT_NAME) \
    $(LDFLAG_LIBS) \

.PHONY: lib
lib: $(OBJ_DIR)/DxLibMake/$(DXLIB_A_OUT_NAME)

$(OBJ_DIR)/DxLibMake/$(DXLIB_A_OUT_NAME): $(OBJS)
	rm -f $(OBJ_DIR)/DxLibMake/$(DXLIB_A_OUT_NAME)
	ar rcs $(OBJ_DIR)/DxLibMake/$(DXLIB_A_OUT_NAME) $(OBJS)

.PHONY: objs
objs: $(OBJS)

$(OBJ_DIR)/%.o: %.cpp DxLibMake/.patched
	mkdir -p $(OBJ_DIR)/DxLibMake $(OBJ_DIR)/DxLibMake/Linux
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(SRCS_CPP): DxLibMake/.patched

.PHONY: patch
patch: DxLibMake/.patched

DxLibMake/.patched: DxLibMake/.extracted
	# Linux用コンパイル設定追加
	patch -uNp1 --no-backup-if-mismatch -d DxLibMake < 0001-DxCompileConfig.patch
	# Linux向け型定義
	cp -a DxLibMake/DxDataType{Android,Linux}.h
	# 不足ヘッダー追加
	patch -uNp1 --no-backup-if-mismatch -d DxLibMake < 0003-DxDataTypeLinux.patch
	
	# AndroidをベースにLinux用のコードを生成
	#  単純にコピー
	cp -a DxLibMake/Android DxLibMake/Linux
	#  ファイル名をAndroid->Linux
	for f in $$(find DxLibMake/Linux/ -type f); do t=$$(echo $$f | sed s/Android/Linux/g); mv $$f $$t; done
	#  各種Androidっぽい字句->対応するLinuxっぽい字句
	sed -i \
    -e s/Android/Linux/g \
    -e s/ANDROID/LINUX/g \
    -e s/android/linux/g \
    -e s/GANDR/GLINUX/g \
    -e s/ANDR/LINUX/g \
    DxLibMake/Linux/*.{h,cpp}
	# iOSをベースにするコードを上書き
	for f in \
            DxFile:h \
            DxFile:cpp \
            DxFont:h \
            DxFont:cpp \
            DxLog:h \
            DxLog:cpp \
            DxSound:h \
            DxSound:cpp \
            ; do \
        IFS=: f=($${f}); \
        cat DxLibMake/iOS/$${f[0]}iOS.$${f[1]} | sed \
            -e s/IOS/LINUX/g \
            -e s/iOS/Linux/g \
            > DxLibMake/Linux/$${f[0]}Linux.$${f[1]} ; \
    done
	#  Linuxで利用しないコードの削除
	rm -f DxLibMake/Linux/DxJavaLinux.h

	#  各ヘッダ内でLinux向けのifdefを追加
	for f in \
            DxBaseFunc.h:Linux/DxBaseFuncLinux.h \
            DxBaseImage.cpp:Linux/DxBaseImageLinux.h \
            DxDataType.h:DxDataTypeLinux.h \
            DxFile.h:Linux/DxFileLinux.h \
            DxFont.cpp:Linux/DxFontLinux.h \
            DxGraphics.cpp:Linux/DxGraphicsLinux.h \
            DxGraphicsFilter.cpp:Linux/DxGraphicsFilterLinux.h \
            DxInput.h:Linux/DxInputLinux.h \
            DxLib.h:DxFunctionLinux.h \
            DxLive2DCubism4.h:Linux/DxLive2DCubism4Linux.h \
            DxLog.h:Linux/DxLogLinux.h \
            DxMask.cpp:Linux/DxMaskLinux.h \
            DxMemory.h:Linux/DxMemoryLinux.h \
            DxModel.cpp:Linux/DxModelLinux.h \
            DxMovie.h:Linux/DxMovieLinux.h \
            DxSound.h:Linux/DxSoundLinux.h \
            DxSoundConvert.h:Linux/DxSoundConvertLinux.h \
            DxSystem.h:Linux/DxSystemLinux.h \
            DxThread.h:Linux/DxThreadLinux.h \
            DxUseCLibOgg.cpp:Linux/DxUseCLibOggLinux.h \
            ; do \
        IFS=: f=($${f}); \
        echo -e "\n#ifdef __linux__\n#include \"$${f[1]}\"\n#endif // __linux__" | \
        sed -i -e '/\/\/ __ANDR/r/dev/stdin' DxLibMake/$${f[0]} ; \
    done
	
	#  なんかバグい
	patch -uNp1 --no-backup-if-mismatch -d DxLibMake < 0011-not-work-printfDxBase_WCHAR_T.patch
	#  名前干渉
	patch -uNp1 --no-backup-if-mismatch -d DxLibMake < 0012-Always-already-defined.patch
	#  Linux専用関数
	cp -a DxFunctionLinux.h DxLibMake
	cp -a DxWindowLinux.h DxLibMake/Linux
	cp -a DxWindowLinux.cpp DxLibMake/Linux
	#  Android特化コードを削除
	cp -a DxSystemLinux.h DxLibMake/Linux
	cp -a DxInputLinux.h DxLibMake/Linux
	#  微修正
	patch -uNp1 --no-backup-if-mismatch -d DxLibMake < 0100-File-cpp.patch
	patch -uNp1 --no-backup-if-mismatch -d DxLibMake < 0100-Log-cpp.patch
	patch -uNp1 --no-backup-if-mismatch -d DxLibMake < 0100-Sound-h.patch
	patch -uNp1 --no-backup-if-mismatch -d DxLibMake < 0100-Sound-cpp.patch
	patch -uNp1 --no-backup-if-mismatch -d DxLibMake < 0100-Thread-cpp.patch
	#  (Androidベース、要分離)
	cp -a DxSystemLinux.cpp DxLibMake/Linux
	#  (要分離)
	cp -a DxGraphicsLinux.h DxLibMake/Linux
	cp -a DxGraphicsLinux.cpp DxLibMake/Linux
	#  Android特化コードを削除
	cp -a DxInputLinux.cpp DxLibMake/Linux
	#  (iOSベース？)
	cp -a DxFontLinux.cpp DxLibMake/Linux

	touch DxLibMake/.patched
	@# debug
	@#rm -rf DxLibMake.old DxLibMake.new
	@#cp -ai DxLibMake{,.old}
	@#cp -ai DxLibMake{,.new}

.PHONY: extract-source
extract-source: DxLibMake/.extracted

DxLibMake/.extracted: DxLibMake$(DXLIB_VER).zip
	$(UNZIP) DxLibMake$(DXLIB_VER).zip
	find DxLibMake -type f | while read f; do cat "$$f" | tr -d '\r' > tmp; \
    (iconv -f utf-8 -t utf-8 tmp -o "$$f" 2>/dev/null) \
    || (iconv -f cp932 -t utf-8 tmp -o "$$f"); rm -f tmp; done
	touch DxLibMake/.extracted

.PHONY: get-source
get-source: DxLibMake$(DXLIB_VER).zip

DxLibMake$(DXLIB_VER).zip:
	[ -f "DxLibMake$(DXLIB_VER).zip" ] || \
    curl -LO https://dxlib.xsrv.jp/DxLib/DxLibMake$(DXLIB_VER).zip

.PHONY: clean_objs
clean_objs:
	rm -rf $(OBJS)

.PHONY: clean
clean:
	rm -rf DxLibMake
	rm -f $(SAMPLES)

.PHONY: clean_all
clean_all: clean
	rm DxLibMake*.zip
