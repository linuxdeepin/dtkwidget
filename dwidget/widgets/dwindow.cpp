#include "dwindow.h"

#include <QHBoxLayout>

DWIDGET_BEGIN_NAMESPACE

DWindow::DWindow(DWidget *parent): DWidget(parent)
{

}

void DWindow::setContentLayout(QLayout *l) {
    DWidget::setLayout(l);
}

void DWindow::setContentWidget(QWidget *w) {
    QHBoxLayout *l = new QHBoxLayout;
    l->setSpacing(0);
    l->setMargin(0);
    l->addWidget(w);
    l->setAlignment(w, Qt::AlignCenter);
    setContentLayout(l);
}

DWIDGET_END_NAMESPACE
