cmake_minimum_required (VERSION 3.10)

option(DVERSION "define project version" "5.5.22")
if(DVERSION)
  project (DtkWidget
    VERSION ${DVERSION}
    DESCRIPTION "DTK Widget module"
    HOMEPAGE_URL "https://github.com/linuxdeepin/dtkwidget"
    LANGUAGES CXX C
  )
else()
  project (DtkWidget
    #VERSION "${DTK_REPO_MODULE_VERSION}"
    VERSION "5.5.23"
    DESCRIPTION "DTK Widget module"
    HOMEPAGE_URL "https://github.com/linuxdeepin/dtkwidget"
    LANGUAGES CXX C
  )
endif()
#set(BREAKVERSION "5")
include(GNUInstallDirs)
# set project name
#message(${CMAKE_INSTALL_LIBDIR})
option(NOTPACKAGE "to make package" ON)
option(LINUXNAME "linuxname" OFF)
set(SPECPATH "qt5/mkspecs/modules/")
if(LINUXNAME)
  if (${LINUXNAME} STREQUAL "archlinux")
    set(SPECPATH "qt/mkspecs/modules")
    message("arch")
  endif()
else()
  set(LINUXNAME "debian")
endif()
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX /usr)
endif ()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -Wextra")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--as-needed")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fsanitize=address -fno-omit-frame-pointer")
else ()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")
endif ()


add_subdirectory(src)

add_subdirectory(examples)
add_subdirectory(tools)
if(NOTPACKAGE)
  message("it is testing mode ,if you want to package ,set NOTPACKAGE OFF")
  add_subdirectory(plugin)
  add_subdirectory(tests)
endif()

find_package (Qt5 CONFIG REQUIRED COMPONENTS DBus Xml)

set (BUILD_DOCS ON CACHE BOOL "Generate doxygen-based documentation")

if (BUILD_DOCS)
  add_subdirectory(doc)
endif ()

configure_file(misc/DtkWidgetConfig.cmake.in DtkWidgetConfig.cmake @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/DtkWidgetConfig.cmake DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/DtkWidget/")

configure_file(misc/dtkwidget.pc.in dtkwidget.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/dtkwidget.pc DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

configure_file(misc/qt_lib_dtkwidget.pri.in qt_lib_dtkwidget.pri @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qt_lib_dtkwidget.pri DESTINATION "${CMAKE_INSTALL_LIBDIR}/${SPECPATH}")
