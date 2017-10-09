TEMPLATE  = subdirs

CONFIG += ordered no_keywords

SUBDIRS  += \
    src \
    tools \
    examples

win* {
    !system(python $$PWD/tools/translate_generation.py $$PWD): error("Failed to generate translation")
} else {
    !system($$PWD/tools/translate_generation.sh): error("Failed to generate translation")
}
