#ifndef DARROWRECTANGLE_P_H
#define DARROWRECTANGLE_P_H

#include "darrowrectangle.h"

#include "private/dobject_p.h"

#include <QPointer>

DWIDGET_BEGIN_NAMESPACE

class DArrowRectanglePrivate : public DObjectPrivate
{
    D_DECLARE_PUBLIC(DArrowRectangle)

public:
    explicit DArrowRectanglePrivate(DArrowRectangle::ArrowDirection direction, DArrowRectangle *q);

    void init();

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

public:
    int m_radius = 3;
    int m_arrowHeight = 8;
    int m_arrowWidth = 12;
    int m_margin = 5;
    int m_arrowX = 0;
    int m_arrowY = 0;

    qreal m_shadowBlurRadius = 20;
    qreal m_shadowDistance = 1;
    qreal m_shadowXOffset = 0;
    qreal m_shadowYOffset = 5;
    int m_shadowWidth = 5; //Abandoned
    int m_borderWidth = 1;
    QColor m_borderColor = QColor(0, 0, 0, 255 * 0.1);
    QColor m_backgroundColor = QColor(255, 255, 255, 255);

    DArrowRectangle::ArrowDirection m_arrowDirection;
    QPoint m_lastPos = QPoint(0, 0);

    QPointer<QWidget> m_content;
    DPlatformWindowHandle *m_handle = NULL;
    DBlurEffectWidget *m_blurBackground = NULL;
};

DWIDGET_END_NAMESPACE

#endif // DARROWRECTANGLE_P_H
