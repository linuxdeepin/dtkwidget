// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef RUBBERBANDEXAMPLE_H
#define RUBBERBANDEXAMPLE_H

#include <QWidget>
#include <QLabel>

#include <DRubberBand>

#include <dtkwidget_global.h>
#include "examplewindowinterface.h"
#include "pagewindowinterface.h"

class RubberBandExampleWindow : public PageWindowInterface
{
    Q_OBJECT

public:
    explicit RubberBandExampleWindow(QWidget *parent = nullptr);
};

class DRubberBandExample : public ExampleWindowInterface
{
    Q_OBJECT

public:
    explicit DRubberBandExample(QWidget *parent = nullptr);

    QString getTitleName() const override;
    QString getDescriptionInfo() const override;
    int getFixedHeight() const override;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Dtk::Widget::DRubberBand *m_pRubberBand;
    QPoint m_origin;
};

#endif // RUBBERBANDEXAMPLE_H
