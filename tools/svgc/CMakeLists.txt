set(BIN_NAME dtk-svgc)

find_package(Dtk REQUIRED COMPONENTS Gui)
find_package(Qt5 REQUIRED COMPONENTS Svg)

add_executable(
    ${BIN_NAME}
    main.cpp
)

target_link_libraries(
    ${BIN_NAME} PRIVATE
    Qt5::Svg
    Dtk::Gui
)

install(TARGETS ${BIN_NAME} DESTINATION "${TOOL_INSTALL_DIR}")
