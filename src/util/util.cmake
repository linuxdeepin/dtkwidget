file(GLOB UTILHEAEDER ${CMAKE_CURRENT_LIST_DIR}/../../include/dtkwidget/util/*.h)
set(UTILESOURCE
  ${CMAKE_CURRENT_LIST_DIR}/dfileiconprovider.cpp 
  ${CMAKE_CURRENT_LIST_DIR}/dthumbnailprovider.cpp 
  ${CMAKE_CURRENT_LIST_DIR}/dwidgetutil.cpp 
  ${CMAKE_CURRENT_LIST_DIR}/dhidpihelper.cpp 
  ${CMAKE_CURRENT_LIST_DIR}/dapplicationsettings.cpp 
  ${CMAKE_CURRENT_LIST_DIR}/daccessibilitychecker.cpp	
  ${CMAKE_CURRENT_LIST_DIR}/ddesktopservices_linux.cpp 
  ${CMAKE_CURRENT_LIST_DIR}/dtrashmanager_linux.cpp 
  ${CMAKE_CURRENT_LIST_DIR}/dregionmonitor.cpp
)
set(util_SRC
  ${CMAKE_CURRENT_LIST_DIR}/private/dregionmonitor_p.h 
  ${UTILESOURCE}
  ${UTILHEAEDER}
)
