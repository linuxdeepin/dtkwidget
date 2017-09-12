/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef DLISTWIDGET_H
#define DLISTWIDGET_H

#include <QMap>
#include <QBoxLayout>

#include "dscrollarea.h"
#include "dtkwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class DListWidgetPrivate;
class DListWidget : public DScrollArea
{
    Q_OBJECT

    Q_PROPERTY(int firstChecked READ firstChecked NOTIFY firstCheckedChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged FINAL)
    Q_PROPERTY(int visibleCount READ visibleCount NOTIFY visibleCountChanged FINAL)
    Q_PROPERTY(int spacing READ spacing WRITE setSpacing)
    Q_PROPERTY(QBoxLayout::Direction direction READ direction WRITE setDirection)
    Q_PROPERTY(bool checkable READ checkable WRITE setCheckable)
    Q_PROPERTY(bool toggleable READ toggleable WRITE setToggleable NOTIFY toggleableChanged)
    Q_PROPERTY(bool enableHorizontalScroll READ enableHorizontalScroll WRITE setEnableHorizontalScroll NOTIFY enableHorizontalScrollChanged)
    Q_PROPERTY(bool enableVerticalScroll READ enableVerticalScroll WRITE setEnableVerticalScroll NOTIFY enableVerticalScrollChanged)
    Q_PROPERTY(CheckMode checkMode READ checkMode)

    Q_ENUMS(CheckMode)
public:
    enum CheckMode{
        Radio,
        MultipleCheck
    };

    explicit DListWidget(CheckMode checkMode = Radio, QWidget *parent = 0);
    explicit DListWidget(QBoxLayout::Direction direction, CheckMode checkMode = Radio, QWidget *parent = 0);

public Q_SLOTS:
    int addWidget(QWidget *w, Qt::Alignment a = Qt::AlignHCenter);
    void addWidgets(const QList<QWidget*> &ws, Qt::Alignment a = Qt::AlignHCenter);
    void insertWidget(int index, QWidget *w, Qt::Alignment a = Qt::AlignHCenter);
    void insertWidgets(int index, const QList<QWidget*> &ws, Qt::Alignment a = Qt::AlignHCenter);
    void setItemSize(int w, int h);
    void clear(bool isDelete = true);
    void removeWidget(int index, bool isDelete = true);
    void showWidget(int index);
    void hideWidget(int index);
    void setChecked(int index, bool checked);
    void setCheckMode(CheckMode checkMode);
    void setCheckable(bool checkable);
    void setToggleable(bool toggleable);
    void setEnableHorizontalScroll(bool enableHorizontalScroll);
    void setEnableVerticalScroll(bool enableVerticalScroll);
    void setSpacing(int spacing);
    void setDirection(QBoxLayout::Direction direction);

public:
    int count() const;
    int indexOf(const QWidget* widget);
    QWidget *getWidget(int index) const;
    int firstChecked() const;
    QList<int> checkedList() const;
    bool checkable() const;
    bool toggleable() const;
    bool isChecked(int index) const;
    int visibleCount() const;
    int spacing() const;
    QBoxLayout::Direction direction() const;
    CheckMode checkMode() const;
    QList<QWidget*> widgetList() const;
    QSize itemSize() const;
    bool enableHorizontalScroll() const;
    bool enableVerticalScroll() const;
    void setContentsMargins(int left, int top, int right, int bottom);
    void setContentsMargins(const QMargins &margins);
    void getContentsMargins(int *left, int *top, int *right, int *bottom) const;
    QMargins contentsMargins() const;

    QRect contentsRect() const;

    QSize sizeHint() const Q_DECL_OVERRIDE;

Q_SIGNALS:
    void firstCheckedChanged(int index);
    void checkedChanged(int index, bool checked);
    void countChanged(int count);
    void checkableChanged(bool checkable);
    void toggleableChanged(bool toggleable);
    void visibleCountChanged(int visibleCount);
    void clicked(int index);
    void enableHorizontalScrollChanged(bool enableHorizontalScroll);
    void enableVerticalScrollChanged(bool enableVerticalScroll);

private Q_SLOTS:
    void updateSize();

protected:
    DListWidget(DListWidgetPrivate &dd, QWidget *parent);
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void setVisibleCount(int count);

private:
    Q_DISABLE_COPY(DListWidget)
    D_DECLARE_PRIVATE(DListWidget)
};

DWIDGET_END_NAMESPACE

#endif // DLISTWIDGET_H
