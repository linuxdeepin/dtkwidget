#cmake_minimum_required(VERSION 3.5)
set(BIN_NAME dtk-svgc)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# find_package 
find_package(DtkCMake REQUIRED)
find_package(DtkGui REQUIRED)
find_package(DtkCore REQUIRED)

find_package(Qt5 REQUIRED COMPONENTS Core)
find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt5 REQUIRED COMPONENTS Svg)

add_executable(
  ${BIN_NAME}
  svgc/main.cpp
)
target_link_libraries(
  ${BIN_NAME} PRIVATE
  Qt5::Widgets 
  Qt5::Core
  Qt5::Svg
  ${DtkGui_LIBRARIES}
  ${DtkCore_LIBRARIES}
)
install(TARGETS ${BIN_NAME} DESTINATION lib/libdtk/DWidget/bin)
