set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(LIB_NAME PrintPreviewSettingsPlugin)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


find_package(DtkGui REQUIRED)
find_package(DtkCore REQUIRED)
#find_package(DtkWidget REQUIRED)

find_package(Qt5 REQUIRED COMPONENTS Core)
find_package(Qt5 REQUIRED COMPONENTS Widgets)
#find_package(Qt5 REQUIRED COMPONENTS Svg)
find_package(Qt5 REQUIRED COMPONENTS PrintSupport)

add_library(${LIB_NAME} SHARED 
  settingsplugin.h 
  settingsplugin.cpp
)
target_link_libraries(${LIB_NAME} PRIVATE 
  Qt5::Widgets
  Qt5::Core
  #  Qt5::Svg
  Qt5::PrintSupport
  dtkwidget
  ${DtkGui_LIBRARIES}
  ${DtkCore_LIBRARIES}
)
target_include_directories(${LIB_NAME} PUBLIC
  ../../../include/dtkwidget/DWidget/
  ../../../include/dtkwidget/widgets/
  ../../../include/dtkwidget/util/
  ../../../include/dtkwidget/global/
)
