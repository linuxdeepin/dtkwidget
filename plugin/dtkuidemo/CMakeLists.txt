#cmake_minimum_required(VERSION 3.5)

#project(dtkuidemo VERSION 0.1 LANGUAGES CXX)
set(DEMO dtkuidemo)

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
set(RCC_FILES dtkuidemo.qrc)
add_executable(
  ${DEMO}
  main.cpp 
  mainwindow.h
  mainwindow.cpp
  mainwindow.ui
  ${RCC_FILES}
)
target_link_libraries(
  ${DEMO} PRIVATE 
  dtkwidget
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Core
  ${DtkGui_LIBRARIES}
)
target_include_directories(${DEMO} PUBLIC
  #${DtkWidget_INCLUDE_DIRS}
  ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
  ../../../include/dtkwidget/DWidget/
  ../../../include/dtkwidget/widgets/
  ../../../include/dtkwidget/util/
  ../../../include/dtkwidget/global/
)
