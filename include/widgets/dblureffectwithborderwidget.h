// SPDX-FileCopyrightText: 2017 - 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
#ifndef DBLUREFFECTWITHBORDERWIDGET_H
#define DBLUREFFECTWITHBORDERWIDGET_H

#include <dblureffectwidget.h>

DWIDGET_BEGIN_NAMESPACE

class DBlurEffectWithBorderWidgetPrivate;
class DBlurEffectWithBorderWidget : public DBlurEffectWidget
{
    Q_OBJECT
public:
    explicit DBlurEffectWithBorderWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    D_DECLARE_PRIVATE(DBlurEffectWithBorderWidget)
};

DWIDGET_END_NAMESPACE

#endif // DBLUREFFECTWITHBORDERWIDGET_H
