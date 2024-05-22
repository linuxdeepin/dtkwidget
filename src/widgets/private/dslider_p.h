// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DSLIDER_P_H
#define DSLIDER_P_H

#include <DSlider>

#include <DObjectPrivate>
#include <DIconButton>
#include <DFloatingWidget>
#include <DLabel>

#include <QLayout>

DWIDGET_BEGIN_NAMESPACE

class SliderStrip;
class DSliderPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    Q_DECLARE_PUBLIC(DSlider)

public:
    DSliderPrivate(DSlider *q);

    void init();

public:
    SpecialSlider *slider;
    bool handleVisible;
    Qt::Orientation orientation = Qt::Horizontal;
    QSlider::TickPosition aleDirection = QSlider::NoTicks;

    DIconButton *leftIcon;
    DIconButton *rightIcon;

    SliderStrip *left;
    SliderStrip *right;

    QGridLayout *layout;
    DFloatingWidget *tipvalue;
    DLabel *label;

    QSize iconSize;
    bool mouseWheelEnabled;

    void updtateTool(int value);

    bool needFocus;
};

DWIDGET_END_NAMESPACE

#endif // DSLIDER_P_H
