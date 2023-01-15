file(GLOB_RECURSE UTIL_HEADERS "${CMAKE_CURRENT_LIST_DIR}/*.h")
file(GLOB_RECURSE UTIL_SOURCES "${CMAKE_CURRENT_LIST_DIR}/*.cpp")
if(LINUX)
  list(REMOVE_ITEM UTIL_SOURCES
       ${CMAKE_CURRENT_LIST_DIR}/ddesktopservices_win.cpp
       ${CMAKE_CURRENT_LIST_DIR}/dtrashmanager_win.cpp)
else()
  list(REMOVE_ITEM UTIL_SOURCES
       ${CMAKE_CURRENT_LIST_DIR}/ddesktopservices_linux.cpp
       ${CMAKE_CURRENT_LIST_DIR}/dtrashmanager_linux.cpp)
endif()
set(UTIL ${UTIL_SOURCES} ${UTIL_HEADERS})
