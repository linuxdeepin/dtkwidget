// SPDX-FileCopyrightText: 2017 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DMAINWINDOW_P_H
#define DMAINWINDOW_P_H

#include <dtkwidget_global.h>
#include <DMainWindow>
#include <DShadowLine>

#include <DFrame>
#include <DObjectPrivate>

class QShortcut;

DWIDGET_BEGIN_NAMESPACE

class DSidebarHelper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(bool expanded READ expanded WRITE setExpanded NOTIFY expandChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)

public:
    DSidebarHelper(QObject *parent = nullptr) : QObject (parent){ }
    virtual~DSidebarHelper(){}

    QColor backgroundColor() const
    {
        return m_backgroundColor;
    }
    void setBackgroundColor(QColor backgroundColor)
    {
        if (m_backgroundColor == backgroundColor)
            return;

        m_backgroundColor = backgroundColor;
        Q_EMIT backgroundColorChanged(m_backgroundColor);
    }

    bool visible() const
    {
        return m_visible;
    }

    void setVisible(bool visible)
    {
        if (m_visible == visible)
            return;

        m_visible = visible;
        Q_EMIT visibleChanged(m_visible);
    }

    bool expanded() const
    {
        return m_expanded;
    }

    void setExpanded(bool expanded)
    {
        if (m_expanded == expanded)
            return;

        m_expanded = expanded;
        Q_EMIT expandChanged(m_expanded);
    }

    inline bool sectionVisible() const
    {
        return m_visible ? m_expanded : false;
    }

    int width() const
    {
        return m_width;
    }

    void setWidth(int width)
    {
        if (m_width == width)
            return;

        m_width = width;
        Q_EMIT widthChanged(m_width);
    }

Q_SIGNALS:
    void backgroundColorChanged(QColor backgroundColor);
    void visibleChanged(bool visible);
    void expandChanged(bool expanded);
    void widthChanged(int width);

private:
    bool m_visible = false;
    bool m_expanded = true;
    int m_width = -1;
    QColor m_backgroundColor;

};

class DPlatformWindowHandle;
class DTitlebar;
class DMainWindowPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DMainWindowPrivate(DMainWindow *qq);

    void init();
    void updateTitleShadowGeometry();

    DPlatformWindowHandle   *handle     = Q_NULLPTR;
    DTitlebar               *titlebar   = Q_NULLPTR;
    DShadowLine             *titleShadow = nullptr;
    QShortcut               *help       = Q_NULLPTR;
    DSidebarHelper          *sidebarHelper = nullptr;
    QWidget                 *sidebarWidget = nullptr;
    QToolBar                *tb = nullptr;
    DVerticalLine           *sidebarSep = nullptr;

private:
    D_DECLARE_PUBLIC(DMainWindow)
    void _q_autoShowFeatureDialog();
};

DWIDGET_END_NAMESPACE

#endif // DMAINWINDOW_P_H
