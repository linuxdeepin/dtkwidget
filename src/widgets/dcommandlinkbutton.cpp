#include "dcommandlinkbutton.h"
#include "dpalettehelper.h"

#include <DStyleOption>
#include <DStylePainter>

DWIDGET_BEGIN_NAMESPACE

enum Margins{
    LeftMargins = 3,
    RightMargins = LeftMargins,
    TopLeftMargins = 3,
    BottomMargins = LeftMargins,
    TextMargins = 4
};

/*!
  \class Dtk::Widget::DCommandLinkButton
  \inmodule dtkwidget

  \brief DCommandLinkButton  一个继承于 QAbstractButton 的按钮,外形和链接很像;
  也可以是带有箭头的链接。常用于点击之后,跳转到另外一个窗口或者页面,比如浏览器的前进后退按钮
 */

/*!
  \brief 构造函数
  \a text 控件显示的文字
  \a parent 控件的父对象
 */
DCommandLinkButton::DCommandLinkButton(const QString text, QWidget *parent)
    : QAbstractButton(parent)
{
    this->setText(text);
}

/*!
  \brief 获取控件的矩形大小
  \return 返回本的控件矩形大小
 */
QSize DCommandLinkButton::sizeHint() const
{
    QString text = this->text();
    QSize size = fontMetrics().size(0, text);
    size += QSize(LeftMargins + RightMargins + TextMargins *2, TopLeftMargins + BottomMargins);
    return size;
}

/*!
  \brief 初始化的一个 option 的风格,和一些基本的属性
  \a option 实参是一个用来初始化的(按钮控件的)风格属性
 */
void DCommandLinkButton::initStyleOption(DStyleOptionButton *option) const
{
    option->initFrom(this);

    if (isDown())
        option->state |= QStyle::State_Sunken;

    DPalette pa = DPaletteHelper::instance()->palette(this);
    option->palette.setBrush(DPalette::ButtonText, pa.highlight());
    option->text = this->text();
}

/*!
  \brief 绘画事件
  \a e 此处不使用
 */
void DCommandLinkButton::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    DStyleOptionButton opt;
    initStyleOption(&opt);

    DStylePainter painter(this);
    painter.drawControl(DStyle::CE_TextButton, opt);
}

DWIDGET_END_NAMESPACE
