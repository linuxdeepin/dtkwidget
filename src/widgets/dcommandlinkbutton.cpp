#include "dcommandlinkbutton.h"
#include "dapplicationhelper.h"

#include <DStyleOption>
#include <DStylePainter>

DWIDGET_BEGIN_NAMESPACE

DCommandLinkButton::DCommandLinkButton(const QString text, QWidget *parent)
    : QAbstractButton(parent)
{
    this->setText(text);
}

QSize DCommandLinkButton::sizeHint() const
{
    QString text = this->text();
    QSize size = fontMetrics().size(0, text);

    return size;
}

void DCommandLinkButton::initStyleOption(DStyleOptionButton *option) const
{
    option->initFrom(this);

    if (isDown())
        option->state |= QStyle::State_Sunken;

    DPalette pa = DApplicationHelper::instance()->palette(this);
    option->palette.setBrush(DPalette::ButtonText, pa.highlight());
    option->text = this->text();
}

void DCommandLinkButton::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    DStyleOptionButton opt;
    initStyleOption(&opt);

    DStylePainter painter(this);
    painter.drawControl(DStyle::CE_TextButton, opt);
}

DWIDGET_END_NAMESPACE
