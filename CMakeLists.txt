cmake_minimum_required(VERSION 3.5)
set(DVERSION "5.5.22")
# set project name
project(dtkwidget 
  VERSION ${DVERSION}
  LANGUAGES CXX
)
add_subdirectory(src)
add_subdirectory(plugin)
add_subdirectory(examples)
add_subdirectory(tools)
add_subdirectory(tests)
install(FILES misc/cmake/DtkWidgetConfig.cmake DESTINATION /usr/lib/cmake/DtkWidget/)
