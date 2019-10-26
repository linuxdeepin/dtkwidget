#include "dwarningbutton.h"
#include "dapplicationhelper.h"

#include <QStyleOptionButton>
#include <QStylePainter>

DWIDGET_BEGIN_NAMESPACE

/*!
 * \~chinese \class DWarningButton
 * \~chinese \brief 警告按钮控件, 继承于 DPushButton 控件, 主要替换了文字和背景的颜色
 * \~chinese \note 通常在执行一些比较危险的动作时候, 给出的一个提醒标识, 但是并不强制阻止; 如: 某些小程序需要较高的权限才能够运行; 删除文件不可恢复等
 * \~chinese \see DPushButton
 */

/*!
 * \~chinese \brief 构造函数
 * \~chinese \param[in] parent 本控件的父对象
 */
DWarningButton::DWarningButton(QWidget *parent): DPushButton(parent)
{
}

/*!
 * \~chinese \brief 初始化风格属性
 * \~chinese \param[in] option 用来初始化本控件的基础的风格属性
 */
void DWarningButton::initStyleOption(QStyleOptionButton *option) const
{
    DPushButton::initStyleOption(option);
    DPalette pa = DApplicationHelper::instance()->palette(this);
    option->palette.setBrush(QPalette::ButtonText, pa.textWarning());
}

void DWarningButton::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    QStyleOptionButton opt;
    initStyleOption(&opt);
    QStylePainter painter(this);
    painter.drawControl(QStyle::CE_PushButton, opt);
}

DWIDGET_END_NAMESPACE
