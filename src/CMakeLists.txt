if("${QT_VERSION_MAJOR}" STREQUAL "5")
    pkg_check_modules(QGSettings REQUIRED IMPORTED_TARGET gsettings-qt) #Dtk6 removed.
endif()
pkg_check_modules(XcbUtil REQUIRED IMPORTED_TARGET xcb-util)
pkg_check_modules(StartupNotification REQUIRED IMPORTED_TARGET libstartup-notification-1.0)
pkg_check_modules(Xext REQUIRED IMPORTED_TARGET xext)
pkg_check_modules(Xi REQUIRED IMPORTED_TARGET xi)
pkg_check_modules(X11 REQUIRED IMPORTED_TARGET x11)

include(util/util.cmake)
include(widgets/widgets.cmake)

file(GLOB TRANSLATIONS translations/*.ts)
if("${QT_VERSION_MAJOR}" STREQUAL "5")
    qt5_add_translation(QM_FILES ${TRANSLATIONS})
else()
    qt6_add_translation(QM_FILES ${TRANSLATIONS})
endif()

add_library(${LIBNAME} SHARED
    ${WIDGETS}
    ${QM_FILES}
    ${UTIL}
    ${PUBLIC_HEADERS}
)

set(TRANSLATIONS_INSTALL_DIR
    "dtk${PROJECT_VERSION_MAJOR}/DWidget/translations"
)

target_compile_definitions(${LIBNAME} PRIVATE
    SN_API_NOT_YET_FROZEN
    DTK_NO_MULTIMEDIA
    DWIDGET_TRANSLATIONS_DIR="${TRANSLATIONS_INSTALL_DIR}"
    LIBDTKWIDGET_LIBRARY
    D_IGNORE_DEPRECATIONS
)

if(DTK_STATIC_TRANSLATION)
target_compile_definitions(${LIBNAME} PRIVATE
    DTK_STATIC_TRANSLATION
)
endif()

target_include_directories(${LIBNAME}
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

target_link_libraries(${LIBNAME}
PUBLIC
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::DBus
    Qt${QT_VERSION_MAJOR}::PrintSupport
    Dtk${DTK_VERSION_MAJOR}::Gui
    Dtk${DTK_VERSION_MAJOR}::Core
PRIVATE
    Qt${QT_VERSION_MAJOR}::Concurrent
    Qt${QT_VERSION_MAJOR}::GuiPrivate
    Qt${QT_VERSION_MAJOR}::WidgetsPrivate
    Qt${QT_VERSION_MAJOR}::PrintSupportPrivate
    PkgConfig::StartupNotification
    PkgConfig::Xext
    PkgConfig::Xi
    PkgConfig::X11
    PkgConfig::XcbUtil
)

if("${QT_VERSION_MAJOR}" STREQUAL "5")
    target_link_libraries(${LIBNAME} PRIVATE
        PkgConfig::QGSettings
        Qt5::X11Extras
    )
endif()

target_link_directories(${LIBNAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${LIBRARY_INSTALL_DIR}>
)

set_target_properties(${LIBNAME}
    PROPERTIES
    VERSION ${CMAKE_PROJECT_VERSION}
    SOVERSION ${CMAKE_PROJECT_VERSION_MAJOR}
    EXPORT_NAME Widget
    PUBLIC_HEADER "${PUBLIC_HEADERS}"
)
install(
    TARGETS ${LIBNAME}
    EXPORT ${DtkWidget}Targets
    DESTINATION ${LIBRARY_INSTALL_DIR}
    PUBLIC_HEADER DESTINATION ${INCLUDE_INSTALL_DIR}
)

install(
    EXPORT ${DtkWidget}Targets 
    NAMESPACE Dtk${DTK_VERSION_MAJOR}:: 
    FILE ${DtkWidget}Targets.cmake 
    DESTINATION "${CONFIG_CMAKE_INSTALL_DIR}"
)

install(FILES ${QM_FILES} DESTINATION "${CMAKE_INSTALL_DATADIR}/${TRANSLATIONS_INSTALL_DIR}")
