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

#ifndef DFLOWLAYOUT_H
#define DFLOWLAYOUT_H

#include "dtkwidget_global.h"

#include <dobject.h>

#include <QLayout>
#include <QWidget>
#include <QListView>

DWIDGET_BEGIN_NAMESPACE

class DFlowLayoutPrivate;
class DFlowLayout : public QLayout, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(int horizontalSpacing READ horizontalSpacing WRITE setHorizontalSpacing NOTIFY horizontalSpacingChanged)
    Q_PROPERTY(int verticalSpacing READ verticalSpacing WRITE setVerticalSpacing NOTIFY verticalSpacingChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(Flow flow READ flow WRITE setFlow NOTIFY flowChanged)
    Q_PROPERTY(QSize sizeHint READ sizeHint NOTIFY sizeHintChanged)

public:
    typedef QListView::Flow Flow;
    Q_ENUMS(Flow)

    explicit DFlowLayout(QWidget *parent);
    DFlowLayout();
    ~DFlowLayout();

    void insertItem(int index, QLayoutItem *item);
    void insertWidget(int index, QWidget *widget);
    void insertLayout(int index, QLayout *layout);
    void insertSpacing(int index, int size);
    void insertStretch(int index, int stretch = 0);
    void insertSpacerItem(int index, QSpacerItem *spacerItem);

    void addSpacing(int size);
    void addStretch(int stretch = 0);
    void addSpacerItem(QSpacerItem *spacerItem);
    void addItem(QLayoutItem *item) Q_DECL_OVERRIDE;
    bool hasHeightForWidth() const Q_DECL_OVERRIDE;
    int heightForWidth(int) const Q_DECL_OVERRIDE;
    int count() const Q_DECL_OVERRIDE;
    QLayoutItem *itemAt(int index) const Q_DECL_OVERRIDE;
    QSize minimumSize() const Q_DECL_OVERRIDE;
    void setGeometry(const QRect &rect) Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    QLayoutItem *takeAt(int index) Q_DECL_OVERRIDE;
    Qt::Orientations expandingDirections() const Q_DECL_OVERRIDE;

    int horizontalSpacing() const;
    int verticalSpacing() const;
    Flow flow() const;

public Q_SLOTS:
    void setHorizontalSpacing(int horizontalSpacing);
    void setVerticalSpacing(int verticalSpacing);
    void setSpacing(int spacing);
    void setFlow(Flow flow);

Q_SIGNALS:
    void horizontalSpacingChanged(int horizontalSpacing);
    void verticalSpacingChanged(int verticalSpacing);
    void countChanged(int count);
    void flowChanged(Flow flow);
    void sizeHintChanged(QSize sizeHint) const;

private:
    D_DECLARE_PRIVATE(DFlowLayout)
};

DWIDGET_END_NAMESPACE

#endif // DFLOWLAYOUT_H
