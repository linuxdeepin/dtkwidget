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

#ifndef PAGEWINDOWINTERFACE_H
#define PAGEWINDOWINTERFACE_H

#include <QWidget>
#include <QMouseEvent>

class ExampleWindowInterface;

class PageWindowInterface : public QWidget
{
public:
    explicit PageWindowInterface(QWidget *parent);
    virtual ~PageWindowInterface() override;

public:
    virtual void initPageWindow();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;

    virtual QWidget *doLayout(ExampleWindowInterface *pExample);
    void addExampleWindow(ExampleWindowInterface *pExample);

private:
    QList<ExampleWindowInterface *> m_exampleList;
};

#endif // PAGEWINDOWINTERFACE_H
