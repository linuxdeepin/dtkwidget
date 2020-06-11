/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DCLIPEFFECTWIDGET_H
#define DCLIPEFFECTWIDGET_H

#include "dtkwidget_global.h"
#include "dobject.h"

#include <QWidget>
#include <QPainterPath>

DWIDGET_BEGIN_NAMESPACE

class DClipEffectWidgetPrivate;
class DClipEffectWidget : public QWidget, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(QMargins margins READ margins WRITE setMargins NOTIFY marginsChanged)
    Q_PROPERTY(QPainterPath clipPath READ clipPath WRITE setClipPath NOTIFY clipPathChanged)

public:
    explicit DClipEffectWidget(QWidget *parent);

    QMargins margins() const;
    QPainterPath clipPath() const;

public Q_SLOTS:
    void setMargins(QMargins margins);
    void setClipPath(const QPainterPath &path);

Q_SIGNALS:
    void marginsChanged(QMargins margins);
    void clipPathChanged(QPainterPath clipPath);

protected:
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;

private:
    using QWidget::move;
    using QWidget::resize;
    using QWidget::setGeometry;

    D_DECLARE_PRIVATE(DClipEffectWidget)
};

DWIDGET_END_NAMESPACE

#endif // DCLIPEFFECTWIDGET_H
