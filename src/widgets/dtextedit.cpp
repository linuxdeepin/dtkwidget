#include "dtextedit.h"

#include <QPainter>
#include <QEvent>

#include <DStyle>
#include <DObjectPrivate>

DWIDGET_BEGIN_NAMESPACE

class DTextEditPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate
{
public:
    DTextEditPrivate(DTextEdit* qq);
    ~DTextEditPrivate();

public:
    QWidget* widgetTop;
    QWidget* widgetButtom;
    QWidget* widgetLeft;
    QWidget* widgetRight;

public:
    D_DECLARE_PUBLIC(DTextEdit)
};

DTextEdit::DTextEdit(QWidget *parent)
    : QTextEdit(parent)
    , DObject(*new DTextEditPrivate(this))
{
    Q_D(DTextEdit);

    viewport()->setAutoFillBackground(false);
    setFrameShape(QFrame::NoFrame);

    d->widgetTop->setFixedWidth(1);
    d->widgetButtom->setFixedWidth(1);
    d->widgetLeft->setFixedHeight(1);
    d->widgetRight->setFixedHeight(1);

    addScrollBarWidget(d->widgetTop, Qt::AlignTop);
    addScrollBarWidget(d->widgetButtom, Qt::AlignBottom);
    addScrollBarWidget(d->widgetLeft, Qt::AlignLeft);
    addScrollBarWidget(d->widgetRight, Qt::AlignRight);
}

DTextEdit::DTextEdit(const QString &text, QWidget *parent)
    : DTextEdit(parent)
{
    setText(text);
}

bool DTextEdit::event(QEvent *e)
{
    Q_D(DTextEdit);

    if (e->type() == QEvent::Paint) {
        QPainter p(this);
        p.setRenderHints(QPainter::Antialiasing);

        QStyleOptionFrame panel;
        initStyleOption(&panel);
        style()->drawPrimitive(QStyle::PE_PanelLineEdit, &panel, &p, this);

        return true;
    } else if (e->type() == QEvent::Polish) {
        DStyleHelper dstyle(style());
        int frame_radius = dstyle.pixelMetric(DStyle::PM_FrameRadius, nullptr, this);
        setViewportMargins(frame_radius, 0, frame_radius, 0);

        d->widgetTop->setFixedHeight(frame_radius);
        d->widgetButtom->setFixedHeight(frame_radius);
        d->widgetLeft->setFixedWidth(frame_radius);
        d->widgetRight->setFixedWidth(frame_radius);
    }

    return QTextEdit::event(e);
}

DTextEditPrivate::DTextEditPrivate(DTextEdit *qq)
    : DObjectPrivate(qq)
{
    widgetTop = new QWidget(qq);
    widgetButtom = new QWidget(qq);
    widgetLeft = new QWidget(qq);
    widgetRight = new QWidget(qq);
}

DTextEditPrivate::~DTextEditPrivate()
{
}

DWIDGET_END_NAMESPACE
