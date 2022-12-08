@page daboutdialog DAboutDialog

# DAboutDialog

## 概述

DAboutDialog 类提供了应用程序的关于对话框

详细信息请参考 [Dtk::Widget::DAboutDialog](classDtk_1_1Widget_1_1DAboutDialog.html)

项目目录结构在同一目录下

## CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.1.0) # 指定cmake最低版本

project(example1 VERSION 1.0.0 LANGUAGES CXX) # 指定项目名称, 版本, 语言 cxx就是c++

set(CMAKE_CXX_STANDARD 11) # 指定c++标准
set(CMAKE_CXX_STANDARD_REQUIRED ON) # 指定c++标准要求,至少为11以上
set(target example1) # 指定目标名称

set(CMAKE_AUTOMOC ON) # support qt moc # 支持qt moc
set(CMAKE_AUTORCC ON) # support qt resource file # 支持qt资源文件
set(CMAKE_AUTOUIC ON) # support qt ui file # 支持qt ui文件(非必须)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # support clangd 支持 clangd

if (CMAKE_VERSION VERSION_LESS "3.7.0") # 如果cmake版本小于3.7.0
    set(CMAKE_INCLUDE_CURRENT_DIR ON) # 设置包含当前目录
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED) # 寻找Qt5组件Widgets
find_package(Qt5 COMPONENTS Gui  REQUIRED) # 寻找Qt5组件Gui
find_package(Dtk COMPONENTS Widget REQUIRED) # 寻找Dtk组件Widget
find_package(Dtk COMPONENTS Core REQUIRED) # 寻找Dtk组件Core
find_package(Dtk COMPONENTS Gui) # 寻找Dtk组件Gui

add_executable(example1     # 添加可执行文件
        main.cpp
)

target_link_libraries(example1 PRIVATE
    Qt5::Widgets
    Qt5::Gui
    dtkgui
    dtkcore
    dtkwidget
) # 链接库

```

## main.cpp

```cpp
#include <DAboutDialog>
#include <DApplication>
DWIDGET_USE_NAMESPACE // 使用Dtk widget命名空间
int main(int argc, char *argv[]) {
    DApplication app(argc, argv);
    app.setProductName("Dtk example"); // 设置产品名称
    app.setOrganizationName("deepin");
    DAboutDialog about;        // 创建DAboutDialog对象
    about.setWindowTitle("这是一个最简单的例子"); // 设置窗口标题
    about.show();              // 显示DAboutDialog
    return app.exec();          // 运行程序
}
```

这是一个最简单的例子，运行结果如下

![example](/docs/images/daboutdialog_example.png)

但是这个例子并不是我们实际上使用的样子，在我们实际上的使用过程中，我们需要添加一些信息，比如我们的logo，我们的版本号，我们的版权信息等等，这些信息都应该在app中设置，如下

### CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.1.0)

project(example VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(target example)

set(CMAKE_AUTOMOC ON) # support qt moc
set(CMAKE_AUTORCC ON) # support qt resource file
set(CMAKE_AUTOUIC ON) # support qt ui file

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # suppot clangd

if (CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Gui  REQUIRED)
find_package(Dtk COMPONENTS Widget REQUIRED)
find_package(Dtk COMPONENTS Core REQUIRED)
find_package(Dtk COMPONENTS Gui)

add_executable(example
        widget.hpp
        widget.cpp
        main.cpp
)

target_link_libraries(example PRIVATE
    Qt5::Widgets
    Qt5::Gui
    dtkgui
    dtkcore
    dtkwidget
)
```

### main.cpp

```cpp
#include <DAboutDialog>
#include <DApplication>
#include "widget.hpp"

DWIDGET_USE_NAMESPACE
int main(int argc, char *argv[])
{
    DApplication a(argc, argv);
    example example1;
    example1.show();
    a.setApplicationName("dtk example");
    DAboutDialog dialog;
    return a.exec();
}
```

### widget.hpp

```cpp
#ifndef EXAMPLE_H
#define EXAMPLE_H
#include <DPushButton>
#include <DLineEdit>
#include <DLabel>
#include <QVBoxLayout>
#include <DMainWindow>
DWIDGET_USE_NAMESPACE
class example : public DMainWindow
{
    Q_OBJECT
public:
 example();
 ~example();
private:
    QVBoxLayout *mainlayout;
    DLabel *label;
    DLineEdit *text;
    DPushButton *button;

private slots:
void  printlog();
};
#endif

```

### widget.cpp

```cpp
#include "widget.hpp"
#include <cstdio>
#include <QDebug>
example::example()
{
    mainlayout= new QVBoxLayout;
    button = new DPushButton;
    label = new DLabel("test");
    text = new DLineEdit();
    mainlayout->addWidget(label);
    mainlayout->addWidget(text);
    mainlayout->addWidget(button);
    setLayout(mainlayout);
    connect(button,SIGNAL(clicked()),this,SLOT(printlog()));
}
example::~example(){}

void example::printlog()
{
    printf("%s\n","clicked");
    qDebug() << "button clicked!"; // 如果没设置相应环境变量则此Debug无法输出
}
```

效果如下图

![example](/docs/images/daboutdialog_example2.png)
