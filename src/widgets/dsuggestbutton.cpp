#include "dsuggestbutton.h"
#include "dstyleoption.h"

#include <QStylePainter>
#include <QStyleOptionButton>

DWIDGET_BEGIN_NAMESPACE

DSuggestButton::DSuggestButton(QWidget *parent)
    : QPushButton(parent)
{

}

void DSuggestButton::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);

    QStylePainter p(this);
    DStyleOptionSuggestButton option;
    initStyleOption(&option);
    option.init(this);
    p.drawControl(QStyle::CE_PushButton, option);
}

DWIDGET_END_NAMESPACE
