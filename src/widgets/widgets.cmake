# TODO Maybe need to check if is apple or else 
file(GLOB MPRIS_HEADERS ${CMAKE_CURRENT_LIST_DIR}/private/mpris/*.h)
file(GLOB MPRIS_SOURCES ${CMAKE_CURRENT_LIST_DIR}/private/mpris/*.cpp)
file(GLOB WIDGETS_SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.cpp ${CMAKE_CURRENT_LIST_DIR}/private/*.cpp)
file(GLOB WIDGETS_PRIVATE_HEADERS ${CMAKE_CURRENT_LIST_DIR}/private/*.h)
file(GLOB SETTINGS ${CMAKE_CURRENT_LIST_DIR}/private/settings/*)
file(GLOB NOTIFICATIONS ${CMAKE_CURRENT_LIST_DIR}/private/startupnotifications/*)
file(GLOB KEYBOARD ${CMAKE_CURRENT_LIST_DIR}/private/keyboardmonitor/*)
file(GLOB_RECURSE RESOURCES ${CMAKE_CURRENT_LIST_DIR}/*.qrc)

if (DTK_VERSION_MAJOR EQUAL 6)
  list(REMOVE_ITEM UTIL_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/dapplicationhelper.cpp)
  list(REMOVE_ITEM UTIL_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/dapplicationhelper.h)
  list(REMOVE_ITEM UTIL_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/darrowlineexpand.cpp)
  list(REMOVE_ITEM UTIL_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/darrowlineexpand.h)
  list(REMOVE_ITEM UTIL_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/dbaseexpand.cpp)
  list(REMOVE_ITEM UTIL_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/dbaseexpand.h)
  list(REMOVE_ITEM UTIL_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/dexpandgroup.cpp)
  list(REMOVE_ITEM UTIL_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/dexpandgroup.h)
  list(REMOVE_ITEM UTIL_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/dimagebutton.cpp)
  list(REMOVE_ITEM UTIL_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/dimagebutton.h)
  list(REMOVE_ITEM UTIL_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/dsegmentedcontrol.cpp)
  list(REMOVE_ITEM UTIL_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/dsegmentedcontrol.h)
  list(REMOVE_ITEM UTIL_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/dshortcutedit.cpp)
  list(REMOVE_ITEM UTIL_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/dshortcutedit.h)
  list(REMOVE_ITEM UTIL_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/dthememanager.cpp)
  list(REMOVE_ITEM UTIL_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/dthememanager.h)
  list(REMOVE_ITEM UTIL_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/dtoast.cpp)
  list(REMOVE_ITEM UTIL_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/dtoast.h)

endif()

set(WIDGETS
  ${MPRIS_HEADERS}
  ${MPRIS_SOURCES}
  ${WIDGETS_SOURCES}
  ${WIDGETS_PRIVATE_HEADERS}
  ${SETTINGS}
  ${NOTIFICATIONS}
  ${KEYBOARD}
  ${RESOURCES}
)
