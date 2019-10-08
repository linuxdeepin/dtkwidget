/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zhangwengeng <zhangwengeng_cm@deepin.com>
 *
 * Maintainer: zhangwengeng <zhangwengeng_cm@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

    QSize sizeHint() const override;
    void setWidget(QWidget *widget);

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
