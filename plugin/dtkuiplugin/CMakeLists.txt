#cmake_minimum_required(VERSION 3.5)

#project(dtkuiplugin VERSION 0.1 LANGUAGES CXX)
set(UIPLUGIN dtkuiplugin)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(DtkCMake REQUIRED)

find_package(DtkGui REQUIRED)
#find_package(DtkWidget REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS UiPlugin)
set(RCC_FILES dtkuiplugin.qrc)
add_library(
  ${UIPLUGIN} SHARED
  dcustomerplugins.h
  dcustomerplugins.cpp
  dcustomermacrowidget.h
  ${RCC_FILES}
)
target_link_libraries(
  ${UIPLUGIN} PRIVATE 
  ${DtkGui_LIBRARIES}
  #${DtkWidget_LIBRARIES}
  dtkwidget
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::UiPlugin
)
target_include_directories(${UIPLUGIN} PUBLIC
  ${DtkWidget_INCLUDE_DIRS}
  ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
  ../../../include/dtkwidget/DWidget/
  ../../../include/dtkwidget/widgets/
  ../../../include/dtkwidget/util/
  ../../../include/dtkwidget/global/
)
