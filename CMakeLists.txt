cmake_minimum_required(VERSION 3.10)

set(VERSION
    "5.6.8"
    CACHE STRING "define project version"
)

project(DtkWidget
    VERSION ${VERSION}
    DESCRIPTION "DTK Widget module"
    HOMEPAGE_URL "https://github.com/linuxdeepin/dtkwidget"
    LANGUAGES CXX C
)

set(LIB_NAME dtkwidget)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

# Set build option
set(BUILD_PLUGINS
    ON
    CACHE BOOL "Build plugin and plugin example"
)

# Set install path
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX /usr)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(INCLUDE_INSTALL_DIR
    "${CMAKE_INSTALL_INCLUDEDIR}/dtk${PROJECT_VERSION_MAJOR}/DWidget"
)
set(TOOL_INSTALL_DIR
    "${CMAKE_INSTALL_LIBDIR}/dtk${PROJECT_VERSION_MAJOR}/DWidget/bin"
)
set(LIBRARY_INSTALL_DIR
    "${CMAKE_INSTALL_LIBDIR}"
)
set(MKSPECS_INSTALL_DIR
    "${CMAKE_INSTALL_LIBDIR}/qt5/mkspecs/modules"
    CACHE STRING "Install dir for qt pri files"
)
set(CONFIG_INSTALL_DIR
    "${CMAKE_INSTALL_LIBDIR}/cmake/DtkWidget"
    CACHE STRING "Install directory for cmake files"
)
set(PKGCONFIG_INSTALL_DIR
    "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
    CACHE STRING "Install directory for pkgconfig files"
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -Wextra")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--as-needed")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_TESTING ON)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")
endif()
# find_package
find_package(Dtk REQUIRED COMPONENTS Core Gui)
find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core)
find_package(Qt5 REQUIRED COMPONENTS
    Core
    Network
    Concurrent
    Widgets
    PrintSupport
    LinguistTools
    X11Extras
    DBus
)
find_package(PkgConfig REQUIRED)

file(GLOB D_HEADERS "${PROJECT_SOURCE_DIR}/include/DWidget/*")

set(AUTOCONFIG ${CMAKE_CURRENT_BINARY_DIR}/dtkwidget_config.h)
get_filename_component(CONFIG_INCLUDE ${AUTOCONFIG} DIRECTORY)
set(CONFIG_CONTENT)
string(APPEND CONFIG_CONTENT "// This is an auto-generated config\n")

foreach(header ${D_HEADERS})
    get_filename_component(thename ${header} NAME)
    string(APPEND CONFIG_CONTENT "#define DTKWIDGET_CLASS_${thename}\n")
endforeach()

file(WRITE ${AUTOCONFIG} ${CONFIG_CONTENT})

file(GLOB_RECURSE PUBLIC_HEADERS "${PROJECT_SOURCE_DIR}/include/*.h")
list(APPEND PUBLIC_HEADERS ${D_HEADERS} ${AUTOCONFIG})

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(tools)

if(BUILD_TESTING)
    message("==================================")
    message("      Now Testing is enabled      ")
    message("==================================")
    enable_testing()
    add_subdirectory(tests)
endif()

if(BUILD_PLUGINS)
    message("===================================")
    message(" You can build and run plugins now ")
    message("===================================")
    add_subdirectory(plugin)
endif()

set(BUILD_DOCS
    OFF
    CACHE BOOL "Generate doxygen-based documentation"
)

if(BUILD_DOCS)
    message("===================================")
    message("     You can build docs  now       ")
    message("===================================")
    add_subdirectory(docs)
endif()

configure_package_config_file(
    misc/DtkWidgetConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/DtkWidgetConfig.cmake
    INSTALL_DESTINATION ${CONFIG_INSTALL_DIR}
    PATH_VARS TOOL_INSTALL_DIR
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/DtkWidgetConfigVersion.cmake
    VERSION ${VERSION}
    COMPATIBILITY SameMajorVersion
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/DtkWidgetConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/DtkWidgetConfigVersion.cmake
    DESTINATION ${CONFIG_INSTALL_DIR}
)

configure_file(misc/dtkwidget.pc.in dtkwidget.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/dtkwidget.pc DESTINATION ${PKGCONFIG_INSTALL_DIR})

configure_file(misc/qt_lib_dtkwidget.pri.in qt_lib_dtkwidget.pri @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qt_lib_dtkwidget.pri DESTINATION ${MKSPECS_INSTALL_DIR})
