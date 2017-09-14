/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DPAGEINDICATOR_H
#define DPAGEINDICATOR_H

#include "dtkwidget_global.h"
#include "dobject.h"

#include <QWidget>

DWIDGET_BEGIN_NAMESPACE

class DPageIndicatorPrivate;
class LIBDTKWIDGETSHARED_EXPORT DPageIndicator : public QWidget, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DPageIndicator)
    D_DECLARE_PRIVATE(DPageIndicator)
    Q_PROPERTY(QColor pointColor READ pointColor WRITE setPointColor DESIGNABLE true)
    Q_PROPERTY(QColor secondaryPointColor READ secondaryPointColor WRITE setSecondaryPointColor DESIGNABLE true)
    Q_PROPERTY(int pointRadius READ pointRadius WRITE setPointRadius)
    Q_PROPERTY(int secondaryPointRadius READ secondaryPointRadius WRITE setSecondaryPointRadius)

public:
    explicit DPageIndicator(QWidget *parent = 0);

    int pageCount() const;
    void setPageCount(const int count);

    void nextPage();
    void previousPage();
    void setCurrentPage(const int index);
    int currentPageIndex() const;

    QColor pointColor() const;
    void setPointColor(QColor color);

    QColor secondaryPointColor() const;
    void setSecondaryPointColor(QColor color);

    int pointRadius() const;
    void setPointRadius(int size);

    int secondaryPointRadius() const;
    void setSecondaryPointRadius(int size);

    int pointDistance() const;
    void setPointDistance(int distance);

protected:
    void paintEvent(QPaintEvent *e) override;
};

DWIDGET_END_NAMESPACE

#endif // DPAGEINDICATOR_H
