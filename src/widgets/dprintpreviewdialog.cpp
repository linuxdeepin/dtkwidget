#include "dprintpreviewdialog.h"
#include "private/dprintpreviewdialog_p.h"
#include "dframe.h"
#include "diconbutton.h"
#include "dlabel.h"
#include "dlineedit.h"
#include <DWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

DWIDGET_BEGIN_NAMESPACE

DPrintPreviewDialogPrivate::DPrintPreviewDialogPrivate(DPrintPreviewDialog *qq)
    : DDialogPrivate(qq)
{
}

void DPrintPreviewDialogPrivate::initui()
{
    Q_Q(DPrintPreviewDialog);
    QHBoxLayout *mainlayout = new QHBoxLayout();
    mainlayout->setContentsMargins(QMargins(0, 0, 0, 0));
    DFrame *pframe = new DFrame;
    pframe->setLayout(mainlayout);

    QVBoxLayout *pleftlayout = new QVBoxLayout;
    //pleftlayout->setSizeConstraint(QLayout::SetMaximumSize);
    initleft(pleftlayout);
    DVerticalLine *pvline = new DVerticalLine;
    QVBoxLayout *prightlayout = new QVBoxLayout;
    initright(prightlayout);
    mainlayout->addLayout(pleftlayout);
    mainlayout->addWidget(pvline);
    mainlayout->addLayout(prightlayout);
    q->addContent(pframe);
}

void DPrintPreviewDialogPrivate::initleft(QVBoxLayout *layout)
{
    pview = new DFrame;
    pview->setFixedSize(364, 470);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->addWidget(pview);
    layout->setAlignment(pview, Qt::AlignCenter);
    QHBoxLayout *pbottomlayout = new QHBoxLayout;
    layout->addLayout(pbottomlayout);
    firstBtn = new DIconButton(DStyle::SP_ArrowPrev);
    prevPageBtn = new DIconButton(DStyle::SP_ArrowLeft);
    jumpPageEdit = new DLineEdit();
    jumpPageEdit->setMaximumWidth(50);
    jumpPageEdit->setClearButtonEnabled(false);
    totalPageLabel = new DLabel("/123");
    nextPageBtn = new DIconButton(DStyle::SP_ArrowRight);
    lastBtn = new DIconButton(DStyle::SP_ArrowNext);
    pbottomlayout->addWidget(firstBtn);
    pbottomlayout->addWidget(prevPageBtn);
    pbottomlayout->addStretch();
    pbottomlayout->addWidget(jumpPageEdit);
    pbottomlayout->addWidget(totalPageLabel);
    pbottomlayout->addStretch();
    pbottomlayout->addWidget(nextPageBtn);
    pbottomlayout->addWidget(lastBtn);
}

void DPrintPreviewDialogPrivate::initright(QVBoxLayout *layout)
{
    QVBoxLayout *ptoplayout = new QVBoxLayout;
    DWidget *ptopwidget = new DWidget;
    ptopwidget->setFixedWidth(450);
    ptopwidget->setLayout(ptoplayout);
    QPushButton *pbtn = new QPushButton;
    pbtn->setFixedSize(100, 100);
    ptoplayout->addWidget(pbtn);

    //  QHBoxLayout

    QHBoxLayout *pbottomlayout = new QHBoxLayout;
    cancelBtn = new DPushButton(QObject::tr("Cancel"));
    printBtn = new DPushButton(QObject::tr("Print"));
    cancelBtn->setFixedWidth(170);
    printBtn->setFixedWidth(170);

    layout->addWidget(ptopwidget);
    layout->addLayout(pbottomlayout);
}

DPrintPreviewDialog::DPrintPreviewDialog(QWidget *parent)
    : DDialog(*new DPrintPreviewDialogPrivate(this), parent)
{
    Q_D(DPrintPreviewDialog);
    setFixedSize(851, 606);
    d->initui();
}

DWIDGET_END_NAMESPACE
