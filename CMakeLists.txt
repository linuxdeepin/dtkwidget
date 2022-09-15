cmake_minimum_required (VERSION 3.10)

set (DVERSION "5.6.0.2" CACHE STRING "define project version")
project (DtkWidget
  VERSION ${DVERSION}
  DESCRIPTION "DTK Widget module"
  HOMEPAGE_URL "https://github.com/linuxdeepin/dtkwidget"
  LANGUAGES CXX C
)
#set(BREAKVERSION "5")
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Set build option
set (BUILD_PLUGINS ON CACHE BOOL "BUILD PLUGIN EXAMPLES")

# Set install path
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX /usr)
endif ()
set (INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_FULL_INCLUDEDIR}/libdtk-${CMAKE_PROJECT_VERSION}/DWidget")
set (TOOL_INSTALL_DIR "${CMAKE_INSTALL_FULL_LIBDIR}/libdtk-${CMAKE_PROJECT_VERSION}/DWidget/bin")
set (LIBRARY_INSTALL_DIR "${CMAKE_INSTALL_FULL_LIBDIR}")
set (MKSPECS_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/qt5/mkspecs/modules" CACHE STRING "INSTALL DIR FOR qt pri files")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -Wextra")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--as-needed")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fsanitize=address -fno-omit-frame-pointer")
  set(BUILD_TESTING ON)
else ()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")
endif ()


add_subdirectory(src)

add_subdirectory(examples)
add_subdirectory(tools)

if(BUILD_TESTING)
  message("==================================")
  message("       Now Testing is enabled     ")
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
find_package (Qt5 CONFIG REQUIRED COMPONENTS DBus Xml)

set (BUILD_DOCS ON CACHE BOOL "Generate doxygen-based documentation")

if (BUILD_DOCS)
  add_subdirectory(docs)
endif ()

configure_package_config_file(misc/DtkWidgetConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/DtkWidgetConfig.cmake
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/DtkWidget"
    PATH_VARS INCLUDE_INSTALL_DIR LIBRARY_INSTALL_DIR TOOL_INSTALL_DIR
)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/DtkWidgetConfigVersion.cmake"
    VERSION ${DVERSION}
    COMPATIBILITY SameMajorVersion
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/DtkWidgetConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/DtkWidgetConfigVersion.cmake
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/DtkWidget"
)

configure_file(misc/dtkwidget.pc.in dtkwidget.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/dtkwidget.pc DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

configure_file(misc/qt_lib_dtkwidget.pri.in qt_lib_dtkwidget.pri @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qt_lib_dtkwidget.pri DESTINATION "${MKSPECS_INSTALL_DIR}")
