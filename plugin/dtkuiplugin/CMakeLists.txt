cmake_minimum_required(VERSION 3.5)

set(UIPLUGIN dtkuiplugin)
project(${UIPLUGIN} VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(DtkGui REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS UiPlugin)

set(LIB_DWIDGET dtkwidget)
set(INC_DWIDGET)
if (CMAKE_PROJECT_NAME STREQUAL ${UIPLUGIN})
    message("compile ${UIPLUGIN} individually")
    include(GNUInstallDirs)
    if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set(CMAKE_INSTALL_PREFIX /usr)
    endif ()
    find_package(DtkWidget REQUIRED)
    set(LIB_DWIDGET ${DtkWidget_LIBRARIES})
    set(INC_DWIDGET ${DTKWIDGET_INCLUDE_DIR})
else()
    list(APPEND INC_DWIDGET ${CMAKE_CURRENT_LIST_DIR}/../../../include/dtkwidget/DWidget)
    list(APPEND INC_DWIDGET ${CMAKE_CURRENT_LIST_DIR}/../../../include/dtkwidget/widgets)
    list(APPEND INC_DWIDGET ${CMAKE_CURRENT_LIST_DIR}/../../../include/dtkwidget/util)
    list(APPEND INC_DWIDGET ${CMAKE_CURRENT_LIST_DIR}/../../../include/dtkwidget/global)
endif()

set(RCC_FILES dtkuiplugin.qrc)
add_library(
    ${UIPLUGIN} SHARED
    dcustomerplugins.h
    dcustomerplugins.cpp
    dcustomermacrowidget.h
    ${RCC_FILES}
)

target_link_libraries(
    ${UIPLUGIN} PRIVATE
    ${LIB_DWIDGET}
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::UiPlugin
)

target_include_directories(${UIPLUGIN} PUBLIC
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
    ${INC_DWIDGET}
)

set (INSTALL_PLUGIN OFF CACHE BOOL "install dtk designer plugin")
if (INSTALL_PLUGIN)
    set(QT_PLUGIN_DESIGNER_PATH "qt5/plugins/designer/")
    install(TARGETS ${UIPLUGIN} DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}/${QT_PLUGIN_DESIGNER_PATH}")
    message("install path:" "${CMAKE_INSTALL_FULL_LIBDIR}/${QT_PLUGIN_DESIGNER_PATH}")
endif()
