TEMPLATE  = subdirs

CONFIG += ordered no_keywords

SUBDIRS  += \
    src \
    examples

linux* {
    !system($$PWD/tool/translate_generation.sh): error("Failed to generate translation")
}
win32* {
    !system(python $$PWD/tool/translate_generation.py $$PWD): error("Failed to generate translation")
}
macx* {
    !system($$PWD/tool/translate_generation.sh): error("Failed to generate translation")
}
