// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFLOATINGWIDGET_H
#define DFLOATINGWIDGET_H

#include <dtkwidget_global.h>
#include <DObject>

#include <QWidget>

DWIDGET_BEGIN_NAMESPACE

class DStyleOptionFloatingWidget;
class DBlurEffectWidget;
class DFloatingWidgetPrivate;
class DFloatingWidget : public QWidget, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DFloatingWidget)
    Q_PROPERTY(bool blurBackgroundEnabled READ blurBackgroundIsEnabled WRITE setBlurBackgroundEnabled)

public:
    explicit DFloatingWidget(QWidget *parent = nullptr);

    virtual QSize sizeHint() const override;
    void setWidget(QWidget *widget);
    void setFramRadius(int radius);

protected:
    DFloatingWidget(DFloatingWidgetPrivate &dd, QWidget *parent);

    void paintEvent(QPaintEvent* e) override;
    bool event(QEvent *event) override;

    using QWidget::setContentsMargins;
    using QWidget::setAutoFillBackground;

public:
    virtual void initStyleOption(DStyleOptionFloatingWidget *option) const;
    bool blurBackgroundIsEnabled() const;
    DBlurEffectWidget *blurBackground() const;

public Q_SLOTS:
    void setBlurBackgroundEnabled(bool blurBackgroundEnabled);
};

DWIDGET_END_NAMESPACE

#endif // DFLOATINGWIDGET_H
