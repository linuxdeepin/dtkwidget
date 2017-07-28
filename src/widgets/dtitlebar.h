/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DTITLEBAR_H
#define DTITLEBAR_H

#include <QWidget>
#include <QMenu>

#include <dobject.h>
#include "dtkwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class DTitlebarPrivate;

///
/// \brief The DTitlebar class is an universal title bar on the top of windows.
///
class LIBDTKWIDGETSHARED_EXPORT DTitlebar : public QWidget , public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
public:
    explicit DTitlebar(QWidget *parent = 0);

#ifndef QT_NO_MENU
    QMenu *menu() const;
    void setMenu(QMenu *menu);
#endif

    QWidget *customWidget() const;
    void setCustomWidget(QWidget *, bool fixCenterPos = false);
    void setCustomWidget(QWidget *, Qt::AlignmentFlag flag = Qt::AlignCenter, bool fixCenterPos = false);
    void setWindowFlags(Qt::WindowFlags type);
    int buttonAreaWidth() const;
    bool separatorVisible() const;

    void setVisible(bool visible) Q_DECL_OVERRIDE;

    void resize(int width, int height);
    void resize(const QSize &);

Q_SIGNALS:
    void optionClicked();
    void doubleClicked();
    void mousePressed(Qt::MouseButtons buttons);
    void mouseMoving(Qt::MouseButton botton);

#ifdef Q_OS_WIN
    void mousePosPressed(Qt::MouseButtons buttons, QPoint pos);
    void mousePosMoving(Qt::MouseButton botton, QPoint pos);
#endif

public Q_SLOTS:
    void setFixedHeight(int h);
    void setSeparatorVisible(bool visible);
    void setTitle(const QString &title);
    void setIcon(const QPixmap &icon);
    Q_DECL_DEPRECATED void setWindowState(Qt::WindowState windowState);
    /// Maximized/Minumized
    void toggleWindowState();

private Q_SLOTS:
#ifndef QT_NO_MENU
    void showMenu();
#endif

protected:
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    D_DECLARE_PRIVATE(DTitlebar)
    D_PRIVATE_SLOT(void _q_toggleWindowState())
    D_PRIVATE_SLOT(void _q_showMinimized())

#ifndef QT_NO_MENU
    D_PRIVATE_SLOT(void _q_addDefaultMenuItems())
    D_PRIVATE_SLOT(void _q_helpActionTriggered())
    D_PRIVATE_SLOT(void _q_aboutActionTriggered())
    D_PRIVATE_SLOT(void _q_quitActionTriggered())
#endif
};

DWIDGET_END_NAMESPACE

#endif // DTITLEBAR_H
