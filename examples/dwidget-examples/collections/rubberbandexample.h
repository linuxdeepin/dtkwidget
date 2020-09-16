/*
 * Copyright (C) 2020 ~ 2020 Deepin Technology Co., Ltd.
 *
 * Author:     sunkang <sunkang@uniontech.com>
 *
 * Maintainer: sunkang <sunkang@uniontech.com>
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

#ifndef RUBBERBANDEXAMPLE_H
#define RUBBERBANDEXAMPLE_H

#include <QWidget>
#include <QLabel>

#include <DRubberBand>

#include "dtkwidget_global.h"
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
