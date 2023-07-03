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
if(DTK_VERSION_MAJOR EQUAL 6)
  list(REMOVE_ITEM UTIL_SOURCES
       ${CMAKE_CURRENT_LIST_DIR}/dapplicationsettings.cpp )
  list(REMOVE_ITEM UTIL_HEADERS
       ${CMAKE_CURRENT_LIST_DIR}/dapplicationsettings.h )
  list(REMOVE_ITEM UTIL_SOURCES
       ${CMAKE_CURRENT_LIST_DIR}/dregionmonitor.cpp)  
  list(REMOVE_ITEM UTIL_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/dregionmonitor.h)
  list(REMOVE_ITEM UTIL_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/dthumbnailprovider.cpp)
  list(REMOVE_ITEM UTIL_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/dthumbnailprovider.h)
  list(REMOVE_ITEM UTIL_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/dtrashmanager_linux.cpp)
  list(REMOVE_ITEM UTIL_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/dtrashmanager_linux.h)
  list(REMOVE_ITEM UTIL_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/dtrashmanager_win.cpp)
  list(REMOVE_ITEM UTIL_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/dtrashmanager_win.h)
endif()
set(UTIL ${UTIL_SOURCES} ${UTIL_HEADERS})
