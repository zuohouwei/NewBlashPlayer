cmake_minimum_required(VERSION 3.0)
project(NBMediaPlayerDemo)

set(CMAKE_CXX_STANDARD 98)

add_compile_options(-DBUILD_TARGET_LINUX64)

include(FindPkgConfig)

pkg_search_module(FT2 REQUIRED freetype2)
pkg_search_module(XExt REQUIRED xext)
pkg_search_module(XFt REQUIRED xft)
pkg_search_module(XRender REQUIRED xrender)

include_directories(${FT2_INCLUDE_DIRS})

add_executable(NewBlashPlayer
        NBMainApp.cpp
        NBMainWindow.cpp
        NBGLSurface.cpp
        main.cpp)

add_subdirectory(XGUI)

set(BUILD_DEPS
        XGUI
        ${XFt_LIBRARIES}
        ${XRender_LIBRARIES}
        ${XExt_LIBRARIES}
        ${FT2_LIBRARIES})

target_link_libraries(NewBlashPlayer
        NBMediaPlayer
        GLU
        GL
        X11
        ${BUILD_DEPS}
        pthread)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../NBMediaPlayer/platform/Ubuntu/NBAVPlayer NBAVPlayer)
