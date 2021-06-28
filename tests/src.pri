QT += x11extras dbus widgets-private

!contains(QT, printsupport): QT += printsupport printsupport-private
!contains(QT, concurrent): QT += concurrent

# 防止源文件编译失败指定相应宏
isEmpty(DTK_NO_MULTIMEDIA){
    DEFINES += DTK_NO_MULTIMEDIA
}

DMODULE_NAME=dwidget
load(dtk_translation)

include($$PWD/../src/util/util.pri)
include($$PWD/../src/widgets/widgets.pri)
