#include "dcommandlinkbutton.h"
#include "dapplicationhelper.h"

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
 * \~chinese \class 一个继承于 QAbstractButton 的按钮,外形和链接很像;也可以是带有箭头的链接
 *                  常用于点击之后,跳转到另外一个窗口或者页面,比如浏览器的前进后退按钮
 */

/*!
 * \~chinese \brief 构造函数
 * \~chinese \param[in] text 控件显示的文字
 * \~chinese \param[in] parent 控件的父对象
 */
DCommandLinkButton::DCommandLinkButton(const QString text, QWidget *parent)
    : QAbstractButton(parent)
{
    this->setText(text);
}

/*!
 * \~chinese \brief 获取控件的矩形大小
 * \~chinese \return 返回本的控件矩形大小
 */
QSize DCommandLinkButton::sizeHint() const
{
    QString text = this->text();
    QSize size = fontMetrics().size(0, text);
    size += QSize(LeftMargins + RightMargins + TextMargins *2, TopLeftMargins + BottomMargins);
    return size;
}

/*!
 * \~chinese \brief 初始化的一个 option 的风格,和一些基本的属性
 * \~chinese \param [in] option 实参是一个用来初始化的(按钮控件的)风格属性
 */
void DCommandLinkButton::initStyleOption(DStyleOptionButton *option) const
{
    option->initFrom(this);

    if (isDown())
        option->state |= QStyle::State_Sunken;

    DPalette pa = DApplicationHelper::instance()->palette(this);
    option->palette.setBrush(DPalette::ButtonText, pa.highlight());
    option->text = this->text();
}

/*!
 * \~chinese \brief 绘画事件
 * \~chinese \param [in] e 此处不使用
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
