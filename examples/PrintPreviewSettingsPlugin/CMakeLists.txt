set(PLUGIN_NAME PrintPreviewSettingsPlugin)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_library(${PLUGIN_NAME} SHARED 
  settingsplugin.h 
  settingsplugin.cpp
)

target_link_libraries(${PLUGIN_NAME} PRIVATE 
  ${LIBNAME}
)
