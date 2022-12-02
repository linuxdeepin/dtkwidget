# TODO Maybe need to check if is apple or else 
file(GLOB PRIVATEMPRISHEADER ${CMAKE_CURRENT_LIST_DIR}/private/mpris/*.h)
file(GLOB PRIVATEMPRISSOURCE ${CMAKE_CURRENT_LIST_DIR}/private/mpris/*.cpp)
file(GLOB WIDGETSOURCE ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
file(GLOB WIDGETHEADER ${CMAKE_CURRENT_LIST_DIR}/../../include/dtkwidget/widgets/*)
file(GLOB WIDGETPROVATEHEADER ${CMAKE_CURRENT_LIST_DIR}/private/*.h)
file(GLOB WIDGETPROVATESOURCE ${CMAKE_CURRENT_LIST_DIR}/private/*.cpp)
file(GLOB SETTINGS ${CMAKE_CURRENT_LIST_DIR}/private/settings/*)
file(GLOB NOTIFICATION ${CMAKE_CURRENT_LIST_DIR}/private/startupnotifications/*)
set(KEYBOARD 
  ${CMAKE_CURRENT_LIST_DIR}/private/keyboardmonitor/dkeyboardmonitor.h
  ${CMAKE_CURRENT_LIST_DIR}/private/keyboardmonitor/dkeyboardmonitor.cpp
)
set(ICONS ${CMAKE_CURRENT_LIST_DIR}/icons.qrc)
set(ICONTHEME ${CMAKE_CURRENT_LIST_DIR}/assets/icons/dtk-icon-theme.qrc)
set(widgets_SRC
  ${PRIVATEMPRISHEADER}
  ${PRIVATEMPRISSOURCE}

  ${SETTINGS}
 
  ${NOTIFICATION}

  ${KEYBOARD}

  ${WIDGETPROVATEHEADER}
  ${WIDGETPROVATESOURCE}

  ${ICONS}

  ${ICONTHEME}

  ${WIDGETHEADER}
  ${WIDGETSOURCE}
)
