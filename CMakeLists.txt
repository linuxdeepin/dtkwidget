cmake_minimum_required(VERSION 3.13)

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" DTK_FILE_VERSION)
string(STRIP "${DTK_FILE_VERSION}" DTK_FILE_VERSION)
set(DTK_VERSION "${DTK_FILE_VERSION}" CACHE STRING "define project version")

project(DtkWidget
    VERSION ${DTK_VERSION}
    DESCRIPTION "DTK Widget module"
    HOMEPAGE_URL "https://github.com/linuxdeepin/dtkwidget"
    LANGUAGES CXX C
)

# Set install path
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX /usr)
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if("${PROJECT_VERSION_MAJOR}" STREQUAL "5")
    set(QT_VERSION_MAJOR 5)
    include(dtkwidget.cmake)
elseif("${PROJECT_VERSION_MAJOR}" STREQUAL "6")
    set(QT_VERSION_MAJOR "6")
    set(DTK_VERSION_MAJOR "6")
    include(dtkwidget.cmake)
else()
    message(SEND_ERROR "not support Prject Version ${PROJECT_VERSION}.")
endif()
