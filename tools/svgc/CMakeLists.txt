set(TARGET_NAME dtk-svgc)
set(BIN_NAME ${TARGET_NAME}${DTK_VERSION_MAJOR})

find_package(Dtk${DTK_VERSION_MAJOR} REQUIRED COMPONENTS Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Svg)

add_executable(
    ${BIN_NAME}
    main.cpp
)

target_link_libraries(
    ${BIN_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Svg
    Dtk${DTK_VERSION_MAJOR}::Gui
)

install(TARGETS ${BIN_NAME} DESTINATION "${TOOL_INSTALL_DIR}")
