set(DEMO dtkuidemo)

set(CMAKE_AUTOUIC ON)

# We must find Qt5Widgets explicitly to include Qt5::uic target for AUTOUIC, although Qt5::Widgets is implicitly linked to demo through dtkwidget
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

add_executable(
    ${DEMO}
    main.cpp
    mainwindow.h
    mainwindow.cpp
    dtkuidemo.qrc
    mainwindow.ui
)

target_link_libraries(${DEMO} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::GuiPrivate
    ${LIBNAME}
)
