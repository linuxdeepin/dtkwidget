# SET the version
#cmake_minimum_required(VERSION 3.5)

set(LIB_NAME dtkwidget)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# find_package 
find_package(DtkGui REQUIRED)
find_package(DtkCore REQUIRED)
#find_package(DtkWidget REQUIRED)

find_package(Qt5 REQUIRED COMPONENTS Core)
find_package(Qt5 REQUIRED COMPONENTS Network)
find_package(Qt5 REQUIRED COMPONENTS Concurrent)
find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt5 REQUIRED COMPONENTS PrintSupport)
#find_package(Qt5 REQUIRED COMPONENTS Multimedia)
find_package(Qt5 REQUIRED COMPONENTS LinguistTools)

find_package(PkgConfig REQUIRED)

pkg_check_modules(QGSettings REQUIRED gsettings-qt)
pkg_check_modules(XCBUTIL REQUIRED xcb-util) 
pkg_check_modules(STARUPNOTIFICATION REQUIRED libstartup-notification-1.0)
pkg_check_modules(XEXT REQUIRED xext)
pkg_check_modules(XI REQUIRED xi)
pkg_check_modules(X11 REQUIRED x11)

include(util/util.cmake)
include(widgets/widgets.cmake)

#file(GLOB UTILHEAEDER ../include/dtkwidget/util/*.h)
if (UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()

set(CONFIGNAME ../include/dtkwidget/global/dtkwidget_config.h)
file(WRITE ${CONFIGNAME} "// it is auto make config\n")
file(GLOB CONFIGSOURCE ../include/dtkwidget/DWidget/*)

foreach(FILENAME ${CONFIGSOURCE})
  get_filename_component(thefile ${FILENAME} NAME)
  file(APPEND ${CONFIGNAME} "#define DTKWIDGET_CLASS_${thefile}\n")
endforeach()

file(GLOB TRANSLATE translations/*.ts)
qt5_add_translation(TRANSLATEDFILES ${TRANSLATE})

add_definitions(-DSN_API_NOT_YET_FROZEN)
add_definitions(-DDTK_NO_MULTIMEDIA)
#TrainslateDir
add_definitions(-DDWIDGET_TRANSLATIONS_DIR="libdtk-${CMAKE_PROJECT_VERSION}/DWidget/translations")
find_package(Qt5 COMPONENTS X11Extras)
find_package(Qt5 COMPONENTS DBus)
add_library(${LIB_NAME} SHARED
  ${widgets_SRC}
  ${TRANSLATEDFILES}
  ${util_SRC}
)
target_link_libraries(
  ${LIB_NAME} PRIVATE
  Qt5::Widgets
  Qt5::Network
  Qt5::Concurrent
  Qt5::Core
  Qt5::DBus
  Qt5::X11Extras
  Qt5::PrintSupport
  ${DtkGui_LIBRARIES}
  ${DtkCore_LIBRARIES}
  ${QGSettings_LIBRARIES}
  ${STARUPNOTIFICATION_LIBRARIES}
  ${XEXT_LIBRARIES}
  ${XI_LIBRARIES}
  ${X11_LIBRARIES}
  ${XCBUTIL_LIBRARIES}
)
target_include_directories(${LIB_NAME} PUBLIC
  ../include/dtkwidget/DWidget/
  ../include/dtkwidget/util/
  ../include/dtkwidget/widgets/
  ../include/dtkwidget/global/
  ../include/

  ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
  ${Qt5Widgets_PRIVATE_INCLUDE_DIRS}
  ${Qt5PrintSupport_PRIVATE_INCLUDE_DIRS}
  ${QGSettings_INCLUDE_DIRS}
  ${STARUPNOTIFICATION_INCLUDE_DIRS}
  ${XEXT_INCLUDE_DIRS}
  ${XI_INCLUDE_DIRS}
  ${X11_INCLUDE_DIRS}
  ${XCBUTIL_INCLUDE_DIRS}
)
set_target_properties(${LIB_NAME} PROPERTIES
    VERSION ${CMAKE_PROJECT_VERSION}
    SOVERSION ${CMAKE_PROJECT_VERSION_MAJOR}
)
install(TARGETS ${LIB_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY ../include/dtkwidget/DWidget/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/libdtk-${CMAKE_PROJECT_VERSION}/DWidget/" FILES_MATCHING PATTERN "*")
install(DIRECTORY ../include/dtkwidget/util/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/libdtk-${CMAKE_PROJECT_VERSION}/DWidget/" FILES_MATCHING PATTERN "*")
install(DIRECTORY ../include/dtkwidget/global/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/libdtk-${CMAKE_PROJECT_VERSION}/DWidget/" FILES_MATCHING PATTERN "*")
install(DIRECTORY ../include/dtkwidget/widgets/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/libdtk-${CMAKE_PROJECT_VERSION}/DWidget/" FILES_MATCHING PATTERN "*")

install(FILES ${TRANSLATEDFILES} DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/libdtk-${CMAKE_PROJECT_VERSION}/DWidget/translations")
