#cmake_minimum_required(VERSION 3.5)

set(BIN_NAME test)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "-fno-access-control")
add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

#find_package(PkgConfig REQUIRED)
#pkg_search_module(GLIB REQUIRED glib-2.0)

find_package(Qt5 REQUIRED COMPONENTS PrintSupport Concurrent Widgets Test Core DBus REQUIRED)
find_package(GTest REQUIRED)
find_package(DtkCMake REQUIRED)
find_package(DtkGui REQUIRED)
find_package(DtkCore REQUIRED)
file(GLOB WIDGET_TEST testcases/widgets/*.cpp)

add_executable(${BIN_NAME}
  main.cpp
  ${WIDGET_TEST}
)

target_link_libraries(${BIN_NAME} PRIVATE 
  Qt5::Test 
  Qt5::Widgets
  Qt5::Core
  Qt5::DBus
  Qt5::PrintSupport
  Qt5::Concurrent
  #${GLIB_LDFLAGS}
  ${DtkGui_LIBRARIES}
  ${DtkCore_LIBRARIES}
  ${GTEST_LIBRARIES}
  dtkwidget
	-lpthread
)
target_include_directories(${BIN_NAME} PUBLIC 
  ../include/dtkwidget/util/
  ../include/dtkwidget/global/
  ../include/dtkwidget/DWidget/
  ../include/dtkwidget/widgets/
  ../src/widgets/
  #${GLIB_INCLUDE_DIRS}
)
