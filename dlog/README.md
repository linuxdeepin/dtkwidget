# Deepin Tool Kit Log

DLog is the log module of deepin tool kit for Qt/C++

# Install

````
mkdir build && cd build
qmake ..
sudo make install
````

# Usage

Just add pkgconfig in .pro file

````
unix {
    CONFIG+=link_pkgconfig
    PKGCONFIG+=dtklog
}
````

# Example

````
#include <QCoreApplication>

#include <DLog>

using namespace Dtk::Log;

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    /* 1 you can use standrd deepin application log format */
    // 1.1 log to console
    DLogManager::instance()->registerAppender(true);
    dInfoTime();
    /* 1.2 log to standrd deepin user cache path: ~/.cache/{organ}/{appname}/ */
    // app.setOrganizationName("dtk-test"); // must set
    // app.setApplicationName("dlog-example"); // must set
    // dInfo()<< "LogFile:" << DLogManager::instance()->getlogFilePath();
    // DLogManager::instance()->registerAppender(false);

    /* 2 Register your own logger format;*/
    //  ConsoleAppender* consoleAppender = new ConsoleAppender;
    //  consoleAppender->setFormat("[%{type:-7}] <%{Function}> %{message}\n");
    //  logger->registerAppender(consoleAppender);

    dInfo("Starting the application");
    int result = 1;
    dWarning() << "Something went wrong." << "Result code is" << result;
    return result;
}
````

You can get full example from:

[dlog-example](https://github.com/deepin-tool-kit/dtk-example/tree/master/dlog-example)

# Document

````
doxygen -g dlog
doxygen -w html
firefox html/index
````
