TARGET      = dcustomwidgets
QT         += widgets dtkwidget uiplugin

load(qt_build_config)
#MODULE_VERSION = 5.11.3
PLUGIN_CLASS_NAME = DCustomWidgetsPlugin
PLUGIN_TYPE = designer
CONFIG += tool_plugin
load(qt_plugin)

SOURCES += \
    dcustomerplugins.cpp

HEADERS += \
    dcustomerplugins.h \
    dcustomermacrowidget.h

RESOURCES += dtkuiplugin.qrc

