// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DBOXWIDGET_H
#define DBOXWIDGET_H

#include <dtkwidget_global.h>

#include <dobject.h>

#include <QFrame>
#include <QBoxLayout>

DWIDGET_BEGIN_NAMESPACE

class DBoxWidgetPrivate;
class DBoxWidget : public QFrame, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(QBoxLayout::Direction direction READ direction WRITE setDirection NOTIFY directionChanged)

public:
    explicit DBoxWidget(QBoxLayout::Direction direction, QWidget *parent = 0);

    QBoxLayout::Direction direction() const;
    QBoxLayout *layout() const;

    void addWidget(QWidget *widget);
    QSize sizeHint() const Q_DECL_OVERRIDE;

public Q_SLOTS:
    void setDirection(QBoxLayout::Direction direction);

Q_SIGNALS:
    void sizeChanged(QSize size);
    void directionChanged(QBoxLayout::Direction direction);

protected:
    virtual void updateSize(const QSize &size);
    bool event(QEvent *ee) Q_DECL_OVERRIDE;

private:
    D_DECLARE_PRIVATE(DBoxWidget)
};

class DHBoxWidget : public DBoxWidget
{
    Q_OBJECT
public:
    explicit DHBoxWidget(QWidget *parent = 0);
};

class DVBoxWidget : public DBoxWidget
{
    Q_OBJECT
public:
    explicit DVBoxWidget(QWidget *parent = 0);
};

DWIDGET_END_NAMESPACE

#endif // DBOXWIDGET_H
