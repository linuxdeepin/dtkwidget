// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DWATERPROGRESS_H
#define DWATERPROGRESS_H

#include <dtkwidget_global.h>
#include <DObject>

#include <QScopedPointer>
#include <QWidget>

DWIDGET_BEGIN_NAMESPACE

class DWaterProgressPrivate;
class LIBDTKWIDGETSHARED_EXPORT DWaterProgress : public QWidget, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
public:
    explicit DWaterProgress(QWidget *parent = 0);
    ~DWaterProgress();

    int value() const;

Q_SIGNALS:
    void valueChanged();

public Q_SLOTS:
    void start();
    void stop();
    void setValue(int value);
    void setTextVisible(bool visible);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void changeEvent(QEvent *e) override;

private:
    D_DECLARE_PRIVATE(DWaterProgress)
};

DWIDGET_END_NAMESPACE

#endif // DWATERPROGRESS_H
