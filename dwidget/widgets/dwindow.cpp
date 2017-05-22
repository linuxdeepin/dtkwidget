#include "dwindow.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QMenu>
#include <DApplication>

#include "dtitlebar.h"
#include "private/dwidget_p.h"

DWIDGET_BEGIN_NAMESPACE

class DWindowPrivate: public DWidgetPrivate
{
    D_DECLARE_PUBLIC(DWindow)

public:
    explicit DWindowPrivate(DWindow *q);
    void init();

    QMenu   *m_menu     = nullptr;
    QWidget *m_parent   = nullptr;
};

DWindowPrivate::DWindowPrivate(DWindow *q): DWidgetPrivate(q)
{

}

void DWindowPrivate::init()
{
    D_Q(DWindow);

    m_menu = new QMenu(q);
    q->setTitlebarMenu(m_menu);

    const DApplication *dapp = qobject_cast<DApplication*>(qApp);
    if (dapp) {
        q->setWindowTitle(dapp->productName());
    } else {
        q->setWindowTitle(qApp->applicationDisplayName());
    }
}

DWindow::DWindow(QWidget *parent): DWidget(*(new DWindowPrivate(this)))
{
    D_D(DWindow);

    setParent(parent);

    d->init();
}

void DWindow::setParent(QWidget *parent)
{
    D_D(DWindow);
    d->m_parent = parent;
    QObject::setParent(parent);
}

QWidget *DWindow::parentWidget() const
{
    D_DC(DWindow);
    return d->m_parent;
}

void DWindow::setContentLayout(QLayout *l)
{
    DWidget::setLayout(l);
}

void DWindow::setContentWidget(QWidget *w)
{
    QHBoxLayout *l = new QHBoxLayout;
    l->setSpacing(0);
    l->setMargin(0);
    l->addWidget(w);
    l->setAlignment(w, Qt::AlignCenter);
    setContentLayout(l);
}

DWIDGET_END_NAMESPACE
