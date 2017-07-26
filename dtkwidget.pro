TEMPLATE  = subdirs
CONFIG = ordered no_keywords

SUBDIRS  += \
    src \
    examples

linux* {
    !system($$PWD/tool/translate_generation.sh): error("Failed to generate translation")
}

win32* {
    !system(python $$PWD/tool/translate_generation.py $$PWD/dwidget): error("Failed to generate translation")
}

macx* {
    !system($$PWD/tool/translate_generation.sh): error("Failed to generate translation")
}


TRANSLATIONS += $$PWD/translations/$${TARGET}.ts \
                $$PWD/translations/$${TARGET}_zh_CN.ts

translations.path = $$PREFIX/share/$${TARGET}/translations
translations.files = $$PWD/translations/*.qm

INSTALLS += translations
