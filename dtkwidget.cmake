set(LIBNAME dtk${DTK_VERSION_MAJOR}widget)
set(DtkWidget Dtk${DTK_VERSION_MAJOR}Widget)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

message(STATUS "Current Qt Version: ${QT_VERSION_MAJOR}")
message(STATUS "Current Dtk Version: ${DTK_VERSION_MAJOR}")

set(BUILD_EXAMPLES ON CACHE BOOL "Build examples")
set(BUILD_VERSION "0" CACHE STRING "buildversion")
set(BUILD_PLUGINS ON CACHE BOOL "Build plugin and plugin example")

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
    "${CMAKE_INSTALL_LIBDIR}/qt${QT_VERSION_MAJOR}/mkspecs/modules"
    CACHE STRING "Install dir for qt pri files"
)
set(CONFIG_CMAKE_INSTALL_DIR
    "${CMAKE_INSTALL_LIBDIR}/cmake/${DtkWidget}"
    CACHE STRING "Install directory for cmake files"
)
set(PKGCONFIG_INSTALL_DIR
    "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
    CACHE STRING "Install directory for pkgconfig files"
)

find_package(Dtk${DTK_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core
    Network
    Concurrent
    Widgets
    PrintSupport
    LinguistTools
    DBus)
find_package(PkgConfig REQUIRED)

if("${QT_VERSION_MAJOR}" STREQUAL "5")
    find_package(Qt5 REQUIRED COMPONENTS X11Extras)
elseif("${QT_VERSION_MAJOR}" STREQUAL "6")
    if (${Qt6Core_VERSION} VERSION_GREATER_EQUAL "6.10.0")
      set(QT_NO_PRIVATE_MODULE_WARNING ON)
      find_package(Qt6 REQUIRED COMPONENTS GuiPrivate WidgetsPrivate PrintSupportPrivate)
    endif()
else()
    message(FATAL_ERROR "Unsupported Qt Version: ${QT_VERSION_MAJOR}")
endif()

file(GLOB D_HEADERS "${PROJECT_SOURCE_DIR}/include/DWidget/*")
if("${DTK_VERSION_MAJOR}" STREQUAL "6")
  list(REMOVE_ITEM D_HEADERS "${PROJECT_SOURCE_DIR}/include/DWidget/DApplicationHelper")
  list(REMOVE_ITEM D_HEADERS "${PROJECT_SOURCE_DIR}/include/DWidget/DApplicationSettings")
  list(REMOVE_ITEM D_HEADERS "${PROJECT_SOURCE_DIR}/include/DWidget/DArrowLineExpand")
  list(REMOVE_ITEM D_HEADERS "${PROJECT_SOURCE_DIR}/include/DWidget/DExpandGroup")
  list(REMOVE_ITEM D_HEADERS "${PROJECT_SOURCE_DIR}/include/DWidget/DImageButton")
  list(REMOVE_ITEM D_HEADERS "${PROJECT_SOURCE_DIR}/include/DWidget/DSegmentedControl")
  list(REMOVE_ITEM D_HEADERS "${PROJECT_SOURCE_DIR}/include/DWidget/DSegmentedHighlight")
  list(REMOVE_ITEM D_HEADERS "${PROJECT_SOURCE_DIR}/include/DWidget/DThemeManager")
  list(REMOVE_ITEM D_HEADERS "${PROJECT_SOURCE_DIR}/include/DWidget/DToast")
endif()

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

list(APPEND PUBLIC_HEADERS ${D_HEADERS})
list(APPEND PUBLIC_HEADERS ${AUTOCONFIG})


set (BUILD_DOCS ON CACHE BOOL "Generate doxygen-based documentation")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wextra")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--as-needed -pie")
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_TESTING ON)
  endif ()
  string(REPLACE "-O3" "-Ofast" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
endif()

if (BUILD_DOCS)
  add_subdirectory(docs)
endif ()

add_subdirectory(src)
if(BUILD_TESTING)
  message(STATUS "==================================")
  message(STATUS "       Now Testing is enabled     ")
  message(STATUS "==================================")
  enable_testing()
  add_subdirectory(tests)
endif()
if(BUILD_EXAMPLES)
  message(STATUS "===================================")
  message(STATUS "You can build and run examples now ")
  message(STATUS "===================================")
  add_subdirectory(examples)
endif()
add_subdirectory(tools)

if(BUILD_PLUGINS)
  message(STATUS "===================================")
  message(STATUS "Plugins will be built later")
  message(STATUS "===================================")
  add_subdirectory(plugin)
endif()

configure_package_config_file(misc/DtkWidgetConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${DtkWidget}Config.cmake
    INSTALL_DESTINATION ${CONFIG_CMAKE_INSTALL_DIR}
    PATH_VARS TOOL_INSTALL_DIR)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${DtkWidget}ConfigVersion.cmake"
    VERSION ${DTK_VERSION}
    COMPATIBILITY SameMajorVersion
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${DtkWidget}Config.cmake DESTINATION ${CONFIG_CMAKE_INSTALL_DIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${DtkWidget}ConfigVersion.cmake DESTINATION ${CONFIG_CMAKE_INSTALL_DIR})

configure_file(misc/DtkWidget.pc.in ${LIBNAME}.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${LIBNAME}.pc DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

configure_file(misc/qt_lib_DtkWidget.pri.in qt_lib_DtkWidget.pri @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qt_lib_DtkWidget.pri DESTINATION "${MKSPECS_INSTALL_DIR}")
