cmake_minimum_required(VERSION 3.23)
project(collections)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



find_package(DtkGui REQUIRED)
find_package(DtkCore REQUIRED)
#find_package(DtkWidget REQUIRED)

find_package(Qt5 REQUIRED COMPONENTS Core)
find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt5 REQUIRED COMPONENTS Svg)
find_package(Qt5 REQUIRED COMPONENTS PrintSupport)

set(IMAGES images.qrc)
set(RESCOUCES resources.qrc)
set(THEME icons/theme-icons.qrc)
add_executable(${PROJECT_NAME}
  ${IMAGES}
  ${RESCOUCES}
  ${THEME}
  cameraform.ui

  mainwindow.h 
  buttonexample.h 
  examplewindowinterface.h 
  pagewindowinterface.h 
  editexample.h 
  sliderexample.h 
  listviewexample.h 
  windowexample.h 
  tooltipexample.h 
  spinnerexample.h 
  dialogexample.h 
  progressbarexample.h 
  layoutexample.h 
  scrollbarexample.h 
  rubberbandexample.h 
  widgetexample.h 
  lcdnumberexample.h 
  menuexample.h

  main.cpp
  mainwindow.cpp 
  buttonexample.cpp 
  examplewindowinterface.cpp 
  pagewindowinterface.cpp 
  editexample.cpp 
  sliderexample.cpp 
  listviewexample.cpp 
  windowexample.cpp 
  tooltipexample.cpp 
  spinnerexample.cpp 
  dialogexample.cpp 
  progressbarexample.cpp 
  layoutexample.cpp 
  scrollbarexample.cpp 
  rubberbandexample.cpp 
  widgetexample.cpp 
  lcdnumberexample.cpp 
  menuexample.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE 
  Qt5::Widgets
  Qt5::Core
	Qt5::Svg
	Qt5::PrintSupport
	dtkwidget
	${DtkGui_LIBRARIES}
	${DtkCore_LIBRARIES}
)
target_include_directories(${PROJECT_NAME} PUBLIC
  ../../../src/widgets/
  ../../../src/
  ../../../src/util/
  ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
)
