#include "dwindow.h"

#include <QHBoxLayout>
#include <DMenu>
#include <DTitlebar>

#include <private/dwidget_p.h>

DWIDGET_BEGIN_NAMESPACE

class DWindowPrivate: public DWidgetPrivate
{
    D_DECLARE_PUBLIC(DWindow)

public:
    explicit DWindowPrivate(DWindow *q);
    DMenu   *dbusMenu;
};

DWindowPrivate::DWindowPrivate(DWindow *q): DWidgetPrivate(q)
{

}

DWindow::DWindow(DWidget *parent): DWidget(*(new DWindowPrivate(this)), parent)
{
    D_D(DWindow);
    d->dbusMenu = new DMenu;
    d->dbusMenu->attatch(this);
    setTitlebarMenu(d->dbusMenu);
}

DMenu * DWindow::dbusMenu()
{
    D_D(DWindow);
    return d->dbusMenu;
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
