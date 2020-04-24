/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     Chris Xiong <chirs241097@gmail.com>
 *
 * Maintainer: Chris Xiong <chirs241097@gmail.com>
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

#include "dtkwidget_global.h"
#include "dobject.h"

#include <QProgressBar>

DWIDGET_BEGIN_NAMESPACE

class DColoredProgressBarPrivate;
class LIBDTKWIDGETSHARED_EXPORT DColoredProgressBar : public QProgressBar, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
public:
    explicit DColoredProgressBar(QWidget *parent = nullptr);
    void addThreshold(int threshold, QBrush brush);
    void removeThreshold(int threshold);
    QList<int> thresholds() const;

protected:
    void paintEvent(QPaintEvent *) override;

private:
    D_DECLARE_PRIVATE(DColoredProgressBar)
};

DWIDGET_END_NAMESPACE
