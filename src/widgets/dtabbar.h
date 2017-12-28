/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
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
#ifndef DTABBAR_H
#define DTABBAR_H

#include <QTabBar>

#include <dtkwidget_global.h>
#include <dobject.h>

DCORE_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

class DTabBarPrivate;
class DTabBar : public QTabBar, public DObject
{
    Q_OBJECT

    Q_PROPERTY(bool visibleAddButton READ visibleAddButton WRITE setVisibleAddButton)

public:
    explicit DTabBar(QWidget *parent = 0);

    void setTabMinimumSize(int index, const QSize &size);
    void setTabMaximumSize(int index, const QSize &size);

    QSize sizeHint() const override;
    bool visibleAddButton() const;

Q_SIGNALS:
    void tabAddRequested();
    void tabReleaseRequested(int index);

public Q_SLOTS:
    void setVisibleAddButton(bool visibleAddButton);

protected:
    void paintEvent(QPaintEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dragMoveEvent(QDragMoveEvent *e) override;
    void dropEvent(QDropEvent *e) override;

    QSize tabSizeHint(int index) const override;
    QSize minimumTabSizeHint(int index) const override;
    virtual QSize maximumTabSizeHint(int index) const;

    void tabInserted(int index) override;
    void tabRemoved(int index) override;
    void tabLayoutChange() override;

private:
    D_DECLARE_PRIVATE(DTabBar)
    Q_PRIVATE_SLOT(d_func(), void _q_startDrag(int))
};

DWIDGET_END_NAMESPACE

#endif // DTABBAR_H
