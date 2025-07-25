// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>
#include <QTest>
#include <QDebug>

#include "dslider.h"
#include "private/dslider_p.h"

DWIDGET_USE_NAMESPACE

class ut_DSlider : public testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    QWidget *widget = nullptr;
    DSlider *dslider = nullptr;
};

void ut_DSlider::SetUp()
{
    widget = new QWidget;
    dslider = new DSlider(Qt::Horizontal, widget);
    widget->resize(300, 100);
}

void ut_DSlider::TearDown()
{
    if (widget) {
        delete widget;
        widget = nullptr;
    }
}

TEST_F(ut_DSlider, testDSliderValue)
{
    // 测试 pagestep 的设置
    dslider->resize(widget->width(), dslider->height());
    dslider->setPageStep(10);
    ASSERT_TRUE(dslider->pageStep() == 10);

    // 测试最大最小值的设置
    dslider->setValue(50);
    ASSERT_TRUE(dslider->value() == 50);

    dslider->setMaximum(90);
    dslider->setMinimum(10);
    ASSERT_TRUE(dslider->maximum() == 90);
    ASSERT_TRUE(dslider->minimum() == 10);

    dslider->setValue(95);
    ASSERT_TRUE(dslider->value() == 90);

    dslider->setValue(5);
    ASSERT_TRUE(dslider->value() == 10);
}

TEST_F(ut_DSlider, testDSliderIcon)
{
    // 测试 Icon 的设置
    const QIcon icon(QIcon::fromTheme("preferences-system"));
    const QSize iconSize(30, 30);
    dslider->setLeftIcon(icon);
    dslider->setRightIcon(icon);
    dslider->setIconSize(iconSize);
    ASSERT_TRUE(dslider->d_func()->leftIcon->iconSize() == iconSize);
    ASSERT_TRUE(dslider->d_func()->rightIcon->iconSize() == iconSize);
}

TEST_F(ut_DSlider, testDSliderSignal)
{
    dslider->setMouseWheelEnabled(true);
    ASSERT_TRUE(dslider->d_func()->mouseWheelEnabled == true);

    int v = 0;
    QObject::connect(dslider, &DSlider::valueChanged, [&v](int value){
        ASSERT_EQ(value, v);
    });

    for (int i = dslider->minimum(); i <= dslider->maximum(); ++i) {
        v = i;
        dslider->setValue(i);
    }
}

TEST_F(ut_DSlider, testDSliderTips)
{
    const QString tips("tips");
    dslider->setTipValue(tips);
    ASSERT_EQ(dslider->d_func()->label->text(), tips);
}

TEST_F(ut_DSlider, testDSliderss)
{
    QStringList leftTicks, rightTicks, aboveTicks, belowTicks;
    leftTicks  << "left tick 1"  <<  "left tick 2";
    rightTicks << "right tick 1" <<  "right tick 2";
    aboveTicks << "above tick 1" <<  "above tick 2";
    belowTicks << "below tick 1" <<  "below tick 2";

    dslider->setLeftTicks(std::as_const(leftTicks));
    dslider->setRightTicks(std::as_const(rightTicks));
    dslider->setAboveTicks(std::as_const(aboveTicks));
    dslider->setBelowTicks(std::as_const(belowTicks));

    // TODO 是否考虑预留接口以获取 leftTicks、rightTicks、aboveTicks、belowTicks
}

TEST_F(ut_DSlider, testDSliderHandle)
{
    dslider->setHandleVisible(true);
    ASSERT_TRUE(dslider->handleVisible());

    dslider->setHandleVisible(false);
    ASSERT_FALSE(dslider->handleVisible());
}

TEST_F(ut_DSlider, testDSliderPositions)
{
    QList <int> list;
    list << 0 << 25 << 50 << 75 << 100;
    dslider->setMarkPositions(list);
    // TODO 是否考虑预留接口以获取 markPositions

    bool res;
    dslider->setEnabledAcrossStyle(false);
    res = dslider->d_func()->slider->property("_d_dtk_sldier_across").toBool();
    ASSERT_FALSE(res);
    dslider->setEnabledAcrossStyle(true);
    res = dslider->d_func()->slider->property("_d_dtk_sldier_across").toBool();
    ASSERT_TRUE(res);
}
