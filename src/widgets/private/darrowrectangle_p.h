// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DARROWRECTANGLE_P_H
#define DARROWRECTANGLE_P_H

#include <DArrowRectangle>

#include <DWindowManagerHelper>
#include <DObjectPrivate>

#include <QPointer>

DGUI_USE_NAMESPACE
DWIDGET_BEGIN_NAMESPACE

class DArrowRectanglePrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
    D_DECLARE_PUBLIC(DArrowRectangle)

public:
    explicit DArrowRectanglePrivate(DArrowRectangle::ArrowDirection direction, DArrowRectangle *q);

    void init(DArrowRectangle::FloatMode floatMode);

    virtual void show(int x, int y);
    void setContent(QWidget *content);
    void resizeWithContent();
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *e);

    QPainterPath getLeftCornerPath();
    QPainterPath getRightCornerPath();
    QPainterPath getTopCornerPath();
    QPainterPath getBottomCornerPath();

    const QRect currentScreenRect(const int x, const int y);

    void verticalMove(int x, int y);
    void horizontalMove(int x, int y);

    void updateClipPath();

    bool radiusEnabled();

public:
    int m_radius = 3;
    int m_arrowHeight = 8;
    int m_arrowWidth = 12;
    int m_margin = 5;
    int m_arrowX = 0;
    int m_arrowY = 0;

    qreal m_shadowBlurRadius = 20;
    qreal m_shadowDistance = 0;
    qreal m_shadowXOffset = 0;
    qreal m_shadowYOffset = 5;
    int m_shadowWidth = 5; //Abandoned
    int m_borderWidth = 1;
    QColor m_borderColor = QColor(0, 0, 0, 255 * 0.1);
    QColor m_backgroundColor;

    DArrowRectangle::ArrowDirection m_arrowDirection;
    QPoint m_lastPos = QPoint(0, 0);

    DArrowRectangle::FloatMode floatMode = DArrowRectangle::FloatWindow;
    QPointer<QWidget> m_content;
    DPlatformWindowHandle *m_handle = nullptr;
    DBlurEffectWidget *m_blurBackground = nullptr;
    DWindowManagerHelper *m_wmHelper = nullptr;
    bool leftRightRadius = false;
    bool radiusArrowStyleEnable = false;
};

DWIDGET_END_NAMESPACE

#endif // DARROWRECTANGLE_P_H
