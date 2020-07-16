#ifndef DPRINTPREVIEWDIALOG_P_H
#define DPRINTPREVIEWDIALOG_P_H

#include "dprintpreviewdialog.h"
#include "ddialog_p.h"
class QVBoxLayout;
class QPushButton;
DWIDGET_BEGIN_NAMESPACE
class DFrame;
class DIconButton;
class DLineEdit;
class DLabel;
class DPrintPreviewDialogPrivate : public DDialogPrivate
{
public:
    explicit DPrintPreviewDialogPrivate(DPrintPreviewDialog *qq);
    void initui();
    void initleft(QVBoxLayout *layout);
    void initright(QVBoxLayout *layout);

    DFrame *pview;
    DIconButton *firstBtn;
    DIconButton *prevPageBtn;
    DLineEdit *jumpPageEdit;
    DLabel *totalPageLabel;
    DIconButton *nextPageBtn;
    DIconButton *lastBtn;
    QPushButton *cancelBtn;
    QPushButton *printBtn;
    Q_DECLARE_PUBLIC(DPrintPreviewDialog)
};

DWIDGET_END_NAMESPACE

#endif // DPRINTPREVIEWDIALOG_P_H
