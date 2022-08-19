// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DPAGEINDICATOR_H
#define DPAGEINDICATOR_H

#include <dtkwidget_global.h>
#include <DObject>

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
    Q_PROPERTY(int pageCount READ pageCount WRITE setPageCount)
    Q_PROPERTY(int currentPage READ currentPageIndex WRITE setCurrentPage)
    Q_PROPERTY(int pointDistance READ pointDistance WRITE setPointDistance)

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
