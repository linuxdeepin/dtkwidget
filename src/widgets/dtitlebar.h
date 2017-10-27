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

#ifndef DTITLEBAR_H
#define DTITLEBAR_H

#include <QWidget>
#include <QMenu>

#include <dobject.h>
#include "dtkwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class DTitlebarPrivate;
class LIBDTKWIDGETSHARED_EXPORT DTitlebar : public QWidget, public DTK_CORE_NAMESPACE::DObject
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
    Q_DECL_DEPRECATED void setWindowFlags(Qt::WindowFlags type);

    int buttonAreaWidth() const;
    bool separatorVisible() const;

    void setVisible(bool visible) Q_DECL_OVERRIDE;
    void setEmbedMode(bool embed);

    void resize(int width, int height);
    void resize(const QSize &);

    void setDisableFlags(Qt::WindowFlags flags);
    Qt::WindowFlags disableFlags() const;
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
    void setIcon(const QIcon &icon);
    void Q_DECL_DEPRECATED setIcon(const QPixmap &icon);
    Q_DECL_DEPRECATED void setWindowState(Qt::WindowState windowState);
    /// Maximized/Minumized
    void toggleWindowState();

private Q_SLOTS:
#ifndef QT_NO_MENU
    void showMenu();
#endif

protected:
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    D_DECLARE_PRIVATE(DTitlebar)
    D_PRIVATE_SLOT(void _q_toggleWindowState())
    D_PRIVATE_SLOT(void _q_showMinimized())
    D_PRIVATE_SLOT(void _q_onTopWindowMotifHintsChanged(quint32))

#ifndef QT_NO_MENU
    D_PRIVATE_SLOT(void _q_addDefaultMenuItems())
    D_PRIVATE_SLOT(void _q_helpActionTriggered())
    D_PRIVATE_SLOT(void _q_aboutActionTriggered())
    D_PRIVATE_SLOT(void _q_quitActionTriggered())
#endif
};

DWIDGET_END_NAMESPACE

#endif // DTITLEBAR_H
