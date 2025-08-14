set(BINNAME "ut-${LIBNAME}")

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)
find_package(GTest REQUIRED)

if("${QT_VERSION_MAJOR}" STREQUAL "5")
    pkg_check_modules(QGSettings REQUIRED IMPORTED_TARGET gsettings-qt) #Dtk6 removed.
endif()

pkg_check_modules(XcbUtil REQUIRED IMPORTED_TARGET xcb-util)
pkg_check_modules(StartupNotification REQUIRED IMPORTED_TARGET libstartup-notification-1.0)
pkg_check_modules(Xext REQUIRED IMPORTED_TARGET xext)
pkg_check_modules(Xi REQUIRED IMPORTED_TARGET xi)
pkg_check_modules(X11 REQUIRED IMPORTED_TARGET x11)

set(WIDGET_TEST 
    testcases/widgets/ut_daboutdialog.cpp
    testcases/widgets/ut_dabstractdialog.cpp
    testcases/widgets/ut_dalertcontrol.cpp
    testcases/widgets/ut_danchor.cpp
    # TODO break the test
    #testcases/widgets/ut_danchors.cpp
    testcases/widgets/ut_darrowbutton.cpp
    testcases/widgets/ut_darrowlinedrawer.cpp
    # TODO: Qt6 no longer support 
    #testcases/widgets/ut_darrowlineexpand.cpp
    testcases/widgets/ut_darrowrectangle.cpp
    testcases/widgets/ut_dbackgroundgroup.cpp
    # TODO: Qt6 no longer support
    #testcases/widgets/ut_dbaseexpand.cpp
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
    testcases/widgets/ut_dlicensedialog.cpp
    testcases/widgets/ut_dlineedit.cpp
    testcases/widgets/ut_dlistview.cpp
    testcases/widgets/ut_dloadingindicator.cpp
    testcases/widgets/ut_dmainwindow.cpp
    testcases/widgets/ut_dmessagemanager.cpp
    testcases/widgets/ut_dmpriscontrol.cpp
    testcases/widgets/ut_dpageindicator.cpp
    testcases/widgets/ut_dpalettehelper.cpp
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
    testcases/widgets/ut_dswitchlineexpand.cpp
    testcases/widgets/ut_dtabbar.cpp
    testcases/widgets/ut_dtextedit.cpp
    testcases/widgets/ut_dtickeffect.cpp
    testcases/widgets/ut_dtiplabel.cpp
    testcases/widgets/ut_dtitlebar.cpp
    testcases/widgets/ut_dtoolbutton.cpp
    testcases/widgets/ut_dtooltip.cpp
    testcases/widgets/ut_dwarningbutton.cpp
    testcases/widgets/ut_dwatermarkhelper.cpp
    # FIXME break
    # testcases/widgets/ut_dwaterprogress.cpp
    testcases/widgets/ut_dwindowclosebutton.cpp
    testcases/widgets/ut_dwindowmaxbutton.cpp
    testcases/widgets/ut_dwindowminbutton.cpp
    testcases/widgets/ut_dwindowoptionbutton.cpp
    testcases/widgets/ut_dwindowquitfullbutton.cpp
    testcases/widgets/ut_dcombobox.cpp
    testcases/widgets/ut_dwatermarkwidget.cpp
)

include(${PROJECT_SOURCE_DIR}/src/util/util.cmake)
include(${PROJECT_SOURCE_DIR}/src/widgets/widgets.cmake)

set(RESCOUCES data.qrc)

add_executable(${BINNAME}
    main.cpp
    ${RESCOUCES}
    ${UTIL}
    ${WIDGETS}
    ${WIDGET_TEST}
    ${PUBLIC_HEADERS}
)

target_compile_definitions(${BINNAME} PRIVATE
    SN_API_NOT_YET_FROZEN
    DTK_NO_MULTIMEDIA
    DWIDGET_TRANSLATIONS_DIR="dtk${PROJECT_VERSION_MAJOR}/DWidget/translations"
    D_IGNORE_DEPRECATIONS
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${BINNAME} PRIVATE -fprofile-instr-generate -ftest-coverage)
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${BINNAME} PRIVATE -fprofile-arcs -ftest-coverage)
endif()

target_compile_options(${BINNAME} PRIVATE -fno-access-control -fsanitize=address)
target_link_options(${BINNAME} PRIVATE -fsanitize=address)

target_include_directories(${BINNAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/src/widgets
    ${PROJECT_SOURCE_DIR}/include/DWidget
    ${PROJECT_SOURCE_DIR}/include/util
    ${PROJECT_SOURCE_DIR}/include/widgets
    ${PROJECT_SOURCE_DIR}/include/global
    ${PROJECT_SOURCE_DIR}/include
    ${CONFIG_INCLUDE}
)

target_link_libraries(${BINNAME} PRIVATE 
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::WidgetsPrivate
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::GuiPrivate
    Qt${QT_VERSION_MAJOR}::DBus
    Qt${QT_VERSION_MAJOR}::PrintSupport
    Qt${QT_VERSION_MAJOR}::PrintSupportPrivate
    Qt${QT_VERSION_MAJOR}::Concurrent
    Qt${QT_VERSION_MAJOR}::Network
    PkgConfig::StartupNotification
    PkgConfig::Xext
    PkgConfig::Xi
    PkgConfig::X11
    PkgConfig::XcbUtil
    Dtk${DTK_VERSION_MAJOR}::Gui
    Dtk${DTK_VERSION_MAJOR}::Core
    GTest::GTest
    gmock
    pthread
    m
    gcov
)

if("${QT_VERSION_MAJOR}" STREQUAL "5")
    target_link_libraries(${BINNAME} PRIVATE
        PkgConfig::QGSettings
        Qt5::X11Extras
    )
endif()

add_test(NAME ${BINNAME} COMMAND ${BINNAME})
