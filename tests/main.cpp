// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QApplication>
#include <QTimer>
#include <gtest/gtest.h>


#ifdef QT_DEBUG
#include <sanitizer/asan_interface.h>
#endif

/**
  添加Qt事件循环,兼容gtest.
 */
int runTest(QCoreApplication &app)
{
    int ret = 0;
    QTimer::singleShot(0, &app, [&app, &ret](){
        ret = RUN_ALL_TESTS();
        app.quit();
    });
    app.exec();
    return ret;
}

int main(int argc, char *argv[])
{
    // gerrit编译时没有显示器，需要指定环境变量
    qputenv("QT_QPA_PLATFORM", "offscreen");

    QApplication app(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);

#ifdef QT_DEBUG
    __sanitizer_set_report_path("asan.log");
#endif

    return runTest(app);
}
