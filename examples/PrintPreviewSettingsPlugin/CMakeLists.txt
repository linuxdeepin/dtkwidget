set(PLUGIN_NAME PrintPreviewSettingsPlugin)

find_package(Qt5 REQUIRED COMPONENTS Core)

add_library(${PLUGIN_NAME} SHARED 
  settingsplugin.h 
  settingsplugin.cpp
)

target_link_libraries(${PLUGIN_NAME} PRIVATE 
  ${LIB_NAME}
)
