cmake_minimum_required(VERSION 3.10)

set(UIPLUGIN dtkuiplugin)
project(${UIPLUGIN} VERSION 1.0.0 LANGUAGES CXX)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS UiPlugin Gui)

set(LIB_DWIDGET ${LIB_NAME})

if (CMAKE_PROJECT_NAME STREQUAL ${UIPLUGIN})
    message("compile ${UIPLUGIN} individually")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    include(GNUInstallDirs)
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set(CMAKE_INSTALL_PREFIX /usr)
    endif()
    find_package(DtkWidget REQUIRED)
    set(LIB_DWIDGET Dtk::Widget)
endif()

add_library(
    ${UIPLUGIN} SHARED
    dcustomerplugins.h
    dcustomerplugins.cpp
    dcustomermacrowidget.h
    dtkuiplugin.qrc
)

target_link_libraries(
    ${UIPLUGIN} PRIVATE
    ${LIB_DWIDGET}
    Qt${QT_VERSION_MAJOR}::UiPlugin
    Qt${QT_VERSION_MAJOR}::GuiPrivate
)

set(INSTALL_PLUGIN OFF CACHE BOOL "Install dtk designer plugin")
if(INSTALL_PLUGIN)
    set(QT_PLUGIN_DESIGNER_PATH "qt5/plugins/designer")
    install(TARGETS ${UIPLUGIN} DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}/${QT_PLUGIN_DESIGNER_PATH}")
    message(STATUS "Install path:" "${CMAKE_INSTALL_FULL_LIBDIR}/${QT_PLUGIN_DESIGNER_PATH}")
endif()
