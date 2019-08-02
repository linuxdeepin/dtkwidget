#include "dsuggestbutton.h"
#include "dstyleoption.h"

#include <QStylePainter>
#include <QStyleOptionButton>

DWIDGET_BEGIN_NAMESPACE

/*!
 * \~chinese \class DSuggestButton
 * \~chinese \brief 这个控件提供了DTK中默认的按钮样式.
 */

/*!
 * \~chinese \brief DSuggestButton的构造函数
 *
 * @param parent
 */
DSuggestButton::DSuggestButton(QWidget *parent)
    : QPushButton(parent)
{

}

/*!
 * \~chinese \brief DSuggestButton的重载构造函数，接收默认显示的文字
 *
 * @param text 默认显示的文字
 * @param parent
 */
DSuggestButton::DSuggestButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{

}


void DSuggestButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QStylePainter p(this);
    DStyleOptionButton option;
    option.features |= DStyleOptionButton::ButtonFeatures(DStyleOptionButton::SuggestButton);
    initStyleOption(&option);
    option.init(this);
    p.drawControl(QStyle::CE_PushButton, option);
}

DWIDGET_END_NAMESPACE
