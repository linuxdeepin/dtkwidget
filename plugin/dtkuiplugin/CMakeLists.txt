set(UIPLUGIN dtkuiplugin)
project(${UIPLUGIN} VERSION 1.0.0 LANGUAGES CXX)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS UiPlugin Gui)

set(LIB_DWIDGET ${LIBNAME})

if (CMAKE_PROJECT_NAME STREQUAL ${UIPLUGIN})
    message(STATUS "compile ${UIPLUGIN} individually")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    include(GNUInstallDirs)
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set(CMAKE_INSTALL_PREFIX /usr)
    endif()
    find_package(Dtk${DTK_VERSION_MAJOR} REQUIRED COMPONENT Widget)
    set(LIB_DWIDGET Dtk${DTK_VERSION_MAJOR}::Widget)
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
    set(QT_PLUGIN_DESIGNER_PATH "qt${QT_VERSION_MAJOR}/plugins/designer")
    install(TARGETS ${UIPLUGIN} DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}/${QT_PLUGIN_DESIGNER_PATH}")
    message(STATUS "Install path:" "${CMAKE_INSTALL_FULL_LIBDIR}/${QT_PLUGIN_DESIGNER_PATH}")
endif()
