cmake_minimum_required(VERSION 3.13)

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" FILE_VERSION)
string(STRIP "${FILE_VERSION}" FILE_VERSION)

project(DtkWidget
    VERSION ${FILE_VERSION}
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

option(DTK5 "Build DTK5." ON)
if(DTK5)
    set(DTK_VERSION_MAJOR "5")
    set(DTK_NAME_SUFFIX "")
else()
    set(DTK_VERSION_MAJOR "6")
    set(DTK_NAME_SUFFIX "6")
endif()

set(DTK_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(DTK_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(DTK_VERSION "${DTK_VERSION_MAJOR}.${DTK_VERSION_MINOR}.${DTK_VERSION_PATCH}")
set(QT_VERSION_MAJOR ${DTK_VERSION_MAJOR})

include(dtkwidget.cmake)
