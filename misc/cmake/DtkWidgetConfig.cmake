find_package(Dtk REQUIRED Core)
find_package(Dtk REQUIRED Gui)
set(DTKWIDGET_INCLUDE_DIR /usr/include/libdtk/DWidget)
set(DTKWIDGET_TOOL_DIR /usr/lib/libdtk/DWidget/bin)
set(DtkWidget_LIBRARIES dtkwidget ${DtkCore_LIBRARIES}  ${DtkGui_LIBRARIES})
include_directories("${DTKWIDGET_INCLUDE_DIR}")