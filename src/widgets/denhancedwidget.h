// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DENHANCEDWIDGET_H
#define DENHANCEDWIDGET_H

#include <QWidget>

#include <dtkwidget_global.h>

DWIDGET_BEGIN_NAMESPACE

class DEnhancedWidgetPrivate;
class DEnhancedWidget: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QWidget *target READ target WRITE setTarget NOTIFY targetChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)

public:
    explicit DEnhancedWidget(QWidget *target, QObject *parent = 0);
    ~DEnhancedWidget();

    QWidget *target() const;
    bool enabled() const;

public Q_SLOTS:
    void setTarget(QWidget *target);
    void setEnabled(bool enabled);

Q_SIGNALS:
    void xChanged(int x);
    void yChanged(int y);
    void positionChanged(const QPoint &point);
    void widthChanged(int width);
    void heightChanged(int height);
    void sizeChanged(const QSize &size);
    void targetChanged(QWidget *target);
    void enabledChanged(bool enabled);
    void showed();

protected:
    bool eventFilter(QObject *o, QEvent *e) Q_DECL_OVERRIDE;

private:
    explicit DEnhancedWidget(DEnhancedWidgetPrivate *dd, QWidget *w, QObject *parent = 0);

    DEnhancedWidgetPrivate *d_ptr;

    Q_DECLARE_PRIVATE(DEnhancedWidget)
};

DWIDGET_END_NAMESPACE

#endif // DENHANCEDWIDGET_H
