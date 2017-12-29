
find_package(Dtk REQUIRED Core)

set(DTKWIDGET_INCLUDE_DIR "/usr/include/libdtk-2.0/DWidget")
set(DtkWidget_LIBRARIES dtkwidget ${DtkCore_LIBRARIES})
include_directories("${DTKWIDGET_INCLUDE_DIR}")
