set(BIN_NAME collections)
set(CMAKE_AUTOUIC ON)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Widgets)

add_executable(${BIN_NAME}
    images.qrc
    resources.qrc
    icons/theme-icons.qrc
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
    imageviewerexample.h
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
    imageviewerexample.cpp
)

target_link_libraries(${BIN_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::GuiPrivate
    ${LIBNAME}
)
install(
    TARGETS ${BIN_NAME}
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/dtk${PROJECT_VERSION_MAJOR}/DWidget/examples/"
)
