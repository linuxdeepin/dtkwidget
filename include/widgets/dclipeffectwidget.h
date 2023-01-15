// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DCLIPEFFECTWIDGET_H
#define DCLIPEFFECTWIDGET_H

#include <dtkwidget_global.h>
#include <DObject>

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
