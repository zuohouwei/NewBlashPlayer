cmake_minimum_required(VERSION 3.0)
project(XGUI)

set(CMAKE_CXX_STANDARD 98)

include(FindPkgConfig)

pkg_search_module(FT2 REQUIRED freetype2)

include_directories(
    ${FT2_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(SRC_SET
        Button.cpp
        Input.cpp
        Painter.cpp
        FontXft.cpp
        Screen.cpp
        ScrollArea.cpp
        SurfaceView.cpp
        ScrollBar.cpp
        TextBuffer.cpp
        TextLine.cpp
        TextSelection.cpp
        TextView.cpp
        Types.cpp
        Widget.cpp
        Window.cpp
        XApp.cpp)

add_compile_options(-D_XGUI_USE_XDBE)

add_library(XGUI STATIC ${SRC_SET})
target_link_libraries(XGUI
        libev)

set_property(TARGET XGUI PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})