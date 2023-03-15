// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DSIDEBARHELPER_H
#define DSIDEBARHELPER_H

#include <DObject>
#include <dtkwidget_global.h>

#include <QColor>

DWIDGET_BEGIN_NAMESPACE

class DSidebarHelperPrivate;

class DSidebarHelper : public QObject, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(bool expanded READ expanded WRITE setExpanded NOTIFY expandChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)

public:
    DSidebarHelper(QObject *parent = nullptr);
    virtual ~DSidebarHelper() override;

    QColor backgroundColor() const;
    void setBackgroundColor(QColor backgroundColor);

    bool visible() const;
    void setVisible(bool visible);

    bool expanded() const;
    void setExpanded(bool expanded);

    int width() const;
    void setWidth(int width);

Q_SIGNALS:
    void backgroundColorChanged(QColor backgroundColor);
    void visibleChanged(bool visible);
    void expandChanged(bool expanded);
    void widthChanged(int width);

private:
    D_DECLARE_PRIVATE(DSidebarHelper)
};
DWIDGET_END_NAMESPACE

#endif // DSIDEBARHELPER_H
