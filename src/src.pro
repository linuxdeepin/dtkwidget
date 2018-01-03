include($$PWD/lib.pri)

TEMPLATE = lib
CONFIG += qt
TARGET = dtkwidget

DEFINES += LIBDTKWIDGET_LIBRARY

QT += multimedia multimediawidgets concurrent
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets widgets-private
  # Qt >= 5.8
  greaterThan(QT_MAJOR_VERSION, 5)|greaterThan(QT_MINOR_VERSION, 7): QT += gui-private
  else: QT += platformsupport-private
}
linux* {
    QT += x11extras dbus
    CONFIG += link_pkgconfig
    PKGCONFIG += x11 xext dtkcore
}

mac* {
    QT += svg
    DEFINES += DTK_TITLE_DRAG_WINDOW
    CONFIG += link_pkgconfig
    PKGCONFIG += dtkcore
}

win* {
    QT += svg
    DEFINES += DTK_TITLE_DRAG_WINDOW

    #DEPENDS dtkcore
    INCLUDEPATH += $$INCLUDE_INSTALL_DIR\libdtk-$$VERSION\DCore
    LIBS += -L$$LIB_INSTALL_DIR -ldtkcore
}

HEADERS += dtkwidget_global.h

includes.path = $${DTK_INCLUDEPATH}/DWidget
includes.files += $$PWD/dtkwidget_global.h $$PWD/DtkWidgets

include($$PWD/util/util.pri)
include($$PWD/widgets/widgets.pri)

linux* {
    includes.files += $$PWD/platforms/linux/*.h
}
win32* {
    includes.files += $$PWD/platforms/windows/*.h
}

QMAKE_PKGCONFIG_NAME = DTK_WIDGET
QMAKE_PKGCONFIG_DESCRIPTION = Deepin Tool Kit Widget Module
QMAKE_PKGCONFIG_INCDIR = $$includes.path
QMAKE_PKGCONFIG_REQUIRES += dtkcore

# CMake configure
INC_DIR = $$replace(includes.path, "/", "\/")
CMD = sed -i -E \'s/DTKWIDGET_INCLUDE_DIR \".*\"\\)$/DTKWIDGET_INCLUDE_DIR \"$${INC_DIR}\"\\)/\' ../cmake/DtkWidget/DtkWidgetConfig.cmake
system($$CMD)

cmake_config.path = $$LIB_INSTALL_DIR
cmake_config.files = $$PWD/../cmake

# add translations
TRANSLATIONS += $$PWD/../translations/$${TARGET}2.ts \
                $$PWD/../translations/$${TARGET}2_zh_CN.ts

translations.path = $$PREFIX/share/$${TARGET}/translations
translations.files = $$PWD/../translations/*.qm

INSTALLS += translations cmake_config

# create DtkWidgets file
defineTest(containIncludeFiles) {
    header = $$absolute_path($$ARGS)
    header_dir = $$quote($$dirname(header))

    for (file, includes.files) {
        file_ap = $$absolute_path($$file)
        file_dir = $$quote($$dirname(file_ap))

        isEqual(file_dir, $$header_dir):return(true)
    }

    return(false)
}

defineTest(updateDtkWidgetsFile) {
    dtkwidgets_include_files = $$HEADERS
    dtkwidgets_file_content = $$quote($${LITERAL_HASH}ifndef DTK_WIDGETS_MODULE_H)
    dtkwidgets_file_content += $$quote($${LITERAL_HASH}defined DTK_WIDGETS_MODULE_H)

    for(header, dtkwidgets_include_files) {
        containIncludeFiles($$header) {
            dtkwidgets_file_content += $$quote($${LITERAL_HASH}include \"$$basename(header)\")
        }
    }

    dtkwidgets_file_content += $$quote($${LITERAL_HASH}endif)
    !write_file($$PWD/DtkWidgets, dtkwidgets_file_content, exe):return(false)

    return(true)
}

!updateDtkWidgetsFile():warning(Cannot create "DtkWidgets" header file)
