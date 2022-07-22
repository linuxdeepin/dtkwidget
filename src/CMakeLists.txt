# SET the version
#cmake_minimum_required(VERSION 3.5)

set(LIB_NAME ${PROJECT_NAME})
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# find_package 
find_package(DtkCMake REQUIRED)
find_package(DtkGui REQUIRED)
find_package(DtkCore REQUIRED)
#find_package(DtkWidget REQUIRED)

find_package(Qt5 REQUIRED COMPONENTS Core)
find_package(Qt5 REQUIRED COMPONENTS Network)
find_package(Qt5 REQUIRED COMPONENTS Concurrent)
find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt5 REQUIRED COMPONENTS PrintSupport)
find_package(Qt5 REQUIRED COMPONENTS Multimedia)

find_package(PkgConfig REQUIRED)

pkg_check_modules(QGSettings REQUIRED gsettings-qt)
pkg_check_modules(STARUPNOTIFICATION REQUIRED libstartup-notification-1.0)
pkg_check_modules(XEXT REQUIRED xext)
pkg_check_modules(XI REQUIRED xi)

file(GLOB WIDGETPROVATEHEADER widgets/private/*.h)
file(GLOB WIDGETPROVATESOURCE widgets/private/*.cpp)

set(ICONS widgets/icons.qrc)
set(ICONTHEME widgets/assets/icons/dtk-icon-theme.qrc)
file(GLOB UTILHEAEDER ../include/dtkwidget/util/*.h)
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


#if(LINUX)
file(GLOB PRIVATEMPRISHEADER widgets/private/mpris/*.h)
file(GLOB PRIVATEMPRISSOURCE widgets/private/mpris/*.cpp)
file(GLOB WIDGETSOURCE widgets/*.cpp)
file(GLOB WIDGETHEADER ../include/dtkwidget/widgets/*)
file(GLOB SETTIONS widgets/private/settings/*)
file(GLOB NOTIFICATION widgets/private/startupnotifications/*)



add_definitions(-DSN_API_NOT_YET_FROZEN)
#TrainslateDir
add_definitions(-DDWIDGET_TRANSLATIONS_DIR="${PROJECT_NAME}/DWidget/translate")
find_package(Qt5 COMPONENTS X11Extras)
find_package(Qt5 COMPONENTS DBus)
add_library(${PROJECT_NAME} SHARED
  # Header

  #WIDGETS
  ${WIDGETHEADER}
  ${WIDGETSOURCE}
  ${PRIVATEMPRISHEADER}
  ${PRIVATEMPRISSOURCE}
  widgets/private/keyboardmonitor/dkeyboardmonitor.h
  widgets/private/keyboardmonitor/dkeyboardmonitor.cpp
  ${WIDGETPROVATESOURCE}
  ${WIDGETPROVATEHEADER}  
  ${SETTIONS}
  ${NOTIFICATION}

  #UTILS
  ${UTILHEAEDER}
  util/private/dregionmonitor_p.h
  
  util/dfileiconprovider.cpp 
  util/dthumbnailprovider.cpp 
  util/dwidgetutil.cpp 
  util/dhidpihelper.cpp 
  util/dapplicationsettings.cpp 
  util/daccessibilitychecker.cpp	
  util/ddesktopservices_linux.cpp 
  util/dtrashmanager_linux.cpp 
  util/dregionmonitor.cpp

  ${ICONS}
  ${ICONTHEME}
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
  #Qt5::WaylandCompositor
  #Qt5::XcbQpa
  Qt5::Multimedia
  ${DtkGui_LIBRARIES}
  ${DtkCore_LIBRARIES}
  ${QGSettings_LIBRARIES}
  ${STARUPNOTIFICATION_LIBRARIES}
  ${XEXT_LIBRARIES}
  ${XI_LIBRARIES}
)
target_include_directories(${LIB_NAME} PUBLIC
  ../include/dtkwidget/DWidget/
  ../include/dtkwidget/util/
  ../include/dtkwidget/widgets/
  ../include/dtkwidget/global/
  ../include/
  #widgets/private/startupnotifications/
  platforms/x11
  ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
  ${Qt5Widgets_PRIVATE_INCLUDE_DIRS}
  ${Qt5PrintSupport_PRIVATE_INCLUDE_DIRS}
  ${QGSettings_INCLUDE_DIRS}
  ${STARUPNOTIFICATION_INCLUDE_DIRS}
  ${XEXT_INCLUDE_DIRS}
  ${XI_INCLUDE_DIRS}
)
set_target_properties(${LIB_NAME} PROPERTIES
    VERSION ${CMAKE_PROJECT_VERSION}
    SOVERSION ${CMAKE_PROJECT_VERSION_MAJOR}
)
install(TARGETS ${LIB_NAME} DESTINATION lib/)
install(DIRECTORY ../include/dtkwidget/DWidget/ DESTINATION include/libdtk/DWidget/ FILES_MATCHING PATTERN "*")
install(DIRECTORY ../include/dtkwidget/util/ DESTINATION include/libdtk/DWidget/ FILES_MATCHING PATTERN "*")
install(DIRECTORY ../include/dtkwidget/global/ DESTINATION include/libdtk/DWidget/ FILES_MATCHING PATTERN "*")
install(DIRECTORY ../include/dtkwidget/widgets/ DESTINATION include/libdtk/DWidget/ FILES_MATCHING PATTERN "*")

install(DIRECTORY translations DESTINATION share/libdtk/DWidget/)
#target_link_directories(${PROJECT_NAME} PUBLIC .)
#message(STATUS ${Qt5Gui_PRIVATE_INCLUDE_DIRS})
#endif(LINUX)

#if(APPLE)
#	find_package(Qt5 COMPONENTS Svg)
#	find_package(Qt5 COMPONENTS DBus)
#endif(APPLE)
#
#if(WIN32)
#	find_package(Qt5 COMPONENTS Svg)
#endif(WIN32)
