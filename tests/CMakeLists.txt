#cmake_minimum_required(VERSION 3.5)

set(BIN_NAME "ut-${PROJECT_NAME}")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "-fno-access-control")
add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

add_definitions(-DSN_API_NOT_YET_FROZEN)
add_definitions(-DDTK_NO_MULTIMEDIA)
#TrainslateDir
add_definitions(-DDWIDGET_TRANSLATIONS_DIR="libdtk-${CMAKE_PROJECT_VERSION}/DWidget/translate")


find_package(DtkGui REQUIRED)
find_package(DtkCore REQUIRED)
#find_package(DtkWidget REQUIRED)

find_package(Qt5 REQUIRED COMPONENTS Core)
find_package(Qt5 REQUIRED COMPONENTS Test)
find_package(Qt5 REQUIRED COMPONENTS Network)
find_package(Qt5 REQUIRED COMPONENTS Concurrent)
find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt5 REQUIRED COMPONENTS PrintSupport)
#find_package(Qt5 REQUIRED COMPONENTS Multimedia)
find_package(Qt5 COMPONENTS X11Extras)
find_package(Qt5 COMPONENTS DBus)

find_package(GTest REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(QGSettings REQUIRED gsettings-qt)
pkg_check_modules(XCBUTIL REQUIRED xcb-util) 
pkg_check_modules(STARUPNOTIFICATION REQUIRED libstartup-notification-1.0)
pkg_check_modules(XEXT REQUIRED xext)
pkg_check_modules(XI REQUIRED xi)
pkg_check_modules(X11 REQUIRED x11)


set(WIDGET_TEST 
  testcases/widgets/ut_daboutdialog.cpp
  testcases/widgets/ut_dabstractdialog.cpp
  testcases/widgets/ut_dalertcontrol.cpp
  testcases/widgets/ut_danchor.cpp
  # TODO break the test
  #testcases/widgets/ut_danchors.cpp
  testcases/widgets/ut_darrowbutton.cpp
  testcases/widgets/ut_darrowlinedrawer.cpp
  testcases/widgets/ut_darrowlineexpand.cpp
  testcases/widgets/ut_darrowrectangle.cpp
  testcases/widgets/ut_dbackgroundgroup.cpp
  testcases/widgets/ut_dbaseexpand.cpp
  testcases/widgets/ut_dbaseline.cpp
  testcases/widgets/ut_dblureffectwidget.cpp
  testcases/widgets/ut_dboxwidget.cpp
  testcases/widgets/ut_dbuttonbox.cpp
  testcases/widgets/ut_dcircleprogress.cpp
  testcases/widgets/ut_dclipeffectwidget.cpp
  testcases/widgets/ut_dcoloredprogressbar.cpp
  testcases/widgets/ut_dcommandlinkbutton.cpp
  testcases/widgets/ut_dcrumbedit.cpp
  testcases/widgets/ut_ddialog.cpp
  testcases/widgets/ut_ddialogclosebutton.cpp
  testcases/widgets/ut_ddrawer.cpp
  testcases/widgets/ut_ddrawergroup.cpp
  testcases/widgets/ut_denhancedwidget.cpp
  # TODO test break
  #testcases/widgets/ut_dexpandgroup.cpp
  testcases/widgets/ut_dfilechooseredit.cpp
  testcases/widgets/ut_dfiledialog.cpp
  testcases/widgets/ut_dfloatingbutton.cpp
  testcases/widgets/ut_dfloatingmessage.cpp
  testcases/widgets/ut_dfloatingwidget.cpp
  # TODO break
  # testcases/widgets/ut_dflowlayout.cpp
  testcases/widgets/ut_dframe.cpp
  testcases/widgets/ut_dgraphicsclipeffect.cpp
  testcases/widgets/ut_dgraphicsgloweffect.cpp
  testcases/widgets/ut_dheaderline.cpp
  testcases/widgets/ut_diconbutton.cpp
  testcases/widgets/ut_dimageviewer.cpp
  testcases/widgets/ut_dinputdialog.cpp
  testcases/widgets/ut_dipv4lineedit.cpp
  testcases/widgets/ut_dkeysequenceedit.cpp
  testcases/widgets/ut_dlabel.cpp
  testcases/widgets/ut_dlineedit.cpp
  testcases/widgets/ut_dlistview.cpp
  testcases/widgets/ut_dloadingindicator.cpp
  testcases/widgets/ut_dmainwindow.cpp
  testcases/widgets/ut_dmessagemanager.cpp
  testcases/widgets/ut_dmpriscontrol.cpp
  testcases/widgets/ut_dpageindicator.cpp
  testcases/widgets/ut_dpasswordedit.cpp
  testcases/widgets/ut_dpicturesequenceview.cpp
  # TODO PREAK
  #testcases/widgets/ut_dprintpickcolorwidget.cpp
  #testcases/widgets/ut_dprintpreviewdialog.cpp
  testcases/widgets/ut_dprintpreviewwidget.cpp
  testcases/widgets/ut_dprogressbar.cpp
  testcases/widgets/ut_dpushbutton.cpp
  #TODO BREAK kf.windowsystem: Could not find any platform plugin
  # testcases/widgets/ut_dsearchcombobox.cpp
  testcases/widgets/ut_dsearchedit.cpp
  testcases/widgets/ut_dsettingsdialog.cpp
  testcases/widgets/ut_dsettingswidgetfactory.cpp
  testcases/widgets/ut_dshaowline.cpp
  testcases/widgets/ut_dsimplelistview.cpp
  testcases/widgets/ut_dslider.cpp
  # TODO break
  #testcases/widgets/ut_dspinbox.cpp
  # testcases/widgets/ut_dspinner.cpp
  testcases/widgets/ut_dstackwidget.cpp
  testcases/widgets/ut_dstyleditemdelegate.cpp
  testcases/widgets/ut_dstyleoption.cpp
  testcases/widgets/ut_dsuggestbutton.cpp
  testcases/widgets/ut_dswitchbutton.cpp
  testcases/widgets/ut_dtabbar.cpp
  testcases/widgets/ut_dtextedit.cpp
  testcases/widgets/ut_dtickeffect.cpp
  testcases/widgets/ut_dtiplabel.cpp
  testcases/widgets/ut_dtitlebar.cpp
  testcases/widgets/ut_dtoolbutton.cpp
  testcases/widgets/ut_dtooltip.cpp
  testcases/widgets/ut_dwarningbutton.cpp
  # FIXME break
  # testcases/widgets/ut_dwaterprogress.cpp
  testcases/widgets/ut_dwindowclosebutton.cpp
  testcases/widgets/ut_dwindowmaxbutton.cpp
  testcases/widgets/ut_dwindowminbutton.cpp
  testcases/widgets/ut_dwindowoptionbutton.cpp
  testcases/widgets/ut_dwindowquitfullbutton.cpp
)

include(../src/util/util.cmake)
include(../src/widgets/widgets.cmake)

add_executable(${BIN_NAME}
  main.cpp
  ${util_SRC}
  ${widgets_SRC}
  ${WIDGET_TEST}
)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${BIN_NAME} PRIVATE -fprofile-instr-generate -ftest-coverage)
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${BIN_NAME} PRIVATE -fprofile-arcs -ftest-coverage)
endif()
target_link_libraries(${BIN_NAME} PRIVATE 
  Qt5::Test 
  Qt5::Widgets
  Qt5::Core
  Qt5::DBus
  Qt5::PrintSupport
  Qt5::Concurrent
  Qt5::X11Extras
  Qt5::Network
  #${GLIB_LDFLAGS}
  ${QGSettings_LIBRARIES}
  ${STARUPNOTIFICATION_LIBRARIES}
  ${XEXT_LIBRARIES}
  ${XI_LIBRARIES}
  ${DtkGui_LIBRARIES}
  ${DtkCore_LIBRARIES}
  ${GTEST_LIBRARIES}
  ${X11_LIBRARIES}
  ${XCBUTIL_LIBRARIES}
  -lpthread
  -lm
  -lgcov
)
target_include_directories(${BIN_NAME} PUBLIC 
  ../include/dtkwidget/util/
  ../include/dtkwidget/global/
  ../include/dtkwidget/DWidget/
  ../include/dtkwidget/widgets/
  ../include/
  ../src/widgets/
  #../src/platforms/x11
  ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
  ${Qt5Widgets_PRIVATE_INCLUDE_DIRS}
  ${Qt5PrintSupport_PRIVATE_INCLUDE_DIRS}
  ${QGSettings_INCLUDE_DIRS}
  ${STARUPNOTIFICATION_INCLUDE_DIRS}
  ${XEXT_INCLUDE_DIRS}
  ${XI_INCLUDE_DIRS}
  ${X11_INCLUDE_DIRS}
  ${XCBUTIL_INCLUDE_DIRS}
  #${GLIB_INCLUDE_DIRS}
)
add_test(NAME ${BIN_NAME} COMMAND ${BIN_NAME})
