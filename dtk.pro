TEMPLATE  = subdirs

SUBDIRS  += \
    dbase \
    dutil \
    dwidget \
    test \
    examples \

win32* {
!system(python $$PWD/tool/translate_generation.py $$PWD/dwidget): error("Failed to generate translation")
}


dutil.depends = dbase
dwidget.depends = dutil
test.depends = dwidget
examples.depends = test
