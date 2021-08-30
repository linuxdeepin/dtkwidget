include($$PWD/widgets/widgets.pri)
include($$PWD/util/util.pri)
# 暂时屏蔽打印预览的单元测试，too slow and crashed sometimes
!contains(DEFINES, DTK_NO_PRINTPREVIEWTEST): include($$PWD/printpreview/printpreview.pri)
