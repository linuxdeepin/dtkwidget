#include "dtitlebar.h"

#include <QDebug>
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

const int DefaultTitlebarHeight = 36;
const int DefaultIconHeight = 20;
const int DefaultIconWidth = 20;

class DTitlebarPrivate : public DObjectPrivate
{
protected:
    DTitlebarPrivate(DTitlebar *qq);

private:
    void init();

    QHBoxLayout         *mainLayout;
    DLabel              *iconLabel;
    DLabel              *titleLabel;
    DWindowMinButton    *minButton;
    DWindowMaxButton    *maxButton;
    DWindowCloseButton  *closeButton;
    DWindowOptionButton *optionButton;

    QWidget             *coustomAtea;
    QWidget             *buttonArea;
    QWidget             *titleArea;
    QWidget             *titlePadding;
    Q_DECLARE_PUBLIC(DTitlebar)
};

DTitlebarPrivate::DTitlebarPrivate(DTitlebar *qq): DObjectPrivate(qq)
{
}

void DTitlebarPrivate::init()
{
    D_Q(DTitlebar);

    mainLayout      = new QHBoxLayout;
    iconLabel       = new DLabel;
    titleLabel      = new DLabel;
    minButton       = new DWindowMinButton;
    maxButton       = new DWindowMaxButton;
    closeButton     = new DWindowCloseButton;
    optionButton    = new DWindowOptionButton;
    coustomAtea     = new QWidget;
    buttonArea      = new QWidget;
    titleArea       = new QWidget;
    titlePadding    = new QWidget;

    mainLayout->setContentsMargins(20, 7, 20, 0);
    mainLayout->setSpacing(0);

    titleLabel->setText(qApp->applicationName());
    // TODO: use QSS
    titleLabel->setStyleSheet("font-size: 14px");

    QHBoxLayout *buttonAreaLayout = new QHBoxLayout;
    buttonAreaLayout->setMargin(0);
    buttonAreaLayout->setSpacing(0);
    buttonAreaLayout->addWidget(optionButton);
    buttonAreaLayout->addWidget(minButton);
    buttonAreaLayout->addWidget(maxButton);
    buttonAreaLayout->addWidget(closeButton);
    buttonArea->setLayout(buttonAreaLayout);
    buttonArea->adjustSize();

    QHBoxLayout *titleAreaLayout = new QHBoxLayout;
    titleAreaLayout->setMargin(0);
    titleAreaLayout->setSpacing(0);
    titlePadding->setFixedSize(buttonArea->size());
    titleAreaLayout->addWidget(titlePadding);
    titleAreaLayout->addStretch();
    titleAreaLayout->addWidget(iconLabel);
    titleAreaLayout->addSpacing(10);
    titleAreaLayout->addWidget(titleLabel);
    titleAreaLayout->addStretch();
    titleArea->setLayout(titleAreaLayout);

    QHBoxLayout *coustomAteaLayout = new QHBoxLayout;
    coustomAteaLayout->setMargin(0);
    coustomAteaLayout->setSpacing(0);
    coustomAteaLayout->addWidget(titleArea);
    coustomAtea->setLayout(coustomAteaLayout);

    mainLayout->addWidget(coustomAtea);
    mainLayout->addWidget(buttonArea);

    q->setLayout(mainLayout);
    q->setFixedHeight(DefaultTitlebarHeight);

    q->connect(optionButton, &DWindowOptionButton::clicked, q, &DTitlebar::optionClicked);
    q->connect(closeButton, &DWindowCloseButton::clicked, q, &DTitlebar::closeClicked);
    q->connect(maxButton, &DWindowMaxButton::maximum, q, &DTitlebar::maximumClicked);
    q->connect(maxButton, &DWindowMaxButton::restore, q, &DTitlebar::restoreClicked);
    q->connect(minButton, &DWindowMaxButton::clicked, q, &DTitlebar::minimumClicked);
}

DTitlebar::DTitlebar(QWidget *parent) :
    QWidget(parent),
    DObject(*new DTitlebarPrivate(this))
{
    d_func()->init();
}

///
/// \brief setWindowFlags
/// \param type
/// accpet  WindowTitleHint, WindowSystemMenuHint, WindowMinimizeButtonHint, WindowMaximizeButtonHint
/// and WindowMinMaxButtonsHint.
void DTitlebar::setWindowFlags(Qt::WindowFlags type)
{
    D_D(DTitlebar);
    d->titleLabel->setVisible(type & Qt::WindowTitleHint);
    d->iconLabel->setVisible(type & Qt::WindowTitleHint);
    d->minButton->setVisible(type & Qt::WindowMinimizeButtonHint);
    d->maxButton->setVisible(type & Qt::WindowMaximizeButtonHint);
    d->optionButton->setVisible(type & Qt::WindowSystemMenuHint);
    d->buttonArea->adjustSize();
    d->titlePadding->setFixedSize(d->buttonArea->size());
}

void DTitlebar::setFixedHeight(int h)
{
    QWidget::setFixedHeight(h);
}

void DTitlebar::setTitle(const QString &title)
{
    D_D(DTitlebar);
    d->titleLabel->setText(title);
}

void DTitlebar::setIcon(const QPixmap &icon)
{
    D_D(DTitlebar);
    d->iconLabel->setPixmap(icon.scaled(DefaultIconWidth, DefaultIconHeight, Qt::KeepAspectRatio));
}

DWIDGET_END_NAMESPACE
