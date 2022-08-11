// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DFRAME_H
#define DFRAME_H

#include <dtkwidget_global.h>
#include <DPalette>
#include <DObject>

#include <QFrame>

DWIDGET_BEGIN_NAMESPACE

class DFramePrivate;
class DFrame : public QFrame, public DCORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DFrame)

public:
    explicit DFrame(QWidget *parent = nullptr);

    void setFrameRounded(bool on);
    void setBackgroundRole(DGUI_NAMESPACE::DPalette::ColorType type);
    using QFrame::setBackgroundRole;

protected:
    DFrame(DFramePrivate &dd, QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;
};

class DHorizontalLine : public QFrame
{
    Q_OBJECT
public:
    explicit DHorizontalLine(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags())
        : QFrame(parent, f)
    {
        setFrameShape(HLine);
    }
};

class DVerticalLine : public QFrame
{
    Q_OBJECT
public:
    explicit DVerticalLine(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags())
        : QFrame(parent, f)
    {
        setFrameShape(VLine);
    }
};

DWIDGET_END_NAMESPACE

#endif // DFRAME_H
