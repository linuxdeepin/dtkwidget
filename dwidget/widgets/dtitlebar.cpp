#include "dtitlebar.h"

#include <QHBoxLayout>
#include <QApplication>

#include <private/dobject_p.h>

#include "dwindowclosebutton.h"
#include "dwindowmaxbutton.h"
#include "dwindowminbutton.h"
#include "dwindowrestorebutton.h"
#include "dwindowoptionbutton.h"
#include "dlabel.h"

DWIDGET_BEGIN_NAMESPACE

class DTitlebarPrivate : public DObjectPrivate
{
protected:
    DTitlebarPrivate(DTitlebar *qq);

private:
    void init();

    QHBoxLayout      layout;
    DLabel           iconLabel;
    DLabel           titleLabel;
    DWindowMinButton minButton;
    DWindowMaxButton maxButton;
    DWindowRestoreButton restoreButton;
    DWindowCloseButton closeButton;
    DWindowOptionButton optionButton;

    Q_DECLARE_PUBLIC(DTitlebar)
};

DTitlebarPrivate::DTitlebarPrivate(DTitlebar *qq): DObjectPrivate(qq) {

}

void DTitlebarPrivate::init() {
    D_Q(DTitlebar);

    titleLabel.setText(qApp->applicationName());
    layout.addWidget(&iconLabel);
    layout.addWidget(&titleLabel);
    layout.addStretch();
    layout.addWidget(&optionButton);
    layout.addWidget(&minButton);
    layout.addWidget(&maxButton);
    layout.addWidget(&closeButton);

    q->setLayout(&layout);
}

DTitlebar::DTitlebar(QWidget *parent) :
    QWidget(parent),
    DObject(*new DTitlebarPrivate(this))
{
    d_func()->init();
}

void DTitlebar::setTitle(const QString& title) {
    D_D(DTitlebar);
    d->titleLabel.setText(title);
}

void DTitlebar::setIcon(const QPixmap& icon) {
    D_D(DTitlebar);
    d->iconLabel.setPixmap(icon);
}

DWIDGET_END_NAMESPACE
