// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DARROWBUTTON_H
#define DARROWBUTTON_H

#include <QWidget>
#include <QLabel>
#include <QStyle>
#include <QStateMachine>
#include <QState>
#include <QDebug>

#include <dtkwidget_global.h>
#include <dconstants.h>

DWIDGET_BEGIN_NAMESPACE

class ArrowButtonIcon : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(int arrowButtonDirection READ arrowDirection)
    Q_PROPERTY(int arrowButtonState READ buttonState)
public:
    explicit ArrowButtonIcon(QWidget *parent = 0);
    void setArrowDirection(int direction);
    void setButtonState(int state);
    int arrowDirection() const;
    int buttonState() const;

private:
    int m_direction;
    int m_buttonState;
};

class LIBDTKWIDGETSHARED_EXPORT DArrowButton : public QLabel
{
    Q_OBJECT
public:
    enum ArrowDirection {
        ArrowUp,
        ArrowDown,
        ArrowLeft,
        ArrowRight
    };

    enum ArrowButtonState {
        ArrowStateNormal,
        ArrowStateHover,
        ArrowStatePress
    };

    explicit DArrowButton(QWidget *parent = 0);
    void setArrowDirection(ArrowDirection direction);
    int arrowDirection() const;
    int buttonState() const;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

Q_SIGNALS:
    void mousePress();
    void mouseRelease();
    void mouseEnter();
    void mouseLeave();

private:
    void initButtonState();
    void setButtonState(ArrowButtonState state);
    void updateIconDirection(ArrowDirection direction);
    void updateIconState(ArrowButtonState state);

private:
    ArrowButtonIcon *m_normalLabel = NULL;
    ArrowButtonIcon *m_hoverLabel = NULL;
    ArrowButtonIcon *m_pressLabel = NULL;

    ArrowDirection m_arrowDirection = ArrowDown;
    ArrowButtonState m_buttonState = ArrowStateNormal;
};

DWIDGET_END_NAMESPACE

#endif // DARROWBUTTON_H
