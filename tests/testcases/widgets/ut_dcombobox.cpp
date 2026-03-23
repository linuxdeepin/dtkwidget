// SPDX-FileCopyrightText: 2023 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QTest>
#include "private/qcombobox_p.h"
#include "dcombobox.h"
DWIDGET_USE_NAMESPACE
class ut_DComboBox : public testing::Test
{
protected:
    void SetUp() override
    {
        target = new DComboBox();
    }
    void TearDown() override
    {
        if (target)
            delete target;
    }
    DComboBox *target = nullptr;
};

TEST_F(ut_DComboBox, maxVisibleItems)
{
    // Add an additional item to display the arrow.
    for (int i = 0; i < target->maxVisibleItems() + 1; i++) {
        target->addItem(QString::number(i));
    }
    target->show();
    (void)QTest::qWaitForWindowExposed(target->windowHandle());

    const QPoint arrowPos(target->rect().right() - 1, target->rect().center().y());
    QTest::mouseClick(target, Qt::LeftButton, Qt::KeyboardModifiers(), arrowPos);

    auto container = target->findChild<QComboBoxPrivateContainer *>();
    ASSERT_TRUE(container);
    const auto oldRect = container->rect();
    target->hide();

    target->addItem(QString("more"));
    target->show();

    QTest::mouseClick(target, Qt::LeftButton, Qt::KeyboardModifiers(), arrowPos);
    ASSERT_EQ(container->rect().height(), oldRect.height());
}

TEST_F(ut_DComboBox, popupLeaveClearsHighlightButPreservesKeyboardBase)
{
    for (int i = 0; i < 5; ++i)
        target->addItem(QString::number(i));

    target->setCurrentIndex(2);
    target->show();
    (void)QTest::qWaitForWindowExposed(target->windowHandle());

    const QPoint arrowPos(target->rect().right() - 1, target->rect().center().y());
    QTest::mouseClick(target, Qt::LeftButton, Qt::KeyboardModifiers(), arrowPos);

    auto container = target->findChild<QComboBoxPrivateContainer *>();
    ASSERT_TRUE(container);

    auto popupView = target->view();
    ASSERT_TRUE(popupView);

    popupView->setCurrentIndex(target->model()->index(4, target->modelColumn(), target->rootModelIndex()));
    ASSERT_TRUE(popupView->currentIndex().isValid());

    QCursor::setPos(target->mapToGlobal(QPoint(-20, -20)));
    QEvent leaveEvent(QEvent::Leave);
    QCoreApplication::sendEvent(container, &leaveEvent);
    ASSERT_FALSE(popupView->currentIndex().isValid());

    QKeyEvent keyEvent(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier);
    QCoreApplication::sendEvent(popupView, &keyEvent);
    ASSERT_TRUE(popupView->currentIndex().isValid());
    ASSERT_EQ(popupView->currentIndex().row(), 3);
}
