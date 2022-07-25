cmake_minimum_required(VERSION 3.5)
set(DVERSION "5.5.22")
set(BREAKVERSION "5")
include(GNUInstallDirs)
# set project name
project(dtkwidget 
  VERSION ${DVERSION}
  LANGUAGES CXX
)
#message(${CMAKE_INSTALL_LIBDIR})
option(NOTPACKAGE "to make package" ON)
add_subdirectory(src)

add_subdirectory(examples)
add_subdirectory(tools)
if(NOTPACKAGE)
  message("it is testing mode ,if you want to package ,set NOTPACKAGE OFF")
  add_subdirectory(plugin)
  add_subdirectory(tests)
endif()
install(FILES misc/cmake/DtkWidgetConfig.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/DtkWidget/)
