// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QTest>
#include <QDebug>

#include "darrowlinedrawer.h"
#include "ddrawer.h"
#include "dbaseline.h"
#include "private/ddrawer_p.h"

#include <QLabel>

DWIDGET_USE_NAMESPACE

class ut_DArrowLineDrawer : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    QWidget *widget = nullptr;
    DArrowLineDrawer *drawer = nullptr;
};

void ut_DArrowLineDrawer::SetUp()
{
    widget = new QWidget;
    widget->resize(300, 200);
    drawer = new DArrowLineDrawer(widget);
    drawer->resize(100, 30);
}

void ut_DArrowLineDrawer::TearDown()
{
    widget->deleteLater();
}

TEST_F(ut_DArrowLineDrawer, testDArrowLineDrawerSetTitle)
{
    const QString title("xxxxxxxxxxxx");
    drawer->setTitle(title);
    // TODO
}

TEST_F(ut_DArrowLineDrawer, testDArrowLineDrawerSetExpand)
{
    bool expand = false;
    drawer->setExpand(expand);
    expand = true;
    drawer->setExpand(expand);
    // TODO
}


#if DTK_VERSION < DTK_VERSION_CHECK(6, 0, 0, 0) // dtk 6.0 no longer support
TEST_F(ut_DArrowLineDrawer, testHeaderLineAndResizeEvent)
{
    DBaseLine *headerLine = drawer->headerLine();
    ASSERT_TRUE(headerLine);

    QMouseEvent mousePress(QEvent::MouseButtonPress, QPointF(0, 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    headerLine->mousePressEvent(&mousePress);

    QMouseEvent mouseMove(QEvent::MouseMove, QPointF(0, 0), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
    headerLine->mouseMoveEvent(&mouseMove);

    QEvent changeEvent(QEvent::FontChange);
    headerLine->changeEvent(&changeEvent);

    const int LoopCount = 20;
    int originWidth = drawer->width();

    for (int count = 0; count <= LoopCount; ++count) {
        drawer->resize(originWidth + count, drawer->height());
    }

    ASSERT_TRUE(QTest::qWaitFor([&] { return (drawer->width() == (originWidth + LoopCount)); }));
}
#endif
