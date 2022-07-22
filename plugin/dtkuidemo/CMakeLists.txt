cmake_minimum_required(VERSION 3.5)

project(dtkuidemo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(DtkCMake REQUIRED)

find_package(DtkGui REQUIRED)
find_package(DtkWidget REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
set(RCC_FILES dtkuidemo.qrc)
add_executable(
	${PROJECT_NAME}
	main.cpp 
	mainwindow.h
	mainwindow.cpp
	mainwindow.ui
	${RCC_FILES}
)
target_link_libraries(
	${PROJECT_NAME} PRIVATE 
	${DtkWidget_LIBRARIES}
	Qt${QT_VERSION_MAJOR}::Widgets
	Qt${QT_VERSION_MAJOR}::Core
	)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${DtkWidget_INCLUDE_DIRS}
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
)
