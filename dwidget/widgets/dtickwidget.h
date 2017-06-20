#ifndef DTICKWIDGET_H
#define DTICKWIDGET_H

#include "dwidget_global.h"
#include "dobject.h"

#include <QGraphicsEffect>

DWIDGET_BEGIN_NAMESPACE

class DTickWidgetPrivate;
class LIBDTKWIDGETSHARED_EXPORT DTickWidget : public QGraphicsEffect, public DObject
{
    Q_OBJECT
public:
    explicit DTickWidget(QWidget *widget, QWidget *parent = 0);

    enum Direction{
        LeftToRight,
        RightToLeft,
        TopToBottom,
        BottomToTop
    };

    void play();
    void stop();
    void pause();
    void resume();

    void setDirection(Direction direction);
    void setDuration(const int duration);

Q_SIGNALS:
    void finished();
    void stateChanged();

protected:
    void draw(QPainter *painter) Q_DECL_OVERRIDE;

private:
    D_DECLARE_PRIVATE(DTickWidget)
};

DWIDGET_END_NAMESPACE

#endif // DTICKWIDGET_H
