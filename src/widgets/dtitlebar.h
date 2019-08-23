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

#include <QFrame>
#include <QMenu>

#include <dobject.h>
#include "dtkwidget_global.h"

DWIDGET_BEGIN_NAMESPACE

class DTitlebarPrivate;
class LIBDTKWIDGETSHARED_EXPORT DTitlebar : public QFrame, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT
public:
    explicit DTitlebar(QWidget *parent = Q_NULLPTR);

#ifndef QT_NO_MENU
    QMenu *menu() const;
    void setMenu(QMenu *menu);
#endif

    D_DECL_DEPRECATED QWidget *customWidget() const;
    D_DECL_DEPRECATED_X("Use addWidget") void setCustomWidget(QWidget *, bool fixCenterPos = false);
    D_DECL_DEPRECATED_X("Use addWidget") void setCustomWidget(QWidget *, Qt::AlignmentFlag flag = Qt::AlignCenter, bool fixCenterPos = false);

    void addWidget(QWidget *w, Qt::Alignment alignment = Qt::AlignCenter);
    void removeWidget(QWidget *w);

    int buttonAreaWidth() const;
    bool separatorVisible() const;

    bool autoHideOnFullscreen() const;
    void setAutoHideOnFullscreen(bool autohide);

    void setVisible(bool visible) Q_DECL_OVERRIDE;
    void setEmbedMode(bool embed);

    bool menuIsVisible() const;
    void setMenuVisible(bool visible);

    bool menuIsDisabled() const;
    void setMenuDisabled(bool disabled);

    void setDisableFlags(Qt::WindowFlags flags);
    Qt::WindowFlags disableFlags() const;

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

Q_SIGNALS:
    void optionClicked();
    void doubleClicked();
    void mousePressed(Qt::MouseButtons buttons);
    void mouseMoving(Qt::MouseButton button);

#ifdef DTK_TITLE_DRAG_WINDOW
    void mousePosPressed(Qt::MouseButtons buttons, QPoint pos);
    void mousePosMoving(Qt::MouseButton button, QPoint pos);
#endif

public Q_SLOTS:
    void setFixedHeight(int h);
    void setBackgroundTransparent(bool transparent);
    void setSeparatorVisible(bool visible);
    void setTitle(const QString &title);
    void setIcon(const QIcon &icon);
    /// Maximized/Minumized
    void toggleWindowState();

private Q_SLOTS:
#ifndef QT_NO_MENU
    void showMenu();
#endif

protected:
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;
    bool event(QEvent *e) override;
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
