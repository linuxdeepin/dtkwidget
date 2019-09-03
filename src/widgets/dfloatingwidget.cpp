#include "dfloatingwidget.h"
#include "private/dfloatingwidget_p.h"
#include "dstyleoption.h"
#include "dstyle.h"

#include <QHBoxLayout>
#include <QEvent>
#include <QDebug>

DWIDGET_BEGIN_NAMESPACE

//DFloatingWidgetPrivate 为 DFloatingWidget 的私有数据类
DFloatingWidgetPrivate::DFloatingWidgetPrivate(DFloatingWidget *qq)
    : DObjectPrivate(qq)
{
}

DFloatingWidgetPrivate::~DFloatingWidgetPrivate()
{
}

void DFloatingWidgetPrivate::init()
{
}

DFloatingWidget::DFloatingWidget(QWidget *parent)
    : QWidget(parent)
    , DObject(*new DFloatingWidgetPrivate(this))
{
    D_D(DFloatingWidget);
    d->init();
    setAutoFillBackground(false);
}

void DFloatingWidget::setWidget(QWidget *widget)
{
    D_D(DFloatingWidget);

    if (!d->layout) {
        d->layout = new QHBoxLayout(this);
        d->layout->setMargin(0);
    }

    d->layout->addWidget(widget);
}

QSize DFloatingWidget::sizeHint() const
{
    D_DC(DFloatingWidget);

    QSize size = d->layout ? d->layout->sizeHint() : QSize(0, 0);
    DStyleHelper dstyle(style());
    size = dstyle.sizeFromContents(DStyle::CT_FloatingWidget, nullptr, size, this);

    return size;
}

void DFloatingWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    DStylePainter painter(this);
    DStyleOptionFloatingWidget opt;
    initStyleOption(&opt);
    painter.drawControl(DStyle::CE_FloatingWidget, opt);
}

void DFloatingWidget::initStyleOption(DStyleOptionFloatingWidget *option) const
{
    option->init(this);
    option->initFrom(this);
}

DWIDGET_END_NAMESPACE
