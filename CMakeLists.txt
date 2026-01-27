cmake_minimum_required(VERSION 3.29)
project(OpenLR2 LANGUAGES C CXX)

# Make sure _DLL is not defined so that we use a specific DxLib DLL.
# @gomazk
# > my custom dxlib was only DxLib_vs2015_x86_MT. 3.24f based, can load old dxa
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# We don't use them and they cause fake errors in clangd
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

add_library(SQLite STATIC lib/sqlite/sqlite3.c)
target_include_directories(SQLite PUBLIC lib/sqlite)

add_library(
    TinyXML
    STATIC
    lib/tinyxml/tinystr.cpp
    lib/tinyxml/tinyxml.cpp
    lib/tinyxml/tinyxmlerror.cpp
    lib/tinyxml/tinyxmlparser.cpp
)
target_include_directories(TinyXML PUBLIC lib/tinyxml)

if(WIN32)
    add_library(FMOD INTERFACE IMPORTED GLOBAL)
    target_include_directories(FMOD INTERFACE lib)
    target_link_directories(FMOD INTERFACE lib/FMOD)
    # TODO(no-vs-solutions): adjust paths with CMAKE_SIZEOF_VOID_P and use instead of target_link_directories
    # set_target_properties(
    #     FMOD
    #     PROPERTIES
    #         IMPORTED_LOCATION_DEBUG ${CMAKE_CURRENT_LIST_DIR}/lib/FMOD/bin64/fmodL.dll
    #         IMPORTED_IMPLIB_DEBUG ${CMAKE_CURRENT_LIST_DIR}/lib/FMOD/bin64/fmodL_vc.lib
    #         IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/lib/FMOD/bin64/fmod.dll
    #         IMPORTED_IMPLIB ${CMAKE_CURRENT_LIST_DIR}/lib/FMOD/bin64/fmod_vc.lib
    # )
else()
    message(FATAL_ERROR "No FMOD for this platform")
endif()

if(WIN32)
    add_library(DxLib INTERFACE IMPORTED GLOBAL)
    target_include_directories(DxLib INTERFACE lib)
    target_link_directories(DxLib INTERFACE lib/DxLib)
else()
    message(FATAL_ERROR "No DxLib for this platform")
endif()

add_library(md5 STATIC lib/md5.cpp)
target_include_directories(md5 PUBLIC lib)

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
    LR2/dbgtool.cpp
    LR2/strclass.cpp
)
set_target_properties(OpenLR2Lib PROPERTIES CXX_STANDARD 23)
target_include_directories(OpenLR2Lib PUBLIC lib)
target_link_libraries(
    OpenLR2Lib
    PUBLIC DxLib FMOD SQLite TinyXML md5
)

if(WIN32)
    target_compile_definitions(OpenLR2Lib PUBLIC NOMINMAX WIN32_LEAN_AND_MEAN)
    target_link_libraries(OpenLR2Lib PUBLIC winmm.lib)
endif()

if(MSVC)
    target_compile_options(OpenLR2Lib PUBLIC /utf-8)
endif()

add_executable(OpenLR2 WIN32 LR2/Main.cpp OpenLR2.manifest)
set_target_properties(OpenLR2 PROPERTIES CXX_STANDARD 23)
target_link_libraries(OpenLR2 PUBLIC OpenLR2Lib)

if(WIN32)
    # FIXME: SkinEditor manifest
    add_executable(
        SkinEditor
        WIN32
        SkinEditor/ImageLoader.cpp
        SkinEditor/imgui/imgui.cpp
        SkinEditor/imgui/imgui_demo.cpp
        SkinEditor/imgui/imgui_draw.cpp
        SkinEditor/imgui/imgui_impl_sdl3.cpp
        SkinEditor/imgui/imgui_impl_sdlrenderer3.cpp
        SkinEditor/imgui/imgui_tables.cpp
        SkinEditor/imgui/imgui_widgets.cpp
        SkinEditor/main.cpp
        SkinEditor/ImageLoader.cpp
        SkinEditor/seWindowManager.cpp
        SkinEditor/skin.cpp
        SkinEditor/winWorkspace.cpp
    )
    set_target_properties(SkinEditor PROPERTIES CXX_STANDARD 23)
    target_link_libraries(
        SkinEditor
        PUBLIC OpenLR2Lib ${CMAKE_CURRENT_LIST_DIR}/SkinEditor/SDL3.lib
    )
    target_include_directories(SkinEditor PUBLIC SkinEditor/SDL3/include)
else()
    message(WARNING "SkinEditor is not supported for this platform")
endif()
