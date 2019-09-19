#include "dwarningbutton.h"
#include "dapplicationhelper.h"

#include <QStyleOptionButton>
#include <QStylePainter>

DWIDGET_BEGIN_NAMESPACE

DWarningButton::DWarningButton(QWidget *parent): DPushButton(parent)
{
}

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
