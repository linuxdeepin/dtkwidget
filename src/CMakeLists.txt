# SET the version
cmake_minimum_required(VERSION 3.5)

set(DVERSION 5.5)
# set project name
project(dtkwidget 
  VERSION ${DVERSION}
  LANGUAGES CXX
)

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
#file(GLOB WIDGETHEADER widgets/*.h)
#file(GLOB WIDGETSOURCE widgets/*)

file(GLOB PROVATEHEADER widgets/private/*.h)
file(GLOB PROVATESOURCE widgets/private/*.cpp)

set(ICONS widgets/icons.qrc)
set(ICONTHEME widgets/assets/icons/dtk-icon-theme.qrc)
#file(GLOB UTIL util/*)
#file(GLOB UTILPRIVATE util/private/*)
# define if is LINUX
if (UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()

#if(LINUX)
file(GLOB PRIVATEMPRISHEADER widgets/private/mpris/*.h)
file(GLOB PRIVATEMPRISSOURCE widgets/private/mpris/*.cpp)
file(GLOB WIDGETSOURCE widgets/*.cpp)
file(GLOB WIDGETHEADER widgets/*.h)
file(GLOB SETTIONS widgets/private/settings/*)
file(GLOB NOTIFICATION widgets/private/startupnotifications/*)
add_definitions(-DSN_API_NOT_YET_FROZEN)
find_package(Qt5 COMPONENTS X11Extras)
find_package(Qt5 COMPONENTS DBus)
#find_package(Qt5 REQUIRED COMPONENTS WaylandCompositor) 
#find_package(Qt5 REQUIRED COMPONENTS XcbQpa) 
add_library(${PROJECT_NAME} SHARED
	# Header

  widgets/daboutdialog.cpp
  widgets/daboutdialog.h
  widgets/dabstractdialog.cpp
  widgets/dabstractdialog.h
  widgets/dalertcontrol.cpp
  widgets/dalertcontrol.h
  widgets/danchors.cpp
  widgets/danchors.h
  widgets/dapplication.cpp
  widgets/dapplication.h
  widgets/dapplicationhelper.cpp
  widgets/dapplicationhelper.h
  widgets/darrowbutton.cpp
  widgets/darrowbutton.h
  widgets/darrowlinedrawer.cpp
  widgets/darrowlinedrawer.h
  widgets/darrowlineexpand.cpp
  widgets/darrowlineexpand.h
  widgets/darrowrectangle.cpp
  widgets/darrowrectangle.h
  widgets/dbackgroundgroup.cpp
  widgets/dbackgroundgroup.h
  widgets/dbaseexpand.cpp
  widgets/dbaseexpand.h
  widgets/dbaseline.cpp
  widgets/dbaseline.h
  widgets/dblureffectwidget.cpp
  widgets/dblureffectwidget.h
  widgets/dboxwidget.cpp
  widgets/dboxwidget.h
  widgets/dbuttonbox.cpp
  widgets/dbuttonbox.h
  widgets/dcircleprogress.cpp
  widgets/dcircleprogress.h
  widgets/dclipeffectwidget.cpp
  widgets/dclipeffectwidget.h
  widgets/dcoloredprogressbar.cpp
  widgets/dcoloredprogressbar.h
  widgets/dcombobox.cpp
  widgets/dcombobox.h
  widgets/dcommandlinkbutton.cpp
  widgets/dcommandlinkbutton.h
  widgets/dconstants.h
  widgets/dcrumbedit.cpp
  widgets/dcrumbedit.h
  widgets/ddialog.cpp
  widgets/ddialog.h
  widgets/ddialogclosebutton.cpp
  widgets/ddialogclosebutton.h
  widgets/ddrawer.cpp
  widgets/ddrawer.h
  widgets/ddrawergroup.cpp
  widgets/ddrawergroup.h
  widgets/denhancedwidget.cpp
  widgets/denhancedwidget.h
  widgets/dexpandgroup.cpp
  widgets/dexpandgroup.h
  widgets/dfilechooseredit.cpp
  widgets/dfilechooseredit.h
  widgets/dfiledialog.cpp
  widgets/dfiledialog.h
  widgets/dfloatingbutton.cpp
  widgets/dfloatingbutton.h
  widgets/dfloatingmessage.cpp
  widgets/dfloatingmessage.h
  widgets/dfloatingwidget.cpp
  widgets/dfloatingwidget.h
  widgets/dflowlayout.cpp
  widgets/dflowlayout.h
  widgets/dfontcombobox.cpp
  widgets/dfontcombobox.h
  widgets/dframe.cpp
  widgets/dframe.h
  widgets/dgraphicsclipeffect.cpp
  widgets/dgraphicsclipeffect.h
  widgets/dgraphicsgloweffect.cpp
  widgets/dgraphicsgloweffect.h
  widgets/dheaderline.cpp
  widgets/dheaderline.h
  widgets/dialog_constants.h
  widgets/dialogs.pri
  widgets/diconbutton.cpp
  widgets/diconbutton.h
  widgets/dimagebutton.cpp
  widgets/dimagebutton.h
  widgets/dinputdialog.cpp
  widgets/dinputdialog.h
  #  widgets/dinputdialog_p.h
  widgets/dipv4lineedit.cpp
  widgets/dipv4lineedit.h
  widgets/dkeysequenceedit.cpp
  widgets/dkeysequenceedit.h
  widgets/dlabel.cpp
  widgets/dlabel.h
  widgets/dlineedit.cpp
  widgets/dlineedit.h
  widgets/dlistview.cpp
  widgets/dlistview.h
  widgets/dloadingindicator.cpp
  widgets/dloadingindicator.h
  widgets/dmainwindow.cpp
  widgets/dmainwindow.h
  widgets/dmessagemanager.cpp
  widgets/dmessagemanager.h
  widgets/dmpriscontrol.cpp
  widgets/dmpriscontrol.h
  widgets/dpageindicator.cpp
  widgets/dpageindicator.h
  widgets/dpalettehelper.cpp
  widgets/dpalettehelper.h
  widgets/dpasswordedit.cpp
  widgets/dpasswordedit.h
  widgets/dpicturesequenceview.cpp
  widgets/dpicturesequenceview.h
  widgets/dplatformwindowhandle.cpp
  widgets/dplatformwindowhandle.h
  widgets/dprintpickcolorwidget.cpp
  widgets/dprintpickcolorwidget.h
  widgets/dprintpreviewdialog.cpp
  widgets/dprintpreviewdialog.h
  widgets/dprintpreviewwidget.cpp
  widgets/dprintpreviewwidget.h
  widgets/dprogressbar.cpp
  widgets/dprogressbar.h
  widgets/dsearchcombobox.cpp
  widgets/dsearchcombobox.h
  widgets/dsearchedit.cpp
  widgets/dsearchedit.h
  widgets/dsegmentedcontrol.cpp
  widgets/dsegmentedcontrol.h
  widgets/dsettingsdialog.cpp
  widgets/dsettingsdialog.h
  widgets/dsettingswidgetfactory.cpp
  widgets/dsettingswidgetfactory.h
  widgets/dshadowline.cpp
  widgets/dshadowline.h
  widgets/dshortcutedit.cpp
  widgets/dshortcutedit.h
  widgets/dsimplelistitem.cpp
  widgets/dsimplelistitem.h
  widgets/dsimplelistview.cpp
  widgets/dsimplelistview.h
  widgets/dslider.cpp
  widgets/dslider.h
  widgets/dspinbox.cpp
  widgets/dspinbox.h
  widgets/dspinner.cpp
  widgets/dspinner.h
  widgets/dstackwidget.cpp
  widgets/dstackwidget.h
  widgets/dstyle.cpp
  widgets/dstyle.h
  widgets/dstyleditemdelegate.cpp
  widgets/dstyleditemdelegate.h
  widgets/dstyleoption.cpp
  widgets/dstyleoption.h
  widgets/dsuggestbutton.cpp
  widgets/dsuggestbutton.h
  widgets/dswitchbutton.cpp
  widgets/dswitchbutton.h
  widgets/dswitchlineexpand.cpp
  widgets/dswitchlineexpand.h
  widgets/dtabbar.cpp
  widgets/dtabbar.h
  widgets/dtabletwindowoptionbutton.cpp
  widgets/dtabletwindowoptionbutton.h
  widgets/dtextedit.cpp
  widgets/dtextedit.h
  widgets/dthememanager.cpp
  widgets/dthememanager.h
  widgets/dtickeffect.cpp
  widgets/dtickeffect.h
  widgets/dtiplabel.cpp
  widgets/dtiplabel.h
  widgets/dtitlebar.cpp
  widgets/dtitlebar.h
  widgets/dtoast.cpp
  widgets/dtoast.h
  widgets/dtoolbutton.cpp
  widgets/dtoolbutton.h
  widgets/dtooltip.cpp
  widgets/dtooltip.h
  widgets/dvideowidget.cpp
  widgets/dvideowidget.h
  widgets/dwarningbutton.cpp
  widgets/dwarningbutton.h
  widgets/dwaterprogress.cpp
  widgets/dwaterprogress.h
  widgets/dwidgetstype.h
  widgets/dwindowclosebutton.cpp
  widgets/dwindowclosebutton.h
  widgets/dwindowmaxbutton.cpp
  widgets/dwindowmaxbutton.h
  widgets/dwindowminbutton.cpp
  widgets/dwindowminbutton.h
  widgets/dwindowoptionbutton.cpp
  widgets/dwindowoptionbutton.h
  widgets/dwindowquitfullbutton.cpp
  widgets/dwindowquitfullbutton.h
  widgets/dx11window.h
  
  ${PRIVATEMPRISHEADER}
  ${PRIVATEMPRISSOURCE}
  widgets/private/keyboardmonitor/dkeyboardmonitor.h
  widgets/private/keyboardmonitor/dkeyboardmonitor.cpp
  
  ${PROVATESOURCE}
  ${PROVATEHEADER}
  ${SETTIONS}
  ${NOTIFICATION}
  util/dfileiconprovider.h 
  util/dthumbnailprovider.h 
  util/dwidgetutil.h 
  util/ddesktopservices.h 
  util/dtrashmanager.h 
  util/dhidpihelper.h 
  util/dapplicationsettings.h 
  util/daccessibilitychecker.h
  util/dregionmonitor.h 
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
  
  util/private/dregionmonitor_p.h
  dtkwidget_global.h
  ${ICONS}
  ${ICONTHEME}
)
target_link_libraries(
  ${PROJECT_NAME} PRIVATE
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
target_include_directories(${PROJECT_NAME} PUBLIC
  widgets/
  widgets/private/startupnotifications/
  util/
  platforms/x11
  ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
  ${Qt5Widgets_PRIVATE_INCLUDE_DIRS}
  ${Qt5PrintSupport_PRIVATE_INCLUDE_DIRS}
  ${QGSettings_INCLUDE_DIRS}
  ${STARUPNOTIFICATION_INCLUDE_DIRS}
  ${XEXT_INCLUDE_DIRS}
  ${XI_INCLUDE_DIRS}
)
target_link_directories(${PROJECT_NAME} PUBLIC .)
message(STATUS ${Qt5Gui_PRIVATE_INCLUDE_DIRS})
#endif(LINUX)

#if(APPLE)
#	find_package(Qt5 COMPONENTS Svg)
#	find_package(Qt5 COMPONENTS DBus)
#endif(APPLE)
#
#if(WIN32)
#	find_package(Qt5 COMPONENTS Svg)
#endif(WIN32)
