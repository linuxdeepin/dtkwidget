/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef DBASELINE_H
#define DBASELINE_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

#include "dtkwidget_global.h"
#include "dconstants.h"

DWIDGET_BEGIN_NAMESPACE

class LIBDTKWIDGETSHARED_EXPORT DBaseLine : public QLabel
{
    Q_OBJECT
public:
    explicit DBaseLine(QWidget *parent = 0);

    void setLeftContent(QWidget *content);
    void setRightContent(QWidget *content);

    QBoxLayout *leftLayout();
    QBoxLayout *rightLayout();

    void setLeftMargin(int margin);
    void setRightMargin(int margin);
    int leftMargin() const;
    int rightMargin() const;

private:
    QHBoxLayout *m_mainLayout = NULL;
    QHBoxLayout *m_leftLayout= NULL;
    QHBoxLayout *m_rightLayout = NULL;

    int m_leftMargin = 10;
    int m_rightMargin = HEADER_RIGHT_MARGIN;
};

DWIDGET_END_NAMESPACE

#endif // DBASELINE_H
