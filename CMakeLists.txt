cmake_minimum_required(VERSION 3.29)
project(OpenLR2 LANGUAGES C CXX)

# Better compatibility with Wine. vcpkg target triplet must match this.
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# We don't use them and they cause fake errors in clangd
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

find_package(JPEG REQUIRED)
find_package(Ogg CONFIG REQUIRED)
find_package(Opus CONFIG REQUIRED)
find_package(OpusFile CONFIG REQUIRED)
find_package(PNG REQUIRED)
find_package(TIFF REQUIRED)
find_package(Vorbis CONFIG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(unofficial-theora CONFIG REQUIRED)

add_library(SQLite STATIC dep/sqlite/sqlite3.c)
target_include_directories(SQLite PUBLIC dep/sqlite)

add_library(
    ImGui
    STATIC
    dep/imgui/imgui.cpp
    dep/imgui/imgui_draw.cpp
    dep/imgui/imgui_tables.cpp
    dep/imgui/imgui_widgets.cpp
    dep/imgui/imgui_impl_win32.cpp
    dep/imgui/imgui_impl_dx9.cpp
    dep/imgui/imgui_impl_dx11.cpp
    dep/imgui/implot.cpp
    dep/imgui/implot_items.cpp
)
target_include_directories(ImGui PUBLIC dep/imgui)
if(WIN32)
    target_compile_definitions(ImGui PUBLIC NOMINMAX WIN32_LEAN_AND_MEAN)
    target_link_libraries(ImGui PUBLIC d3d9.lib d3d11.lib dxgi.lib)
endif()
if(MSVC)
    target_compile_options(ImGui PRIVATE /utf-8)
endif()

add_library(
    TinyXML
    STATIC
    dep/tinyxml/tinystr.cpp
    dep/tinyxml/tinyxml.cpp
    dep/tinyxml/tinyxmlerror.cpp
    dep/tinyxml/tinyxmlparser.cpp
)
target_include_directories(TinyXML PUBLIC dep/tinyxml)

if(WIN32)
    add_library(FMOD SHARED IMPORTED GLOBAL)
    target_include_directories(FMOD INTERFACE dep/FMOD)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set_target_properties(
            FMOD
            PROPERTIES
                IMPORTED_LOCATION_DEBUG
                    ${CMAKE_CURRENT_LIST_DIR}/dep/FMOD/bin64/fmodL.dll
                IMPORTED_IMPLIB_DEBUG
                    ${CMAKE_CURRENT_LIST_DIR}/dep/FMOD/bin64/fmodL_vc.lib
                IMPORTED_LOCATION
                    ${CMAKE_CURRENT_LIST_DIR}/dep/FMOD/bin64/fmod.dll
                IMPORTED_IMPLIB
                    ${CMAKE_CURRENT_LIST_DIR}/dep/FMOD/bin64/fmod_vc.lib
        )
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set_target_properties(
            FMOD
            PROPERTIES
                IMPORTED_LOCATION_DEBUG
                    ${CMAKE_CURRENT_LIST_DIR}/dep/FMOD/bin86/fmodL.dll
                IMPORTED_IMPLIB_DEBUG
                    ${CMAKE_CURRENT_LIST_DIR}/dep/FMOD/bin86/fmodL_vc.lib
                IMPORTED_LOCATION
                    ${CMAKE_CURRENT_LIST_DIR}/dep/FMOD/bin86/fmod.dll
                IMPORTED_IMPLIB
                    ${CMAKE_CURRENT_LIST_DIR}/dep/FMOD/bin86/fmod_vc.lib
        )
    else()
        message(FATAL_ERROR "Unsupported CMAKE_SIZEOF_VOID_P")
    endif()
elseif(LINUX)
    add_library(FMOD SHARED IMPORTED GLOBAL)
    target_include_directories(FMOD INTERFACE dep/FMOD)
    set_target_properties(
        FMOD
        PROPERTIES
            IMPORTED_LOCATION
                ${CMAKE_CURRENT_LIST_DIR}/dep/FMOD/bin64/libfmod.so.14
            IMPORTED_LOCATION_DEBUG
                ${CMAKE_CURRENT_LIST_DIR}/dep/FMOD/bin64/libfmodL.so.14
    )
else()
    message(FATAL_ERROR "No FMOD for this platform")
endif()

if(WIN32)
    add_library(
        DxLib
        STATIC
        # DxUseCLib
        dep/DxLib/DxUseCLib.cpp
        dep/DxLib/DxUseCLibLive2DCubism4.cpp
        dep/DxLib/DxUseCLibOgg.cpp
        dep/DxLib/DxUseCLibPhysics.cpp
        dep/DxLib/Windows/DxUseCLibOggD3D11.cpp
        dep/DxLib/Windows/DxUseCLibOggD3D9.cpp
        dep/DxLib/Windows/DxUseCLibOggWin.cpp
        dep/DxLib/Windows/DxUseCStrmBaseFilter.cpp
        # DxDrawLib
        dep/DxLib/DxMemImgDrawFunction0.cpp
        dep/DxLib/DxMemImgDrawFunction1.cpp
        dep/DxLib/DxMemImgDrawFunction2.cpp
        dep/DxLib/DxMemImgDrawFunction3.cpp
        # DxLib
        dep/DxLib/DxArchive_.cpp
        dep/DxLib/DxASyncLoad.cpp
        dep/DxLib/DxBaseFunc.cpp
        dep/DxLib/DxBaseImage.cpp
        dep/DxLib/DxChar.cpp
        dep/DxLib/DxCharCodeTable.cpp
        dep/DxLib/DxFile.cpp
        dep/DxLib/DxFont.cpp
        dep/DxLib/DxGateway.cpp
        dep/DxLib/DxGraphics.cpp
        dep/DxLib/DxGraphicsFilter.cpp
        dep/DxLib/DxHandle.cpp
        dep/DxLib/DxHeap.cpp
        dep/DxLib/DxInput.cpp
        dep/DxLib/DxInputString.cpp
        dep/DxLib/DxLive2DCubism4.cpp
        dep/DxLib/DxLog.cpp
        dep/DxLib/DxMask.cpp
        dep/DxLib/DxMath.cpp
        dep/DxLib/DxMemImg.cpp
        dep/DxLib/DxMemory.cpp
        dep/DxLib/DxModel.cpp
        dep/DxLib/DxModelLoader0.cpp
        dep/DxLib/DxModelLoader1.cpp
        dep/DxLib/DxModelLoader2.cpp
        dep/DxLib/DxModelLoader3.cpp
        dep/DxLib/DxModelLoader4.cpp
        dep/DxLib/DxModelLoaderVMD.cpp
        dep/DxLib/DxModelRead.cpp
        dep/DxLib/DxMovie.cpp
        dep/DxLib/DxNetwork.cpp
        dep/DxLib/DxRingBuffer.cpp
        dep/DxLib/DxSoftImage.cpp
        dep/DxLib/DxSound.cpp
        dep/DxLib/DxSoundConvert.cpp
        dep/DxLib/DxSystem.cpp
        dep/DxLib/DxThread.cpp
        dep/DxLib/Windows/DxBaseFuncWin.cpp
        dep/DxLib/Windows/DxBaseImageWin.cpp
        dep/DxLib/Windows/DxFileWin.cpp
        dep/DxLib/Windows/DxFontWin.cpp
        dep/DxLib/Windows/DxGraphicsAPIWin.cpp
        dep/DxLib/Windows/DxGraphicsD3D11.cpp
        dep/DxLib/Windows/DxGraphicsD3D9.cpp
        dep/DxLib/Windows/DxGraphicsFilterD3D11.cpp
        dep/DxLib/Windows/DxGraphicsFilterD3D9.cpp
        dep/DxLib/Windows/DxGraphicsFilterWin.cpp
        dep/DxLib/Windows/DxGraphicsWin.cpp
        dep/DxLib/Windows/DxGuid.cpp
        dep/DxLib/Windows/DxInputWin.cpp
        dep/DxLib/Windows/DxKeyHookBinary.cpp
        dep/DxLib/Windows/DxLive2DCubism4Win.cpp
        dep/DxLib/Windows/DxLogWin.cpp
        dep/DxLib/Windows/DxMaskD3D11.cpp
        dep/DxLib/Windows/DxMaskD3D9.cpp
        dep/DxLib/Windows/DxMaskWin.cpp
        dep/DxLib/Windows/DxMemoryWin.cpp
        dep/DxLib/Windows/DxModelD3D11.cpp
        dep/DxLib/Windows/DxModelD3D9.cpp
        dep/DxLib/Windows/DxModelWin.cpp
        dep/DxLib/Windows/DxMovieWin.cpp
        dep/DxLib/Windows/DxNetworkWin.cpp
        dep/DxLib/Windows/DxShaderCodeBin_Base3D_D3D11.cpp
        dep/DxLib/Windows/DxShaderCodeBin_Base3D_D3D9.cpp
        dep/DxLib/Windows/DxShaderCodeBin_Base_D3D11.cpp
        dep/DxLib/Windows/DxShaderCodeBin_Base_D3D9.cpp
        dep/DxLib/Windows/DxShaderCodeBin_Filter_D3D11.cpp
        dep/DxLib/Windows/DxShaderCodeBin_Filter_D3D9.cpp
        dep/DxLib/Windows/DxShaderCodeBin_Live2D_Cubism4_D3D11.cpp
        dep/DxLib/Windows/DxShaderCodeBin_Live2D_Cubism4_D3D9.cpp
        dep/DxLib/Windows/DxShaderCodeBin_Model_D3D11.cpp
        dep/DxLib/Windows/DxShaderCodeBin_Model_D3D9.cpp
        dep/DxLib/Windows/DxShaderCodeBin_RgbaMix_D3D11.cpp
        dep/DxLib/Windows/DxShaderCodeBin_RgbaMix_D3D9.cpp
        dep/DxLib/Windows/DxShaderCodeTxt_D3D11.cpp
        dep/DxLib/Windows/DxShaderCodeTxt_D3D9.cpp
        dep/DxLib/Windows/DxSoundConvertWin.cpp
        dep/DxLib/Windows/DxSoundWin.cpp
        dep/DxLib/Windows/DxSystemWin.cpp
        dep/DxLib/Windows/DxThreadWin.cpp
        dep/DxLib/Windows/DxWinAPI.cpp
        dep/DxLib/Windows/DxWindow.cpp
    )
    target_compile_definitions(DxLib PUBLIC DX_LIB_NOT_DEFAULTPATH)
    target_include_directories(DxLib PUBLIC dep/DxLib)
    target_link_libraries(
        DxLib
        PUBLIC
            JPEG::JPEG
            Ogg::ogg
            Opus::opus
            OpusFile::opusfile
            PNG::PNG
            TIFF::TIFF
            Vorbis::vorbis
            Vorbis::vorbisenc
            Vorbis::vorbisfile
            ZLIB::ZLIB
            unofficial::theora::theora
            unofficial::theora::theoradec
            unofficial::theora::theoraenc
    )
    # DxUseCLib seems to want internal Ogg headers
    target_include_directories(
        DxLib
        PUBLIC dep/dxlib-for-linux/DxLib/insufficient_include
    )
	if(MSVC)
		target_compile_options(DxLib PUBLIC /utf-8)
	endif()
elseif(LINUX)
    add_library(DxLib STATIC IMPORTED GLOBAL)
    set_target_properties(
        DxLib
        PROPERTIES
            IMPORTED_LOCATION
                ${CMAKE_CURRENT_LIST_DIR}/dep/dxlib-for-linux/DxLib/DxLibMake/libDxLib.a
    )

    target_include_directories(DxLib INTERFACE dep/dxlib-for-linux/DxLib/DxLib)

    target_link_libraries(
        DxLib
        INTERFACE
            JPEG::JPEG
            Ogg::ogg
            Opus::opus
            OpusFile::opusfile
            PNG::PNG
            TIFF::TIFF
            Vorbis::vorbis
            Vorbis::vorbisenc
            Vorbis::vorbisfile
            ZLIB::ZLIB
            unofficial::theora::theora
            unofficial::theora::theoradec
            unofficial::theora::theoraenc
    )

    # dxlib-for-linux specific
    find_package(OpenAL REQUIRED)
    target_link_libraries(DxLib INTERFACE OpenAL::OpenAL)
    target_link_libraries(DxLib INTERFACE EGL GL X11 pthread)
else()
    message(FATAL_ERROR "No DxLib for this platform")
endif()

add_library(md5 STATIC dep/md5/md5.cpp)
target_include_directories(md5 PUBLIC dep/md5)

set(IXWEBSOCKET_SOURCES
    dep/ixwebsocket/IXBench.cpp
    dep/ixwebsocket/IXCancellationRequest.cpp
    dep/ixwebsocket/IXConnectionState.cpp
    dep/ixwebsocket/IXDNSLookup.cpp
    dep/ixwebsocket/IXExponentialBackoff.cpp
    dep/ixwebsocket/IXGetFreePort.cpp
    dep/ixwebsocket/IXGzipCodec.cpp
    dep/ixwebsocket/IXHttp.cpp
    dep/ixwebsocket/IXHttpClient.cpp
    dep/ixwebsocket/IXHttpServer.cpp
    dep/ixwebsocket/IXNetSystem.cpp
    dep/ixwebsocket/IXSelectInterrupt.cpp
    dep/ixwebsocket/IXSelectInterruptFactory.cpp
    dep/ixwebsocket/IXSelectInterruptPipe.cpp
    dep/ixwebsocket/IXSelectInterruptEvent.cpp
    dep/ixwebsocket/IXSetThreadName.cpp
    dep/ixwebsocket/IXSocket.cpp
    dep/ixwebsocket/IXSocketConnect.cpp
    dep/ixwebsocket/IXSocketFactory.cpp
    dep/ixwebsocket/IXSocketServer.cpp
    dep/ixwebsocket/IXSocketTLSOptions.cpp
    dep/ixwebsocket/IXStrCaseCompare.cpp
    dep/ixwebsocket/IXUdpSocket.cpp
    dep/ixwebsocket/IXUrlParser.cpp
    dep/ixwebsocket/IXUuid.cpp
    dep/ixwebsocket/IXUserAgent.cpp
    dep/ixwebsocket/IXWebSocket.cpp
    dep/ixwebsocket/IXWebSocketCloseConstants.cpp
    dep/ixwebsocket/IXWebSocketHandshake.cpp
    dep/ixwebsocket/IXWebSocketHttpHeaders.cpp
    dep/ixwebsocket/IXWebSocketPerMessageDeflate.cpp
    dep/ixwebsocket/IXWebSocketPerMessageDeflateCodec.cpp
    dep/ixwebsocket/IXWebSocketPerMessageDeflateOptions.cpp
    dep/ixwebsocket/IXWebSocketProxyServer.cpp
    dep/ixwebsocket/IXWebSocketServer.cpp
    dep/ixwebsocket/IXWebSocketTransport.cpp
)
add_library(IXWebSocket STATIC ${IXWEBSOCKET_SOURCES})
target_include_directories(IXWebSocket PUBLIC dep)
if(WIN32)
    target_compile_definitions(IXWebSocket PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_link_libraries(IXWebSocket PUBLIC ws2_32)
endif()

# Separate target so that SkinEditor can use it
add_library(
    OpenLR2Lib
    LR2/En_audio.cpp
    LR2/En_dbio.cpp
    LR2/En_fileutil.cpp
    LR2/En_graphic.cpp
    LR2/En_input.cpp
    LR2/En_recordmovie.cpp
    LR2/En_timer.cpp
    LR2/En_value.cpp
    LR2/En_xml.cpp
    LR2/LR2_audio.cpp
    LR2/LR2_bmsload.cpp
    LR2/LR2_configsave.cpp
    LR2/LR2_customir.cpp
    LR2/LR2_arena.cpp
    LR2/arena_backend.cpp
    LR2/arena_backend_ir.cpp
    LR2/arena_backend_lan.cpp
    LR2/arena_lan_net.cpp
    LR2/arena_lan_client.cpp
    LR2/arena_lan_server.cpp
    LR2/arena_imgui.cpp
    LR2/arena_imgui_gui.cpp
    LR2/arena_gui_util.cpp
    LR2/arena_keys.cpp
    LR2/arena_maniac.cpp
    LR2/LR2_gameloop.cpp
    LR2/LR2_ghost.cpp
    LR2/LR2_ir.cpp
    LR2/LR2_makemp3.cpp
    LR2/LR2_replay.cpp
    LR2/LR2_skindraw.cpp
    LR2/LR2_skinload.cpp
    LR2/LR2_skinmanage.cpp
    LR2/LR2_skinobject.cpp
    LR2/LR2_songmanage.cpp
    LR2/LR2_statlong.cpp
    LR2/LR2_statplay.cpp
    LR2/LR2_version.cpp
    LR2/Scene02_Songselect.cpp
    LR2/Scene03_Decide.cpp
    LR2/Scene04_Play.cpp
    LR2/Scene05_Result.cpp
    LR2/Scene06_Keyconfig.cpp
    LR2/Scene07_Skinselect.cpp
    LR2/Scene08_Lunaris.cpp
    LR2/Scene09_Po4menu.cpp
    LR2/Scene10_Po4decide.cpp
    LR2/Scene11_Po4select.cpp
    LR2/Scene13_Courseresult.cpp
    LR2/strclass.cpp
)
set_target_properties(OpenLR2Lib PROPERTIES CXX_STANDARD 23)
target_include_directories(OpenLR2Lib PUBLIC dep)
target_compile_definitions(OpenLR2Lib PUBLIC MSGPACK_NO_BOOST)
target_link_libraries(
    OpenLR2Lib
    PUBLIC DxLib FMOD SQLite TinyXML md5 ImGui IXWebSocket
)

if(WIN32)
    target_compile_definitions(OpenLR2Lib PUBLIC NOMINMAX WIN32_LEAN_AND_MEAN)
    target_link_libraries(OpenLR2Lib PUBLIC winmm.lib)
endif()

if(MSVC)
    target_compile_options(OpenLR2Lib PUBLIC /utf-8)
endif()

# 4GB patch for 32-bit.
if(MSVC)
    target_link_options(OpenLR2Lib PUBLIC /LARGEADDRESSAWARE)
else()
    # untested
    # target_link_options(OpenLR2Lib PUBLIC -Wl,--large-address-aware)
endif()

# 'game' god struct is very big while on the stack.
if(MSVC)
    target_link_options(OpenLR2Lib PUBLIC /STACK:10485760)
else()
    # untested
    # target_link_options(OpenLR2Lib PUBLIC -Wl,--stack,10485760)
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH_SUFFIX "_x64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(ARCH_SUFFIX "_x86")
else()
    message(FATAL_ERROR "Unsupported CMAKE_SIZEOF_VOID_P")
endif()

add_executable(OpenLR2 WIN32 LR2/Main.cpp OpenLR2.manifest)
set_target_properties(OpenLR2 PROPERTIES CXX_STANDARD 23)
target_link_libraries(OpenLR2 PUBLIC OpenLR2Lib)
set_target_properties(OpenLR2 PROPERTIES OUTPUT_NAME "OpenLR2${ARCH_SUFFIX}")

set(IR_CONF_SUFFIX $<$<CONFIG:Debug>:_D>)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(IR_ARCH_SUFFIX ".x64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(IR_ARCH_SUFFIX ".x86")
else()
    message(FATAL_ERROR "Unsupported CMAKE_SIZEOF_VOID_P")
endif()

add_library(ExampleIR SHARED ExampleIR/ExternalIR.cpp)
set_target_properties(ExampleIR PROPERTIES CXX_STANDARD 23)
# For LR2_customir_api.h.
# If you are making an IR module of your own, you should copy this file into your project instead.
target_include_directories(ExampleIR PUBLIC LR2)
set_target_properties(
    ExampleIR
    PROPERTIES OUTPUT_NAME "ExampleIR${IR_CONF_SUFFIX}${IR_ARCH_SUFFIX}"
)
