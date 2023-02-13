pkg_check_modules(QGSettings REQUIRED IMPORTED_TARGET gsettings-qt)
pkg_check_modules(XcbUtil REQUIRED IMPORTED_TARGET xcb-util)
pkg_check_modules(StartupNotification REQUIRED IMPORTED_TARGET libstartup-notification-1.0)
pkg_check_modules(Xext REQUIRED IMPORTED_TARGET xext)
pkg_check_modules(Xi REQUIRED IMPORTED_TARGET xi)
pkg_check_modules(X11 REQUIRED IMPORTED_TARGET x11)

include(util/util.cmake)
include(widgets/widgets.cmake)

file(GLOB TRANSLATIONS translations/*.ts)
qt5_add_translation(QM_FILES ${TRANSLATIONS})

add_library(${LIB_NAME} SHARED
    ${WIDGETS}
    ${QM_FILES}
    ${UTIL}
    ${PUBLIC_HEADERS}
)

set(TRANSLATIONS_INSTALL_DIR
    "dtk${PROJECT_VERSION_MAJOR}/DWidget/translations"
)

target_compile_definitions(${LIB_NAME} PRIVATE
    SN_API_NOT_YET_FROZEN
    DTK_NO_MULTIMEDIA
    DWIDGET_TRANSLATIONS_DIR="${TRANSLATIONS_INSTALL_DIR}"
    LIBDTKWIDGET_LIBRARY
)

if(DTK_STATIC_TRANSLATION)
target_compile_definitions(${LIB_NAME} PRIVATE
    DTK_STATIC_TRANSLATION
)
endif()

target_include_directories(${LIB_NAME}
PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/DWidget>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/util>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/widgets>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/global>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CONFIG_INCLUDE}>
INTERFACE
    $<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>
)

target_link_libraries(${LIB_NAME}
PUBLIC
    Qt5::Widgets
    Qt5::Network
    Qt5::Core
    Qt5::DBus
    Qt5::PrintSupport
    Dtk::Gui
    Dtk::Core
PRIVATE
    Qt5::Concurrent
    Qt5::X11Extras
    Qt5::GuiPrivate
    Qt5::WidgetsPrivate
    Qt5::PrintSupportPrivate
    PkgConfig::QGSettings
    PkgConfig::StartupNotification
    PkgConfig::Xext
    PkgConfig::Xi
    PkgConfig::X11
    PkgConfig::XcbUtil
)

target_link_directories(${LIB_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${LIBRARY_INSTALL_DIR}>
)

set_target_properties(${LIB_NAME}
    PROPERTIES
    VERSION ${CMAKE_PROJECT_VERSION}
    SOVERSION ${CMAKE_PROJECT_VERSION_MAJOR}
    EXPORT_NAME Widget
    PUBLIC_HEADER "${PUBLIC_HEADERS}"
)
install(
    TARGETS ${LIB_NAME}
    EXPORT DtkWidgetTargets
    DESTINATION ${LIBRARY_INSTALL_DIR}
    PUBLIC_HEADER DESTINATION ${INCLUDE_INSTALL_DIR}
)

install(
    EXPORT DtkWidgetTargets
    FILE DtkWidgetTargets.cmake
    NAMESPACE Dtk::
    DESTINATION ${CONFIG_INSTALL_DIR}
)

install(FILES ${QM_FILES} DESTINATION "${CMAKE_INSTALL_DATADIR}/${TRANSLATIONS_INSTALL_DIR}")

dconfig_meta_files(COMMONID org.deepin.dtkwidget FILES ${CMAKE_CURRENT_LIST_DIR}/org.deepin.dtkwidget.feature-display.json)
